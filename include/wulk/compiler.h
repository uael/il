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
