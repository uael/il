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

#ifndef   WULK_OPTS_H__
# define  WULK_OPTS_H__

#include <adt/bool.h>

#define WULK_SRC_DIR_MSIZE 256

typedef struct wulk_opts_t wulk_opts_t;

enum wulk_backend_n {
  WULK_BACKEND_NONE = 0,
  WULK_BACKEND_DOT,
  WULK_BACKEND_PPRINT,
  WULK_BACKEND_FIRM_ASM,
  WULK_BACKEND_FIRM_AMD64,
  WULK_BACKEND_FIRM_ARM,
  WULK_BACKEND_FIRM_IA32,
  WULK_BACKEND_FIRM_MIPS,
  WULK_BACKEND_FIRM_SPARC
};

struct wulk_opts_t {
  const char *in, *out;
  char src_dir[WULK_SRC_DIR_MSIZE];
  bool echo, firm_graph, verbose, output_asm;
  int optimize_lvl;
  enum wulk_backend_n target;
  adt_vector_of(char *) opts_errs;
};

bool wulk_opts_init(wulk_opts_t *self, int argc, char **argv);
void wulk_opts_dtor(wulk_opts_t *self);

#endif /* WULK_OPTS_H__ */
