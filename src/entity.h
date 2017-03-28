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
  unsigned refs;
  const char *name;
  jl_type_t type;
  size_t offset;
  short field_width;
  short field_offset;
};

jl_entity_t jl_field(const char *name, jl_type_t type);
void jl_field_init(jl_entity_t *self, const char *name, jl_type_t type);

struct jl_var_t {
  unsigned refs;
  const char *name;
  jl_type_t type;
  jl_expr_t initializer;
};

jl_entity_t jl_var_undefined();
jl_entity_t jl_var_int(const char *name, int d);
jl_entity_t jl_var_float(const char *name, float f);
jl_entity_t jl_var_string(const char *name, const char *s);
jl_entity_t jl_var(const char *name, jl_type_t type, jl_expr_t initializer);
void jl_var_init(jl_entity_t *self, const char *name, jl_type_t type, jl_expr_t initializer);

struct jl_param_t {
  unsigned refs;
  const char *name;
  unsigned position;
  jl_type_t type;
  jl_expr_t initializer;
};

jl_entity_t jl_param_undefined();
jl_entity_t jl_param_int(unsigned position, const char *name, int d);
jl_entity_t jl_param_float(unsigned position, const char *name, float f);
jl_entity_t jl_param_string(unsigned position, const char *name, const char *s);
void jl_param_init(jl_entity_t *self, unsigned position, const char *name, jl_type_t type, jl_expr_t initializer);

struct jl_func_t {
  unsigned refs;
  enum jl_func_specifier_n specifiers;
  const char *name;
  jl_type_t return_type;
  jl_entity_r params;
  jl_stmt_t body;
};

jl_entity_t jl_func_undefined();
jl_entity_t jl_func_decl(enum jl_func_specifier_n specifiers, jl_type_t return_type, const char *name, jl_entity_r params);
jl_entity_t jl_proc_decl(enum jl_func_specifier_n specifiers, const char *name, jl_entity_r params);
jl_entity_t jl_func_def(jl_entity_t prototype, jl_stmt_t body);
void jl_func_init(jl_entity_t *self, enum jl_func_specifier_n s, jl_type_t r, const char * n, jl_entity_r p, jl_stmt_t b);
bool jl_func_is_inline(jl_entity_t *self);
bool jl_func_is_noreturn(jl_entity_t *self);
void jl_func_add_specifier(jl_entity_t *self, enum jl_func_specifier_n specifier);
void jl_func_rem_specifier(jl_entity_t *self, enum jl_func_specifier_n specifier);

struct jl_enum_t {
  unsigned refs;
  const char *name;
  jl_entity_r vars;
};

jl_entity_t jl_enum_undefined();
jl_entity_t jl_enum(const char *name, jl_entity_r fields);
jl_entity_t jl_enum_anonymous(jl_entity_r fields);
void jl_enum_init(jl_entity_t *self, const char *name, jl_entity_r fields);

struct jl_struct_t {
  unsigned refs;
  const char *name;
  jl_entity_r fields;
};

jl_entity_t jl_struct_undefined();
jl_entity_t jl_struct(const char *name, jl_entity_r fields);
jl_entity_t jl_struct_anonymous(jl_entity_r fields);
void jl_struct_init(jl_entity_t *self, const char *name, jl_entity_r fields);

struct jl_union_t {
  unsigned refs;
  const char *name;
  jl_entity_r fields;
};

jl_entity_t jl_union_undefined();
jl_entity_t jl_union(const char *name, jl_entity_r fields);
jl_entity_t jl_union_anonymous(jl_entity_r fields);
void jl_union_init(jl_entity_t *self, const char *name, jl_entity_r fields);

struct jl_label_t {
  unsigned refs;
  const char *name;
  jl_stmt_t next;
};

#endif /* JL_ENTITY_H__ */
