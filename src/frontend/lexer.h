#ifndef _JAYL_FIR_LEXER_H_
#define _JAYL_FIR_LEXER_H_

#include "jay_ctx.h"

/**
 * The type struct lexer_t.
 */
typedef struct _lexer_t {
  const char *filename;
  jay_ctx_t *ctx;

  /**
   * Lex the this input file into a token stream.
   * @param this The lexer_t instance.
   * @param token_stream Every matched token will be append to
   * this token stream.
   */
  void (*lex)(struct _lexer_t *this, fir_toks_t *token_stream);

  /**
   * Lex a buffer into a token stream.
   * @param this The lexer_t instance.
   * @param buffer The buffer to lex.
   * @param token_streamEvery matched token will be append to
   * this token stream.
   */
  void (*lex_str)(struct _lexer_t *this, const char *buffer, fir_toks_t *token_stream);
} lexer_t;

/**
 * The lexer_t ctor.
 * @param this The lexer_t instance to init.
 * @param ctx The current fir context.
 * @param in The input file to lex.
 */
void lexer_ctor(lexer_t *this, jay_ctx_t *ctx, const char *in);

/**
 * The lexer_t dtor.
 * @param this The lexer_t instance to destroy.
 */
void lexer_dtor(lexer_t *this);

#endif /* _JAYL_FIR_LEXER_H_ */
