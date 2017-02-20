#include "fir_ctx.h"

#include "fir_lexer.h"
#include "fir_parser.h"

void ctx_ctor(fir_ctx_t *this, struct _fir_parser_t *parser, struct _be_t *be) {
  char *sep;

  assert(this);
  assert(this->in);

  if ((sep = strrchr(this->in, '/'))) {
    strncpy(this->src_dir, this->in, (size_t) (sep - this->in));
  } else {
    strcpy(this->src_dir, ".");
  }

  this->parser = parser;
  this->be = be;
}

void ctx_dtor(fir_ctx_t *this) {
  assert(this);

  array_clear(&this->prg.types);
  deque_destroy(&this->prg.tus);
}
