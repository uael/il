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

#ifndef   JL_STMT_H__
# define  JL_STMT_H__

#include "entity_t.h"
#include "expr_t.h"
#include "stmt_t.h"
#include "type_t.h"

typedef struct jl_stmt_expr_t jl_stmt_expr_t;
typedef struct jl_stmt_label_t jl_stmt_label_t;
typedef struct jl_stmt_case_t jl_stmt_case_t;
typedef struct jl_stmt_default_t jl_stmt_default_t;
typedef struct jl_stmt_compound_t jl_stmt_compound_t;
typedef struct jl_stmt_if_t jl_stmt_if_t;
typedef struct jl_stmt_switch_t jl_stmt_switch_t;
typedef struct jl_stmt_while_t jl_stmt_while_t;
typedef struct jl_stmt_do_t jl_stmt_do_t;
typedef struct jl_stmt_for_t jl_stmt_for_t;
typedef struct jl_stmt_goto_t jl_stmt_goto_t;
typedef struct jl_stmt_break_t jl_stmt_break_t;
typedef struct jl_stmt_continue_t jl_stmt_continue_t;
typedef struct jl_stmt_return_t jl_stmt_return_t;
typedef struct jl_stmt_decl_t jl_stmt_decl_t;

struct jl_stmt_expr_t {
  unsigned refs;
  jl_expr_t expr;
};

struct jl_stmt_label_t {
  unsigned refs;
  const char *name;
};

struct jl_stmt_case_t {
  unsigned refs;
  jl_expr_t cond;
  jl_stmt_t body;
};

struct jl_stmt_default_t {
  unsigned refs;
  jl_stmt_t body;
};

struct jl_stmt_compound_t {
  unsigned refs;
  jl_stmt_r stmts;
};

struct jl_stmt_if_t {
  unsigned refs;
  jl_expr_t cond;
  jl_stmt_t then_body, else_body;
};

struct jl_stmt_switch_t {
  unsigned refs;
  jl_expr_t cond;
  jl_stmt_t body;
};

struct jl_stmt_while_t {
  unsigned refs;
  jl_expr_t cond;
  jl_stmt_t body;
};

struct jl_stmt_do_t {
  unsigned refs;
  jl_expr_t cond;
  jl_stmt_t body;
};

struct jl_stmt_for_t {
  jl_stmt_expr_t lhs, mhs;
  jl_expr_t rhs;
  jl_stmt_t body;
};

struct jl_stmt_goto_t {
  unsigned refs;
  const char *label;
};

struct jl_stmt_break_t {
  unsigned refs;
};

struct jl_stmt_continue_t {
  unsigned refs;
};

struct jl_stmt_return_t {
  unsigned refs;
  jl_expr_t expr;
};

struct jl_stmt_decl_t {
  jl_type_t decl;
};

jl_stmt_t jl_stmt_undefined();

#endif /* JL_STMT_H__ */
