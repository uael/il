#include "lexer.h"

#include "jay/jay_lexer.h"

void lexer_ctor(lexer_t *this, jay_ctx_t *ctx, const char *in) {
  assert(this);
  assert(ctx);

  this->ctx = ctx;
  this->filename = in ? in : ctx->in;
  this->lex = jay_lex;
  this->lex_str = jay_lex_str;
}

void lexer_dtor(lexer_t *this) {
  assert(this);
}
