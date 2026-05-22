/*
  Print.h - Base class that provides print() and println()
  Copyright (c) 2016 Arduino LLC.  All right reserved.

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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef ARDUINO_CORE_API_PRINT_H
#define ARDUINO_CORE_API_PRINT_H

#include <cstdint>
#include <cstdio>

#include "String.h"
#include "Printable.h"

static constexpr std::uint8_t DEC { 10 };
static constexpr std::uint8_t HEX { 16 };
static constexpr std::uint8_t OCT { 8 };
static constexpr std::uint8_t BIN { 2 };

namespace arduino {

  class Print {
      int write_error;
      std::size_t printNumber(std::uint32_t, std::uint8_t);
      std::size_t printULLNumber(std::uint64_t, std::uint8_t);
      std::size_t printFloat(double, int);
    protected:
      void setWriteError(const int err = 1) { write_error = err; }
    public:
      Print() : write_error(0) {}
      virtual ~Print() = default;

      [[nodiscard]] int getWriteError() const { return write_error; }
      void clearWriteError() { setWriteError(0); }

      virtual std::size_t write(std::uint8_t) = 0;
      std::size_t write(const char *str) {
      if (str == nullptr) return 0;
      return write(reinterpret_cast<const uint8_t *>(str), strlen(str));
    }

      virtual std::size_t write(const std::uint8_t *buffer, std::size_t size);
      std::size_t write(const char *buffer, const std::size_t size) {
      return write(reinterpret_cast<const std::uint8_t *>(buffer), size);
    }

      // default to zero, meaning "a single write may block"
      // should be overridden by subclasses with buffering
      virtual int availableForWrite() { return 0; }

      std::size_t print(const __FlashStringHelper *);
      std::size_t print(const String &);
      std::size_t print(const char[]);
      std::size_t print(char);
      std::size_t print(std::uint8_t, int = DEC);
      std::size_t print(int, int = DEC);
      std::size_t print(unsigned int, int = DEC);
      std::size_t print(std::int32_t, int = DEC);
      std::size_t print(std::uint32_t, int = DEC);
      std::size_t print(std::int64_t, int = DEC);
      std::size_t print(std::uint64_t, int = DEC);
      std::size_t print(double, int = 2);
      std::size_t print(const Printable&);

      std::size_t println(const __FlashStringHelper *);
      std::size_t println(const String &s);
      std::size_t println(const char[]);
      std::size_t println(char);
      std::size_t println(std::uint8_t, int = DEC);
      std::size_t println(int, int = DEC);
      std::size_t println(unsigned int, int = DEC);
      std::size_t println(std::int32_t, int = DEC);
      std::size_t println(std::uint32_t, int = DEC);
      std::size_t println(std::int64_t, int = DEC);
      std::size_t println(std::uint64_t, int = DEC);
      std::size_t println(double, int = 2);
      std::size_t println(const Printable&);
      std::size_t println();

      virtual void flush() { /* Empty implementation for backward compatibility */ }
  };

}

using arduino::Print;

#endif
