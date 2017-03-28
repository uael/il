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
#include <string.h>
#include <adt/bool.h>
#include <adt/xmalloc.h>

#define jl_roundup32(x) \
  (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

#define adt_vector_of(t) struct { \
    size_t size, capacity; t *data; \
  }

#define adt_vector_size(v) (v).size

#define adt_vector_capacity(v) (v).capacity

#define adt_vector_data(v) (v).data

#define adt_vector_length(v) adt_vector_size(v)

#define adt_vector_at(v, i) adt_vector_data(v)[i]

#define adt_vector_front(v) adt_vector_at(v, 0)

#define adt_vector_back(v) adt_vector_at(v, adt_vector_size(v)-1)

#define adt_vector_dtor(v) do { \
    adt_vector_size(v) = adt_vector_capacity(v) = 0; \
    if (adt_vector_data(v)) free(adt_vector_data(v)); \
    adt_vector_data(v) = NULL; \
  } while (false)

#define adt_vector_resize(v, s) ( \
    adt_vector_capacity(v) = (s), \
    adt_vector_data(v) = xrealloc(adt_vector_data(v), sizeof(*adt_vector_data(v)) * adt_vector_capacity(v)) \
  )

#define adt_vector_growth(v) ( \
    adt_vector_size(v) == adt_vector_capacity(v) \
      ? ((adt_vector_capacity(v) = adt_vector_capacity(v) ? adt_vector_capacity(v) << 1 : 2), (v)) \
      : ((adt_vector_data(v) = xrealloc(adt_vector_data(v), sizeof(*adt_vector_data(v)) * adt_vector_capacity(v))), (v)) \
  )

#define adt_vector_grow(v, i) do { \
    size_t __s = (size_t) (i); \
    if (adt_vector_capacity(v) <= __s) { \
      adt_vector_capacity(v) = __s + 1; \
      jl_roundup32(adt_vector_capacity(v)); \
      adt_vector_data(v) = xrealloc( \
        adt_vector_data(v), sizeof(*adt_vector_data(v)) * adt_vector_capacity(v) \
      ); \
    } \
  } while (false)

#define adt_vector_push(v, x) do { \
    adt_vector_grow(v, adt_vector_size(v) + 1); \
    adt_vector_data(v)[adt_vector_size(v)++] = (x); \
  } while (false)

#define adt_vector_pop(v) adt_vector_data(v)[--adt_vector_size(v)]

#define adt_vector_unshift(v, x) do { \
    adt_vector_grow(v, adt_vector_size(v) + 1); \
    memmove( \
      adt_vector_data(v) + 1, \
      adt_vector_data(v), \
      adt_vector_size(v) * sizeof(*adt_vector_data(v)) \
    ); \
    adt_vector_size(v) += 1; \
    adt_vector_data(v)[0] = (x); \
  } while (false)

#define adt_vector_shift(v) \
  memmove(adt_vector_data(v), adt_vector_data(v) + 1, --adt_vector_size(v) * sizeof(*adt_vector_data(v)))

#define adt_vector_clear(v) do { \
    memset(adt_vector_data(v), 0, adt_vector_capacity(v)); \
    adt_vector_size(v) = 0; \
  } while (false)

#define adt_vector_foreach(v, var) \
    for (size_t __k = 1, __i = 0; __k && __i != adt_vector_length(v); __k = !__k, __i++) \
      for (var = *(adt_vector_data(v)+__i); __k; __k = !__k)

#endif /* JL_VECTOR_H__ */
