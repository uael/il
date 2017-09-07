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

/*!@file unic/config.h
 * @author uael
 */
#ifndef __UNIC_CONFIG_H
# define __UNIC_CONFIG_H

#include "cc.h"

#if UNIC_BUILD_DYNAMIC_LINK && defined CC_MSVC
# define U_EXPORT_LINK __declspec(dllexport)
# define U_IMPORT_LINK __declspec(dllimport)
#elif HAS_ATTRIBUTE(visibility)
# define U_EXPORT_LINK __attribute__((visibility("default")))
# define U_IMPORT_LINK
#else
# define U_EXPORT_LINK
# define U_IMPORT_LINK
#endif
#if UNIC_COMPILE
# ifdef __cplusplus
#   define U_EXT extern "C" U_IMPORT_LINK
#   define U_API extern "C" U_EXPORT_LINK
# else
#   define U_EXT extern U_IMPORT_LINK
#   define U_API extern U_EXPORT_LINK
# endif
#else
# ifdef __cplusplus
#   define U_EXT extern "C" U_IMPORT_LINK
#   define U_API extern "C" U_IMPORT_LINK
# else
#   define U_EXT extern U_IMPORT_LINK
#   define U_API extern U_IMPORT_LINK
# endif
#endif

#endif /* !__UNIC_CONFIG_H */
