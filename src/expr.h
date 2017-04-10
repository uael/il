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

#ifndef   JL_EXPR_H__
# define  JL_EXPR_H__

#include "ir.h"

#define jl_expr_undefined() ((jl_expr_t) {.kind = JL_EXPR_UNDEFINED})
#define jl_expr_undef(eptr) (*(eptr) = jl_expr_undefined())

#define jl_val(vtype, val) ((jl_val_t) {.type = vtype, val})

jl_expr_t jl_exprs(jl_expr_t *exprs);
jl_expr_t jl_exprs_start(jl_expr_t expr);
jl_expr_t jl_id(jl_lloc_t lloc, const char *id, jl_type_t type);
jl_expr_t jl_const_int(jl_lloc_t lloc, int d);
jl_expr_t jl_const_float(jl_lloc_t lloc, float f);
jl_expr_t jl_const_string(jl_lloc_t lloc, const char *s);
jl_expr_t jl_const(jl_lloc_t lloc, jl_val_t value);
jl_expr_t jl_unary(jl_lloc_t op_lloc, enum jl_op_n op, jl_expr_t operand);
jl_expr_t jl_cast(jl_lloc_t type_lloc, jl_type_t type, jl_expr_t operand);
jl_expr_t jl_binary(enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
jl_expr_t jl_ternary(jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs);
jl_expr_t jl_array_read(jl_expr_t lhs, jl_lloc_t pos_lloc, jl_expr_t pos);
jl_expr_t jl_array_write(jl_array_read_t array_read, jl_expr_t rhs);
jl_expr_t jl_field_read(jl_expr_t lhs, jl_id_t id);
jl_expr_t jl_field_write(jl_field_read_t field_read, jl_expr_t rhs);
jl_expr_t jl_call(jl_expr_t lhs, jl_lloc_t args_lloc, jl_exprs_t args);

void jl_expr_dtor(jl_expr_t *self);
bool jl_expr_is_constant(jl_expr_t self);
int jl_const_parse(jl_lloc_t lloc, const char *s, size_t len, jl_expr_t *out);
long jl_eval_long(jl_expr_t expr);
unsigned long jl_eval_ulong(jl_expr_t expr);

#endif /* JL_EXPR_H__ */
