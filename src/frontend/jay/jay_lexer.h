#ifndef _JAYL_JAY_LEXER_H_
#define _JAYL_JAY_LEXER_H_

#include "frontend/lexer.h"

void jay_lex(lexer_t *this, fir_toks_t *token_stream);

void jay_lex_str(lexer_t *this, const char *buffer, fir_toks_t *token_stream);

#endif /* _JAYL_JAY_LEXER_H_ */
