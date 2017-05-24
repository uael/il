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

#ifndef   WULK_LEXER_H__
# define  WULK_LEXER_H__

#include <adt/vector.h>

#include "token.h"

struct wulk_compiler_t;
struct wulk_parser_t;

typedef struct wulk_lexer_t wulk_lexer_t;
typedef struct wulk_lexer_event_t wulk_lexer_event_t;

typedef adt_vector_of(wulk_lexer_t) wulk_lexer_r;
typedef adt_vector_of(wulk_lexer_event_t) wulk_lexer_event_r;

enum wulk_lexer_event_n {
  WULK_LEXER_EVENT_ON_PUSH = 0
};

struct wulk_lexer_event_t {
  enum wulk_lexer_event_n kind;
  wulk_lexer_t *lexer;
  void *data;

  bool (*callback)(wulk_lexer_event_t *self, void *arg);
  void (*dtor)(wulk_lexer_event_t *self);
};

struct wulk_lexer_t {
  struct wulk_compiler_t *compiler;
  char *buffer;
  size_t length;
  wulk_loc_t loc;
  wulk_token_r queue;
  unsigned char cap;
  wulk_lexer_event_r events;
  wulk_lexer_t *parent;
  wulk_lexer_r childs;

  void (*enqueue)(wulk_lexer_t *self, unsigned n);
};

void wulk_lexer_init(wulk_lexer_t *self, struct wulk_compiler_t *compiler, uint32_t file_id, const char *buffer, size_t length);
void wulk_lexer_init_f(wulk_lexer_t *self, struct wulk_parser_t *fe);
void wulk_lexer_dtor(wulk_lexer_t *self, bool free_all);
void wulk_lexer_fork(wulk_lexer_t *destination, wulk_lexer_t *source);
void wulk_lexer_join(wulk_lexer_t *fork);
void wulk_lexer_skip(wulk_lexer_t *self, unsigned n);

size_t wulk_lexer_length(wulk_lexer_t *self);
bool wulk_lexer_push(wulk_lexer_t *self, wulk_token_t token);
void wulk_lexer_attach(wulk_lexer_t *self, wulk_lexer_event_t event);
bool wulk_lexer_is_root(wulk_lexer_t *self);

wulk_token_t wulk_lexer_peek(wulk_lexer_t *self);
wulk_token_t wulk_lexer_peekn(wulk_lexer_t *self, unsigned n);
wulk_token_t wulk_lexer_next(wulk_lexer_t *self);
wulk_token_t wulk_lexer_consume(wulk_lexer_t *self, unsigned char type);
wulk_token_t wulk_lexer_consume_id(wulk_lexer_t *self, const char *id);
void wulk_lexer_undo(wulk_lexer_t *lexer, wulk_token_t until);
void wulk_lexer_undon(wulk_lexer_t *lexer, unsigned n);

#endif /* WULK_LEXER_H__ */
