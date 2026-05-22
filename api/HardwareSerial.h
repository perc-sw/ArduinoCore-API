/*
  HardwareSerial.h - Hardware serial interface for Arduino
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
#ifndef ARDUINO_CORE_API_HARDWARE_SERIAL_H
#define ARDUINO_CORE_API_HARDWARE_SERIAL_H

#include <cstdint>
#include "Stream.h"

namespace arduino {

  // XXX: Those constants should be defined as const int / enums?
  // XXX: shall we use namespaces too?
  enum class SerialParity : std::uint32_t { EVEN, ODD, NONE, MARK, SPACE };

  enum class SerialStop : std::uint32_t { BIT_1, BIT_1_5, BIT_2 };

  enum class SerialData : std::uint32_t { BITS_5, BITS_6, BITS_7, BITS_8 };

  class HardwareSerial : public Stream {
    public:
      virtual void   begin(std::uint32_t)                = 0;
      virtual void   begin(std::uint32_t, std::uint16_t) = 0;
      virtual void   end()                               = 0;
      virtual int    available()                         = 0;
      virtual int    peek()                              = 0;
      virtual int    read()                              = 0;
      virtual void   flush()                             = 0;
      virtual size_t write(std::uint8_t)                 = 0;
      using Print::write; // pull in write(str) and write(buf, size) from Print
      virtual operator bool() = 0;
  };

  // XXX: Are we keeping the serialEvent API?
  extern void serialEventRun() __attribute__((weak));

} // namespace arduino

#endif // ARDUINO_CORE_API_HARDWARE_SERIAL_H
