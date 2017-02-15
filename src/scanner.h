#ifndef _JAYL_SCANNER_H_
#define _JAYL_SCANNER_H_

#include <jayl/deque.h>
#include <jayl/ir.h>
#include "context.h"

typedef deque_of(ir_token_t) ir_ts_t;

typedef struct _ir_lexer_t {
  const char *filename;

  ir_ts_t (*lex)(struct _ir_lexer_t *this);
  ir_ts_t (*lex_str)(struct _ir_lexer_t *this, const char *buffer);
} ir_lexer_t;

void ir_lexer_ctor(ir_lexer_t *this, jayl_ctx_t *ctx);

void ir_lexer_dtor(ir_lexer_t *this);

#endif /* _JAYL_SCANNER_H_ */
