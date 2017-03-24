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

#include "fe.h"

#include "compiler.h"
#include "lexer.h"
#include "symbol.h"
#include "program.h"

#include "c/c_fe.h"

void jl_fe_init(jl_fe_t *self, enum jl_fe_n kind, jl_compiler_t *compiler) {
  *self = (jl_fe_t) {
    .compiler = compiler,
    .kind = kind,
    .scope = xmalloc(sizeof(jl_scope_t))
  };

  switch (self->kind) {
    case JL_FRONTEND_C:
      self->parse = c_fe_parse;
      break;
    case JL_FRONTEND_JAY:
      break;
  }
}

void jl_fe_dtor(jl_fe_t *self) {
  jl_scope_t *parent;
  jl_deque_dtor(self->sources);
  while (self->scope) {
    parent = self->scope->parent;
    free(self->scope);
    self->scope = parent;
  }
}

void jl_fe_push_src(jl_fe_t *self, const char *src) {
  jl_deque_push(self->sources, src);
}

void jl_fe_scope(jl_fe_t *self, const char *id) {
  jl_scope_t *parent;
  jl_sym_t *sym;

  sym = id ? jl_sym_get(self->scope, id) : NULL;
  parent = self->scope;
  self->scope = xmalloc(sizeof(jl_scope_t));
  self->scope->sym = sym;
  self->scope->sym->scope = self->scope;
  self->scope->parent = parent;
}

void jl_fe_unscope(jl_fe_t *self) {
  self->scope = self->scope->parent;
}
