/*
  Common.cpp - Common function implementations
  Copyright (c) 2017 Arduino LLC. All right reserved.

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
#include "Common.h"

/* C++ prototypes */
std::int32_t map(const std::int32_t x, const std::int32_t in_min, const std::int32_t in_max,
                 const std::int32_t out_min, const std::int32_t out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

std::uint16_t makeWord(const std::uint16_t w) { return w; }
std::uint16_t makeWord(const byte h, const byte l) { return (h << 8) | l; }
