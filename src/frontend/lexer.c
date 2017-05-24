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

#include <stdlib.h>
#include <stdio.h>

#include "adt/string.h"

#include "lexer.h"

#include "util/io.h"
#include "compiler.h"
#include "c/c_lexer.h"

void wulk_lexer_init(wulk_lexer_t *self, wulk_compiler_t *compiler, uint32_t file_id, const char *buffer, size_t length) {
  *self = (wulk_lexer_t) {
    .compiler = compiler,
    .loc = (wulk_loc_t) {
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
    case WULK_PARSER_C:
      c_lexer_init(self);
      break;
    case WULK_PARSER_JAY:
      break;
    default:
      break;
  }
}

void wulk_lexer_init_f(wulk_lexer_t *self, wulk_parser_t *fe) {
  size_t len;
  uint32_t file_id;
  const char *filename, *buffer;

  if (!adt_deque_length(fe->sources)) {
    wulk_fatal_err(fe->compiler, "No input files");
  }

  file_id = (uint32_t) adt_deque_cursor(fe->sources);
  filename = adt_deque_shift(fe->sources);
  if (!wulk_fexists(filename)) {
    wulk_fatal_err(fe->compiler, "Input file does not exists '%s'", filename);
  }
  buffer = wulk_fread(filename, &len);
  if (!buffer) {
    wulk_fatal_err(fe->compiler, "Unable to read input file '%s'", filename);
  }

  wulk_lexer_init(self,  fe->compiler, file_id, buffer, len);
  xfree((char *) buffer);
}

void wulk_lexer_dtor(wulk_lexer_t *self, bool free_all) {
  wulk_token_t token;
  wulk_lexer_event_t event;
  wulk_lexer_t child;

  if (self->buffer && wulk_lexer_is_root(self)) {
    xfree(self->buffer);
  }
  self->queue.cursor = 0;
  adt_deque_foreach(self->queue, token) {
    wulk_token_dtor(&token);
  }
  adt_vector_foreach(self->events, event) {
    if (event.dtor) {
      event.dtor(&event);
    }
  }
  adt_vector_foreach(self->childs, child) {
    wulk_lexer_dtor(&child, true);
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

void wulk_lexer_fork(wulk_lexer_t *destination, wulk_lexer_t *source) {
  destination->parent = source;
  destination->compiler = source->compiler;
  destination->loc = source->loc;
  destination->buffer = source->buffer;
  destination->length = source->length;
  destination->enqueue = source->enqueue;
  destination->cap = 1;
}

void wulk_lexer_join(wulk_lexer_t *fork) {
  if (adt_deque_size(fork->queue)) {
    fork->parent->loc = fork->loc;
  }
  adt_deque_clear(fork->queue);
}

void wulk_lexer_skip(wulk_lexer_t *self, unsigned n) {
  self->loc.position += n;
}

size_t wulk_lexer_length(wulk_lexer_t *self) {
  return adt_deque_length(self->queue);
}

bool wulk_lexer_push(wulk_lexer_t *self, wulk_token_t token) {
  wulk_lexer_event_t event;
  unsigned i;

  adt_vector_foreach(self->events, event) {
    if (event.kind == WULK_LEXER_EVENT_ON_PUSH) {
      if (!event.callback(&event, &token)) {
        wulk_token_dtor(&token);
        return false;
      }
    }
  }
  token.cursor = adt_deque_size(self->queue);
  adt_deque_push(self->queue, token);
  if (wulk_lexer_is_root(self) && self->compiler->opts.echo) {
    for (i = 0; i<token.leading_ws; ++i) {
      putc(' ', stdout);
    }
    switch (token.kind) {
      case WULK_TOKEN_IDENTIFIER:
      case WULK_TOKEN_NUMBER:
        fputs(token.value, stdout);
        break;
      default:
        fputs(token.name, stdout);
        break;
    }
  }
  return true;
}

void wulk_lexer_attach(wulk_lexer_t *self, wulk_lexer_event_t event) {
  event.lexer = self;
  adt_vector_push(self->events, event);
}

bool wulk_lexer_is_root(wulk_lexer_t *self) {
  return self->parent == NULL;
}

void wulk_lexer_next_src(wulk_lexer_t *self, wulk_parser_t *fe) {
  wulk_lexer_dtor(self, false);
  wulk_lexer_init_f(self, fe);
  self->compiler = fe->compiler;
}

void wulk_lexer_enqueue(wulk_lexer_t *self, unsigned n) {
  if (adt_vector_size(self->childs)) {
    wulk_lexer_enqueue(&adt_vector_back(self->childs), n);
  } else if (self->loc.position < self->length) {
    self->enqueue(self, n);
  } else if (!wulk_lexer_is_root(self)) {
    (void) adt_vector_pop(self->parent->childs);
  } else if (!wulk_lexer_length(self)) {
    if (adt_deque_length(self->compiler->fe.sources)) {
      wulk_lexer_next_src(self, &self->compiler->fe);
    } else {
      wulk_token_t token = {'\0'};
      wulk_lexer_push(self, token);
    }
  }
}

wulk_token_t wulk_lexer_peek(wulk_lexer_t *self) {
  if (wulk_lexer_length(self) < 1) {
    wulk_lexer_enqueue(self, self->cap);
  }
  return adt_deque_front(self->queue);
}

wulk_token_t wulk_lexer_peekn(wulk_lexer_t *self, unsigned n) {
  n++;
  if (wulk_lexer_length(self) <= n) {
    wulk_lexer_enqueue(self, n < self->cap ? self->cap : n);
  }
  return adt_deque_at(
    self->queue,
    (wulk_lexer_length(self) < n ? wulk_lexer_length(self) : n)-1
  );
}

wulk_token_t wulk_lexer_next(wulk_lexer_t *self) {
  wulk_token_t result;

  if (wulk_lexer_length(self) < 1) {
    wulk_lexer_enqueue(self, self->cap);
  }
  if (wulk_lexer_length(self) && adt_deque_front(self->queue).type != 0) {
    result = adt_deque_shift(self->queue);
  } else {
    result = adt_deque_front(self->queue);
  }
  return result;
}

wulk_token_t wulk_lexer_consume(wulk_lexer_t *self, unsigned char type) {
  wulk_token_t result;

  if (type != '\n') {
    while (wulk_lexer_peek(self).type == '\n') {
      wulk_lexer_next(self);
    }
  }

  if ((result = wulk_lexer_peek(self)).type != type) {
    wulk_parse_err(self->compiler, result.loc,
      "Unexpected token '" BOLD "%c" RESET "' expected '" BOLD "%c" "'" RESET,
      result.type,
      type
    );
  }
  wulk_lexer_next(self);
  return result;
}

wulk_token_t wulk_lexer_consume_id(wulk_lexer_t *self, const char *id) {
  wulk_token_t result;
  if ((result = wulk_lexer_peek(self)).kind != WULK_TOKEN_IDENTIFIER) {
    wulk_parse_err(self->compiler, result.loc,
      "Unexpected token '" BOLD "%c" RESET "' expected identifier '" BOLD "%s" "'" RESET,
      result.type,
      id
    );
  }
  if (id && strcmp(id, result.value) != 0) {
    wulk_parse_err(self->compiler, result.loc,
      "Unexpected identifier '" BOLD "%s" RESET "' expected '" BOLD "%s" "'" RESET,
      result.value,
      id
    );
  }
  wulk_lexer_next(self);
  return result;
}

void wulk_lexer_undo(wulk_lexer_t *lexer, wulk_token_t until) {
  while (adt_deque_cursor(lexer->queue) > until.cursor) --adt_deque_cursor(lexer->queue);
}

void wulk_lexer_undon(wulk_lexer_t *lexer, unsigned n) {
  adt_deque_cursor(lexer->queue) -= n;
}
