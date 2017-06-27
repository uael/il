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

#include <memory.h>
#include <il/adt/hash.h>
#include <assert.h>
#include "u/vector.h"

typedef struct ill ill_t;
typedef struct ill_fe ill_fe_t;
typedef struct ill_be ill_be_t;
typedef struct ill_event ill_event_t;
typedef struct ill_src ill_src_t;
typedef struct ill_loc ill_loc_t;
typedef struct ill_token ill_token_t;
typedef struct ill_lloc ill_lloc_t;
typedef struct ill_lexer ill_lexer_t;
typedef struct ill_scope ill_scope_t;
typedef struct ill_tu ill_tu_t;
typedef struct ill_opts ill_opts_t;
typedef struct ill_flag ill_flag_t;
typedef struct ill_kval ill_kval_t;
typedef enum ill_errlvl ill_errlvl_t;
typedef enum ill_errkind ill_errkind_t;
typedef struct ill_err ill_err_t;

enum ill_errkind {
  IL_ERRKIND_OPTS,
  IL_ERRKIND_FE,
  IL_ERRKIND_BE
};

enum ill_errlvl {
  IL_ERRLVL_NOTICE,
  IL_ERRLVL_WARNING,
  IL_ERRLVL_ERROR,
  IL_ERRLVL_FATAL
};

struct ill_err {
  ill_errkind_t kind;
  ill_errlvl_t lvl;
  char *msg;
  ill_err_t *prev;
};

struct ill_event {
  unsigned int kind : 4;
  void *sender;
  void *data;

  bool (*callback)(ill_event_t *self, void *arg);
  void (*dtor)(ill_event_t *self);
};

struct ill_src {
  char filename[FILENAME_MAX+1];
  char path[PATH_MAX+1];
  FILE *fd;
  char *buffer;
  size_t length;
};

struct ill_loc {
  ill_src_t *source;
  uint32_t lineno;
  uint32_t colno;
  uint32_t position;
};

struct ill_token {
  unsigned int kind : 4;
  char type;
  const char *name, *value;
  size_t cursor;
  ill_loc_t loc;
  uint32_t length, leading_ws;
};

struct ill_lloc {
  ill_lexer_t *lexer;
  size_t begin, end;
};

struct ill_lexer {
  ill_t *il;
  ill_lexer_t *parent;
  uvec_of(ill_lexer_t) childs;
  uvec_of(ill_event_t) events;

  ill_loc_t loc;
  unsigned char cap;
  uvec_of(ill_token_t) queue;

  void (*enqueue)(ill_lexer_t *self, unsigned n);
};

struct ill_scope {
  int dummy;
};

struct ill_fe {
  void *impl;
  ill_t *il;
  ill_fe_t *parent;
  uvec_of(ill_fe_t) childs;
  uvec_of(ill_event_t) events;

  uvec_of(ill_err_t) errs;
  ill_scope_t *scope;
  uvec_of(ill_src_t) sources;

  void (*parse)(ill_fe_t *self);
};

struct ill_be {
  void *impl;
  ill_t *il;
  ill_be_t *parent;
  uvec_of(ill_be_t) childs;
  uvec_of(ill_event_t) events;

  uvec_of(ill_err_t) errs;

  void (*emit)(ill_be_t *self);
};

struct ill_tu {
  ill_lloc_t lloc;
  int dummy;
};

struct ill_flag {
  char f, *lf, *help;
  int (* callback)(ill_t *il, ill_flag_t *opt);
  bool match;
};

struct ill_kval {
  char k, *lk, *help;
  bool global;
  int (*callback)(ill_t *il, ill_kval_t *opt, const char *val);
  uvec_of(char *) values;
};

KHASH_DECLARE(ill_flags, const char *, ill_flag_t)
KHASH_DECLARE(ill_flags_sc, khint32_t, ill_flag_t *)
KHASH_DECLARE(ill_kvals, const char *, ill_kval_t)
KHASH_DECLARE(ill_kvals_sc, khint32_t, ill_kval_t *)

struct ill_opts {
  const char *program;
  ill_flags_t flags;
  ill_flags_sc_t flags_sc;
  ill_kvals_t kvals;
  ill_kvals_sc_t kvals_sc;
  uvec_of(char *) values;
  uvec_of(ill_err_t) errs;
  int (*callback)(ill_t *il, const char *val);
};

struct ill {
  ill_fe_t fe;
  ill_be_t be;
  uvec_of(ill_tu_t) tus;
  ill_opts_t opts;
};

