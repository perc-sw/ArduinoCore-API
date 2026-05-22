/*
  Interrupts.h - Arduino interrupt management functions
  Copyright (c) 2018 Arduino LLC. All right reserved.

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

#ifndef ARDUINO_CORE_API_W_INTERRUPTS_CPP
#define ARDUINO_CORE_API_W_INTERRUPTS_CPP

#include <cstdint>
#include <cstdlib>
#include "Common.h"

namespace arduino {

  template <typename T>
  using voidTemplateFuncPtrParam = void (*)(T param);

  template <typename T>
  struct __container__ {
      void*                       param;
      voidTemplateFuncPtrParam<T> function;
  };

  // C++ only overloaded version of attachInterrupt function
  template <typename T>
  void attachInterrupt(pin_size_t interruptNum, voidTemplateFuncPtrParam<T> userFunc, PinStatus mode, T& param) {
    // TODO : Remove new operator for something else. No dynamic allocation.
    __container__<T>* cont = new __container__<T>();
    cont->param            = &param;
    cont->function         = userFunc;

    // TODO: check lambda scope
    // TODO: add structure to delete(__container__) when detachInterrupt() is called
    auto f = [](void* a) -> void {
      T param = *(T*)((__container__<T>*)a)->param;
      (((__container__<T>*)a)->function)(param);
    };

    attachInterruptParam(interruptNum, f, mode, cont);
  }

  template <typename T>
  void attachInterrupt(const pin_size_t interruptNum, const voidTemplateFuncPtrParam<T*> userFunc, const PinStatus mode,
                       T* param) {
    attachInterruptParam(interruptNum, reinterpret_cast<voidFuncPtrParam>(userFunc), mode,
                         reinterpret_cast<void*>(param));
  }

} // namespace arduino

#endif
