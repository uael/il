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

#ifndef  ILL_H__
# define ILL_H__

#include "u/vector.h"
#include "il/adt/hash.h"

typedef enum ill_errlvl ill_errlvl_t;
typedef enum ill_errkind ill_errkind_t;

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
typedef struct ill_opt ill_opt_t;
typedef struct ill_err ill_err_t;

typedef int (*ill_optcb_t)(void *app, __const char *val);

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

#define ILL_EVENT_SUPER(T_sender) \
  uint8_t kind; \
  T_sender *sender; \
  int (*callback)(ill_event_t *self, void *arg); \
  void (*dtor)(ill_event_t *self)

struct ill_event {
  ILL_EVENT_SUPER(void);
};

struct ill_src {
  char filename[FILENAME_MAX + 1];
  char path[PATH_MAX + 1];
  int fd;
  char *buffer;
  size_t length;
};

void
ill_src_init(ill_src_t *self, __const char *filename) {
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

struct ill_loc {
  ill_src_t *source;
  uint32_t lineno;
  uint32_t colno;
  uint32_t position;
};

struct ill_token {
  unsigned int kind : 4;
  char type;
  __const char *name, *value;
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

#define ill_lexer_attach(lexer, event) ill_lexer_pattach((ill_lexer_t *)(lexer), (ill_event_t *) &(event))

void
ill_lexer_pattach(ill_lexer_t *lexer, ill_event_t *event) {
  event->sender = lexer;
  uvec_push(lexer->events, *event);
}

int
ill_lexer_trigger(ill_lexer_t *self, uint8_t ekind, void *arg) {
  ill_event_t event;
  int r = 0;

  foreach(event, in(T_uvec, self->events)) {
    if (event.kind == ekind) {
      if ((r = event.callback(&event, arg)) != 0) {
        return r;
      }
    }
  }

  return r;
}

struct ill_scope {
  int dummy;
};

#define ILL_FE_SUPER \
  ill_t *il; \
  ill_fe_t *parent; \
  uvec_of(ill_fe_t) childs; \
  uvec_of(ill_event_t) events; \
  uvec_of(ill_err_t) errs; \
  ill_scope_t *scope; \
  uvec_of(ill_src_t) sources; \
  void (*parse)(ill_fe_t *self)

struct ill_fe {
  ILL_FE_SUPER;
};

#define ILL_BE_SUPER \
  ill_t *il; \
  ill_be_t *parent; \
  uvec_of(ill_be_t) childs; \
  uvec_of(ill_event_t) events; \
  uvec_of(ill_err_t) errs; \
  void (*emit)(ill_be_t *self)

struct ill_be {
  ILL_BE_SUPER;
};

struct ill_tu {
  ill_lloc_t lloc;
  int dummy;
};

struct ill_opt {
  char f, *lf, *help;
  ill_optcb_t callback;
  bool kval, global, match;
};

KHASH_DECLARE(ill_optmap, __const char *, ill_opt_t)
KHASH_DECLARE(ill_optmap_sc, khint32_t, ill_opt_t *)

struct ill_opts {
  __const char *program;
  ill_optmap_t map;
  ill_optmap_sc_t map_sc;
  uvec_of(ill_err_t) errs;
  ill_optcb_t callback;
};

struct ill {
  ill_fe_t *fe;
  ill_be_t *be;
  uvec_of(ill_tu_t) tus;
};

KHASH_MAP_IMPL_STR(ill_optmap, ill_opt_t)
KHASH_MAP_IMPL_INT(ill_optmap_sc, ill_opt_t *)

void
ill_err_dtor(ill_err_t *self) {
  free(self->msg);
  if (self->prev) {
    ill_err_dtor(self->prev);
  }
}

void
ill_opt_dtor(ill_opt_t *self) {}

void
ill_opts_init(ill_opts_t *self, ill_opt_t *opts, ill_optcb_t callback) {
  int r;
  unsigned it;

  *self = (ill_opts_t) {
    .callback = callback
  };
  if (opts) {
    while (opts->lf) {
      ill_opt_t *opt = opts++;

      it = kh_put(ill_optmap, &self->map, opt->lf, &r);
      if (r != 0) {
        kh_value(&self->map, it) = *opt;
        opt = self->map.vals + it;
      }
      if (opt->f) {
        it = kh_put(ill_optmap_sc, &self->map_sc, (uint32_t) opt->f, &r);
        if (r != 0) {
          kh_value(&self->map_sc, it) = opt;
        }
      }
    }
  }
}

void
ill_opts_dtor(ill_opts_t *self) {
  ill_err_t err;
  ill_opt_t opt;

  if (ds_size(self->errs)) {
    foreach(err, in(T_uvec, self->errs)) {
      ill_err_dtor(&err);
    }
  }
  uvec_dtor(self->errs);
  kh_foreach_value(&self->map, opt, {
    ill_opt_dtor(&opt);
  });
  kh_destroy(ill_optmap, &self->map);
  kh_destroy(ill_optmap_sc, &self->map_sc);
}

ill_opt_t *
ill_opt(ill_opts_t *opts, char id) {
  unsigned it;

  it = kh_get(ill_optmap_sc, &opts->map_sc, (uint32_t) id);
  if (it == kh_end(&opts->map_sc)) {
    return NULL;
  }
  return kh_value(&opts->map_sc, it);
}

ill_opt_t *
ill_lopt(ill_opts_t *opts, __const char *id) {
  unsigned it;

  it = kh_get(ill_optmap, &opts->map, id);
  if (it == kh_end(&opts->map)) {
    return NULL;
  }
  return &kh_value(&opts->map, it);
}

int
ill_opts_parse(ill_opts_t *self, void *app_ptr, int argc, char **argv) {
  char *arg, key, *lkey, *val, errmsg[UINT8_MAX];
  ill_opt_t *opt;
  int i;

  if (argc) {
    if ((self->program = strrchr(argv[0], '/'))) {
      ++self->program;
    } else {
      self->program = argv[0];
    }
    for (i = 1; i < argc; ++i) {
      arg = argv[i];
      val = nullptr;
      if (*arg == '-') {
        if (*(arg + 1) == '-') {
          opt = ill_lopt(self, lkey = arg + 2);
          if (opt == nullptr) {
            sprintf(errmsg, "unrecognized command line option ‘%s’", lkey);
            goto opts_warning;
          }
          if (opt->kval) {
            if (i < argc - 1) {
              val = argv[++i];
            } else {
              sprintf(errmsg, "missing argument for command line option ‘%s’", lkey);
              goto opts_error;
            }
            if (!opt->global && opt->match) {
              sprintf(errmsg, "duplicate value for command line option ‘%s’: ‘%s’", lkey, val);
              goto opts_warning;
            }
          } else if (!opt->global && opt->match) {
            sprintf(errmsg, "duplicate command line option ‘%s’", lkey);
            goto opts_warning;
          }
        } else {
          opt = ill_opt(self, key = arg[1]);
          if (opt == nullptr) {
            sprintf(errmsg, "unrecognized command line option ‘%c’", key);
            goto opts_warning;
          }
          if (opt->kval) {
            if (arg[2] != '\0') {
              val = arg + 2;
            } else if (i < argc - 1) {
              val = argv[++i];
            } else {
              sprintf(errmsg, "missing argument for command line option ‘%c’", key);
              goto opts_error;
            }
            if (!opt->global && opt->match) {
              sprintf(errmsg, "duplicate value for command line option ‘%c’: ‘%s’", key, val);
              goto opts_warning;
            }
          } else if (arg[2] != '\0') {
            sprintf(errmsg, "unrecognized command line option ‘%c’", key);
            goto opts_warning;
          } else if (!opt->global && opt->match) {
            sprintf(errmsg, "duplicate command line option ‘%c’", key);
            goto opts_warning;
          }
        }
        if (opt->callback == nullptr || opt->callback(app_ptr, val) == 0) {
          opt->match = true;
        }
      } else if (self->callback && (self->callback(app_ptr, arg) != 0)) {
        sprintf(errmsg, "invalid command line argument ‘%s’", arg);
        goto opts_error;
      }
      continue;
        opts_warning:
      uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_WARNING, strdup(errmsg)}));
      continue;
        opts_error:
      uvec_push(self->errs, ((ill_err_t) {IL_ERRKIND_OPTS, IL_ERRLVL_ERROR, strdup(errmsg)}));
    }
  }
  return ds_size(self->errs) > 0 ? 1 : 0;
}

int
ill_push_input(ill_t *self, __const char *filename) {
  ill_src_t src = {0};

  ill_src_init(&src, filename);
  uvec_push(self->fe->sources, src);
  return 0;
}

void
ill_init(ill_t *self, ill_fe_t *fe, ill_be_t *be) {
  *self = (ill_t) {fe, be};
}

void
ill_dtor(UNUSED ill_t *self) {

}

#endif /* ILL_H__ */
