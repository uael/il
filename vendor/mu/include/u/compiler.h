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

/*!\file compiler.h
 * \author Mattias Jansson <www.github.com/rampantpixels>
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_COMPILER_H__
# define U_COMPILER_H__

#include "platform.h"

/*!\defgroup Compiler
 * \brief Compiler features support
 * @{
 */

/*!\def COMPILER_CLANG
 * \bried Defined to 1 if compiling with clang, 0 otherwise
 */
#define COMPILER_CLANG 0

/*!\def COMPILER_MSVC
 * \bried Defined to 1 if compiling with Microsoft compiler, 0 otherwise
 */
#define COMPILER_GCC 0

/*!\def COMPILER_GCC
 * \bried Defined to 1 if compiling with GCC, 0 otherwise
 */
#define COMPILER_MSVC 0

/*!\def COMPILER_INTEL
 * \bried Defined to 1 if compiling with Intel compiler, 0 otherwise
 */
#define COMPILER_INTEL 0

/*!\def COMPILER_NAME
 * \brief A string naming the compiler used
 * 
 * \def COMPILER_DESCRIPTION
 * \brief A string with a more detailed description of the compiler used, name and version.
 * 
 * \def RESTRICT
 * \brief Restrict attribute, defined to nothing if compiler does not support restrict
 * 
 * \def thread_local
 * \brief Thread local attribute, defined to nothing if the compiler/platform/architecture does not
 * support thread local variables.
 * instead.
 * 
 * \def DEPRECATED
 * \brief Deprecated attribute, marking a function/variable as deprecated
 * 
 * \def FORCEINLINE
 * \brief Attribute to force function to be inlined
 * 
 * \def NOINLINE
 * \brief Attribute to prevent function from being inlined
 * 
 * \def PURECALL
 * \brief Attribute declaring function to be pure, meaning it has no effects except the return value
 * and the return value depends only on the parameters and/or global variables.
 * 
 * \def CONSTCALL
 * \brief Attribute declaring function to be const, meaning it does not examine any values except
 * the arguments, and has no effects except the return value. Basically this is just slightly
 * more strict class than the PURECALL attribute, since function is not allowed to read global
 * memory. Note that a function that has pointer arguments and examines the data pointed to
 * must not be declared const. Likewise, a function that calls a non-const function must
 * usually not be const. It does not make sense for a const function to return void.
 * 
 * \def ALIGN
 * \brief Variable or type attribute declaring the variable/type to have the specifiedmemory alignment
 * \param alignment Alignment
 * 
 * \def alignof
 * \brief Get the alignment of the given type or variable
 * \param type Type or variable
 * 
 * \def ALIGNED_STRUCT
 * \brief Declare an aligned struct type
 * \param name Struct name
 * \param alignment Alignment
 *
 * @}
 */

#define NX_STRINGIFY(x) #x
#define STRINGIFY(x) NX_STRINGIFY(x)
#define VERSION_NO(A, B, C) (((A)*10000UL)+(B)*100UL+(C))

#ifndef __has_builtin
#  define __has_builtin(X) u_has_builtin_ ## X
#endif
#ifndef __has_feature
#  define __has_feature(X) u_has_feature_ ## X
#endif
#ifndef __has_extension
#  define __has_extension __has_feature
#endif
#ifndef __has_attribute
#  define __has_attribute(X) u_has_attribute_ ## X
#endif

#ifndef HAS_BUILTIN
# define HAS_BUILTIN(X) (__has_builtin(X) || u_has_builtin_ ## X)
# define HAS_FEATURE(X) (__has_feature(X) || u_has_feature_ ## X)
# define HAS_EXTENSION(X) (__has_extension(X) || u_has_extension_ ## X)
# define HAS_ATTRIBUTE(X) (__has_attribute(X) || u_has_attribute_ ## X)
#endif

