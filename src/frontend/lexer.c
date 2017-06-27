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

#include <stdlib.h>
#include <stdio.h>

#include "il/adt/string.h"

#include "il/lexer.h"

#include "util/io.h"
#include "il/compiler.h"
#include "c/c_lexer.h"

void il_lexer_init(il_lexer_t *self, il_compiler_t *compiler, uint32_t file_id, const char *buffer, size_t length) {
  *self = (il_lexer_t) {
    .compiler = compiler,
    .loc = (il_loc_t) {
      .file_id = file_id
    },
    .buffer = buffer ? xstrndup(buffer, length) : NULL,
    .length = length,
    .cap = 10
  };

  if (self->buffer) {
    self->buffer[length] = '\0';
  }
  switch (compiler->fe.kind) {
    case IL_PARSER_C:
      c_lexer_init(self);
      break;
    case IL_PARSER_JAY:
      break;
    default:
      break;
  }
}

void il_lexer_init_f(il_lexer_t *self, il_parser_t *fe) {
  size_t len;
  uint32_t file_id;
  const char *filename, *buffer;

  if (!adt_deque_length(fe->sources)) {
    il_fatal_err(fe->compiler, "No input files");
  }

  file_id = (uint32_t) adt_deque_cursor(fe->sources);
  filename = adt_deque_shift(fe->sources);
  if (!il_fexists(filename)) {
    il_fatal_err(fe->compiler, "Input file does not exists '%s'", filename);
  }
  buffer = il_fread(filename, &len);
  if (!buffer) {
    il_fatal_err(fe->compiler, "Unable to read input file '%s'", filename);
  }

  il_lexer_init(self,  fe->compiler, file_id, buffer, len);
  xfree((char *) buffer);
}

void il_lexer_dtor(il_lexer_t *self, bool free_all) {
  il_token_t token;
  il_lexer_event_t event;
  il_lexer_t child;

  if (self->buffer && il_lexer_is_root(self)) {
    xfree(self->buffer);
  }
  self->queue.cursor = 0;
  adt_deque_foreach(self->queue, token) {
    il_token_dtor(&token);
  }
  adt_vector_foreach(self->events, event) {
    if (event.dtor) {
      event.dtor(&event);
    }
  }
  adt_vector_foreach(self->childs, child) {
    il_lexer_dtor(&child, true);
  }
  if (free_all) {
    adt_vector_dtor(self->childs);
    adt_deque_dtor(self->queue);
    adt_vector_dtor(self->events);
  } else {
    adt_vector_clear(self->childs);
    adt_deque_clear(self->queue);
    adt_vector_clear(self->events);
  }
}

void il_lexer_fork(il_lexer_t *destination, il_lexer_t *source) {
  destination->parent = source;
  destination->compiler = source->compiler;
  destination->loc = source->loc;
  destination->buffer = source->buffer;
  destination->length = source->length;
  destination->enqueue = source->enqueue;
  destination->cap = 1;
}

void il_lexer_join(il_lexer_t *fork) {
  if (adt_deque_size(fork->queue)) {
    fork->parent->loc = fork->loc;
  }
  adt_deque_clear(fork->queue);
}

void il_lexer_skip(il_lexer_t *self, unsigned n) {
  self->loc.position += n;
}

size_t il_lexer_length(il_lexer_t *self) {
  return adt_deque_length(self->queue);
}

bool il_lexer_push(il_lexer_t *self, il_token_t token) {
  il_lexer_event_t event;
  unsigned i;

  adt_vector_foreach(self->events, event) {
    if (event.kind == IL_LEXER_EVENT_ON_PUSH) {
      if (!event.callback(&event, &token)) {
        il_token_dtor(&token);
        return false;
      }
    }
  }
  token.cursor = adt_deque_size(self->queue);
  adt_deque_push(self->queue, token);
  if (il_lexer_is_root(self) && self->compiler->opts.echo) {
    for (i = 0; i<token.leading_ws; ++i) {
      putc(' ', stdout);
    }
    switch (token.kind) {
      case IL_TOKEN_IDENTIFIER:
      case IL_TOKEN_NUMBER:
        fputs(token.value, stdout);
        break;
      default:
        fputs(token.name, stdout);
        break;
    }
  }
  return true;
}

