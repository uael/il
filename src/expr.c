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
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <expr_t.h>

#include "expr.h"

#include "entity.h"
#include "stmt.h"
#include "type.h"

void jl_expr_id_dtor(jl_expr_id_t *self);
void jl_expr_const_dtor(jl_expr_const_t *self);
void jl_expr_unary_dtor(jl_expr_unary_t *self);
void jl_expr_binary_dtor(jl_expr_binary_t *self);
void jl_expr_ternary_dtor(jl_expr_ternary_t *self);
void jl_expr_array_read_dtor(jl_expr_array_read_t *self);
void jl_expr_array_write_dtor(jl_expr_array_write_t *self);
void jl_expr_field_read_dtor(jl_expr_field_read_t *self);
void jl_expr_field_write_dtor(jl_expr_field_write_t *self);
void jl_expr_call_dtor(jl_expr_call_t *self);
void jl_expr_list_dtor(jl_expr_list_t *self);

void jl_expr_dtor(jl_expr_t *self) {
  jl_type_dtor(&self->type);
  switch (self->kind) {
    case JL_EXPR_UNDEFINED:
      return;
    case JL_EXPR_ID:
      if (jl_pu(self, id) && jl_pu(self, id)->refs <= 0) {
        jl_expr_id_dtor(jl_pu(self, id));
        xfree(jl_pu(self, id));
        jl_pu(self, id) = NULL;
      }
      break;
    case JL_EXPR_CONST:
      if (jl_pu(self, const) && jl_pu(self, const)->refs <= 0) {
        jl_expr_const_dtor(jl_pu(self, const));
        xfree(jl_pu(self, const));
        jl_pu(self, const) = NULL;
      }
      break;
    case JL_EXPR_UNARY:
      if (jl_pu(self, unary) && jl_pu(self, unary)->refs <= 0) {
        jl_expr_unary_dtor(jl_pu(self, unary));
        xfree(jl_pu(self, unary));
        jl_pu(self, unary) = NULL;
      }
      break;
    case JL_EXPR_BINARY:
      if (jl_pu(self, binary) && jl_pu(self, binary)->refs <= 0) {
        jl_expr_binary_dtor(jl_pu(self, binary));
        xfree(jl_pu(self, binary));
        jl_pu(self, binary) = NULL;
      }
      break;
    case JL_EXPR_TERNARY:
      if (jl_pu(self, ternary) && jl_pu(self, ternary)->refs <= 0) {
        jl_expr_ternary_dtor(jl_pu(self, ternary));
        xfree(jl_pu(self, ternary));
        jl_pu(self, ternary) = NULL;
      }
      break;
    case JL_EXPR_ARRAY_READ:
      if (jl_pu(self, array_read) && jl_pu(self, array_read)->refs <= 0) {
        jl_expr_array_read_dtor(jl_pu(self, array_read));
        xfree(jl_pu(self, array_read));
        jl_pu(self, array_read) = NULL;
      }
      break;
    case JL_EXPR_ARRAY_WRITE:
      if (jl_pu(self, array_write) && jl_pu(self, array_write)->refs <= 0) {
        jl_expr_array_write_dtor(jl_pu(self, array_write));
        xfree(jl_pu(self, array_write));
        jl_pu(self, array_write) = NULL;
      }
      break;
    case JL_EXPR_FIELD_READ:
      if (jl_pu(self, field_read) && jl_pu(self, field_read)->refs <= 0) {
        jl_expr_field_read_dtor(jl_pu(self, field_read));
        xfree(jl_pu(self, field_read));
        jl_pu(self, field_read) = NULL;
      }
      break;
    case JL_EXPR_FIELD_WRITE:
      if (jl_pu(self, field_write) && jl_pu(self, field_write)->refs <= 0) {
        jl_expr_field_write_dtor(jl_pu(self, field_write));
        xfree(jl_pu(self, field_write));
        jl_pu(self, field_write) = NULL;
      }
      break;
    case JL_EXPR_CALL:
      if (jl_pu(self, call) && jl_pu(self, call)->refs <= 0) {
        jl_expr_call_dtor(jl_pu(self, call));
        xfree(jl_pu(self, call));
        jl_pu(self, call) = NULL;
      }
      break;
    case JL_EXPR_LIST:
      if (jl_pu(self, list) && jl_pu(self, list)->refs <= 0) {
        jl_expr_list_dtor(jl_pu(self, list));
        xfree(jl_pu(self, list));
        jl_pu(self, list) = NULL;
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
        jl_pu(self, id) = xmalloc(sizeof(jl_expr_id_t));
        *jl_pu(self, id) = (jl_expr_id_t) {.refs = 0};
        break;
      case JL_EXPR_CONST:
        jl_pu(self, const) = xmalloc(sizeof(jl_expr_const_t));
        *jl_pu(self, const) = (jl_expr_const_t) {.refs = 0};
        break;
      case JL_EXPR_UNARY:
        jl_pu(self, unary) = xmalloc(sizeof(jl_expr_unary_t));
        *jl_pu(self, unary) = (jl_expr_unary_t) {.refs = 0};
        break;
      case JL_EXPR_BINARY:
        jl_pu(self, binary) = xmalloc(sizeof(jl_expr_binary_t));
        *jl_pu(self, binary) = (jl_expr_binary_t) {.refs = 0};
        break;
      case JL_EXPR_TERNARY:
        jl_pu(self, ternary) = xmalloc(sizeof(jl_expr_ternary_t));
        *jl_pu(self, ternary) = (jl_expr_ternary_t) {.refs = 0};
        break;
      case JL_EXPR_ARRAY_READ:
        jl_pu(self, array_read) = xmalloc(sizeof(struct jl_expr_array_read_t));
        *jl_pu(self, array_read) = (jl_expr_array_read_t) {.refs = 0};
        break;
      case JL_EXPR_ARRAY_WRITE:
        jl_pu(self, array_write) = xmalloc(sizeof(jl_expr_array_write_t));
        *jl_pu(self, array_write) = (jl_expr_array_write_t) {.refs = 0};
        break;
      case JL_EXPR_FIELD_READ:
        jl_pu(self, field_read) = xmalloc(sizeof(jl_expr_field_read_t));
        *jl_pu(self, field_read) = (jl_expr_field_read_t) {.refs = 0};
        break;
      case JL_EXPR_FIELD_WRITE:
        jl_pu(self, field_write) = xmalloc(sizeof(jl_expr_field_write_t));
        *jl_pu(self, field_write) = (jl_expr_field_write_t) {.refs = 0};
        break;
      case JL_EXPR_CALL:
        jl_pu(self, call) = xmalloc(sizeof(jl_expr_call_t));
        *jl_pu(self, call) = (jl_expr_call_t) {.refs = 0};
        break;
      case JL_EXPR_LIST:
        jl_pu(self, list) = xmalloc(sizeof(jl_expr_list_t));
        *jl_pu(self, list) = (jl_expr_list_t) {.refs = 0};
        break;
    }
  }
}

