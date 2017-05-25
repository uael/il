/*
 * Wulk - Wu uniform language kit
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
