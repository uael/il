#ifndef _JAYL_TOKENIZE_H
#define _JAYL_TOKENIZE_H

#include <jayl/token.h>

/**
 * Table is indexed by ASCII value, which is also assigned to
 * corresponding token type. To get a token of a particular type,
 * access basic_token[type].
 */
extern const jayl_token_t basic_token[128];

/**
 * Reconstruct string representation of token.
 *
 * @param token The token to stringify
 * @return The string representation of the token
 */
String jayl_token_str(jayl_token_t token);

/**
 * Transform preprocessing number to actual numeric literal.
 *
 * @param token The token number
 * @return The token as numerical literal
 */
jayl_token_t jayl_token_convert(jayl_token_t token);

/**
 * Parse and return next preprocessing token from given line. Assume
 * comments are removed and line continuations are applied. endptr is
 * set to point to one index past the last character producing the
 * token.
 *
 * Destructively overwrites input buffer for string constants.
 *
 * @param in The in line
 * @param endptr The endptr
 * @return The tokenized input
 */
jayl_token_t jayl_tokenize(char *in, char **endptr);

#endif /* _JAYL_TOKENIZE_H */
