#include <jayl/ir.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.h"
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

static inline ir_tok_t __peek(struct _parser_t *this) {
  return _peek();
}

static inline ir_tok_t __peekn(struct _parser_t *this, unsigned n) {
  return _peekn(n);
}

static inline ir_tok_t __next(struct _parser_t *this) {
  return _next();
}

static inline ir_tok_t __consume(struct _parser_t *this, ir_tok tok) {
  return _consume(tok);
}

void parser_ctor(fir_parser_t *this, ctx_t *ctx) {
  assert(ctx->lexer);
  parser_ctor2(this, ctx, ctx->lexer, ctx->src_dir);
}

void parser_ctor2(fir_parser_t *this, ctx_t *ctx, lexer_t *lexer, char *src_dir) {
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

void parser_dtor(fir_parser_t *parser) {

}