void il_lexer_attach(il_lexer_t *self, il_lexer_event_t event) {
  event.lexer = self;
  adt_vector_push(self->events, event);
}

bool il_lexer_is_root(il_lexer_t *self) {
  return self->parent == NULL;
}

void il_lexer_next_src(il_lexer_t *self, il_parser_t *fe) {
  il_lexer_dtor(self, false);
  il_lexer_init_f(self, fe);
  self->compiler = fe->compiler;
}

void il_lexer_enqueue(il_lexer_t *self, unsigned n) {
  if (adt_vector_size(self->childs)) {
    il_lexer_enqueue(&adt_vector_back(self->childs), n);
  } else if (self->loc.position < self->length) {
    self->enqueue(self, n);
  } else if (!il_lexer_is_root(self)) {
    (void) adt_vector_pop(self->parent->childs);
  } else if (!il_lexer_length(self)) {
    if (adt_deque_length(self->compiler->fe.sources)) {
      il_lexer_next_src(self, &self->compiler->fe);
    } else {
      il_token_t token = {'\0'};
      il_lexer_push(self, token);
    }
  }
}

il_token_t il_lexer_peek(il_lexer_t *self) {
  if (il_lexer_length(self) < 1) {
    il_lexer_enqueue(self, self->cap);
  }
  return adt_deque_front(self->queue);
}

il_token_t il_lexer_peekn(il_lexer_t *self, unsigned n) {
  n++;
  if (il_lexer_length(self) <= n) {
    il_lexer_enqueue(self, n < self->cap ? self->cap : n);
  }
  return adt_deque_at(
    self->queue,
    (il_lexer_length(self) < n ? il_lexer_length(self) : n)-1
  );
}

il_token_t il_lexer_next(il_lexer_t *self) {
  il_token_t result;

  if (il_lexer_length(self) < 1) {
    il_lexer_enqueue(self, self->cap);
  }
  if (il_lexer_length(self) && adt_deque_front(self->queue).type != 0) {
    result = adt_deque_shift(self->queue);
  } else {
    result = adt_deque_front(self->queue);
  }
  return result;
}

il_token_t il_lexer_consume(il_lexer_t *self, unsigned char type) {
  il_token_t result;

  if (type != '\n') {
    while (il_lexer_peek(self).type == '\n') {
      il_lexer_next(self);
    }
  }

  if ((result = il_lexer_peek(self)).type != type) {
    il_parse_err(self->compiler, result.loc,
      "Unexpected token '" BOLD "%c" RESET "' expected '" BOLD "%c" "'" RESET,
      result.type,
      type
    );
  }
  il_lexer_next(self);
  return result;
}

il_token_t il_lexer_consume_id(il_lexer_t *self, const char *id) {
  il_token_t result;
  if ((result = il_lexer_peek(self)).kind != IL_TOKEN_IDENTIFIER) {
    il_parse_err(self->compiler, result.loc,
      "Unexpected token '" BOLD "%c" RESET "' expected identifier '" BOLD "%s" "'" RESET,
      result.type,
      id
    );
  }
  if (id && strcmp(id, result.value) != 0) {
    il_parse_err(self->compiler, result.loc,
      "Unexpected identifier '" BOLD "%s" RESET "' expected '" BOLD "%s" "'" RESET,
      result.value,
      id
    );
  }
  il_lexer_next(self);
  return result;
}

void il_lexer_undo(il_lexer_t *lexer, il_token_t until) {
  while (adt_deque_cursor(lexer->queue) > until.cursor) --adt_deque_cursor(lexer->queue);
}

void il_lexer_undon(il_lexer_t *lexer, unsigned n) {
  adt_deque_cursor(lexer->queue) -= n;
}
