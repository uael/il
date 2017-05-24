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

#include "symbol.h"

void wulk_sym_dtor(wulk_sym_t *self) {}

void wulk_scope_dtor(wulk_scope_t *self) {
  wulk_sym_t sym;
  wulk_scope_t *scope;

  kh_foreach_value(&self->symtab, sym, {
    wulk_sym_dtor(&sym);
  });
  wulk_symtab_dtor(&self->symtab);
  adt_vector_foreach(self->childs, scope) {
    wulk_scope_dtor(scope);
    xfree(scope);
    scope = NULL;
  }
  adt_vector_dtor(self->childs);
}

bool wulk_sym_has_flag(wulk_sym_t *self, unsigned flag) {
  return (bool) (self->flags & flag);
}

wulk_sym_t *wulk_sym_put(wulk_scope_t *scope, const char *id, int *r) {
  wulk_sym_t *sym;
  unsigned it;

  it = kh_put(wulk_symtab, &scope->symtab, id, r);
  if (r == 0) {
    return NULL;
  }
  sym = &kh_value(&scope->symtab, it);
  sym->id = id;
  return sym;
}

wulk_sym_t *wulk_sym_get(wulk_scope_t *scope, const char *id, int *r) {
  unsigned it;

  it = kh_get(wulk_symtab, &scope->symtab, id);
  if (it == kh_end(&scope->symtab)) {
    *r = 1;
    return NULL;
  }
  *r = 0;
  return &kh_value(&scope->symtab, it);
}

KHASH_MAP_IMPL_STR(wulk_symtab, wulk_sym_t)
