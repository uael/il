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

#include <adt/xmalloc.h>

#include "expr.h"
#include "exprs.h"

void jl_expr_init(jl_expr_t *self, jl_expr_n kind, jl_loc_t loc, void *ptr) {
  *self = (jl_expr_t) {
    .kind = kind,
    .loc = loc
  };

  switch (kind) {
    case JL_EXPR_ID:
      self->_id = ptr ? ptr : xmalloc(sizeof(jl_expr_id_t));
      break;
    case JL_EXPR_CONST:
      self->_const = ptr ? ptr : xmalloc(sizeof(jl_expr_const_t));
      break;
    case JL_EXPR_UNARY:
      self->_unary = ptr ? ptr : xmalloc(sizeof(jl_expr_unary_t));
      break;
    case JL_EXPR_BINARY:
      self->_binary = ptr ? ptr : xmalloc(sizeof(jl_expr_binary_t));
      break;
    case JL_EXPR_TERNARY:
      self->_ternary = ptr ? ptr : xmalloc(sizeof(jl_expr_ternary_t));
      break;
    case JL_EXPR_ARRAY_READ:
      self->_array_read = ptr ? ptr : xmalloc(sizeof(jl_expr_array_read_t));
      break;
    case JL_EXPR_ARRAY_WRITE:
      self->_array_write = ptr ? ptr : xmalloc(sizeof(jl_expr_array_write_t));
      break;
    case JL_EXPR_FIELD_READ:
      self->_field_read = ptr ? ptr : xmalloc(sizeof(jl_expr_field_read_t));
      break;
    case JL_EXPR_FIELD_WRITE:
      self->_field_write = ptr ? ptr : xmalloc(sizeof(jl_expr_field_write_t));
      break;
    case JL_EXPR_CALL:
      self->_call = ptr ? ptr : xmalloc(sizeof(jl_expr_call_t));
      break;
  }
}

void jl_expr_dtor(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      if (self->_id->refs <= 0) {
        jl_expr_id_dtor(self->_id);
        free(self->_id);
        self->_id = NULL;
      }
      break;
    case JL_EXPR_CONST:
      if (self->_const->refs <= 0) {
        jl_expr_const_dtor(self->_const);
        free(self->_const);
        self->_const = NULL;
      }
      break;
    case JL_EXPR_UNARY:
      if (self->_unary->refs <= 0) {
        jl_expr_unary_dtor(self->_unary);
        free(self->_unary);
        self->_unary = NULL;
      }
      break;
    case JL_EXPR_BINARY:
      if (self->_binary->refs <= 0) {
        jl_expr_binary_dtor(self->_binary);
        free(self->_binary);
        self->_binary = NULL;
      }
      break;
    case JL_EXPR_TERNARY:
      if (self->_ternary->refs <= 0) {
        jl_expr_ternary_dtor(self->_ternary);
        free(self->_ternary);
        self->_ternary = NULL;
      }
      break;
    case JL_EXPR_ARRAY_READ:
      if (self->_array_read->refs <= 0) {
        jl_expr_array_read_dtor(self->_array_read);
        free(self->_array_read);
        self->_array_read = NULL;
      }
      break;
    case JL_EXPR_ARRAY_WRITE:
      if (self->_array_write->refs <= 0) {
        jl_expr_array_write_dtor(self->_array_write);
        free(self->_array_write);
        self->_array_write = NULL;
      }
      break;
    case JL_EXPR_FIELD_READ:
      if (self->_field_read->refs <= 0) {
        jl_expr_field_read_dtor(self->_field_read);
        free(self->_field_read);
        self->_field_read = NULL;
      }
      break;
    case JL_EXPR_FIELD_WRITE:
      if (self->_field_write->refs <= 0) {
        jl_expr_field_write_dtor(self->_field_write);
        free(self->_field_write);
        self->_field_write = NULL;
      }
      break;
    case JL_EXPR_CALL:
      if (self->_call->refs <= 0) {
        jl_expr_call_dtor(self->_call);
        free(self->_call);
        self->_call = NULL;
      }
      break;
  }
}
