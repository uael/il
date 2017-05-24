/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>

#include "expr.h"

#include "entity.h"
#include "stmt.h"
#include "type.h"

wulk_expr_t wulk_exprs(wulk_expr_t *exprs) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_EXPRS
  };

  if (exprs) {
    while (exprs->kind) {
      adt_vector_push(expr.exprs.vector, *exprs);
      ++exprs;
    }
  }
  return expr;
}

wulk_expr_t wulk_exprs_start(wulk_expr_t expr) {
  wulk_expr_t exprs = (wulk_expr_t) {
    .kind = WULK_EXPR_EXPRS
  };

  adt_vector_push(exprs.exprs.vector, expr);
  return exprs;
}

static void wulk_exprs_dtor(wulk_exprs_t *self) {
  wulk_expr_t expr;

  adt_vector_foreach(self->vector, expr) {
    wulk_expr_dtor(&expr);
  }
  adt_vector_dtor(self->vector);
}

wulk_expr_t wulk_id(wulk_lloc_t lloc, const char *id, wulk_type_t type) {
  return (wulk_expr_t) {
    .kind = WULK_EXPR_ID,
    .id = {
      .id = id,
      .lloc = lloc,
      .type = type
    }
  };
}

static void wulk_id_dtor(wulk_id_t *self) {
  wulk_type_dtor(&self->type);
}

wulk_expr_t wulk_const_int(wulk_lloc_t lloc, int d) {
  return wulk_const(lloc, wulk_val(wulk_int(), .ul = (unsigned long) d));
}

wulk_expr_t wulk_const_float(wulk_lloc_t lloc, float f) {
  return wulk_const(lloc, wulk_val(wulk_float(), .f = f));
}

wulk_expr_t wulk_const_string(wulk_lloc_t lloc, const char *s) {
  return wulk_const(lloc, wulk_val(wulk_pointer(wulk_char()), .s = s));
}

wulk_expr_t wulk_const(wulk_lloc_t lloc, wulk_val_t value) {
  assert(wulk_defined(value.type));
  return (wulk_expr_t) {
    .kind = WULK_EXPR_CONST,
    .constant = {
      .value = value,
      .lloc = lloc
    }
  };
}

static void wulk_const_dtor(wulk_const_t *self) {
  wulk_type_dtor(&self->type);
}

wulk_expr_t wulk_unary(wulk_lloc_t op_lloc, enum wulk_op_n op, wulk_expr_t operand) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_UNARY,
    .unary = {
      .op = op,
      .operand = xmalloc(sizeof(wulk_expr_t)),
      .lloc = wulk_lloc(op_lloc, operand.lloc),
      .type = operand.type
    }
  };

  *expr.unary.operand = operand;
  return expr;
}

wulk_expr_t wulk_cast(wulk_lloc_t type_lloc, wulk_type_t type, wulk_expr_t operand) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_UNARY,
    .unary = {
      .op = WULK_OP_CAST,
      .operand = xmalloc(sizeof(wulk_expr_t)),
      .lloc = wulk_lloc(type_lloc, operand.lloc),
      .type = type
    }
  };

  *expr.unary.operand = operand;
  return expr;
}

static void wulk_unary_dtor(wulk_unary_t *self) {
  if (self->op == WULK_OP_CAST) {
    wulk_type_dtor(&self->type);
  }
  wulk_expr_dtor(self->operand);
  xfree(self->operand);
  self->operand = NULL;
}

wulk_expr_t wulk_binary(enum wulk_op_n op, wulk_expr_t lhs, wulk_expr_t rhs) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_BINARY,
    .binary = {
      .op = op,
      .lhs = xmalloc(sizeof(wulk_expr_t)),
      .rhs = xmalloc(sizeof(wulk_expr_t)),
      .lloc = wulk_lloc(lhs.lloc, rhs.lloc),
      .type = lhs.type
    }
  };

  *expr.binary.lhs = lhs;
  *expr.binary.rhs = rhs;
  return expr;
}

static void wulk_binary_dtor(wulk_binary_t *self) {
  wulk_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  wulk_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

wulk_expr_t wulk_ternary(wulk_expr_t lhs, wulk_expr_t mhs, wulk_expr_t rhs) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_TERNARY,
    .ternary = {
      .lhs = xmalloc(sizeof(wulk_expr_t)),
      .mhs = xmalloc(sizeof(wulk_expr_t)),
      .rhs = xmalloc(sizeof(wulk_expr_t)),
      .lloc = wulk_lloc(lhs.lloc, rhs.lloc),
      .type = mhs.type
    }
  };

  *expr.ternary.lhs = lhs;
  *expr.ternary.mhs = mhs;
  *expr.ternary.rhs = rhs;
  return expr;
}

