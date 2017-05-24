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
