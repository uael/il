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

#ifndef   WULK_ENTITY_H__
# define  WULK_ENTITY_H__

#include "ir.h"

#define wulk_entity_undefined() ((wulk_entity_t) {.kind = WULK_ENTITY_UNDEFINED})
#define wulk_entity_undef(eptr) (*(eptr) = wulk_entity_undefined())

wulk_entity_t wulk_field(const char *name, wulk_type_t type);
wulk_entity_t wulk_var_int(const char *name, int d);
wulk_entity_t wulk_var_float(const char *name, float f);
wulk_entity_t wulk_var_string(const char *name, const char *s);
wulk_entity_t wulk_var(const char *name, wulk_type_t type, wulk_expr_t initializer);
wulk_entity_t wulk_param_int(unsigned position, const char *name, int d);
wulk_entity_t wulk_param_float(unsigned position, const char *name, float f);
wulk_entity_t wulk_param_string(unsigned position, const char *name, const char *s);
wulk_entity_t wulk_param(unsigned position, const char *name, wulk_type_t type, wulk_expr_t initializer);
wulk_entity_t wulk_func_decl(wulk_type_t return_type, const char *name, wulk_param_t *params);
wulk_entity_t wulk_proc_decl(const char *name, wulk_param_t *params);
wulk_entity_t wulk_func_def(wulk_func_t prototype, wulk_stmt_t body);
wulk_entity_t wulk_func(wulk_type_t return_type, const char *name, wulk_param_t *params, wulk_stmt_t body);
wulk_entity_t wulk_enum_anonymous(wulk_field_t *fields);
wulk_entity_t wulk_enum(const char *name, wulk_field_t *fields);
wulk_entity_t wulk_struct_anonymous(wulk_field_t *fields);
wulk_entity_t wulk_struct(const char *name, wulk_field_t *fields);
wulk_entity_t wulk_union_anonymous(wulk_field_t *fields);
wulk_entity_t wulk_union(const char *name, wulk_field_t *fields);

void wulk_entity_dtor(wulk_entity_t *self);
bool wulk_entity_equals(wulk_entity_t a, wulk_entity_t b);
wulk_entity_r wulk_entity_fields(wulk_entity_t self);
wulk_field_t *wulk_entity_field_lookup(wulk_entity_t self, const char *name);
wulk_entity_t *wulk_entity_add_field(wulk_entity_t *self, const char *name, wulk_type_t type, short width);

#endif /* WULK_ENTITY_H__ */
