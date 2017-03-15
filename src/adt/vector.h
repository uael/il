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

#ifndef   JL_VECTOR_H__
# define  JL_VECTOR_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <adt/xmalloc.h>

#define jl_roundup32(x) \
  (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

#define jl_vector_of(t) struct { \
    size_t size, capacity; t *data; \
  }

#define jl_vector_size(v) (v).size

#define jl_vector_capacity(v) (v).capacity

#define jl_vector_data(v) (v).data

#define jl_vector_length(v) jl_vector_size(v)

#define jl_vector_at(v, i) jl_vector_data(v)[i]

#define jl_vector_front(v) jl_vector_at(v, 0)

#define jl_vector_back(v) jl_vector_at(v, jl_vector_size(v)-1)

#define jl_vector_dtor(v) do { \
    jl_vector_size(v) = jl_vector_capacity(v) = 0; \
    if (jl_vector_data(v)) free(jl_vector_data(v)); \
    jl_vector_data(v) = NULL; \
  } while (false)

#define jl_vector_resize(v, s) ( \
    jl_vector_capacity(v) = (s), \
    jl_vector_data(v) = (__typeof__(jl_vector_data(v))) xrealloc(jl_vector_data(v), sizeof(*jl_vector_data(v)) * jl_vector_capacity(v)) \
  )

#define jl_vector_growth(v) ( \
    jl_vector_size(v) == jl_vector_capacity(v) \
      ? ((jl_vector_capacity(v) = jl_vector_capacity(v) ? jl_vector_capacity(v) << 1 : 2), (v)) \
      : ((jl_vector_data(v) = (__typeof__(jl_vector_data(v))) xrealloc(jl_vector_data(v), sizeof(*jl_vector_data(v)) * jl_vector_capacity(v))), (v)) \
  )

#define jl_vector_grow(v, i) do { \
    size_t __s = (size_t) (i); \
    if (jl_vector_capacity(v) <= __s) { \
      jl_vector_capacity(v) = __s + 1; \
      jl_roundup32(jl_vector_capacity(v)); \
      jl_vector_data(v) = (__typeof__(jl_vector_data(v))) xrealloc( \
        jl_vector_data(v), sizeof(*jl_vector_data(v)) * jl_vector_capacity(v) \
      ); \
    } \
  } while (false)

#define jl_vector_push(v, x) do { \
    jl_vector_grow(v, jl_vector_size(v) + 1); \
    jl_vector_data(v)[jl_vector_size(v)++] = (x); \
  } while (false)

#define jl_vector_pop(v) jl_vector_data(v)[--jl_vector_size(v)]

#define jl_vector_unshift(v, x) do { \
    jl_vector_grow(v, jl_vector_size(v) + 1); \
    memmove( \
      jl_vector_data(v) + 1, \
      jl_vector_data(v), \
      jl_vector_size(v) * sizeof(*jl_vector_data(v)) \
    ); \
    jl_vector_size(v) += 1; \
    jl_vector_data(v)[0] = (x); \
  } while (false)

#define jl_vector_shift(v) __extension__ ({ \
    __typeof__(*jl_vector_data(v)) __r = jl_vector_front(v); \
    memmove(jl_vector_data(v), jl_vector_data(v) + 1, --jl_vector_size(v) * sizeof(*jl_vector_data(v))); \
    __r; \
  })

#define jl_vector_clear(v) do { \
    memset(jl_vector_data(v), 0, jl_vector_capacity(v)); \
    jl_vector_size(v) = jl_vector_capacity(v) = 0; \
  } while (false)

#define jl_vector_foreach(v, var) \
    for (size_t __k = 1, __i = 0; __k && __i != jl_vector_length(v); __k = !__k, __i++) \
      for (var = *(jl_vector_data(v)+__i); __k; __k = !__k)

#endif /* JL_VECTOR_H__ */
