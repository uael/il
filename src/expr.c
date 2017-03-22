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

jl_expr_t jl_expr_undefined() {
  return (jl_expr_t){JL_EXPR_UNDEFINED};
}

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
    case JL_EXPR_UNDEFINED:
    default:
      return false;
  }
}

jl_type_t jl_expr_get_type(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      return self->u._id->type;
    case JL_EXPR_CONST:
      return self->u._const->type;
    case JL_EXPR_UNARY:
      return self->u._unary->type;
    case JL_EXPR_BINARY:
      return self->u._binary->type;
    case JL_EXPR_TERNARY:
      return self->u._ternary->type;
    case JL_EXPR_ARRAY_READ:
      return self->u._array_read->type;
    case JL_EXPR_ARRAY_WRITE:
      return self->u._array_write->type;
    case JL_EXPR_FIELD_READ:
      return self->u._field_read->type;
    case JL_EXPR_FIELD_WRITE:
      return self->u._field_write->type;
    case JL_EXPR_CALL:
      return self->u._call->type;
    case JL_EXPR_UNDEFINED:
    default:
      puts("cannot access next on undefined expression");
      exit(1);
  }
}

void jl_expr_set_type(jl_expr_t *self, jl_type_t type) {
  switch (self->kind) {
    case JL_EXPR_ID:
      self->u._id->type = type;
      break;
    case JL_EXPR_CONST:
      self->u._const->type = type;
      break;
    case JL_EXPR_UNARY:
      self->u._unary->type = type;
      break;
    case JL_EXPR_BINARY:
      self->u._binary->type = type;
      break;
    case JL_EXPR_TERNARY:
      self->u._ternary->type = type;
      break;
    case JL_EXPR_ARRAY_READ:
      self->u._array_read->type = type;
      break;
    case JL_EXPR_ARRAY_WRITE:
      self->u._array_write->type = type;
      break;
    case JL_EXPR_FIELD_READ:
      self->u._field_read->type = type;
      break;
    case JL_EXPR_FIELD_WRITE:
      self->u._field_write->type = type;
      break;
    case JL_EXPR_CALL:
      self->u._call->type = type;
      break;
    case JL_EXPR_UNDEFINED:
    default:
      puts("cannot set type on undefined expression");
      exit(1);
  }
}

jl_expr_t jl_expr_get_next(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      return self->u._id->next;
    case JL_EXPR_CONST:
      return self->u._const->next;
    case JL_EXPR_UNARY:
      return self->u._unary->next;
    case JL_EXPR_BINARY:
      return self->u._binary->next;
    case JL_EXPR_TERNARY:
      return self->u._ternary->next;
    case JL_EXPR_ARRAY_READ:
      return self->u._array_read->next;
    case JL_EXPR_ARRAY_WRITE:
      return self->u._array_write->next;
    case JL_EXPR_FIELD_READ:
      return self->u._field_read->next;
    case JL_EXPR_FIELD_WRITE:
      return self->u._field_write->next;
    case JL_EXPR_CALL:
      return self->u._call->next;
    case JL_EXPR_UNDEFINED:
    default:
      puts("cannot access next on undefined expression");
      exit(1);
  }
}

