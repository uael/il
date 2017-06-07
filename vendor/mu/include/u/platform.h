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

/*!\file platform.h
 * \author Mattias Jansson <www.github.com/rampantpixels>
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_PLATFORM_H__
# define U_PLATFORM_H__

/*!\def U_COMPILE
 * \bried Defined to 1 when compiling the u library,
 * undefined (or zero) when using the library
 */
#ifndef U_COMPILE
# define U_COMPILE 0
#endif

/*!\def U_BUILD_DYNAMIC_LINK
 * \brief compiler option
 */
#ifndef U_BUILD_DYNAMIC_LINK
# define U_BUILD_DYNAMIC_LINK 0
#endif

/*!\defgroup Platform
 * \brief All supported platform flags
 * @{
 */

/*!\def PLATFORM_ANDROID
 * \brief Defined to 1 if compiling for android platforms, 0 otherwise
 */
#define PLATFORM_ANDROID 0

/*!\def PLATFORM_BSD
 * \brief Defined to 1 if compiling for bsd platforms, 0 otherwise
 */
#define PLATFORM_BSD 0

/*!\def PLATFORM_IOS
 * \brief Defined to 1 if compiling for ios platforms, 0 otherwise
 */
#define PLATFORM_IOS 0

/*!\def PLATFORM_IOS_SIMULATOR
 * \brief Defined to 1 if compiling for ios simulator platforms, 0 otherwise
 */
#define PLATFORM_IOS_SIMULATOR 0

/*!\def PLATFORM_LINUX
 * \brief Defined to 1 if compiling for linux platforms, 0 otherwise
 */
#define PLATFORM_LINUX 0

/*!\def PLATFORM_LINUX_RASPBERRYPI
 * \brief Defined to 1 if compiling for linus raspberrypi platforms, 0 otherwise
 */
#define PLATFORM_LINUX_RASPBERRYPI 0

/*!\def PLATFORM_MACOSX
 * \brief Defined to 1 if compiling for macosx platforms, 0 otherwise
 */
#define PLATFORM_MACOSX 0

/*!\def PLATFORM_WINDOWS
 * \brief Defined to 1 if compiling for windows platforms, 0 otherwise
 */
#define PLATFORM_WINDOWS 0

/*!\def PLATFORM_PNACL
 * \brief Defined to 1 if compiling for pnacl platforms, 0 otherwise
 */
#define PLATFORM_PNACL 0

/*!\def PLATFORM_TIZEN
 * \brief Defined to 1 if compiling for tizen platforms, 0 otherwise
 */
#define PLATFORM_TIZEN 0

/*!@}
 * \defgroup Platform traits
 * \brief Which kind is the actual platform
 * @{
 */

/*!\def PLATFORM_APPLE
 * \brief Defined to 1 if compiling for Apple platforms 
 * (MacOS X, iOS, iOS simulator), 0 otherwise
 */
#define PLATFORM_APPLE 0

/*!\def PLATFORM_POSIX
 * \brief Defined to 1 if compiling for POSIX platforms 
 * (Linux, BSD, MacOS X, iOS, iOS simulator, Android), 0 otherwise
 */
#define PLATFORM_POSIX 0

/*!@}
 * \defgroup Platform groups
 * \brief Which kind is the actual platform
 * @{
 */

/*!\def PLATFORM_MOBILE
 * \brief Defined to 1 if compiling for mobile platforms 
 * (iOS, iOS simulator, Android), 0 otherwise
 */
#define PLATFORM_MOBILE 0

/*!\def PLATFORM_DESKTOP
 * \brief Defined to 1 if compiling for desktop platforms 
 * (Windows, MacOS X, Linux, Raspberry Pi, BSD), 0 otherwise
 */
#define PLATFORM_DESKTOP 0

/*!\def PLATFORM_FAMILY_CONSOLE
 * \brief Defined to 1 if compiling for console platforms 
 * (iOS, iOS simulator, Android, Tizen), 0 otherwise
 */
#define PLATFORM_CONSOLE 0

/*!@}
 * \defgroup Platform architectures
 * \brief The platform architecture
 * @{
 */

/*!\def ARCH_ARM
 * \brief Defined to 1 if compiling for ARM architectures, 0 otherwise
 */
#define ARCH_ARM 0

/*!\def ARCH_ARM5
 * \brief Defined to 1 if compiling for ARMv5 architectures, 0 otherwise
 */
