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

#ifndef   JL_TYPE_T_H__
# define  JL_TYPE_T_H__

#include <assert.h>

#include "api.h"

typedef struct jl_type_t jl_type_t;
typedef struct jl_pointer_t jl_pointer_t;
typedef struct jl_array_t jl_array_t;
typedef struct jl_compound_t jl_compound_t;

enum jl_type_n {
  JL_TYPE_UNDEFINED = 0,
  JL_TYPE_VOID,
  JL_TYPE_POINTER,
  JL_TYPE_ARRAY,
  JL_TYPE_COMPOUND,
  JL_TYPE_BOOL,
  JL_TYPE_CHAR,
  JL_TYPE_SHORT,
  JL_TYPE_INT,
  JL_TYPE_LONG,
  JL_TYPE_FLOAT,
  JL_TYPE_DOUBLE,
  JL_TYPE_LONG_LONG,
  JL_TYPE_LONG_DOUBLE
};

enum jl_type_specifier_n {
  JL_TYPE_SPECIFIER_NONE = 0,
  JL_TYPE_SPECIFIER_SIGNED = 1 << 0,
  JL_TYPE_SPECIFIER_UNSIGNED = 1 << 1,
  JL_TYPE_SPECIFIER_COMPLEX = 1 << 2,
  JL_TYPE_SPECIFIER_IMAGINARY = 1 << 3,
  JL_TYPE_SPECIFIER_TYPEDEF = 1 << 4,
  JL_TYPE_SPECIFIER_EXTERN = 1 << 5,
  JL_TYPE_SPECIFIER_STATIC = 1 << 6,
  JL_TYPE_SPECIFIER_THREAD_LOCAL = 1 << 7,
  JL_TYPE_SPECIFIER_AUTO = 1 << 8,
  JL_TYPE_SPECIFIER_REGISTER = 1 << 9,
  JL_FUNC_SPECIFIER_INLINE = 1 << 10,
  JL_FUNC_SPECIFIER_NORETURN = 1 << 11
};

enum jl_type_qualifier_n {
  JL_TYPE_QUALIFIER_NONE = 0,
  JL_TYPE_QUALIFIER_CONST = 1 << 0,
  JL_TYPE_QUALIFIER_VOLATILE = 1 << 1,
  JL_TYPE_QUALIFIER_RESTRICT = 1 << 2,
  JL_TYPE_QUALIFIER_ATOMIC = 1 << 3
};

struct jl_type_t {
  unsigned kind : 8;
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;
  size_t size;
  union {
    jl_pointer_t *_pointer;
    jl_array_t *_array;
    jl_compound_t *_compound;
  } u;
};

void jl_type_undef(jl_type_t *self);
void jl_type_dtor(jl_type_t *self);
void jl_type_switch(jl_type_t *self, enum jl_type_n kind);
void jl_type_acquire(jl_type_t *self);
void jl_type_release(jl_type_t *self);
void jl_type_update_size(jl_type_t *self);
bool jl_type_is_defined(jl_type_t self);
bool jl_ptype_is_defined(jl_type_t *self);
bool jl_type_is_ref(jl_type_t type);
bool jl_type_is_func(jl_type_t type);
bool jl_type_equals(jl_type_t a, jl_type_t b);
jl_type_t jl_literal(enum jl_type_n kind);
jl_type_t jl_type_deref(jl_type_t a);
size_t jl_sizeof(jl_type_t type);
size_t jl_type_alignment(jl_type_t type);

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

#define jl_type_undefined() ((jl_type_t) {JL_TYPE_UNDEFINED})

#define jl_type_is_bool(t) ((t).kind == JL_TYPE_BOOL)
#define jl_type_is_char(t) ((t).kind == JL_TYPE_CHAR)
#define jl_type_is_short(t) ((t).kind == JL_TYPE_SHORT)
#define jl_type_is_int(t) ((t).kind == JL_TYPE_INT)
#define jl_type_is_long(t) ((t).kind == JL_TYPE_LONG)
#define jl_type_is_float(t) ((t).kind == JL_TYPE_FLOAT)
#define jl_type_is_double(t) ((t).kind == JL_TYPE_DOUBLE)
#define jl_type_is_long_double(t) ((t).kind == LONG_DOUBLE)
#define jl_type_is_long_long(t) ((t).kind == JL_TYPE_LONG_LONG)
#define jl_type_is_void(t) ((t).kind == JL_TYPE_VOID)
#define jl_type_is_pointer(t) ((t).kind == JL_TYPE_POINTER)
#define jl_type_is_array(t) ((t).kind == JL_TYPE_ARRAY)
#define jl_type_is_compound(t) ((t).kind == JL_TYPE_COMPOUND)
#define jl_type_is_literal(t) ((t).kind >= JL_TYPE_BOOL)
#define jl_ptype_is_literal(t) ((t)->kind >= JL_TYPE_BOOL)
#define jl_ptype_is_bool(t) ((t)->kind == JL_TYPE_BOOL)
#define jl_ptype_is_char(t) ((t)->kind == JL_TYPE_CHAR)
#define jl_ptype_is_short(t) ((t)->kind == JL_TYPE_SHORT)
#define jl_ptype_is_int(t) ((t)->kind == JL_TYPE_INT)
#define jl_ptype_is_long(t) ((t)->kind == JL_TYPE_LONG)
#define jl_ptype_is_float(t) ((t)->kind == JL_TYPE_FLOAT)
#define jl_ptype_is_double(t) ((t)->kind == JL_TYPE_DOUBLE)
#define jl_ptype_is_long_double(t) ((t)->kind == LONG_DOUBLE)
#define jl_ptype_is_long_long(t) ((t)->kind == JL_TYPE_LONG_LONG)
#define jl_ptype_is_void(t) ((t)->kind == JL_TYPE_VOID)
#define jl_ptype_is_pointer(t) ((t)->kind == JL_TYPE_POINTER)
#define jl_ptype_is_array(t) ((t)->kind == JL_TYPE_ARRAY)
#define jl_ptype_is_compound(t) ((t)->kind == JL_TYPE_COMPOUND)

