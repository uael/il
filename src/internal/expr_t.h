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

#ifndef   JL_EXPR_T_H__
# define  JL_EXPR_T_H__

#include <adt/vector.h>

#include "token.h"
#include "type_t.h"

typedef struct jl_expr_t jl_expr_t;
typedef struct jl_expr_id_t jl_expr_id_t;
typedef struct jl_expr_const_t jl_expr_const_t;
typedef struct jl_expr_unary_t jl_expr_unary_t;
typedef struct jl_expr_binary_t jl_expr_binary_t;
typedef struct jl_expr_ternary_t jl_expr_ternary_t;
typedef struct jl_expr_array_read_t jl_expr_array_read_t;
typedef struct jl_expr_array_write_t jl_expr_array_write_t;
typedef struct jl_expr_field_read_t jl_expr_field_read_t;
typedef struct jl_expr_field_write_t jl_expr_field_write_t;
typedef struct jl_expr_call_t jl_expr_call_t;

typedef jl_vector_of(jl_expr_t) jl_expr_r;

enum jl_expr_n {
  JL_EXPR_UNDEFINED = 0,
  JL_EXPR_ID,
  JL_EXPR_CONST,
  JL_EXPR_UNARY,
  JL_EXPR_BINARY,
  JL_EXPR_TERNARY,
  JL_EXPR_ARRAY_READ,
  JL_EXPR_ARRAY_WRITE,
  JL_EXPR_FIELD_READ,
  JL_EXPR_FIELD_WRITE,
  JL_EXPR_CALL
};

enum jl_op_n {
  JL_OP_CAST,   /* (T) l        */
  JL_OP_EN,     /* (l)          */
  JL_OP_CALL,   /* l()          */
  JL_OP_VA_ARG, /* va_arg(l, T) */
  JL_OP_NOT,    /* !l           */
  JL_OP_ASSIGN, /* l = r        */
  JL_OP_ADD,    /* l + r        */
  JL_OP_SUB,    /* l - r        */
  JL_OP_MUL,    /* l * r        */
  JL_OP_DIV,    /* l / r        */
  JL_OP_MOD,    /* l % r        */
  JL_OP_AND,    /* l & r        */
  JL_OP_OR,     /* l | r        */
  JL_OP_XOR,    /* l ^ r        */
  JL_OP_SHL,    /* l << r       */
  JL_OP_SHR,    /* l >> r       */
  JL_OP_EQ,     /* l == r       */
  JL_OP_NE,     /* l != r       */
  JL_OP_GE,     /* l >= r       */
  JL_OP_GT,     /* l > r        */
  JL_OP_NEG     /* l ~ r        */
};

struct jl_expr_t {
  unsigned kind : 8;
  jl_loc_t loc;
  union {
    struct jl_expr_id_t *_id;
    struct jl_expr_const_t *_const;
    struct jl_expr_unary_t *_unary;
    struct jl_expr_binary_t *_binary;
    struct jl_expr_ternary_t *_ternary;
    struct jl_expr_array_read_t *_array_read;
    struct jl_expr_array_write_t *_array_write;
    struct jl_expr_field_read_t *_field_read;
    struct jl_expr_field_write_t *_field_write;
    struct jl_expr_call_t *_call;
  } u;
};

jl_expr_t jl_expr_undefined();
void jl_expr_dtor(jl_expr_t *self);
void jl_expr_switch(jl_expr_t *self, enum jl_expr_n kind);
void jl_expr_acquire(jl_expr_t *self);
void jl_expr_release(jl_expr_t *self);
bool jl_expr_is_defined(jl_expr_t *self);
jl_type_t jl_expr_get_type(jl_expr_t self);
void jl_expr_set_type(jl_expr_t *self, jl_type_t type);
jl_expr_t jl_expr_get_next(jl_expr_t *self);
void jl_expr_set_next(jl_expr_t *self, jl_expr_t next);

