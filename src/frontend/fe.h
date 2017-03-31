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

#ifndef   JL_FE_H__
# define  JL_FE_H__

#include <adt/deque.h>

#include "entity_t.h"
#include "expr_t.h"
#include "stmt_t.h"
#include "type_t.h"

struct jl_compiler_t;
struct jl_lexer_t;
struct jl_scope_t;
struct jl_program_t;

jl_token_t jl_lexer_peek(struct jl_lexer_t *self);
jl_token_t jl_lexer_peekn(struct jl_lexer_t *self, unsigned n);
jl_token_t jl_lexer_next(struct jl_lexer_t *self);
jl_token_t jl_lexer_consume(struct jl_lexer_t *self, unsigned char type);
jl_token_t jl_lexer_consume_id(struct jl_lexer_t *self, const char *id);
void jl_lexer_undo(struct jl_lexer_t *lexer, jl_token_t until);

typedef struct jl_fe_t jl_fe_t;

enum jl_fe_n {
  JL_FRONTEND_UNDEFINED = 0,
  JL_FRONTEND_C,
  JL_FRONTEND_JAY
};

struct jl_fe_t {
  struct jl_compiler_t *compiler;
  struct jl_lexer_t *lexer;
  enum jl_fe_n kind;
  adt_deque_of(const char *) sources;

  struct jl_scope_t *scope;
  jl_entity_t entity;
  jl_expr_t expr;
  jl_stmt_t stmt;
  jl_type_t type;

  void (*parse)(struct jl_fe_t *self, struct jl_program_t *out);
};

void jl_fe_init(jl_fe_t *self, enum jl_fe_n kind, struct jl_compiler_t *compiler);
void jl_fe_dtor(jl_fe_t *self);
void jl_fe_parse(struct jl_fe_t *self, struct jl_lexer_t *lexer, struct jl_program_t *out);
void jl_fe_push_src(jl_fe_t *self, const char *src);
void jl_fe_scope(jl_fe_t *self);
void jl_fe_unscope(jl_fe_t *self);

#endif /* JL_FE_H__ */
