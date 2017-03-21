/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
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

#ifndef   JL_FRULE_H__
# define  JL_FRULE_H__

#include "fval.h"

typedef struct jl_frule_t jl_frule_t;

struct jl_frule_t {
  jl_fval_n expected;
  bool (*callback)(jl_fval_t *, jl_frontend_t *, jl_lexer_t *, jl_program_t *);
};

bool jl_frule_validate(jl_frule_t self, jl_fval_t *fval, jl_frontend_t *fe, jl_lexer_t *lexer, jl_program_t *out);

#define $$ *fval
#define JL_RULEFN(name) name

#define JL_RULEDC(name) \
  static bool JL_RULEFN(name)(jl_fval_t *fval, jl_frontend_t *fe, jl_lexer_t *lexer, jl_program_t *out)

#define Jl_RULEBG \
  jl_fval_t $1, $2, $3, $4, $5, $6, $7, $8, $9; jl_sym_t *sym; *fval = jl_fval_undefined()

#define Jl_RULEED \
  return fval->kind != JL_FVAL_UNDEFINED

#define JL_RULEDF(name) \
  JL_RULEDC(name)

#define JL_MATCHR(n, name, expected) \
  if (jl_frule_validate((jl_frule_t) {expected, JL_RULEFN(name)}, &$ ## n, fe, lexer, out))

#define JL_MATCHT(n, name) \
  if (jl_lexer_peek(lexer).type == name \
    ? ($ ## n = jl_fval_token(jl_lexer_peek(lexer)), jl_lexer_next(lexer), true) \
    : ((n==1?(void)0:jl_lexer_undo(lexer, $1.begin)), false))

#endif /* JL_FRULE_H__ */
