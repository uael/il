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

#ifndef   JL_TYPES_H__
# define  JL_TYPES_H__

#include <assert.h>
#include <stddef.h>
#include <stddef.h>

#include "entity.h"
#include "expr.h"

typedef enum jl_type_n jl_type_n;
typedef enum jl_literal_n jl_literal_n;
typedef enum jl_qualifier_n jl_qualifier_n;

typedef struct jl_type_t jl_type_t;
typedef struct jl_literal_t jl_literal_t;
typedef struct jl_pointer_t jl_pointer_t;
typedef struct jl_array_t jl_array_t;
typedef struct jl_compound_t jl_compound_t;

enum jl_type_n {
  JL_TYPE_UNDEFINED = 0,
  JL_TYPE_LITERAL,
  JL_TYPE_POINTER,
  JL_TYPE_ARRAY,
  JL_TYPE_COMPOUND
};

enum jl_literal_n {
  JL_LITERANL_VOID = 0,
  JL_LITERAL_NULL,
  JL_LITERAL_STRING,
  JL_LITERAL_BOOL,
  JL_LITERAL_CHAR,
  JL_LITERAL_BYTE,
  JL_LITERAL_SHORT,
  JL_LITERAL_INT,
  JL_LITERAL_LONG,
  JL_LITERAL_FLOAT,
  JL_LITERAL_DOUBLE
};

enum jl_qualifier_n {
  JL_QUALIFIER_NONE = 0,
  JL_QUALIFIER_UNSIGNED = 0 << 1,
  JL_QUALIFIER_CONST = 0 << 2,
  JL_QUALIFIER_VOLATILE = 0 << 3
};

struct jl_type_t {
  jl_type_n kind;
  unsigned char qualifiers;
  union {
    jl_literal_t *_literal;
    jl_pointer_t *_pointer;
    jl_array_t *_array;
    jl_compound_t *_compound;
  };
};

void jl_type_init(jl_type_t *self, jl_type_n kind, unsigned char qualifiers);
void jl_type_init_literal(jl_type_t *self, jl_literal_t *literal, unsigned char qualifiers);
void jl_type_init_pointer(jl_type_t *self, jl_pointer_t *pointer, unsigned char qualifiers);
void jl_type_init_array(jl_type_t *self, jl_array_t *array, unsigned char qualifiers);
void jl_type_init_compound(jl_type_t *self, jl_compound_t *compound, unsigned char qualifiers);
#define jl_type_is_defined(t) ((t).kind != JL_TYPE_UNDEFINED)
#define jl_type_is_literal(t) ((t).kind == JL_TYPE_LITERAL)
#define jl_type_is_pointer(t) ((t).kind == JL_TYPE_POINTER)
#define jl_type_is_array(t) ((t).kind == JL_TYPE_ARRAY)
#define jl_type_is_compound(t) ((t).kind == JL_TYPE_COMPOUND)
#define jl_type_literal(t) ((void) assert(jl_type_is_literal(t)), (t)._literal)
#define jl_type_pointer(t) ((void) assert(jl_type_is_pointer(t)), (t)._pointer)
#define jl_type_array(t) ((void) assert(jl_type_is_array(t)), (t)._array)
#define jl_type_compound(t) ((void) assert(jl_type_is_compound(t)), (t)._compound)
void jl_type_dtor(jl_type_t *self);

struct jl_literal_t {
  unsigned refs;
  jl_literal_n kind;
  union {
    char *s;
    int i;
    float f;
  };
};

void jl_literal_init_s(jl_literal_t *self, char *value);
void jl_literal_init_i(jl_literal_t *self, int value);
void jl_literal_init_in(jl_literal_t *self, jl_literal_n kind, int value);
void jl_literal_init_f(jl_literal_t *self, float value);
void jl_literal_init_fn(jl_literal_t *self, jl_literal_n kind, float value);
void jl_literal_dtor(jl_literal_t *self);

struct jl_pointer_t {
  unsigned refs;
  jl_type_t of;
};

void jl_pointer_init(jl_pointer_t *self, jl_type_t of);
void jl_pointer_dtor(jl_pointer_t *self);

struct jl_array_t {
  unsigned refs;
  jl_type_t of;
  jl_expr_t size;
};

void jl_array_init(jl_array_t *self, jl_type_t of, jl_expr_t size);
void jl_array_dtor(jl_array_t *self);

struct jl_compound_t {
  unsigned refs;
  jl_entity_t entity;
};

void jl_compound_init(jl_compound_t *self, jl_entity_t entity);
void jl_compound_dtor(jl_compound_t *self);

#endif /* JL_TYPES_H__ */
