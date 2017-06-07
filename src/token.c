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

#include "token.h"

#include "lexer.h"


void il_token_dtor(UNUSED il_token_t *self) {}

il_lloc_t il_lloc_begin(il_lexer_t *lexer) {
  return (il_lloc_t) {
    .lexer = lexer,
    .begin = il_lexer_peek(lexer).cursor
  };
}

il_lloc_t il_lloc_end(il_lloc_t self){
  self.end = il_lexer_peek(self.lexer).cursor;
  return self;
}

il_rtoken_t il_llocate(il_lloc_t lloc) {
  if (lloc.lexer) {
    return (il_rtoken_t) {
      adt_vector_at(lloc.lexer->queue, lloc.begin),
      adt_vector_at(lloc.lexer->queue, lloc.end)
    };
  }
  return (il_rtoken_t) {{0}, {0}};
}
