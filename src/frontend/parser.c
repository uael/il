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

void il_fe_init(il_parser_t *self, enum il_parser_n kind, il_compiler_t *compiler) {
  *self = (il_parser_t) {
    .compiler = compiler,
    .kind = kind,
    .scope = xmalloc(sizeof(il_scope_t))
  };
  *self->scope = (il_scope_t) {0};

  switch (self->kind) {
    case IL_PARSER_C:
      self->parse = c_parser_parse;
      break;
    case IL_PARSER_JAY:
      break;
    default:
      break;
  }
}

void il_fe_dtor(il_parser_t *self) {
  const char *src;

  adt_vector_foreach(self->sources, src) {
    xfree((void *) src);
    src = NULL;
  }
  adt_deque_dtor(self->sources);
  while (self->scope->parent) {
    self->scope = self->scope->parent;
  }
  il_scope_dtor(self->scope);
  xfree(self->scope);
  self->scope = NULL;
  if (self->lexer) {
    il_lexer_dtor(self->lexer, true);
    self->lexer = NULL;
  }
}

void il_fe_parse(il_parser_t *self, il_lexer_t *lexer, struct il_program_t *out) {
  il_lexer_t l = (il_lexer_t) {0};

  if (!lexer) {
    il_lexer_init_f(&l, self);
    self->lexer = &l;
  }
  il_program_init(out);
  self->parse(self, out);
  if (!lexer) {
    il_lexer_dtor(&l, true);
    self->lexer = NULL;
  }
}

void il_fe_push_src(il_parser_t *self, const char *src) {
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

void il_fe_scope(il_parser_t *self) {
  il_scope_t *parent;

  parent = self->scope;
  self->scope = xmalloc(sizeof(il_scope_t));
  *self->scope = (il_scope_t) {0};
  self->scope->parent = parent;
  adt_vector_push(parent->childs, self->scope);
}

void il_fe_unscope(il_parser_t *self) {
  self->scope = self->scope->parent;
}
