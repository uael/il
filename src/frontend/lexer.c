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

#include <stdlib.h>
#include <token.h>

#include <adt/string.h>
#include <util/io.h>
#include <compiler.h>

#include "lexer.h"

#include "jay/jay_lexer.h"
#include "c/c_lexer.h"

void jl_lexer_init(jl_lexer_t *self, jl_fe_t *fe, uint32_t file_id, const char *buffer, size_t length) {
  *self = (jl_lexer_t) {
    .fe = fe,
    .loc = (jl_loc_t) {
      .file_id = file_id,
    },
    .buffer = buffer ? xstrndup(buffer, length) : NULL,
    .length = length,
    .cap = 10
  };

  if (self->buffer) {
    self->buffer[length] = '\0';
  }
  switch (fe->kind) {
    case JL_FRONTEND_C:
      c_lexer_init(self);
      break;
    case JL_FRONTEND_JAY:
      self->enqueue = jay_lexer_stack;
      break;
  }
}

void jl_lexer_init_f(jl_lexer_t *self, jl_fe_t *fe) {
  size_t len;
  uint32_t file_id;
  const char *filename, *buffer;

  if (!jl_deque_length(fe->sources)) {
    puts("no sources files to parse");
    exit(1);
  }

  file_id = (uint32_t) jl_deque_cursor(fe->sources);
  filename = jl_deque_shift(fe->sources);
  if (!jl_fexists(filename)) {
    puts("file does not exists");
    exit(1);
  }
  buffer = jl_fread(filename, &len);
  if (!buffer) {
    puts("unable to read source file");
    exit(1);
  }

  jl_lexer_init(self,  fe, file_id, buffer, len);
}

void jl_lexer_dtor(jl_lexer_t *self) {
  if (self->buffer) {
    free(self->buffer);
  }
  jl_token_t token;
  jl_lexer_event_t event;
  self->queue.cursor = 0;
  jl_deque_foreach(self->queue, token) {
    jl_token_dtor(&token);
  }
  jl_deque_dtor(self->queue);
  jl_vector_foreach(self->events, event) {
    if (event.dtor) {
      event.dtor(&event);
    }
  }
  jl_vector_dtor(self->events);
}

void jl_lexer_fork(jl_lexer_t *destination, jl_lexer_t *source) {
  destination->parent = source;
  destination->fe = source->fe;
  destination->loc = source->loc;
  destination->buffer = source->buffer;
  destination->length = source->length;
  destination->enqueue = source->enqueue;
  destination->cap = 1;
}

void jl_lexer_join(jl_lexer_t *fork) {
  if (jl_deque_size(fork->queue)) {
    fork->parent->loc = fork->loc;
  }
  jl_deque_clear(fork->queue);
}

void jl_lexer_skip(jl_lexer_t *self, unsigned n) {
  self->loc.position += n;
}

size_t jl_lexer_length(jl_lexer_t *self) {
  return jl_deque_length(self->queue);
}

bool jl_lexer_push(jl_lexer_t *self, jl_token_t token) {
  jl_lexer_event_t event;
  jl_vector_foreach(self->events, event) {
    if (event.kind == JL_LEXER_EVENT_ON_PUSH) {
      if (!event.callback(&event, &token)) {
        jl_token_dtor(&token);
        return false;
      }
    }
  }
  token.cursor = jl_deque_size(self->queue);
  jl_deque_push(self->queue, token);
  if (jl_lexer_is_root(self) && self->fe->compiler->opts.echo) {
    switch (token.kind) {
      case JL_TOKEN_IDENTIFIER:
      case JL_TOKEN_NUMBER:
        printf("%s ", token.u.s);
        break;
      case JL_TOKEN_FLOAT:
        printf("%f ", token.u.f);
        break;
      case JL_TOKEN_INT:
        printf("%d ", token.u.i);
        break;
      default:
        if (token.length>1) {
          printf("%s ", token.name);
        }
        else {
          printf("%s", token.name);
        }
        break;
    }
  }
  return true;
}

void jl_lexer_attach(jl_lexer_t *self, jl_lexer_event_t event) {
  event.lexer = self;
  jl_vector_push(self->events, event);
}

bool jl_lexer_is_root(jl_lexer_t *self) {
  return self->parent == NULL;
}

void jl_lexer_enqueue(jl_lexer_t *self, unsigned n) {
  if (self->loc.position < self->length) {
    self->enqueue(self, n);
  } else if (!jl_lexer_length(self)) {
    jl_token_t token = {'\0'};
    jl_lexer_push(self, token);
  }
}

jl_token_t jl_lexer_peek(jl_lexer_t *self) {
  if (jl_lexer_length(self) < 1) {
    jl_lexer_enqueue(self, self->cap);
  }
  return jl_deque_front(self->queue);
}

jl_token_t jl_lexer_peekn(jl_lexer_t *self, unsigned n) {
  n++;
  if (jl_lexer_length(self) <= n) {
    jl_lexer_enqueue(self, n < self->cap ? self->cap : n);
  }
  return jl_deque_at(
    self->queue,
    (jl_lexer_length(self) < n ? jl_lexer_length(self) : n)-1
  );
}

jl_token_t jl_lexer_next(jl_lexer_t *self) {
  jl_token_t result;

  if (jl_lexer_length(self) < 1) {
    jl_lexer_enqueue(self, self->cap);
  }
  if (jl_lexer_length(self) && jl_deque_front(self->queue).type != 0) {
    result = jl_deque_shift(self->queue);
  } else {
    result = jl_deque_front(self->queue);
  }
  return result;
}

jl_token_t jl_lexer_consume(jl_lexer_t *self, unsigned char type) {
  jl_token_t result;

  if (type != '\n') {
    while (jl_lexer_peek(self).type == '\n') {
      jl_lexer_next(self);
    }
  }

  if ((result = jl_lexer_peek(self)).type != type) {
    puts("unexpected token");
    exit(1);
  }
  jl_lexer_next(self);
  return result;
}

jl_token_t jl_lexer_consume_id(jl_lexer_t *self, const char *id) {
  jl_token_t result;
  if ((result = jl_lexer_peek(self)).kind != JL_TOKEN_IDENTIFIER) {
    puts("unexpected token");
    exit(1);
  }
  if (strcmp(id, result.u.s) != 0) {
    puts("unexpected identifier");
    exit(1);
  }
  jl_lexer_next(self);
  return result;
}

void jl_lexer_undo(jl_lexer_t *lexer, jl_token_t until) {
  while (jl_deque_cursor(lexer->queue) > until.cursor) --jl_deque_cursor(lexer->queue);
}

void jl_lexer_undon(jl_lexer_t *lexer, unsigned n) {
  jl_deque_cursor(lexer->queue) -= n;
}
