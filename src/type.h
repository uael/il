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

jl_type_t jl_type_undefined();
void jl_type_dtor(jl_type_t *self);
void jl_type_switch(jl_type_t *self, jl_type_n kind);
void jl_type_acquire(jl_type_t *self);
void jl_type_release(jl_type_t *self);
bool jl_type_is_defined(jl_type_t *self);

#define jl_type_is_literal(t) ((t).kind == JL_TYPE_LITERAL)
#define jl_type_is_pointer(t) ((t).kind == JL_TYPE_POINTER)
#define jl_type_is_array(t) ((t).kind == JL_TYPE_ARRAY)
#define jl_type_is_compound(t) ((t).kind == JL_TYPE_COMPOUND)

#define jl_type_is_specified(t) ((t).specifiers != 0)
#define jl_type_is_signed(t) ((t).specifiers & JL_TYPE_SPECIFIER_SIGNED)
#define jl_type_is_unsigned(t) ((t).specifiers & JL_TYPE_SPECIFIER_UNSIGNED)
#define jl_type_is_extern(t) ((t).specifiers & JL_TYPE_SPECIFIER_EXTERN)
#define jl_type_is_static(t) ((t).specifiers & JL_TYPE_SPECIFIER_STATIC)
#define jl_type_is_thread_local(t) ((t).specifiers & JL_TYPE_SPECIFIER_THREAD_LOCAL)
#define jl_type_is_auto(t) ((t).specifiers & JL_TYPE_SPECIFIER_AUTO)
#define jl_type_is_register(t) ((t).specifiers & JL_TYPE_SPECIFIER_REGISTER)

#define jl_type_is_qualified(t) ((t).qualifiers != 0)
#define jl_type_is_const(t) ((t).qualifiers & JL_TYPE_QUALIFIER_CONST)
#define jl_type_is_volatile(t) ((t).qualifiers & JL_TYPE_QUALIFIER_VOLATILE)
#define jl_type_is_restrict(t) ((t).qualifiers & JL_TYPE_QUALIFIER_RESTRICT)
#define jl_type_is_atomic(t) ((t).qualifiers & JL_TYPE_QUALIFIER_ATOMIC)


jl_type_t jl_literal_void();
jl_type_t jl_literal_null();
jl_type_t jl_literal_string();
jl_type_t jl_literal_bool();
jl_type_t jl_literal_char();
jl_type_t jl_literal_short();
jl_type_t jl_literal_int();
jl_type_t jl_literal_long();
jl_type_t jl_literal_double();
jl_type_t jl_literal_float();
void jl_literal_init(jl_type_t *self, jl_literal_n kind);
jl_literal_n jl_literal_get_kind(jl_type_t *self);
void jl_literal_set_kind(jl_type_t *self, jl_literal_n kind);


jl_type_t jl_pointer_of(jl_type_t of);
void jl_pointer_init(jl_type_t *self, jl_type_t of);
jl_type_t jl_pointer_get_of(jl_type_t *self);
void jl_pointer_set_of(jl_type_t *self, jl_type_t of);


jl_type_t jl_array_of(jl_type_t of);
jl_type_t jl_array_nof(jl_type_t of, jl_expr_t size);
void jl_array_init(jl_type_t *self, jl_type_t of, jl_expr_t size);
jl_type_t jl_array_get_of(jl_type_t *self);
void jl_array_set_of(jl_type_t *self, jl_type_t of);
jl_expr_t jl_array_get_size(jl_type_t *self);
void jl_array_set_size(jl_type_t *self, jl_expr_t size);

#endif /* JL_TYPE_H__ */
