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

#include "token.h"

#include "lexer.h"


void jl_token_dtor(jl_token_t *self) {}

jl_lloc_t jl_lloc_begin(jl_lexer_t *lexer) {
  return (jl_lloc_t) {
    .lexer = lexer,
    .begin = jl_lexer_peek(lexer).cursor
  };
}

jl_lloc_t jl_lloc_end(jl_lloc_t self){
  self.end = jl_lexer_peek(self.lexer).cursor;
  return self;
}

jl_rtoken_t jl_llocate(jl_lloc_t lloc) {
  if (lloc.lexer) {
    return (jl_rtoken_t) {
      adt_vector_at(lloc.lexer->queue, lloc.begin),
      adt_vector_at(lloc.lexer->queue, lloc.end)
    };
  }
  return (jl_rtoken_t) {{0}, {0}};
}
