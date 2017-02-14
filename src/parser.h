#ifndef _JAYL_PARSER_H_
#define _JAYL_PARSER_H_

#include "scanner.h"

typedef array_of(ir_tu_t) ir_prog_t;

///**
// * Parse the token stream into an array of translation unit.
// * @param ts The token stream.
// * @return An array of translation unit.
// */
//ir_prog_t parse(ir_ts_t *ts);
//
///**
// * Peek lookahead of 1.
// * @param ts The token stream.
// * @return The next token.
// */
//ir_token_t peek(ir_ts_t *ts);
//
///**
// * Peek lookahead of n.
// * @param ts The token stream.
// * @param n The number of token to peek.
// * @return The next token.
// */
//ir_token_t peekn(ir_ts_t *ts, int n);
//
///**
// * Consume and return next token.
// * @param ts The token stream.
// * @return The next token.
// */
//ir_token_t next(ir_ts_t *ts);
//
///**
// * Skip the next token.
// * @param ts The token stream.
// */
//void skip(ir_ts_t *ts);
//
///**
// * Consume and return next token, or fail of not of expected type.
// * @param ts The token stream.
// * @param token The token to consume.
// * @return The next token.
// */
//ir_token_t consume(ir_ts_t *ts, ir_tok tok);

#endif /* _JAYL_PARSER_H_ */
