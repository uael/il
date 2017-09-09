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

/*!@file unic/types.h
 * @author uael
 */
#ifndef __UNIC_TYPES_H
# define __UNIC_TYPES_H

#include <string.h>

#define nil ((void *) 0)

typedef char i8_t;
typedef unsigned char u8_t;
typedef short i16_t;
typedef unsigned short u16_t;
typedef int i32_t;
typedef unsigned int u32_t;
typedef long long i64_t;
typedef unsigned long long u64_t;
typedef float f32_t;
typedef double f64_t;

#ifndef __cplusplus
# if defined CC_MSVC && CC_MSVC < 1900
#   define bool unsigned char
#   define true 1
#   define false 0
#   define __bool_true_false_are_defined 1
# else
#   include <stdbool.h>
# endif
#endif

typedef bool bool_t;

#define I8_MIN (-128)
#define I16_MIN (-32768)
#define I32_MIN (-2147483647 - 1)
#define I64_MIN (-9223372036854775807LL - 1)

#define I8_MAX 127
#define I16_MAX 32767
#define I32_MAX 2147483647
#define I64_MAX 9223372036854775807LL

#define U8_MAX 0xffU
#define U16_MAX 0xffffU
#define U32_MAX 0xffffffffUL
#define U64_MAX 0xffffffffffffffffULL

#define i8eq(a, b) ((bool_t)((i8_t)(a) == (i8_t)(b)))
#define u8eq(a, b) ((bool_t)((u8_t)(a) == (u8_t)(b)))
#define i16eq(a, b) ((bool_t)((i16_t)(a) == (i16_t)(b)))
#define u16eq(a, b) ((bool_t)((u16_t)(a) == (u16_t)(b)))
#define i32eq(a, b) ((bool_t)((i32_t)(a) == (i32_t)(b)))
#define u32eq(a, b) ((bool_t)((u32_t)(a) == (u32_t)(b)))
#define i64eq(a, b) ((bool_t)((i64_t)(a) == (i64_t)(b)))
#define u64eq(a, b) ((bool_t)((u64_t)(a) == (u64_t)(b)))
#define f32eq(a, b) ((bool_t)((f32_t)(a) == (f32_t)(b)))
#define f64eq(a, b) ((bool_t)((f64_t)(a) == (f64_t)(b)))
#define streq(a, b) ((bool_t)(strcmp(a, b) == 0))

#define i8cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u8cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i16cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u16cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define f32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define f64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))

#endif /* !__UNIC_TYPES_H */
