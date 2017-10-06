/*
 * libil - Intermediate Language cross-platform c library
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#ifndef   IL_C_PP_H__
# define  IL_C_PP_H__

#include <u/stdbool.h>
#include "il/adt/hash.h"

#include "c_lexer.h"

typedef struct c_macro_t c_macro_t;
typedef struct c_pp_t c_pp_t;

KHASH_DECLARE(c_macro_ht, __const char *, c_macro_t)

enum c_macro_n {
  C_MACRO_UNDEFINED = 0,
  C_MACRO_OBJECT,
  C_MACRO_FUNC
};

struct c_macro_t {
  enum c_macro_n kind;
  __const char *name;
  bool stringify;
  bool is__line__;
  bool is__file__;
  bool is_vararg;
  il_token_r params;
  il_token_r replacement;
};

void c_macro_init(c_macro_t *self);
void c_macro_dtor(c_macro_t *self);
void c_macro_expand(c_macro_t *self, c_pp_t *pp, il_lexer_t *into);

struct c_pp_t {
  c_macro_ht_t macros;
  il_lexer_t lexer;
};

void c_pp_init(c_pp_t *self);
void c_pp_dtor(c_pp_t *self);
void c_pp_parse_define(c_pp_t *self, il_lexer_t *lexer);
void c_pp_parse_undef(c_pp_t *self, il_lexer_t *lexer);

bool c_pp_on_push_callback(il_lexer_event_t *self, void *arg);
void c_pp_on_push_dtor(il_lexer_event_t *self);

#endif /* IL_C_PP_H__ */
