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

#include "frule.h"

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
  JL_FRONTEND_C = 0,
  JL_FRONTEND_JAY
};

struct jl_fe_t {
  struct jl_compiler_t *compiler;
  enum jl_fe_n kind;
  adt_deque_of(const char *) sources;

  struct jl_scope_t *scope;
  jl_entity_t entity;
  jl_expr_t expr;
  jl_stmt_t stmt;
  jl_type_t type;

  void (*parse)(struct jl_fe_t *self, struct jl_lexer_t *lexer, struct jl_program_t *out);
};

void jl_fe_init(jl_fe_t *self, enum jl_fe_n kind, struct jl_compiler_t *compiler);
void jl_fe_dtor(jl_fe_t *self);
void jl_fe_push_src(jl_fe_t *self, const char *src);
void jl_fe_scope(jl_fe_t *self);
void jl_fe_unscope(jl_fe_t *self);

#define FE_PEEK() jl_lexer_peek(lexer)
#define FE_PEEKN(n) jl_lexer_peekn(lexer, n)
#define FE_NEXT() jl_lexer_next(lexer)
#define FE_CONSUME(t) jl_lexer_consume(lexer, t)
#define FE_CONSUME_ID(id) jl_lexer_consume_id(lexer, id)
#define FE_UNDO(until) jl_lexer_undo(lexer, until)
#define FE_UNDON(n) jl_lexer_undon(lexer, n)

#define FE_FRULE(val, name) \
 FRULE_FN(name)(val, fe, lexer, out)

#define FE_FRULE_DIRECT(name) \
  FE_FRULE(fir, name)

#define FE_MATCHR(n, name) \
  if (_ ## n = (jl_fir_t) {JL_FIR_UNDEFINED}, FE_FRULE(&_ ## n, name))

#define FE_MATCHT(n, name) \
  if (FE_PEEK().type == name \
    ? (_ ## n = (jl_fir_t) {JL_FIR_UNDEFINED}, jl_fir_init_token(&_ ## n, FE_PEEK()), FE_NEXT(), true) \
    : (FE_UNDON(n-1), false))

#endif /* JL_FE_H__ */
