/*
  This file is part of the Arduino_AdvancedAnalog library.
  Copyright (c) 2023-2024 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ARDUINO_CORE_API_DMA_POOL_H
#define ARDUINO_CORE_API_DMA_POOL_H

#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace arduino {

#if defined(__DCACHE_PRESENT)
  #define __CACHE_LINE_SIZE__ __SCB_DCACHE_LINE_SIZE
#elif defined(__cpp_lib_hardware_interference_size)
  #define __CACHE_LINE_SIZE__ std::hardware_constructive_interference_size
#else // No cache.
  #define __CACHE_LINE_SIZE__ alignof(int)
#endif

  // Single-producer, single-consumer, lock-free bounded Queue.
  template <class T>
  class SPSCQueue {
    private:
      size_t               capacity;
      std::atomic<size_t>  head;
      std::atomic<size_t>  tail;
      std::unique_ptr<T[]> buff;

    public:
      explicit SPSCQueue(const size_t size = 0) : capacity(0), head(0), tail(0), buff(nullptr) {
        if (size) {
          if (T* mem = new T[size + 1]) {
            buff.reset(mem);
            capacity = size + 1;
          }
        }
      }

      void reset() {
        tail = head = 0;
      }

      std::size_t empty() const {
        return tail == head;
      }

      explicit operator bool() const {
        return buff.get() != nullptr;
      }

      bool push(T data) {
        std::size_t       curr = head.load(std::memory_order_relaxed);
        const std::size_t next = (curr + 1) % capacity;
        if (!buff || (next == tail.load(std::memory_order_acquire))) {
          return false;
        }

        buff[curr] = data;
        head.store(next, std::memory_order_release);

        return true;
      }

      T pop(const bool peek = false) {
        std::size_t curr = tail.load(std::memory_order_relaxed);

        if (!buff || (curr == head.load(std::memory_order_acquire))) {
          return nullptr;
        }

        T data = buff[curr];

        if (!peek) {
          const std::size_t next = (curr + 1) % capacity;
          tail.store(next, std::memory_order_release);
        }
        return data;
      }
  };

  enum DMABufferFlags : std::uint32_t {
    DMA_BUFFER_READ    = (1 << 0),
    DMA_BUFFER_WRITE   = (1 << 1),
    DMA_BUFFER_DISCONT = (1 << 2),
    DMA_BUFFER_INTRLVD = (1 << 3),
  };

  // Forward declaration of DMAPool class.
  template <class, std::size_t>
  class DMAPool;

  template <class T, size_t A = __CACHE_LINE_SIZE__>
  class DMABuffer {
    private:
      DMAPool<T, A>* pool;
      std::size_t    n_samples;
      std::size_t    n_channels;
      T*             ptr;
      std::uint32_t  ts;
      std::uint32_t  flags;

    public:
      explicit DMABuffer(DMAPool<T, A>* pool = nullptr, const std::size_t samples = 0, const std::size_t channels = 0,
                         T* mem = nullptr) :
          pool(pool), n_samples(samples), n_channels(channels), ptr(mem), ts(0), flags(0) {
      }

      T* data() {
        return ptr;
      }

      std::size_t size() const {
        return n_samples * n_channels;
      }

      std::size_t bytes() const {
        return n_samples * n_channels * sizeof(T);
      }

      void flush() {
#if __DCACHE_PRESENT
        if (ptr) {
          SCB_CleanDCache_by_Addr(data(), bytes());
        }
#endif
      }

      void invalidate() {
#if __DCACHE_PRESENT
        if (ptr) {
          SCB_InvalidateDCache_by_Addr(data(), bytes());
        }
#endif
      }

      std::uint32_t timestamp() const {
        return ts;
      }

      void timestamp(const std::uint32_t time_stamp) {
        this->ts = time_stamp;
      }

      std::uint32_t channels() const {
        return n_channels;
      }

      void release() {
        if (pool && ptr) {
          pool->free(this, flags);
        }
      }

      void set_flags(const std::uint32_t f) {
        flags |= f;
      }

      bool get_flags(const std::uint32_t f = 0xFFFFFFFFU) const {
        return flags & f;
      }

      void clr_flags(const std::uint32_t f = 0xFFFFFFFFU) {
        flags &= (~f);
      }

      T& operator[](size_t i) {
        assert(ptr && i < size());
        return ptr[i];
      }

      const T& operator[](size_t i) const {
        assert(ptr && i < size());
        return ptr[i];
      }

      explicit operator bool() const {
        return (ptr != nullptr);
      }
  };

  template <class T, size_t A = __CACHE_LINE_SIZE__>
  class DMAPool {
    private:
      std::uint8_t*            mem;
      bool                     managed;
      SPSCQueue<DMABuffer<T>*> wqueue;
      SPSCQueue<DMABuffer<T>*> rqueue;

      // Allocates dynamic aligned memory.
      // Note this memory must be freed with aligned_free.
      static void* aligned_malloc(const std::size_t size) {
        void*  stashed;
        const std::size_t offset = A - 1 + sizeof(void*);
        if ((A % 2) || !((stashed = std::malloc(size + offset)))) {
          return nullptr;
        }
        const auto ptr = reinterpret_cast<void**>((reinterpret_cast<uintptr_t>(stashed) + offset) & ~(A - 1));
        ptr[-1] = stashed;
        return ptr;
      }

      // Frees dynamic aligned memory allocated with aligned_malloc.
      static void aligned_free(void* ptr) {
        if (ptr != nullptr) {
          std::free(static_cast<void**>(ptr)[-1]);
        }
      }

    public:
      DMAPool(std::size_t n_samples, std::size_t n_channels, std::size_t n_buffers, void* mem_in = nullptr) :
          mem(static_cast<uint8_t*>(mem_in)), managed(mem_in == nullptr), wqueue(n_buffers), rqueue(n_buffers) {
        // Round up to the next multiple of the alignment.
        const std::size_t bufsize = (((n_samples * n_channels * sizeof(T)) + (A - 1)) & ~(A - 1));
        if (bufsize && rqueue && wqueue) {
          if (mem == nullptr) {
            // Allocate an aligned memory block for the DMA buffers' memory.
            mem = static_cast<uint8_t*>(aligned_malloc(n_buffers * bufsize));
            if (!mem) {
              // Failed to allocate memory.
              return;
            }
          }
          // Allocate the DMA buffers, initialize them using aligned
          // pointers from the pool, and add them to the write queue.
          for (size_t i = 0; i < n_buffers; i++) {
            DMABuffer<T>* buf = new DMABuffer<T>(this, n_samples, n_channels, (T*)&mem[i * bufsize]);
            if (buf == nullptr) {
              break;
            }
            wqueue.push(buf);
          }
        }
      }

      ~DMAPool() {
        while (readable()) {
          delete alloc(DMA_BUFFER_READ);
        }

        while (writable()) {
          delete alloc(DMA_BUFFER_WRITE);
        }

        if (mem && managed) {
          aligned_free(mem);
        }
      }

      bool writable() {
        return !(wqueue.empty());
      }

      bool readable() {
        return !(rqueue.empty());
      }

      void flush() {
        while (readable()) {
          if (DMABuffer<T>* buf = alloc(DMA_BUFFER_READ)) {
            buf->release();
          }
        }
      }

      DMABuffer<T>* alloc(std::uint32_t flags) {
        DMABuffer<T>* buf = nullptr;
        if (flags & DMA_BUFFER_READ) {
          // Get a DMA buffer from the read/ready queue.
          buf = rqueue.pop();
        } else {
          // Get a DMA buffer from the write/free queue.
          buf = wqueue.pop();
        }
        if (buf) {
          buf->clr_flags(DMA_BUFFER_READ | DMA_BUFFER_WRITE);
          buf->set_flags(flags);
        }
        return buf;
      }

      void free(DMABuffer<T>* buf, std::uint32_t flags = 0) {
        if (buf == nullptr) {
          return;
        }
        if (flags == 0) {
          flags = buf->get_flags();
        }
        if (flags & DMA_BUFFER_READ) {
          // Return the DMA buffer to the write/free queue.
          buf->clr_flags();
          wqueue.push(buf);
        } else {
          // Return the DMA buffer to the read/ready queue.
          rqueue.push(buf);
        }
      }
  };

} // namespace arduino

using arduino::DMABuffer;
using arduino::DMAPool;
using arduino::SPSCQueue;
#endif // ARDUINO_CORE_API_DMA_POOL_H
