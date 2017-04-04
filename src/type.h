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

#include <adt/vector.h>

#include "token.h"

struct jl_entity;
struct jl_expr;

typedef struct jl_type jl_type_t;
typedef struct jl_pointer jl_pointer_t;
typedef struct jl_array jl_array_t;
typedef struct jl_compound jl_compound_t;

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

struct jl_pointer {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t *of;
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;
};

struct jl_array {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t *of;
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;

  struct jl_expr *length;
};

struct jl_compound {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t *next;
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;

  struct jl_entity *entity;
};

struct jl_type {
  union {
    struct {
      jl_lloc_t lloc;
      size_t size, align;
      jl_type_t *next;
      enum jl_type_specifier_n specifiers;
      enum jl_type_qualifier_n qualifiers;
    };
    jl_pointer_t pointer;
    jl_array_t array;
    jl_compound_t compound;
  };
  enum jl_type_n kind;
};

#include "entity.h"
#include "expr.h"
#include "stmt.h"

void jl_type_dtor(jl_type_t *self);
bool jl_type_is_ref(jl_type_t type);
bool jl_type_is_func(jl_type_t type);
bool jl_type_equals(jl_type_t a, jl_type_t b);
jl_type_t jl_type_deref(jl_type_t a);

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

jl_entity_r jl_type_fields(jl_type_t self);
jl_field_t *jl_field_lookup(jl_type_t self, const char *name);

#define jl_type_undefined() ((jl_type_t) {.kind = JL_TYPE_UNDEFINED})
#define jl_type_undef(tptr) (*(tptr) = jl_type_undefined())

#define jl_type_specified(t, by) ((t).specifiers & JL_TYPE_SPECIFIER_ ## by)
#define jl_type_qualified(t, by) ((t).qualifiers & JL_TYPE_QUALIFIER_ ## by)

#endif /* JL_TYPE_H__ */
