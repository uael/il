#include "jay_ctx.h"

#include "frontend/parser.h"

void ctx_ctor(jay_ctx_t *this, struct _parser_t *parser, struct _be_t *be) {
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

void ctx_dtor(jay_ctx_t *this) {
  assert(this);

  array_clear(&this->prg.types);
  deque_destroy(&this->prg.tus);
}
