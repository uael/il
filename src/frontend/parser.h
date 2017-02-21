#ifndef _JAYL_PARSER_H_
#define _JAYL_PARSER_H_

#include "lexer.h"

/**
 * The type struct parser_t.
 */
typedef struct _parser_t {
  jay_ctx_t *ctx;
  lexer_t *lexer;
  char src_dir[256];
  fir_toks_t toks;

  /**
   * Peek a token, at the front of the parser token stream.
   * @param this The parser_t instance.
   * @return The current token, at the front of the parser token stream.
   */
  fir_tok_t (*peek)(struct _parser_t *this);

  /**
   * Peek a token, at the front + n of the parser token stream.
   * @param this The parser_t instance.
   * @param n The offset for peek.
   * @return The token at front + n of the parser token stream.
   */
  fir_tok_t (*peekn)(struct _parser_t *this, unsigned n);

  /**
   * Consume and return next token.
   * @param this The parser_t instance.
   * @return The next token of the parser token stream.
   */
  fir_tok_t (*next)(struct _parser_t *this);

  /**
   * Consume and return next token, or fail of not of expected type.
   * @param this The parser_t instance.
   * @param tok The token to consume.
   * @return The next token of the parser token stream.
   */
  fir_tok_t (*consume)(struct _parser_t *this, fir_tok tok);

  /**
   * Parse this instance and append matched translation units to a tu deque.
   * @param this The parser_t instance.
   * @param translation_units Where matched translation units will be appended.
   */
  void (*parse)(struct _parser_t *this, fir_tus_t *translation_units);

  /**
   * Parse a buffer and append matched translation units to a tu deque.
   * @param this The parser_t instance.
   * @param buffer The string to parse.
   * @param translation_units Where matched translation units will be appended.
   */
  void (*parse_str)(struct _parser_t *this, const char *buffer, fir_tus_t *translation_units);
} parser_t;

/**
 * The parser_t ctor.
 * @param this The parser_t instance to init.
 * @param ctx The current fir context instance.
 * @param lexer The lexer to use on parse.
 * @param src_dir The src directory, where files will be peeked at.
 */
void parser_ctor(parser_t *this, jay_ctx_t *ctx, lexer_t *lexer, char *src_dir);

/**
 * The parser_t dtor.
 * @param this The parser_t instance to destroy.
 */
void parser_dtor(parser_t *this);

#endif /* _JAYL_PARSER_H_ */