#if __STDC_VERSION__ > 201100L
# ifndef u_has_feature_stdnoreturn_h
#   define u_has_feature_stdnoreturn_h 1
# endif
# ifndef u_has_feature_c_max_align_t
#   define u_has_feature_c_max_align_t 1
# endif
# ifndef u_has_feature_c_generic_selections
#   define u_has_feature_c_generic_selections 1
# endif
#endif

#if HAS_BUILTIN(__sync_val_compare_and_swap)
# ifndef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
#   define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1
# endif
# ifndef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2
#   define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1
# endif
# ifndef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#   define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1
#   if !defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) && (UINTPTR_MAX >= UINT64_MAX)
#     define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1
#   endif
# endif
#endif

#if defined(__clang__)
# undef  COMPILER_CLANG
# define COMPILER_CLANG 1
# define COMPILER_NAME "clang"
# define COMPILER_DESCRIPTION COMPILER_NAME " " #__clang_major__ "." #__clang_minor__
# define COMPILER_VERSION VERSION_NO(__clang_major__, __clang_minor__, __clang_patchlevel__)
# if COMPILER_VERSION > 30200UL
#   define u_has_feature_stdnoreturn_h 1
# endif
#elif defined(_MSC_VER)
# undef  COMPILER_MSVC
# define COMPILER_MSVC 1
# define COMPILER_NAME "msvc"
# define COMPILER_DESCRIPTION COMPILER_NAME " " STRINGIFY(_MSC_VER)
# define COMPILER_VERSION _MSC_VER
#elif defined(__GNUC__)
# undef  COMPILER_GCC
# define COMPILER_GCC 1
# define COMPILER_NAME "gcc"
# define COMPILER_DESCRIPTION COMPILER_NAME " " STRINGIFY(__GNUC__) "." STRINGIFY(__GNUC_MINOR__)
# define COMPILER_VERSION VERSION_NO(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
# define u_has_attribute_always_inline 1
# define u_has_attribute_noinline 1
# define u_has_attribute_weak 1
# define u_has_attribute_weakref 1
# define u_has_attribute_constructor 1
# define u_has_attribute_destructor 1
# define u_has_attribute_vector_size 1
# define u_has_attribute_warn_unused_result 1
# define u_has_extension_attribute_const 1
# define u_has_attribute_pure 1
# if defined(__GNUC_GNU_INLINE__) || (COMPILER_VERSION < 40300UL)
#  define u_has_attribute_gnu_inline 1
# endif
# define u_has_attribute_aligned 1
# define u_has_attribute_noreturn 1
# define u_has_attribute___noreturn__ 1
# define u_has_attribute_deprecated 1
# define u_has_attribute_unused 1
# ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#  define u_has_builtin___sync_val_compare_and_swap 1
#  define u_has_builtin___sync_lock_test_and_set 1
#  define u_has_builtin___sync_lock_release 1
#  define u_has_builtin___sync_synchronize 1
#  define u_has_builtin___sync_fetch_and_add 1
#  define u_has_builtin___sync_fetch_and_sub 1
#  define u_has_builtin___sync_fetch_and_or 1
#  define u_has_builtin___sync_fetch_and_and 1
#  define u_has_builtin___sync_fetch_and_xor 1
# endif
# if COMPILER_VERSION >= 30000UL
#  define u_has_builtin___builtin_expect 1
# endif
# if COMPILER_VERSION >= 40300UL
#  define u_has_attribute_error 1
# endif
# if COMPILER_VERSION >= 40600UL
#  define u_has_feature_c_static_assert 1
#  define u_has_attribute_externally_visible 1
# endif
# if defined(__GNUC_STDC_INLINE__) || COMPILER_VERSION >= 40300UL
#  define u_has_feature_c_inline 1
# endif
# define u_has_feature_gnu_thread_local 1
# define u_has_feature_gnu_alignof 1
# define u_has_feature_statement_expression 1
# define u_has_feature_tgmath_h 1
# define TYPEOF(X) __typeof__(X)
# if (COMPILER_VERSION >= 40700UL) && (COMPILER_VERSION < 40900UL)
#  define u_has_feature_stdnoreturn_h 1
#  define u_has_feature_stdalign_h 1
#  if __STDC_VERSION__ > 201100L
#   define u_has_feature_c_max_align_t 1
#   define __STDC_NO_ATOMICS__ 1
#   define u_has_feature_c_generic_selections 0
#  endif
# endif
# if (COMPILER_VERSION >= 40900UL) && (COMPILER_VERSION < 41000UL)
#  define u_has_feature_stdnoreturn_h 1
#  define u_has_feature_stdalign_h 1
#  if __STDC_VERSION__ > 201100L
#   define u_has_feature_c_max_align_t 1
#   define u_has_feature_c_generic_selections 1
#  endif
# endif
#elif defined(__ICL) || defined(__ICC) || defined(__INTEL_COMPILER)
# undef  COMPILER_INTEL
# define COMPILER_INTEL 1
# define COMPILER_NAME "intel"
# if defined(__ICL)
#   define COMPILER_DESCRIPTION COMPILER_NAME " " #__ICL
# elif defined(__ICC)
#   define COMPILER_DESCRIPTION COMPILER_NAME " " #__ICC
# endif
# define COMPILER_VERSION VERSION_NO(__INTEL_COMPILER, 0, 0)
# if __INTEL_COMPILER < 1310
#   if defined(__GNUC__) && defined(u_has_feature_tgmath_h)
#     undef u_has_feature_tgmath_h
#   endif
# else
#   define u_has_feature_stdatomic_h 1
# endif
#else
# warning Unknown compiler
# define COMPILER_NAME "unknown"
# define COMPILER_DESCRIPTION "unknown"
#endif

