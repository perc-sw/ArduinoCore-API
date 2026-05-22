/*
  HardwareI2C.h - Hardware I2C interface for Arduino
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

#ifndef ARDUINO_CORE_API_HARDWARE_I2C_H
#define ARDUINO_CORE_API_HARDWARE_I2C_H

#include <cstdint>
#include "Stream.h"

namespace arduino {

  class HardwareI2C : public Stream {
    public:
      virtual void begin() = 0;
      virtual void begin(std::uint8_t address) = 0;
      virtual void end() = 0;

      virtual void setClock(std::uint32_t freq) = 0;

      virtual void beginTransmission(std::uint8_t address) = 0;
      virtual std::uint8_t endTransmission(bool stopBit) = 0;
      virtual std::uint8_t endTransmission() = 0;

      virtual std::size_t requestFrom(std::uint8_t address, std::size_t len, bool stopBit) = 0;
      virtual std::size_t requestFrom(std::uint8_t address, std::size_t len) = 0;

      virtual void onReceive(void(*)(int)) = 0;
      virtual void onRequest(void(*)()) = 0;
  };

}

#endif
