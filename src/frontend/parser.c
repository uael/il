#include "parser.h"

#include <assert.h>

#include "jay/jay_parser.h"

#define __peek() deque_front(&this->toks)
#define __next() deque_pop_front(&this->toks)

static fir_tok_t peek(struct _parser_t *this) {
  return deque_front(&this->toks);
}

static fir_tok_t peekn(struct _parser_t *this, unsigned n) {
  return deque_get(&this->toks, n);
}

static fir_tok_t next(struct _parser_t *this) {
  return deque_pop_front(&this->toks);
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
  deque_destroy(&this->toks);
}
