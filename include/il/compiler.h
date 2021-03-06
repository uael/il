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

#ifndef   IL_COMPILER_H__
# define  IL_COMPILER_H__

#include <il/adt/string.h>
#include <u/compiler.h>

#include "opts.h"
#include "parser.h"
#include "be.h"
#include "program.h"

typedef struct il_compiler_t il_compiler_t;

struct il_compiler_t {
  __const char *program;
  il_opts_t opts;
  il_program_t ast;
  il_parser_t fe;
  il_backend_t be;
  string_r strtab;
  char *in, *out;
};

void il_init(il_compiler_t *self, int argc, char **argv);
void il_dtor(il_compiler_t *self);
void il_parse(il_compiler_t *self);
NORETURN void il_err(il_compiler_t *self, __const char *format, ...);
NORETURN void il_fatal_err(il_compiler_t *self, __const char *format, ...);
NORETURN void il_parse_err(il_compiler_t *self, il_loc_t loc, __const char *format, ...);

__const char *il_strdup(il_compiler_t *self, __const char *str);
__const char *il_strndup(il_compiler_t *self, __const char *str, size_t n);

#endif /* IL_COMPILER_H__ */
