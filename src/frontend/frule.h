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

#include "fir.h"

struct jl_lexer_t;
struct jl_fe_t;
struct jl_program_t;

typedef struct jl_frule_t jl_frule_t;

struct jl_frule_t {
  enum jl_fir_n expected;
  bool (*callback)(jl_fir_t *, struct jl_fe_t *, struct jl_lexer_t *, struct jl_program_t *);
};

bool jl_frule_validate(jl_frule_t self, jl_fir_t *fir, struct jl_fe_t *fe, struct jl_lexer_t *lexer, struct jl_program_t *out);

#define _0 fir
#define FRULE_FN(name) name

#define FRULE_DECL(name) \
  static bool FRULE_FN(name)(jl_fir_t *fir, jl_fe_t *fe, jl_lexer_t *lexer, jl_program_t *out)

#define FRULE_DEF(name) \
  FRULE_DECL(name)

#define FRULE_BODY_BEGIN \
  jl_fir_t _1, _2, _3, _4, _5, _6, _7, _8, _9; jl_sym_t *sym; jl_token_t token; *fir = (jl_fir_t) {JL_FIR_UNDEFINED}

#define FRULE_BODY_END \
  return fir->kind != JL_FIR_UNDEFINED

#define FRULE_OR else

#endif /* JL_FRULE_H__ */
