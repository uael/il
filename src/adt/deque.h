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

#ifndef   WULK_DEQUE_H__
# define  WULK_DEQUE_H__

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <adt/bool.h>
#include <adt/xmalloc.h>

#define wulk_roundup32(x) \
  (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

#define adt_deque_of(t) struct { \
    size_t size, capacity, cursor; t *data; \
  }

#define adt_deque_size(v) (v).size

#define adt_deque_capacity(v) (v).capacity

#define adt_deque_cursor(v) (v).cursor

#define adt_deque_data(v) (v).data

#define adt_deque_length(v) (adt_deque_size(v)-adt_deque_cursor(v))

#define adt_deque_at(v, i) adt_deque_data(v)[adt_deque_cursor(v)+i]

#define adt_deque_front(v) adt_deque_at(v, 0)

#define adt_deque_back(v) adt_deque_at(v, adt_deque_size(v)-1)

#define adt_deque_dtor(v) do { \
    adt_deque_size(v) = adt_deque_capacity(v) = adt_deque_cursor(v) = 0; \
    if (adt_deque_data(v)) xfree(adt_deque_data(v)); \
    adt_deque_data(v) = NULL; \
  } while (false)

#define adt_deque_resize(v, s) ( \
    adt_deque_capacity(v) = (s), \
    adt_deque_data(v) = xrealloc(adt_deque_data(v), sizeof(*adt_deque_data(v)) * adt_deque_capacity(v)) \
  )

#define adt_deque_growth(v) ( \
    adt_deque_size(v) == adt_deque_capacity(v) \
      ? ((adt_deque_capacity(v) = adt_deque_capacity(v) ? adt_deque_capacity(v) << 1 : 2), (v)) \
      : ((adt_deque_data(v) = xrealloc(adt_deque_data(v), sizeof(*adt_deque_data(v)) * adt_deque_capacity(v))), (v)) \
  )

#define adt_deque_grow(v, i) do { \
    size_t __s = (size_t) (i); \
    if (adt_deque_capacity(v) <= __s) { \
      adt_deque_capacity(v) = __s + 1; \
      wulk_roundup32(adt_deque_capacity(v)); \
      adt_deque_data(v) = xrealloc( \
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

#endif /* WULK_DEQUE_H__ */
