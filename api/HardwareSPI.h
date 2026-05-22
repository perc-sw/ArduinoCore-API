/*
  HardwareSPI.h - Hardware SPI interface for Arduino
  Copyright (c) 2018 Arduino LLC.  All right reserved.

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

#ifndef ARDUINO_CORE_API_HARDWARE_SPI_H
#define ARDUINO_CORE_API_HARDWARE_SPI_H

#include "Common.h"
#include "Stream.h"

#define SPI_HAS_TRANSACTION

namespace arduino {

  enum SPIMode : std::uint32_t {
    SPI_MODE0 = 0,
    SPI_MODE1 = 1,
    SPI_MODE2 = 2,
    SPI_MODE3 = 3,
  };

  // Platforms should define SPI_HAS_PERIPHERAL_MODE if SPI peripheral
  // mode is supported, to allow applications to check whether peripheral
  // mode is available or not.
  enum SPIBusMode : std::uint32_t {
    SPI_CONTROLLER = 0,
    SPI_PERIPHERAL = 1,
  };

  class SPISettings {
    public:
      SPISettings(const std::uint32_t clock, const BitOrder bitOrder, const SPIMode dataMode,
                  const SPIBusMode busMode = SPI_CONTROLLER) {
        if (__builtin_constant_p(clock)) {
          init_AlwaysInline(clock, bitOrder, dataMode, busMode);
        } else {
          init_MightInline(clock, bitOrder, dataMode, busMode);
        }
      }

      SPISettings(const std::uint32_t clock, const BitOrder bitOrder, const int dataMode,
                  const SPIBusMode busMode = SPI_CONTROLLER) {
        if (__builtin_constant_p(clock)) {
          init_AlwaysInline(clock, bitOrder, static_cast<SPIMode>(dataMode), busMode);
        } else {
          init_MightInline(clock, bitOrder, static_cast<SPIMode>(dataMode), busMode);
        }
      }

      // Default speed set to 4MHz, SPI mode set to MODE 0 and Bit order set to MSB first.
      SPISettings() {
        init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0, SPI_CONTROLLER);
      }

      bool operator==(const SPISettings& rhs) const {
        if ((this->_clockFreq == rhs._clockFreq) && (this->_bitOrder == rhs._bitOrder) &&
            (this->_dataMode == rhs._dataMode) && (this->_busMode == rhs._busMode)) {
          return true;
        }
        return false;
      }

      bool operator!=(const SPISettings& rhs) const {
        return !(*this == rhs);
      }

      std::uint32_t getClockFreq() const {
        return _clockFreq;
      }

      SPIMode getDataMode() const {
        return _dataMode;
      }

      BitOrder getBitOrder() const {
        return (_bitOrder);
      }

      SPIBusMode getBusMode() const {
        return _busMode;
      }

    private:
      void init_MightInline(const std::uint32_t clock, const BitOrder bitOrder, const SPIMode dataMode,
                            const SPIBusMode busMode) {
        init_AlwaysInline(clock, bitOrder, dataMode, busMode);
      }

      // Core developer MUST use an helper function in beginTransaction() to use this data
      void init_AlwaysInline(const std::uint32_t clock, const BitOrder bitOrder, const SPIMode dataMode,
                             const SPIBusMode busMode) __attribute__((__always_inline__)) {
        this->_clockFreq = clock;
        this->_dataMode  = dataMode;
        this->_bitOrder  = bitOrder;
        this->_busMode   = busMode;
      }

      uint32_t   _clockFreq;
      SPIMode    _dataMode;
      BitOrder   _bitOrder;
      SPIBusMode _busMode;

      friend class HardwareSPI;
  };

  const auto DEFAULT_SPI_SETTINGS = SPISettings();

  class HardwareSPI {
    public:
      virtual ~HardwareSPI() = default;

      virtual std::uint8_t  transfer(std::uint8_t data)            = 0;
      virtual std::uint16_t transfer16(std::uint16_t data)         = 0;
      virtual void          transfer(void* buf, std::size_t count) = 0;

      // Transaction Functions
      virtual void usingInterrupt(int interruptNumber)    = 0;
      virtual void notUsingInterrupt(int interruptNumber) = 0;
      virtual void beginTransaction(SPISettings settings) = 0;
      virtual void endTransaction()                       = 0;

      // SPI Configuration methods
      virtual void attachInterrupt() = 0;
      virtual void detachInterrupt() = 0;

      virtual void begin() = 0;
      virtual void end()   = 0;
  };

  // Alias SPIClass to HardwareSPI since it's already the defacto standard for SPI class name
  using SPIClass = HardwareSPI;
} // namespace arduino

#endif
