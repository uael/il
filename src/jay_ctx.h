#ifndef _JAYL_JAY_CTX_H_
#define _JAYL_JAY_CTX_H_

#include "fir.h"

struct _parser_t;
struct _be_t;

/**
 * The type struct fir_ctx_t.
 * @brief Contains all program options and ref to lexer and parser.
 */
typedef struct _jay_ctx_t {
  const char *program;
  const char *in;
  const char *out;
  char src_dir[256];
  bool verbose;

  struct _parser_t *parser;
  struct _be_t *be;

  fir_prg_t prg;
} jay_ctx_t;

/**
 * fir_ctx_t ctor.
 * @param this The fir_ctx_t ptr to ctor.
 */
void ctx_ctor(jay_ctx_t *this, struct _parser_t *parser, struct _be_t *be);

/**
 * fir_ctx_t dtor.
 * @param this The fir_ctx_t ptr to ctor.
 */
void ctx_dtor(jay_ctx_t *this);

#endif /* _JAYL_JAY_CTX_H_ */