static void wulk_ternary_dtor(wulk_ternary_t *self) {
  wulk_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  wulk_expr_dtor(self->mhs);
  xfree(self->mhs);
  self->mhs = NULL;
  wulk_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

wulk_expr_t wulk_array_read(wulk_expr_t lhs, wulk_lloc_t pos_lloc, wulk_expr_t pos) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_ARRAY_READ,
    .array_read = {
      .lhs = xmalloc(sizeof(wulk_expr_t)),
      .pos = xmalloc(sizeof(wulk_expr_t)),
      .lloc = wulk_lloc(lhs.lloc, pos_lloc),
      .type = wulk_type_deref(lhs.type)
    }
  };

  *expr.array_read.lhs = lhs;
  *expr.array_read.pos = pos;
  return expr;
}

static void wulk_array_read_dtor(wulk_array_read_t *self) {
  wulk_type_dtor(&self->type);
  wulk_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  wulk_expr_dtor(self->pos);
  xfree(self->pos);
  self->pos = NULL;
}

wulk_expr_t wulk_array_write(wulk_array_read_t array_read, wulk_expr_t rhs) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_ARRAY_WRITE,
    .array_write = {
      .read = array_read,
      .rhs = xmalloc(sizeof(wulk_expr_t)),
    }
  };

  expr.lloc = wulk_lloc(array_read.lloc, rhs.lloc);
  *expr.array_write.rhs = rhs;
  return expr;
}

static void wulk_array_write_dtor(wulk_array_write_t *self) {
  wulk_array_read_dtor(&self->read);
  wulk_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

wulk_expr_t wulk_field_read(wulk_expr_t lhs, wulk_id_t id) {
  wulk_field_t *field = wulk_field_lookup(lhs.type, id.id);
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_FIELD_READ,
    .field_read = {
      .lhs = xmalloc(sizeof(wulk_expr_t)),
      .id = id,
      .lloc = wulk_lloc(lhs.lloc, id.lloc),
      .type = field ? field->type : wulk_type_undefined()
    }
  };

  *expr.field_read.lhs = lhs;
  return expr;
}

static void wulk_field_read_dtor(wulk_field_read_t *self) {
  wulk_id_dtor(&self->id);
  wulk_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
}

wulk_expr_t wulk_field_write(wulk_field_read_t field_read, wulk_expr_t rhs) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_FIELD_WRITE,
    .field_write = {
      .read = field_read,
      .rhs = xmalloc(sizeof(wulk_expr_t))
    }
  };

  expr.lloc = wulk_lloc(field_read.lloc, rhs.lloc);
  *expr.field_write.rhs = rhs;
  return expr;
}

