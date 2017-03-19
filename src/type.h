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

#ifndef   JL_TYPE_H__
# define  JL_TYPE_H__

#include "entity_t.h"
#include "expr_t.h"
#include "stmt_t.h"
#include "type_t.h"

struct jl_literal_t {
  unsigned refs;
  jl_literal_n kind;
};

jl_type_t jl_void();
jl_type_t jl_null();
jl_type_t jl_string();
jl_type_t jl_bool();
jl_type_t jl_char();
jl_type_t jl_short();
jl_type_t jl_int();
jl_type_t jl_long();
jl_type_t jl_double();
jl_type_t jl_float();
jl_type_t jl_literal(jl_literal_n kind);
void jl_literal_init(jl_type_t *self, jl_literal_n kind);

struct jl_pointer_t {
  unsigned refs;
  jl_type_t of;
};

jl_type_t jl_pointer(jl_type_t of);
void jl_pointer_init(jl_type_t *self, jl_type_t of);

struct jl_array_t {
  unsigned refs;
  jl_type_t of;
  jl_expr_t size;
};

jl_type_t jl_array(jl_type_t of);
jl_type_t jl_narray(jl_type_t of, jl_expr_t size);
void jl_array_init(jl_type_t *self, jl_type_t of, jl_expr_t size);

struct jl_compound_t {
  unsigned refs;
  jl_entity_t entity;
};

jl_type_t jl_compound(jl_entity_t entity);
void jl_compound_init(jl_type_t *self, jl_entity_t entity);

#endif /* JL_TYPE_H__ */
