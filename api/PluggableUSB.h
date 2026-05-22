/*
  PluggableUSB.h
  Copyright (c) 2015 Arduino LLC

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

#ifndef ARDUINO_CORE_API_PLUGGABLE_USB_H
#define ARDUINO_CORE_API_PLUGGABLE_USB_H

#include <cstddef>
#include <cstdint>
#include "USBAPI.h"

namespace arduino {

  class PluggableUSBModule {
    public:
      PluggableUSBModule(const std::uint8_t numEps, const std::uint8_t numIfs, const unsigned int* epType) :
          numEndpoints(numEps), numInterfaces(numIfs), endpointType(epType) {
      }
      virtual ~PluggableUSBModule() = default;

    protected:
      virtual bool    setup(USBSetup& setup)                = 0;
      virtual int     getInterface(uint8_t* interfaceCount) = 0;
      virtual int     getDescriptor(USBSetup& setup)        = 0;
      virtual uint8_t getShortName(char* name) {
        name[0] = 'A' + pluggedInterface;
        return 1;
      }

      std::uint8_t pluggedInterface { 0 };
      std::uint8_t pluggedEndpoint { 0 };

      const uint8_t       numEndpoints;
      const uint8_t       numInterfaces;
      const unsigned int* endpointType;

      PluggableUSBModule* next = nullptr;

      friend class PluggableUSB_;
  };

  class PluggableUSB_ {
    public:
      PluggableUSB_();
      bool plug(PluggableUSBModule* node);
      int  getInterface(uint8_t* interfaceCount) const;
      int  getDescriptor(USBSetup& setup) const;
      bool setup(USBSetup& setup) const;
      void getShortName(char* iSerialNum) const;

    private:
      uint8_t             lastIf;
      uint8_t             lastEp;
      PluggableUSBModule* rootNode { nullptr };
      uint8_t             totalEP;
  };
} // namespace arduino

// core need to define
void* epBuffer(unsigned int n); // -> returns a pointer to the Nth element of the EP buffer structure

// Replacement for global singleton.
// This function prevents static-initialization-order-fiasco
// https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
arduino::PluggableUSB_& PluggableUSB();

#endif // ARDUINO_CORE_API_PLUGGABLE_USB_H
