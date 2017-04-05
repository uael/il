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

#include "ir.h"

#define jl_type_undefined() ((jl_type_t) {.kind = JL_TYPE_UNDEFINED})
#define jl_type_undef(tptr) (*(tptr) = jl_type_undefined())

#define jl_type_specified(t, by) ((t).specifiers & JL_TYPE_SPECIFIER_ ## by)
#define jl_type_qualified(t, by) ((t).qualifiers & JL_TYPE_QUALIFIER_ ## by)

enum {
  JL_TYPE_MERGE_ERROR_NONE = 0,
  JL_TYPE_MERGE_ERROR_TWO_OR_MORE_DATA_TYPE,
  JL_TYPE_MERGE_ERROR_BOTH_LONG_AND,
  JL_TYPE_MERGE_ERROR_BOTH_LONG_LONG_AND
};

jl_type_t jl_void();
jl_type_t jl_bool();
jl_type_t jl_char();
jl_type_t jl_short();
jl_type_t jl_int();
jl_type_t jl_uint();
jl_type_t jl_long();
jl_type_t jl_ulong();
jl_type_t jl_double();
jl_type_t jl_float();
jl_type_t jl_long_long();
jl_type_t jl_long_double();
jl_type_t jl_pointer(jl_type_t of);
jl_type_t jl_array(jl_type_t of, jl_expr_t size);
jl_type_t jl_compound(jl_entity_t entity);

void jl_type_dtor(jl_type_t *self);
bool jl_type_is_ref(jl_type_t type);
bool jl_type_is_func(jl_type_t type);
bool jl_type_equals(jl_type_t a, jl_type_t b);
jl_type_t jl_type_deref(jl_type_t a);
jl_entity_r jl_type_fields(jl_type_t self);
jl_field_t *jl_field_lookup(jl_type_t self, const char *name);
unsigned jl_type_merge(jl_type_t *self, enum jl_type_n with);

#endif /* JL_TYPE_H__ */