#if PLATFORM_POSIX
# ifndef _GNU_SOURCE
#   define _GNU_SOURCE
# endif
#endif

#if COMPILER_INTEL
# if (__ICC < 1400) && (__GNUC__ == 4) && (__GNUC_MINOR__ > 2)
#   undef __GNUC_MINOR__
#   define __GNUC_MINOR__ 2
#   undef __GNUC_PATCHLEVEL__
#   define __GNUC_PATCHLEVEL__ 0
# endif
#endif

#ifdef __GNUC__
# define extension __extension__
#endif

#define restrict __restrict

#if COMPILER_MSVC
# define thread_local __declspec(thread)
#elif COMPILER_GCC
# define thread_local __thread
#else
# define thread_local _Thread_local
#endif

#if HAS_ATTRIBUTE(noreturn) || (COMPILER_GCC && (__GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 70)))
# define NORETURN __attribute__((__noreturn__))
#elif defined(__STDC__) && __STDC_VERSION__ >= 201112L
# define NORETURN _Noreturn
#elif COMPILER_MSVC
# define NORETURN __declspec(noreturn)
#else
# define NORETURN
#endif

#if HAS_ATTRIBUTE(deprecated)
# define DEPRECATED __attribute__((__deprecated__))
#elif COMPILER_MSVC
# define DEPRECATED __declspec(deprecated)
#else
# define DEPRECATED
#endif

