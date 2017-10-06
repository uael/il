/*
 * libil - Intermediate Language cross-platform c library
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

#ifndef   IL_OPTS_H__
# define  IL_OPTS_H__

#include <u/stdbool.h>
#include <u/vector.h>

#include "err.h"

typedef struct il_opts_t il_opts_t;

enum il_backend_n {
  IL_BACKEND_NONE = 0,
  IL_BACKEND_DOT,
  IL_BACKEND_PPRINT,
  IL_BACKEND_FIRM_ASM,
  IL_BACKEND_FIRM_AMD64,
  IL_BACKEND_FIRM_ARM,
  IL_BACKEND_FIRM_IA32,
  IL_BACKEND_FIRM_MIPS,
  IL_BACKEND_FIRM_SPARC
};

struct il_opts_t {
  __const char *in, *out;
  char src_dir[UINT8_MAX];
  bool echo, firm_graph, verbose, output_asm;
  int optimize_lvl;
  enum il_backend_n target;
  adt_vector_of(char *) opts_errs;
};

bool il_opts_init(il_opts_t *self, int argc, char **argv);
void il_opts_dtor(il_opts_t *self);

#endif /* IL_OPTS_H__ */
