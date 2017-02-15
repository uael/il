#ifndef _JAYL_OPT_H_
#define _JAYL_OPT_H_

#include <jayl/hash.h>
#include "context.h"

/**
 * Specify command line argument pattern as a rule template with an
 * associated callback, which is invoked on match.
 *
 *  "-S"        Flag, which can match in conjunction with other flags.
 *              Matches -Sv if both -S and -v are flag options.
 *  "--help"    Toggle option. Like flags, but must match exactly as a
 *              single token.
 *  "-I:"       Option with argument. The next token, or suffix of
 *              this token, is passed as argument to callback. Matches
 *              both -Ifoo and -I foo.
 *  "-std="     Option with argument, but only accepting parameter as
 *              direct suffix. Matches -std=c89, but not -std= c89.
 */
typedef struct _opt_t {
  const char *rule;

  void (*callback)(struct _opt_t *this, jayl_ctx_t *ctx);

  const char *o, *v;
} opt_t;

/**
 * Parse command line arguments according to option specification, and
 * return the number of tokens consumed in the process. Reshuffle argv
 * to have non-matching tokens at the end.
 */
int parse_args(int optc, opt_t *optv, int argc, char *argv[], jayl_ctx_t *ctx);

#endif /* _JAYL_OPT_H_ */