KHASH_MAP_IMPL_STR(ill_flags, ill_flag_t)
KHASH_MAP_IMPL_INT(ill_flags_sc, ill_flag_t *)
KHASH_MAP_IMPL_STR(ill_kvals, ill_kval_t)
KHASH_MAP_IMPL_INT(ill_kvals_sc, ill_kval_t *)

void
ill_src_init(ill_src_t *self, const char *filename) {
  char *sep = nullptr;

#if !COMPILER_MSVC
  sep = realpath(filename, self->filename);
#else
  // todo
#endif
  if (sep == nullptr) {
    memcpy(self->filename, filename, strlen(filename) + 1);
  }

#if COMPILER_MSVC
  if ((sep = strrchr(self->filename, '\\'))) {
    memcpy(self->path, self->filename, (size_t) (sep - self->filename));
  }
#else
  if ((sep = strrchr(self->filename, '/'))) {
    memcpy(self->path, self->filename, (size_t) (sep - self->filename));
  }
#endif
  else {
    UNUSED char *file = getcwd(self->filename, PATH_MAX);
  }
}

void
ill_src_dtor(ill_src_t *self) {

}

void
ill_err_dtor(ill_err_t *self) {
  free(self->msg);
  if (self->prev) {
    ill_err_dtor(self->prev);
  }
}

void
ill_flag_dtor(ill_flag_t *self) {}

void
ill_kval_dtor(ill_kval_t *self) {
  uvec_dtor(self->values);
}

void
ill_opts_init(ill_opts_t *self, ill_flag_t *flags, ill_kval_t *kvals, int (*callback)(ill_t *, const char *)) {
  int r;
  unsigned it;

  *self = (ill_opts_t) {
    .callback = callback
  };
  if (flags) {
    while (flags->lf) {
      ill_flag_t *flag = flags++;

      it = kh_put(ill_flags, &self->flags, flag->lf, &r);
      if (r != 0) {
        kh_value(&self->flags, it) = *flag;
        flag = self->flags.vals + it;
      }
      if (flag->f) {
        it = kh_put(ill_flags_sc, &self->flags_sc, (uint32_t) flag->f, &r);
        if (r != 0) {
          kh_value(&self->flags_sc, it) = flag;
        }
      }
    }
  }
  if (kvals) {
    while (kvals->lk) {
      ill_kval_t *kval = kvals++;

      it = kh_put(ill_kvals, &self->kvals, kval->lk, &r);
      if (r != 0) {
        kh_value(&self->kvals, it) = *kval;
        kval = self->kvals.vals + it;
      }
      if (kval->k) {
        it = kh_put(ill_kvals_sc, &self->kvals_sc, (uint32_t) kval->k, &r);
        if (r != 0) {
          kh_value(&self->kvals_sc, it) = kval;
        }
      }
    }
  }
}

void
ill_opts_dtor(ill_opts_t *self) {
  ill_err_t err;
  ill_flag_t flag;
  ill_kval_t kval;

  if (ds_size(self->errs)) {
    foreach(err, in(T_uvec, self->errs)) {
      ill_err_dtor(&err);
    }
  }
  uvec_dtor(self->errs);
  kh_foreach_value(&self->flags, flag, {
    ill_flag_dtor(&flag);
  });
  kh_destroy(ill_flags, &self->flags);
  kh_destroy(ill_flags_sc, &self->flags_sc);
  kh_foreach_value(&self->kvals, kval, {
    ill_kval_dtor(&kval);
  });
  kh_destroy(ill_kvals, &self->kvals);
  kh_destroy(ill_kvals_sc, &self->kvals_sc);
}

ill_flag_t *
ill_flag(ill_opts_t *opts, char id) {
  unsigned it;

  it = kh_get(ill_flags_sc, &opts->flags_sc, (uint32_t) id);
  if (it == kh_end(&opts->flags_sc)) {
    return NULL;
  }
  return kh_value(&opts->flags_sc, it);
}

ill_flag_t *
ill_lflag(ill_opts_t *opts, const char *id) {
  unsigned it;

  it = kh_get(ill_flags, &opts->flags, id);
  if (it == kh_end(&opts->flags)) {
    return NULL;
  }
  return &kh_value(&opts->flags, it);
}

ill_kval_t *
ill_kval(ill_opts_t *opts, char id) {
  unsigned it;

  it = kh_get(ill_kvals_sc, &opts->kvals_sc, (uint32_t) id);
  if (it == kh_end(&opts->kvals_sc)) {
    return NULL;
  }
  return kh_value(&opts->kvals_sc, it);
}

