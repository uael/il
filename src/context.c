#include "context.h"
#include "scanner.h"

void ctx_ctor(jayl_ctx_t *this) {
  this->lexer = malloc(sizeof(ir_lexer_t));
  ir_lexer_ctor(this->lexer, this);
}

void ctx_dtor(jayl_ctx_t *this) {
  ir_lexer_dtor(this->lexer);
  free(this->lexer);
}
