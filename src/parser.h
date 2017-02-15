#ifndef _JAYL_PARSER_H_
#define _JAYL_PARSER_H_

#include "lexer.h"

typedef deque_of(ir_tu_t) ir_prog_t;

typedef struct _parser_t {
  lexer_t *lexer;
  ir_toks_t toks;

  ir_prog_t (*parse)(struct _parser_t *this);
  ir_prog_t (*parse_str)(struct _parser_t *this, const char *buffer);
  ir_prog_t (*parse_toks)(struct _parser_t *this, ir_toks_t *toks);

  ir_tok_t (*peek)(struct _parser_t *this);
  ir_tok_t (*peekn)(struct _parser_t *this, unsigned n);
  ir_tok_t (*next)(struct _parser_t *this);
  ir_tok_t (*consume)(struct _parser_t *this, ir_tok tok);
} parser_t;

void parser_ctor(parser_t *parser, ctx_t *ctx);

void parser_dtor(parser_t *parser);

#endif /* _JAYL_PARSER_H_ */
