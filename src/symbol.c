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

#include "symbol.h"

bool jl_sym_has_flag(jl_sym_t *self, unsigned flag) {
  return (bool) (self->flags & flag);
}

jl_sym_t *jl_sym_put(jl_symtab_t *symtab, const char *id) {
  jl_sym_t *sym;
  unsigned it;
  int r;

  it = kh_put(jl_symtab, symtab, id, &r);
  if (r == 0) {
    return NULL;
  }
  sym = &kh_value(symtab, it);
  sym->id = id;
  return sym;
}

jl_sym_t *jl_sym_get(jl_symtab_t *symtab, const char *id) {
  unsigned it;

  it = kh_get(jl_symtab, symtab, id);
  if (it == kh_end(symtab)) {
    return NULL;
  }
  return &kh_value(symtab, it);
}

KHASH_MAP_IMPL_STR(jl_symtab, jl_sym_t)
