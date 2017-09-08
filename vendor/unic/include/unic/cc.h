/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
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

/*!@file unic/cc.h
 * @author uael
 */
#ifndef __UNIC_CC_H
# define __UNIC_CC_H

#define VERNO(A, B, C) (((A)*10000UL)+(B)*100UL+(C))

#ifndef __has_include
# define __has_include(X) 0
#endif
#ifndef __has_builtin
# define __has_builtin(X) unic_has_builtin_ ## X
#endif
#ifndef __has_feature
# define __has_feature(X) unic_has_feature_ ## X
#endif
#ifndef __has_extension
# define __has_extension __has_feature
#endif
#ifndef __has_attribute
# define __has_attribute(X) unic_has_attribute_ ## X
#endif

#define HAS_INCLUDE(X) __has_include(X)
#define HAS_BUILTIN(X) (__has_builtin(X) || unic_has_builtin_ ## X)
#define HAS_FEATURE(X) (__has_feature(X) || unic_has_feature_ ## X)
#define HAS_EXTENSION(X) (__has_extension(X) || unic_has_extension_ ## X)
#define HAS_ATTRIBUTE(X) (__has_attribute(X) || unic_has_attribute_ ## X)

#if __STDC_VERSION__ > 201100L
# ifndef unic_has_feature_stdnoreturn_h
#   define unic_has_feature_stdnoreturn_h 1
# endif
#endif

#if defined __clang__
# define CC_CLANG VERNO(__clang_major__, __clang_minor__, __clang_patchlevel__)
# if CC_CLANG > 30200UL
#   define unic_has_feature_stdnoreturn_h 1
# endif
#elif defined _MSC_VER
# define CC_MSVC _MSC_VER
#elif defined __GNUC__
# define CC_GCC VERNO(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
# define unic_has_attribute_weak 1
# define unic_has_attribute_always_inline 1
# define unic_has_attribute_noinline 1
# define unic_has_extension_attribute_const 1
# define unic_has_attribute_pure 1
# define unic_has_builtin___builtin_popcount 1
# if defined __GNUC_GNU_INLINE__ || (CC_GCC < 40300UL)
#  define unic_has_attribute_gnu_inline 1
# endif
# define unic_has_attribute_aligned 1
# define unic_has_attribute_noreturn 1
# define unic_has_attribute___noreturn__ 1
# define unic_has_attribute_unused 1
# if CC_GCC >= 30000UL
#  define unic_has_builtin___builtin_expect 1
# endif
# if CC_GCC >= 40600UL
#  define unic_has_attribute_externally_visible 1
# endif
# if defined __GNUC_STDC_INLINE__ || CC_GCC >= 40300UL
#  define unic_has_feature_c_inline 1
# endif
# if CC_GCC >= 40700UL
#  define unic_has_feature_stdnoreturn_h 1
#  define unic_has_feature_stdalign_h 1
# endif
#elif defined __INTEL_COMPILER
# define CC_INTEL VERNO(__INTEL_COMPILER, 0, 0)
#endif

#define restrict __restrict
#define volatile __volatile

#if HAS_FEATURE(c_inline)
# define inline __attribute__((__always_inline__)) __inline__
#elif defined CC_GCC
# ifdef inline
#   undef inline
# endif
# if HAS_ATTRIBUTE(gnu_inline)
#   define inline __attribute__((__always_inline__,__gnu_inline__,__weak__)) __inline__
# else
#   define inline __attribute__((__always_inline__,__weak__)) __inline__
# endif
#elif HAS_ATTRIBUTE(always_inline)
# define inline __attribute__((__always_inline__)) inline
#elif defined CC_MSVC || defined CC_INTEL
# define inline __inline
#else
# define inline static inline
#endif

#if HAS_FEATURE(stdnoreturn_h) || HAS_INCLUDE("stdnoreturn.h")
# include <stdnoreturn.h>
#elif HAS_ATTRIBUTE(noreturn) || (defined CC_GCC && CC_GCC >= 20700UL)
# define _Noreturn __attribute__((__noreturn__))
#elif defined __STDC__ && __STDC_VERSION__ >= 201112L
# define _Noreturn _Noreturn
#elif defined CC_MSVC
# define _Noreturn __declspec(noreturn)
#else
# define _Noreturn
#endif

#if HAS_ATTRIBUTE(pure)
# define purecall __attribute__((__pure__))
#else
# define purecall
#endif

#if HAS_EXTENSION(attribute_const) || HAS_ATTRIBUTE(const)
# define constcall __attribute__((__const__))
#else
# define constcall
#endif

#if defined CC_GCC || HAS_ATTRIBUTE(unused)
# define UNUSED __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED /*@unused@*/
#else
# define UNUSED
#endif

#endif /* !__UNIC_CC_H */
