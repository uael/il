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

#ifndef   IL_ENTITY_H__
# define  IL_ENTITY_H__

#include "ir.h"

#define il_entity_undefined() ((il_entity_t) {.kind = IL_ENTITY_UNDEFINED})
#define il_entity_undef(eptr) (*(eptr) = il_entity_undefined())

il_entity_t il_field(const char *name, il_type_t type);
il_entity_t il_var_int(const char *name, int d);
il_entity_t il_var_float(const char *name, float f);
il_entity_t il_var_string(const char *name, const char *s);
il_entity_t il_var(const char *name, il_type_t type, il_expr_t initializer);
il_entity_t il_param_int(unsigned position, const char *name, int d);
il_entity_t il_param_float(unsigned position, const char *name, float f);
il_entity_t il_param_string(unsigned position, const char *name, const char *s);
il_entity_t il_param(unsigned position, const char *name, il_type_t type, il_expr_t initializer);
il_entity_t il_func_decl(il_type_t return_type, const char *name, il_param_t *params);
il_entity_t il_proc_decl(const char *name, il_param_t *params);
il_entity_t il_func_def(il_func_t prototype, il_stmt_t body);
il_entity_t il_func(il_type_t return_type, const char *name, il_param_t *params, il_stmt_t body);
il_entity_t il_enum_anonymous(il_field_t *fields);
il_entity_t il_enum(const char *name, il_field_t *fields);
il_entity_t il_struct_anonymous(il_field_t *fields);
il_entity_t il_struct(const char *name, il_field_t *fields);
il_entity_t il_union_anonymous(il_field_t *fields);
il_entity_t il_union(const char *name, il_field_t *fields);

void il_entity_dtor(il_entity_t *self);
bool il_entity_equals(il_entity_t a, il_entity_t b);
il_entity_r il_entity_fields(il_entity_t self);
il_field_t *il_entity_field_lookup(il_entity_t self, const char *name);
il_entity_t *il_entity_add_field(il_entity_t *self, const char *name, il_type_t type, short width);

#endif /* IL_ENTITY_H__ */
