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

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
# include <stdlib.h>
# include <io.h>
# include <process.h> /* for getpid() and the exec..() family */
# include <direct.h> /* for _getcwd() and _chdir() */
# include <sys\types.h>
# include <sys\stat.h>
# ifndef __HAVE_UNISTD
# define __HAVE_UNISTD

# define srandom srand
# define random rand

/* Values for the second argument to access.
   These may be OR'd together.  */
# define R_OK    4       /* Test for read permission.  */
# define W_OK    2       /* Test for write permission.  */
//#define   X_OK    1       /* execute permission - unsupported in windows*/
# define F_OK    0       /* Test for existence.  */

# define access _access
# define dup2 _dup2
# define execve _execve
# define ftruncate _chsize
# define unlink _unlink
# define fileno _fileno
# define getcwd _getcwd
# define chdir _chdir
# define isatty _isatty
# define lseek _lseek
/* read, write, and close are NOT being #defined here, because while there are file handle specific versions for Windows, they probably don't work for sockets. You need to look at your app and consider whether to call e.g. closesocket(). */

# define ssize_t int

# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define STDERR_FILENO 2
/* should be in some equivalent to <sys/types.h> */
typedef __int8            int8_t;
typedef __int16           int16_t;
typedef __int32           int32_t;
typedef __int64           int64_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;
# endif
#else
# include <unistd.h>
#endif

#include <adt/xmalloc.h>
#include <adt/string.h>

#include "opts.h"

#include "util/io.h"

