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

#ifndef   JL_EXPRS_H__
# define  JL_EXPRS_H__

#include <stdbool.h>

#include "expr.h"
#include "types.h"

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

struct jl_expr_id_t {
  unsigned refs;
  const char *id;
  bool is_keyword;
};

void jl_expr_id_init(jl_expr_t *self, const char *id, bool is_keyword);
void jl_expr_id_dtor(jl_expr_t *self);

struct jl_expr_const_t {
  unsigned refs;
  jl_type_t type;
  union {
    char *s;
    int i;
    float f;
  };
};

void jl_expr_const_init(jl_expr_t *self, jl_type_t type);
void jl_expr_const_dtor(jl_expr_t *self);

struct jl_expr_unary_t {
  unsigned refs;
  jl_type_t type;
  jl_op_n op;
  jl_expr_t operand;
};

void jl_expr_unary_init(jl_expr_t *self, jl_op_n op, jl_expr_t operand);
void jl_expr_unary_dtor(jl_expr_t *self);

struct jl_expr_binary_t {
  unsigned refs;
  jl_type_t type;
  jl_op_n op;
  jl_expr_t lhs, rhs;
};

void jl_expr_binary_init(jl_expr_t *self, jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
void jl_expr_binary_dtor(jl_expr_t *self);

struct jl_expr_ternary_t {
  unsigned refs;
  jl_type_t type;
  jl_op_n op1, op2;
  jl_expr_t lhs, mhs, rhs;
};

void jl_expr_ternary_init(jl_expr_t *self, jl_op_n op1, jl_op_n op2, jl_expr_t lhs, jl_expr_t mhs,
  jl_expr_t rhs);
void jl_expr_ternary_dtor(jl_expr_t *self);

struct jl_expr_array_read_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t lhs, pos;
};

void jl_expr_array_read_dtor(jl_expr_t *self);

struct jl_expr_array_write_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t lhs, pos, rhs;
};

void jl_expr_array_write_dtor(jl_expr_t *self);

struct jl_expr_field_read_t {
  unsigned refs;
  jl_type_t type;
  jl_op_n op;
  jl_expr_t lhs;
  jl_expr_id_t field;
};

void jl_expr_field_read_dtor(jl_expr_t *self);

struct jl_expr_field_write_t {
  unsigned refs;
  jl_type_t type;
  jl_op_n op;
  jl_expr_t lhs, rhs;
  jl_expr_id_t field;
};

void jl_expr_field_write_dtor(jl_expr_t *self);

struct jl_expr_call_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t lhs;
  jl_expr_r args;
};

void jl_expr_call_dtor(jl_expr_t *self);

#endif /* JL_EXPRS_H__ */
