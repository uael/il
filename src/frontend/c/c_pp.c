/*
 * MIT License
 *
 * Copyright (c) 2016-2017 uael <www.github.com/uael>
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

#include <token.h>
#include <assert.h>
#include <ctype.h>
#include "c_pp.h"
#include "c_lexer.h"

bool c_pp_op_push_callback(jl_lexer_event_t *self, jl_lexer_t *lexer, void *arg) {
  jl_token_t *token;
  c_macro_ht_t *macros;

  if (!self->data) {
    self->data = c_macro_ht_new();
  }
  token = (jl_token_t *) arg;
  macros = (c_macro_ht_t *) self->data;


  return true;
}

void c_pp_op_push_dtor(jl_lexer_event_t *self) {
  if (self->data) {
    c_macro_ht_free(self->data);
  }
}

KHASH_MAP_IMPL_STR(c_macro_ht, c_macro_t);
