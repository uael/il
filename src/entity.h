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

#include <adt/vector.h>

#include "type.h"

struct jl_expr;
struct jl_stmt;
struct jl_type;

typedef struct jl_entity jl_entity_t;
typedef struct jl_entity jl_entity_t;
typedef struct jl_field jl_field_t;
typedef struct jl_var jl_var_t;
typedef struct jl_param jl_param_t;
typedef struct jl_func jl_func_t;
typedef struct jl_enum jl_enum_t;
typedef struct jl_struct jl_struct_t;
typedef struct jl_union jl_union_t;
typedef struct jl_label jl_label_t;

typedef adt_vector_of(jl_entity_t) jl_entity_r;

enum jl_entity_n {
  JL_ENTITY_UNDEFINED = 0,
  JL_ENTITY_FIELD,
  JL_ENTITY_VAR,
  JL_ENTITY_PARAM,
  JL_ENTITY_FUNC,
  JL_ENTITY_ENUM,
  JL_ENTITY_STRUCT,
  JL_ENTITY_UNION,
  JL_ENTITY_LABEL
};

struct jl_field {
  jl_lloc_t lloc;
  size_t size;
  jl_type_t type;
  const char *name;

  size_t offset;
  short field_width, field_offset;
};

struct jl_var {
  jl_lloc_t lloc;
  size_t size;
  jl_type_t type;
  const char *name;

  struct jl_expr *initializer;
};

struct jl_param {
  jl_lloc_t lloc;
  size_t size;
  jl_type_t type;
  const char *name;

  unsigned position;
  struct jl_expr *initializer;
};

struct jl_func {
  jl_lloc_t lloc;
  size_t size;
  jl_type_t return_type;
  const char *name;

  jl_entity_r params;
  struct jl_expr *body;
};

struct jl_enum {
  jl_lloc_t lloc;
  size_t size;
  jl_type_t type;
  const char *name;

  jl_entity_r vars;
};

struct jl_struct {
  jl_lloc_t lloc;
  size_t size;
  jl_type_t type;
  const char *name;

  jl_entity_r fields;
};

struct jl_union {
  jl_lloc_t lloc;
  size_t size;
  jl_type_t type;
  const char *name;

  jl_entity_r fields;
};

struct jl_label {
  jl_lloc_t lloc;
  size_t size;
  jl_type_t type;
  const char *name;

  struct jl_stmt *next;
};

struct jl_entity {
  union {
    struct {
      jl_lloc_t lloc;
      size_t size;
      jl_type_t type;
      const char *name;
    };
    jl_field_t field;
    jl_var_t variable;
    jl_param_t parameter;
    jl_func_t function;
    jl_enum_t enumerable;
    jl_struct_t structure;
    jl_union_t u_structure;
    jl_label_t label;
  };
  enum jl_entity_n kind;
};

void jl_entity_undef(jl_entity_t *self);
void jl_entity_dtor(jl_entity_t *self);
void jl_entity_switch(jl_entity_t *self, enum jl_entity_n kind);
void jl_entity_acquire(jl_entity_t *self);
void jl_entity_release(jl_entity_t *self);
void jl_entity_update_size(jl_entity_t *self);
bool jl_entity_is_defined(jl_entity_t *self);
bool jl_entity_equals(jl_entity_t a, jl_entity_t b);
const char *jl_entity_name(jl_entity_t self);
jl_type_t jl_entity_type(jl_entity_t self);
jl_entity_r jl_entity_fields(jl_entity_t self);
jl_field_t *jl_entity_field_lookup(jl_entity_t self, const char *name);

#define jl_entity_undefined() (jl_entity_t) {JL_ENTITY_UNDEFINED};

#define jl_entity_is_field(e) ((e).kind == JL_ENTITY_FIELD)
#define jl_entity_is_var(e) ((e).kind == JL_ENTITY_VAR)
#define jl_entity_is_param(e) ((e).kind == JL_ENTITY_PARAM)
#define jl_entity_is_func(e) ((e).kind == JL_ENTITY_FUNC)
#define jl_entity_is_enum(e) ((e).kind == JL_ENTITY_ENUM)
#define jl_entity_is_struct(e) ((e).kind == JL_ENTITY_STRUCT)
#define jl_entity_is_union(e) ((e).kind == JL_ENTITY_UNION)
#define jl_entity_is_label(e) ((e).kind == JL_ENTITY_LABEL)
#define jl_pentity_is_field(e) ((e)->kind == JL_ENTITY_FIELD)
#define jl_pentity_is_var(e) ((e)->kind == JL_ENTITY_VAR)
#define jl_pentity_is_param(e) ((e)->kind == JL_ENTITY_PARAM)
#define jl_pentity_is_func(e) ((e)->kind == JL_ENTITY_FUNC)
#define jl_pentity_is_enum(e) ((e)->kind == JL_ENTITY_ENUM)
#define jl_pentity_is_struct(e) ((e)->kind == JL_ENTITY_STRUCT)
#define jl_pentity_is_union(e) ((e)->kind == JL_ENTITY_UNION)
#define jl_pentity_is_label(e) ((e)->kind == JL_ENTITY_LABEL)

void jl_entity_add_field(jl_entity_t *self, const char *name, jl_type_t type);

jl_entity_t jl_var_undefined();
jl_entity_t jl_var_int(const char *name, int d);
jl_entity_t jl_var_float(const char *name, float f);
jl_entity_t jl_var_string(const char *name, const char *s);
jl_entity_t jl_var(const char *name, jl_type_t type, struct jl_expr *initializer);
void jl_var_init(jl_entity_t *self, const char *name, jl_type_t type, struct jl_expr *initializer);

jl_entity_t jl_param_undefined();
jl_entity_t jl_param_int(unsigned position, const char *name, int d);
jl_entity_t jl_param_float(unsigned position, const char *name, float f);
jl_entity_t jl_param_string(unsigned position, const char *name, const char *s);
void jl_param_init(jl_entity_t *self, unsigned position, const char *name, jl_type_t type, struct jl_expr *initializer);

jl_entity_t jl_func_undefined();
jl_entity_t jl_func_decl(jl_type_t return_type, const char *name, jl_entity_r params);
jl_entity_t jl_proc_decl(const char *name, jl_entity_r params);
jl_entity_t jl_func_def(jl_entity_t prototype, struct jl_stmt *body);
void jl_func_init(jl_entity_t *self, jl_type_t r, const char * n, jl_entity_r p, struct jl_stmt *b);

jl_entity_t jl_enum_undefined();
jl_entity_t jl_enum(const char *name, jl_entity_r fields);
jl_entity_t jl_enum_anonymous(jl_entity_r fields);
void jl_enum_init(jl_entity_t *self, const char *name, jl_entity_r fields);

jl_entity_t jl_struct_undefined();
jl_entity_t jl_struct(const char *name, jl_field_t *fields, size_t n_fields);
jl_entity_t jl_struct_anonymous(jl_field_t *fields, size_t n_fields);
void jl_struct_init(jl_entity_t *self, const char *name, jl_field_t *fields, size_t n_fields);

jl_entity_t jl_union_undefined();
jl_entity_t jl_union(const char *name, jl_field_t *fields, size_t n_fields);
jl_entity_t jl_union_anonymous(jl_field_t *fields, size_t n_fields);
void jl_union_init(jl_entity_t *self, const char *name, jl_field_t *fields, size_t n_fields);

#endif /* JL_ENTITY_H__ */
