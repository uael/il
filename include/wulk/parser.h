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

#ifndef   WULK_PARSER_H__
# define  WULK_PARSER_H__

#include <adt/deque.h>

#include "lexer.h"

struct wulk_scope_t;
struct wulk_program_t;

typedef struct wulk_parser_t wulk_parser_t;

enum wulk_parser_n {
  WULK_PARSER_UNDEFINED = 0,
  WULK_PARSER_C,
  WULK_PARSER_JAY
};

struct wulk_parser_t {
  enum wulk_parser_n kind;
  struct wulk_compiler_t *compiler;
  struct wulk_scope_t *scope;
  wulk_lexer_t *lexer;
  adt_deque_of(const char *) sources;

  void (*parse)(wulk_parser_t *self, struct wulk_program_t *out);
};

void wulk_fe_init(wulk_parser_t *self, enum wulk_parser_n kind, struct wulk_compiler_t *compiler);
void wulk_fe_dtor(wulk_parser_t *self);
void wulk_fe_parse(wulk_parser_t *self, wulk_lexer_t *lexer, struct wulk_program_t *out);
void wulk_fe_push_src(wulk_parser_t *self, const char *src);
void wulk_fe_scope(wulk_parser_t *self);
void wulk_fe_unscope(wulk_parser_t *self);

#endif /* WULK_PARSER_H__ */
