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

#ifndef   IL_PARSER_H__
# define  IL_PARSER_H__

#include <adt/deque.h>

#include "lexer.h"

struct il_scope_t;
struct il_program_t;

typedef struct il_parser_t il_parser_t;

enum il_parser_n {
  IL_PARSER_UNDEFINED = 0,
  IL_PARSER_C,
  IL_PARSER_JAY
};

struct il_parser_t {
  enum il_parser_n kind;
  struct il_compiler_t *compiler;
  struct il_scope_t *scope;
  il_lexer_t *lexer;
  adt_deque_of(const char *) sources;

  void (*parse)(il_parser_t *self, struct il_program_t *out);
};

void il_fe_init(il_parser_t *self, enum il_parser_n kind, struct il_compiler_t *compiler);
void il_fe_dtor(il_parser_t *self);
void il_fe_parse(il_parser_t *self, il_lexer_t *lexer, struct il_program_t *out);
void il_fe_push_src(il_parser_t *self, const char *src);
void il_fe_scope(il_parser_t *self);
void il_fe_unscope(il_parser_t *self);

#endif /* IL_PARSER_H__ */
