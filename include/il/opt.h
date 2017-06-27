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

#ifndef  IL_OPT_H__
# define IL_OPT_H__

#include <u/stdbool.h>
#include <u/vector.h>

#include "err.h"
#include "compiler.h"

typedef struct il_opt_flag il_opt_flag_t;
typedef struct il_opt_kval il_opt_kval_t;
typedef struct il_builder il_builder_t;

struct il_opt_flag {
  char f, *lf;
  int (* callback)(il_builder_t *builder, il_opt_flag_t *opt);
};

FORCEINLINE
il_opt_flag_t il_opt_flag(char f, char *lf, int (* callback)(il_builder_t *, il_opt_flag_t *));

struct il_opt_kval {
  char k, *lk;
  bool global;
  int (* callback)(il_builder_t *builder, il_opt_kval_t *opt);
  uvec_of(char *) values;
};

FORCEINLINE
il_opt_kval_t il_opt_kval(char f, char *lf, bool global, int (* callback)(il_builder_t *, il_opt_kval_t *));

struct il_builder {
  il_compiler_t *compiler;
  adt_deque_of(char *) argv;
  uvec_of(il_opt_flag_t) flags;
  uvec_of(il_opt_kval_t) kvals;
  uvec_of(il_err_t) errs;
};

bool il_builder_init(il_builder_t *self, il_compiler_t *compiler, int argc, char **argv);
void il_builder_register_flag(il_builder_t *self, il_opt_flag_t opt);
void il_builder_register_kval(il_builder_t *self, il_opt_kval_t opt);

#endif /* IL_OPT_H__ */
