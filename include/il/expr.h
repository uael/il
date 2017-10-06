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

#ifndef   IL_EXPR_H__
# define  IL_EXPR_H__

#include "ir.h"

#define il_expr_undefined() ((il_expr_t) {.kind = IL_EXPR_UNDEFINED})
#define il_expr_undef(eptr) (*(eptr) = il_expr_undefined())

#define il_val(vtype, val) ((il_val_t) {.type = vtype, val})

il_expr_t il_exprs(il_expr_t *exprs);
il_expr_t il_exprs_start(il_expr_t expr);
il_expr_t il_id(il_lloc_t lloc, __const char *id, il_type_t type);
il_expr_t il_const_int(il_lloc_t lloc, int d);
il_expr_t il_const_float(il_lloc_t lloc, float f);
il_expr_t il_const_string(il_lloc_t lloc, __const char *s);
il_expr_t il_const(il_lloc_t lloc, il_val_t value);
il_expr_t il_unary(il_lloc_t op_lloc, enum il_op_n op, il_expr_t operand);
il_expr_t il_cast(il_lloc_t type_lloc, il_type_t type, il_expr_t operand);
il_expr_t il_binary(enum il_op_n op, il_expr_t lhs, il_expr_t rhs);
il_expr_t il_ternary(il_expr_t lhs, il_expr_t mhs, il_expr_t rhs);
il_expr_t il_array_read(il_expr_t lhs, il_lloc_t pos_lloc, il_expr_t pos);
il_expr_t il_array_write(il_array_read_t array_read, il_expr_t rhs);
il_expr_t il_field_read(il_expr_t lhs, il_id_t id);
il_expr_t il_field_write(il_field_read_t field_read, il_expr_t rhs);
il_expr_t il_call(il_expr_t lhs, il_lloc_t args_lloc, il_exprs_t args);

void il_expr_dtor(il_expr_t *self);
bool il_expr_is_constant(il_expr_t self);
int il_const_parse(il_lloc_t lloc, __const char *s, size_t len, il_expr_t *out);
long il_eval_long(il_expr_t expr);
unsigned long il_eval_ulong(il_expr_t expr);

#endif /* IL_EXPR_H__ */
