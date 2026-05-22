/*
  CanMsgRingbuffer.h - Library for CAN message handling
  Copyright (c) 2023 Arduino. All right reserved.

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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef ARDUINO_CORE_API_CAN_MSG_RING_BUFFER_H
#define ARDUINO_CORE_API_CAN_MSG_RING_BUFFER_H

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <cstdint>
#include "CanMsg.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino {

  /**************************************************************************************
   * CLASS DECLARATION
   **************************************************************************************/

  class CanMsgRingbuffer {
    public:
      static std::size_t constexpr RING_BUFFER_SIZE = 32U;

      CanMsgRingbuffer();

      [[nodiscard]] bool isFull() const {
        return (_num_elems == RING_BUFFER_SIZE);
      }
      void enqueue(CanMsg const& msg);

      [[nodiscard]] bool isEmpty() const {
        return (_num_elems == 0);
      }
      CanMsg dequeue();

      [[nodiscard]] std::size_t available() const {
        return _num_elems;
      }

    private:
      CanMsg               _buf[RING_BUFFER_SIZE];
      volatile std::size_t _head;
      volatile std::size_t _tail;
      volatile std::size_t _num_elems;

      static std::size_t next(size_t const idx) {
        return ((idx + 1) % RING_BUFFER_SIZE);
      }
  };

  /**************************************************************************************
   * NAMESPACE
   **************************************************************************************/

} // namespace arduino

#endif /* ARDUINO_CORE_API_CAN_MSG_RING_BUFFER_H */
