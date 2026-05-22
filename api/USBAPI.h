/*
  USBAPI.h
  Copyright (c) 2005-2014 Arduino.  All right reserved.

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

#ifndef ARDUINO_CORE_API_USBAPI_H
#define ARDUINO_CORE_API_USBAPI_H

#include <cstdint>

namespace arduino {
  //================================================================================
  //================================================================================
  //  Low level API

  typedef struct __attribute__((packed)) {
      union {
          std::uint8_t bmRequestType;
          struct {
              std::uint8_t direction : 5;
              std::uint8_t type : 2;
              std::uint8_t transferDirection : 1;
          };
      };
      std::uint8_t  bRequest;
      std::uint8_t  wValueL;
      std::uint8_t  wValueH;
      std::uint16_t wIndex;
      std::uint16_t wLength;
  } USBSetup;

} // namespace arduino

//================================================================================
// USB APIs (C scope)
//================================================================================

int USB_SendControl(std::uint8_t flags, const void* d, int len);
int USB_RecvControl(void* d, int len);
int USB_RecvControlLong(void* d, int len);

uint8_t USB_Available(std::uint8_t ep);
uint8_t USB_SendSpace(std::uint8_t ep);
int     USB_Send(std::uint8_t ep, const void* data, int len); // blocking
int     USB_Recv(std::uint8_t ep, void* data, int len);       // non-blocking
int     USB_Recv(std::uint8_t ep);                            // non-blocking
void    USB_Flush(std::uint8_t ep);

#endif // ARDUINO_CORE_API_USBAPI_H
