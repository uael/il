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

#include "fval.h"

jl_fval_t jl_fval_undefined() {
  return (jl_fval_t) {JL_FVAL_UNDEFINED};
}

void jl_fval_init_string(jl_fval_t *self, const char *s) {
  self->kind = JL_FVAL_STRING;
  self->u.s = s;
}

void jl_fval_init_token(jl_fval_t *self, jl_token_t token) {
  self->kind = JL_FVAL_TOKEN;
  self->begin = token;
  self->end = token;
  self->u.token = token;
}

void jl_fval_init_expr(jl_fval_t *self, jl_expr_t expr) {
  self->kind = JL_FVAL_EXPR;
  self->u.expr = expr;
}

void jl_fval_undef(jl_fval_t *self) {
  *self = jl_fval_undefined();
}

void jl_fval_begin(jl_fval_t *self, jl_token_t token) {
  self->begin = token;
}

void jl_fval_end(jl_fval_t *self, jl_token_t token) {
  self->end = token;
}
