/**
 * @file compiler_features.hpp
 * @author Robert Morley
 * @date 2026-04-22
 *
 * @brief
 *
 * @version 1.0.0.0
 *
 * @copyright Copyright (c) 2026
 */
#ifndef UOFT_PHYSICS_ESS_COMPILE_FEATURES_H
#define UOFT_PHYSICS_ESS_COMPILE_FEATURES_H

// Defines if and ARM or RISCV core is used.
#define ARM_CORE        __arm__
#define RISCV_CORE      __riscv

#ifndef __has_cpp_attribute
  #define __has_cpp_attribute(x) 0
#endif

// __cplusplus values for each standard version.
#define CXX_STANDARD_11 201103L
#define CXX_STANDARD_14 201402L
#define CXX_STANDARD_17 201703L
#define CXX_STANDARD_20 202002L
#define CXX_STANDARD_23 202302L
#define CXX_STANDARD_26 202603L

#define REQUIRED_CPP_STANDARD(standard)   (__cplusplus >= standard)

#if __cplusplus < CXX_STANDARD_17
#error "Compiler must support C++17 and above."
#endif

#ifdef __cpp_consteval
  #if __cpp_consteval
    #define CONSTEVAL consteval
  #endif
#else
  #define CONSTEVAL constexpr
#endif

// Library checks.
#ifdef __cpp_lib_math_constants
  #define HAS_MATH_CONSTANT_LIB() 1
#else
   #define HAS_MATH_CONSTANT_LIB() 0
#endif

#endif //UOFT_PHYSICS_ESS_COMPILE_FEATURES_H
