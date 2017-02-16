#ifndef _JAYL_SCANNER_H_
#define _JAYL_SCANNER_H_

#include <jayl/ir.h>
#include "context.h"

typedef struct _lexer_t {
  const char *filename;
  ctx_t *ctx;

  void (*lex)(struct _lexer_t *this, fir_toks_t *token_stream);
  void (*lex_str)(struct _lexer_t *this, const char *buffer, fir_toks_t *token_stream);
} lexer_t;

void lexer_ctor(lexer_t *this, ctx_t *ctx);

void lexer_ctor2(lexer_t *this, ctx_t *ctx, const char *in);

void lexer_dtor(lexer_t *this);

#endif /* _JAYL_SCANNER_H_ */
