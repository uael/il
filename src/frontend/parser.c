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

#include <stdio.h>

#include "parser.h"

#include "compiler.h"
#include "symbol.h"
#include "program.h"

#include "c/c_parser.h"

void wulk_fe_init(wulk_parser_t *self, enum wulk_parser_n kind, wulk_compiler_t *compiler) {
  *self = (wulk_parser_t) {
    .compiler = compiler,
    .kind = kind,
    .scope = xmalloc(sizeof(wulk_scope_t))
  };
  *self->scope = (wulk_scope_t) {0};

  switch (self->kind) {
    case WULK_PARSER_C:
      self->parse = c_parser_parse;
      break;
    case WULK_PARSER_JAY:
      break;
    default:
      break;
  }
}

void wulk_fe_dtor(wulk_parser_t *self) {
  const char *src;

  adt_vector_foreach(self->sources, src) {
    xfree((void *) src);
    src = NULL;
  }
  adt_deque_dtor(self->sources);
  while (self->scope->parent) {
    self->scope = self->scope->parent;
  }
  wulk_scope_dtor(self->scope);
  xfree(self->scope);
  self->scope = NULL;
  if (self->lexer) {
    wulk_lexer_dtor(self->lexer, true);
    self->lexer = NULL;
  }
}

void wulk_fe_parse(wulk_parser_t *self, wulk_lexer_t *lexer, struct wulk_program_t *out) {
  wulk_lexer_t l = (wulk_lexer_t) {0};

  if (!lexer) {
    wulk_lexer_init_f(&l, self);
    self->lexer = &l;
  }
  wulk_program_init(out);
  self->parse(self, out);
  if (!lexer) {
    wulk_lexer_dtor(&l, true);
    self->lexer = NULL;
  }
}

void wulk_fe_push_src(wulk_parser_t *self, const char *src) {
  const char *file;

#ifdef _MSC_VER
  // todo
  file = xstrdup(src);
#else
  file = xmalloc(FILENAME_MAX + 1);
  realpath(src, (char *) file);
#endif
  adt_deque_push(self->sources, file);
}

void wulk_fe_scope(wulk_parser_t *self) {
  wulk_scope_t *parent;

  parent = self->scope;
  self->scope = xmalloc(sizeof(wulk_scope_t));
  *self->scope = (wulk_scope_t) {0};
  self->scope->parent = parent;
  adt_vector_push(parent->childs, self->scope);
}

void wulk_fe_unscope(wulk_parser_t *self) {
  self->scope = self->scope->parent;
}
