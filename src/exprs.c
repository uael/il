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

#include "exprs.h"

static void jl_expr_init(jl_expr_t *self, jl_expr_n kind, jl_loc_t loc, void *ptr) {
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
    default:
      break;
  }
}

void jl_expr_id_init(jl_expr_t *self, const char *id, bool is_keyword) {
  jl_expr_init(self, JL_EXPR_ID, (jl_loc_t){0}, NULL);
  *self->_id = (jl_expr_id_t) {
    .id = id,
    .is_keyword = is_keyword
  };
}

void jl_expr_id_dtor(jl_expr_t *self) {}


void jl_expr_const_init(jl_expr_t *self, jl_type_t type) {
  jl_expr_init(self, JL_EXPR_CONST, (jl_loc_t){0}, NULL);
  *self->_const = (jl_expr_const_t) {
    .type = type
  };
}

void jl_expr_const_dtor(jl_expr_t *self) {}

void jl_expr_unary_init(jl_expr_t *self, jl_op_n op, jl_expr_t operand) {

}

void jl_expr_unary_dtor(jl_expr_t *self) {

}

void jl_expr_binary_init(jl_expr_t *self, jl_op_n op, jl_expr_t lhs, jl_expr_t rhs) {

}

void jl_expr_binary_dtor(jl_expr_t *self) {

}

void jl_expr_ternary_init(jl_expr_t *self, jl_op_n op1, jl_op_n op2, jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs) {

}

void jl_expr_ternary_dtor(jl_expr_t *self) {

}

void jl_expr_array_read_dtor(jl_expr_t *self) {

}

void jl_expr_array_write_dtor(jl_expr_t *self) {

}

void jl_expr_field_read_dtor(jl_expr_t *self) {

}

void jl_expr_field_write_dtor(jl_expr_t *self) {

}

void jl_expr_call_dtor(jl_expr_t *self) {

}
