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

typedef enum jl_entity_n jl_entity_n;
typedef enum jl_func_specifier_n jl_func_specifier_n;

typedef struct jl_entity_t jl_entity_t;
typedef struct jl_var_t jl_var_t;
typedef struct jl_param_t jl_param_t;
typedef struct jl_func_t jl_func_t;
typedef struct jl_enum_t jl_enum_t;
typedef struct jl_struct_t jl_struct_t;
typedef struct jl_union_t jl_union_t;
typedef struct jl_label_t jl_label_t;
typedef jl_vector_of(jl_entity_t) jl_entity_r;

enum jl_entity_n {
  JL_ENTITY_UNDEFINED = 0,
  JL_ENTITY_VAR,
  JL_ENTITY_PARAM,
  JL_ENTITY_FUNC,
  JL_ENTITY_ENUM,
  JL_ENTITY_STRUCT,
  JL_ENTITY_UNION,
  JL_ENTITY_LABEL
};

enum jl_func_specifier_n {
  JL_FUNC_SPECIFIER_NONE = 0,
  JL_FUNC_SPECIFIER_INLINE = 1 << 0,
  JL_FUNC_SPECIFIER_NORETURN = 1 << 1
};

struct jl_entity_t {
  jl_entity_n kind : 8;
  union {
    struct jl_var_t *_var;
    struct jl_param_t *_param;
    struct jl_func_t *_func;
    struct jl_enum_t *_enum;
    struct jl_struct_t *_struct;
    struct jl_union_t *_union;
    struct jl_label_t *_label;
  };
};

jl_entity_t jl_entity_undefined();
void jl_entity_undef(jl_entity_t *self);
void jl_entity_dtor(jl_entity_t *self);
void jl_entity_switch(jl_entity_t *self, jl_entity_n kind);
void jl_entity_acquire(jl_entity_t *self);
void jl_entity_release(jl_entity_t *self);
bool jl_entity_is_defined(jl_entity_t *self);
const char *jl_entity_name(jl_entity_t self);

#define jl_entity_is_var(e) ((e).kind == JL_ENTITY_VAR)
#define jl_entity_is_param(e) ((e).kind == JL_ENTITY_PARAM)
#define jl_entity_is_func(e) ((e).kind == JL_ENTITY_FUNC)
#define jl_entity_is_enum(e) ((e).kind == JL_ENTITY_ENUM)
#define jl_entity_is_struct(e) ((e).kind == JL_ENTITY_STRUCT)
#define jl_entity_is_union(e) ((e).kind == JL_ENTITY_UNION)
#define jl_entity_is_label(e) ((e).kind == JL_ENTITY_LABEL)
#define jl_pentity_is_var(e) ((e)->kind == JL_ENTITY_VAR)
#define jl_pentity_is_param(e) ((e)->kind == JL_ENTITY_PARAM)
#define jl_pentity_is_func(e) ((e)->kind == JL_ENTITY_FUNC)
#define jl_pentity_is_enum(e) ((e)->kind == JL_ENTITY_ENUM)
#define jl_pentity_is_struct(e) ((e)->kind == JL_ENTITY_STRUCT)
#define jl_pentity_is_union(e) ((e)->kind == JL_ENTITY_UNION)
#define jl_pentity_is_label(e) ((e)->kind == JL_ENTITY_LABEL)

#define jl_entity_var(e) ((void) assert(jl_entity_is_var(e)), (e)._var)
#define jl_entity_param(e) ((void) assert(jl_entity_is_param(e)), (e)._param)
#define jl_entity_func(e) ((void) assert(jl_entity_is_func(e)), (e)._func)
#define jl_entity_enum(e) ((void) assert(jl_entity_is_enum(e)), (e)._enum)
#define jl_entity_struct(e) ((void) assert(jl_entity_is_struct(e)), (e)._struct)
#define jl_entity_union(e) ((void) assert(jl_entity_is_union(e)), (e)._union)
#define jl_entity_label(e) ((void) assert(jl_entity_is_label(e)), (e)._label)
#define jl_pentity_var(e) ((void) assert(jl_pentity_is_var(e)), (e)->_var)
#define jl_pentity_param(e) ((void) assert(jl_pentity_is_param(e)), (e)->_param)
#define jl_pentity_func(e) ((void) assert(jl_pentity_is_func(e)), (e)->_func)
#define jl_pentity_enum(e) ((void) assert(jl_pentity_is_enum(e)), (e)->_enum)
#define jl_pentity_struct(e) ((void) assert(jl_pentity_is_struct(e)), (e)->_struct)
#define jl_pentity_union(e) ((void) assert(jl_pentity_is_union(e)), (e)->_union)
#define jl_pentity_label(e) ((void) assert(jl_pentity_is_label(e)), (e)->_label)

#endif /* JL_ENTITY_T_H__ */
