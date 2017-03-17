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

#include "entity_t.h"
#include "expr_t.h"
#include "stmt_t.h"
#include "type_t.h"

jl_expr_t jl_expr_undefined();
void jl_expr_dtor(jl_expr_t *self);
void jl_expr_switch(jl_expr_t *self, jl_expr_n kind);
void jl_expr_acquire(jl_expr_t *self);
void jl_expr_release(jl_expr_t *self);
bool jl_expr_is_defined(jl_expr_t *self);
jl_type_t jl_expr_get_type(jl_expr_t *self);
void jl_expr_set_type(jl_expr_t *self, jl_type_t type);
jl_expr_t jl_expr_get_next(jl_expr_t *self);
void jl_expr_set_next(jl_expr_t *self, jl_expr_t next);

#define jl_expr_is_id(t) ((t).kind == JL_EXPR_ID)
#define jl_expr_is_const(t) ((t).kind == JL_EXPR_CONST)
#define jl_expr_is_unary(t) ((t).kind == JL_EXPR_UNARY)
#define jl_expr_is_binary(t) ((t).kind == JL_EXPR_BINARY)
#define jl_expr_is_ternary(t) ((t).kind == JL_EXPR_TERNARY)
#define jl_expr_is_array_read(t) ((t).kind == JL_EXPR_ARRAY_READ)
#define jl_expr_is_array_write(t) ((t).kind == JL_EXPR_ARRAY_WRITE)
#define jl_expr_is_field_read(t) ((t).kind == JL_EXPR_FIELD_READ)
#define jl_expr_is_field_write(t) ((t).kind == JL_EXPR_FIELD_WRITE)
#define jl_expr_is_call(t) ((t).kind == JL_EXPR_CALL)


jl_expr_t jl_id(const char *id, bool is_keyword);
void jl_id_init(jl_expr_t *self, const char *id, bool is_keyword);
const char *jl_id_get_id(jl_expr_t *self);
void jl_id_set_id(jl_expr_t *self, const char *id);
bool jl_id_is_keyword(jl_expr_t *self);
void jl_id_set_is_keyword(jl_expr_t *self, bool is_keyword);


jl_expr_t jl_const_int(int d);
jl_expr_t jl_const_float(float f);
jl_expr_t jl_const_string(const char *s);
void jl_const_init(jl_expr_t *self, jl_type_t type);
int jl_const_get_int(jl_expr_t *self);
void jl_const_set_int(jl_expr_t *self, int d);
float jl_const_get_float(jl_expr_t *self);
void jl_const_set_float(jl_expr_t *self, float d);
const char *jl_const_get_string(jl_expr_t *self);
void jl_const_set_string(jl_expr_t *self, const char *s);


jl_expr_t jl_unary(jl_op_n op, jl_expr_t operand);
void jl_unary_init(jl_expr_t *self, jl_op_n op, jl_expr_t operand);
jl_op_n jl_unary_get_op(jl_expr_t *self);
void jl_unary_set_op(jl_expr_t *self, jl_op_n op);
jl_expr_t jl_unary_get_operand(jl_expr_t *self);
void jl_unary_set_operand(jl_expr_t *self, jl_expr_t operand);


jl_expr_t jl_binary(jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
void jl_binary_init(jl_expr_t *self, jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
jl_op_n jl_binary_get_op(jl_expr_t *self);
void jl_binary_set_op(jl_expr_t *self, jl_op_n op);
jl_expr_t jl_binary_get_lhs(jl_expr_t *self);
void jl_binary_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_binary_get_rhs(jl_expr_t *self);
void jl_binary_set_rhs(jl_expr_t *self, jl_expr_t rhs);


jl_expr_t jl_ternary(jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs);
void jl_ternary_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs);
jl_expr_t jl_ternary_get_lhs(jl_expr_t *self);
void jl_ternary_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_ternary_get_mhs(jl_expr_t *self);
void jl_ternary_set_mhs(jl_expr_t *self, jl_expr_t mhs);
jl_expr_t jl_ternary_get_rhs(jl_expr_t *self);
void jl_ternary_set_rhs(jl_expr_t *self, jl_expr_t rhs);


jl_expr_t jl_array_read(jl_expr_t lhs, jl_expr_t pos);
void jl_array_read_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t pos);
jl_expr_t jl_array_read_get_lhs(jl_expr_t *self);
void jl_array_read_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_array_read_get_pos(jl_expr_t *self);
void jl_array_read_set_pos(jl_expr_t *self, jl_expr_t pos);


jl_expr_t jl_array_write(jl_expr_t lhs, jl_expr_t pos, jl_expr_t rhs);
void jl_array_write_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t pos, jl_expr_t rhs);
jl_expr_t jl_array_write_get_lhs(jl_expr_t *self);
void jl_array_write_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_array_write_get_pos(jl_expr_t *self);
void jl_array_write_set_pos(jl_expr_t *self, jl_expr_t pos);
jl_expr_t jl_array_write_get_rhs(jl_expr_t *self);
void jl_array_write_set_rhs(jl_expr_t *self, jl_expr_t rhs);


jl_expr_t jl_field_read(jl_expr_t lhs, bool ptr, jl_expr_t field);
void jl_field_read_init(jl_expr_t *self, jl_expr_t lhs, bool ptr, jl_expr_t field);
jl_expr_t jl_field_read_get_lhs(jl_expr_t *self);
void jl_field_read_set_lhs(jl_expr_t *self, jl_expr_t lhs);
bool jl_field_read_get_ptr(jl_expr_t *self);
void jl_field_read_set_ptr(jl_expr_t *self, bool ptr);
jl_expr_t jl_field_read_get_field(jl_expr_t *self);
void jl_field_read_set_field(jl_expr_t *self, jl_expr_t field);


jl_expr_t jl_field_write(jl_expr_t lhs, bool ptr, jl_expr_t field, jl_expr_t rhs);
void jl_field_write_init(jl_expr_t *self, jl_expr_t lhs, bool ptr, jl_expr_t field, jl_expr_t rhs);
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
jl_expr_t jl_call_get_lhs(jl_expr_t *self);
void jl_call_set_lhs(jl_expr_t *self, jl_expr_t lhs);
jl_expr_t jl_call_get_args(jl_expr_t *self);
void jl_call_set_args(jl_expr_t *self, jl_expr_t args);

#endif /* JL_EXPR_H__ */
