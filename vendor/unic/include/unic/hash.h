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

/*!@file unic/hash.h
 * @author uael
 */
#ifndef __UNIC_HASH_H
# define __UNIC_HASH_H

#include <string.h>

#include "types.h"
#include "config.h"

#define i8_hash(key) ((u32_t)(key))
#define u8_hash(key) ((u32_t)(key))
#define i16_hash(key) ((u32_t)(((u64_t)(key) * 1299709 + 104729) % 323780508946331ULL))
#define u16_hash(key) ((u32_t)(((u64_t)(key) * 1299709 + 104729) % 323780508946331ULL))
#define i32_hash(key) ((u32_t)(((u64_t)(key) * 1299709 + 104729) % 323780508946331ULL))
#define u32_hash(key) ((u32_t)(((u64_t)(key) * 1299709 + 104729) % 323780508946331ULL))
#define i64_hash(key) ((u32_t)(((u64_t)(key) * 1299709 + 104729) % 323780508946331ULL))
#define u64_hash(key) ((u32_t)(((u64_t)(key) * 1299709 + 104729) % 323780508946331ULL))
#define i8_eq(a, b) ((bool_t)((i8_t)(a) == (i8_t)(b)))
#define u8_eq(a, b) ((bool_t)((u8_t)(a) == (u8_t)(b)))
#define i16_eq(a, b) ((bool_t)((i16_t)(a) == (i16_t)(b)))
#define u16_eq(a, b) ((bool_t)((u16_t)(a) == (u16_t)(b)))
#define i32_eq(a, b) ((bool_t)((i32_t)(a) == (i32_t)(b)))
#define u32_eq(a, b) ((bool_t)((u32_t)(a) == (u32_t)(b)))
#define i64_eq(a, b) ((bool_t)((i64_t)(a) == (i64_t)(b)))
#define u64_eq(a, b) ((bool_t)((u64_t)(a) == (u64_t)(b)))
#define str_eq(a, b) ((bool_t)(strcmp(a, b) == 0))

static inline purecall constcall u32_t
str_hash(const i8_t *s) {
  u32_t h = (u32_t) *s;
  if (h) for (++s; *s; ++s) h = (h << 5) - h + (u32_t) *s;
  return h;
}

#endif /* !__UNIC_HASH_H */
