#include "fir_parser.h"

#include <stdlib.h>
#include <assert.h>

#include "frontend/jayl_parser.h"

#define _peek() deque_front(&this->toks)
#define _peekn(n) deque_get(&this->toks, n)
#define _next() deque_pop_front(&this->toks)
#define _consume(tok) ({ \
    if (_peek().kind != tok) { \
      puts("unexpected token"); \
      exit(1); \
    } \
    _next(); _peek(); \
  })

static inline fir_tok_t __peek(struct _fir_parser_t *this) {
  return _peek();
}

static inline fir_tok_t __peekn(struct _fir_parser_t *this, unsigned n) {
  return _peekn(n);
}

static inline fir_tok_t __next(struct _fir_parser_t *this) {
  return _next();
}

static inline fir_tok_t __consume(struct _fir_parser_t *this, fir_tok tok) {
  return _consume(tok);
}

void fir_parser_ctor(fir_parser_t *this, fir_ctx_t *ctx) {
  assert(ctx->lexer);
  fir_parser_ctor2(this, ctx, ctx->lexer, ctx->src_dir);
}

void fir_parser_ctor2(fir_parser_t *this, fir_ctx_t *ctx, fir_lexer_t *lexer, char *src_dir) {
  this->ctx = ctx;
  this->lexer = lexer;

  strcpy(this->src_dir, src_dir);

  this->peek = __peek;
  this->peekn = __peekn;
  this->next = __next;
  this->consume = __consume;
  this->parse = jayl_parse;
  this->parse_str = jayl_parse_str;
}

void fir_parser_dtor(fir_parser_t *parser) {

}