static void wulk_field_write_dtor(wulk_field_write_t *self) {
  wulk_field_read_dtor(&self->read);
  wulk_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

wulk_expr_t wulk_call(wulk_expr_t lhs, wulk_lloc_t args_lloc, wulk_exprs_t args) {
  wulk_expr_t expr = (wulk_expr_t) {
    .kind = WULK_EXPR_CALL,
    .call = {
      .lhs = xmalloc(sizeof(wulk_expr_t)),
      .args = args,
      .lloc = wulk_lloc(lhs.lloc, args_lloc)
    }
  };

  *expr.call.lhs = lhs;
  return expr;
}

static void wulk_call_dtor(wulk_call_t *self) {
  wulk_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  wulk_exprs_dtor(&self->args);
}

void wulk_expr_dtor(wulk_expr_t *self) {
  switch (self->kind) {
    case WULK_EXPR_UNDEFINED:
      break;
    case WULK_EXPR_EXPRS:
      wulk_exprs_dtor(&self->exprs);
      break;
    case WULK_EXPR_ID:
      wulk_id_dtor(&self->id);
      break;
    case WULK_EXPR_CONST:
      wulk_const_dtor(&self->constant);
      break;
    case WULK_EXPR_UNARY:
      wulk_unary_dtor(&self->unary);
      break;
    case WULK_EXPR_BINARY:
      wulk_binary_dtor(&self->binary);
      break;
    case WULK_EXPR_TERNARY:
      wulk_ternary_dtor(&self->ternary);
      break;
    case WULK_EXPR_ARRAY_READ:
      wulk_array_read_dtor(&self->array_read);
      break;
    case WULK_EXPR_ARRAY_WRITE:
      wulk_array_write_dtor(&self->array_write);
      break;
    case WULK_EXPR_FIELD_READ:
      wulk_field_read_dtor(&self->field_read);
      break;
    case WULK_EXPR_FIELD_WRITE:
      wulk_field_write_dtor(&self->field_write);
      break;
    case WULK_EXPR_CALL:
      wulk_call_dtor(&self->call);
      break;
    default:
      break;
  }
  *self = wulk_expr_undefined();
}

bool wulk_expr_is_constant(wulk_expr_t self) {
  switch (self.kind) {
    case WULK_EXPR_CONST:
      return true;
    case WULK_EXPR_UNARY:
      return wulk_expr_is_constant(*self.unary.operand);
    case WULK_EXPR_BINARY:
      return wulk_expr_is_constant(*self.binary.lhs) && wulk_expr_is_constant(*self.binary.rhs);
    case WULK_EXPR_TERNARY:
      return wulk_expr_is_constant(*self.ternary.lhs) && wulk_expr_is_constant(*self.ternary.mhs)
             && wulk_expr_is_constant(*self.ternary.rhs);
    default:
      return false;
  }
}

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

static wulk_type_t constant_integer_type(unsigned long int value, enum suffix suffix, int is_decimal) {
  switch (suffix) {
    case SUFFIX_U:
      if (value <= UINT_MAX) {
        return wulk_uint();
      } else {
        return wulk_ulong();
      }
    case SUFFIX_L:
    case SUFFIX_LL:
      if (value <= LONG_MAX) {
        return wulk_long();
      } else {
        if (is_decimal) {
          puts("Conversion of decimal constant to unsigned.");
        }
        return wulk_ulong();
      }
    case SUFFIX_UL:
    case SUFFIX_ULL:
      return wulk_ulong();
    case SUFFIX_NONE:
    default:
      if (value <= INT_MAX) {
        return wulk_int();
      } else if (!is_decimal && value <= UINT_MAX) {
        return wulk_uint();
      } else if (value <= LONG_MAX) {
        return wulk_long();
      } else {
        if (is_decimal) {
          puts("Conversion of decimal constant to unsigned.");
        }
        return wulk_ulong();
      }
  }
}

int wulk_const_parse(wulk_lloc_t lloc, const char *s, size_t len, wulk_expr_t *out) {
  const char *str;
  char *endptr;
  enum suffix suffix;

  str = s;
  /*
   * Try to read as integer. Handle suffixes u, l, ll, ul, ull, in all
   * permutations of upper- and lower case.
   */
  errno = 0;
  *out = (wulk_expr_t) {.kind = WULK_EXPR_CONST, .lloc = lloc};
  out->constant.ul = strtoul(str, &endptr, 0);
  suffix = read_integer_suffix(endptr, &endptr);
  if ((size_t) (endptr - str) == len) {
    assert(isdigit(*str));
    out->type = constant_integer_type(out->constant.ul, suffix, *str != '0');
  } else {
    /*
     * If the integer conversion did not consume the whole token,
     * try to read as floating point number.
     *
     * Note: not using strtold for long double conversion, so might
     * get incorrect results compared to other compilers.
     */
    errno = 0;
    out->constant.d = strtod(str, &endptr);
    if ((size_t) (endptr - str) < len) {
      if (*endptr == 'f' || *endptr == 'F') {
        out->constant.f = (float) out->constant.d;
        out->type = wulk_float();
        endptr++;
      } else if (*endptr == 'l' || *endptr == 'L') {
        out->constant.ld = out->constant.d;
        out->type = wulk_long_double();
        endptr++;
      } else {
        out->type = wulk_double();
      }
    } else {
      out->type = wulk_double();
    }
  }
  if (errno || ((size_t) (endptr - str) != len)) {
    if (errno == ERANGE) {
      return ERANGE;
    } else {
      return 1;
    }
  }
  return 0;
}

long wulk_eval_long(wulk_expr_t expr) {
  return 0;
}

unsigned long wulk_eval_ulong(wulk_expr_t expr) {
  return 0;
}
