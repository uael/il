/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef   WULK_COMPILER_H__
# define  WULK_COMPILER_H__

#include <adt/string.h>
#include <adt/attr.h>

#include "opts.h"
#include "parser.h"
#include "be.h"
#include "program.h"

typedef struct wulk_compiler_t wulk_compiler_t;

struct wulk_compiler_t {
  const char *program;
  wulk_opts_t opts;
  wulk_program_t ast;
  wulk_parser_t fe;
  wulk_backend_t be;
  string_r strtab;
};

void wulk_init(wulk_compiler_t *self, int argc, char **argv);
void wulk_dtor(wulk_compiler_t *self);
void wulk_parse(wulk_compiler_t *self);
WULK_NORETURN wulk_err(wulk_compiler_t *self, const char *format, ...);
WULK_NORETURN wulk_fatal_err(wulk_compiler_t *self, const char *format, ...);
WULK_NORETURN wulk_parse_err(wulk_compiler_t *self, wulk_loc_t loc, const char *format, ...);

const char *wulk_strdup(wulk_compiler_t *self, const char *str);
const char *wulk_strndup(wulk_compiler_t *self, const char *str, size_t n);

#endif /* WULK_COMPILER_H__ */
