#ifndef _JAYL_CONTEXT_H_
#define _JAYL_CONTEXT_H_

#include <stdbool.h>

struct _fir_lexer_t;
struct _fir_parser_t;

typedef struct _ctx_t {
  const char *program;
  const char *in;
  const char *out;
  char src_dir[256];
  bool verbose;
  struct _fir_lexer_t *lexer;
  struct _fir_parser_t *parser;
} fir_ctx_t;

void ctx_ctor(fir_ctx_t *this);

void ctx_dtor(fir_ctx_t *this);

#endif /* _JAYL_CONTEXT_H_ */
