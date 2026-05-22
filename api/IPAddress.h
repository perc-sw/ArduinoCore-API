/*
  IPAddress.h - Base class that provides IPAddress
  Copyright (c) 2011 Adrian McEwen.  All right reserved.

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

#ifndef ARDUINO_CORE_API_IPADDRESS_H
#define ARDUINO_CORE_API_IPADDRESS_H

#include <cstdint>
#include "Printable.h"
#include "String.h"

#define IPADDRESS_V4_BYTES_INDEX 12
#define IPADDRESS_V4_DWORD_INDEX 3

// forward declarations of global name space friend classes
class EthernetClass;
class DhcpClass;
class DNSClient;

namespace arduino {

  // A class to make it easier to handle and pass around IP addresses

  enum IPType { IPv4, IPv6 };

  class IPAddress : public Printable {
    private:
      union {
          uint8_t  bytes[16];
          uint32_t dword[4];
      } _address;
      IPType _type;

      // Access the raw byte array containing the address.  Because this returns a pointer
      // to the internal structure rather than a copy of the address this function should only
      // be used when you know that the usage of the returned uint8_t* will be transient and not
      // stored.
      uint8_t* raw_address() {
        return _type == IPv4 ? &_address.bytes[IPADDRESS_V4_BYTES_INDEX] : _address.bytes;
      }

    public:
      // Constructors

      // Default IPv4
      IPAddress();
      explicit IPAddress(IPType);
      IPAddress(std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t);
      IPAddress(std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t,
                std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t);

      // IPv4; see implementation note
      explicit IPAddress(std::uint32_t);

      // Default IPv4
      explicit IPAddress(const std::uint8_t*);
      IPAddress(IPType, const std::uint8_t*);

      // If IPv4 fails tries IPv6 see fromString function
      explicit IPAddress(const char*);

      bool fromString(const char*);
      bool fromString(const String& address) {
        return fromString(address.c_str());
      }

      // Overloaded cast operator to allow IPAddress objects to be used where a uint32_t is expected
      // NOTE: IPv4 only; see implementation note
      explicit operator std::uint32_t() const {
        return _type == IPv4 ? _address.dword[IPADDRESS_V4_DWORD_INDEX] : 0;
      };

      bool operator==(const IPAddress&) const;
      bool operator!=(const IPAddress& addr) const {
        return !(*this == addr);
      };

      // NOTE: IPv4 only; we don't know the length of the pointer
      bool operator==(const std::uint8_t*) const;

      // Overloaded index operator to allow getting and setting individual octets of the address
      std::uint8_t  operator[](int) const;
      std::uint8_t& operator[](int);

      // Overloaded copy operators to allow initialization of IPAddress objects from other types
      // NOTE: IPv4 only
      IPAddress& operator=(const std::uint8_t*);
      // NOTE: IPv4 only; see implementation note
      IPAddress& operator=(std::uint32_t);
      // If IPv4 fails tries IPv6 see fromString function
      IPAddress& operator=(const char*);

      virtual std::size_t printTo(Print& p) const;
      [[nodiscard]] String         toString() const;

      [[nodiscard]] IPType type() const {
        return _type;
      }

      friend class UDP;
      friend class Client;
      friend class Server;

      friend ::EthernetClass;
      friend ::DhcpClass;
      friend ::DNSClient;

    protected:
      bool   fromString4(const char* address);
      bool   fromString6(const char* address);
      [[nodiscard]] String toString4() const;
      [[nodiscard]] String toString6() const;
  };

  extern const IPAddress IN6ADDR_ANY;
  extern const IPAddress INADDR_NONE;
} // namespace arduino

using arduino::IPAddress;

#endif // ARDUINO_CORE_API_IPADDRESS_H