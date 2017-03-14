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

#ifndef   JL_EXPR_H__
# define  JL_EXPR_H__

#include <adt/vector.h>

#include "token.h"

struct jl_expr_id_t;
struct jl_expr_const_t;
struct jl_expr_unary_t;
struct jl_expr_binary_t;
struct jl_expr_ternary_t;
struct jl_expr_array_read_t;
struct jl_expr_array_write_t;
struct jl_expr_field_read_t;
struct jl_expr_field_write_t;
struct jl_expr_call_t;

typedef enum jl_expr_n jl_expr_n;
typedef enum jl_op_n jl_op_n;

typedef struct jl_expr_t jl_expr_t;

typedef jl_vector_of(jl_expr_t) jl_expr_r;

enum jl_expr_n {
  JL_EXPR_ID = 0,
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
  JL_OP_GT      /* l > r        */
};

struct jl_expr_t {
  jl_expr_n kind;
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
  };
};

void jl_expr_init(jl_expr_t *self, jl_expr_n kind, jl_loc_t loc, void *ptr);
void jl_expr_dtor(jl_expr_t *self);

#endif /* JL_EXPR_H__ */
