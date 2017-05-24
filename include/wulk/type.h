/*
 * Wulk - Wu uniform language kit
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#ifndef   WULK_TYPE_H__
# define  WULK_TYPE_H__

#include "ir.h"

#define wulk_type_undefined() ((wulk_type_t) {.kind = WULK_TYPE_UNDEFINED})
#define wulk_type_undef(tptr) (*(tptr) = wulk_type_undefined())

#define wulk_type_specified(t, by) ((t).specifiers & WULK_TYPE_SPECIFIER_ ## by)
#define wulk_type_qualified(t, by) ((t).qualifiers & WULK_TYPE_QUALIFIER_ ## by)

enum {
  WULK_TYPE_MERGE_ERROR_NONE = 0,
  WULK_TYPE_MERGE_ERROR_TWO_OR_MORE_DATA_TYPE,
  WULK_TYPE_MERGE_ERROR_BOTH_LONG_AND,
  WULK_TYPE_MERGE_ERROR_BOTH_LONG_LONG_AND
};

wulk_type_t wulk_void(void);
wulk_type_t wulk_bool(void);
wulk_type_t wulk_char(void);
wulk_type_t wulk_short(void);
wulk_type_t wulk_int(void);
wulk_type_t wulk_uint(void);
wulk_type_t wulk_long(void);
wulk_type_t wulk_ulong(void);
wulk_type_t wulk_double(void);
wulk_type_t wulk_float(void);
wulk_type_t wulk_long_long(void);
wulk_type_t wulk_long_double(void);
wulk_type_t wulk_pointer(wulk_type_t of);
wulk_type_t wulk_array(wulk_type_t of, wulk_expr_t size);
wulk_type_t wulk_compound(wulk_entity_t entity);

void wulk_type_dtor(wulk_type_t *self);
bool wulk_type_is_ref(wulk_type_t type);
bool wulk_type_is_func(wulk_type_t type);
bool wulk_type_equals(wulk_type_t a, wulk_type_t b);
wulk_type_t wulk_type_deref(wulk_type_t a);
wulk_entity_r wulk_type_fields(wulk_type_t self);
wulk_field_t *wulk_field_lookup(wulk_type_t self, const char *name);
unsigned wulk_type_merge(wulk_type_t *self, enum wulk_type_n with);

#endif /* WULK_TYPE_H__ */
