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

#include <stdarg.h>
#include <stdio.h>

#include "compiler.h"

#include "util/io.h"

void wulk_init(wulk_compiler_t *self, int argc, char **argv) {
  char *sep;

  if ((sep = strrchr(argv[0], '/'))) {
    argv[0] = ++sep;
  }
  *self = (wulk_compiler_t) {
    .program = argv[0]
  };
  if (!wulk_opts_init(&self->opts, argc, argv)) {
    wulk_opts_dtor(&self->opts);
    exit(EXIT_SUCCESS);
  }
  if (!self->opts.in) {
    wulk_fatal_err(self, "no input file");
  }
  if (adt_vector_size(self->opts.opts_errs)) {
    wulk_err(self, adt_vector_front(self->opts.opts_errs));
  }
  wulk_fe_init(&self->fe, WULK_PARSER_C, self);
  wulk_fe_push_src(&self->fe, self->opts.in);
}

void wulk_dtor(wulk_compiler_t *self) {
  const char *str;

  wulk_opts_dtor(&self->opts);
  if (wulk_defined(self->fe)) {
    wulk_fe_dtor(&self->fe);
  }
  adt_vector_foreach(self->strtab, str) {
    xfree((void *) str);
  }
  adt_vector_dtor(self->strtab);
}

void wulk_parse(wulk_compiler_t *self) {
  wulk_fe_parse(&self->fe, NULL, &self->ast);
}

WULK_NORETURN wulk_err(wulk_compiler_t *self, const char *format, ...) {
  va_list args;

  if (self) {
    printf(BOLD "%s: " BOLD RED "error: " RESET, self->program);
  } else {
    fputs(BOLD RED "error: " RESET, stdout);
  }
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  putc('\n', stdout);

  if (self) {
    wulk_dtor(self);
  }
  exit(EXIT_FAILURE);
}

WULK_NORETURN wulk_fatal_err(wulk_compiler_t *self, const char *format, ...) {
  va_list args;

  if (self) {
    printf(BOLD "%s: " BOLD RED "fatal error: " RESET, self->program);
  } else {
    fputs(BOLD RED "fatal error: " RESET, stdout);
  }
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  putc('\n', stdout);

  if (self) {
    wulk_dtor(self);
  }
  exit(EXIT_FAILURE);
}

WULK_NORETURN wulk_parse_err(wulk_compiler_t *self, wulk_loc_t loc, const char *format, ...) {
  size_t begin = loc.position - loc.colno;
  const char *ptr, *file;
  wulk_lexer_t lexer = (wulk_lexer_t) {0};
  wulk_token_t eol;
  va_list args;

  file = adt_vector_at(self->fe.sources, loc.file_id);
  wulk_lexer_fork(&lexer, self->fe.lexer);
  lexer.loc = loc;
  ptr = lexer.buffer + begin;
  while ((eol = wulk_lexer_next(&lexer)).type != '\n' && eol.type != '\0');
  printf(BOLD "%s:" RESET " In function ‘" BOLD "syntax_error" RESET "’:\n", file);
  printf(BOLD "%s:%d:%d: " BOLD RED "error: " RESET, file, loc.colno + 1, loc.lineno + 1);
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  printf("\n%.*s\n", (int) (eol.loc.position - begin), ptr);
  for (unsigned i = 0; i < loc.colno; ++i) {
    putc(' ', stdout);
  }
  fputs(BOLD YELLOW "^\n" RESET, stdout);

  wulk_lexer_dtor(&lexer, true);
  wulk_dtor(self);
  exit(EXIT_FAILURE);
}

const char *wulk_strdup(wulk_compiler_t *self, const char *str) {
  return wulk_strndup(self, str, strlen(str));
}

const char *wulk_strndup(wulk_compiler_t *self, const char *str, size_t n) {
  adt_vector_push(self->strtab, "");
  adt_vector_back(self->strtab) = xmalloc(n + 1);
  strncpy((char *) adt_vector_back(self->strtab), str, n + 1);
  return adt_vector_back(self->strtab);
}
