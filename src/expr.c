/*
 * libil - Intermediate Language cross-platform c library
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
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

il_expr_t il_exprs(il_expr_t *exprs) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_EXPRS
  };

  if (exprs) {
    while (exprs->kind) {
      adt_vector_push(expr.exprs.vector, *exprs);
      ++exprs;
    }
  }
  return expr;
}

il_expr_t il_exprs_start(il_expr_t expr) {
  il_expr_t exprs = (il_expr_t) {
    .kind = IL_EXPR_EXPRS
  };

  adt_vector_push(exprs.exprs.vector, expr);
  return exprs;
}

static void il_exprs_dtor(il_exprs_t *self) {
  il_expr_t expr;

  adt_vector_foreach(self->vector, expr) {
    il_expr_dtor(&expr);
  }
  adt_vector_dtor(self->vector);
}

il_expr_t il_id(il_lloc_t lloc, const char *id, il_type_t type) {
  return (il_expr_t) {
    .kind = IL_EXPR_ID,
    .id = {
      .id = id,
      .lloc = lloc,
      .type = type
    }
  };
}

static void il_id_dtor(il_id_t *self) {
  il_type_dtor(&self->type);
}

il_expr_t il_const_int(il_lloc_t lloc, int d) {
  return il_const(lloc, il_val(il_int(), .ul = (unsigned long) d));
}

il_expr_t il_const_float(il_lloc_t lloc, float f) {
  return il_const(lloc, il_val(il_float(), .f = f));
}

il_expr_t il_const_string(il_lloc_t lloc, const char *s) {
  return il_const(lloc, il_val(il_pointer(il_char()), .s = s));
}

il_expr_t il_const(il_lloc_t lloc, il_val_t value) {
  assert(il_defined(value.type));
  return (il_expr_t) {
    .kind = IL_EXPR_CONST,
    .constant = {
      .value = value,
      .lloc = lloc
    }
  };
}

static void il_const_dtor(il_const_t *self) {
  il_type_dtor(&self->type);
}

il_expr_t il_unary(il_lloc_t op_lloc, enum il_op_n op, il_expr_t operand) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_UNARY,
    .unary = {
      .op = op,
      .operand = xmalloc(sizeof(il_expr_t)),
      .lloc = il_lloc(op_lloc, operand.lloc),
      .type = operand.type
    }
  };

  *expr.unary.operand = operand;
  return expr;
}

il_expr_t il_cast(il_lloc_t type_lloc, il_type_t type, il_expr_t operand) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_UNARY,
    .unary = {
      .op = IL_OP_CAST,
      .operand = xmalloc(sizeof(il_expr_t)),
      .lloc = il_lloc(type_lloc, operand.lloc),
      .type = type
    }
  };

  *expr.unary.operand = operand;
  return expr;
}

static void il_unary_dtor(il_unary_t *self) {
  if (self->op == IL_OP_CAST) {
    il_type_dtor(&self->type);
  }
  il_expr_dtor(self->operand);
  xfree(self->operand);
  self->operand = NULL;
}

il_expr_t il_binary(enum il_op_n op, il_expr_t lhs, il_expr_t rhs) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_BINARY,
    .binary = {
      .op = op,
      .lhs = xmalloc(sizeof(il_expr_t)),
      .rhs = xmalloc(sizeof(il_expr_t)),
      .lloc = il_lloc(lhs.lloc, rhs.lloc),
      .type = lhs.type
    }
  };

  *expr.binary.lhs = lhs;
  *expr.binary.rhs = rhs;
  return expr;
}

static void il_binary_dtor(il_binary_t *self) {
  il_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  il_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

il_expr_t il_ternary(il_expr_t lhs, il_expr_t mhs, il_expr_t rhs) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_TERNARY,
    .ternary = {
      .lhs = xmalloc(sizeof(il_expr_t)),
      .mhs = xmalloc(sizeof(il_expr_t)),
      .rhs = xmalloc(sizeof(il_expr_t)),
      .lloc = il_lloc(lhs.lloc, rhs.lloc),
      .type = mhs.type
    }
  };

  *expr.ternary.lhs = lhs;
  *expr.ternary.mhs = mhs;
  *expr.ternary.rhs = rhs;
  return expr;
}

static void il_ternary_dtor(il_ternary_t *self) {
  il_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  il_expr_dtor(self->mhs);
  xfree(self->mhs);
  self->mhs = NULL;
  il_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

il_expr_t il_array_read(il_expr_t lhs, il_lloc_t pos_lloc, il_expr_t pos) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_ARRAY_READ,
    .array_read = {
      .lhs = xmalloc(sizeof(il_expr_t)),
      .pos = xmalloc(sizeof(il_expr_t)),
      .lloc = il_lloc(lhs.lloc, pos_lloc),
      .type = il_type_deref(lhs.type)
    }
  };

  *expr.array_read.lhs = lhs;
  *expr.array_read.pos = pos;
  return expr;
}

static void il_array_read_dtor(il_array_read_t *self) {
  il_type_dtor(&self->type);
  il_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  il_expr_dtor(self->pos);
  xfree(self->pos);
  self->pos = NULL;
}

il_expr_t il_array_write(il_array_read_t array_read, il_expr_t rhs) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_ARRAY_WRITE,
    .array_write = {
      .read = array_read,
      .rhs = xmalloc(sizeof(il_expr_t)),
    }
  };

  expr.lloc = il_lloc(array_read.lloc, rhs.lloc);
  *expr.array_write.rhs = rhs;
  return expr;
}

static void il_array_write_dtor(il_array_write_t *self) {
  il_array_read_dtor(&self->read);
  il_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

il_expr_t il_field_read(il_expr_t lhs, il_id_t id) {
  il_field_t *field = il_field_lookup(lhs.type, id.id);
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_FIELD_READ,
    .field_read = {
      .lhs = xmalloc(sizeof(il_expr_t)),
      .id = id,
      .lloc = il_lloc(lhs.lloc, id.lloc),
      .type = field ? field->type : il_type_undefined()
    }
  };

  *expr.field_read.lhs = lhs;
  return expr;
}

static void il_field_read_dtor(il_field_read_t *self) {
  il_id_dtor(&self->id);
  il_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
}

il_expr_t il_field_write(il_field_read_t field_read, il_expr_t rhs) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_FIELD_WRITE,
    .field_write = {
      .read = field_read,
      .rhs = xmalloc(sizeof(il_expr_t))
    }
  };

  expr.lloc = il_lloc(field_read.lloc, rhs.lloc);
  *expr.field_write.rhs = rhs;
  return expr;
}

static void il_field_write_dtor(il_field_write_t *self) {
  il_field_read_dtor(&self->read);
  il_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

il_expr_t il_call(il_expr_t lhs, il_lloc_t args_lloc, il_exprs_t args) {
  il_expr_t expr = (il_expr_t) {
    .kind = IL_EXPR_CALL,
    .call = {
      .lhs = xmalloc(sizeof(il_expr_t)),
      .args = args,
      .lloc = il_lloc(lhs.lloc, args_lloc)
    }
  };

  *expr.call.lhs = lhs;
  return expr;
}

static void il_call_dtor(il_call_t *self) {
  il_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  il_exprs_dtor(&self->args);
}

void il_expr_dtor(il_expr_t *self) {
  switch (self->kind) {
    case IL_EXPR_UNDEFINED:
      break;
    case IL_EXPR_EXPRS:
      il_exprs_dtor(&self->exprs);
      break;
    case IL_EXPR_ID:
      il_id_dtor(&self->id);
      break;
    case IL_EXPR_CONST:
      il_const_dtor(&self->constant);
      break;
    case IL_EXPR_UNARY:
      il_unary_dtor(&self->unary);
      break;
    case IL_EXPR_BINARY:
      il_binary_dtor(&self->binary);
      break;
    case IL_EXPR_TERNARY:
      il_ternary_dtor(&self->ternary);
      break;
    case IL_EXPR_ARRAY_READ:
      il_array_read_dtor(&self->array_read);
      break;
    case IL_EXPR_ARRAY_WRITE:
      il_array_write_dtor(&self->array_write);
      break;
    case IL_EXPR_FIELD_READ:
      il_field_read_dtor(&self->field_read);
      break;
    case IL_EXPR_FIELD_WRITE:
      il_field_write_dtor(&self->field_write);
      break;
    case IL_EXPR_CALL:
      il_call_dtor(&self->call);
      break;
    default:
      break;
  }
  *self = il_expr_undefined();
}

bool il_expr_is_constant(il_expr_t self) {
  switch (self.kind) {
    case IL_EXPR_CONST:
      return true;
    case IL_EXPR_UNARY:
      return il_expr_is_constant(*self.unary.operand);
    case IL_EXPR_BINARY:
      return il_expr_is_constant(*self.binary.lhs) && il_expr_is_constant(*self.binary.rhs);
    case IL_EXPR_TERNARY:
      return il_expr_is_constant(*self.ternary.lhs) && il_expr_is_constant(*self.ternary.mhs)
             && il_expr_is_constant(*self.ternary.rhs);
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

static il_type_t constant_integer_type(unsigned long int value, enum suffix suffix, int is_decimal) {
  switch (suffix) {
    case SUFFIX_U:
      if (value <= UINT_MAX) {
        return il_uint();
      } else {
        return il_ulong();
      }
    case SUFFIX_L:
    case SUFFIX_LL:
      if (value <= LONG_MAX) {
        return il_long();
      } else {
        if (is_decimal) {
          puts("Conversion of decimal constant to unsigned.");
        }
        return il_ulong();
      }
    case SUFFIX_UL:
    case SUFFIX_ULL:
      return il_ulong();
    case SUFFIX_NONE:
    default:
      if (value <= INT_MAX) {
        return il_int();
      } else if (!is_decimal && value <= UINT_MAX) {
        return il_uint();
      } else if (value <= LONG_MAX) {
        return il_long();
      } else {
        if (is_decimal) {
          puts("Conversion of decimal constant to unsigned.");
        }
        return il_ulong();
      }
  }
}

int il_const_parse(il_lloc_t lloc, const char *s, size_t len, il_expr_t *out) {
  const char *str;
  char *endptr;
  enum suffix suffix;

  str = s;
  /*
   * Try to read as integer. Handle suffixes u, l, ll, ul, ull, in all
   * permutations of upper- and lower case.
   */
  errno = 0;
  *out = (il_expr_t) {.kind = IL_EXPR_CONST, .lloc = lloc};
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
        out->type = il_float();
        endptr++;
      } else if (*endptr == 'l' || *endptr == 'L') {
        out->constant.ld = out->constant.d;
        out->type = il_long_double();
        endptr++;
      } else {
        out->type = il_double();
      }
    } else {
      out->type = il_double();
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

long il_eval_long(il_expr_t expr) {
  return 0;
}

unsigned long il_eval_ulong(il_expr_t expr) {
  return 0;
}