#define ARCH_ARM5 0

/*!\def ARCH_ARM6
 * \brief Defined to 1 if compiling for ARMv6 architectures, 0 otherwise
 */
#define ARCH_ARM6 0

/*!\def ARCH_ARM7
 * \brief Defined to 1 if compiling for ARMv7 architectures, 0 otherwise
 */
#define ARCH_ARM7 0

/*!\def ARCH_ARM8
 * \brief Defined to 1 if compiling for ARMv8 architectures, 0 otherwise
 */
#define ARCH_ARM8 0

/*!\def ARCH_ARM_64
 * \brief Defined to 1 if compiling for 64-bit ARM architectures, 0 otherwise
 */
#define ARCH_ARM_64 0

/*!
 * \def ARCH_ARM8_64
 * \brief Defined to 1 if compiling for 64-bit ARMv8 architectures, 0 otherwise
 */
#define ARCH_ARM8_64 0

/*!\def ARCH_X86
 * \brief Defined to 1 if compiling for x86 architectures, 0 otherwise
 */
#define ARCH_X86 0

/*!\def ARCH_X86_64
 * \brief Defined to 1 if compiling for x86-64 architectures, 0 otherwise
 */
#define ARCH_X86_64 0

/*!\def ARCH_PPC
 * \brief Defined to 1 if compiling for PPC architectures, 0 otherwise
 */
#define ARCH_PPC 0

/*!\def ARCH_PPC_64
 * \brief Defined to 1 if compiling for 64-bit PPC architectures, 0 otherwise
 */
#define ARCH_PPC_64 0

/*!\def ARCH_IA64
 * \brief Defined to 1 if compiling for IA64 architectures, 0 otherwise
 */
#define ARCH_IA64 0

/*!\def ARCH_MIPS
 * \brief Defined to 1 if compiling for MIPS architectures, 0 otherwise
 */
#define ARCH_MIPS 0

/*!\def ARCH_MIPS_64
 * \brief Defined to 1 if compiling for 64-bit MIPS architectures, 0 otherwise
 */
#define ARCH_MIPS_64 0

/*!\def ARCH_GENERIC
 * \brief Defined to 1 if compiling for pnacl generic architecture, 0 otherwise
 */
#define ARCH_GENERIC 0

/*!@}
 * \defgroup Platform architecture details
 * \brief The platform architecture details
 * @{
 */

/*!\def ARCH_SSE2
 * \brief Defined to 1 if compiling with SSE2 instruction set enabled, 0 otherwise
 */
#define ARCH_SSE2 0

/*!\def ARCH_SSE3
 * \brief Defined to 1 if compiling with SSE3 instruction set enabled, 0 otherwise
 */
#define ARCH_SSE3 0

/*!\def ARCH_SSE4
 * \brief Defined to 1 if compiling with SSE4 instruction set enabled, 0 otherwise
 */
#define ARCH_SSE4 0

/*!\def ARCH_SSE4_FMA3
 * \brief Defined to 1 if compiling with SSE4 instruction set 
 * (including FMA3 instruction) enabled, 0 otherwise
 */
#define ARCH_SSE4_FMA3 0

/*!\def ARCH_NEON
 * \brief Defined to 1 if compiling with NEON instruction set enabled, 0 otherwise
 */
#define ARCH_NEON 0

/*!\def ARCH_THUMB
 * \brief Defined to 1 if compiling for ARM THUMB instruction set, 0 otherwise
 */
#define ARCH_THUMB 0

/*!\def ARCH_ENDIAN_LITTLE
 * \brief Defined to 1 if compiling for little endian architectures, 0 otherwise
 */
#define ARCH_ENDIAN_LITTLE 0

/*!\def ARCH_ENDIAN_BIG
 * \brief Defined to 1 if compiling for big endian architectures, 0 otherwise
 */
#define ARCH_ENDIAN_BIG 0

/*!@}*/

