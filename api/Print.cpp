/*
  Print.cpp - Base class that provides print() and println()
  Copyright (c) 2014 Arduino.  All right reserved.

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

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "Print.h"

using namespace arduino;

// Public Methods //////////////////////////////////////////////////////////////

/* default implementation: may be overridden */
std::size_t Print::write(const std::uint8_t *buffer, std::size_t size) {
  size_t n = 0;

  while (size--) {
    if (write(*buffer++)) {
      n++;
    }
    else {
      break;
    }
  }
  return n;
}

std::size_t Print::print(const __FlashStringHelper *ifsh) {
  return print(reinterpret_cast<const char *>(ifsh));
}

std::size_t Print::print(const String &s) {
  return write(s.c_str(), s.length());
}

std::size_t Print::print(const char str[]) {
  return write(str);
}

std::size_t Print::print(const char c){
  return write(c);
}

std::size_t Print::print(const unsigned char b, const int base) {
  return print(static_cast<unsigned long>(b), base);
}

std::size_t Print::print(const int n, const int base) {
  return print(static_cast<long>(n), base);
}

std::size_t Print::print(const unsigned int n, const int base) {
  return print(static_cast<unsigned long>(n), base);
}

std::size_t Print::print(std::int32_t n, const int base)
{
  if (base == 0) {
    return write(n);
  } else if (base == 10) {
    if (n < 0) {
      const auto t = static_cast<int>(print('-'));
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  } else {
    return printNumber(n, base);
  }
}

std::size_t Print::print(const std::uint32_t n, const int base) {
  if (base == 0) return write(n);
  else return printNumber(n, base);
}

std::size_t Print::print(std::int64_t n, const int base) {
  if (base == 0) {
    return write(n);
  } else if (base == 10) {
    if (n < 0) {
      const auto t = static_cast<int>(print('-'));
      n = -n;
      return printULLNumber(n, 10) + t;
    }
    return printULLNumber(n, 10);
  } else {
    return printULLNumber(n, base);
  }
}

std::size_t Print::print(const std::uint64_t n, const int base) {
  if (base == 0) return write(n);
  else return printULLNumber(n, base);
}

std::size_t Print::print(const double n, const int digits) {
  return printFloat(n, digits);
}

std::size_t Print::println(const __FlashStringHelper *ifsh) {
  size_t n = print(ifsh);
  n += println();
  return n;
}

std::size_t Print::print(const Printable& x) {
  return x.printTo(*this);
}

std::size_t Print::println() {
  return write("\r\n");
}

std::size_t Print::println(const String &s) {
  size_t n = print(s);
  n += println();
  return n;
}

std::size_t Print::println(const char c[]) {
  size_t n = print(c);
  n += println();
  return n;
}

std::size_t Print::println(const char c) {
  size_t n = print(c);
  n += println();
  return n;
}

std::size_t Print::println(const unsigned char b, const int base) {
  size_t n = print(b, base);
  n += println();
  return n;
}

std::size_t Print::println(const int num, const int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}

std::size_t Print::println(const unsigned int num, const int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}

std::size_t Print::println(const std::int32_t num, const int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}

std::size_t Print::println(const std::uint32_t num, const int base) {
  auto n = print(num, base);
  n += println();
  return n;
}

std::size_t Print::println(const std::int64_t num, const int base) {
  auto n = print(num, base);
  n += println();
  return n;
}

std::size_t Print::println(const std::uint64_t num, const int base) {
  auto n = print(num, base);
  n += println();
  return n;
}

std::size_t Print::println(const double num, const int digits) {
  auto n = print(num, digits);
  n += println();
  return n;
}

std::size_t Print::println(const Printable& x) {
  auto n = print(x);
  n += println();
  return n;
}

// Private Methods /////////////////////////////////////////////////////////////

std::size_t Print::printNumber(std::uint32_t n, std::uint8_t base) {
  char buf[8 * sizeof(std::uint32_t) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) {
    base = 10;
  }

  do {
    const char c = n % base;
    n /= base;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return write(str);
}

// REFERENCE IMPLEMENTATION FOR ULL
// size_t Print::printULLNumber(unsigned long long n, uint8_t base)
// {
  // // if limited to base 10 and 16 the bufsize can be smaller
  // char buf[65];
  // char *str = &buf[64];

  // *str = '\0';

  // // prevent crash if called with base == 1
  // if (base < 2) base = 10;

    // do {
      // unsigned long long t = n / base;
      // char c = n - t * base;  // faster than c = n%base;
      // n = t;
      // *--str = c < 10 ? c + '0' : c + 'A' - 10;
  // } while(n);

  // return write(str);
// }

// FAST IMPLEMENTATION FOR ULL
std::size_t Print::printULLNumber(std::uint64_t n64, std::uint8_t base) {
  // if limited to base 10 and 16 the bufsize can be 20
  char buf[64];
  uint8_t i = 0;
  uint8_t innerLoops = 0;

  // Special case workaround https://github.com/arduino/ArduinoCore-API/issues/178
  if (n64 == 0) {
    write('0');
    return 1;
  }

  // prevent crash if called with base == 1
  if (base < 2) {
    base = 10;
  }

  // process chunks that fit in "16 bit math".
  const std::uint16_t top = 0xFFFF / base;
  std::uint16_t th16 = 1;
  while (th16 < top) {
    th16 *= base;
    innerLoops++;
  }

  while (n64 > th16) {
    // 64 bit math part
    const uint64_t q = n64 / th16;
    uint16_t r = n64 - q*th16;
    n64 = q;

    // 16 bit math loop to do remainder. (note buffer is filled reverse)
    for (uint8_t j=0; j < innerLoops; j++) {
      const uint16_t qq = r/base;
      buf[i++] = r - qq*base;
      r = qq;
    }
  }

  std::uint16_t n16 = n64;
  while (n16 > 0) {
    const uint16_t qq = n16/base;
    buf[i++] = n16 - qq*base;
    n16 = qq;
  }

  const std::size_t bytes = i;
  for (; i > 0; i--) {
    write(static_cast<char>(buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
  }
  return bytes;
}

std::size_t Print::printFloat(double number, int digits) {
  if (digits < 0) {
    digits = 2;
  }

  size_t n = 0;

  if (std::isnan(number)) {
    return print("nan");
  }
  if (std::isinf(number)) {
    return print("inf");
  }
  if (number > std::numeric_limits<double>::max()) {
    return print ("ovf");
  }  // constant determined empirically
  if (number < std::numeric_limits<double>::max()) {
    return print ("ovf");
  }  // constant determined empirically

  // Handle negative numbers
  if (number < 0.0) {
     n += print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (std::uint8_t i = 0; static_cast<int>(i) < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  const auto int_part = static_cast<std::uint32_t>(number);
  auto remainder = number - static_cast<double>(int_part);
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += print(".");
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    const auto toPrint = static_cast<unsigned int>(remainder);
    n += print(toPrint);
    remainder -= toPrint;
  }

  return n;
}
