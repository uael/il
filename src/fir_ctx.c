#include "fir_ctx.h"

#include "fir_lexer.h"
#include "fir_parser.h"

void ctx_ctor(fir_ctx_t *this) {
  char *sep;

  assert(this);
  assert(this->in);

  if ((sep = strrchr(this->in, '/'))) {
    strncpy(this->src_dir, this->in, (size_t) (sep - this->in));
  } else {
    strcpy(this->src_dir, ".");
  }

  this->lexer = malloc(sizeof(fir_lexer_t));
  fir_lexer_ctor(this->lexer, this);

  this->parser = malloc(sizeof(fir_parser_t));
  fir_parser_ctor(this->parser, this);
}

void ctx_dtor(fir_ctx_t *this) {
  assert(this);

  fir_lexer_dtor(this->lexer);
  free(this->lexer);

  fir_parser_dtor(this->parser);
  free(this->parser);

  array_clear(&this->prg.types);
  deque_destroy(&this->prg.tus);
}
