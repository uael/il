#ifndef _JAYL_TOKENIZE_H_
#define _JAYL_TOKENIZE_H_

#include <jayl/ir.h>

/**
 * Table is indexed by ASCII value, which is also assigned to
 * corresponding token type. To get a token of a particular type,
 * access tokens[ir_tok].
 */
extern const ir_token_t tokens[128];

#endif /* _JAYL_TOKENIZE_H_ */