#if HAS_FEATURE(c_inline)
# define inline __inline__
# define FORCEINLINE __attribute__((__always_inline__)) __inline__
# if HAS_ATTRIBUTE(externally_visible)
#   define INSTANTIATE __attribute__((__externally_visible__)) extern __inline__
# else
#   define INSTANTIATE extern __inline__
# endif
#elif COMPILER_GCC
# ifdef inline
#   undef inline
# endif
# if HAS_ATTRIBUTE(gnu_inline)
#   define inline __attribute__((__gnu_inline__,__weak__)) __inline__
#   define FORCEINLINE __attribute__((__always_inline__,__gnu_inline__,__weak__)) __inline__
# else
#   define inline __attribute__((__weak__)) __inline__
#   define FORCEINLINE __attribute__((__always_inline__,__weak__)) __inline__
# endif
# define static_inline static __inline__
# if HAS_ATTRIBUTE(externally_visible)
#   define INSTANTIATE __attribute__((__externally_visible__))
# else
#   define INSTANTIATE
# endif
#elif HAS_ATTRIBUTE(always_inline)
# define FORCEINLINE __attribute__((__always_inline__)) inline
#elif COMPILER_MSVC || COMPILER_INTEL
# define FORCEINLINE __forceinline
#else
# define FORCEINLINE static inline
#endif

#if HAS_ATTRIBUTE(noinline)
# define NOINLINE __attribute__((__noinline__))
#elif COMPILER_MSVC || COMPILER_INTEL
# define NOINLINE __declspec(noinline)
#else
# define NOINLINE
#endif

#if HAS_ATTRIBUTE(pure)
# define PURECALL __attribute__((__pure__))
#else
# define PURECALL
#endif

#if HAS_EXTENSION(attribute_const)
# define CONSTCALL __attribute__((__const__))
#else
# define CONSTCALL
#endif

#ifndef INSTANTIATE
# if HAS_ATTRIBUTE(externally_visible)
#   define INSTANTIATE __attribute__((__externally_visible__)) extern __inline__
# else
#   define INSTANTIATE extern __inline__
# endif
#endif

#if HAS_ATTRIBUTE(weak)
# define WEAK(ID) __attribute__((__weak__))
#elif COMPILER_MSVC
# define WEAK(ID) __declspec(selectany)
#else
# define WEAK(ID) _Pragma(STRINGIFY(weak ID))
#endif

#if HAS_ATTRIBUTE(weakref)
# define WEAKREF(ID, ...) __attribute__((__weakref__(#__VA_ARGS__)))
#else
# define WEAKREF(ID, ...) _Pragma(STRINGIFY(weak ID=__VA_ARGS__))
#endif

#if HAS_ATTRIBUTE(warn_unused_result)
# define UNUSED_RESULT __attribute__((__warn_unused_result__))
#elif COMPILER_MSVC && COMPILER_VERSION >= 1700
# define UNUSED_RESULT _Check_return_
#else
# define UNUSED_RESULT
#endif

#ifdef __cplusplus
# define UNUSED [[unused]]
#elif COMPILER_GCC || HAS_ATTRIBUTE(unused)
# define UNUSED __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED /*@unused@*/
#else
# define UNUSED x
#endif