void jl_expr_acquire(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      ++jl_pu(self, id)->refs;
      break;
    case JL_EXPR_CONST:
      ++jl_pu(self, const)->refs;
      break;
    case JL_EXPR_UNARY:
      ++jl_pu(self, unary)->refs;
      break;
    case JL_EXPR_BINARY:
      ++jl_pu(self, binary)->refs;
      break;
    case JL_EXPR_TERNARY:
      ++jl_pu(self, ternary)->refs;
      break;
    case JL_EXPR_ARRAY_READ:
      ++jl_pu(self, array_read)->refs;
      break;
    case JL_EXPR_ARRAY_WRITE:
      ++jl_pu(self, array_write)->refs;
      break;
    case JL_EXPR_FIELD_READ:
      ++jl_pu(self, field_read)->refs;
      break;
    case JL_EXPR_FIELD_WRITE:
      ++jl_pu(self, field_write)->refs;
      break;
    case JL_EXPR_CALL:
      ++jl_pu(self, call)->refs;
      break;
    case JL_EXPR_LIST:
      ++jl_pu(self, list)->refs;
      break;
    default:
      break;
  }
}

void jl_expr_release(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      --jl_pu(self, id)->refs;
      break;
    case JL_EXPR_CONST:
      --jl_pu(self, const)->refs;
      break;
    case JL_EXPR_UNARY:
      --jl_pu(self, unary)->refs;
      break;
    case JL_EXPR_BINARY:
      --jl_pu(self, binary)->refs;
      break;
    case JL_EXPR_TERNARY:
      --jl_pu(self, ternary)->refs;
      break;
    case JL_EXPR_ARRAY_READ:
      --jl_pu(self, array_read)->refs;
      break;
    case JL_EXPR_ARRAY_WRITE:
      --jl_pu(self, array_write)->refs;
      break;
    case JL_EXPR_FIELD_READ:
      --jl_pu(self, field_read)->refs;
      break;
    case JL_EXPR_FIELD_WRITE:
      --jl_pu(self, field_write)->refs;
      break;
    case JL_EXPR_CALL:
      --jl_pu(self, call)->refs;
      break;
    case JL_EXPR_LIST:
      --jl_pu(self, list)->refs;
      break;
    default:
      break;
  }
}

