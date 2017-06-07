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

/*!\file deque.h
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_DEQUE_H__
# define U_DEQUE_H__

#include "buffer.h"

#define jl_roundup32(x) \
  (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

#define adt_deque_of(T) struct { \
    ds_super(T); size_t cursor; \
  }

#define adt_deque_size(v) (v).size

#define adt_deque_capacity(v) (v).cap

#define adt_deque_cursor(v) (v).cursor

#define adt_deque_data(v) (v).data

#define adt_deque_length(v) (adt_deque_size(v)-adt_deque_cursor(v))

#define adt_deque_at(v, i) adt_deque_data(v)[adt_deque_cursor(v)+i]

#define adt_deque_front(v) adt_deque_at(v, 0)

#define adt_deque_back(v) adt_deque_at(v, adt_deque_size(v)-1)

#define adt_deque_dtor(v) do { \
    adt_deque_size(v) = adt_deque_capacity(v) = adt_deque_cursor(v) = 0; \
    if (adt_deque_data(v)) free(adt_deque_data(v)); \
    adt_deque_data(v) = NULL; \
  } while (false)

#define adt_deque_resize(v, s) ( \
    adt_deque_capacity(v) = (s), \
    adt_deque_data(v) = realloc(adt_deque_data(v), sizeof(*adt_deque_data(v)) * adt_deque_capacity(v)) \
  )

#define adt_deque_growth(v) ( \
    adt_deque_size(v) == adt_deque_capacity(v) \
      ? ((adt_deque_capacity(v) = adt_deque_capacity(v) ? adt_deque_capacity(v) << 1 : 2), (v)) \
      : ((adt_deque_data(v) = realloc(adt_deque_data(v), sizeof(*adt_deque_data(v)) * adt_deque_capacity(v))), (v)) \
  )

#define adt_deque_grow(v, i) do { \
    size_t __s = (size_t) (i); \
    if (adt_deque_capacity(v) <= __s) { \
      adt_deque_capacity(v) = __s + 1; \
      jl_roundup32(adt_deque_capacity(v)); \
      adt_deque_data(v) = realloc( \
        adt_deque_data(v), sizeof(*adt_deque_data(v)) * adt_deque_capacity(v) \
      ); \
    } \
  } while (false)

#define adt_deque_push(v, x) do { \
    adt_deque_grow(v, adt_deque_size(v) + 1); \
    adt_deque_data(v)[adt_deque_size(v)++] = (x); \
  } while (false)

#define adt_deque_pop(v) adt_deque_data(v)[--adt_deque_size(v)]

#define adt_deque_unshift(v, x) do { \
    adt_deque_grow(v, adt_deque_size(v) + 1); \
    memmove( \
      adt_deque_data(v) + 1 + adt_deque_cursor(v), \
      adt_deque_data(v) + adt_deque_cursor(v), \
      adt_deque_size(v) * sizeof(*adt_deque_data(v)) \
    ); \
    adt_deque_size(v) += 1; \
    adt_deque_data(v)[adt_deque_cursor(v)] = (x); \
  } while (false)

#define adt_deque_shift(v) adt_deque_data(v)[adt_deque_cursor(v)++]

#define adt_deque_clear(v) do { \
    memset(adt_deque_data(v), 0, adt_deque_capacity(v)); \
    adt_deque_size(v) = adt_deque_cursor(v) = 0; \
  } while (false)

#define adt_deque_foreach(v, var) \
    for (size_t __k = 1, __i = 0; __k && __i != adt_deque_length(v); __k = !__k, __i++) \
      for (var = *(adt_deque_data(v)+__i+adt_deque_cursor(v)); __k; __k = !__k)

#endif /* U_DEQUE_H__ */
