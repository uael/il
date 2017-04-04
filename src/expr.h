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

struct jl_expr_id_t {
  unsigned refs;
  const char *id;
  bool is_keyword;
};

jl_expr_t jl_id(const char *id, jl_type_t type);
void jl_id_init(jl_expr_t *self, const char *id, jl_type_t type);

struct jl_expr_const_t {
  unsigned refs;
  union {
    const char *_s;
    float _f;
    double _d;
    long double _ld;
    unsigned long int _ul;
  } u;
};

jl_expr_t jl_const_int(int d);
jl_expr_t jl_const_float(float f);
jl_expr_t jl_const_string(const char *s);
void jl_const_init(jl_expr_t *self, jl_type_t type);
int jl_const_parse(const char *s, size_t len, jl_expr_t *out);

struct jl_expr_unary_t {
  unsigned refs;
  enum jl_op_n op;
  jl_expr_t operand;
};

jl_expr_t jl_unary(enum jl_op_n op, jl_expr_t operand);
jl_expr_t jl_cast(jl_type_t type, jl_expr_t operand);
void jl_unary_init(jl_expr_t *self, enum jl_op_n op, jl_expr_t operand);

struct jl_expr_binary_t {
  unsigned refs;
  enum jl_op_n op;
  jl_expr_t lhs, rhs;
};

jl_expr_t jl_binary(enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);
void jl_binary_init(jl_expr_t *self, enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs);

struct jl_expr_ternary_t {
  unsigned refs;
  jl_expr_t lhs, mhs, rhs;
};

jl_expr_t jl_ternary(jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs);
void jl_ternary_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs);

struct jl_expr_array_read_t {
  unsigned refs;
  jl_expr_t lhs, pos;
};

jl_expr_t jl_array_read(jl_expr_t lhs, jl_expr_t pos);
void jl_array_read_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t pos);

struct jl_expr_array_write_t {
  unsigned refs;
  jl_expr_t lhs, pos, rhs;
};

jl_expr_t jl_array_write(jl_expr_t lhs, jl_expr_t pos, jl_expr_t rhs);
void jl_array_write_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t pos, jl_expr_t rhs);

struct jl_expr_field_read_t {
  unsigned refs;
  bool ptr;
  jl_expr_t lhs;
  jl_expr_t field;
  short width, offset;
};

jl_expr_t jl_field_read(jl_expr_t lhs, jl_expr_t field, short width, short offset);
void jl_field_read_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_t field, short width, short offset);

struct jl_expr_field_write_t {
  unsigned refs;
  bool ptr;
  jl_expr_t lhs, rhs;
  jl_expr_t field;
  short width, offset;
};

jl_expr_t jl_field_write(jl_expr_t lhs, bool ptr, jl_expr_t field, jl_expr_t rhs);
void jl_field_write_init(jl_expr_t *self, jl_expr_t lhs, bool ptr, jl_expr_t field, jl_expr_t rhs);

struct jl_expr_call_t {
  unsigned refs;
  jl_expr_t lhs;
  jl_expr_r args;
};

jl_expr_t jl_call(jl_expr_t lhs, jl_expr_r args);
void jl_call_init(jl_expr_t *self, jl_expr_t lhs, jl_expr_r args);

struct jl_expr_list_t {
  unsigned refs;
  jl_expr_r exprs;
};

jl_expr_t jl_exprs(jl_expr_r exprs);
jl_expr_t jl_exprs_start(jl_expr_t first);
void jl_exprs_init(jl_expr_t *self, jl_expr_r exprs);
size_t jl_exprs_length(jl_expr_t *self);
void jl_exprs_push(jl_expr_t *self, jl_expr_t expr);
jl_expr_t jl_exprs_pop(jl_expr_t *self);
void jl_exprs_unshift(jl_expr_t *self, jl_expr_t expr);
jl_expr_t jl_exprs_shift(jl_expr_t *self);

#endif /* JL_EXPR_H__ */
