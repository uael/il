/*
 * MIT License
 *
 * Copyright (c) 2016-2017 uael <www.github.com/uael>
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

#ifndef   JL_C_PP_H__
# define  JL_C_PP_H__

#include <adt/bool.h>
#include <adt/hash.h>

#include "c_lexer.h"

typedef struct c_macro_t c_macro_t;
typedef struct c_pp_t c_pp_t;

KHASH_DECLARE(c_macro_ht, const char *, c_macro_t)

enum c_macro_n {
  C_MACRO_UNDEFINED = 0,
  C_MACRO_OBJECT,
  C_MACRO_FUNC
};

struct c_macro_t {
  enum c_macro_n kind;
  const char *name;
  bool stringify;
  bool is__line__;
  bool is__file__;
  bool is_vararg;
  jl_token_r params;
  jl_token_r replacement;
};

void c_macro_init(c_macro_t *self);
void c_macro_dtor(c_macro_t *self);
void c_macro_expand(c_macro_t *self, c_pp_t *pp, jl_lexer_t *into);

struct c_pp_t {
  c_macro_ht_t macros;
  jl_lexer_t lexer;
};

void c_pp_init(c_pp_t *self);
void c_pp_dtor(c_pp_t *self);
void c_pp_parse_define(c_pp_t *self, jl_lexer_t *lexer);
void c_pp_parse_undef(c_pp_t *self, jl_lexer_t *lexer);

bool c_pp_on_push_callback(jl_lexer_event_t *self, void *arg);
void c_pp_on_push_dtor(jl_lexer_event_t *self);

#endif /* JL_C_PP_H__ */
