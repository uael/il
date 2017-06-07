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

#ifndef   IL_TOKEN_H__
# define  IL_TOKEN_H__

#include <adt/deque.h>

#include "api.h"

struct il_lexer_t;

typedef struct il_loc il_loc_t;
typedef struct il_token il_token_t;
typedef struct il_rtoken il_rtoken_t;
typedef struct il_lloc il_lloc_t;

typedef adt_deque_of(il_token_t) il_token_r;

enum {
  IL_TOKEN_UNDEFINED = 0,
  IL_TOKEN_KEYWORD,
  IL_TOKEN_SYNTAX,
  IL_TOKEN_NUMBER,
  IL_TOKEN_IDENTIFIER,
  IL_TOKEN_STRING_LITERAL
};

struct il_loc {
  uint32_t lineno;
  uint32_t colno;
  uint32_t position;
  uint32_t file_id;
};

struct il_token {
  unsigned int kind : 4;
  char type;
  const char *name, *value;
  size_t cursor;
  il_loc_t loc;
  uint32_t length, leading_ws;
};

struct il_rtoken {
  il_token_t begin, end;
};

struct il_lloc {
  struct il_lexer_t *lexer;
  size_t begin, end;
};

#define il_lloc(b, e) ((il_lloc_t) {b.lexer, b.begin, e.end})
#define il_no_lloc() ((il_lloc_t) {NULL, 0, 0})

void il_token_dtor(il_token_t *self);

il_lloc_t il_lloc_begin(struct il_lexer_t *lexer);
il_lloc_t il_lloc_end(il_lloc_t self);
il_rtoken_t il_llocate(il_lloc_t lloc);

#endif /* IL_TOKEN_H__ */
