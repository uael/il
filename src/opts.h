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

#ifndef   JL_OPTS_H__
# define  JL_OPTS_H__

#include <stdbool.h>

#define JL_SRC_DIR_MSIZE 256

typedef struct jl_opts_t jl_opts_t;

enum jl_backend_n {
  JL_BACKEND_NONE = 0,
  JL_BACKEND_DOT,
  JL_BACKEND_PPRINT,
  JL_BACKEND_FIRM_ASM,
  JL_BACKEND_FIRM_AMD64,
  JL_BACKEND_FIRM_ARM,
  JL_BACKEND_FIRM_IA32,
  JL_BACKEND_FIRM_MIPS,
  JL_BACKEND_FIRM_SPARC
};

struct jl_opts_t {
  const char *in, *out;
  char src_dir[JL_SRC_DIR_MSIZE];
  bool echo, firm_graph, verbose, output_asm;
  int optimize_lvl;
  enum jl_backend_n target;
};

void jl_opts_parse(jl_opts_t *self, int argc, char *argv[]);

#endif /* JL_OPTS_H__ */