bool jl_expr_is_defined(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_ID:
      return jl_pu(self, id) != NULL;
    case JL_EXPR_CONST:
      return jl_pu(self, const) != NULL;
    case JL_EXPR_UNARY:
      return jl_pu(self, unary) != NULL;
    case JL_EXPR_BINARY:
      return jl_pu(self, binary) != NULL;
    case JL_EXPR_TERNARY:
      return jl_pu(self, ternary) != NULL;
    case JL_EXPR_ARRAY_READ:
      return jl_pu(self, array_read) != NULL;
    case JL_EXPR_ARRAY_WRITE:
      return jl_pu(self, array_write) != NULL;
    case JL_EXPR_FIELD_READ:
      return jl_pu(self, field_read) != NULL;
    case JL_EXPR_FIELD_WRITE:
      return jl_pu(self, field_write) != NULL;
    case JL_EXPR_CALL:
      return jl_pu(self, call) != NULL;
    case JL_EXPR_LIST:
      return jl_pu(self, list) != NULL;
    case JL_EXPR_UNDEFINED:
    default:
      return false;
  }
}

bool jl_expr_is_constant(jl_expr_t self) {
  switch (self.kind) {
    case JL_EXPR_CONST:
      return true;
    case JL_EXPR_UNARY:
      return jl_expr_is_constant(jl_expr_unary(self)->operand);
    case JL_EXPR_BINARY:
      return jl_expr_is_constant(jl_expr_binary(self)->lhs) && jl_expr_is_constant(jl_expr_binary(self)->rhs);
    case JL_EXPR_TERNARY:
      return jl_expr_is_constant(jl_expr_ternary(self)->lhs) && jl_expr_is_constant(jl_expr_ternary(self)->mhs)
        && jl_expr_is_constant(jl_expr_ternary(self)->rhs);
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


jl_expr_t jl_id(const char *id, jl_type_t type) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_id_init(&expr, id, type);
  return expr;
}

void jl_id_init(jl_expr_t *self, const char *id, jl_type_t type) {
  jl_expr_switch(self, JL_EXPR_ID);
  jl_pexpr_id(self)->id = id;
  jl_expr_set_type(self, type);
}

void jl_expr_id_dtor(jl_expr_id_t *self) {}


jl_expr_t jl_const_int(int d) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_const_init(&expr, jl_int());
  jl_pu(jl_u(expr, const), ul) = (unsigned long) d;
  return expr;
}

