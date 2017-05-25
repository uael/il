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

#ifndef   WULK_SYMBOL_H__
# define  WULK_SYMBOL_H__

#include <adt/hash.h>
#include <adt/vector.h>

#include "entity.h"

typedef struct wulk_sym_t wulk_sym_t;
typedef struct wulk_scope_t wulk_scope_t;
typedef adt_vector_of(wulk_scope_t *) wulk_scope_r;

KHASH_DECLARE(wulk_symtab, const char *, wulk_sym_t)

struct wulk_sym_t {
  const char *id;
  unsigned flags;
  wulk_scope_t *scope;
  wulk_entity_t entity;
};

struct wulk_scope_t {
  wulk_scope_t *parent;
  wulk_sym_t *sym;
  wulk_symtab_t symtab;
  wulk_scope_r childs;
};

void wulk_sym_dtor(wulk_sym_t *self);
void wulk_scope_dtor(wulk_scope_t *self);
bool wulk_sym_has_flag(wulk_sym_t *self, unsigned flag);
wulk_sym_t *wulk_sym_put(wulk_scope_t *scope, const char *id, int *r);
wulk_sym_t *wulk_sym_get(wulk_scope_t *scope, const char *id, int *r);

#endif /* WULK_SYMBOL_H__ */