void jl_expr_set_next(jl_expr_t *self, jl_expr_t next) {
  switch (self->kind) {
    case JL_EXPR_ID:
      self->u._id->next = next;
      break;
    case JL_EXPR_CONST:
      self->u._const->next = next;
      break;
    case JL_EXPR_UNARY:
      self->u._unary->next = next;
      break;
    case JL_EXPR_BINARY:
      self->u._binary->next = next;
      break;
    case JL_EXPR_TERNARY:
      self->u._ternary->next = next;
      break;
    case JL_EXPR_ARRAY_READ:
      self->u._array_read->next = next;
      break;
    case JL_EXPR_ARRAY_WRITE:
      self->u._array_write->next = next;
      break;
    case JL_EXPR_FIELD_READ:
      self->u._field_read->next = next;
      break;
    case JL_EXPR_FIELD_WRITE:
      self->u._field_write->next = next;
      break;
    case JL_EXPR_CALL:
      self->u._call->next = next;
      break;
    case JL_EXPR_UNDEFINED:
    default:
      puts("cannot set next on undefined expression");
      exit(1);
  }
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

void jl_unary_init(jl_expr_t *self, enum jl_op_n op, jl_expr_t operand) {
  jl_expr_switch(self, JL_EXPR_UNARY);
  jl_pexpr_unary(self)->op = op;
  jl_pexpr_unary(self)->operand = operand;
}

void jl_expr_unary_dtor(jl_expr_t *self) {}


jl_expr_t jl_binary(enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
void jl_binary_init(jl_expr_t *self, enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
void jl_expr_binary_dtor(jl_expr_t *self) {}
enum jl_op_n jl_binary_get_op(jl_expr_t *self);
void jl_binary_set_op(jl_expr_t *self, enum jl_op_n op);
jl_expr_t jl_binary_get_lhs(jl_expr_t *self);
void jl_binary_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_binary_get_rhs(jl_expr_t *self);
void jl_binary_set_rhs(jl_expr_t *self, jl_expr_t rhs);


jl_expr_t jl_ternary(jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs);
void jl_ternary_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs);
void jl_expr_ternary_dtor(jl_expr_t *self) {}
jl_expr_t jl_ternary_get_lhs(jl_expr_t *self);
void jl_ternary_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_ternary_get_mhs(jl_expr_t *self);
void jl_ternary_set_mhs(jl_expr_t *self, jl_expr_t mhs);
jl_expr_t jl_ternary_get_rhs(jl_expr_t *self);
void jl_ternary_set_rhs(jl_expr_t *self, jl_expr_t rhs);


jl_expr_t jl_array_read(jl_expr_t lhs, jl_expr_t pos);
void jl_array_read_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t pos);
void jl_expr_array_read_dtor(jl_expr_t *self) {}
jl_expr_t jl_array_read_get_lhs(jl_expr_t *self);
void jl_array_read_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_array_read_get_pos(jl_expr_t *self);
void jl_array_read_set_pos(jl_expr_t *self, jl_expr_t pos);


jl_expr_t jl_array_write(jl_expr_t lhs, jl_expr_t pos, jl_expr_t rhs);
void jl_array_write_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t pos, jl_expr_t rhs);
void jl_expr_array_write_dtor(jl_expr_t *self) {}
jl_expr_t jl_array_write_get_lhs(jl_expr_t *self);
void jl_array_write_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_array_write_get_pos(jl_expr_t *self);
void jl_array_write_set_pos(jl_expr_t *self, jl_expr_t pos);
jl_expr_t jl_array_write_get_rhs(jl_expr_t *self);
void jl_array_write_set_rhs(jl_expr_t *self, jl_expr_t rhs);


jl_expr_t jl_field_read(jl_expr_t lhs, bool ptr, jl_expr_t field);
void jl_field_read_init(jl_expr_t *self, jl_expr_t lhs, bool ptr, jl_expr_t field);
void jl_expr_field_read_dtor(jl_expr_t *self) {}
jl_expr_t jl_field_read_get_lhs(jl_expr_t *self);
void jl_field_read_set_lhs(jl_expr_t *self, jl_expr_t lhs);
bool jl_field_read_get_ptr(jl_expr_t *self);
void jl_field_read_set_ptr(jl_expr_t *self, bool ptr);
jl_expr_t jl_field_read_get_field(jl_expr_t *self);
void jl_field_read_set_field(jl_expr_t *self, jl_expr_t field);


jl_expr_t jl_field_write(jl_expr_t lhs, bool ptr, jl_expr_t field, jl_expr_t rhs);
void jl_field_write_init(jl_expr_t *self, jl_expr_t lhs, bool ptr, jl_expr_t field, jl_expr_t rhs);
void jl_expr_field_write_dtor(jl_expr_t *self) {}
jl_expr_t jl_field_write_get_lhs(jl_expr_t *self);
void jl_field_write_set_lhs(jl_expr_t *self, jl_expr_t lhs);
bool jl_field_write_get_ptr(jl_expr_t *self);
void jl_field_write_set_ptr(jl_expr_t *self, bool ptr);
jl_expr_t jl_field_write_get_field(jl_expr_t *self);
void jl_field_write_set_field(jl_expr_t *self, jl_expr_t field);
jl_expr_t jl_field_write_get_rhs(jl_expr_t *self);
void jl_field_write_set_rhs(jl_expr_t *self, jl_expr_t rhs);


jl_expr_t jl_call(jl_expr_t lhs, jl_expr_t args);
void jl_call_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t args);
void jl_expr_call_dtor(jl_expr_t *self) {}
jl_expr_t jl_call_get_lhs(jl_expr_t *self);
void jl_call_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_call_get_args(jl_expr_t *self);
void jl_call_set_args(jl_expr_t *self, jl_expr_t args);