ill_kval_t *
ill_lkval(ill_opts_t *opts, const char *id) {
  unsigned it;

  it = kh_get(ill_kvals, &opts->kvals, id);
  if (it == kh_end(&opts->kvals)) {
    return NULL;
  }
  return &kh_value(&opts->kvals, it);
}

int
ill_opt_parse(ill_opts_t *self, ill_t *il, int argc, char **argv) {
  char *arg, errmsg[UINT8_MAX];
  ill_flag_t *flag;
  ill_kval_t *kval;
  int i;

  if (argc) {
    if ((self->program = strrchr(argv[0], '/'))) {
      ++self->program;
    } else {
      self->program = argv[0];
    }
    for (i = 1; i < argc; ++i) {
      arg = argv[i];
      if (*arg == '-') {
        if (*(arg + 1) == '-') {
          flag = ill_lflag(self, arg + 2);

          if (flag == nullptr) {
            kval = ill_lkval(self, arg + 2);

            if (kval == nullptr) {
              sprintf(errmsg, "unrecognized command line option ‘%s’", arg + 2);
              uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_WARNING, strdup(errmsg)}));
            } else {
              if (i < argc - 1) {
                char *val = argv[++i];

                if (kval->callback == nullptr || kval->callback(il, kval, val) == 0) {
                  if (!kval->global && ds_size(kval->values)) {
                    sprintf(errmsg, "duplicate value for command line option ‘%s’", arg + 2);
                    uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_WARNING, strdup(errmsg)}));
                  } else {
                    uvec_push(kval->values, val);
                  }
                }
                continue;
              }
              sprintf(errmsg, "missing argument for command line option ‘%s’", arg + 2);
              uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_ERROR, strdup(errmsg)}));
            }
          } else {
            if (flag->match) {
              sprintf(errmsg, "duplicate command line option ‘%s’", arg + 2);
              uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_WARNING, strdup(errmsg)}));
            } else if (flag->callback == nullptr || flag->callback(il, flag) == 0) {
              flag->match = true;
            }
          }
        } else {
          if (arg[2] != '\0') {
            kval = ill_kval(self, arg[1]);

            if (kval == nullptr) {
              sprintf(errmsg, "unrecognized command line option ‘%c’", arg[1]);
              uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_ERROR, strdup(errmsg)}));
            } else {
              if (kval->callback == nullptr || kval->callback(il, kval, arg + 2) == 0) {
                if (!kval->global && ds_size(kval->values)) {
                  sprintf(errmsg, "duplicate value for command line option ‘%c’", arg[1]);
                  uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_WARNING, strdup(errmsg)}));
                } else {
                  uvec_push(kval->values, arg + 2);
                }
              }
            }
          } else {
            flag = ill_flag(self, arg[1]);

            if (flag == nullptr) {
              kval = ill_kval(self, arg[1]);

              if (kval == nullptr) {
                sprintf(errmsg, "unrecognized command line option ‘%c’", arg[1]);
                uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_WARNING, strdup(errmsg)}));
              } else {
                if (i < argc - 1) {
                  char *val = argv[++i];

                  if (kval->callback == nullptr || kval->callback(il, kval, val) == 0) {
                    if (!kval->global && ds_size(kval->values)) {
                      sprintf(errmsg, "duplicate value for command line option ‘%c’", arg[1]);
                      uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_WARNING, strdup(errmsg)}));
                    } else {
                      uvec_push(kval->values, val);
                    }
                  }
                  continue;
                }
                sprintf(errmsg, "missing argument for command line option ‘%c’", arg[1]);
                uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_ERROR, strdup(errmsg)}));
              }
            } else {
              if (flag->match) {
                sprintf(errmsg, "duplicate command line option ‘%c’", arg[1]);
                uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_WARNING, strdup(errmsg)}));
              } else if (flag->callback == nullptr || flag->callback(il, flag) == 0) {
                flag->match = true;
              }
            }
          }
        }
      } else {
        if (!self->callback || (self->callback(il, arg) == 0)) {
          uvec_push(self->values, arg);
        }
      }
    }
  }
}

int
ill_push_source(ill_t *self, const char *filename) {
  ill_src_t src = {0};

  ill_src_init(&src, filename);
  uvec_push(self->fe.sources, src);
  return 0;
}

