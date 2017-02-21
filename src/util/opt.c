#include "opt.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <p99.h>

#define IS_FLAG(opt) ((strlen(opt.rule) == 2) && (opt.rule[0] == '-'))

/**
 * Check if argv matches given option. Argument vector is offset, and
 * not the same as input to the program. Start reading from index 0 of
 * argv.
 *
 * In case of match, invoke callback and return the number of tokens
 * consumed. In case of flag option, return number of characters
 * matched. Otherwise, return 0.
 */
static int match_arg(opt_t opt, int argc, char *argv[], jay_ctx_t *ctx) {
  int i;
  size_t rulelen, arglen;
  char lastchar, flag;
  assert(opt.callback);

  rulelen = strlen(opt.rule);
  arglen = strlen(argv[0]);
  lastchar = opt.rule[rulelen - 1];
  opt.o = opt.rule + 1;
  switch (lastchar) {
    case ':':
      rulelen -= 1;
      if (!strncmp(opt.rule, argv[0], rulelen)) {
        if (arglen == rulelen) {
          if (argc < 2) {
            fprintf(stderr, "Missing argument to %s.\n", opt.rule);
            exit(1);
          }
          opt.v = argv[1];
          opt.callback(&opt, ctx);
          return 2;
        } else {
          assert(arglen > rulelen);
          opt.v = argv[0] + rulelen;
          opt.callback(&opt, ctx);
          return 1;
        }
      }
      break;
    case '=':
      if (!strncmp(opt.rule, argv[0], rulelen)) {
        if (arglen == rulelen) {
          fprintf(stderr, "Missing argument to %s.\n", argv[0]);
          exit(1);
        }
        opt.v = argv[0] + rulelen;
        opt.callback(&opt, ctx);
        return 1;
      }
      break;
    default:
      if (!IS_FLAG(opt)) {
        if (!strncmp(opt.rule, argv[0], rulelen)) {
          opt.v = argv[0];
          opt.callback(&opt, ctx);
          return 1;
        }
      } else {
        flag = opt.rule[1];
        for (i = 1; i < strlen(argv[0]); ++i) {
          if (argv[0][i] == flag) {
            opt.callback(&opt, ctx);
            return 1;
          }
        }
      }
  }

  return 0;
}

/**
 * Matching works by looping through each input token, trying every
 * option in sequence. All non-flag options are tried first, meaning
 * an input like -std is first checked as "-std", then flags -s, -t, -d.
 * All characters in the token must match a flag to be accepted.
 *
 * Unmatched tokens are bubbled up to the last argv position. First
 * token is skipped, as it is assumed to contain program name.
 */
int parse_args(int optc, opt_t *optv, int argc, char *argv[], jay_ctx_t *ctx) {
  char *tmp;
  int i = 1, j, c;

  while (i < argc) {
    c = 0;
    if (*(argv[i]) == '-') {
      for (j = 0; j < optc; ++j) {
        if (!IS_FLAG(optv[j])) {
          c = match_arg(optv[j], argc - i, argv + i, ctx);
          if (c) {
            i += c;
            break;
          }
        }
      }
      if (!c) {
        for (j = 0; j < optc; ++j) {
          if (IS_FLAG(optv[j])) {
            c += match_arg(optv[j], argc - i, argv + i, ctx);
          }
        }
        if (c == strlen(argv[i]) - 1) {
          i += 1;
        } else {
          fprintf(stderr, "Invalid option %s.\n", argv[i]);
          exit(1);
        }
      }
    } else {
      for (j = i + 1; j < argc; j++) {
        tmp = argv[j - 1];
        argv[j - 1] = argv[j];
        argv[j] = tmp;
      }
      argc -= 1;
    }
  }

  return i;
}
