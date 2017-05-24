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
