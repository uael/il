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

#ifndef   IL_LEXER_H__
# define  IL_LEXER_H__

#include <il/adt/vector.h>

#include "token.h"

struct il_compiler_t;
struct il_parser_t;

typedef struct il_lexer_t il_lexer_t;
typedef struct il_lexer_event_t il_lexer_event_t;

typedef adt_vector_of(il_lexer_t) il_lexer_r;
typedef adt_vector_of(il_lexer_event_t) il_lexer_event_r;

enum il_lexer_event_n {
  IL_LEXER_EVENT_ON_PUSH = 0
};

struct il_lexer_event_t {
  enum il_lexer_event_n kind;
  il_lexer_t *lexer;
  void *data;

  bool (*callback)(il_lexer_event_t *self, void *arg);
  void (*dtor)(il_lexer_event_t *self);
};

struct il_lexer_t {
  struct il_compiler_t *compiler;
  char *buffer;
  size_t length;
  il_loc_t loc;
  il_token_r queue;
  unsigned char cap;
  il_lexer_event_r events;
  il_lexer_t *parent;
  il_lexer_r childs;

  void (*enqueue)(il_lexer_t *self, unsigned n);
};

void il_lexer_init(il_lexer_t *self, struct il_compiler_t *compiler, uint32_t file_id, const char *buffer, size_t length);
void il_lexer_init_f(il_lexer_t *self, struct il_parser_t *fe);
void il_lexer_dtor(il_lexer_t *self, bool free_all);
void il_lexer_fork(il_lexer_t *destination, il_lexer_t *source);
void il_lexer_join(il_lexer_t *fork);
void il_lexer_skip(il_lexer_t *self, unsigned n);

size_t il_lexer_length(il_lexer_t *self);
bool il_lexer_push(il_lexer_t *self, il_token_t token);
void il_lexer_attach(il_lexer_t *self, il_lexer_event_t event);
bool il_lexer_is_root(il_lexer_t *self);

il_token_t il_lexer_peek(il_lexer_t *self);
il_token_t il_lexer_peekn(il_lexer_t *self, unsigned n);
il_token_t il_lexer_next(il_lexer_t *self);
il_token_t il_lexer_consume(il_lexer_t *self, unsigned char type);
il_token_t il_lexer_consume_id(il_lexer_t *self, const char *id);
void il_lexer_undo(il_lexer_t *lexer, il_token_t until);
void il_lexer_undon(il_lexer_t *lexer, unsigned n);

#endif /* IL_LEXER_H__ */
