/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef   WULK_VECTOR_H__
# define  WULK_VECTOR_H__

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "adt/bool.h"
#include "adt/xmalloc.h"

#define wulk_roundup32(x) \
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
    if (adt_vector_data(v)) xfree(adt_vector_data(v)); \
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

#define adt_vector_erase(v, i) \
  memmove(adt_vector_data(v) + i, adt_vector_data(v) + i + 1, --adt_vector_size(v) * sizeof(*adt_vector_data(v)))

#define adt_vector_grow(v, i) do { \
    size_t __s = (size_t) (i); \
    if (adt_vector_capacity(v) <= __s) { \
      adt_vector_capacity(v) = __s + 1; \
      wulk_roundup32(adt_vector_capacity(v)); \
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
  adt_vector_erase(v, 0)

#define adt_vector_clear(v) do { \
    memset(adt_vector_data(v), 0, adt_vector_capacity(v)); \
    adt_vector_size(v) = 0; \
  } while (false)

#define adt_vector_foreach(v, var) \
    for (size_t __k = 1, __i = 0; __k && __i != adt_vector_length(v); __k = !__k, __i++) \
      for (var = *(adt_vector_data(v)+__i); __k; __k = !__k)

#endif /* WULK_VECTOR_H__ */
