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

#include "il/opt.h"

FORCEINLINE
il_opt_flag_t il_opt_flag(char f, char *lf, int (* callback)(il_builder_t *, il_opt_flag_t *)) {
  return (il_opt_flag_t) {
    f, lf, callback
  };
}

FORCEINLINE
il_opt_kval_t il_opt_kval(char f, char *lf, bool global, int (* callback)(il_builder_t *, il_opt_kval_t *)) {
  return (il_opt_kval_t) {
    f, lf, global, callback
  };
}

bool il_builder_init(il_builder_t *self, il_compiler_t *compiler, int argc, char **argv) {
  uint8_t i;

  *self = (il_builder_t) {
    .compiler = compiler
  };
  for (i = 1; i < argc; ++i) {
    adt_deque_push(self->argv, argv[i]);
  }
  return true;
}

void il_builder_register_flag(il_builder_t *self, il_opt_flag_t opt) {
  (void) uvec_push(self->flags, opt);
}

void il_builder_register_kval(il_builder_t *self, il_opt_kval_t opt) {
  (void) uvec_push(self->kvals, opt);
}
