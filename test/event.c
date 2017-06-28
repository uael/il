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

#include "cute.h"
#include "ill.h"

typedef struct cpp cpp_t;
typedef struct cpp_obs cpp_obs_t;

typedef int (*ill_eventcb_t)(ill_event_t *, void *);

struct cpp {
  int dummy;
};

struct cpp_obs {
  ILL_EVENT_SUPER(ill_lexer_t);
  cpp_t cpp;
};

int cpp_obs_push_token(UNUSED cpp_obs_t *self, UNUSED ill_token_t *token) {
  return 0;
}

CUTEST_DATA {
  cpp_obs_t cpp_obs;
};

CUTEST_SETUP {}
CUTEST_TEARDOWN {}

CUTEST(event, 001);

int
main(void) {
  CUTEST_DATA test = {};

  CUTEST_PASS(event, 001);
  return EXIT_SUCCESS;
}

CUTEST(event, 001) {
  ill_lexer_t lexer = {0};
  ill_token_t token = {.kind = '\n', .name = "EOL"};

  self->cpp_obs = (cpp_obs_t) {.kind = 1, .callback = (ill_eventcb_t) cpp_obs_push_token};
  ill_lexer_attach(&lexer, self->cpp_obs);
  ill_lexer_trigger(&lexer, 1, &token);
  return CUTE_SUCCESS;
}
