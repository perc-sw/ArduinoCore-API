/*
  String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All rights reserved.
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
#include "String.h"
#include <cfloat>
#include <memory>
#include "Common.h"
#include "deprecated-avr-comp/avr/dtostrf.h"
#include "itoa.h"

namespace arduino {

  /************************************i*********/
  /*  Static Member Initialisation             */
  /*********************************************/

  // size_t const String::FLT_MAX_DECIMAL_PLACES;
  // size_t const String::DBL_MAX_DECIMAL_PLACES;

  /*********************************************/
  /*  Constructors                             */
  /*********************************************/

  String::String(const char* cstr) {
    init();
    if (cstr) {
      copy(cstr, strlen(cstr));
    }
  }

  String::String(const char* cstr, const std::size_t length) {
    init();
    if (cstr) {
      copy(cstr, length);
    }
  }

  String::String(const String& str) {
    init();
    *this = str;
  }

  String::String(const __FlashStringHelper* str) {
    init();
    *this = str;
  }

  String::String(String&& str) noexcept : buffer(str.buffer), capacity(str.capacity), len(str.len) {
    str.buffer   = nullptr;
    str.capacity = 0;
    str.len      = 0;
  }

  String::String(const char c) {
    init();
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    *this  = buf;
  }

  String::String(const std::uint8_t value, const std::uint8_t base) {
    init();
    char buf[1 + 8 * sizeof(unsigned char)];
    utoa(value, buf, base);
    *this = buf;
  }

  String::String(const int value, const std::uint8_t base) {
    init();
    char buf[2 + 8 * sizeof(int)];
    itoa(value, buf, base);
    *this = buf;
  }

  String::String(const unsigned int value, const std::uint8_t base) {
    init();
    char buf[1 + 8 * sizeof(unsigned int)];
    utoa(value, buf, base);
    *this = buf;
  }

  String::String(const std::int32_t value, const std::uint8_t base) {
    init();
    char buf[2 + 8 * sizeof(long)];
    ltoa(value, buf, base);
    *this = buf;
  }

  String::String(const std::uint32_t value, const std::uint8_t base) {
    init();
    char buf[1 + 8 * sizeof(unsigned long)];
    ultoa(value, buf, base);
    *this = buf;
  }

  String::String(const float value, std::uint8_t decimalPlaces) {
    static constexpr std::size_t FLOAT_BUF_SIZE{ FLT_MAX_10_EXP + FLT_MAX_DECIMAL_PLACES + 1 /* '-' */ + 1 /* '.' */ +
                                                 1 /* '\0' */ };
    init();
    char buf[FLOAT_BUF_SIZE];
    decimalPlaces = min(static_cast<std::size_t>(decimalPlaces), FLT_MAX_DECIMAL_PLACES);
    *this         = dtostrf(value, (decimalPlaces + 2u), decimalPlaces, buf);
  }

  String::String(const double value, std::uint8_t decimalPlaces) {
    static constexpr std::size_t DOUBLE_BUF_SIZE{ DBL_MAX_10_EXP + DBL_MAX_DECIMAL_PLACES + 1 /* '-' */ + 1 /* '.' */ +
                                                  1 /* '\0' */ };
    init();
    char buf[DOUBLE_BUF_SIZE];
    decimalPlaces = min(static_cast<std::size_t>(decimalPlaces), DBL_MAX_DECIMAL_PLACES);
    *this         = dtostrf(value, (decimalPlaces + 2u), decimalPlaces, buf);
  }

  String::~String() {
    if (buffer) {
      free(buffer);
    }
  }

  /*********************************************/
  /*  Memory Management                        */
  /*********************************************/

  bool String::reserve(const std::size_t size) {
    if (buffer && capacity >= size) {
      return true;
    }

    if (changeBuffer(size)) {
      if (len == 0) {
        buffer[0] = 0;
      }
      return true;
    }

    return false;
  }

  inline void String::init() {
    buffer   = nullptr;
    capacity = 0;
    len      = 0;
  }

  void String::invalidate() {
    if (buffer) {
      free(buffer);
    }
    buffer   = nullptr;
    capacity = len = 0;
  }

  bool String::changeBuffer(const std::size_t maxStrLen) {
    if (auto newbuffer = static_cast<char*>(realloc(buffer, maxStrLen + 1))) {
      buffer   = newbuffer;
      capacity = maxStrLen;
      return true;
    }

    return false;
  }

  /*********************************************/
  /*  Copy and Move                            */
  /*********************************************/

  String& String::copy(const char* cstr, const std::size_t length) {
    if (!reserve(length)) {
      invalidate();
      return *this;
    }
    len = length;
    memcpy(buffer, cstr, length);
    buffer[len] = '\0';
    return *this;
  }

  String& String::copy(const __FlashStringHelper* pstr, const std::size_t length) {
    if (!reserve(length)) {
      invalidate();
      return *this;
    }
    len = length;
    strcpy_P(buffer, (PGM_P)pstr);
    return *this;
  }

  void String::move(String& rhs) {
    if (this != &rhs) {
      free(buffer);

      buffer   = rhs.buffer;
      len      = rhs.len;
      capacity = rhs.capacity;

      rhs.buffer   = nullptr;
      rhs.len      = 0;
      rhs.capacity = 0;
    }
  }

  String& String::operator=(const String& rhs) {
    if (this == &rhs) {
      return *this;
    }

    if (rhs.buffer) {
      copy(rhs.buffer, rhs.len);
    } else {
      invalidate();
    }

    return *this;
  }

  String& String::operator=(String&& rhs) noexcept {
    move(rhs);
    return *this;
  }

  String& String::operator=(const char* cstr) {
    if (cstr) {
      copy(cstr, strlen(cstr));
    } else {
      invalidate();
    }

    return *this;
  }

  String& String::operator=(const __FlashStringHelper* pstr) {
    if (pstr) {
      copy(pstr, strlen_P((PGM_P)pstr));
    } else {
      invalidate();
    }

    return *this;
  }

  /*********************************************/
  /*  concat                                   */
  /*********************************************/

  bool String::concat(const String& str) {
    return concat(str.buffer, str.len);
  }

  bool String::concat(const char* cstr, const std::size_t length) {
    const std::size_t new_len = len + length;
    if (!cstr) {
      return false;
    }

    if (length == 0) {
      return true;
    }

    if (!reserve(new_len)) {
      return false;
    }

    memcpy(buffer + len, cstr, length);
    len         = new_len;
    buffer[len] = '\0';

    return true;
  }

  bool String::concat(const char* cstr) {
    if (!cstr) {
      return false;
    }

    return concat(cstr, strlen(cstr));
  }

  bool String::concat(const char c) {
    return concat(&c, 1);
  }

  bool String::concat(const std::uint8_t num) {
    char buf[1 + 3 * sizeof(unsigned char)];
    itoa(num, buf, 10);
    return concat(buf);
  }

  bool String::concat(const int num) {
    char buf[2 + 3 * sizeof(int)];
    itoa(num, buf, 10);
    return concat(buf);
  }

  bool String::concat(const unsigned int num) {
    char buf[1 + 3 * sizeof(unsigned int)];
    utoa(num, buf, 10);
    return concat(buf);
  }

  bool String::concat(const std::int32_t num) {
    char buf[2 + 3 * sizeof(long)];
    ltoa(num, buf, 10);
    return concat(buf);
  }

  bool String::concat(const std::uint32_t num) {
    char buf[1 + 3 * sizeof(unsigned long)];
    ultoa(num, buf, 10);
    return concat(buf);
  }

  bool String::concat(const float num) {
    char        buf[20];
    const char* string = dtostrf(num, 4, 2, buf);
    return concat(string);
  }

  bool String::concat(const double num) {
    char        buf[20];
    const char* string = dtostrf(num, 4, 2, buf);
    return concat(string);
  }

  bool String::concat(const __FlashStringHelper* str) {
    if (!str) {
      return false;
    }

    const int length = strlen_P(reinterpret_cast<const char*>(str));

    if (length == 0) {
      return true;
    }

    const std::size_t new_len = len + length;

    if (!reserve(new_len)) {
      return false;
    }

    strcpy_P(buffer + len, reinterpret_cast<const char*>(str));
    len = new_len;

    return true;
  }

  /*********************************************/
  /*  Concatenate                              */
  /*********************************************/

  StringSumHelper& operator+(const StringSumHelper& lhs, const String& rhs) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(rhs.buffer, rhs.len)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const char* cstr) {
    auto& a = const_cast<StringSumHelper&>(lhs);
    if (!cstr || !a.concat(cstr))
      a.invalidate();
    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const char c) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(c)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const unsigned char num) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(num)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const int num) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(num)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const unsigned int num) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(num)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const std::int32_t num) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(num)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const std::uint32_t num) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(num)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const float num) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(num)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const double num) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(num)) {
      a.invalidate();
    }

    return a;
  }

  StringSumHelper& operator+(const StringSumHelper& lhs, const __FlashStringHelper* rhs) {
    auto& a = const_cast<StringSumHelper&>(lhs);

    if (!a.concat(rhs)) {
      a.invalidate();
    }

    return a;
  }

  /*********************************************/
  /*  Comparison                               */
  /*********************************************/

  int String::compareTo(const String& s) const {
    if (!buffer || !s.buffer) {

      if (s.buffer && s.len > 0) {
        return 0 - *reinterpret_cast<unsigned char*>(s.buffer);
      }

      if (buffer && len > 0) {
        return *reinterpret_cast<unsigned char*>(buffer);
      }

      return 0;
    }

    return strcmp(buffer, s.buffer);
  }

  int String::compareTo(const char* cstr) const {
    if (!buffer || !cstr) {
      if (cstr && *cstr) {
        return 0 - *cstr;
      }

      if (buffer && len > 0) {
        return *reinterpret_cast<unsigned char*>(buffer);
      }

      return 0;
    }
    return strcmp(buffer, cstr);
  }

  bool String::equals(const String& s) const {
    return (len == s.len && compareTo(s) == 0);
  }

  bool String::equals(const char* cstr) const {
    if (len == 0) {
      return (cstr == nullptr || *cstr == 0);
    }

    if (cstr == nullptr) {
      return buffer[0] == 0;
    }

    return strcmp(buffer, cstr) == 0;
  }

  bool String::equalsIgnoreCase(const String& str) const {
    if (this == &str) {
      return true;
    }

    if (len != str.len) {
      return false;
    }

    if (len == 0) {
      return true;
    }

    const char* p1 = buffer;
    const char* p2 = str.buffer;

    while (*p1) {
      if (tolower(*p1++) != tolower(*p2++)) {
        return false;
      }
    }

    return true;
  }

  bool String::startsWith(const String& str) const {
    if (len < str.len) {
      return false;
    }

    return startsWith(str, 0);
  }

  bool String::startsWith(const String& str, const std::size_t offset) const {
    if (offset > len - str.len || !buffer || !str.buffer) {
      return false;
    }

    return strncmp(&buffer[offset], str.buffer, str.len) == 0;
  }

  bool String::endsWith(const String& str) const {
    if (len < str.len || !buffer || !str.buffer) {
      return false;
    }

    return strcmp(&buffer[len - str.len], str.buffer) == 0;
  }

  /*********************************************/
  /*  Character Access                         */
  /*********************************************/

  char String::charAt(const std::size_t index) const {
    return operator[](index);
  }

  void String::setCharAt(const std::size_t index, const char c) const {
    if (index < len) {
      buffer[index] = c;
    }
  }

  char& String::operator[](const std::size_t index) {
    static char dummy_writable_char;

    if (index >= len || !buffer) {
      dummy_writable_char = 0;
      return dummy_writable_char;
    }

    return buffer[index];
  }

  char String::operator[](const std::size_t index) const {
    if (index >= len || !buffer) {
      return 0;
    }

    return buffer[index];
  }

  void String::getBytes(unsigned char* buf, const std::size_t buf_size, const std::size_t index) const {
    if (!buf_size || !buf) {
      return;
    }

    if (index >= len) {
      buf[0] = 0;
      return;
    }

    unsigned int n = buf_size - 1;

    if (n > len - index) {
      n = len - index;
    }

    strncpy(reinterpret_cast<char*>(buf), buffer + index, n);

    buf[n] = 0;
  }

  /*********************************************/
  /*  Search                                   */
  /*********************************************/

  int String::indexOf(const char ch) const {
    return indexOf(ch, 0);
  }

  int String::indexOf(const char ch, const std::size_t fromIndex) const {
    if (fromIndex >= len) {
      return -1;
    }

    const char* temp = strchr(buffer + fromIndex, ch);

    if (temp == nullptr) {
      return -1;
    }

    return temp - buffer;
  }

  int String::indexOf(const String& str) const {
    return indexOf(str, 0);
  }

  int String::indexOf(const String& str, const std::size_t fromIndex) const {
    if (fromIndex >= len) {
      return -1;
    }

    const char* found = strstr(buffer + fromIndex, str.buffer);

    if (found == nullptr) {
      return -1;
    }

    return found - buffer;
  }

  int String::lastIndexOf(const char ch) const {
    return lastIndexOf(ch, len - 1);
  }

  int String::lastIndexOf(const char ch, const std::size_t fromIndex) const {
    if (fromIndex >= len) {
      return -1;
    }

    const char temp_char = buffer[fromIndex + 1];

    buffer[fromIndex + 1] = '\0';

    const char* temp = strrchr(buffer, ch);

    buffer[fromIndex + 1] = temp_char;

    if (temp == nullptr) {
      return -1;
    }

    return temp - buffer;
  }

  int String::lastIndexOf(const String& str) const {
    return lastIndexOf(str, len - str.len);
  }

  int String::lastIndexOf(const String& str, std::size_t fromIndex) const {
    if (str.len == 0 || len == 0 || str.len > len) {
      return -1;
    }

    if (fromIndex >= len) {
      fromIndex = len - 1;
    }

    int found = -1;

    for (const char* p = buffer; p <= buffer + fromIndex; p++) {
      p = strstr(p, str.buffer);

      if (!p) {
        break;
      }

      if (static_cast<unsigned int>(p - buffer) <= fromIndex) {
        found = p - buffer;
      }
    }

    return found;
  }

  String String::substring(std::size_t beginIndex, std::size_t endIndex) const {
    if (beginIndex > endIndex) {
      const std::size_t temp = endIndex;
      endIndex               = beginIndex;
      beginIndex             = temp;
    }

    String out;

    if (beginIndex >= len) {
      return out;
    }

    if (endIndex > len) {
      endIndex = len;
    }

    out.copy(buffer + beginIndex, endIndex - beginIndex);

    return out;
  }

  /*********************************************/
  /*  Modification                             */
  /*********************************************/

  void String::replace(const char find, const char replace) const {
    if (!buffer) {
      return;
    }

    for (char* p = buffer; *p; p++) {
      if (*p == find)
        *p = replace;
    }
  }

  void String::replace(const String& find, const String& replace) {
    if (len == 0 || find.len == 0) {
      return;
    }

    int diff = static_cast<int>(replace.len - find.len);

    char* readFrom = buffer;
    char* foundAt;

    if (diff == 0) {
      while ((foundAt = strstr(readFrom, find.buffer)) != nullptr) {
        memcpy(foundAt, replace.buffer, replace.len);
        readFrom = foundAt + replace.len;
      }
    } else if (diff < 0) {
      unsigned int size = len; // compute size needed for result
      diff              = 0 - diff;

      while ((foundAt = strstr(readFrom, find.buffer)) != nullptr) {
        readFrom = foundAt + find.len;
        size -= diff;
      }

      if (size == len) {
        return;
      }

      int index = static_cast<int>(len) - 1;

      while (index >= 0 && (index = lastIndexOf(find, index)) >= 0) {
        readFrom = buffer + index + find.len;
        memmove(readFrom - diff, readFrom, len - (readFrom - buffer));
        len -= diff;
        buffer[len] = 0;
        memcpy(buffer + index, replace.buffer, replace.len);
        index--;
      }
    } else {
      unsigned int size = len; // compute size needed for result

      while ((foundAt = strstr(readFrom, find.buffer)) != nullptr) {
        readFrom = foundAt + find.len;
        size += diff;
      }

      if (size == len) {
        return;
      }

      if (size > capacity && !changeBuffer(size)) {
        return; // XXX: tell user!
      }

      int index = static_cast<int>(len) - 1;

      while (index >= 0 && (index = lastIndexOf(find, index)) >= 0) {
        readFrom = buffer + index + find.len;
        memmove(readFrom + diff, readFrom, len - (readFrom - buffer));
        len += diff;
        buffer[len] = 0;
        memcpy(buffer + index, replace.buffer, replace.len);
        index--;
      }
    }
  }

  void String::remove(const std::size_t index) {
    // Pass the biggest integer as the count. The remove method
    // below will take care of truncating it at the end of the
    // string.
    remove(index, static_cast<std::size_t>(-1));
  }

  void String::remove(const std::size_t index, std::size_t count) {
    if (index >= len) {
      return;
    }

    if (count <= 0) {
      return;
    }

    if (count > len - index) {
      count = len - index;
    }

    char* writeTo = buffer + index;
    len           = len - count;

    memmove(writeTo, buffer + index + count, len - index);
    buffer[len] = 0;
  }

  void String::toLowerCase() const {
    if (!buffer) {
      return;
    }

    for (char* p = buffer; *p; p++) {
      *p = tolower(*p);
    }
  }

  void String::toUpperCase() const {
    if (!buffer) {
      return;
    }

    for (char* p = buffer; *p; p++) {
      *p = toupper(*p);
    }
  }

  void String::trim() {
    if (!buffer || len == 0) {
      return;
    }

    const char* begin = buffer;

    while (isspace(*begin)) {
      begin++;
    }

    const char* end = buffer + len - 1;

    while (isspace(*end) && end >= begin) {
      end--;
    }

    len = end + 1 - begin;
    if (begin > buffer) {
      memmove(buffer, begin, len);
    }

    buffer[len] = 0;
  }

  /*********************************************/
  /*  Parsing / Conversion                     */
  /*********************************************/

  std::int32_t String::toInt() const {
    if (buffer) {
      return atol(buffer);
    }

    return 0;
  }

  float String::toFloat() const {
    return static_cast<float>(toDouble());
  }

  double String::toDouble() const {
    if (buffer) {
      return atof(buffer);
    }

    return 0;
  }

} // namespace arduino
