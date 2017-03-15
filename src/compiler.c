/*
 * MIT License
 *
 * Copyright (c) 2014 CreoLabs. All rights reserved.
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

#include "compiler.h"

void jl_init(jl_compiler_t *self, int argc, char **argv) {
  *self = (jl_compiler_t) {
    .program = argv[0]
  };

  jl_opts_parse(&self->opts, argc, argv);
  jl_frontend_init(&self->fe, JL_FRONTEND_JAY, self);
}

void jl_dtor(jl_compiler_t *self) {
  const char *str;

  jl_frontend_dtor(&self->fe);
  jl_vector_foreach(self->strtab, str) {
    free((void *) str);
  }
  jl_vector_dtor(self->strtab);
}

const char *jl_strdup(jl_compiler_t *self, const char *str) {
  return jl_strndup(self, str, strlen(str));
}

const char *jl_strndup(jl_compiler_t *self, const char *str, size_t n) {
  jl_vector_push(self->strtab, "");
  jl_vector_back(self->strtab) = xmalloc(n + 1);
  strncpy((char *) jl_vector_back(self->strtab), str, n + 1);
  return jl_vector_back(self->strtab);
}