#if defined(__pnacl__)
# undef  PLATFORM_PNACL
# define PLATFORM_PNACL 1
# define PLATFORM_NAME "PNaCl"
# define PLATFORM_DESCRIPTION "PNaCl"
# undef  ARCH_GENERIC
# define ARCH_GENERIC 1
# undef  ARCH_ENDIAN_LITTLE
# define ARCH_ENDIAN_LITTLE 1
#elif defined(__ANDROID__)
# undef  PLATFORM_ANDROID
# define PLATFORM_ANDROID 1
# undef  PLATFORM_POSIX
# define PLATFORM_POSIX 1
# define PLATFORM_NAME "Android"
# if defined(__arm__)
#   undef  ARCH_ARM
#   define ARCH_ARM 1
#   ifdef __ARM_ARCH_7A__
#     undef  ARCH_ARM7
#     define ARCH_ARM7 1
#     define PLATFORM_DESCRIPTION "Android ARMv7"
#   elif defined(__ARM_ARCH_5TE__)
#     undef  ARCH_ARM5
#     define ARCH_ARM5 1
#     define PLATFORM_DESCRIPTION "Android ARMv5"
#   else
#     error Unsupported ARM architecture
#   endif
# elif defined(__aarch64__)
#   undef  ARCH_ARM
#   define ARCH_ARM 1
#   undef  ARCH_ARM_64
#   define ARCH_ARM_64 1
#   undef  ARCH_ARM8_64
#   define ARCH_ARM8_64 1
#   define PLATFORM_DESCRIPTION "Android ARM64v8"
# elif defined(__i386__)
#   undef  ARCH_X86
#   define ARCH_X86 1
#   define PLATFORM_DESCRIPTION "Android x86"
# elif defined(__x86_64__)
#   undef  ARCH_X86_64
#   define ARCH_X86_64 1
#   define PLATFORM_DESCRIPTION "Android x86-64"
# elif defined(__mips__) && defined(__mips64)
#   undef  ARCH_MIPS
#   define ARCH_MIPS 1
#   undef  ARCH_MIPS_64
#   define ARCH_MIPS_64 1
#   define PLATFORM_DESCRIPTION "Android MIPS64"
#   ifndef _MIPS_ISA
#     define _MIPS_ISA 7
#   endif
# elif defined(__mips__)
#   undef  ARCH_MIPS
#   define ARCH_MIPS 1
#   define PLATFORM_DESCRIPTION "Android MIPS"
#   ifndef _MIPS_ISA
#     define _MIPS_ISA 6
#   endif
# else
#   error Unknown architecture
# endif
# if ARCH_MIPS
#   if defined(__MIPSEL__) || defined(__MIPSEL) || defined(_MIPSEL)
#     undef  ARCH_ENDIAN_LITTLE
#     define ARCH_ENDIAN_LITTLE 1
#   else
#     undef  ARCH_ENDIAN_BIG
#     define ARCH_ENDIAN_BIG 1
#   endif
# elif defined(__AARCH64EB__) || defined(__ARMEB__)
#   undef  ARCH_ENDIAN_BIG
#   define ARCH_ENDIAN_BIG 1
# else
#   undef  ARCH_ENDIAN_LITTLE
#   define ARCH_ENDIAN_LITTLE 1
# endif
# undef  PLATFORM_MOBILE
# define PLATFORM_MOBILE 1
# undef  PLATFORM_CONSOLE
# define PLATFORM_CONSOLE 1
#elif defined(__TIZEN__)
# undef  PLATFORM_TIZEN
# define PLATFORM_TIZEN 1
# undef  PLATFORM_POSIX
# define PLATFORM_POSIX 1
# define PLATFORM_NAME "Tizen"
# if defined(__arm__)
#   undef  ARCH_ARM
#   define ARCH_ARM 1
#   ifdef __ARM_ARCH_7A__
#     undef  ARCH_ARM7
#     define ARCH_ARM7 1
#     define PLATFORM_DESCRIPTION "Tizen ARMv7"
#   elif defined(__ARM_ARCH_5TE__)
#     undef  ARCH_ARM5
#     define ARCH_ARM5 1
#     define PLATFORM_DESCRIPTION "Tizen ARMv5"
#   else
#     error Unsupported ARM architecture
#   endif
# elif defined(__i386__)
#   undef  ARCH_X86
#   define ARCH_X86 1
#   define PLATFORM_DESCRIPTION "Tizen x86"
# elif defined(__x86_64__)
#   undef  ARCH_X86_64
#   define ARCH_X86_64 1
#   define PLATFORM_DESCRIPTION "Tizen x86-64"
# else
#   error Unknown architecture
# endif
# if defined(__AARCH64EB__) || defined(__ARMEB__)
#   undef  ARCH_ENDIAN_BIG
#   define ARCH_ENDIAN_BIG 1
# else
#   undef  ARCH_ENDIAN_LITTLE
#   define ARCH_ENDIAN_LITTLE 1
# endif
# undef  PLATFORM_MOBILE
# define PLATFORM_MOBILE 1
# undef  PLATFORM_CONSOLE
# define PLATFORM_CONSOLE 1
#elif defined(__APPLE__) && __APPLE__
# undef  PLATFORM_APPLE
# define PLATFORM_APPLE 1
# undef  PLATFORM_POSIX
# define PLATFORM_POSIX 1
# include <TargetConditionals.h>
# if defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
#   undef  PLATFORM_IOS
#   define PLATFORM_IOS 1
#   define PLATFORM_NAME "iOS"
#   if defined(__arm__)
#     undef  ARCH_ARM
#     define ARCH_ARM 1
#     if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7S__)
#       undef  ARCH_ARM7
#       define ARCH_ARM7 1
#       define PLATFORM_DESCRIPTION "iOS ARMv7"
#       ifndef __ARM_NEON__
#         error Missing ARM NEON support
#       endif
#     elif defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6__)
#       undef  ARCH_ARM6
#       define ARCH_ARM6 1
#       define PLATFORM_DESCRIPTION "iOS ARMv6"
#     else
#       error Unrecognized ARM architecture
#     endif
#   elif defined(__arm64__)
#     undef  ARCH_ARM
#     define ARCH_ARM 1
#     undef  ARCH_ARM_64
#     define ARCH_ARM_64 1
#     if defined(__ARM64_ARCH_8__)
#       undef  ARCH_ARM8_64
#       define ARCH_ARM8_64 1
#       define PLATFORM_DESCRIPTION "iOS ARM64v8"
#     else
#       error Unrecognized ARM architecture
#     endif
#   elif defined(__i386__)
#     undef  PLATFORM_IOS_SIMULATOR
#     define PLATFORM_IOS_SIMULATOR 1
#     undef  ARCH_X86
#     define ARCH_X86 1
#     define PLATFORM_DESCRIPTION "iOS x86 (simulator)"
#   elif defined(__x86_64__)
#     undef  PLATFORM_IOS_SIMULATOR
#     define PLATFORM_IOS_SIMULATOR 1
#     undef  ARCH_X86_64
#     define ARCH_X86_64 1
#     define PLATFORM_DESCRIPTION "iOS x86_64 (simulator)"
#   else
#     error Unknown architecture
#   endif
#   undef  ARCH_ENDIAN_LITTLE
#   define ARCH_ENDIAN_LITTLE 1
#   undef  PLATFORM_MOBILE
#   define PLATFORM_MOBILE 1
#   undef  PLATFORM_CONSOLE
#   define PLATFORM_CONSOLE 1
# elif defined(__MACH__)
#   undef  PLATFORM_MACOSX
#   define PLATFORM_MACOSX 1
#   define PLATFORM_NAME "MacOSX"
#   if defined(__x86_64__) ||  defined(__x86_64) || defined(__amd64)
#     undef  ARCH_X86_64
#     define ARCH_X86_64 1
#     undef  ARCH_ENDIAN_LITTLE
#     define ARCH_ENDIAN_LITTLE 1
#     define PLATFORM_DESCRIPTION "MacOSX x86-64"
#   elif defined(__i386__) || defined(__intel__)
#     undef  ARCH_X86
#     define ARCH_X86 1
#     undef  ARCH_ENDIAN_LITTLE
#     define ARCH_ENDIAN_LITTLE 1
#     define PLATFORM_DESCRIPTION "MacOSX x86"
#   elif defined(__powerpc64__) || defined(__POWERPC64__)
#     undef  ARCH_PPC_64
#     define ARCH_PPC_64 1
#     undef  ARCH_ENDIAN_BIG
#     define ARCH_ENDIAN_BIG 1
#     define PLATFORM_DESCRIPTION "MacOSX PPC64"
#   elif defined(__powerpc__) || defined(__POWERPC__)
#     undef  ARCH_PPC
#     define ARCH_PPC 1
#     undef  ARCH_ENDIAN_BIG
#     define ARCH_ENDIAN_BIG 1
#     define PLATFORM_DESCRIPTION "MacOSX PPC"
#   else
#     error Unknown architecture
#   endif
#   undef  PLATFORM_DESKTOP
#   define PLATFORM_DESKTOP 1
# else
#   error Unknown Apple Platform
# endif
#elif defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
# undef  PLATFORM_WINDOWS
# define PLATFORM_WINDOWS 1
# define PLATFORM_NAME "Windows"
# if defined(__x86_64__) || defined(_M_AMD64) || defined(_AMD64_)
#   undef  ARCH_X86_64
#   define ARCH_X86_64 1
#   define PLATFORM_DESCRIPTION "Windows x86-64"
# elif defined(__x86__) || defined(_M_IX86) || defined(_X86_)
#   undef  ARCH_X86
#   define ARCH_X86 1
#   define PLATFORM_DESCRIPTION "Windows x86"
# elif defined(__ia64__) || defined(_M_IA64) || defined(_IA64_)
#   undef  ARCH_IA64
#   define ARCH_IA64 1
#   define PLATFORM_DESCRIPTION "Windows IA-64"
# elif defined(_M_ARM)
#   undef  ARCH_ARM
#   define ARCH_ARM 1
#   if _M_ARM == 5
#     undef  ARCH_ARM5
#     define ARCH_ARM5 1
#     define PLATFORM_DESCRIPTION "Windows ARMv5"
#   elif _M_ARM == 6
#     undef  ARCH_ARM6
#     define ARCH_ARM6 1
#     define PLATFORM_DESCRIPTION "Windows ARMv6"
#   elif _M_ARM == 7
#     undef  ARCH_ARM7
#     define ARCH_ARM7 1
#     define PLATFORM_DESCRIPTION "Windows ARMv7"
#   else
#     error Unknown ARM architecture
#   endif
# else
#   error Unknown architecture
# endif
# undef  ARCH_ENDIAN_LITTLE
# define ARCH_ENDIAN_LITTLE 1
# undef  PLATFORM_DESKTOP
# define PLATFORM_DESKTOP 1
#elif defined(__BSD__) || defined(__FreeBSD__)
# undef  PLATFORM_BSD
# define PLATFORM_BSD 1
# undef  PLATFORM_POSIX
# define PLATFORM_POSIX 1
# define PLATFORM_NAME "BSD"
# if defined(__x86_64__) || defined(__x86_64) || defined(__amd64)
#   undef  ARCH_X86_64
#   define ARCH_X86_64 1
#   undef  ARCH_ENDIAN_LITTLE
#   define ARCH_ENDIAN_LITTLE 1
#   define PLATFORM_DESCRIPTION "BSD x86-64"
# elif defined(__i386__) || defined(__intel__) || defined(_M_IX86)
#   undef  ARCH_X86
#   define ARCH_X86 1
#   undef  ARCH_ENDIAN_LITTLE
#   define ARCH_ENDIAN_LITTLE 1
#   define PLATFORM_DESCRIPTION "BSD x86"
# elif defined(__powerpc64__) || defined(__POWERPC64__)
#   undef  ARCH_PPC_64
#   define ARCH_PPC_64 1
#   undef  ARCH_ENDIAN_BIG
#   define ARCH_ENDIAN_BIG 1
#   define PLATFORM_DESCRIPTION "BSD PPC64"
# elif defined(__powerpc__) || defined(__POWERPC__)
#   undef  ARCH_PPC
#   define ARCH_PPC 1
#   undef  ARCH_ENDIAN_BIG
#   define ARCH_ENDIAN_BIG 1
#   define PLATFORM_DESCRIPTION "BSD PPC"
# else
#   error Unknown architecture
# endif
# undef  PLATFORM_DESKTOP
# define PLATFORM_DESKTOP 1
#elif defined(__linux__) || defined(__linux)
# undef  PLATFORM_LINUX
# define PLATFORM_LINUX 1
# undef  PLATFORM_POSIX
# define PLATFORM_POSIX 1
# define PLATFORM_NAME "Linux"
# if defined(__x86_64__) || defined(__x86_64) || defined(__amd64)
#   undef  ARCH_X86_64
#   define ARCH_X86_64 1
#   undef  ARCH_ENDIAN_LITTLE
#   define ARCH_ENDIAN_LITTLE 1
#   define PLATFORM_DESCRIPTION "Linux x86-64"
# elif defined(__i386__) || defined(__intel__) || defined(_M_IX86)
#   undef  ARCH_X86
#   define ARCH_X86 1
#   undef  ARCH_ENDIAN_LITTLE
#   define ARCH_ENDIAN_LITTLE 1
#   define PLATFORM_DESCRIPTION "Linux x86"
# elif defined(__powerpc64__) || defined(__POWERPC64__)
#   undef  ARCH_PPC_64
#   define ARCH_PPC_64 1
#   undef  ARCH_ENDIAN_BIG
#   define ARCH_ENDIAN_BIG 1
#   define PLATFORM_DESCRIPTION "Linux PPC64"
# elif defined(__powerpc__) || defined(__POWERPC__)
#   undef  ARCH_PPC
#   define ARCH_PPC 1
#   undef  ARCH_ENDIAN_BIG
#   define ARCH_ENDIAN_BIG 1
#   define PLATFORM_DESCRIPTION "Linux PPC"
# elif defined(__arm__)
#   undef  ARCH_ARM
#   define ARCH_ARM 1
#   if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7S__)
#     undef  ARCH_ARM7
#     define ARCH_ARM7 1
#     define PLATFORM_DESCRIPTION "Linux ARMv7"
#     ifndef __ARM_NEON__
#       error Missing ARM NEON support
#     endif
#   elif defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6ZK__)
#     undef  ARCH_ARM6
#     define ARCH_ARM6 1
#     define PLATFORM_DESCRIPTION "Linux ARMv6"
#   else
#     error Unrecognized ARM architecture
#   endif
#   if defined(__ARMEB__)
#     undef  ARCH_ENDIAN_BIG
#     define ARCH_ENDIAN_BIG 1
#   else
#     undef  ARCH_ENDIAN_LITTLE
#     define ARCH_ENDIAN_LITTLE 1
#   endif
# elif defined(__arm64__) || defined(__aarch64__)
#   undef  ARCH_ARM
#   define ARCH_ARM 1
#   undef  ARCH_ARM_64
#   define ARCH_ARM_64 1
#   undef  ARCH_ENDIAN_LITTLE
#   define ARCH_ENDIAN_LITTLE 1
#   if defined(__ARM64_ARCH_8__)
#     undef  ARCH_ARM8_64
#     define ARCH_ARM8_64 1
#     define PLATFORM_DESCRIPTION "Linux ARM64v8"
#   else
#     error Unrecognized ARM architecture
#   endif
#   if defined(__AARCH64EB__)
#     undef  ARCH_ENDIAN_BIG
#     define ARCH_ENDIAN_BIG 1
#   else
#     undef  ARCH_ENDIAN_LITTLE
#     define ARCH_ENDIAN_LITTLE 1
#   endif
# else
#   error Unknown architecture
# endif
# if defined(__raspberrypi__)
#   undef  PLATFORM_LINUX_RASPBERRYPI
#   define PLATFORM_LINUX_RASPBERRYPI 1
# endif
# undef  PLATFORM_DESKTOP
# define PLATFORM_DESKTOP 1
#else
# error Unknown platform
#endif

