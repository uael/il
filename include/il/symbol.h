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

#ifndef   IL_SYMBOL_H__
# define  IL_SYMBOL_H__

#include <adt/hash.h>
#include <adt/vector.h>

#include "entity.h"

typedef struct il_sym_t il_sym_t;
typedef struct il_scope_t il_scope_t;
typedef adt_vector_of(il_scope_t *) il_scope_r;

KHASH_DECLARE(il_symtab, const char *, il_sym_t)

struct il_sym_t {
  const char *id;
  unsigned flags;
  il_scope_t *scope;
  il_entity_t entity;
};

struct il_scope_t {
  il_scope_t *parent;
  il_sym_t *sym;
  il_symtab_t symtab;
  il_scope_r childs;
};

void il_sym_dtor(il_sym_t *self);
void il_scope_dtor(il_scope_t *self);
bool il_sym_has_flag(il_sym_t *self, unsigned flag);
il_sym_t *il_sym_put(il_scope_t *scope, const char *id, int *r);
il_sym_t *il_sym_get(il_scope_t *scope, const char *id, int *r);

#endif /* IL_SYMBOL_H__ */
