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

#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

#define jl_expr_id(t) ((void) assert(jl_expr_is_id(*t)), (t)->_id)
#define jl_expr_const(t) ((void) assert(jl_expr_is_const(*t)), (t)->_const)
#define jl_expr_unary(t) ((void) assert(jl_expr_is_unary(*t)), (t)->_unary)
#define jl_expr_binary(t) ((void) assert(jl_expr_is_binary(*t)), (t)->_binary)
#define jl_expr_ternary(t) ((void) assert(jl_expr_is_ternary(*t)), (t)->_ternary)
#define jl_expr_array_read(t) ((void) assert(jl_expr_is_array_read(*t)), (t)->_array_read)
#define jl_expr_array_write(t) ((void) assert(jl_expr_is_array_write(*t)), (t)->_array_write)
#define jl_expr_field_read(t) ((void) assert(jl_expr_is_field_read(*t)), (t)->_field_read)
#define jl_expr_field_write(t) ((void) assert(jl_expr_is_field_write(*t)), (t)->_field_write)
#define jl_expr_call(t) ((void) assert(jl_expr_is_call(*t)), (t)->_call)

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

struct jl_expr_id_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  const char *id;
  bool is_keyword;
};

struct jl_expr_const_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  union {
    const char *s;
    int i;
    float f;
  };
};

struct jl_expr_unary_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  jl_op_n op;
  jl_expr_t operand;
};

struct jl_expr_binary_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  jl_op_n op;
  jl_expr_t lhs, rhs;
};

struct jl_expr_ternary_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  jl_expr_t lhs, mhs, rhs;
};

struct jl_expr_array_read_t {
  unsigned refs;
  jl_expr_t next;
  jl_type_t type;
  jl_expr_t lhs, pos;
};

struct jl_expr_array_write_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  jl_expr_t lhs, pos, rhs;
};

struct jl_expr_field_read_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  bool ptr;
  jl_expr_t lhs;
  jl_expr_t field;
};

struct jl_expr_field_write_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  bool ptr;
  jl_expr_t lhs, rhs;
  jl_expr_id_t field;
};

struct jl_expr_call_t {
  unsigned refs;
  jl_type_t type;
  jl_expr_t next;
  jl_expr_t lhs;
  jl_expr_r args;
};


jl_expr_t jl_expr_undefined() {
  return (jl_expr_t){JL_EXPR_UNDEFINED};
}

void jl_expr_dtor(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_UNDEFINED:
      return;
    case JL_EXPR_ID:
      if (self->_id->refs <= 0) {
        jl_expr_id_dtor(self);
        free(self->_id);
        self->_id = NULL;
      }
      break;
    case JL_EXPR_CONST:
      if (self->_const->refs <= 0) {
        jl_expr_const_dtor(self);
        free(self->_const);
        self->_const = NULL;
      }
      break;
    case JL_EXPR_UNARY:
      if (self->_unary->refs <= 0) {
        jl_expr_unary_dtor(self);
        free(self->_unary);
        self->_unary = NULL;
      }
      break;
    case JL_EXPR_BINARY:
      if (self->_binary->refs <= 0) {
        jl_expr_binary_dtor(self);
        free(self->_binary);
        self->_binary = NULL;
      }
      break;
    case JL_EXPR_TERNARY:
      if (self->_ternary->refs <= 0) {
        jl_expr_ternary_dtor(self);
        free(self->_ternary);
        self->_ternary = NULL;
      }
      break;
    case JL_EXPR_ARRAY_READ:
      if (self->_array_read->refs <= 0) {
        jl_expr_array_read_dtor(self);
        free(self->_array_read);
        self->_array_read = NULL;
      }
      break;
    case JL_EXPR_ARRAY_WRITE:
      if (self->_array_write->refs <= 0) {
        jl_expr_array_write_dtor(self);
        free(self->_array_write);
        self->_array_write = NULL;
      }
      break;
    case JL_EXPR_FIELD_READ:
      if (self->_field_read->refs <= 0) {
        jl_expr_field_read_dtor(self);
        free(self->_field_read);
        self->_field_read = NULL;
      }
      break;
    case JL_EXPR_FIELD_WRITE:
      if (self->_field_write->refs <= 0) {
        jl_expr_field_write_dtor(self);
        free(self->_field_write);
        self->_field_write = NULL;
      }
      break;
    case JL_EXPR_CALL:
      if (self->_call->refs <= 0) {
        jl_expr_call_dtor(self);
        free(self->_call);
        self->_call = NULL;
      }
      break;
  }
  *self = jl_expr_undefined();
}