jl_expr_t jl_const_float(float f) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_const_init(&expr, jl_float());
  jl_pu(jl_u(expr, const), f) = f;
  return expr;
}

jl_expr_t jl_const_string(const char *s) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_const_init(&expr, jl_pointer(jl_char()));
  jl_pu(jl_u(expr, const), s) = s;
  return expr;
}

void jl_const_init(jl_expr_t *self, jl_type_t type) {
  jl_expr_switch(self, JL_EXPR_CONST);
  jl_expr_set_type(self, type);
}

void jl_expr_const_dtor(jl_expr_const_t *self) {}

enum suffix {
  SUFFIX_NONE = 0,
  SUFFIX_U = 0x1,
  SUFFIX_L = 0x2,
  SUFFIX_UL = SUFFIX_U | SUFFIX_L,
  SUFFIX_LL = (SUFFIX_L << 1) | SUFFIX_L,
  SUFFIX_ULL = SUFFIX_U | SUFFIX_LL
};

static enum suffix read_integer_suffix(char *ptr, char **endptr) {
  enum suffix s = SUFFIX_NONE;

  if (tolower(*ptr) == 'u') {
    s = SUFFIX_U;
    ptr++;
  }
  if (tolower(*ptr) == 'l') {
    s |= SUFFIX_L;
    ptr++;
    if (*ptr == ptr[-1]) {
      s |= SUFFIX_LL;
      ptr++;
    }

    if (!(s & SUFFIX_U) && tolower(*ptr) == 'u') {
      s |= SUFFIX_U;
      ptr++;
    }
  }
  *endptr = ptr;
  return s;
}

static jl_type_t constant_integer_type(unsigned long int value, enum suffix suffix, int is_decimal) {
  switch (suffix) {
    case SUFFIX_U:
      if (value <= UINT_MAX) {
        return jl_uint();
      } else {
        return jl_ulong();
      }
    case SUFFIX_L:
    case SUFFIX_LL:
      if (value <= LONG_MAX) {
        return jl_long();
      } else {
        if (is_decimal) {
          puts("Conversion of decimal constant to unsigned.");
        }
        return jl_ulong();
      }
    case SUFFIX_UL:
    case SUFFIX_ULL:
      return jl_ulong();
    case SUFFIX_NONE:
    default:
      if (value <= INT_MAX) {
        return jl_int();
      } else if (!is_decimal && value <= UINT_MAX) {
        return jl_uint();
      } else if (value <= LONG_MAX) {
        return jl_long();
      } else {
        if (is_decimal) {
          puts("Conversion of decimal constant to unsigned.");
        }
        return jl_ulong();
      }
  }
}

int jl_const_parse(const char *s, size_t len, jl_expr_t *out) {
  const char *str;
  char *endptr;
  enum suffix suffix;
  jl_expr_const_t *_const;
  jl_expr_t expr = jl_expr_undefined();

  str = s;
  jl_expr_switch(&expr, JL_EXPR_CONST);
  _const = jl_u(expr, const);
  /*
   * Try to read as integer. Handle suffixes u, l, ll, ul, ull, in all
   * permutations of upper- and lower case.
   */
  errno = 0;
  jl_pu(_const, ul) = strtoul(str, &endptr, 0);
  suffix = read_integer_suffix(endptr, &endptr);
  if ((size_t) (endptr - str) == len) {
    assert(isdigit(*str));
    expr.type = constant_integer_type(jl_pu(_const, ul), suffix, *str != '0');
  } else {
    /*
     * If the integer conversion did not consume the whole token,
     * try to read as floating point number.
     *
     * Note: not using strtold for long double conversion, so might
     * get incorrect results compared to other compilers.
     */
    errno = 0;
    expr.type = jl_double();
    _const->u._d = strtod(str, &endptr);
    if ((size_t) (endptr - str) < len) {
      if (*endptr == 'f' || *endptr == 'F') {
        expr.type = jl_float();
        jl_pu(_const, f) = (float) jl_pu(_const, d);
        endptr++;
      } else if (*endptr == 'l' || *endptr == 'L') {
        expr.type = jl_long_double();
        jl_pu(_const, ld) = (long double) jl_pu(_const, d);
        endptr++;
      }
    }
  }
  if (errno || ((size_t) (endptr - str) != len)) {
    if (errno == ERANGE) {
      return ERANGE;
    } else {
      return 1;
    }
  }
  *out = expr;
  return 0;
}


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

