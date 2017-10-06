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

#include <stdio.h>

#include "il/symbol.h"

void il_sym_dtor(UNUSED il_sym_t *self) {}

void il_scope_dtor(il_scope_t *self) {
  il_sym_t sym;
  il_scope_t *scope;

  kh_foreach_value(&self->symtab, sym, {
    il_sym_dtor(&sym);
  });
  il_symtab_dtor(&self->symtab);
  adt_vector_foreach(self->childs, scope) {
    il_scope_dtor(scope);
    xfree(scope);
    scope = NULL;
  }
  adt_vector_dtor(self->childs);
}

bool il_sym_has_flag(il_sym_t *self, unsigned flag) {
  return (bool) (self->flags & flag);
}

il_sym_t *il_sym_put(il_scope_t *scope, __const char *id, int *r) {
  il_sym_t *sym;
  unsigned it;

  it = kh_put(il_symtab, &scope->symtab, id, r);
  if (r == 0) {
    return NULL;
  }
  sym = &kh_value(&scope->symtab, it);
  sym->id = id;
  return sym;
}

il_sym_t *il_sym_get(il_scope_t *scope, __const char *id, int *r) {
  unsigned it;

  it = kh_get(il_symtab, &scope->symtab, id);
  if (it == kh_end(&scope->symtab)) {
    *r = 1;
    return NULL;
  }
  *r = 0;
  return &kh_value(&scope->symtab, it);
}

KHASH_MAP_IMPL_STR(il_symtab, il_sym_t)
