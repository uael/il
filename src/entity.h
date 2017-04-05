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

#ifndef   JL_ENTITY_H__
# define  JL_ENTITY_H__

#include "ir.h"

#define jl_entity_undefined() ((jl_entity_t) {JL_ENTITY_UNDEFINED})
#define jl_entity_undef(eptr) (*(eptr) = jl_entity_undefined())

jl_entity_t jl_field(const char *name, jl_type_t type);
jl_entity_t jl_var_int(const char *name, int d);
jl_entity_t jl_var_float(const char *name, float f);
jl_entity_t jl_var_string(const char *name, const char *s);
jl_entity_t jl_var(const char *name, jl_type_t type, jl_expr_t initializer);
jl_entity_t jl_param_int(unsigned position, const char *name, int d);
jl_entity_t jl_param_float(unsigned position, const char *name, float f);
jl_entity_t jl_param_string(unsigned position, const char *name, const char *s);
jl_entity_t jl_param(unsigned position, const char *name, jl_type_t type, jl_expr_t initializer);
jl_entity_t jl_func_decl(jl_type_t return_type, const char *name, jl_param_t *params);
jl_entity_t jl_proc_decl(const char *name, jl_param_t *params);
jl_entity_t jl_func_def(jl_func_t prototype, jl_stmt_t body);
jl_entity_t jl_func(jl_type_t return_type, const char *name, jl_param_t *params, jl_stmt_t body);
jl_entity_t jl_enum_anonymous(jl_field_t *fields);
jl_entity_t jl_enum(const char *name, jl_field_t *fields);
jl_entity_t jl_struct_anonymous(jl_field_t *fields);
jl_entity_t jl_struct(const char *name, jl_field_t *fields);
jl_entity_t jl_union_anonymous(jl_field_t *fields);
jl_entity_t jl_union(const char *name, jl_field_t *fields);

void jl_entity_dtor(jl_entity_t *self);
bool jl_entity_equals(jl_entity_t a, jl_entity_t b);
jl_entity_r jl_entity_fields(jl_entity_t self);
jl_field_t *jl_entity_field_lookup(jl_entity_t self, const char *name);

void jl_entity_add_field(jl_entity_t *self, const char *name, jl_type_t type);

#endif /* JL_ENTITY_H__ */
