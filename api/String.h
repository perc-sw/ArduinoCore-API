/*
  String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

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

#ifndef ARDUINO_CORE_API_ARDUINO_STRINGS_H
#define ARDUINO_CORE_API_ARDUINO_STRINGS_H

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#if defined(__AVR__)
  #include "avr/pgmspace.h"
#else
  #include "deprecated-avr-comp/avr/pgmspace.h"
#endif

namespace arduino {

  // When compiling programs with this class, the following gcc parameters
  // dramatically increase performance and memory (RAM) efficiency, typically
  // with little or no increase in code size.
  //     -felide-constructors
  //     -std=c++0x

  class __FlashStringHelper;

#define F(string_literal) (reinterpret_cast<const __FlashStringHelper*>(PSTR(string_literal)))

  // An inherited class for holding the result of a concatenation.  These
  // result objects are assumed to be writable by subsequent concatenations.
  class StringSumHelper;

  // The string class
  class String {
      friend class StringSumHelper;
      // use a function pointer to allow for "if (s)" without the
      // complications of an operator bool(). for more information, see:
      // http://www.artima.com/cppsource/safebool.html
      using StringIfHelper_t = void (String::*)();
      // typedef void (String::*StringIfHelperType)() const;
      void StringIfHelper() { }

      static constexpr std::size_t FLT_MAX_DECIMAL_PLACES = 10;
      static constexpr std::size_t DBL_MAX_DECIMAL_PLACES = FLT_MAX_DECIMAL_PLACES;

    public:
      // constructors
      // creates a copy of the initial value.
      // if the initial value is null or invalid, or if memory allocation
      // fails, the string will be marked as invalid (i.e. "if (s)" will
      // be false).
      explicit String(const char* cstr = "");
      String(const char*, std::size_t);
      String(const uint8_t* cstr, const std::size_t length) : String(reinterpret_cast<const char*>(cstr), length) {
      }
      String(const String&);
      explicit String(const __FlashStringHelper*);
      String(String&&) noexcept;
      explicit String(char);
      explicit String(unsigned char, std::uint8_t = 10);
      explicit String(int, std::uint8_t = 10);
      explicit String(unsigned int, std::uint8_t = 10);
      explicit String(std::int32_t, std::uint8_t = 10);
      explicit String(std::uint32_t, std::uint8_t = 10);
      explicit String(float, std::uint8_t = 2);
      explicit String(double, std::uint8_t = 2);
      ~String();

      // memory management
      // return true on success, false on failure (in which case, the string
      // is left unchanged).  reserve(0), if successful, will validate an
      // invalid string (i.e., "if (s)" will be true afterwards)
      bool                       reserve(std::size_t);
      [[nodiscard]] unsigned int length() const {
        return len;
      }
      [[nodiscard]] bool isEmpty() const {
        return length() == 0;
      }

      // creates a copy of the assigned value.  if the value is null or
      // invalid, or if the memory allocation fails, the string will be
      // marked as invalid ("if (s)" will be false).
      String& operator=(const String&);
      String& operator=(const char*);
      String& operator=(const __FlashStringHelper*);
      String& operator=(String&&) noexcept;

      // concatenate (works w/ built-in types)

      // returns true on success, false on failure (in which case, the string
      // is left unchanged).  if the argument is null or invalid, the
      // concatenation is considered unsuccessful.
      bool concat(const String&);
      bool concat(const char*);
      bool concat(const char*, std::size_t);
      bool concat(const uint8_t* cstr, const std::size_t length) {
        return concat(reinterpret_cast<const char*>(cstr), length);
      }
      bool concat(char);
      bool concat(std::uint8_t);
      bool concat(int);
      bool concat(unsigned int);
      bool concat(std::int32_t);
      bool concat(std::uint32_t);
      bool concat(float);
      bool concat(double);
      bool concat(const __FlashStringHelper*);

      // if there's not enough memory for the concatenated value, the string
      // will be left unchanged (but this isn't signalled in any way)
      String& operator+=(const String& rhs) {
        concat(rhs);
        return (*this);
      }
      String& operator+=(const char* cstr) {
        concat(cstr);
        return (*this);
      }
      String& operator+=(const char c) {
        concat(c);
        return (*this);
      }
      String& operator+=(const unsigned char num) {
        concat(num);
        return (*this);
      }
      String& operator+=(const int num) {
        concat(num);
        return (*this);
      }
      String& operator+=(const unsigned int num) {
        concat(num);
        return (*this);
      }
      String& operator+=(const long num) {
        concat(num);
        return (*this);
      }
      String& operator+=(const unsigned long num) {
        concat(num);
        return (*this);
      }
      String& operator+=(const float num) {
        concat(num);
        return (*this);
      }
      String& operator+=(const double num) {
        concat(num);
        return (*this);
      }
      String& operator+=(const __FlashStringHelper* str) {
        concat(str);
        return (*this);
      }

      friend StringSumHelper& operator+(const StringSumHelper&, const String&);
      friend StringSumHelper& operator+(const StringSumHelper&, const char*);
      friend StringSumHelper& operator+(const StringSumHelper&, char);
      friend StringSumHelper& operator+(const StringSumHelper&, unsigned char);
      friend StringSumHelper& operator+(const StringSumHelper&, int);
      friend StringSumHelper& operator+(const StringSumHelper&, unsigned int);
      friend StringSumHelper& operator+(const StringSumHelper&, long);
      friend StringSumHelper& operator+(const StringSumHelper&, unsigned long);
      friend StringSumHelper& operator+(const StringSumHelper&, float);
      friend StringSumHelper& operator+(const StringSumHelper&, double);
      friend StringSumHelper& operator+(const StringSumHelper&, const __FlashStringHelper*);

      // comparison (only works w/ Strings and "strings")
      explicit operator StringIfHelper_t() const {
        return buffer != nullptr ? &String::StringIfHelper : nullptr;
      }
      [[nodiscard]] int  compareTo(const String&) const;
      int                compareTo(const char* cstr) const;
      [[nodiscard]] bool equals(const String&) const;
      bool               equals(const char* cstr) const;

      friend bool operator==(const String& a, const String& b) {
        return a.equals(b);
      }
      friend bool operator==(const String& a, const char* b) {
        return a.equals(b);
      }
      friend bool operator==(const char* a, const String& b) {
        return b == a;
      }
      friend bool operator<(const String& a, const String& b) {
        return a.compareTo(b) < 0;
      }
      friend bool operator<(const String& a, const char* b) {
        return a.compareTo(b) < 0;
      }
      friend bool operator<(const char* a, const String& b) {
        return b.compareTo(a) > 0;
      }

      friend bool operator!=(const String& a, const String& b) {
        return !(a == b);
      }
      friend bool operator!=(const String& a, const char* b) {
        return !(a == b);
      }
      friend bool operator!=(const char* a, const String& b) {
        return !(a == b);
      }
      friend bool operator>(const String& a, const String& b) {
        return b < a;
      }
      friend bool operator>(const String& a, const char* b) {
        return b < a;
      }
      friend bool operator>(const char* a, const String& b) {
        return b < a;
      }
      friend bool operator<=(const String& a, const String& b) {
        return b >= a;
      }
      friend bool operator<=(const String& a, const char* b) {
        return b >= a;
      }
      friend bool operator<=(const char* a, const String& b) {
        return b >= a;
      }
      friend bool operator>=(const String& a, const String& b) {
        return !(a < b);
      }
      friend bool operator>=(const String& a, const char* b) {
        return !(a < b);
      }
      friend bool operator>=(const char* a, const String& b) {
        return !(a < b);
      }

      [[nodiscard]] bool equalsIgnoreCase(const String&) const;
      [[nodiscard]] bool startsWith(const String&) const;
      [[nodiscard]] bool startsWith(const String&, unsigned int offset) const;
      [[nodiscard]] bool endsWith(const String&) const;

      // character access
      [[nodiscard]] char charAt(std::size_t) const;
      void               setCharAt(std::size_t, char) const;
      char               operator[](std::size_t) const;
      char&              operator[](std::size_t);
      void               getBytes(unsigned char*, std::size_t, std::size_t = 0) const;

      void toCharArray(char* buf, const std::size_t buf_size, const std::size_t index = 0) const {
        getBytes(reinterpret_cast<unsigned char*>(buf), buf_size, index);
      }

      [[nodiscard]] const char* c_str() const {
        return buffer;
      }
      char* begin() {
        return buffer;
      }
      char* end() {
        return buffer + length();
      }
      [[nodiscard]] const char* begin() const {
        return c_str();
      }
      [[nodiscard]] const char* end() const {
        return c_str() + length();
      }

      // search
      [[nodiscard]] int    indexOf(char) const;
      [[nodiscard]] int    indexOf(char, std::size_t) const;
      [[nodiscard]] int    indexOf(const String&) const;
      [[nodiscard]] int    indexOf(const String&, std::size_t) const;
      [[nodiscard]] int    lastIndexOf(char) const;
      [[nodiscard]] int    lastIndexOf(char, std::size_t) const;
      [[nodiscard]] int    lastIndexOf(const String&) const;
      [[nodiscard]] int    lastIndexOf(const String&, std::size_t) const;
      [[nodiscard]] String substring(const std::size_t beginIndex) const {
        return substring(beginIndex, len);
      };
      [[nodiscard]] String substring(std::size_t, std::size_t) const;

      // modification
      void replace(char, char) const;
      void replace(const String&, const String&);
      void remove(std::size_t);
      void remove(std::size_t, std::size_t);
      void toLowerCase() const;
      void toUpperCase() const;
      void trim();

      // parsing/conversion
      [[nodiscard]] long   toInt() const;
      [[nodiscard]] float  toFloat() const;
      [[nodiscard]] double toDouble() const;

    protected:
      char*       buffer{};   // the actual char array
      std::size_t capacity{}; // the array length minus one (for the '\0')
      std::size_t len{};      // the String length (not counting the '\0')
    protected:
      void init();
      void invalidate();
      bool changeBuffer(unsigned int maxStrLen);

      // copy and move
      String& copy(const char* cstr, std::size_t length);
      String& copy(const __FlashStringHelper* pstr, unsigned int length);
      void    move(String& rhs);
  };

  class StringSumHelper : public String {
    public:
      explicit StringSumHelper(const String& s) : String(s) {
      }
      explicit StringSumHelper(const char* p) : String(p) {
      }
      explicit StringSumHelper(const char c) : String(c) {
      }
      explicit StringSumHelper(const unsigned char num) : String(num) {
      }
      explicit StringSumHelper(const int num) : String(num) {
      }
      explicit StringSumHelper(const unsigned int num) : String(num) {
      }
      explicit StringSumHelper(const long num) : String(num) {
      }
      explicit StringSumHelper(const unsigned long num) : String(num) {
      }
      explicit StringSumHelper(const float num) : String(num) {
      }
      explicit StringSumHelper(const double num) : String(num) {
      }
  };

} // namespace arduino

using arduino::__FlashStringHelper;
using arduino::String;

#endif // ARDUINO_CORE_API_ARDUINO_STRINGS_H
