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