#if U_BUILD_DYNAMIC_LINK && PLATFORM_WINDOWS
# define EXPORT_LINK __declspec(dllexport)
# define IMPORT_LINK __declspec(dllimport)
#else
# define EXPORT_LINK __attribute__((visibility("default")))
# define IMPORT_LINK
#endif
#if U_COMPILE
# ifdef __cplusplus
#   define U_EXT extern "C" IMPORT_LINK
#   define U_API extern "C" EXPORT_LINK
# else
#   define U_EXT extern IMPORT_LINK
#   define U_API extern EXPORT_LINK
# endif
#else
# ifdef __cplusplus
#   define U_EXT extern "C" IMPORT_LINK
#   define U_API extern "C" IMPORT_LINK
# else
#   define U_EXT extern IMPORT_LINK
#   define U_API extern IMPORT_LINK
# endif
#endif

#if defined(__SSE2__) || ARCH_X86_64
# undef  ARCH_SSE2
# define ARCH_SSE2 1
#endif
#ifdef __SSE3__
# undef  ARCH_SSE3
# define ARCH_SSE3 1
#endif
#ifdef __SSE4_1__
# undef  ARCH_SSE4
# define ARCH_SSE4 1
#endif
#ifdef __ARM_NEON__
# undef  ARCH_NEON
# define ARCH_NEON 1
#endif
#ifdef __thumb__
# undef  ARCH_THUMB
# define ARCH_THUMB 1
#endif

#endif /* U_PLATFORM_H__ */
