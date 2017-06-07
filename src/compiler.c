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

#include <stdarg.h>
#include <stdio.h>

#include "compiler.h"

#include "util/io.h"

void il_init(il_compiler_t *self, int argc, char **argv) {
  char *sep;

  if ((sep = strrchr(argv[0], '/'))) {
    argv[0] = ++sep;
  }
  *self = (il_compiler_t) {
    .program = argv[0]
  };
  if (!il_opts_init(&self->opts, argc, argv)) {
    il_opts_dtor(&self->opts);
    exit(EXIT_SUCCESS);
  }
  if (!self->opts.in) {
    il_fatal_err(self, "no input file");
  }
  if (adt_vector_size(self->opts.opts_errs)) {
    il_err(self, adt_vector_front(self->opts.opts_errs));
  }
  il_fe_init(&self->fe, IL_PARSER_C, self);
  il_fe_push_src(&self->fe, self->opts.in);
}

void il_dtor(il_compiler_t *self) {
  const char *str;

  il_opts_dtor(&self->opts);
  if (il_defined(self->fe)) {
    il_fe_dtor(&self->fe);
  }
  adt_vector_foreach(self->strtab, str) {
    xfree((void *) str);
  }
  adt_vector_dtor(self->strtab);
}

void il_parse(il_compiler_t *self) {
  il_fe_parse(&self->fe, NULL, &self->ast);
}

NORETURN void il_err(il_compiler_t *self, const char *format, ...) {
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
    il_dtor(self);
  }
  exit(EXIT_FAILURE);
}

NORETURN void il_fatal_err(il_compiler_t *self, const char *format, ...) {
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
    il_dtor(self);
  }
  exit(EXIT_FAILURE);
}

NORETURN void il_parse_err(il_compiler_t *self, il_loc_t loc, const char *format, ...) {
  size_t begin = loc.position - loc.colno;
  const char *ptr, *file;
  il_lexer_t lexer = (il_lexer_t) {0};
  il_token_t eol;
  va_list args;

  file = adt_vector_at(self->fe.sources, loc.file_id);
  il_lexer_fork(&lexer, self->fe.lexer);
  lexer.loc = loc;
  ptr = lexer.buffer + begin;
  while ((eol = il_lexer_next(&lexer)).type != '\n' && eol.type != '\0');
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

  il_lexer_dtor(&lexer, true);
  il_dtor(self);
  exit(EXIT_FAILURE);
}

const char *il_strdup(il_compiler_t *self, const char *str) {
  return il_strndup(self, str, strlen(str));
}

const char *il_strndup(il_compiler_t *self, const char *str, size_t n) {
  adt_vector_push(self->strtab, "");
  adt_vector_back(self->strtab) = xmalloc(n + 1);
  strncpy((char *) adt_vector_back(self->strtab), str, n + 1);
  return adt_vector_back(self->strtab);
}
