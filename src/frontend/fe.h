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

#include <stddef.h>

#include "adt/vector.h"
#include "lexer.h"

struct jl_compiler_t;

typedef struct jl_frontend_t {
  struct jl_compiler_t *compiler;
  jl_frontend_n kind;
  jl_vector_of(const char *) sources;

  void (*parse)(struct jl_frontend_t *self, jl_lexer_t *lexer, jl_entity_r *out);
} jl_frontend_t;

void jl_frontend_init(jl_frontend_t *self, jl_frontend_n kind, struct jl_compiler_t *compiler);
void jl_frontend_push_src(jl_frontend_t *self, const char *src);

#endif /* JL_FE_H__ */