void jl_expr_switch(jl_expr_t *self, jl_expr_n kind) {
  if (self->kind != kind || !jl_expr_is_defined(self)) {
    jl_expr_dtor(self);
    *self = (jl_expr_t) {
      .kind = kind
    };
    switch (kind) {
      case JL_EXPR_UNDEFINED:
        break;
      case JL_EXPR_ID:
        self->_id = xmalloc(sizeof(jl_expr_id_t));
        break;
      case JL_EXPR_CONST:
        self->_id = xmalloc(sizeof(jl_expr_const_t));
        break;
      case JL_EXPR_UNARY:
        self->_id = xmalloc(sizeof(jl_expr_unary_t));
        break;
      case JL_EXPR_BINARY:
        self->_id = xmalloc(sizeof(jl_expr_binary_t));
        break;
      case JL_EXPR_TERNARY:
        self->_ternary = xmalloc(sizeof(jl_expr_ternary_t));
        break;
      case JL_EXPR_ARRAY_READ:
        self->_array_read = xmalloc(sizeof(struct jl_expr_array_read_t));
        break;
      case JL_EXPR_ARRAY_WRITE:
        self->_array_write = xmalloc(sizeof(jl_expr_array_write_t));
        break;
      case JL_EXPR_FIELD_READ:
        self->_field_read = xmalloc(sizeof(jl_expr_field_read_t));
        break;
      case JL_EXPR_FIELD_WRITE:
        self->_field_write = xmalloc(sizeof(jl_expr_field_write_t));
        break;
      case JL_EXPR_CALL:
        self->_call = xmalloc(sizeof(jl_expr_call_t));
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
      ++self->_id->refs;
      break;
    case JL_EXPR_CONST:
      ++self->_const->refs;
      break;
    case JL_EXPR_UNARY:
      ++self->_unary->refs;
      break;
    case JL_EXPR_BINARY:
      ++self->_binary->refs;
      break;
    case JL_EXPR_TERNARY:
      ++self->_ternary->refs;
      break;
    case JL_EXPR_ARRAY_READ:
      ++self->_array_read->refs;
      break;
    case JL_EXPR_ARRAY_WRITE:
      ++self->_array_write->refs;
      break;
    case JL_EXPR_FIELD_READ:
      ++self->_field_read->refs;
      break;
    case JL_EXPR_FIELD_WRITE:
      ++self->_field_write->refs;
      break;
    case JL_EXPR_CALL:
      ++self->_call->refs;
      break;
  }
}

void jl_expr_release(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_UNDEFINED:
      puts("cannot release undefined type");
      exit(1);
    case JL_EXPR_ID:
      --self->_id->refs;
      break;
    case JL_EXPR_CONST:
      --self->_const->refs;
      break;
    case JL_EXPR_UNARY:
      --self->_unary->refs;
      break;
    case JL_EXPR_BINARY:
      --self->_binary->refs;
      break;
    case JL_EXPR_TERNARY:
      --self->_ternary->refs;
      break;
    case JL_EXPR_ARRAY_READ:
      --self->_array_read->refs;
      break;
    case JL_EXPR_ARRAY_WRITE:
      --self->_array_write->refs;
      break;
    case JL_EXPR_FIELD_READ:
      --self->_field_read->refs;
      break;
    case JL_EXPR_FIELD_WRITE:
      --self->_field_write->refs;
      break;
    case JL_EXPR_CALL:
      --self->_call->refs;
      break;
  }
}

