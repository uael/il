#ifndef _JAYL_FIR_LEXER_H_
#define _JAYL_FIR_LEXER_H_

#include "fir_ctx.h"

/**
 * The type struct fir_lexer_t.
 */
typedef struct _fir_lexer_t {
  const char *filename;
  fir_ctx_t *ctx;

  /**
   * Lex the this input file into a token stream.
   * @param this The fir_lexer_t instance.
   * @param token_stream Every matched token will be append to
   * this token stream.
   */
  void (*lex)(struct _fir_lexer_t *this, fir_toks_t *token_stream);

  /**
   * Lex a buffer into a token stream.
   * @param this The fir_lexer_t instance.
   * @param buffer The buffer to lex.
   * @param token_streamEvery matched token will be append to
   * this token stream.
   */
  void (*lex_str)(struct _fir_lexer_t *this, const char *buffer, fir_toks_t *token_stream);
} fir_lexer_t;

/**
 * The fir_lexer_t ctor.
 * @param this The fir_lexer_t instance to init.
 * @param ctx The current fir context.
 */
void fir_lexer_ctor(fir_lexer_t *this, fir_ctx_t *ctx);

/**
 * The fir_lexer_t ctor.
 * @param this The fir_lexer_t instance to init.
 * @param ctx The current fir context.
 * @param in The input file to lex.
 */
void fir_lexer_ctor2(fir_lexer_t *this, fir_ctx_t *ctx, const char *in);

/**
 * The fir_lexer_t dtor.
 * @param this The fir_lexer_t instance to destroy.
 */
void fir_lexer_dtor(fir_lexer_t *this);

#endif /* _JAYL_FIR_LEXER_H_ */
