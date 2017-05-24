/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
