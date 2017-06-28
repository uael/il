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

typedef struct ill_cfe ill_cfe_t;
typedef struct ill_cbe ill_cbe_t;

struct ill_cfe {
  ILL_FE_SUPER;
  bool echo, prepossess;
};

int
cfe_opt_echo(ill_t *il, UNUSED const char *val) {
  ill_cfe_t *fe = (ill_cfe_t *) il->fe;
  fe->echo = true;
  return 0;
}

int
cfe_opt_prepossess(ill_t *il, UNUSED const char *val) {
  ill_cfe_t *fe = (ill_cfe_t *) il->fe;
  fe->prepossess = true;
  return 0;
}

struct ill_cbe {
  ILL_BE_SUPER;
  const char *out;
};

int
cbe_opt_out(ill_t *il, const char *val) {
  ill_cbe_t *be = (ill_cbe_t *) il->be;
  be->out = val;
  return 0;
}

CUTEST_DATA {
  ill_t il;
  ill_cfe_t fe;
  ill_cbe_t be;
  ill_opts_t opts;
};

CUTEST_SETUP {
  static ill_opt_t opts[4] = {
    {'S', "prepossess", "output the prepossessed input file", (ill_optcb_t) cfe_opt_prepossess},
    {0, "echo", "print the content of the input file", (ill_optcb_t) cfe_opt_echo},
    {'o', "output", "output file name", (ill_optcb_t) cbe_opt_out, true},
    {0, nullptr}
  };
  ill_init(&self->il, (ill_fe_t *) &self->fe, (ill_be_t *) &self->be);
  ill_opts_init(&self->opts, opts, (ill_optcb_t) ill_push_input);
}

CUTEST_TEARDOWN {
  ill_opts_dtor(&self->opts);
  ill_dtor(&self->il);
  self->fe = (ill_cfe_t) {0};
  self->be = (ill_cbe_t) {0};
}

CUTEST(opt, input);
CUTEST(opt, catval);
CUTEST(opt, mmatch);
CUTEST(opt, unrecognized);
CUTEST(opt, missing1);
CUTEST(opt, missing2);
CUTEST(opt, duplicate);

int
main(void) {
  CUTEST_DATA test = {0};

  CUTEST_PASS(opt, input);
  CUTEST_PASS(opt, catval);
  CUTEST_PASS(opt, mmatch);
  CUTEST_PASS(opt, unrecognized);
  CUTEST_PASS(opt, missing1);
  CUTEST_PASS(opt, missing2);
  CUTEST_PASS(opt, duplicate);
  return EXIT_SUCCESS;
}

CUTEST(opt, input) {
  ill_opts_parse(&self->opts, &self->il, 5, (char *[5]) {"cli", "-o", "bla", "-S", "test/opt.c"});
  ASSERT(self->be.out && memcmp("bla", self->be.out, 3) == 0);
  ASSERT(self->fe.prepossess == true);
  return CUTE_SUCCESS;
}

CUTEST(opt, catval) {
  ill_opts_parse(&self->opts, &self->il, 2, (char *[2]) {"cli", "-obla"});
  ASSERT(self->be.out && memcmp("bla", self->be.out, 3) == 0);
  return CUTE_SUCCESS;
}

CUTEST(opt, mmatch) {
  ill_opts_parse(&self->opts, &self->il, 5, (char *[5]) {"cli", "--output", "bla", "--echo", "--prepossess"});
  ASSERT(self->be.out && memcmp("bla", self->be.out, 3) == 0);
  ASSERT(self->fe.echo == true);
  ASSERT(self->fe.prepossess == true);
  return CUTE_SUCCESS;
}

CUTEST(opt, unrecognized) {
  ill_err_t err;

  ill_opts_parse(&self->opts, &self->il, 5, (char *[5]) {"cli", "--foo", "-foo", "-b", "--echo"});
  ASSERT(ds_size(self->opts.errs) == 3);
  err = ds_front(self->opts.errs);
  ASSERT(memcmp(err.msg, STRNSIZE("unrecognized command line option ‘foo’")) == 0);
  err = ds_at(self->opts.errs, 1);
  ASSERT(memcmp(err.msg, STRNSIZE("unrecognized command line option ‘f’")) == 0);
  err = ds_at(self->opts.errs, 2);
  ASSERT(memcmp(err.msg, STRNSIZE("unrecognized command line option ‘b’")) == 0);
  return CUTE_SUCCESS;
}

CUTEST(opt, missing1) {
  ill_err_t err;

  ill_opts_parse(&self->opts, &self->il, 2, (char *[2]) {"cli", "--output"});
  ASSERT(ds_size(self->opts.errs) == 1);
  err = ds_front(self->opts.errs);
  ASSERT(memcmp(err.msg, STRNSIZE("missing argument for command line option ‘output’")) == 0);
  return CUTE_SUCCESS;
}

CUTEST(opt, missing2) {
  ill_err_t err;

  ill_opts_parse(&self->opts, &self->il, 2, (char *[2]) {"cli", "-o"});
  ASSERT(ds_size(self->opts.errs) == 1);
  err = ds_front(self->opts.errs);
  ASSERT(memcmp(err.msg, STRNSIZE("missing argument for command line option ‘o’")) == 0);
  return CUTE_SUCCESS;
}

CUTEST(opt, duplicate) {
  ill_err_t err;

  ill_opts_parse(&self->opts, &self->il, 4, (char *[4]) {"cli", "-obla", "--echo", "-S"});
  ill_opts_parse(&self->opts, &self->il, 8,
    (char *[8]) {"cli", "--output", "bla", "--echo", "-obla", "-o", "bla", "-S"});
  ASSERT(ds_size(self->opts.errs) == 5);
  err = ds_front(self->opts.errs);
  ASSERT(memcmp(err.msg, STRNSIZE("duplicate value for command line option ‘output’: ‘bla’")) == 0);
  err = ds_at(self->opts.errs, 1);
  ASSERT(memcmp(err.msg, STRNSIZE("duplicate command line option ‘echo’")) == 0);
  err = ds_at(self->opts.errs, 2);
  ASSERT(memcmp(err.msg, STRNSIZE("duplicate value for command line option ‘o’: ‘bla’")) == 0);
  err = ds_at(self->opts.errs, 3);
  ASSERT(memcmp(err.msg, STRNSIZE("duplicate value for command line option ‘o’: ‘bla’")) == 0);
  err = ds_at(self->opts.errs, 4);
  ASSERT(memcmp(err.msg, STRNSIZE("duplicate command line option ‘S’")) == 0);
  return CUTE_SUCCESS;
}
