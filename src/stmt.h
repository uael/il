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

#include "adt/vector.h"

struct jl_stmt_expr_t;
struct jl_stmt_label_t;
struct jl_stmt_case_t;
struct jl_stmt_default_t;
struct jl_stmt_compound_t;
struct jl_stmt_if_t;
struct jl_stmt_switch_t;
struct jl_stmt_while_t;
struct jl_stmt_do_t;
struct jl_stmt_for_t;
struct jl_stmt_goto_t;
struct jl_stmt_break_t;
struct jl_stmt_continue_t;
struct jl_stmt_return_t;
struct jl_stmt_decl_t;

typedef enum jl_stmt_n jl_stmt_n;

typedef struct jl_stmt_t jl_stmt_t;

typedef jl_vector_of(jl_stmt_t) jl_stmt_r;

enum jl_stmt_n {
  JL_STMT_EXPR,
  JL_STMT_LABEL,
  JL_STMT_CASE,
  JL_STMT_DEFAULT,
  JL_STMT_COMPOUND,
  JL_STMT_IF,
  JL_STMT_SWITCH,
  JL_STMT_WHILE,
  JL_STMT_DO,
  JL_STMT_FOR,
  JL_STMT_GOTO,
  JL_STMT_BREAK,
  JL_STMT_CONTINUE,
  JL_STMT_RETURN,
  JL_STMT_DECL
};

struct jl_stmt_t {
  jl_stmt_n kind;
  union {
    struct jl_stmt_expr_t *_expr;
    struct jl_stmt_label_t *_label;
    struct jl_stmt_case_t *_case;
    struct jl_stmt_default_t *_default;
    struct jl_stmt_compound_t *_compound;
    struct jl_stmt_if_t *_if;
    struct jl_stmt_switch_t *_switch;
    struct jl_stmt_while_t *_while;
    struct jl_stmt_do_t *_do;
    struct jl_stmt_for_t *_for;
    struct jl_stmt_goto_t *_goto;
    struct jl_stmt_break_t *_break;
    struct jl_stmt_continue_t *_continue;
    struct jl_stmt_return_t *_return;
    struct jl_stmt_decl_t *_decl;
  };
};

void jl_stmt_init(jl_stmt_t *self, jl_stmt_n kind, void *ptr);
void jl_stmt_dtor(jl_stmt_t *self);

#endif /* JL_STMT_H__ */
