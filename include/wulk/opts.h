/*
 * Wulk - Wu uniform language kit
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
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
