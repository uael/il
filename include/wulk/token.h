/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef   WULK_TOKEN_H__
# define  WULK_TOKEN_H__

#include <adt/deque.h>

#include "api.h"

struct wulk_lexer_t;

typedef struct wulk_loc wulk_loc_t;
typedef struct wulk_token wulk_token_t;
typedef struct wulk_rtoken wulk_rtoken_t;
typedef struct wulk_lloc wulk_lloc_t;

typedef adt_deque_of(wulk_token_t) wulk_token_r;

enum {
  WULK_TOKEN_UNDEFINED = 0,
  WULK_TOKEN_KEYWORD,
  WULK_TOKEN_SYNTAX,
  WULK_TOKEN_NUMBER,
  WULK_TOKEN_IDENTIFIER,
  WULK_TOKEN_STRING_LITERAL
};

struct wulk_loc {
  uint32_t lineno;
  uint32_t colno;
  uint32_t position;
  uint32_t file_id;
};

struct wulk_token {
  unsigned int kind : 4;
  char type;
  const char *name, *value;
  size_t cursor;
  wulk_loc_t loc;
  uint32_t length, leading_ws;
};

struct wulk_rtoken {
  wulk_token_t begin, end;
};

struct wulk_lloc {
  struct wulk_lexer_t *lexer;
  size_t begin, end;
};

#define wulk_lloc(b, e) ((wulk_lloc_t) {b.lexer, b.begin, e.end})
#define wulk_no_lloc() ((wulk_lloc_t) {NULL, 0, 0})

void wulk_token_dtor(wulk_token_t *self);

wulk_lloc_t wulk_lloc_begin(struct wulk_lexer_t *lexer);
wulk_lloc_t wulk_lloc_end(wulk_lloc_t self);
wulk_rtoken_t wulk_llocate(wulk_lloc_t lloc);

#endif /* WULK_TOKEN_H__ */
