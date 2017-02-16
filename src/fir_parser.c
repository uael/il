#include "fir_parser.h"

#include <assert.h>

#include "frontend/jay_parser.h"

#define __peek() deque_front(&this->toks)
#define __next() deque_pop_front(&this->toks)

static fir_tok_t peek(struct _fir_parser_t *this) {
  return deque_front(&this->toks);
}

static fir_tok_t peekn(struct _fir_parser_t *this, unsigned n) {
  return deque_get(&this->toks, n);
}

static fir_tok_t next(struct _fir_parser_t *this) {
  return deque_pop_front(&this->toks);
}

static fir_tok_t consume(struct _fir_parser_t *this, fir_tok tok) {
  if (__peek().kind != tok) { \
      puts("unexpected token"); \
      exit(1); \
    } \
    __next();
  return __peek();
}

void fir_parser_ctor(fir_parser_t *this, fir_ctx_t *ctx) {
  fir_parser_ctor2(this, ctx, ctx->lexer, ctx->src_dir);
}

void fir_parser_ctor2(fir_parser_t *this, fir_ctx_t *ctx, fir_lexer_t *lexer, char *src_dir) {
  assert(this);
  assert(ctx);
  assert(lexer);
  assert(src_dir);

  this->ctx = ctx;
  this->lexer = lexer;

  strcpy(this->src_dir, src_dir);

  this->peek = peek;
  this->peekn = peekn;
  this->next = next;
  this->consume = consume;
  this->parse = jay_parse;
  this->parse_str = jay_parse_str;
}

void fir_parser_dtor(fir_parser_t *this) {
  assert(this);
  deque_destroy(&this->toks);
}
