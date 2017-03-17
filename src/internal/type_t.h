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

typedef enum jl_type_n jl_type_n;
typedef enum jl_literal_n jl_literal_n;
typedef enum jl_type_specifier_n jl_type_specifier_n;
typedef enum jl_type_qualifier_n jl_type_qualifier_n;

typedef struct jl_type_t jl_type_t;

enum jl_type_n {
  JL_TYPE_UNDEFINED = 0,
  JL_TYPE_LITERAL,
  JL_TYPE_POINTER,
  JL_TYPE_ARRAY,
  JL_TYPE_COMPOUND
};

enum jl_literal_n {
  JL_LITERAL_VOID = 0,
  JL_LITERAL_NULL,
  JL_LITERAL_STRING,
  JL_LITERAL_BOOL,
  JL_LITERAL_CHAR,
  JL_LITERAL_SHORT,
  JL_LITERAL_INT,
  JL_LITERAL_LONG,
  JL_LITERAL_FLOAT,
  JL_LITERAL_DOUBLE
};

enum jl_type_specifier_n {
  JL_TYPE_SPECIFIER_NONE = 0,
  JL_TYPE_SPECIFIER_SIGNED = 1 << 0,
  JL_TYPE_SPECIFIER_UNSIGNED = 1 << 1,
  JL_TYPE_SPECIFIER_BOOL = 1 << 2,
  JL_TYPE_SPECIFIER_CHAR = 1 << 3,
  JL_TYPE_SPECIFIER_SHORT = 1 << 4,
  JL_TYPE_SPECIFIER_INT = 1 << 5,
  JL_TYPE_SPECIFIER_LONG = 1 << 6,
  JL_TYPE_SPECIFIER_FLOAT = 1 << 7,
  JL_TYPE_SPECIFIER_DOUBLE = 1 << 8,
  JL_TYPE_SPECIFIER_EXTERN = 1 << 9,
  JL_TYPE_SPECIFIER_STATIC = 1 << 10,
  JL_TYPE_SPECIFIER_THREAD_LOCAL = 1 << 11,
  JL_TYPE_SPECIFIER_AUTO = 1 << 12,
  JL_TYPE_SPECIFIER_REGISTER = 1 << 13
};

enum jl_type_qualifier_n {
  JL_TYPE_QUALIFIER_NONE = 0,
  JL_TYPE_QUALIFIER_CONST = 1 << 0,
  JL_TYPE_QUALIFIER_VOLATILE = 1 << 1,
  JL_TYPE_QUALIFIER_RESTRICT = 1 << 2,
  JL_TYPE_QUALIFIER_ATOMIC = 1 << 3
};

struct jl_type_t {
  jl_type_n kind : 8;
  jl_type_specifier_n specifiers;
  jl_type_qualifier_n qualifiers;
  union {
    struct jl_literal_t *_literal;
    struct jl_pointer_t *_pointer;
    struct jl_array_t *_array;
    struct jl_compound_t *_compound;
  };
};

#endif /* JL_TYPE_T_H__ */