void jl_expr_unary_dtor(jl_expr_unary_t *self) {
  jl_expr_dtor(&self->operand);
}


jl_expr_t jl_binary(enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_binary_init(&expr, op, lhs, rhs);
  return expr;
}

void jl_binary_init(jl_expr_t *self, enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs) {
  jl_expr_switch(self, JL_EXPR_BINARY);
  jl_pexpr_binary(self)->op = op;
  jl_pexpr_binary(self)->lhs = lhs;
  jl_pexpr_binary(self)->rhs = rhs;
}

void jl_expr_binary_dtor(jl_expr_binary_t *self) {
  jl_expr_dtor(&self->lhs);
  jl_expr_dtor(&self->rhs);
}


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

void jl_expr_ternary_dtor(jl_expr_ternary_t *self) {
  jl_expr_dtor(&self->lhs);
  jl_expr_dtor(&self->mhs);
  jl_expr_dtor(&self->rhs);
}


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

void jl_expr_array_read_dtor(jl_expr_array_read_t *self) {
  jl_expr_dtor(&self->lhs);
  jl_expr_dtor(&self->pos);
}


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

void jl_expr_array_write_dtor(jl_expr_array_write_t *self) {
  jl_expr_dtor(&self->lhs);
  jl_expr_dtor(&self->pos);
  jl_expr_dtor(&self->rhs);
}


jl_expr_t jl_field_read(jl_expr_t lhs, jl_expr_t field, short width, short offset) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_field_read_init(&expr, lhs, field, width, offset);
  return expr;
}

void jl_field_read_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t field, short width, short offset) {
  jl_expr_switch(self, JL_EXPR_FIELD_READ);
  jl_pexpr_field_read(self)->lhs = lhs;
  jl_pexpr_field_read(self)->field = field;
  jl_pexpr_field_read(self)->width = width;
  jl_pexpr_field_read(self)->offset = offset;
}

void jl_expr_field_read_dtor(jl_expr_field_read_t *self) {
  jl_expr_dtor(&self->lhs);
  jl_expr_dtor(&self->field);
}


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

void jl_expr_field_write_dtor(jl_expr_field_write_t *self) {
  jl_expr_dtor(&self->lhs);
  jl_expr_dtor(&self->field);
  jl_expr_dtor(&self->rhs);
}


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

void jl_expr_call_dtor(jl_expr_call_t *self) {
  jl_expr_t arg;

  adt_vector_foreach(self->args, arg) {
    jl_expr_dtor(&arg);
  }
  adt_vector_dtor(self->args);
  jl_expr_dtor(&self->lhs);
}

jl_expr_t jl_exprs(jl_expr_r exprs) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_exprs_init(&expr, exprs);
  return expr;
}

jl_expr_t jl_exprs_start(jl_expr_t first) {
  jl_expr_t expr = {JL_EXPR_UNDEFINED};

  jl_exprs_init(&expr, (jl_expr_r) {0});
  adt_vector_push(jl_u(expr, list)->exprs, first);
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

void jl_expr_list_dtor(jl_expr_list_t *self) {
  jl_expr_t expr;

  adt_vector_foreach(self->exprs, expr) {
    jl_expr_dtor(&expr);
  }
  adt_vector_dtor(self->exprs);
}
