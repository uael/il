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

/*!@file unic/math.h
 * @author uael
 */
#ifndef __UNIC_MATH_H
# define __UNIC_MATH_H

#include "types.h"
#include "config.h"

#define ISPOW2(n) (((n) & -(n)) == (n))

static inline purecall constcall u32_t
pow2_next32(i32_t n) {
  i32_t j;

  return (u32_t) (
    ISPOW2(n) ? n : (
      (void) ((j = n & 0xFFFF0000) || (j = n)),
        (void) ((n = j & 0xFF00FF00) || (n = j)),
        (void) ((j = n & 0xF0F0F0F0) || (j = n)),
        (void) ((n = j & 0xCCCCCCCC) || (n = j)),
        (void) ((j = n & 0xAAAAAAAA) || (j = n)),
        j << 1
    )
  );
}

#endif /* !__UNIC_MATH_H */
