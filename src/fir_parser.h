#ifndef _JAYL_FIR_PARSER_H_
#define _JAYL_FIR_PARSER_H_

#include "fir_lexer.h"

/**
 * The type struct fir_parser_t.
 */
typedef struct _fir_parser_t {
  fir_ctx_t *ctx;
  fir_lexer_t *lexer;
  char src_dir[256];
  fir_toks_t toks;

  /**
   * Peek a token, at the front of the parser token stream.
   * @param this The fir_parser_t instance.
   * @return The current token, at the front of the parser token stream.
   */
  fir_tok_t (*peek)(struct _fir_parser_t *this);

  /**
   * Peek a token, at the front + n of the parser token stream.
   * @param this The fir_parser_t instance.
   * @param n The offset for peek.
   * @return The token at front + n of the parser token stream.
   */
  fir_tok_t (*peekn)(struct _fir_parser_t *this, unsigned n);

  /**
   * Consume and return next token.
   * @param this The fir_parser_t instance.
   * @return The next token of the parser token stream.
   */
  fir_tok_t (*next)(struct _fir_parser_t *this);

  /**
   * Consume and return next token, or fail of not of expected type.
   * @param this The fir_parser_t instance.
   * @param tok The token to consume.
   * @return The next token of the parser token stream.
   */
  fir_tok_t (*consume)(struct _fir_parser_t *this, fir_tok tok);

  /**
   * Parse this instance and append matched translation units to a tu deque.
   * @param this The fir_parser_t instance.
   * @param translation_units Where matched translation units will be appended.
   */
  void (*parse)(struct _fir_parser_t *this, fir_tus_t *translation_units);

  /**
   * Parse a buffer and append matched translation units to a tu deque.
   * @param this The fir_parser_t instance.
   * @param buffer The string to parse.
   * @param translation_units Where matched translation units will be appended.
   */
  void (*parse_str)(struct _fir_parser_t *this, const char *buffer, fir_tus_t *translation_units);
} fir_parser_t;

/**
 * The fir_parser_t ctor.
 * @param this The fir_parser_t instance to init.
 * @param ctx The current fir context instance.
 */
void fir_parser_ctor(fir_parser_t *this, fir_ctx_t *ctx);

/**
 * The fir_parser_t ctor.
 * @param this The fir_parser_t instance to init.
 * @param ctx The current fir context instance.
 * @param lexer The lexer to use on parse.
 * @param src_dir The src directory, where files will be peeked at.
 */
void fir_parser_ctor2(fir_parser_t *this, fir_ctx_t *ctx, fir_lexer_t *lexer, char *src_dir);

/**
 * The fir_parser_t dtor.
 * @param this The fir_parser_t instance to destroy.
 */
void fir_parser_dtor(fir_parser_t *this);

#endif /* _JAYL_FIR_PARSER_H_ */
