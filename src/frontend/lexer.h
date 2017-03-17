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

#ifndef   JL_LEXER_H__
# define  JL_LEXER_H__

#include <adt/deque.h>

#include "fe.h"
#include "token.h"
#include "entity_t.h"

typedef enum jl_lexer_event_n jl_lexer_event_n;

typedef struct jl_lexer_t jl_lexer_t;
typedef struct jl_lexer_event_t jl_lexer_event_t;

typedef jl_vector_of(jl_lexer_event_t) jl_lexer_event_r;

enum jl_lexer_event_n {
  JL_LEXER_EVENT_ON_PUSH = 0
};

struct jl_lexer_event_t {
  jl_lexer_event_n kind;
  jl_lexer_t *lexer;
  void *data;

  bool (*callback)(jl_lexer_event_t *self, void *arg);
  void (*dtor)(jl_lexer_event_t *self);
};

struct jl_lexer_t {
  jl_frontend_t *fe;
  char *buffer;
  size_t length;
  jl_loc_t loc;
  jl_token_r queue;
  unsigned char cap;
  jl_lexer_event_r events;
  jl_lexer_t *parent;

  void (*enqueue)(jl_lexer_t *self, unsigned n);
};

void jl_lexer_init(jl_lexer_t *self, jl_frontend_t *fe, uint32_t file_id, const char *buffer, size_t length);
void jl_lexer_init_f(jl_lexer_t *self, jl_frontend_t *fe);
void jl_lexer_dtor(jl_lexer_t *self);
void jl_lexer_fork(jl_lexer_t *destination, jl_lexer_t *source);
void jl_lexer_join(jl_lexer_t *fork);
void jl_lexer_skip(jl_lexer_t *self, unsigned n);

size_t jl_lexer_length(jl_lexer_t *self);
bool jl_lexer_push(jl_lexer_t *self, jl_token_t token);
void jl_lexer_attach(jl_lexer_t *self, jl_lexer_event_t event);
bool jl_lexer_is_root(jl_lexer_t *self);

jl_token_t jl_lexer_peek(jl_lexer_t *self);
jl_token_t jl_lexer_peekn(jl_lexer_t *self, unsigned n);
jl_token_t jl_lexer_next(jl_lexer_t *self);
jl_token_t jl_lexer_consume(jl_lexer_t *self, unsigned char type);
jl_token_t jl_lexer_consume_id(jl_lexer_t *self, const char *id);

#endif /* JL_LEXER_H__ */