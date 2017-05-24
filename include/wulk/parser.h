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
