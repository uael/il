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

#ifndef   JL_FVAL_H__
# define  JL_FVAL_H__

#include "entity_t.h"
#include "expr_t.h"
#include "stmt_t.h"
#include "type_t.h"

typedef enum jl_fval_n jl_fval_n;

typedef struct jl_fval_t jl_fval_t;

enum jl_fval_n {
  JL_FVAL_UNDEFINED = 0,
  JL_FVAL_INT,
  JL_FVAL_FLOAT,
  JL_FVAL_STRING,
  JL_FVAL_TOKEN,
  JL_FVAL_ENTITY,
  JL_FVAL_FUNC_SPECIFIER,
  JL_FVAL_EXPR,
  JL_FVAL_OP,
  JL_FVAL_STMT,
  JL_FVAL_TYPE,
  JL_FVAL_LITERAL,
  JL_FVAL_TYPE_SPECIFIER,
  JL_FVAL_TYPE_QUALIFIER
};

struct jl_fval_t {
  jl_fval_n kind : 8;
  jl_token_t begin, end;
  union {
    int d;
    float f;
    const char *s;
    jl_token_t token;
    jl_entity_t entity;
    jl_func_specifier_n func_specifier;
    jl_expr_t expr;
    jl_op_n op;
    jl_stmt_t stmt;
    jl_type_t type;
    jl_literal_n literal;
    jl_type_specifier_n type_specifier;
    jl_type_qualifier_n type_qualifier;
  };
};

jl_fval_t jl_fval_undefined();
jl_fval_t jl_fval_string(const char *s);
jl_fval_t jl_fval_token(jl_token_t token);
jl_fval_t jl_fval_expr(jl_expr_t expr);
void jl_fval_undef(jl_fval_t *self);
void jl_fval_begin(jl_fval_t *self, jl_token_t token);
void jl_fval_end(jl_fval_t *self, jl_token_t token);

#endif /* JL_FVAL_H__ */
