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

#include <stdlib.h>

#include "expr.h"

#include "entity.h"
#include "stmt.h"
#include "type.h"

void jl_expr_id_dtor(jl_expr_t *self);
void jl_expr_const_dtor(jl_expr_t *self);
void jl_expr_unary_dtor(jl_expr_t *self);
void jl_expr_binary_dtor(jl_expr_t *self);
void jl_expr_ternary_dtor(jl_expr_t *self);
void jl_expr_array_read_dtor(jl_expr_t *self);
void jl_expr_array_write_dtor(jl_expr_t *self);
void jl_expr_field_read_dtor(jl_expr_t *self);
void jl_expr_field_write_dtor(jl_expr_t *self);
void jl_expr_call_dtor(jl_expr_t *self);
void jl_expr_list_dtor(jl_expr_t *self);

void jl_expr_dtor(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_UNDEFINED:
      return;
    case JL_EXPR_ID:
      if (self->u._id->refs <= 0) {
        jl_expr_id_dtor(self);
        free(self->u._id);
        self->u._id = NULL;
      }
      break;
    case JL_EXPR_CONST:
      if (self->u._const->refs <= 0) {
        jl_expr_const_dtor(self);
        free(self->u._const);
        self->u._const = NULL;
      }
      break;
    case JL_EXPR_UNARY:
      if (self->u._unary->refs <= 0) {
        jl_expr_unary_dtor(self);
        free(self->u._unary);
        self->u._unary = NULL;
      }
      break;
    case JL_EXPR_BINARY:
      if (self->u._binary->refs <= 0) {
        jl_expr_binary_dtor(self);
        free(self->u._binary);
        self->u._binary = NULL;
      }
      break;
    case JL_EXPR_TERNARY:
      if (self->u._ternary->refs <= 0) {
        jl_expr_ternary_dtor(self);
        free(self->u._ternary);
        self->u._ternary = NULL;
      }
      break;
    case JL_EXPR_ARRAY_READ:
      if (self->u._array_read->refs <= 0) {
        jl_expr_array_read_dtor(self);
        free(self->u._array_read);
        self->u._array_read = NULL;
      }
      break;
    case JL_EXPR_ARRAY_WRITE:
      if (self->u._array_write->refs <= 0) {
        jl_expr_array_write_dtor(self);
        free(self->u._array_write);
        self->u._array_write = NULL;
      }
      break;
    case JL_EXPR_FIELD_READ:
      if (self->u._field_read->refs <= 0) {
        jl_expr_field_read_dtor(self);
        free(self->u._field_read);
        self->u._field_read = NULL;
      }
      break;
    case JL_EXPR_FIELD_WRITE:
      if (self->u._field_write->refs <= 0) {
        jl_expr_field_write_dtor(self);
        free(self->u._field_write);
        self->u._field_write = NULL;
      }
      break;
    case JL_EXPR_CALL:
      if (self->u._call->refs <= 0) {
        jl_expr_call_dtor(self);
        free(self->u._call);
        self->u._call = NULL;
      }
      break;
    case JL_EXPR_LIST:
      if (self->u._list->refs <= 0) {
        jl_expr_list_dtor(self);
        free(self->u._list);
        self->u._list = NULL;
      }
      break;
    default:
      break;
  }
  *self = jl_expr_undefined();
}

