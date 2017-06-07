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

/*!\file math.h
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_MATH_H__
# define U_MATH_H__

#include "types.h"

/*!@def ISPOW2
 * @brief Check if @n is a power of 2.
 * @param n The number to check.
 */
#define ISPOW2(n) (((n) & -(n)) == (n))

FORCEINLINE CONSTCALL bool ispow2(const long n) {
  return ISPOW2(n);
}

#define ROUNDUP32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

FORCEINLINE CONSTCALL size_t roundup32(size_t n) {
  size_t j;
  return ISPOW2(n) ? n : (
    (void) ((j = n & 0xFFFF0000) || (j = n)),
    (void) ((n = j & 0xFF00FF00) || (n = j)),
    (void) ((j = n & 0xF0F0F0F0) || (j = n)),
    (void) ((n = j & 0xCCCCCCCC) || (n = j)),
    (void) ((j = n & 0xAAAAAAAA) || (j = n)),
    j << 1
  );
}

#endif /* U_MATH_H__ */
