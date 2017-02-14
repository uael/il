#ifndef _JAYL_SCANNER_H_
#define _JAYL_SCANNER_H_

#include <jayl/deque.h>
#include <jayl/ir.h>

typedef deque_of(ir_token_t) ir_ts_t;

/**
 *
 * @param buffer
 * @return
 */
ir_ts_t lex(const char *buffer, const char *filename);

/**
 *
 * @param filename
 * @return
 */
ir_ts_t lex_file(const char *filename);

#endif /* _JAYL_SCANNER_H_ */