#define jl_type_is_specified(t) ((t).specifiers != 0)
#define jl_type_is_signed(t) ((t).specifiers & JL_TYPE_SPECIFIER_SIGNED)
#define jl_type_is_unsigned(t) ((t).specifiers & JL_TYPE_SPECIFIER_UNSIGNED)
#define jl_type_is_typedef(t) ((t).specifiers & JL_TYPE_SPECIFIER_TYPEDEF)
#define jl_type_is_extern(t) ((t).specifiers & JL_TYPE_SPECIFIER_EXTERN)
#define jl_type_is_static(t) ((t).specifiers & JL_TYPE_SPECIFIER_STATIC)
#define jl_type_is_thread_local(t) ((t).specifiers & JL_TYPE_SPECIFIER_THREAD_LOCAL)
#define jl_type_is_auto(t) ((t).specifiers & JL_TYPE_SPECIFIER_AUTO)
#define jl_type_is_register(t) ((t).specifiers & JL_TYPE_SPECIFIER_REGISTER)
#define jl_ptype_is_specified(t) ((t)->specifiers != 0)
#define jl_ptype_is_signed(t) ((t)->specifiers & JL_TYPE_SPECIFIER_SIGNED)
#define jl_ptype_is_unsigned(t) ((t)->specifiers & JL_TYPE_SPECIFIER_UNSIGNED)
#define jl_ptype_is_typedef(t) ((t)->specifiers & JL_TYPE_SPECIFIER_TYPEDEF)
#define jl_ptype_is_extern(t) ((t)->specifiers & JL_TYPE_SPECIFIER_EXTERN)
#define jl_ptype_is_static(t) ((t)->specifiers & JL_TYPE_SPECIFIER_STATIC)
#define jl_ptype_is_thread_local(t) ((t)->specifiers & JL_TYPE_SPECIFIER_THREAD_LOCAL)
#define jl_ptype_is_auto(t) ((t)->specifiers & JL_TYPE_SPECIFIER_AUTO)
#define jl_ptype_is_register(t) ((t)->specifiers & JL_TYPE_SPECIFIER_REGISTER)

#define jl_type_is_qualified(t) ((t).qualifiers != 0)
#define jl_type_is_const(t) ((t).qualifiers & JL_TYPE_QUALIFIER_CONST)
#define jl_type_is_volatile(t) ((t).qualifiers & JL_TYPE_QUALIFIER_VOLATILE)
#define jl_type_is_restrict(t) ((t).qualifiers & JL_TYPE_QUALIFIER_RESTRICT)
#define jl_type_is_atomic(t) ((t).qualifiers & JL_TYPE_QUALIFIER_ATOMIC)
#define jl_ptype_is_qualified(t) ((t)->qualifiers != 0)
#define jl_ptype_is_const(t) ((t)->qualifiers & JL_TYPE_QUALIFIER_CONST)
#define jl_ptype_is_volatile(t) ((t)->qualifiers & JL_TYPE_QUALIFIER_VOLATILE)
#define jl_ptype_is_restrict(t) ((t)->qualifiers & JL_TYPE_QUALIFIER_RESTRICT)
#define jl_ptype_is_atomic(t) ((t)->qualifiers & JL_TYPE_QUALIFIER_ATOMIC)

#define jl_type_pointer(t) ((void) assert(jl_type_is_pointer(t)), (t).u._pointer)
#define jl_type_array(t) ((void) assert(jl_type_is_array(t)), (t).u._array)
#define jl_type_compound(t) ((void) assert(jl_type_is_compound(t)), (t).u._compound)
#define jl_ptype_pointer(t) ((void) assert(jl_ptype_is_pointer(t)), (t)->u._pointer)
#define jl_ptype_array(t) ((void) assert(jl_ptype_is_array(t)), (t)->u._array)
#define jl_ptype_compound(t) ((void) assert(jl_ptype_is_compound(t)), (t)->u._compound)

#endif /* JL_TYPE_T_H__ */
