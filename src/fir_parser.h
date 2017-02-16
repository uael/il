#ifndef _JAYL_PARSER_H_
#define _JAYL_PARSER_H_

#include "fir_lexer.h"

typedef struct _fir_parser_t {
  fir_ctx_t *ctx;
  fir_lexer_t *lexer;
  char src_dir[256];
  fir_toks_t toks;

  void (*parse)(struct _fir_parser_t *this, fir_tus_t *translation_units);
  void (*parse_str)(struct _fir_parser_t *this, const char *buffer, fir_tus_t *translation_units);

  fir_tok_t (*peek)(struct _fir_parser_t *this);
  fir_tok_t (*peekn)(struct _fir_parser_t *this, unsigned n);
  fir_tok_t (*next)(struct _fir_parser_t *this);
  fir_tok_t (*consume)(struct _fir_parser_t *this, fir_tok tok);
} fir_parser_t;

void fir_parser_ctor(fir_parser_t *this, fir_ctx_t *ctx);

void fir_parser_ctor2(fir_parser_t *this, fir_ctx_t *ctx, fir_lexer_t *lexer, char *src_dir);

void fir_parser_dtor(fir_parser_t *this);

#endif /* _JAYL_PARSER_H_ */