void jl_expr_switch(jl_expr_t *self, enum jl_expr_n kind) {
  if (self->kind != kind || !jl_expr_is_defined(self)) {
    jl_expr_dtor(self);
    *self = (jl_expr_t) {
      .kind = kind
    };
    switch (kind) {
      case JL_EXPR_UNDEFINED:
        break;
      case JL_EXPR_ID:
        self->u._id = xmalloc(sizeof(jl_expr_id_t));
        break;
      case JL_EXPR_CONST:
        self->u._id = xmalloc(sizeof(jl_expr_const_t));
        break;
      case JL_EXPR_UNARY:
        self->u._id = xmalloc(sizeof(jl_expr_unary_t));
        break;
      case JL_EXPR_BINARY:
        self->u._id = xmalloc(sizeof(jl_expr_binary_t));
        break;
      case JL_EXPR_TERNARY:
        self->u._ternary = xmalloc(sizeof(jl_expr_ternary_t));
        break;
      case JL_EXPR_ARRAY_READ:
        self->u._array_read = xmalloc(sizeof(struct jl_expr_array_read_t));
        break;
      case JL_EXPR_ARRAY_WRITE:
        self->u._array_write = xmalloc(sizeof(jl_expr_array_write_t));
        break;
      case JL_EXPR_FIELD_READ:
        self->u._field_read = xmalloc(sizeof(jl_expr_field_read_t));
        break;
      case JL_EXPR_FIELD_WRITE:
        self->u._field_write = xmalloc(sizeof(jl_expr_field_write_t));
        break;
      case JL_EXPR_CALL:
        self->u._call = xmalloc(sizeof(jl_expr_call_t));
        break;
      case JL_EXPR_LIST:
        self->u._list = xmalloc(sizeof(jl_expr_list_t));
        break;
    }
  }
}

void jl_expr_acquire(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_UNDEFINED:
      puts("cannot acquire undefined type");
      exit(1);
    case JL_EXPR_ID:
      ++self->u._id->refs;
      break;
    case JL_EXPR_CONST:
      ++self->u._const->refs;
      break;
    case JL_EXPR_UNARY:
      ++self->u._unary->refs;
      break;
    case JL_EXPR_BINARY:
      ++self->u._binary->refs;
      break;
    case JL_EXPR_TERNARY:
      ++self->u._ternary->refs;
      break;
    case JL_EXPR_ARRAY_READ:
      ++self->u._array_read->refs;
      break;
    case JL_EXPR_ARRAY_WRITE:
      ++self->u._array_write->refs;
      break;
    case JL_EXPR_FIELD_READ:
      ++self->u._field_read->refs;
      break;
    case JL_EXPR_FIELD_WRITE:
      ++self->u._field_write->refs;
      break;
    case JL_EXPR_CALL:
      ++self->u._call->refs;
      break;
    case JL_EXPR_LIST:
      ++self->u._list->refs;
      break;
    default:
      break;
  }
}

void jl_expr_release(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_UNDEFINED:
      puts("cannot release undefined type");
      exit(1);
    case JL_EXPR_ID:
      --self->u._id->refs;
      break;
    case JL_EXPR_CONST:
      --self->u._const->refs;
      break;
    case JL_EXPR_UNARY:
      --self->u._unary->refs;
      break;
    case JL_EXPR_BINARY:
      --self->u._binary->refs;
      break;
    case JL_EXPR_TERNARY:
      --self->u._ternary->refs;
      break;
    case JL_EXPR_ARRAY_READ:
      --self->u._array_read->refs;
      break;
    case JL_EXPR_ARRAY_WRITE:
      --self->u._array_write->refs;
      break;
    case JL_EXPR_FIELD_READ:
      --self->u._field_read->refs;
      break;
    case JL_EXPR_FIELD_WRITE:
      --self->u._field_write->refs;
      break;
    case JL_EXPR_CALL:
      --self->u._call->refs;
      break;
    case JL_EXPR_LIST:
      --self->u._list->refs;
      break;
    default:
      break;
  }
}

bool jl_expr_is_defined(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      return self->u._id != NULL;
    case JL_EXPR_CONST:
      return self->u._const != NULL;
    case JL_EXPR_UNARY:
      return self->u._unary != NULL;
    case JL_EXPR_BINARY:
      return self->u._binary != NULL;
    case JL_EXPR_TERNARY:
      return self->u._ternary != NULL;
    case JL_EXPR_ARRAY_READ:
      return self->u._array_read != NULL;
    case JL_EXPR_ARRAY_WRITE:
      return self->u._array_write != NULL;
    case JL_EXPR_FIELD_READ:
      return self->u._field_read != NULL;
    case JL_EXPR_FIELD_WRITE:
      return self->u._field_write != NULL;
    case JL_EXPR_CALL:
      return self->u._call != NULL;
    case JL_EXPR_LIST:
      return self->u._list != NULL;
    case JL_EXPR_UNDEFINED:
    default:
      return false;
  }
}

