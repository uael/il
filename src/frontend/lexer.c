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

#include <string.h>

#include "lexer.h"
#include "jay/jay_lexer.h"

void jl_lexer_init(jl_lexer_t *self, jl_frontend_n kind, uint32_t file_id, char *buffer, size_t length) {
  *self = (jl_lexer_t) {
    .kind = kind,
    .loc = (jl_loc_t) {
      .colno = 0,
      .lineno = 0,
      .file_id = file_id,
      .position = 0
    },
    .buffer = strndup(buffer, length),
    .length = length
  };

  self->buffer[length] = '\0';
  switch (kind) {
    case JL_FRONTEND_C:
      break;
    case JL_FRONTEND_JAY:
      self->peek = jay_lexer_peek;
      self->peekn = jay_lexer_peekn;
      self->next = jay_lexer_next;
      self->consume = jay_lexer_consume;
      break;
  }
}

void jl_lexer_dtor(jl_lexer_t *self) {
  free(self->buffer);
}
