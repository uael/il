#ifndef _JAYL_CONTEXT_H_
#define _JAYL_CONTEXT_H_

#include <jayl/array.h>
#include <stdbool.h>

struct _ir_lexer_t;

typedef struct _jayl_ctx_t {
  const char *program;
  const char *in;
  const char *out;
  bool verbose;
  struct _ir_lexer_t *lexer;
} jayl_ctx_t;

void ctx_ctor(jayl_ctx_t *this);

void ctx_dtor(jayl_ctx_t *this);

#endif /* _JAYL_CONTEXT_H_ */
