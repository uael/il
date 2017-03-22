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
struct jl_sym_t;
struct jl_program_t;

typedef enum jl_fe_n jl_fe_n;
typedef struct jl_fe_t jl_fe_t;

enum jl_fe_n {
  JL_FRONTEND_C = 0,
  JL_FRONTEND_JAY
};

struct jl_fe_t {
  struct jl_compiler_t *compiler;
  jl_fe_n kind;
  jl_deque_of(const char *) sources;

  struct jl_sym_t *scope;
  jl_entity_t entity;
  jl_expr_t expr;
  jl_stmt_t stmt;
  jl_type_t type;

  void (*parse)(struct jl_fe_t *self, struct jl_lexer_t *lexer, struct jl_program_t *out);
};

void jl_fe_init(jl_fe_t *self, jl_fe_n kind, struct jl_compiler_t *compiler);
void jl_fe_dtor(jl_fe_t *self);
void jl_fe_push_src(jl_fe_t *self, const char *src);
void jl_fe_scope(jl_fe_t *self, struct jl_program_t *out, const char *id);
void jl_fe_unscope(jl_fe_t *self);

#endif /* JL_FE_H__ */
