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
      self->stack = c_lexer_stack;
      self->peek = jl_lexer_peek;
      self->peekn = jl_lexer_peekn;
      self->next = jl_lexer_next;
      self->consume = jl_lexer_consume;
      break;
    case JL_FRONTEND_JAY:
      self->stack = jay_lexer_stack;
      self->peek = jl_lexer_peek;
      self->peekn = jl_lexer_peekn;
      self->next = jl_lexer_next;
      self->consume = jl_lexer_consume;
      break;
  }
}

jl_token_t jl_lexer_peek(jl_lexer_t *self) {
  if (jl_vector_size(self->token_stack) < 1) {
    self->stack(self, 1);
  }
  return jl_vector_front(self->token_stack);
}

jl_token_t jl_lexer_peekn(jl_lexer_t *self, unsigned n) {
  if (jl_vector_size(self->token_stack) < n) {
    self->stack(self, (unsigned) (n - jl_vector_size(self->token_stack)));
  }
  return jl_vector_at(
    self->token_stack,
    jl_vector_size(self->token_stack) < n ? jl_vector_size(self->token_stack) : n
  );
}

jl_token_t jl_lexer_next(jl_lexer_t *self) {
  if (jl_vector_size(self->token_stack)) {
    jl_token_dtor(&jl_vector_front(self->token_stack));
    jl_vector_shift(self->token_stack);
  }
  if (jl_vector_size(self->token_stack) < 1) {
    self->stack(self, 1);
  }
  return jl_vector_front(self->token_stack);
}

jl_token_t jl_lexer_consume(jl_lexer_t *self, unsigned char type) {
  if (self->peek(self).type != type) {
    puts("unexpected token");
    exit(1);
  }
  return self->next(self);
}

void jl_lexer_dtor(jl_lexer_t *self) {
  free(self->buffer);
  jl_token_t token;
  jl_vector_foreach(self->token_stack, token) {
    jl_token_dtor(&token);
  }
  jl_vector_dtor(self->token_stack);
}