void
ill_init(ill_t *self, ill_flag_t *flags, ill_kval_t *kvals, int (*callback)(ill_t *, const char *), int argc, char **argv) {
  *self = (ill_t) {{0}};

  ill_opts_init(&self->opts, flags, kvals, callback);
  ill_opt_parse(&self->opts, self, argc, argv);
}

void
ill_dtor(ill_t *self) {
  ill_opts_dtor(&self->opts);
}

#define STRNSIZE(s) (s), (sizeof(s)-1)

int
main(void) {
  ill_t il;
  ill_kval_t *output;
  ill_flag_t *echo, *prepossess;
  ill_err_t err;
  static ill_flag_t flags[3] = {
    {'S', "prepossess", "output the prepossessed input file"},
    {0, "echo", "print the content of the input file"},
    {0, nullptr}
  };
  static ill_kval_t kvals[2] = {
    {'o', "output", "output file name", false},
    {0, nullptr}
  };

  ill_init(&il, flags, kvals, ill_push_source, 5, (char *[5]) {"cli", "-o", "bla", "-S", "test/opt.c"});
  output = ill_kval(&il.opts, 'o');
  prepossess = ill_flag(&il.opts, 'S');
  assert(memcmp("bla", ds_front(output->values), 3) == 0);
  assert(prepossess->match == true);
  ill_dtor(&il);

  ill_init(&il, flags, kvals, nullptr, 2, (char *[2]) {"cli", "-obla"});
  output = ill_kval(&il.opts, 'o');
  assert(memcmp("bla", ds_front(output->values), 3) == 0);
  ill_dtor(&il);

  ill_init(&il, flags, kvals, nullptr, 5, (char *[5]) {"cli", "--output", "bla", "--echo", "--prepossess"});
  output = ill_kval(&il.opts, 'o');
  echo = ill_lflag(&il.opts, "echo");
  prepossess = ill_flag(&il.opts, 'S');
  assert(memcmp("bla", ds_front(output->values), 3) == 0);
  assert(echo->match == true);
  assert(prepossess->match == true);
  ill_dtor(&il);

  ill_init(&il, flags, kvals, nullptr, 4, (char *[4]) {"cli", "--foo", "-foo", "-b"});
  assert(ds_size(il.opts.errs) == 3);
  err = ds_front(il.opts.errs);
  assert(memcmp(err.msg, STRNSIZE("unrecognized command line option ‘foo’")) == 0);
  err = ds_at(il.opts.errs, 1);
  assert(memcmp(err.msg, STRNSIZE("unrecognized command line option ‘f’")) == 0);
  err = ds_at(il.opts.errs, 2);
  assert(memcmp(err.msg, STRNSIZE("unrecognized command line option ‘b’")) == 0);
  ill_dtor(&il);

  ill_init(&il, flags, kvals, nullptr, 2, (char *[2]) {"cli", "--output"});
  assert(ds_size(il.opts.errs) == 1);
  err = ds_front(il.opts.errs);
  assert(memcmp(err.msg, STRNSIZE("missing argument for command line option ‘output’")) == 0);
  ill_dtor(&il);

  ill_init(&il, flags, kvals, nullptr, 2, (char *[2]) {"cli", "-o"});
  assert(ds_size(il.opts.errs) == 1);
  err = ds_front(il.opts.errs);
  assert(memcmp(err.msg, STRNSIZE("missing argument for command line option ‘o’")) == 0);
  ill_dtor(&il);

  flags[0].match = true;
  flags[1].match = true;
  uvec_push(kvals[0].values, "bla");
  ill_init(&il, flags, kvals, nullptr, 8, (char *[8]) {"cli", "--output", "bla", "--echo", "-obla", "-o", "bla", "-S"});
  assert(ds_size(il.opts.errs) == 5);
  err = ds_front(il.opts.errs);
  assert(memcmp(err.msg, STRNSIZE("duplicate value for command line option ‘output’")) == 0);
  err = ds_at(il.opts.errs, 1);
  assert(memcmp(err.msg, STRNSIZE("duplicate command line option ‘echo’")) == 0);
  err = ds_at(il.opts.errs, 2);
  assert(memcmp(err.msg, STRNSIZE("duplicate value for command line option ‘o’")) == 0);
  err = ds_at(il.opts.errs, 3);
  assert(memcmp(err.msg, STRNSIZE("duplicate value for command line option ‘o’")) == 0);
  err = ds_at(il.opts.errs, 4);
  assert(memcmp(err.msg, STRNSIZE("duplicate command line option ‘S’")) == 0);
  ill_dtor(&il);

  return EXIT_SUCCESS;
}