#if COMPILER_CLANG
# define PRINTFCALL(start, num) __attribute__((format(printf, start, num)))
# define ALIGN(alignment) __attribute__((__aligned__(alignment)))
# define alignof(type) __alignof__(type)
# define ALIGNED(declaration, alignment) declaration ALIGN(alignment)
# define PACKED(declaration) declaration __attribute__((__packed__))
# define LIKELY(x) __builtin_expect(!!(x), 1)
# define UNLIKELY(x) __builtin_expect(!!(x), 0)
# pragma clang diagnostic ignored "-Wmissing-field-initializers"
# pragma clang diagnostic ignored "-Wmissing-braces"
# if PLATFORM_WINDOWS
#   if (CLANG_VERSION < 30800)
#     error CLang 3.8 or later is required
#   endif
#   define STDCALL ATTR(stdcall)
#   ifndef __USE_MINGW_ANSI_STDIO
#     define __USE_MINGW_ANSI_STDIO 1
#   endif
#   ifndef _CRT_SECURE_NO_WARNINGS
#     define _CRT_SECURE_NO_WARNINGS 1
#   endif
#   ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#     define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#   endif
#   ifndef __MSVCRT_VERSION__
#     define __MSVCRT_VERSION__ 0x0800
#   endif
#   define USE_NO_MINGW_SETJMP_TWO_ARGS 1
# endif
#elif COMPILER_INTEL
# define PRINTFCALL(start, num)
# define ALIGN(alignment) __declspec(align(alignment))
# define alignof(type) __alignof(type)
# define ALIGNED(declaration, alignment) declaration ALIGN(alignment)
# define PACKED(declaration) __pragma(pack(push, 1)) declaration __pragma(pack(pop))
# define LIKELY(x) __builtin_expect(!!(x), 1)
# define UNLIKELY(x) __builtin_expect(!!(x), 0)
# if PLATFORM_WINDOWS
#   define STDCALL __stdcall
#   define va_copy(d,s) ((d)=(s))
# endif
# include <intrin.h>
#elif COMPILER_MSVC
# define PRINTFCALL(start, num)
# define ALIGN(alignment) __declspec(align(alignment))
# define alignof(type) __alignof(type)
# define ALIGNED(declaration, alignment) declaration ALIGN(alignment)
# define PACKED(declaration) __pragma(pack(push, 1)) declaration __pragma(pack(pop))
# define LIKELY(x) (x)
# define UNLIKELY(x) (x)
# pragma warning(disable : 4018)
# pragma warning(disable : 4127)
# pragma warning(disable : 4132)
# pragma warning(disable : 4146)
# pragma warning(disable : 4200)
# pragma warning(disable : 4204)
# pragma warning(disable : 4706)
# pragma warning(disable : 4028)
# ifdef __cplusplus
#   pragma warning(disable : 4100)
#   pragma warning(disable : 4510)
#   pragma warning(disable : 4512)
#   pragma warning(disable : 4610)
# endif
# if PLATFORM_WINDOWS
#   define STDCALL __stdcall
# endif
# if defined(COMPILE) && COMPILE && !defined(_CRT_SECURE_NO_WARNINGS)
#   define _CRT_SECURE_NO_WARNINGS 1
# endif
# ifndef _LINT
#   define static_assert static_assert
# endif
# if _MSC_VER < 1800
#   define va_copy(d,s) ((d)=(s))
# endif
# include <intrin.h>
#elif COMPILER_GCC
# define PRINTFCALL(start, num) __attribute__((format(printf, start, num)))
# define ALIGN(alignment) __attribute__((__aligned__(alignment)))
# define alignof(type) __alignof__(type)
# define ALIGNED(declaration, alignment) declaration __attribute__((__aligned__(alignment)))
# define PACKED(declaration) declaration __attribute__((__packed__))
# define LIKELY(x) __builtin_expect(!!(x), 1)
# define UNLIKELY(x) __builtin_expect(!!(x), 0)
# pragma GCC diagnostic ignored "-Wmissing-field-initializers"
# pragma GCC diagnostic ignored "-Wmissing-braces"
# if COMPILER_VERSION >= 40600UL
#   define static_assert _Static_assert
# endif
# if PLATFORM_WINDOWS
#   define STDCALL ATTR(stdcall)
#   ifndef __USE_MINGW_ANSI_STDIO
#     define __USE_MINGW_ANSI_STDIO 1
#   endif
#   ifndef _CRT_SECURE_NO_WARNINGS
#     define _CRT_SECURE_NO_WARNINGS 1
#   endif
#   ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#     define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#   endif
#   ifndef __MSVCRT_VERSION__
#     define __MSVCRT_VERSION__ 0x0800
#   endif
# endif
#endif

#ifndef static_assert
# define static_assert(EXPR, DIAGSTR) \
  extern char const u_compiletime_assert[ \
   sizeof((void const*[3*(!!(EXPR)) - 1]){ \
     &u_compiletime_assert, \
     "static assertion failed: " STRINGIFY(EXPR) ", " DIAGSTR}) \
  ]
  extern char const u_compiletime_assert[sizeof(void const*[2])];
#endif

#endif /* U_COMPILER_H__ */
