#include "parser.h"

#include <assert.h>

#include "jay/jay_parser.h"

#define __peek() P99_VECTOR_FRONT(&this->toks)
#define __next() P99_VECTOR_SHIFT(&this->toks)

static fir_tok_t peek(struct _parser_t *this) {
  return P99_VECTOR_FRONT(&this->toks);
}

static fir_tok_t peekn(struct _parser_t *this, unsigned n) {
  return P99_VECTOR_AT(&this->toks, n);
}

static fir_tok_t next(struct _parser_t *this) {
  return P99_VECTOR_SHIFT(&this->toks);
}

static fir_tok_t consume(struct _parser_t *this, fir_tok tok) {
  if (__peek().kind != tok) { \
      puts("unexpected token"); \
      exit(1); \
    } \
 (void) __next();
  return __peek();
}

void parser_ctor(parser_t *this, jay_ctx_t *ctx, lexer_t *lexer, char *src_dir) {
  assert(this);
  assert(ctx);
  assert(lexer);

  this->ctx = ctx;
  this->lexer = lexer;

  strcpy(this->src_dir, src_dir ? src_dir : ctx->src_dir);

  this->peek = peek;
  this->peekn = peekn;
  this->next = next;
  this->consume = consume;
  this->parse = jay_parse;
  this->parse_str = jay_parse_str;
}

void parser_dtor(parser_t *this) {
  assert(this);
  P99_VECTOR_DESTROY(&this->toks);
}
