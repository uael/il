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

#ifndef   JL_ENTITIES_H__
# define  JL_ENTITIES_H__

#include "entity.h"
#include "types.h"
#include "stmt.h"
#include "expr.h"

typedef struct jl_var_t jl_var_t;
typedef struct jl_param_t jl_param_t;
typedef struct jl_func_t jl_func_t;
typedef struct jl_enum_t jl_enum_t;
typedef struct jl_struct_t jl_struct_t;
typedef struct jl_union_t jl_union_t;
typedef struct jl_label_t jl_label_t;

struct jl_var_t {
  unsigned refs, flags;
  const char *name;
  jl_type_t type;
  jl_expr_t initializer;
};

void jl_var_init(jl_entity_t *self, const char *name, jl_type_t type, jl_expr_t initializer, unsigned flags);
void jl_var_dtor(jl_entity_t *self);

struct jl_param_t {
  unsigned refs, flags;
  const char *name;
  unsigned position;
  jl_type_t type;
  jl_expr_t initializer;
};

void jl_param_init(jl_entity_t *self, unsigned position, const char *name, jl_type_t type, jl_expr_t initializer,
  unsigned flags);
void jl_param_dtor(jl_entity_t *self);

struct jl_func_t {
  unsigned refs, flags;
  const char *name;
  jl_type_t return_type;
  jl_param_r params;
  jl_stmt_t body;
};

void jl_func_init(jl_entity_t *self, const char *name, jl_type_t return_type, jl_param_r params, jl_stmt_t body,
  unsigned flags);
void jl_func_dtor(jl_entity_t *self);

struct jl_enum_t {
  unsigned refs, flags;
  const char *name;
  jl_type_t item_type;
  jl_var_r vars;
};

void jl_enum_init(jl_entity_t *self, const char *name, jl_type_t item_type, jl_var_r vars, unsigned flags);
void jl_enum_dtor(jl_entity_t *self);

struct jl_struct_t {
  unsigned refs, flags;
  const char *name;
  jl_var_r vars;
};

void jl_struct_init(jl_entity_t *self, const char *name, jl_var_r vars, unsigned flags);
void jl_struct_dtor(jl_entity_t *self);

struct jl_union_t {
  unsigned refs, flags;
  const char *name;
  jl_var_r vars;
};

void jl_union_init(jl_entity_t *self, const char *name, jl_var_r vars, unsigned flags);
void jl_union_dtor(jl_entity_t *self);

struct jl_label_t {
  unsigned refs, flags;
  const char *name;
  jl_stmt_t next;
};

void jl_label_init(jl_entity_t *self, const char *name, jl_stmt_t next, unsigned flags);
void jl_label_dtor(jl_entity_t *label);

#endif /* JL_ENTITIES_H__ */
