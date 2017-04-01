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

#ifndef   JL_COMPILER_H__
# define  JL_COMPILER_H__

#include <adt/string.h>
#include <adt/attr.h>

#include "opts.h"
#include "parser.h"
#include "be.h"

typedef struct jl_compiler_t jl_compiler_t;

struct jl_compiler_t {
  const char *program;
  jl_opts_t opts;
  jl_parser_t fe;
  jl_backend_t be;
  string_r strtab;
};

void jl_init(jl_compiler_t *self, int argc, char **argv);
void jl_dtor(jl_compiler_t *self);
JL_NORETURN jl_err(jl_compiler_t *self, const char *format, ...);
JL_NORETURN jl_fatal_err(jl_compiler_t *self, const char *format, ...);
JL_NORETURN jl_parse_err(jl_compiler_t *self, jl_loc_t loc, const char *format, ...);

const char *jl_strdup(jl_compiler_t *self, const char *str);
const char *jl_strndup(jl_compiler_t *self, const char *str, size_t n);

#endif /* JL_COMPILER_H__ */
