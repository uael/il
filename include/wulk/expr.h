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

#ifndef   WULK_EXPR_H__
# define  WULK_EXPR_H__

#include "ir.h"

#define wulk_expr_undefined() ((wulk_expr_t) {.kind = WULK_EXPR_UNDEFINED})
#define wulk_expr_undef(eptr) (*(eptr) = wulk_expr_undefined())

#define wulk_val(vtype, val) ((wulk_val_t) {.type = vtype, val})

wulk_expr_t wulk_exprs(wulk_expr_t *exprs);
wulk_expr_t wulk_exprs_start(wulk_expr_t expr);
wulk_expr_t wulk_id(wulk_lloc_t lloc, const char *id, wulk_type_t type);
wulk_expr_t wulk_const_int(wulk_lloc_t lloc, int d);
wulk_expr_t wulk_const_float(wulk_lloc_t lloc, float f);
wulk_expr_t wulk_const_string(wulk_lloc_t lloc, const char *s);
wulk_expr_t wulk_const(wulk_lloc_t lloc, wulk_val_t value);
wulk_expr_t wulk_unary(wulk_lloc_t op_lloc, enum wulk_op_n op, wulk_expr_t operand);
wulk_expr_t wulk_cast(wulk_lloc_t type_lloc, wulk_type_t type, wulk_expr_t operand);
wulk_expr_t wulk_binary(enum wulk_op_n op, wulk_expr_t lhs, wulk_expr_t rhs);
wulk_expr_t wulk_ternary(wulk_expr_t lhs, wulk_expr_t mhs, wulk_expr_t rhs);
wulk_expr_t wulk_array_read(wulk_expr_t lhs, wulk_lloc_t pos_lloc, wulk_expr_t pos);
wulk_expr_t wulk_array_write(wulk_array_read_t array_read, wulk_expr_t rhs);
wulk_expr_t wulk_field_read(wulk_expr_t lhs, wulk_id_t id);
wulk_expr_t wulk_field_write(wulk_field_read_t field_read, wulk_expr_t rhs);
wulk_expr_t wulk_call(wulk_expr_t lhs, wulk_lloc_t args_lloc, wulk_exprs_t args);

void wulk_expr_dtor(wulk_expr_t *self);
bool wulk_expr_is_constant(wulk_expr_t self);
int wulk_const_parse(wulk_lloc_t lloc, const char *s, size_t len, wulk_expr_t *out);
long wulk_eval_long(wulk_expr_t expr);
unsigned long wulk_eval_ulong(wulk_expr_t expr);

#endif /* WULK_EXPR_H__ */