jl_type_t jl_expr_get_type(jl_expr_t self) {
  return self.type;
}

void jl_expr_set_type(jl_expr_t *self, jl_type_t type) {
  self->type = type;
}


jl_expr_t jl_id(const char *id, bool is_keyword) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_id_init(&expr, id, is_keyword);
  return expr;
}

void jl_id_init(jl_expr_t *self, const char *id, bool is_keyword) {
  jl_expr_switch(self, JL_EXPR_ID);
  jl_pexpr_id(self)->id = id;
  jl_pexpr_id(self)->is_keyword = is_keyword;
}

void jl_expr_id_dtor(jl_expr_t *self) {}


jl_expr_t jl_const_int(int d) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_const_init(&expr, jl_int());
  jl_expr_const(expr)->u.i = d;
  return expr;
}

jl_expr_t jl_const_float(float f) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_const_init(&expr, jl_float());
  jl_expr_const(expr)->u.f = f;
  return expr;
}

jl_expr_t jl_const_string(const char *s) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_const_init(&expr, jl_string());
  jl_expr_const(expr)->u.s = s;
  return expr;
}

void jl_const_init(jl_expr_t *self, jl_type_t type) {
  jl_expr_switch(self, JL_EXPR_CONST);
  jl_expr_set_type(self, type);
}


void jl_expr_const_dtor(jl_expr_t *self) {}


jl_expr_t jl_unary(enum jl_op_n op, jl_expr_t operand) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_unary_init(&expr, op, operand);
  return expr;
}

jl_expr_t jl_cast(jl_type_t type, jl_expr_t operand) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_unary_init(&expr, JL_OP_CAST, operand);
  jl_expr_set_type(&expr, type);
  return expr;
}

void jl_unary_init(jl_expr_t *self, enum jl_op_n op, jl_expr_t operand) {
  jl_expr_switch(self, JL_EXPR_UNARY);
  jl_pexpr_unary(self)->op = op;
  jl_pexpr_unary(self)->operand = operand;
}

void jl_expr_unary_dtor(jl_expr_t *self) {}


jl_expr_t jl_binary(enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_binary_init(&expr, op, lhs, rhs);
  return expr;
}

void jl_binary_init(jl_expr_t *self, enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs) {
  jl_expr_switch(self, JL_EXPR_BINARY);
  jl_pexpr_binary(self)->lhs = lhs;
  jl_pexpr_binary(self)->rhs = rhs;
}

void jl_expr_binary_dtor(jl_expr_t *self) {}


jl_expr_t jl_ternary(jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_ternary_init(&expr, lhs, mhs, rhs);
  return expr;
}

void jl_ternary_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs) {
  jl_expr_switch(self, JL_EXPR_TERNARY);
  jl_pexpr_ternary(self)->lhs = lhs;
  jl_pexpr_ternary(self)->mhs = mhs;
  jl_pexpr_ternary(self)->rhs = rhs;
}

void jl_expr_ternary_dtor(jl_expr_t *self) {}


jl_expr_t jl_array_read(jl_expr_t lhs, jl_expr_t pos) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_array_read_init(&expr, lhs, pos);
  return expr;
}

void jl_array_read_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t pos) {
  jl_expr_switch(self, JL_EXPR_ARRAY_READ);
  jl_pexpr_array_read(self)->lhs = lhs;
  jl_pexpr_array_read(self)->pos = pos;
}

void jl_expr_array_read_dtor(jl_expr_t *self) {}


