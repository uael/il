#include "fir_lexer.h"

#include "frontend/jay_lexer.h"

void fir_lexer_ctor(fir_lexer_t *this, fir_ctx_t *ctx) {
  fir_lexer_ctor2(this, ctx, ctx->in);
}

void fir_lexer_ctor2(fir_lexer_t *this, fir_ctx_t *ctx, const char *in) {
  assert(this);
  assert(ctx);

  this->ctx = ctx;
  this->filename = in;
  this->lex = jay_lex;
  this->lex_str = jay_lex_str;
}

void fir_lexer_dtor(fir_lexer_t *this) {
  assert(this);
}
