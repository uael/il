#ifndef _JAYL_SCANNER_H_
#define _JAYL_SCANNER_H_

#include <jayl/ir.h>
#include "context.h"

typedef struct _lexer_t {
  const char *filename;

  ir_toks_t (*lex)(struct _lexer_t *this);
  ir_toks_t (*lex_str)(struct _lexer_t *this, const char *buffer);
} lexer_t;

void lexer_ctor(lexer_t *this, ctx_t *ctx);

void lexer_dtor(lexer_t *this);

#endif /* _JAYL_SCANNER_H_ */
