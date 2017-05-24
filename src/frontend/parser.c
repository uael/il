/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
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
