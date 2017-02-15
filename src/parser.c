#include <jayl/ir.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"

#define _peek() deque_front(&this->toks)
#define _peekn(n) deque_get(&this->toks, n)
#define _next() deque_pop_front(&this->toks)
#define _consume(tok) ({ \
    ir_tok_t t = _next(); \
    if (t.kind != tok) { \
      puts("unexpected token"); \
      exit(1); \
    } \
    t; \
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

ir_prog_t parse(struct _parser_t *this);

ir_prog_t parse_str(struct _parser_t *this, const char *buffer);

void parser_ctor(parser_t *parser, ctx_t *ctx) {
  assert(ctx->lexer);
  parser->lexer = ctx->lexer;
  parser->peek = __peek;
  parser->peekn = __peekn;
  parser->next = __next;
  parser->consume = __consume;
  parser->parse = parse;
  parser->parse_str = parse_str;
}

void parser_dtor(parser_t *parser) {

}

ir_prog_t parse(struct _parser_t *this) {
  ir_prog_t result = {};
  ir_tok_t tok;

  if (!deque_len(&this->toks)) {
    this->toks = this->lexer->lex(this->lexer);
  }

  while ((tok = this->peek(this)).kind != IR_TOK_END) {
    switch (tok.kind) {
      case IR_TOK_INCLUDE:
      case IR_TOK_USE:
      case IR_TOK_NAMESPACE:
      case IR_TOK_IDENTIFIER:
      default:
        printf("token: %s\n", str_raw(tok.d.string));
        this->next(this);
        break;
    }
  }

  return result;
}

ir_prog_t parse_str(struct _parser_t *this, const char *buffer) {
  this->toks = this->lexer->lex_str(this->lexer, buffer);
  return this->parse(this);
}