#define jl_expr_is_id(t) ((t).kind == JL_EXPR_ID)
#define jl_expr_is_const(t) ((t).kind == JL_EXPR_CONST)
#define jl_expr_is_unary(t) ((t).kind == JL_EXPR_UNARY)
#define jl_expr_is_binary(t) ((t).kind == JL_EXPR_BINARY)
#define jl_expr_is_ternary(t) ((t).kind == JL_EXPR_TERNARY)
#define jl_expr_is_array_read(t) ((t).kind == JL_EXPR_ARRAY_READ)
#define jl_expr_is_array_write(t) ((t).kind == JL_EXPR_ARRAY_WRITE)
#define jl_expr_is_field_read(t) ((t).kind == JL_EXPR_FIELD_READ)
#define jl_expr_is_field_write(t) ((t).kind == JL_EXPR_FIELD_WRITE)
#define jl_expr_is_call(t) ((t).kind == JL_EXPR_CALL)
#define jl_pexpr_is_id(t) ((t)->kind == JL_EXPR_ID)
#define jl_pexpr_is_const(t) ((t)->kind == JL_EXPR_CONST)
#define jl_pexpr_is_unary(t) ((t)->kind == JL_EXPR_UNARY)
#define jl_pexpr_is_binary(t) ((t)->kind == JL_EXPR_BINARY)
#define jl_pexpr_is_ternary(t) ((t)->kind == JL_EXPR_TERNARY)
#define jl_pexpr_is_array_read(t) ((t)->kind == JL_EXPR_ARRAY_READ)
#define jl_pexpr_is_array_write(t) ((t)->kind == JL_EXPR_ARRAY_WRITE)
#define jl_pexpr_is_field_read(t) ((t)->kind == JL_EXPR_FIELD_READ)
#define jl_pexpr_is_field_write(t) ((t)->kind == JL_EXPR_FIELD_WRITE)
#define jl_pexpr_is_call(t) ((t)->kind == JL_EXPR_CALL)

#define jl_expr_id(t) ((void) assert(jl_expr_is_id(t)), (t).u._id)
#define jl_expr_const(t) ((void) assert(jl_expr_is_const(t)), t.u._const)
#define jl_expr_unary(t) ((void) assert(jl_expr_is_unary(t)), (t).u._unary)
#define jl_expr_binary(t) ((void) assert(jl_expr_is_binary(t)), (t).u._binary)
#define jl_expr_ternary(t) ((void) assert(jl_expr_is_ternary(t)), (t).u._ternary)
#define jl_expr_array_read(t) ((void) assert(jl_expr_is_array_read(t)), (t).u._array_read)
#define jl_expr_array_write(t) ((void) assert(jl_expr_is_array_write(t)), (t).u._array_write)
#define jl_expr_field_read(t) ((void) assert(jl_expr_is_field_read(t)), (t).u._field_read)
#define jl_expr_field_write(t) ((void) assert(jl_expr_is_field_write(t)), (t).u._field_write)
#define jl_expr_call(t) ((void) assert(jl_expr_is_call(t)), (t).u._call)
#define jl_pexpr_id(t) ((void) assert(jl_pexpr_is_id(t)), (t)->u._id)
#define jl_pexpr_const(t) ((void) assert(jl_pexpr_is_const(t)), (t)->u._const)
#define jl_pexpr_unary(t) ((void) assert(jl_pexpr_is_unary(t)), (t)->u._unary)
#define jl_pexpr_binary(t) ((void) assert(jl_pexpr_is_binary(t)), (t)->u._binary)
#define jl_pexpr_ternary(t) ((void) assert(jl_pexpr_is_ternary(t)), (t)->u._ternary)
#define jl_pexpr_array_read(t) ((void) assert(jl_pexpr_is_array_read(t)), (t)->u._array_read)
#define jl_pexpr_array_write(t) ((void) assert(jl_pexpr_is_array_write(t)), (t)->u._array_write)
#define jl_pexpr_field_read(t) ((void) assert(jl_pexpr_is_field_read(t)), (t)->u._field_read)
#define jl_pexpr_field_write(t) ((void) assert(jl_pexpr_is_field_write(t)), (t)->u._field_write)
#define jl_pexpr_call(t) ((void) assert(jl_pexpr_is_call(t)), (t)->u._call)

#endif /* JL_EXPR_T_H__ */