bool il_opts_init(il_opts_t *self, int argc, char **argv) {
  char err[256], *sep, *arg;
  int value;
  static const char *usage =
    BOLD "Usage:" BOLD RED " %s" RESET " [options] file\n"
    "%s compiler options:\n"
    "  -h [ --help ]              print this help message\n"
    "  --echo                     print the content of the input file\n"
    "  --graph                    generate firm graph and dump to file (per \n"
    "                             function)\n"
    "  -v [ --verbose ]           log verbose the compilation process\n"
    "  -S [ --output-assembly ]   write generated assembly to <output>.s\n"
    "  -O [ --optimize ] arg (=2) optimization level 0..5 (default: 2)\n"
    "  -o [ --output ] arg        output file name\n";

  *self = (il_opts_t) {
    .out = "a.out",
    .optimize_lvl = 2,
    .target = IL_BACKEND_FIRM_IA32
  };

  for (int i = 1; i < argc; ++i) {
    arg = argv[i];
    switch (arg[0]) {
      case '-':
        if (isalnum(arg[1])) {
          switch (arg[1]) {
            case 'S':
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                adt_vector_push(self->opts_errs, xstrdup(err));
              } else {
                self->output_asm = true;
              }
              break;
            case 'O':
              if (strlen(arg) > 2) {
                arg += 2;
                if (strlen(arg) > 1 || !isalnum(arg[0])) {
                  sprintf(err, "invalid argument provided to command line option -O"BOLD"‘%s’"RESET, arg);
                  adt_vector_push(self->opts_errs, xstrdup(err));
                } else {
                  value = atoi(arg);
                  if (value < 0 || value > 5) {
                    sprintf(err, "invalid optimize level provided to command line option -O"BOLD"‘%s’"RESET, arg);
                    adt_vector_push(self->opts_errs, xstrdup(err));
                  } else {
                    self->optimize_lvl = value;
                  }
                }
              } else if (i + 1 < argc) {
                arg = argv[++i];
                if (strlen(arg) > 1 || !isalnum(arg[0])) {
                  sprintf(err, "invalid argument provided to command line option -O "BOLD"‘%s’"RESET, arg);
                  adt_vector_push(self->opts_errs, xstrdup(err));
                } else {
                  value = atoi(arg);
                  if (value < 0 || value > 5) {
                    sprintf(err, "invalid optimize level provided to command line option -O "BOLD"‘%s’"RESET, arg);
                    adt_vector_push(self->opts_errs, xstrdup(err));
                  } else {
                    self->optimize_lvl = value;
                  }
                }
              } else {
                if (strlen(arg) > 2) {
                  sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                  adt_vector_push(self->opts_errs, xstrdup(err));
                } else {
                  adt_vector_push(self->opts_errs, xstrdup("command line option argument missing "BOLD"‘-O’"RESET));
                }
              }
              break;
            case 'o':
              if (strlen(arg) > 2) {
                arg += 2;
                self->out = arg;
              } else if (i + 1 < argc) {
                self->out = argv[++i];
              } else {
                if (strlen(arg) > 2) {
                  sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                  adt_vector_push(self->opts_errs, xstrdup(err));
                } else {
                  adt_vector_push(self->opts_errs, xstrdup("command line option argument missing "BOLD"‘-o’"RESET));
                }
              }
              break;
            case 'v':
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                adt_vector_push(self->opts_errs, xstrdup(err));
              } else {
                self->verbose = true;
              }
              break;
            case 'h':
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                adt_vector_push(self->opts_errs, xstrdup(err));
              } else {
                printf(usage, argv[0], argv[0]);
                return false;
              }
              break;
            default:
              sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
              adt_vector_push(self->opts_errs, xstrdup(err));
          }
        } else if (arg[1] == '-') {
          arg += 2;
          if (strcmp(arg, "echo") == 0) {
            self->echo = true;
          } else if (strcmp(arg, "firm-graph") == 0) {
            self->firm_graph = true;
          } else if (strcmp(arg, "output-assembly") == 0) {
            self->output_asm = true;
          } else if (strncmp(arg, "optimize", 8) == 0) {
            if (strlen(arg) > 8) {
              arg += 8;
              if (strlen(arg) > 1 || !isalnum(arg[0])) {
                if (arg[0] == '=') {
                  ++arg;
                  if (strlen(arg) > 1 || !isalnum(arg[0])) {
                    goto optimize_err;
                  } else {
                    goto optimize_match;
                  }
                } else {
                  optimize_err:
                  sprintf(err, "invalid argument provided to command line option --optimize"BOLD"‘%s’"RESET, arg);
                  adt_vector_push(self->opts_errs, xstrdup(err));
                }
              } else {
                optimize_match:
                value = atoi(arg);
                if (value < 0 || value > 5) {
                  sprintf(err, "invalid optimize level provided to command line option --optimize"BOLD"‘%s’"RESET, arg);
                  adt_vector_push(self->opts_errs, xstrdup(err));
                } else {
                  self->optimize_lvl = value;
                }
              }
            } else if (i + 1 < argc) {
              arg = argv[++i];
              if (strlen(arg) > 1 || !isalnum(arg[0])) {
                sprintf(err, "invalid argument provided to command line option --optimize "BOLD"‘%s’"RESET, arg);
                adt_vector_push(self->opts_errs, xstrdup(err));
              } else {
                value = atoi(arg);
                if (value < 0 || value > 5) {
                  sprintf(err, "invalid optimize level provided to command line option --optimize "BOLD"‘%s’"RESET, arg);
                  adt_vector_push(self->opts_errs, xstrdup(err));
                } else {
                  self->optimize_lvl = value;
                }
              }
            } else {
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                adt_vector_push(self->opts_errs, xstrdup(err));
              } else {
                adt_vector_push(self->opts_errs, xstrdup("command line option argument missing "BOLD"‘-o’"RESET));
              }
            }
          } else if (strncmp(arg, "output", 6) == 0) {
            if (strlen(arg) > 6) {
              arg += 6;;
              if (!isalnum(arg[0])) {
                if (arg[0] == '=') {
                  ++arg;
                  if (!isalnum(arg[0])) {
                    goto output_err;
                  } else {
                    goto output_match;
                  }
                } else {
                  output_err:
                  sprintf(err, "invalid argument provided to --output: %s", arg);
                  adt_vector_push(self->opts_errs, xstrdup(err));
                }
              } else {
                output_match:
                self->out = arg;
              }
            } else if (i + 1 < argc) {
              arg = argv[++i];
              if (!isalnum(arg[0])) {
                sprintf(err, "invalid argument provided to --output: %s", arg);
                adt_vector_push(self->opts_errs, xstrdup(err));
              } else {
                self->out = arg;
              }
            } else {
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                adt_vector_push(self->opts_errs, xstrdup(err));
              } else {
                adt_vector_push(self->opts_errs, xstrdup("command line option argument missing "BOLD"‘--output’"RESET));
              }
            }
          } else if (strcmp(arg, "verbose") == 0) {
            self->verbose = true;
          } else if (strcmp(arg, "help") == 0) {
            printf(usage, argv[0], argv[0]);
            return false;
          } else {
            sprintf(err, "unrecognized command line option "BOLD"‘--%s’"RESET, arg);
            adt_vector_push(self->opts_errs, xstrdup(err));
          }
        } else {
          sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
          adt_vector_push(self->opts_errs, xstrdup(err));
        }
        break;
      default:
        if (isalnum(arg[0])) {
          self->in = arg;
        } else {
          sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
          adt_vector_push(self->opts_errs, xstrdup(err));
        }
        break;
    }
  }
  if (self->in && (sep = strrchr(self->in, '/'))) {
    memcpy(self->src_dir, self->in, (size_t) (sep - self->in));
  } else {
    UNUSED char *file = getcwd(self->src_dir, IL_SRC_DIR_MSIZE);
  }
  return true;
}

void il_opts_dtor(il_opts_t *self) {
  char *sep;

  adt_vector_foreach(self->opts_errs, sep) {
    xfree(sep);
  }
  adt_vector_dtor(self->opts_errs);
}