jl_expr_t jl_array_write(jl_expr_t lhs, jl_expr_t pos, jl_expr_t rhs) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_array_write_init(&expr, lhs, pos, rhs);
  return expr;
}

void jl_array_write_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t pos, jl_expr_t rhs) {
  jl_expr_switch(self, JL_EXPR_ARRAY_WRITE);
  jl_pexpr_array_write(self)->lhs = lhs;
  jl_pexpr_array_write(self)->pos = pos;
  jl_pexpr_array_write(self)->rhs = rhs;
}

void jl_expr_array_write_dtor(jl_expr_t *self) {}


jl_expr_t jl_field_read(jl_expr_t lhs, bool ptr, jl_expr_t field) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_field_read_init(&expr, lhs, ptr, field);
  return expr;
}

void jl_field_read_init(jl_expr_t *self, jl_expr_t lhs, bool ptr, jl_expr_t field) {
  jl_expr_switch(self, JL_EXPR_FIELD_READ);
  jl_pexpr_field_read(self)->lhs = lhs;
  jl_pexpr_field_read(self)->ptr = ptr;
  jl_pexpr_field_read(self)->field = field;
}

void jl_expr_field_read_dtor(jl_expr_t *self) {}


jl_expr_t jl_field_write(jl_expr_t lhs, bool ptr, jl_expr_t field, jl_expr_t rhs) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_field_write_init(&expr, lhs, ptr, field, rhs);
  return expr;
}

void jl_field_write_init(jl_expr_t *self, jl_expr_t lhs, bool ptr, jl_expr_t field, jl_expr_t rhs) {
  jl_expr_switch(self, JL_EXPR_FIELD_WRITE);
  jl_pexpr_field_write(self)->lhs = lhs;
  jl_pexpr_field_write(self)->ptr = ptr;
  jl_pexpr_field_write(self)->field = field;
  jl_pexpr_field_write(self)->rhs = rhs;
}

void jl_expr_field_write_dtor(jl_expr_t *self) {}


jl_expr_t jl_call(jl_expr_t lhs, jl_expr_r args) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_call_init(&expr, lhs, args);
  return expr;
}

void jl_call_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_r args) {
  jl_expr_switch(self, JL_EXPR_CALL);
  jl_pexpr_call(self)->lhs = lhs;
  jl_pexpr_call(self)->args = args;
}

void jl_expr_call_dtor(jl_expr_t *self) {
  adt_vector_dtor(jl_pexpr_call(self)->args);
}

jl_expr_t jl_exprs(jl_expr_r exprs) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_exprs_init(&expr, exprs);
  return expr;
}

void jl_exprs_init(jl_expr_t *self, jl_expr_r exprs) {
  jl_expr_switch(self, JL_EXPR_LIST);
  jl_pexpr_list(self)->exprs = exprs;
}

size_t jl_exprs_length(jl_expr_t *self) {
  return adt_vector_length(jl_pexpr_list(self)->exprs);
}

void jl_exprs_push(jl_expr_t *self, jl_expr_t expr) {
  jl_expr_r exprs = jl_pexpr_list(self)->exprs;
  adt_vector_push(exprs, expr);
}
jl_expr_t jl_exprs_pop(jl_expr_t *self) {
  return adt_vector_pop(jl_pexpr_list(self)->exprs);
}

void jl_exprs_unshift(jl_expr_t *self, jl_expr_t expr) {
  jl_expr_r exprs = jl_pexpr_list(self)->exprs;
  adt_vector_unshift(exprs, expr);
}

jl_expr_t jl_exprs_shift(jl_expr_t *self) {
  jl_expr_r exprs = jl_pexpr_list(self)->exprs;
  jl_expr_t expr = adt_vector_front(exprs);
  adt_vector_shift(exprs);
  return expr;
}

void jl_expr_list_dtor(jl_expr_t *self) {
  adt_vector_dtor(jl_pexpr_list(self)->exprs);
}
