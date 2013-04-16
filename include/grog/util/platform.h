/* This file is part of Grog.
 *
 * Copyright (c) 2013 Alvaro Polo
 *
 * Grog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Grog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GROG_UTIL_PLATFORM_H
#define GROG_UTIL_PLATFORM_H

/* Constant definitions for platform properties. */
#define GROG_ARCH_32            1
#define GROG_ARCH_64            2

#define GROG_PLATFORM_WINDOWS   1
#define GROG_PLATFORM_LINUX     2
#define GROG_PLATFORM_OSX       3

#define GROG_COMPILER_GCC       1
#define GROG_COMPILER_CLANG     2
#define GROG_COMPILER_MSVC      3
#define GROG_COMPILER_CYGWIN    4
#define GROG_COMPILER_MINGW     5

#define GROG_BIG_ENDIAN         1
#define GROG_LITTLE_ENDIAN      2

/* Check the platform architecture. */
#if defined(__x86_64__) || defined(_M_X64)   || defined(__powerpc64__) || \
    defined(__alpha__)  || defined(__ia64__) || defined(__s390__)      || \
    defined(__s390x__)
  #define GROG_ARCH_TYPE GROG_ARCH_64
#else
  #define GROG_ARCH_TYPE GROG_ARCH_32
#endif

/* Check the platform OS. */
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( _WIN64 )
  #define GROG_PLATFORM GROG_PLATFORM_WINDOWS
  #define GROG_PLATFORM_IS_WINDOWS
#elif defined( __APPLE_CC__) || (defined(__APPLE__) && defined(__MACH__))
  #define GROG_PLATFORM GROG_PLATFORM_OSX
  #define GROG_PLATFORM_IS_POSIX
#else
  #define GROG_PLATFORM GROG_PLATFORM_LINUX
  #define GROG_PLATFORM_IS_POSIX
#endif

/* Check the compiler. */
#if defined(__llvm__) || defined(__clang__)
  #define GROG_COMPILER GROG_COMPILER_CLANG
  #define GROG_COMPILER_MAJOR_VERSION __clang_major__
  #define GROG_COMPILER_MINOR_VERSION __clang_minor__
#elif defined(__GNUC__)
  #define GROG_COMPILER GROG_COMPILER_GCC
  #define GROG_COMPILER_MAJOR_VERSION __GNUC__
  #define GROG_COMPILER_MINOR_VERSION __GNUC_MINOR__
#elif defined(_MSC_VER)
  #define GROG_COMPILER GROG_COMPILER_MSVC
  #define GROG_COMPILER_MAJOR_VERSION ((_MSC_VER / 100) - 6)
  #define GROG_COMPILER_MINOR_VERSION ((_MSC_VER % 100) / 10)
  #pragma warning( disable : 4290 )
#elif defined(__CYGWIN__)
  #define GROG_COMPILER GROG_COMPILER_CYGWIN
  #define GROG_COMPILER_MAJOR_VERSION __GNUC__
  #define GROG_COMPILER_MINOR_VERSION __GNUC_MINOR__
#elif defined(__MINGW32__)
  #define GROG_COMPILER GROG_COMPILER_MINGW
  #define GROG_COMPILER_MAJOR_VERSION __GNUC__
  #define GROG_COMPILER_MINOR_VERSION __GNUC_MINOR__
#else
  #error cannot determine C++ compiler vendor
#endif

#define GROG_COMPILER_AT_LEAST(major, minor) (\
    GROG_COMPILER_MAJOR_VERSION > major || (\
        major == GROG_COMPILER_MAJOR_VERSION && \
        GROG_COMPILER_MINOR_VERSION >= minor ))

/* Check the endianness. */
#if defined(__i386__) || defined(_M_IX86)    || defined(_X86_)     || \
    defined(__ia64__) || defined(_M_IA64)    || defined(__amd64__) || \
    defined(__amd64)  || defined(__x86_64__) || defined(__x86_64)  || \
    defined(_M_X64)
  #define GROG_ENDIANNESS GROG_LITTLE_ENDIAN
#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || \
      defined(__ppc__) || defined(_M_PPC)
  #define GROG_ENDIANNESS GROG_BIG_ENDIAN
#else
  #error cannot determine endianness (unknown hardware architecture)
#endif

/* Define the symbol export macros. */
#if GROG_PLATFORM == GROG_PLATFORM_WINDOWS
  #ifdef GROG_LIB_BUILD
    #if GROG_COMPILER == GROG_COMPILER_CYGWIN || \
        GROG_COMPILER == GROG_COMPILER_MINGW || \
        GROG_COMPILER == GROG_COMPILER_CLANG
      #define GROG_EXPORT __attribute__((dllexport))
      #define GROG_LOCAL
    #else
      #define GROG_EXPORT __declspec(dllexport)
      #define GROG_LOCAL
    #endif
  #else
    #if GROG_COMPILER == GROG_COMPILER_CYGWIN || \
        GROG_COMPILER == GROG_COMPILER_MINGW || \
        GROG_COMPILER == GROG_COMPILER_CLANG
      #define GROG_EXPORT __attribute__((dllimport))
      #define GROG_LOCAL
    #else
      #define GROG_EXPORT __declspec(dllimport)
      #define GROG_LOCAL
    #endif
  #endif
#else // non-windows
  #define GROG_EXPORT __attribute__ ((visibility ("default")))
  #define GROG_IMPORT
  #define GROG_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

/* Define the macros to export template instantiations for DLLs. */
#if GROG_COMPILER == GROG_COMPILER_MSVC
  #define GROG_EXPORT_TEMPLATE(tempdef, ...) \
      template class GROG_EXPORT tempdef __VA_ARGS__
#else
  #define GROG_EXPORT_TEMPLATE(tempdef, ...)
#endif

// Integer formats of fixed bit width
#if GROG_ARCH_TYPE == GROG_ARCH_32
typedef unsigned long      UInt32;
typedef long               Int32;
#else
typedef unsigned int       UInt32;
typedef int                Int32;
#endif
typedef unsigned short     UInt16;
typedef short              Int16;
typedef unsigned char      UInt8;
typedef char               Int8;
typedef UInt8              Byte;

// Special case: 64-bit types
#if GROG_ARCH_TYPE == GROG_ARCH_64
  #if GROG_COMPILER == GROG_COMPILER_MSVC
typedef unsigned __int64   UInt64;
typedef __int64            Int64;
  #else
typedef unsigned long long UInt64;
typedef long long          Int64;
  #endif
#endif

#endif
