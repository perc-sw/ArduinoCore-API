/*
  Common.h - Common definitions for Arduino core
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

#ifndef ARDUINO_CORE_API_COMMON_H
#define ARDUINO_CORE_API_COMMON_H

#include <algorithm>
#include <cstdint>
#include<limits>
#include <type_traits>
#include "compiler_features.h"

void yield();

enum PinStatus : std::uint8_t {
  LOW     = 0,
  HIGH    = 1,
  CHANGE  = 2,
  FALLING = 3,
  RISING  = 4,
};

enum PinMode : std::uint8_t{
  INPUT                  = 0x0,
  INPUT_PULLUP           = 0x1,
  INPUT_PULLDOWN         = 0x2,
  INPUT_SCHMITT          = 0x3,
  INPUT_SCHMITT_PULLUP   = 0x4,
  INPUT_SCHMITT_PULLDOWN = 0x5,
  OUTPUT                 = 0x6,
  OUTPUT_2mA             = 0x7,
  OUTPUT_4mA             = 0x8,
  OUTPUT_8mA             = 0x9,
  OUTPUT_12mA            = 0xA
} ;

enum BitOrder : std::uint8_t {
  LSBFIRST = 0,
  MSBFIRST = 1,
};

#if HAS_MATH_CONSTANT_LIB()
  #include <numbers>
  template<typename T = double>
  static CONSTEXPR auto PI           { std::numbers::pi_v<T> };

  template<typename T = double>
  static CONSTEXPR auto HALF_PI      { PI<T> / static_cast<T>(2.0) };

  template<typename T = double>
  static CONSTEXPR auto TWO_PI       { PI<T> * static_cast<T>(2.0) };

  template<typename T = double>
  static CONSTEXPR auto DEG_TO_RAD   { PI<T> / static_cast<T>(180.0) };

  template<typename T = double>
  static CONSTEXPR auto RAD_TO_DEG   { static_cast<T>(180.0) / PI<T> };

  template<typename T = double>
  static CONSTEXPR auto EULER        { std::numbers::e_v<T> };
#else
  static constexpr double PI          = 3.1415926535897932384626433832795;
  static constexpr double HALF_PI     = 1.5707963267948966192313216916398;
  static constexpr double TWO_PI      = 6.283185307179586476925286766559;
  static constexpr double DEG_TO_RAD  = 0.017453292519943295769236907684886;
  static constexpr double RAD_TO_DEG  = 57.295779513082320876798154814105;
  static constexpr double EULER       = 2.718281828459045235360287471352;
#endif

static constexpr std::uint32_t SERIAL { 0x0ul };
static constexpr std::uint32_t DISPLAY { 0x1ul };

#if REQUIRED_CPP_STANDARD(CXX_STANDARD_20)
  constexpr auto constrain(const auto value, const auto low, const auto high) -> decltype(auto){
    return std::clamp(value, low, high);
  };
#else
  template<typename T>
  constexpr auto constrain(const T value, const T low, const T high) -> T {
    return std::clamp(value, low, high);
  }
#endif

#if REQUIRED_CPP_STANDARD(CXX_STANDARD_20)
  constexpr auto radians(const auto deg) -> decltype(auto) {
    using value_type = std::remove_cvref_t<decltype(deg)>;
    return deg * DEG_TO_RAD<value_type>;
  }
#else
  template<typename T>
  constexpr auto radians(const T deg) -> T {
    return deg * static_cast<T>(DEG_TO_RAD);
  }
#endif

#if REQUIRED_CPP_STANDARD(CXX_STANDARD_20)
  constexpr auto degrees(const auto rad) -> decltype(auto) {
    using value_type = std::remove_cvref_t<decltype(rad)>;
    return rad * RAD_TO_DEG<value_type>;
  }
#else
  template<typename T>
  constexpr auto degrees(const T rad) -> T {
    return rad * static_cast<T>(RAD_TO_DEG);
  }
#endif

#if REQUIRED_CPP_STANDARD(CXX_STANDARD_20)
  constexpr auto sq(const auto value) -> decltype(auto) {
    return value * value;
  }
#else
  template<typename T>
  constexpr auto sq(const T value) -> T {
    return value * value;
  }
#endif

using voidFuncPtr = void (*)();
using voidFuncPtrParam = void (*)(void*);

// interrupts() / noInterrupts() must be defined by the core

static constexpr std::uint8_t lowByte(const std::uint16_t w) { return w & 0xff; }
static constexpr std::uint8_t highByte(const std::uint16_t w) { return w >> 8; }

#if REQUIRED_CPP_STANDARD(CXX_STANDARD_20)
static constexpr auto bitRead(const auto value, const std::size_t bit) -> bool {
  using value_type = std::remove_cvref_t<decltype(value)>;
  CONSTEXPR auto limit = std::numeric_limits<value_type>::digits;
  if (bit >= limit) {
    return static_cast<value_type>(0);
  } else {
    return ((value >> bit) & static_cast<value_type>(0x01)) > 0;
  }
}
static constexpr void bitSet(const auto& value, const std::size_t bit) {
  using value_type = std::remove_cvref_t<decltype(value)>;
  CONSTEXPR auto limit = std::numeric_limits<value_type>::digits;

  if (bit >= limit) {
    return;
  }

  value |= (static_cast<value_type>(0b1) << bit);
}
static constexpr void bitClear(const auto& value, const std::size_t bit) {
  using value_type = std::remove_cvref_t<decltype(value)>;
  CONSTEXPR auto limit = std::numeric_limits<value_type>::digits;

  if (bit >= limit) {
    return;
  }

  value &= ~(static_cast<value_type>(0b1) << bit);
}
static constexpr void bitToggle(const auto& value, const std::size_t bit) {
  using value_type = std::remove_cvref_t<decltype(value)>;
  CONSTEXPR auto limit = std::numeric_limits<value_type>::digits;

  if (bit >= limit) {
    return;
  }

  value ^= (static_cast<value_type>(0b1) << bit);
}
static constexpr void bitWrite(const auto& value, const std::size_t bit, const bool bitset) {
  using value_type = std::remove_cvref_t<decltype(value)>;
  if (bitset) {
    bitSet(value, bit);
  } else {
    bitClear(value, bit);
  }
}
#else

template<typename T>
static constexpr auto bitRead(const T value, const std::size_t bit) -> bool {
  using value_type = std::remove_reference_t<std::remove_cv_t<T>>;
  constexpr auto limit = std::numeric_limits<value_type>::digits;
  if (bit >= limit) {
    return static_cast<value_type>(0);
  } else {
    return ((value >> bit) & static_cast<value_type>(0x01)) > 0;
  }
}

template<typename T>
static constexpr void bitSet(const T& value, const std::size_t bit) {
  using value_type = std::remove_reference_t<std::remove_cv_t<decltype(value)>>;
  constexpr auto limit = std::numeric_limits<value_type>::digits;

  if (bit >= limit) {
    return;
  }

  value |= (static_cast<value_type>(0b1) << bit);
}

template<typename T>
static constexpr void bitClear(const T& value, const std::size_t bit) {
  using value_type = std::remove_reference_t<std::remove_cv_t<decltype(value)>>;
  constexpr auto limit = std::numeric_limits<value_type>::digits;

  if (bit >= limit) {
    return;
  }

  value &= ~(static_cast<value_type>(0b1) << bit);
}

template<typename T>
static constexpr void bitToggle(const T& value, const std::size_t bit) {
  using value_type = std::remove_reference_t<std::remove_cv_t<decltype(value)>>;
  constexpr auto limit = std::numeric_limits<value_type>::digits;

  if (bit >= limit) {
    return;
  }

  value ^= (static_cast<value_type>(0b1) << bit);
}

template<typename T>
static constexpr void bitWrite(const T& value, const std::size_t bit, const bool bitset) {
  if (bitset) {
    bitSet(value, bit);
  } else {
    bitClear(value, bit);
  }
}

#endif

template<typename T = std::uint32_t>
static constexpr auto bit(const std::size_t b) -> T {
  return static_cast<T>(0b1) << b;
}

using byte = std::uint8_t;

void init();
void initVariant();

#ifndef HOST
int atexit(void (*)()) __attribute__((weak));
#endif

[[noreturn]] int main() __attribute__((weak));

#ifdef EXTENDED_PIN_MODE
// Platforms who want to declare more than 256 pins need to define EXTENDED_PIN_MODE globally
using pin_size_t = std::uint32_t;
#else
using pin_size_t = std::uint8_t;
#endif

void pinMode(pin_size_t pinNumber, PinMode pinMode);
void digitalWrite(pin_size_t pinNumber, PinStatus status);
PinStatus digitalRead(pin_size_t pinNumber);
int analogRead(pin_size_t pinNumber);
void analogReference(uint8_t mode);
void analogWrite(pin_size_t pinNumber, int value);

std::uint32_t millis();
std::uint64_t micros();
void delay(std::uint32_t ms);
void delayMicroseconds(std::uint64_t us);
std::uint32_t pulseIn(pin_size_t pin, uint8_t state, std::uint32_t timeout  = 1000000L);
std::uint64_t pulseInLong(pin_size_t pin, uint8_t state, std::uint64_t timeout  = 1000000L);

void shiftOut(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder, uint8_t val);
std::uint8_t shiftIn(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder);

void attachInterrupt(pin_size_t interruptNumber, voidFuncPtr callback, PinStatus mode);
void attachInterruptParam(pin_size_t interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param);
void detachInterrupt(pin_size_t interruptNumber);

void setup();
[[noreturn]] void loop();

template<typename T>
constexpr auto min(const T& a, const T& b){ return std::min(a, b); };

template<typename T>
constexpr auto max(const T& a, const T& b){ return std::max(a, b); };

/* C++ prototypes */
uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

template<typename ... Ts>
std::uint16_t word(const Ts ... values) {
  static_assert(sizeof...(values) <= 2, "word() takes at most 2 arguments");
  return makeWord(values...);
}

void tone(uint8_t _pin, std::uint32_t frequency, std::uint32_t duration = 0);
void noTone(uint8_t _pin);

// WMath prototypes
std::int32_t random(std::int32_t);
std::int32_t random(std::int32_t, std::int32_t);
void randomSeed(std::uint32_t);
std::int32_t map(std::int32_t, std::int32_t, std::int32_t, std::int32_t, std::int32_t);

#endif
