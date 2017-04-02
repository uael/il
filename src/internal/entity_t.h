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

#ifndef   JL_ENTITY_T_H__
# define  JL_ENTITY_T_H__

#include <adt/vector.h>

#include "api.h"

typedef struct jl_entity_t jl_entity_t;
typedef struct jl_field_t jl_field_t;
typedef struct jl_var_t jl_var_t;
typedef struct jl_param_t jl_param_t;
typedef struct jl_func_t jl_func_t;
typedef struct jl_enum_t jl_enum_t;
typedef struct jl_struct_t jl_struct_t;
typedef struct jl_union_t jl_union_t;
typedef struct jl_label_t jl_label_t;

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

struct jl_entity_t {
  unsigned kind : 8;
  size_t size;
  union {
    struct jl_field_t *_field;
    struct jl_var_t *_var;
    struct jl_param_t *_param;
    struct jl_func_t *_func;
    struct jl_enum_t *_enum;
    struct jl_struct_t *_struct;
    struct jl_union_t *_union;
    struct jl_label_t *_label;
  } u;
};

void jl_entity_undef(jl_entity_t *self);
void jl_entity_dtor(jl_entity_t *self);
void jl_entity_switch(jl_entity_t *self, enum jl_entity_n kind);
void jl_entity_acquire(jl_entity_t *self);
void jl_entity_release(jl_entity_t *self);
void jl_entity_update_size(jl_entity_t *self);
bool jl_entity_is_defined(jl_entity_t *self);
bool jl_entity_equals(jl_entity_t a, jl_entity_t b);
jl_entity_r jl_entity_fields(jl_entity_t self);
jl_field_t *jl_entity_field_lookup(jl_entity_t self, const char *name);
const char *jl_entity_name(jl_entity_t self);

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

#define jl_entity_field(e) (assert(jl_entity_is_field(e)), jl_u(e, field))
#define jl_entity_var(e) (assert(jl_entity_is_var(e)), jl_u(e, var))
#define jl_entity_param(e) (assert(jl_entity_is_param(e)), jl_u(e, param))
#define jl_entity_func(e) (assert(jl_entity_is_func(e)), jl_u(e, func))
#define jl_entity_enum(e) (assert(jl_entity_is_enum(e)), jl_u(e, enum))
#define jl_entity_struct(e) (assert(jl_entity_is_struct(e)), jl_u(e, struct))
#define jl_entity_union(e) (assert(jl_entity_is_union(e)), jl_u(e, union))
#define jl_entity_label(e) (assert(jl_entity_is_label(e)), jl_u(e, label))
#define jl_pentity_field(e) (assert(jl_pentity_is_field(e)), jl_pu(e, field))
#define jl_pentity_var(e) (assert(jl_pentity_is_var(e)), jl_pu(e, var))
#define jl_pentity_param(e) (assert(jl_pentity_is_param(e)), jl_pu(e, param))
#define jl_pentity_func(e) (assert(jl_pentity_is_func(e)), jl_pu(e, func))
#define jl_pentity_enum(e) (assert(jl_pentity_is_enum(e)), jl_pu(e, enum))
#define jl_pentity_struct(e) (assert(jl_pentity_is_struct(e)), jl_pu(e, struct))
#define jl_pentity_union(e) (assert(jl_pentity_is_union(e)), jl_pu(e, union))
#define jl_pentity_label(e) (assert(jl_pentity_is_label(e)), jl_pu(e, label))

#endif /* JL_ENTITY_T_H__ */
