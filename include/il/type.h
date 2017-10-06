/*
 * libil - Intermediate Language cross-platform c library
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

#ifndef   IL_TYPE_H__
# define  IL_TYPE_H__

#include "ir.h"

#define il_type_undefined() ((il_type_t) {.kind = IL_TYPE_UNDEFINED})
#define il_type_undef(tptr) (*(tptr) = il_type_undefined())

#define il_type_specified(t, by) ((t).specifiers & IL_TYPE_SPECIFIER_ ## by)
#define il_type_qualified(t, by) ((t).qualifiers & IL_TYPE_QUALIFIER_ ## by)

enum {
  IL_TYPE_MERGE_ERROR_NONE = 0,
  IL_TYPE_MERGE_ERROR_TWO_OR_MORE_DATA_TYPE,
  IL_TYPE_MERGE_ERROR_BOTH_LONG_AND,
  IL_TYPE_MERGE_ERROR_BOTH_LONG_LONG_AND
};

il_type_t il_void(void);
il_type_t il_bool(void);
il_type_t il_char(void);
il_type_t il_short(void);
il_type_t il_int(void);
il_type_t il_uint(void);
il_type_t il_long(void);
il_type_t il_ulong(void);
il_type_t il_double(void);
il_type_t il_float(void);
il_type_t il_long_long(void);
il_type_t il_long_double(void);
il_type_t il_pointer(il_type_t of);
il_type_t il_array(il_type_t of, il_expr_t size);
il_type_t il_compound(il_entity_t entity);

void il_type_dtor(il_type_t *self);
bool il_type_is_ref(il_type_t type);
bool il_type_is_func(il_type_t type);
bool il_type_equals(il_type_t a, il_type_t b);
il_type_t il_type_deref(il_type_t a);
il_entity_r il_type_fields(il_type_t self);
il_field_t *il_field_lookup(il_type_t self, __const char *name);
unsigned il_type_merge(il_type_t *self, enum il_type_n with);

#endif /* IL_TYPE_H__ */
