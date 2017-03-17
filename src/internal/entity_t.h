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

#ifndef   JL_ENTITY_T_H__
# define  JL_ENTITY_T_H__

#include <adt/vector.h>

typedef enum jl_entity_n jl_entity_n;
typedef enum jl_func_specifier_n jl_func_specifier_n;

typedef struct jl_entity_t jl_entity_t;
typedef jl_vector_of(jl_entity_t) jl_entity_r;

enum jl_entity_n {
  JL_ENTITY_UNDEFINED = 0,
  JL_ENTITY_VAR,
  JL_ENTITY_PARAM,
  JL_ENTITY_FUNC,
  JL_ENTITY_ENUM,
  JL_ENTITY_STRUCT,
  JL_ENTITY_UNION,
  JL_ENTITY_LABEL
};

enum jl_func_specifier_n {
  JL_FUNC_SPECIFIER_NONE = 0,
  JL_FUNC_SPECIFIER_INLINE = 1 << 0,
  JL_FUNC_SPECIFIER_NORETURN = 1 << 1
};

struct jl_entity_t {
  jl_entity_n kind : 8;
  union {
    struct jl_var_t *_var;
    struct jl_param_t *_param;
    struct jl_func_t *_func;
    struct jl_enum_t *_enum;
    struct jl_struct_t *_struct;
    struct jl_union_t *_union;
    struct jl_label_t *_label;
  };
};

#endif /* JL_ENTITY_T_H__ */
