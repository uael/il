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

#include "entity_t.h"
#include "expr_t.h"
#include "stmt_t.h"
#include "type_t.h"

jl_type_t jl_entity_type(jl_entity_t self);

struct jl_field_t {
  const char *name;
  jl_type_t type;
  short field_width;
  short field_offset;

  unsigned refs;
  size_t offset;
};

void jl_entity_add_field(jl_entity_t *self, const char *name, jl_type_t type);

struct jl_var_t {
  const char *name;
  jl_type_t type;
  jl_expr_t initializer;

  unsigned refs;
};

jl_entity_t jl_var_undefined();
jl_entity_t jl_var_int(const char *name, int d);
jl_entity_t jl_var_float(const char *name, float f);
jl_entity_t jl_var_string(const char *name, const char *s);
jl_entity_t jl_var(const char *name, jl_type_t type, jl_expr_t initializer);
void jl_var_init(jl_entity_t *self, const char *name, jl_type_t type, jl_expr_t initializer);

struct jl_param_t {
  const char *name;
  unsigned position;
  jl_type_t type;
  jl_expr_t initializer;

  unsigned refs;
};

jl_entity_t jl_param_undefined();
jl_entity_t jl_param_int(unsigned position, const char *name, int d);
jl_entity_t jl_param_float(unsigned position, const char *name, float f);
jl_entity_t jl_param_string(unsigned position, const char *name, const char *s);
void jl_param_init(jl_entity_t *self, unsigned position, const char *name, jl_type_t type, jl_expr_t initializer);

struct jl_func_t {
  const char *name;
  jl_type_t return_type;
  jl_entity_r params;
  jl_stmt_t body;

  unsigned refs;
};

jl_entity_t jl_func_undefined();
jl_entity_t jl_func_decl(jl_type_t return_type, const char *name, jl_entity_r params);
jl_entity_t jl_proc_decl(const char *name, jl_entity_r params);
jl_entity_t jl_func_def(jl_entity_t prototype, jl_stmt_t body);
void jl_func_init(jl_entity_t *self, jl_type_t r, const char * n, jl_entity_r p, jl_stmt_t b);

struct jl_enum_t {
  const char *name;
  jl_entity_r vars;

  unsigned refs;
};

jl_entity_t jl_enum_undefined();
jl_entity_t jl_enum(const char *name, jl_entity_r fields);
jl_entity_t jl_enum_anonymous(jl_entity_r fields);
void jl_enum_init(jl_entity_t *self, const char *name, jl_entity_r fields);

struct jl_struct_t {
  const char *name;
  jl_entity_r fields;

  unsigned refs;
};

jl_entity_t jl_struct_undefined();
jl_entity_t jl_struct(const char *name, jl_field_t *fields, size_t n_fields);
jl_entity_t jl_struct_anonymous(jl_field_t *fields, size_t n_fields);
void jl_struct_init(jl_entity_t *self, const char *name, jl_field_t *fields, size_t n_fields);

struct jl_union_t {
  const char *name;
  jl_entity_r fields;

  unsigned refs;
};

jl_entity_t jl_union_undefined();
jl_entity_t jl_union(const char *name, jl_field_t *fields, size_t n_fields);
jl_entity_t jl_union_anonymous(jl_field_t *fields, size_t n_fields);
void jl_union_init(jl_entity_t *self, const char *name, jl_field_t *fields, size_t n_fields);

struct jl_label_t {
  const char *name;
  jl_stmt_t next;

  unsigned refs;
};

#endif /* JL_ENTITY_H__ */
