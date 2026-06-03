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
  enum class SerialParity : std::uint32_t {
    EVEN  = 0x1ul,
    ODD   = 0x2ul,
    NONE  = 0x3ul,
    MASK  = 0xFul
  };

  enum class SerialStop : std::uint32_t {
    BIT_1   = 0x00ul,
    BIT_2   = 0x10ul,
    MASK    = 0xF0ul
  };

  enum class SerialData : std::uint32_t {
    BITS_5 = 0x000ul,
    BITS_6 = 0x100ul,
    BITS_7 = 0x200ul,
    BITS_8 = 0x300ul,
    MASK   = 0xF00ul
  };

  static constexpr std::uint32_t SERIAL_5N1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::NONE) | static_cast<std::uint32_t>(SerialData::BITS_5) };
  static constexpr std::uint32_t SERIAL_6N1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::NONE) | static_cast<std::uint32_t>(SerialData::BITS_6) };
  static constexpr std::uint32_t SERIAL_7N1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::NONE) | static_cast<std::uint32_t>(SerialData::BITS_7) };
  static constexpr std::uint32_t SERIAL_8N1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::NONE) | static_cast<std::uint32_t>(SerialData::BITS_8) };

  static constexpr std::uint32_t SERIAL_5N2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::NONE) | static_cast<std::uint32_t>(SerialData::BITS_5) };
  static constexpr std::uint32_t SERIAL_6N2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::NONE) | static_cast<std::uint32_t>(SerialData::BITS_6) };
  static constexpr std::uint32_t SERIAL_7N2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::NONE) | static_cast<std::uint32_t>(SerialData::BITS_7) };
  static constexpr std::uint32_t SERIAL_8N2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::NONE) | static_cast<std::uint32_t>(SerialData::BITS_8) };

  static constexpr std::uint32_t SERIAL_5E1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::EVEN) | static_cast<std::uint32_t>(SerialData::BITS_5) };
  static constexpr std::uint32_t SERIAL_6E1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::EVEN) | static_cast<std::uint32_t>(SerialData::BITS_6) };
  static constexpr std::uint32_t SERIAL_7E1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::EVEN) | static_cast<std::uint32_t>(SerialData::BITS_7) };
  static constexpr std::uint32_t SERIAL_8E1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::EVEN) | static_cast<std::uint32_t>(SerialData::BITS_8) };

  static constexpr std::uint32_t SERIAL_5E2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::EVEN) | static_cast<std::uint32_t>(SerialData::BITS_5) };
  static constexpr std::uint32_t SERIAL_6E2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::EVEN) | static_cast<std::uint32_t>(SerialData::BITS_6) };
  static constexpr std::uint32_t SERIAL_7E2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::EVEN) | static_cast<std::uint32_t>(SerialData::BITS_7) };
  static constexpr std::uint32_t SERIAL_8E2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::EVEN) | static_cast<std::uint32_t>(SerialData::BITS_8) };

  static constexpr std::uint32_t SERIAL_5O1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::ODD) | static_cast<std::uint32_t>(SerialData::BITS_5) };
  static constexpr std::uint32_t SERIAL_6O1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::ODD) | static_cast<std::uint32_t>(SerialData::BITS_6) };
  static constexpr std::uint32_t SERIAL_7O1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::ODD) | static_cast<std::uint32_t>(SerialData::BITS_7) };
  static constexpr std::uint32_t SERIAL_8O1 { static_cast<std::uint32_t>(SerialStop::BIT_1) | static_cast<std::uint32_t>(SerialParity::ODD) | static_cast<std::uint32_t>(SerialData::BITS_8) };

  static constexpr std::uint32_t SERIAL_5O2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::ODD) | static_cast<std::uint32_t>(SerialData::BITS_5) };
  static constexpr std::uint32_t SERIAL_6O2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::ODD) | static_cast<std::uint32_t>(SerialData::BITS_6) };
  static constexpr std::uint32_t SERIAL_7O2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::ODD) | static_cast<std::uint32_t>(SerialData::BITS_7) };
  static constexpr std::uint32_t SERIAL_8O2 { static_cast<std::uint32_t>(SerialStop::BIT_2) | static_cast<std::uint32_t>(SerialParity::ODD) | static_cast<std::uint32_t>(SerialData::BITS_8) };

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
