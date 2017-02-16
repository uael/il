#ifndef _JAYL_SCANNER_H_
#define _JAYL_SCANNER_H_

#include "fir.h"
#include "fir_ctx.h"

typedef struct _fir_lexer_t {
  const char *filename;
  fir_ctx_t *ctx;

  void (*lex)(struct _fir_lexer_t *this, fir_toks_t *token_stream);
  void (*lex_str)(struct _fir_lexer_t *this, const char *buffer, fir_toks_t *token_stream);
} fir_lexer_t;

void fir_lexer_ctor(fir_lexer_t *this, fir_ctx_t *ctx);

void fir_lexer_ctor2(fir_lexer_t *this, fir_ctx_t *ctx, const char *in);

void fir_lexer_dtor(fir_lexer_t *this);

#endif /* _JAYL_SCANNER_H_ */
