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

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <adt/xmalloc.h>
#include <adt/string.h>

#include "opts.h"
#include "util/io.h"

void jl_opts_parse(jl_opts_t *self, int argc, char *argv[]) {
  jl_vector_of(char *) errs = {0};
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

  *self = (jl_opts_t) {
    .out = "a.out",
    .optimize_lvl = 2,
    .target = JL_BACKEND_FIRM_IA32
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
                jl_vector_push(errs, xstrdup(err));
              } else {
                self->output_asm = true;
              }
              break;
            case 'O':
              if (strlen(arg) > 2) {
                strncpy(arg, arg+2, strlen(arg));
                if (strlen(arg) > 1 || !isalnum(arg[0])) {
                  sprintf(err, "invalid argument provided to command line option -O"BOLD"‘%s’"RESET, arg);
                  jl_vector_push(errs, xstrdup(err));
                } else {
                  value = atoi(arg);
                  if (value < 0 || value > 5) {
                    sprintf(err, "invalid optimize level provided to command line option -O"BOLD"‘%s’"RESET, arg);
                    jl_vector_push(errs, xstrdup(err));
                  } else {
                    self->optimize_lvl = value;
                  }
                }
              } else if (i + 1 < argc) {
                arg = argv[++i];
                if (strlen(arg) > 1 || !isalnum(arg[0])) {
                  sprintf(err, "invalid argument provided to command line option -O "BOLD"‘%s’"RESET, arg);
                  jl_vector_push(errs, xstrdup(err));
                } else {
                  value = atoi(arg);
                  if (value < 0 || value > 5) {
                    sprintf(err, "invalid optimize level provided to command line option -O "BOLD"‘%s’"RESET, arg);
                    jl_vector_push(errs, xstrdup(err));
                  } else {
                    self->optimize_lvl = value;
                  }
                }
              } else {
                if (strlen(arg) > 2) {
                  sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                  jl_vector_push(errs, xstrdup(err));
                } else {
                  jl_vector_push(errs, xstrdup("command line option argument missing "BOLD"‘-O’"RESET));
                }
              }
              break;
            case 'o':
              if (strlen(arg) > 2) {
                strncpy(arg, arg+2, strlen(arg));
                self->out = xstrdup(arg);
              } else if (i + 1 < argc) {
                self->out = argv[++i];
              } else {
                if (strlen(arg) > 2) {
                  sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                  jl_vector_push(errs, xstrdup(err));
                } else {
                  jl_vector_push(errs, xstrdup("command line option argument missing "BOLD"‘-o’"RESET));
                }
              }
              break;
            case 'v':
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                jl_vector_push(errs, xstrdup(err));
              } else {
                self->verbose = true;
              }
              break;
            case 'h':
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                jl_vector_push(errs, xstrdup(err));
              } else {
                printf(usage, argv[0], argv[0]);
                goto exit_success;
              }
              break;
            default:
              sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
              jl_vector_push(errs, xstrdup(err));
          }
        } else if (arg[1] == '-') {
          strncpy(arg, arg+2, strlen(arg));
          if (strcmp(arg, "echo") == 0) {
            self->echo = true;
          } else if (strcmp(arg, "firm-graph") == 0) {
            self->firm_graph = true;
          } else if (strcmp(arg, "output-assembly") == 0) {
            self->output_asm = true;
          } else if (strncmp(arg, "optimize", 8) == 0) {
            if (strlen(arg) > 8) {
              strncpy(arg, arg+8, strlen(arg));
              if (strlen(arg) > 1 || !isalnum(arg[0])) {
                if (arg[0] == '=') {
                  strncpy(arg, arg+1, strlen(arg));
                  if (strlen(arg) > 1 || !isalnum(arg[0])) {
                    goto optimize_err;
                  } else {
                    goto optimize_match;
                  }
                } else {
                  optimize_err:
                  sprintf(err, "invalid argument provided to command line option --optimize"BOLD"‘%s’"RESET, arg);
                  jl_vector_push(errs, xstrdup(err));
                }
              } else {
                optimize_match:
                value = atoi(arg);
                if (value < 0 || value > 5) {
                  sprintf(err, "invalid optimize level provided to command line option --optimize"BOLD"‘%s’"RESET, arg);
                  jl_vector_push(errs, xstrdup(err));
                } else {
                  self->optimize_lvl = value;
                }
              }
            } else if (i + 1 < argc) {
              arg = argv[++i];
              if (strlen(arg) > 1 || !isalnum(arg[0])) {
                sprintf(err, "invalid argument provided to command line option --optimize "BOLD"‘%s’"RESET, arg);
                jl_vector_push(errs, xstrdup(err));
              } else {
                value = atoi(arg);
                if (value < 0 || value > 5) {
                  sprintf(err, "invalid optimize level provided to command line option --optimize "BOLD"‘%s’"RESET, arg);
                  jl_vector_push(errs, xstrdup(err));
                } else {
                  self->optimize_lvl = value;
                }
              }
            } else {
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                jl_vector_push(errs, xstrdup(err));
              } else {
                jl_vector_push(errs, xstrdup("command line option argument missing "BOLD"‘-o’"RESET));
              }
            }
          } else if (strncmp(arg, "output", 6) == 0) {
            if (strlen(arg) > 6) {
              strncpy(arg, arg+6, strlen(arg));
              if (!isalnum(arg[0])) {
                if (arg[0] == '=') {
                  strncpy(arg, arg+1, strlen(arg));
                  if (!isalnum(arg[0])) {
                    goto output_err;
                  } else {
                    goto output_match;
                  }
                } else {
                  output_err:
                  sprintf(err, "invalid argument provided to --output: %s", arg);
                  jl_vector_push(errs, xstrdup(err));
                }
              } else {
                output_match:
                self->out = arg;
              }
            } else if (i + 1 < argc) {
              arg = argv[++i];
              if (!isalnum(arg[0])) {
                sprintf(err, "invalid argument provided to --output: %s", arg);
                jl_vector_push(errs, xstrdup(err));
              } else {
                self->out = arg;
              }
            } else {
              if (strlen(arg) > 2) {
                sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
                jl_vector_push(errs, xstrdup(err));
              } else {
                jl_vector_push(errs, xstrdup("command line option argument missing "BOLD"‘--output’"RESET));
              }
            }
          } else if (strcmp(arg, "verbose") == 0) {
            self->verbose = true;
          } else if (strcmp(arg, "help") == 0) {
            printf(usage, argv[0], argv[0]);
            goto exit_success;
          } else {
            sprintf(err, "unrecognized command line option "BOLD"‘--%s’"RESET, arg);
            jl_vector_push(errs, xstrdup(err));
          }
        } else {
          sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
          jl_vector_push(errs, xstrdup(err));
        }
        break;
      default:
        if (isalnum(arg[0])) {
          self->in = arg;
        } else {
          sprintf(err, "unrecognized command line option "BOLD"‘%s’"RESET, arg);
          jl_vector_push(errs, xstrdup(err));
        }
        break;
    }
  }

  if (!self->in) {
    printf(BOLD "%s: " BOLD RED "fatal error: " RESET "no input files\n", argv[0]);
    goto exit_failure;
  }

  if (jl_vector_size(errs)) {
    printf(BOLD "%s: " BOLD RED "error: " RESET "%s\n", argv[0], jl_vector_at(errs, 0));
    goto exit_failure;
  }

  if ((sep = strrchr(self->in, '/'))) {
    strncpy(self->src_dir, self->in, (size_t) (sep - self->in));
  } else {
    getcwd(self->src_dir, JL_SRC_DIR_MSIZE);
  }

  return;

  exit_success:
  jl_vector_foreach(errs, sep) {
    free(sep);
  }
  jl_vector_dtor(errs);
  exit(EXIT_SUCCESS);

  exit_failure:
  jl_vector_foreach(errs, sep) {
    free(sep);
  }
  jl_vector_dtor(errs);
  exit(EXIT_FAILURE);
}
