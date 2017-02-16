#ifndef _JAYL_PARSER_H_
#define _JAYL_PARSER_H_

#include "lexer.h"

typedef deque_of(ir_tu_t) fir_tus_t;

typedef struct _parser_t {
  ctx_t *ctx;
  lexer_t *lexer;
  char src_dir[256];
  fir_toks_t toks;

  void (*parse)(struct _parser_t *this, fir_tus_t *translation_units);
  void (*parse_str)(struct _parser_t *this, const char *buffer, fir_tus_t *translation_units);

  ir_tok_t (*peek)(struct _parser_t *this);
  ir_tok_t (*peekn)(struct _parser_t *this, unsigned n);
  ir_tok_t (*next)(struct _parser_t *this);
  ir_tok_t (*consume)(struct _parser_t *this, ir_tok tok);
} fir_parser_t;

void parser_ctor(fir_parser_t *this, ctx_t *ctx);

void parser_ctor2(fir_parser_t *this, ctx_t *ctx, lexer_t *lexer, char *src_dir);

void parser_dtor(fir_parser_t *this);

#endif /* _JAYL_PARSER_H_ */
