/*
 * MIT License
 *
 * Copyright (c) 2017 Lucas Abel <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!\file types.h
 * \author Mattias Jansson <www.github.com/rampantpixels>
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_TYPES_H__
# define U_TYPES_H__

#include "compiler.h"

#ifdef __cplusplus
# include <cstdarg>
# include <cstdint>
# include <cstddef>
# include <cstdlib>
# include <cstdio>
# include <cfloat>
# include <climits>
# include <ctype.h>
#else
# include <stdarg.h>
# include <stdint.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <float.h>
# include <limits.h>
# include <ctype.h>
#endif
#if !PLATFORM_WINDOWS
# ifdef __cplusplus
#  include <cwchar>
# else
#  include <wchar.h>
# endif
#endif
#if PLATFORM_PNACL || (PLATFORM_POSIX && !PLATFORM_APPLE)
# include <sys/types.h>
#endif

#ifndef __cplusplus
# define nullptr NULL
#endif

#ifndef __cplusplus
# if COMPILER_MSVC && COMPILER_VERSION < 1900
#   define bool	unsigned char
#   define true	1
#   define false	0
#   define __bool_true_false_are_defined	1
# else
#   include <stdbool.h>
# endif
#endif

#define FLOAT32_C(x) (x##f)
#define FLOAT64_C(x) (x)

#define SIZE_REAL 4
#if SIZE_REAL == 8
typedef double real;
# define REAL_C(x) FLOAT64_C(x)
#else
typedef float real;
# define REAL_C(x) FLOAT32_C(x)
#endif

#if ARCH_ARM_64 || ARCH_X86_64 || ARCH_PPC_64 || ARCH_IA64 || ARCH_MIPS_64
# define SIZE_POINTER 8
#else
# define SIZE_POINTER 4
#endif

#if PLATFORM_LINUX_RASPBERRYPI
# define SIZE_WCHAR 4
#else
# if WCHAR_MAX > 0xffff
#   define SIZE_WCHAR 4
# else
#   define SIZE_WCHAR 2
# endif
#endif

#if COMPILER_MSVC
# define PRId64       "I64d"
# define PRIi64       "I64i"
# define PRIdPTR      "Id"
# define PRIiPTR      "Ii"
# define PRIo64       "I64o"
# define PRIu64       "I64u"
# define PRIx64       "I64x"
# define PRIX64       "I64X"
# define PRIoPTR      "Io"
# define PRIuPTR      "Iu"
# define PRIxPTR      "Ix"
# define PRIXPTR      "IX"
# define PRIsize      "Iu"
#else
# ifdef __cplusplus
#   include <cinttypes>
# else
#   include <inttypes.h>
# endif
# define PRIsize      "zu"
#endif

#define PRItick        PRIi64
#define PRIhash        PRIx64

#if SIZE_REAL == 8
# define PRIreal      "lf"
#else
# define PRIreal      "f"
#endif

#if COMPILER_MSVC
# if SIZE_POINTER == 8
#   define PRIfixPTR  "016I64X"
# else
#   define PRIfixPTR  "08IX"
# endif
#else
# if SIZE_POINTER == 8
#   define PRIfixPTR  "016" PRIXPTR
# else
#   define PRIfixPTR  "08" PRIXPTR
# endif
#endif

#if PLATFORM_WINDOWS
# if ARCH_X86
typedef int ssize_t;
# else
typedef int64_t ssize_t;
# endif
#endif

#endif /* U_TYPES_H__ */
