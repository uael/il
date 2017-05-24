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

#include "token.h"

#include "lexer.h"


void wulk_token_dtor(wulk_token_t *self) {}

wulk_lloc_t wulk_lloc_begin(wulk_lexer_t *lexer) {
  return (wulk_lloc_t) {
    .lexer = lexer,
    .begin = wulk_lexer_peek(lexer).cursor
  };
}

wulk_lloc_t wulk_lloc_end(wulk_lloc_t self){
  self.end = wulk_lexer_peek(self.lexer).cursor;
  return self;
}

wulk_rtoken_t wulk_llocate(wulk_lloc_t lloc) {
  if (lloc.lexer) {
    return (wulk_rtoken_t) {
      adt_vector_at(lloc.lexer->queue, lloc.begin),
      adt_vector_at(lloc.lexer->queue, lloc.end)
    };
  }
  return (wulk_rtoken_t) {{0}, {0}};
}
