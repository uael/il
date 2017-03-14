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

#ifndef   JL_DEQUE_H__
# define  JL_DEQUE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <adt/xmalloc.h>

#define jl_roundup32(x) \
  (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

#define jl_deque_of(t) struct { \
    size_t size, capacity, cursor; t *data; \
  }

#define jl_deque_size(v) (v).size

#define jl_deque_capacity(v) (v).capacity

#define jl_deque_cursor(v) (v).cursor

#define jl_deque_data(v) (v).data

#define jl_deque_length(v) (jl_deque_size(v)-jl_deque_cursor(v))

#define jl_deque_at(v, i) jl_deque_data(v)[jl_deque_cursor(v)+i]

#define jl_deque_front(v) jl_deque_at(v, 0)

#define jl_deque_back(v) jl_deque_at(v, jl_deque_size(v)-1)

#define jl_deque_dtor(v) do { \
    jl_deque_size(v) = jl_deque_capacity(v) = 0; \
    if (jl_deque_data(v)) free(jl_deque_data(v)); \
    jl_deque_data(v) = NULL; \
  } while (false)

#define jl_deque_resize(v, s) ( \
    jl_deque_capacity(v) = (s), \
    jl_deque_data(v) = (__typeof__(jl_deque_data(v))) xrealloc(jl_deque_data(v), sizeof(*jl_deque_data(v)) * jl_deque_capacity(v)) \
  )

#define jl_deque_growth(v) ( \
    jl_deque_size(v) == jl_deque_capacity(v) \
      ? ((jl_deque_capacity(v) = jl_deque_capacity(v) ? jl_deque_capacity(v) << 1 : 2), (v)) \
      : ((jl_deque_data(v) = (__typeof__(jl_deque_data(v))) xrealloc(jl_deque_data(v), sizeof(*jl_deque_data(v)) * jl_deque_capacity(v))), (v)) \
  )

#define jl_deque_grow(v, i) do { \
    size_t __s = (size_t) (i); \
    if (jl_deque_capacity(v) <= __s) { \
      jl_deque_capacity(v) = __s + 1; \
      jl_roundup32(jl_deque_capacity(v)); \
      jl_deque_data(v) = (__typeof__(jl_deque_data(v))) xrealloc( \
        jl_deque_data(v), sizeof(*jl_deque_data(v)) * jl_deque_capacity(v) \
      ); \
    } \
  } while (false)

#define jl_deque_push(v, x) do { \
    jl_deque_grow(v, jl_deque_size(v) + 1); \
    jl_deque_data(v)[jl_deque_size(v)++] = (x); \
  } while (false)

#define jl_deque_pop(v) jl_deque_data(v)[--jl_deque_size(v)]

#define jl_deque_unshift(v, x) do { \
    jl_deque_grow(v, jl_deque_size(v) + 1); \
    memmove( \
      jl_deque_data(v) + 1 + jl_deque_cursor(v), \
      jl_deque_data(v) + jl_deque_cursor(v), \
      jl_deque_size(v) * sizeof(*jl_deque_data(v)) \
    ); \
    jl_deque_size(v) += 1; \
    jl_deque_data(v)[jl_deque_cursor(v)] = (x); \
  } while (false)

#define jl_deque_shift(v) jl_deque_data(v)[jl_deque_cursor(v)++]

#define jl_deque_foreach(v, var) \
    for (size_t __k = 1, __i = 0; __k && __i != jl_deque_length(v); __k = !__k, __i++) \
      for (var = *(jl_deque_data(v)+__i+jl_deque_cursor(v)); __k; __k = !__k)

#endif /* JL_DEQUE_H__ */
