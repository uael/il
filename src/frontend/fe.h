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

#include "program.h"
#include "entity_t.h"
#include "expr_t.h"
#include "stmt_t.h"
#include "type_t.h"

struct jl_compiler_t;
struct jl_lexer_t;

typedef enum jl_frontend_n jl_frontend_n;
typedef struct jl_frontend_t jl_frontend_t;

enum jl_frontend_n {
  JL_FRONTEND_C = 0,
  JL_FRONTEND_JAY
};

struct jl_frontend_t {
  struct jl_compiler_t *compiler;
  jl_frontend_n kind;
  jl_deque_of(const char *) sources;

  jl_sym_t *scope;
  jl_entity_t entity;
  jl_expr_t expr;
  jl_stmt_t stmt;
  jl_type_t type;

  void (*parse)(struct jl_frontend_t *self, struct jl_lexer_t *lexer, jl_program_t *out);
};

void jl_frontend_init(jl_frontend_t *self, jl_frontend_n kind, struct jl_compiler_t *compiler);
void jl_frontend_dtor(jl_frontend_t *self);
void jl_frontend_push_src(jl_frontend_t *self, const char *src);
void jl_frontend_scope(jl_frontend_t *self, jl_program_t *out, const char *id);
void jl_frontend_unscope(jl_frontend_t *self);

#endif /* JL_FE_H__ */