bool jl_expr_is_defined(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      return self->_id != NULL;
    case JL_EXPR_CONST:
      return self->_const != NULL;
    case JL_EXPR_UNARY:
      return self->_unary != NULL;
    case JL_EXPR_BINARY:
      return self->_binary != NULL;
    case JL_EXPR_TERNARY:
      return self->_ternary != NULL;
    case JL_EXPR_ARRAY_READ:
      return self->_array_read != NULL;
    case JL_EXPR_ARRAY_WRITE:
      return self->_array_write != NULL;
    case JL_EXPR_FIELD_READ:
      return self->_field_read != NULL;
    case JL_EXPR_FIELD_WRITE:
      return self->_field_write != NULL;
    case JL_EXPR_CALL:
      return self->_call != NULL;
    case JL_EXPR_UNDEFINED:
    default:
      return false;
  }
}

jl_type_t jl_expr_get_type(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      return self->_id->type;
    case JL_EXPR_CONST:
      return self->_const->type;
    case JL_EXPR_UNARY:
      return self->_unary->type;
    case JL_EXPR_BINARY:
      return self->_binary->type;
    case JL_EXPR_TERNARY:
      return self->_ternary->type;
    case JL_EXPR_ARRAY_READ:
      return self->_array_read->type;
    case JL_EXPR_ARRAY_WRITE:
      return self->_array_write->type;
    case JL_EXPR_FIELD_READ:
      return self->_field_read->type;
    case JL_EXPR_FIELD_WRITE:
      return self->_field_write->type;
    case JL_EXPR_CALL:
      return self->_call->type;
    case JL_EXPR_UNDEFINED:
    default:
      puts("cannot access next on undefined expression");
      exit(1);
  }
}

void jl_expr_set_type(jl_expr_t *self, jl_type_t type) {
  switch (self->kind) {
    case JL_EXPR_ID:
      self->_id->type = type;
      break;
    case JL_EXPR_CONST:
      self->_const->type = type;
      break;
    case JL_EXPR_UNARY:
      self->_unary->type = type;
      break;
    case JL_EXPR_BINARY:
      self->_binary->type = type;
      break;
    case JL_EXPR_TERNARY:
      self->_ternary->type = type;
      break;
    case JL_EXPR_ARRAY_READ:
      self->_array_read->type = type;
      break;
    case JL_EXPR_ARRAY_WRITE:
      self->_array_write->type = type;
      break;
    case JL_EXPR_FIELD_READ:
      self->_field_read->type = type;
      break;
    case JL_EXPR_FIELD_WRITE:
      self->_field_write->type = type;
      break;
    case JL_EXPR_CALL:
      self->_call->type = type;
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
      return self->_id->next;
    case JL_EXPR_CONST:
      return self->_const->next;
    case JL_EXPR_UNARY:
      return self->_unary->next;
    case JL_EXPR_BINARY:
      return self->_binary->next;
    case JL_EXPR_TERNARY:
      return self->_ternary->next;
    case JL_EXPR_ARRAY_READ:
      return self->_array_read->next;
    case JL_EXPR_ARRAY_WRITE:
      return self->_array_write->next;
    case JL_EXPR_FIELD_READ:
      return self->_field_read->next;
    case JL_EXPR_FIELD_WRITE:
      return self->_field_write->next;
    case JL_EXPR_CALL:
      return self->_call->next;
    case JL_EXPR_UNDEFINED:
    default:
      puts("cannot access next on undefined expression");
      exit(1);
  }
}

void jl_expr_set_next(jl_expr_t *self, jl_expr_t next) {
  switch (self->kind) {
    case JL_EXPR_ID:
      self->_id->next = next;
      break;
    case JL_EXPR_CONST:
      self->_const->next = next;
      break;
    case JL_EXPR_UNARY:
      self->_unary->next = next;
      break;
    case JL_EXPR_BINARY:
      self->_binary->next = next;
      break;
    case JL_EXPR_TERNARY:
      self->_ternary->next = next;
      break;
    case JL_EXPR_ARRAY_READ:
      self->_array_read->next = next;
      break;
    case JL_EXPR_ARRAY_WRITE:
      self->_array_write->next = next;
      break;
    case JL_EXPR_FIELD_READ:
      self->_field_read->next = next;
      break;
    case JL_EXPR_FIELD_WRITE:
      self->_field_write->next = next;
      break;
    case JL_EXPR_CALL:
      self->_call->next = next;
      break;
    case JL_EXPR_UNDEFINED:
    default:
      puts("cannot set next on undefined expression");
      exit(1);
  }
}


