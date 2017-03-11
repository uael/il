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

#ifndef   JL_LEXER_H__
# define  JL_LEXER_H__

#include "fe.h"
#include "token.h"
#include "entity.h"

typedef struct jl_lexer_t jl_lexer_t;

struct jl_lexer_t {
  jl_frontend_t *fe;
  char *buffer;
  size_t length;
  jl_loc_t loc;
  jl_token_r token_stack;

  void (*stack)(jl_lexer_t *self, unsigned n);
  jl_token_t (*peek)(jl_lexer_t *self);
  jl_token_t (*peekn)(jl_lexer_t *self, unsigned n);
  jl_token_t (*next)(jl_lexer_t *self);
  jl_token_t (*consume)(jl_lexer_t *self, unsigned char type);
};

void jl_lexer_init(jl_lexer_t *self, jl_frontend_t *fe, uint32_t file_id, char *buffer, size_t length);
void jl_lexer_dtor(jl_lexer_t *self);

#endif /* JL_LEXER_H__ */