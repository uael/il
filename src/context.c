#include "context.h"
#include "lexer.h"
#include "parser.h"

void ctx_ctor(ctx_t *this) {
  this->lexer = malloc(sizeof(lexer_t));
  lexer_ctor(this->lexer, this);

  this->parser = malloc(sizeof(parser_t));
  parser_ctor(this->parser, this);
}

void ctx_dtor(ctx_t *this) {
  lexer_dtor(this->lexer);
  free(this->lexer);

  parser_dtor(this->parser);
  free(this->parser);
}