jl_expr_t jl_id(const char *id, bool is_keyword) {
  jl_expr_t expr;

  jl_id_init(&expr, id, is_keyword);
  return expr;
}

void jl_id_init(jl_expr_t *self, const char *id, bool is_keyword) {
  jl_expr_switch(self, JL_EXPR_ID);
  jl_id_set_id(self, id);
  jl_id_set_is_keyword(self, is_keyword);
}

void jl_expr_id_dtor(jl_expr_t *self) {}

const char *jl_id_get_id(jl_expr_t *self) {
  return jl_expr_id(self)->id;
}

void jl_id_set_id(jl_expr_t *self, const char *id) {
  jl_expr_id(self)->id = id;
}

bool jl_id_is_keyword(jl_expr_t *self) {
  return jl_expr_id(self)->is_keyword;
}

void jl_id_set_is_keyword(jl_expr_t *self, bool is_keyword) {
  jl_expr_id(self)->is_keyword = is_keyword;
}


jl_expr_t jl_const_int(int d) {
  jl_expr_t expr;

  jl_const_init(&expr, jl_int());
  jl_const_set_int(&expr, d);
  return expr;
}

jl_expr_t jl_const_float(float f) {
  jl_expr_t expr;

  jl_const_init(&expr, jl_float());
  jl_const_set_float(&expr, f);
  return expr;
}

jl_expr_t jl_const_string(const char *s) {
  jl_expr_t expr;

  jl_const_init(&expr, jl_string());
  jl_const_set_string(&expr, s);
  return expr;
}

void jl_const_init(jl_expr_t *self, jl_type_t type) {
  jl_expr_switch(self, JL_EXPR_CONST);
  jl_expr_set_type(self, type);
}


void jl_expr_const_dtor(jl_expr_t *self) {}

int jl_const_get_int(jl_expr_t *self) {
  return jl_expr_const(self)->i;
}

void jl_const_set_int(jl_expr_t *self, int d) {
  jl_expr_const(self)->i = d;
}

float jl_const_get_float(jl_expr_t *self) {
  return jl_expr_const(self)->f;
}

void jl_const_set_float(jl_expr_t *self, float f) {
  jl_expr_const(self)->f = f;
}

const char *jl_const_get_string(jl_expr_t *self) {
  return jl_expr_const(self)->s;
}

void jl_const_set_string(jl_expr_t *self, const char *s) {
  jl_expr_const(self)->s = s;
}


jl_expr_t jl_unary(jl_op_n op, jl_expr_t operand) {
  jl_expr_t expr;

  jl_unary_init(&expr, op, operand);
  return expr;
}

void jl_unary_init(jl_expr_t *self, jl_op_n op, jl_expr_t operand) {
  jl_expr_switch(self, JL_EXPR_UNARY);
  jl_unary_set_op(self, op);
  jl_unary_set_operand(self, operand);
}

void jl_expr_unary_dtor(jl_expr_t *self) {}

jl_op_n jl_unary_get_op(jl_expr_t *self) {
  return jl_expr_unary(self)->op;
}

void jl_unary_set_op(jl_expr_t *self, jl_op_n op) {
  jl_expr_unary(self)->op = op;
}

jl_expr_t jl_unary_get_operand(jl_expr_t *self) {
  return jl_expr_unary(self)->operand;
}

void jl_unary_set_operand(jl_expr_t *self, jl_expr_t operand) {
  jl_expr_unary(self)->operand = operand;
}


jl_expr_t jl_binary(jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
void jl_binary_init(jl_expr_t *self, jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
void jl_expr_binary_dtor(jl_expr_t *self) {}
jl_op_n jl_binary_get_op(jl_expr_t *self);
void jl_binary_set_op(jl_expr_t *self, jl_op_n op);
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
