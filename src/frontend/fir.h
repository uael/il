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

#ifndef   JL_FIR_H__
# define  JL_FIR_H__

#include "entity_t.h"
#include "expr_t.h"
#include "stmt_t.h"
#include "type_t.h"

typedef struct jl_fir_t jl_fir_t;

enum jl_fir_n {
  JL_FIR_UNDEFINED = 0,
  JL_FIR_INT,
  JL_FIR_FLOAT,
  JL_FIR_STRING,
  JL_FIR_TOKEN,
  JL_FIR_ENTITY,
  JL_FIR_FUNC_SPECIFIER,
  JL_FIR_EXPR,
  JL_FIR_OP,
  JL_FIR_STMT,
  JL_FIR_TYPE,
  JL_FIR_LITERAL,
  JL_FIR_TYPE_SPECIFIER,
  JL_FIR_TYPE_QUALIFIER
};

struct jl_fir_t {
  unsigned kind : 8;
  jl_token_t begin, end;
  union {
    int d;
    float f;
    const char *s;
    enum jl_func_specifier_n func_specifier;
    enum jl_op_n op;
    enum jl_literal_n literal;
    enum jl_type_specifier_n type_specifier;
    enum jl_type_qualifier_n type_qualifier;
    jl_token_t token;
    jl_entity_t entity;
    jl_expr_t expr;
    jl_stmt_t stmt;
    jl_type_t type;
  } u;
};

jl_fir_t jl_fir_undefined();
void jl_fir_init_string(jl_fir_t *self, const char *s);
void jl_fir_init_token(jl_fir_t *self, jl_token_t token);
void jl_fir_init_expr(jl_fir_t *self, jl_expr_t expr);
void jl_fir_undef(jl_fir_t *self);
void jl_fir_begin(jl_fir_t *self, jl_token_t token);
void jl_fir_end(jl_fir_t *self, jl_token_t token);

#endif /* JL_FIR_H__ */
