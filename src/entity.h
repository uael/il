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

#include "adt/vector.h"
#include "stmt.h"

struct jl_var_t;
struct jl_param_t;
struct jl_func_t;
struct jl_enum_t;
struct jl_struct_t;
struct jl_union_t;
struct jl_label_t;

typedef enum jl_entity_n jl_entity_n;
typedef enum jl_linkage_n jl_linkage_n;
typedef enum jl_storage_n jl_storage_n;
typedef enum jl_visibility_n jl_visibility_n;

typedef struct jl_entity_t jl_entity_t;

typedef jl_vector_of(jl_entity_t) jl_entity_r;
typedef jl_vector_of(struct jl_param_t) jl_param_r;
typedef jl_vector_of(jl_entity_t) jl_var_r;

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

enum jl_linkage_n {
  JL_LINKAGE_DEFAULT = 0,
  JL_LINKAGE_EXTERN = 1 << 0,
  JL_LINKAGE_CONST = 1 << 1,
  JL_LINKAGE_WEAK = 1 << 2,
  JL_LINKAGE_INLINE = 1 << 3
};

enum jl_storage_n {
  JL_STORAGE_NONE = 1 << 4,
  JL_STORAGE_STATIC = 1 << 5,
  JL_STORAGE_ABSTRACT = 1 << 6,
  JL_STORAGE_FINAL = 1 << 7
};

enum jl_visibility_n {
  JL_VISIBILITY_PUBLIC = 1 << 8,
  JL_VISIBILITY_PRIVATE = 1 << 9,
  JL_VISIBILITY_PROTECTED = 1 << 10,
  JL_VISIBILITY_LOCAL = 1 << 11
};

struct jl_entity_t {
  jl_entity_n kind;
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

#define jl_entity_is_defined(e) ((e).kind != JL_ENTITY_UNDEFINED)
#define jl_entity_is_var(e) ((e).kind == JL_ENTITY_VAR)
#define jl_entity_is_param(e) ((e).kind == JL_ENTITY_PARAM)
#define jl_entity_is_func(e) ((e).kind == JL_ENTITY_FUNC)
#define jl_entity_is_enum(e) ((e).kind == JL_ENTITY_ENUM)
#define jl_entity_is_struct(e) ((e).kind == JL_ENTITY_STRUCT)
#define jl_entity_is_union(e) ((e).kind == JL_ENTITY_UNION)
#define jl_entity_is_label(e) ((e).kind == JL_ENTITY_LABEL)
#define jl_entity_var(e) ((void) assert(jl_entity_is_var(e)), (e)._var)
#define jl_entity_param(e) ((void) assert(jl_entity_is_param(e)), (e)._param)
#define jl_entity_func(e) ((void) assert(jl_entity_is_func(e)), (e)._func)
#define jl_entity_enum(e) ((void) assert(jl_entity_is_enum(e)), (e)._enum)
#define jl_entity_struct(e) ((void) assert(jl_entity_is_struct(e)), (e)._struct)
#define jl_entity_union(e) ((void) assert(jl_entity_is_union(e)), (e)._union)
#define jl_entity_label(e) ((void) assert(jl_entity_is_label(e)), (e)._label)
void jl_entity_dtor(jl_entity_t *self);

#endif /* JL_ENTITY_H__ */
