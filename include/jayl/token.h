#ifndef _JAYL_TOKEN_H
#define _JAYL_TOKEN_H

#include "string.h"
#include "type.h"

/**
 * Map token type to corresponding numerical ascii value where possible,
 * and fit the remaining tokens in between.
 */
typedef enum {
  END = 0,
  AUTO = END + 1,
  BREAK,
  CASE,
  CHAR,
  CONST,
  CONTINUE,
  DEFAULT,
  DO,
  DOUBLE,
  NEWLINE = '\n',
  ELSE = NEWLINE + 1,
  ENUM,
  EXTERN,
  FLOAT,
  FOR,
  GOTO,
  IF,
  INT,
  LONG,
  REGISTER,
  RETURN,
  SHORT,
  SIGNED,
  SIZEOF,
  STATIC,
  STRUCT,
  SWITCH,
  TYPEDEF,
  UNION,
  UNSIGNED,
  VOID,
  INLINE,

  NOT = '!',
  VOLATILE = NOT + 1,
  HASH = '#',
  WHILE = HASH + 1,
  MODULO = '%',
  AND = '&',

  OPEN_PAREN = '(',
  CLOSE_PAREN = ')',
  STAR = '*',
  PLUS = '+',
  COMMA = ',',
  MINUS = '-',
  DOT = '.',
  SLASH = '/',

  COLON = ':',
  SEMICOLON = ';',
  LT = '<',
  ASSIGN = '=',
  GT = '>',
  QUESTION = '?',
  DOTS = QUESTION + 1,  /* ... */
  LOGICAL_OR,           /* || */
  LOGICAL_AND,          /* && */
  LEQ,                  /* <= */
  GEQ,                  /* >= */
  EQ,                   /* == */
  NEQ,                  /* != */
  ARROW,                /* -> */
  INCREMENT,            /* ++ */
  DECREMENT,            /* -- */
  LSHIFT,               /* << */
  RSHIFT,               /* >> */
  MUL_ASSIGN,           /* *= */
  DIV_ASSIGN,           /* /= */
  MOD_ASSIGN,           /* %= */
  PLUS_ASSIGN,          /* += */
  MINUS_ASSIGN,         /* -= */
  LSHIFT_ASSIGN,        /* <<= */
  RSHIFT_ASSIGN,        /* >>= */
  AND_ASSIGN,           /* &= */
  XOR_ASSIGN,           /* ^= */
  OR_ASSIGN,            /* |= */
  TOKEN_PASTE,          /* ## */

  OPEN_BRACKET = '[',
  CLOSE_BRACKET = ']',
  XOR = '^',
  OPEN_CURLY = '{',
  OR = '|',
  CLOSE_CURLY = '}',
  NEG = '~',

  /*
   * The remaining tokens do not correspond to any fixed string, and
   * are placed at arbitrary locations.
   */
  NUMBER = 116,
  IDENTIFIER,
  STRING,

  /*
   * Pseudo-token representing parameter substitution in macros. Has
   * an immediate integer value referring to the parameter index.
   */
  PARAM,

  /*
   * Pseudo-token representing empty argument for macro expansion in
   * preprocessor.
   */
  EMPTY_ARG,

  /*
   * Preprocessing token representing a number. Valid strings include
   * a superset of numeric constants in C. A conversion to NUMBER
   * token is done before handed to parser.
   */
  PREP_NUMBER
} jayl_token_type_t;

/**
 * The type jayl_value_t
 *
 * Hold an immediate numeric value. Associated type is used to determine
 * which element is valid.
 */
typedef union {
  unsigned long u;
  signed long i;
  float f;
  double d;
  long double ld;
} jayl_value_t;

/**
 * Try to convert val from to type
 * Fail if incompatible.
 *
 * @param val The val to convert
 * @param from The from type
 * @param to  The to type
 * @return The converted value
 */
jayl_value_t convert(jayl_value_t val, Type from, Type to);

/**
 * The type jayl_token_t
 *
 * Representation of token; used in preprocessing, and interface to
 * parser.
 *
 * Tokens keep track of typed numbers, to capture difference between
 * literals like 1 and 1ul. Type should always correspond to one of the
 * basic integer types.
 */
typedef struct {
  jayl_token_type_t token;
  unsigned int leading_whitespace : 16;
  unsigned int is_expandable : 1;
  unsigned int is_char_literal : 1;
  unsigned int disable_expand : 1;
  Type type;
  union {
    String string;
    jayl_value_t val;
  } d;
} jayl_token_t;

/**
 * Peek lookahead of 1.
 *
 * @return The next token
 */
jayl_token_t peek(void);

/**
 * Peek lookahead of n.
 *
 * @param n The number of token to peek
 * @return The next token
 */
jayl_token_t peekn(int n);

/**
 * Consume and return next token.
 *
 * @return The next token
 */
jayl_token_t next(void);

/**
 * Consume and return next token, or fail of not of expected type.
 *
 * @param token The token to consume
 * @return The next token
 */
jayl_token_t consume(jayl_token_type_t token);

#endif /* _JAYL_TOKEN_H */
