#ifndef _JAYL_CONTEXT_H_
#define _JAYL_CONTEXT_H_

#include <stdbool.h>

struct _lexer_t;
struct _parser_t;

typedef struct _ctx_t {
  const char *program;
  const char *in;
  const char *out;
  bool verbose;
  struct _lexer_t *lexer;
  struct _parser_t *parser;
} ctx_t;

void ctx_ctor(ctx_t *this);

void ctx_dtor(ctx_t *this);

#endif /* _JAYL_CONTEXT_H_ */
