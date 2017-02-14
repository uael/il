#include "tokenize.h"

/**
 * Static initializer for token. Only works with string representation
 * that can fit inline.
 */
#define EMPTY {(ir_tok) 0}
#define TOK(t, s) {(t), 0, 0, 0, 0, {(ir_type) 0}, SHORT_STRING_INIT(s), {NULL}}
#define IDN(t, s) {(t), 0, 1, 0, 0, {(ir_type) 0}, SHORT_STRING_INIT(s), {NULL}}

const ir_token_t tokens[] = {
  /* 0x00 */
  TOK(IR_TOK_END, "$"),
  IDN(IR_TOK_AUTO, "auto"),
  IDN(IR_TOK_BREAK, "break"),
  IDN(IR_TOK_CASE, "case"),
  IDN(IR_TOK_CHAR, "char"),
  IDN(IR_TOK_CONST, "const"),
  IDN(IR_TOK_CONTINUE, "continue"),
  IDN(IR_TOK_DEFAULT, "default"),

  /* 0x08 */
  IDN(IR_TOK_DO, "do"),
  IDN(IR_TOK_DOUBLE, "double"),
  TOK(IR_TOK_EOL, "\n"),
  IDN(IR_TOK_ELSE, "else"),
  IDN(IR_TOK_ENUM, "enum"),
  IDN(IR_TOK_EXTERN, "extern"),
  IDN(IR_TOK_FLOAT, "float"),
  IDN(IR_TOK_FOR, "for"),

  /* 0x10 */
  IDN(IR_TOK_GOTO, "goto"),
  IDN(IR_TOK_IF, "if"),
  IDN(IR_TOK_INT, "int"),
  IDN(IR_TOK_LONG, "long"),
  IDN(IR_TOK_REGISTER, "register"),
  IDN(IR_TOK_RETURN, "return"),
  IDN(IR_TOK_SHORT, "short"),
  IDN(IR_TOK_SIGNED, "signed"),

  /* 0x18 */
  IDN(IR_TOK_SIZEOF, "sizeof"),
  IDN(IR_TOK_STATIC, "static"),
  IDN(IR_TOK_STRUCT, "struct"),
  IDN(IR_TOK_SWITCH, "switch"),
  IDN(IR_TOK_TYPEDEF, "typedef"),
  IDN(IR_TOK_UNION, "union"),
  IDN(IR_TOK_UNSIGNED, "unsigned"),
  IDN(IR_TOK_VOID, "void"),

  /* 0x20 */
  IDN(IR_TOK_INLINE, "inline"),
  TOK(IR_TOK_NOT, "!"),
  IDN(IR_TOK_VOLATILE, "volatile"),
  TOK(IR_TOK_HASH, "#"),
  IDN(IR_TOK_WHILE, "while"),
  TOK(IR_TOK_MODULO, "%"),
  TOK(IR_TOK_AND, "&"),
  IDN(IR_TOK_INCLUDE, "include"),

  /* 0x28 */
  TOK(IR_TOK_OPEN_PAREN, "("),
  TOK(IR_TOK_CLOSE_PAREN, ")"),
  TOK(IR_TOK_STAR, "*"),
  TOK(IR_TOK_PLUS, "+"),
  TOK(IR_TOK_COMMA, ","),
  TOK(IR_TOK_MINUS, "-"),
  TOK(IR_TOK_DOT, "."),
  TOK(IR_TOK_SLASH, "/"),

  /* 0x30 */
  IDN(IR_TOK_USE, "use"),
  TOK(IR_TOK_DCOLON, "::"),
  TOK(IR_TOK_DARROW, "=>"),
  TOK(IR_TOK_DQUESTION, "??"),
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,

  /* 0x38 */
  EMPTY,
  EMPTY,
  TOK(IR_TOK_COLON, ":"),
  TOK(IR_TOK_SEMICOLON, ";"),
  TOK(IR_TOK_LT, "<"),
  TOK(IR_TOK_ASSIGN, "="),
  TOK(IR_TOK_GT, ">"),
  TOK(IR_TOK_QUESTION, "?"),

  /* 0x40 */
  TOK(IR_TOK_DOTS, "..."),
  TOK(IR_TOK_LOGICAL_OR, "||"),
  TOK(IR_TOK_LOGICAL_AND, "&&"),
  TOK(IR_TOK_LEQ, "<="),
  TOK(IR_TOK_GEQ, ">="),
  TOK(IR_TOK_EQ, "=="),
  TOK(IR_TOK_NEQ, "!="),
  TOK(IR_TOK_ARROW, "->"),

  /* 0x48 */
  TOK(IR_TOK_INCREMENT, "++"),
  TOK(IR_TOK_DECREMENT, "--"),
  TOK(IR_TOK_LSHIFT, "<<"),
  TOK(IR_TOK_RSHIFT, ">>"),
  TOK(IR_TOK_MUL_ASSIGN, "*="),
  TOK(IR_TOK_DIV_ASSIGN, "/="),
  TOK(IR_TOK_MOD_ASSIGN, "%="),
  TOK(IR_TOK_PLUS_ASSIGN, "+="),

  /* 0x50 */
  TOK(IR_TOK_MINUS_ASSIGN, "-="),
  TOK(IR_TOK_LSHIFT_ASSIGN, "<<="),
  TOK(IR_TOK_RSHIFT_ASSIGN, ">>="),
  TOK(IR_TOK_AND_ASSIGN, "&="),
  TOK(IR_TOK_XOR_ASSIGN, "^="),
  TOK(IR_TOK_OR_ASSIGN, "|="),
  TOK(IR_TOK_TOKEN_PASTE, "##"),
  EMPTY,

  /* 0x58 */
  EMPTY,
  EMPTY,
  EMPTY,
  TOK(IR_TOK_OPEN_BRACKET, "["),
  EMPTY,
  TOK(IR_TOK_CLOSE_BRACKET, "]"),
  TOK(IR_TOK_XOR, "^"),
  EMPTY,

  /* 0x60 */
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,

  /* 0x68 */
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,

  /* 0x70 */
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  {IR_TOK_NUMBER},
  {IR_TOK_IDENTIFIER, 1, 1},
  {IR_TOK_STRING},
  {IR_TOK_PARAM},

  /* 0x78 */
  {IR_TOK_EMPTY_ARG},
  {IR_TOK_PREP_NUMBER},
  EMPTY,
  TOK(IR_TOK_OPEN_CURLY, "{"),
  TOK(IR_TOK_OR, "|"),
  TOK(IR_TOK_CLOSE_CURLY, "}"),
  TOK(IR_TOK_NEG, "~"),
  EMPTY
};
