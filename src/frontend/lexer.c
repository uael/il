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

#include <adt/xmalloc.h>
#include <stdlib.h>

#include "lexer.h"
#include "jay/jay_lexer.h"
#include "c/c_lexer.h"
#include "c/c_pp.h"

void jl_lexer_init(jl_lexer_t *self, jl_frontend_t *fe, uint32_t file_id, char *buffer, size_t length) {
  *self = (jl_lexer_t) {
    .fe = fe,
    .loc = (jl_loc_t) {
      .colno = 0,
      .lineno = 0,
      .file_id = file_id,
      .position = 0
    },
    .buffer = xstrndup(buffer, length),
    .length = length
  };

  self->buffer[length] = '\0';
  switch (fe->kind) {
    case JL_FRONTEND_C:
      c_lexer_init(self);
      break;
    case JL_FRONTEND_JAY:
      self->queue_until = jay_lexer_stack;
      break;
  }
}

void jl_lexer_dup(jl_lexer_t *self, jl_lexer_t *begin) {
  size_t n_size = begin->length-begin->loc.position;
  *self = (jl_lexer_t) {
    .fe = begin->fe,
    .loc = (jl_loc_t) {
      .colno = 0,
      .lineno = 0,
      .file_id = begin->loc.file_id,
      .position = 0
    },
    .buffer = xstrndup(begin->buffer+begin->loc.position, n_size),
    .length = n_size,
    .queue_until = begin->queue_until
  };
  self->buffer[n_size] = '\0';
}

void jl_lexer_dtor(jl_lexer_t *self) {
  free(self->buffer);
  jl_token_t token;
  self->queue.cursor = 0;
  jl_deque_foreach(self->queue, token) {
    jl_token_dtor(&token);
  }
  jl_deque_dtor(self->queue);
}

size_t jl_lexer_length(jl_lexer_t *self) {
  return jl_deque_length(self->queue);
}

bool jl_lexer_push(jl_lexer_t *self, jl_token_t token) {
  jl_lexer_event_t event;
  jl_vector_foreach(self->events, event) {
    if (event.kind == JL_LEXER_EVENT_ON_PUSH) {
      if (!event.callback(&event, self, &token)) {
        return false;
      }
    }
  }
  jl_deque_push(self->queue, token);
  return true;
}

void jl_lexer_attach(jl_lexer_t *self, jl_lexer_event_t event) {
  jl_vector_push(self->events, event);
}

void jl_lexer_queue_until(jl_lexer_t *self, unsigned n) {
  if (self->loc.position < self->length) {
    self->queue_until(self, n);
  }
}

jl_token_t jl_lexer_peek(jl_lexer_t *self) {
  if (jl_lexer_length(self) < 1) {
    jl_lexer_queue_until(self, 10);
  }
  return jl_deque_front(self->queue);
}

jl_token_t jl_lexer_peekn(jl_lexer_t *self, unsigned n) {
  if (jl_lexer_length(self) < n) {
    jl_lexer_queue_until(self, n < 10 ? 10 : n);
  }
  return jl_deque_at(
    self->queue,
    jl_lexer_length(self) < n
      ? jl_lexer_length(self)
      : n
  );
}

jl_token_t jl_lexer_next(jl_lexer_t *self) {
  if (jl_lexer_length(self)) {
    (void) jl_deque_shift(self->queue);
  }
  if (jl_lexer_length(self) < 1) {
    jl_lexer_queue_until(self, 10);
  }
  return jl_deque_front(self->queue);
}

jl_token_t jl_lexer_consume(jl_lexer_t *self, unsigned char type) {
  if (jl_lexer_peek(self).type != type) {
    puts("unexpected token");
    exit(1);
  }
  return jl_lexer_next(self);
}
