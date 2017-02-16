#include "context.h"
#include "lexer.h"
#include "parser.h"

void ctx_ctor(ctx_t *this) {
  char *sep;

  assert(this->in);

  if ((sep = strrchr(this->in, '/'))) {
    strncpy(this->src_dir, this->in, (size_t) (sep - this->in));
  } else {
    strcpy(this->src_dir, ".");
  }

  this->lexer = malloc(sizeof(lexer_t));
  lexer_ctor(this->lexer, this);

  this->parser = malloc(sizeof(fir_parser_t));
  parser_ctor(this->parser, this);
}

void ctx_dtor(ctx_t *this) {
  lexer_dtor(this->lexer);
  free(this->lexer);

  parser_dtor(this->parser);
  free(this->parser);
}
