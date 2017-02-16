#ifndef _JAYL_FIR_CTX_H_
#define _JAYL_FIR_CTX_H_

#include <p99.h>

struct _fir_lexer_t;
struct _fir_parser_t;

/**
 * The type struct fir_ctx_t.
 * @brief Contains all program options and ref to lexer and parser.
 */
typedef struct _fir_ctx_t {
  const char *program;
  const char *in;
  const char *out;
  char src_dir[256];
  bool verbose;

  struct _fir_lexer_t *lexer;
  struct _fir_parser_t *parser;
} fir_ctx_t;

/**
 * fir_ctx_t ctor.
 * @param this The fir_ctx_t ptr to ctor.
 */
void ctx_ctor(fir_ctx_t *this);

/**
 * fir_ctx_t dtor.
 * @param this The fir_ctx_t ptr to ctor.
 */
void ctx_dtor(fir_ctx_t *this);

#endif /* _JAYL_FIR_CTX_H_ */
