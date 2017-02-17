#include "fir.h"

const fir_type_t basic_types[] = {
  {FIR_TYPE_VOID},
  {FIR_TYPE_CHAR},
  {FIR_TYPE_SHORT},
  {FIR_TYPE_INT},
  {FIR_TYPE_LONG},
  {FIR_TYPE_FLOAT},
  {FIR_TYPE_DOUBLE},
  {FIR_TYPE_LDOUBLE},
  {FIR_TYPE_POINTER},
  {FIR_TYPE_FUNCTION},
  {FIR_TYPE_ARRAY},
  {FIR_TYPE_STRUCT},
  {FIR_TYPE_UNION},
  {FIR_TYPE_NAMESPACE}
};

/**
 * Static initializer for token. Only works with string representation
 * that can fit inline.
 */
#define EMPTY {(fir_tok) 0}
#define TOK(t, s) {(t), 0, 0, 0, 0, {(fir_type) 0}, SHORT_STRING_INIT(s), {NULL}}
#define IDN(t, s) {(t), 0, 1, 0, 0, {(fir_type) 0}, SHORT_STRING_INIT(s), {NULL}}

const fir_tok_t tokens[] = {
  /* 0x00 */
  TOK(FIR_TOK_END, "$"),
  IDN(FIR_TOK_AUTO, "auto"),
  IDN(FIR_TOK_BREAK, "break"),
  IDN(FIR_TOK_CASE, "case"),
  IDN(FIR_TOK_CHAR, "char"),
  IDN(FIR_TOK_CONST, "const"),
  IDN(FIR_TOK_CONTINUE, "continue"),
  IDN(FIR_TOK_DEFAULT, "default"),

  /* 0x08 */
  IDN(FIR_TOK_DO, "do"),
  IDN(FIR_TOK_DOUBLE, "double"),
  TOK(FIR_TOK_EOL, "\n"),
  IDN(FIR_TOK_ELSE, "else"),
  IDN(FIR_TOK_ENUM, "enum"),
  IDN(FIR_TOK_EXTERN, "extern"),
  IDN(FIR_TOK_FLOAT, "float"),
  IDN(FIR_TOK_FOR, "for"),

  /* 0x10 */
  IDN(FIR_TOK_GOTO, "goto"),
  IDN(FIR_TOK_IF, "if"),
  IDN(FIR_TOK_INT, "int"),
  IDN(FIR_TOK_LONG, "long"),
  IDN(FIR_TOK_REGISTER, "register"),
  IDN(FIR_TOK_RETURN, "return"),
  IDN(FIR_TOK_SHORT, "short"),
  IDN(FIR_TOK_SIGNED, "signed"),

  /* 0x18 */
  IDN(FIR_TOK_SIZEOF, "sizeof"),
  IDN(FIR_TOK_STATIC, "static"),
  IDN(FIR_TOK_STRUCT, "struct"),
  IDN(FIR_TOK_SWITCH, "switch"),
  IDN(FIR_TOK_TYPEDEF, "typedef"),
  IDN(FIR_TOK_UNION, "union"),
  IDN(FIR_TOK_UNSIGNED, "unsigned"),
  IDN(FIR_TOK_VOID, "void"),

  /* 0x20 */
  IDN(FIR_TOK_INLINE, "inline"),
  TOK(FIR_TOK_NOT, "!"),
  IDN(FIR_TOK_VOLATILE, "volatile"),
  TOK(FIR_TOK_HASH, "#"),
  IDN(FIR_TOK_WHILE, "while"),
  TOK(FIR_TOK_MODULO, "%"),
  TOK(FIR_TOK_AND, "&"),
  IDN(FIR_TOK_INCLUDE, "include"),

  /* 0x28 */
  TOK(FIR_TOK_OPEN_PAREN, "("),
  TOK(FIR_TOK_CLOSE_PAREN, ")"),
  TOK(FIR_TOK_STAR, "*"),
  TOK(FIR_TOK_PLUS, "+"),
  TOK(FIR_TOK_COMMA, ","),
  TOK(FIR_TOK_MINUS, "-"),
  TOK(FIR_TOK_DOT, "."),
  TOK(FIR_TOK_SLASH, "/"),

  /* 0x30 */
  IDN(FIR_TOK_USE, "use"),
  TOK(FIR_TOK_DCOLON, "::"),
  TOK(FIR_TOK_DARROW, "=>"),
  TOK(FIR_TOK_DQUESTION, "??"),
  IDN(FIR_TOK_NAMESPACE, "namespace"),
  EMPTY,
  EMPTY,
  EMPTY,

  /* 0x38 */
  EMPTY,
  EMPTY,
  TOK(FIR_TOK_COLON, ":"),
  TOK(FIR_TOK_SEMICOLON, ";"),
  TOK(FIR_TOK_LT, "<"),
  TOK(FIR_TOK_ASSIGN, "="),
  TOK(FIR_TOK_GT, ">"),
  TOK(FIR_TOK_QUESTION, "?"),

  /* 0x40 */
  TOK(FIR_TOK_DOTS, "..."),
  TOK(FIR_TOK_LOGICAL_OR, "||"),
  TOK(FIR_TOK_LOGICAL_AND, "&&"),
  TOK(FIR_TOK_LEQ, "<="),
  TOK(FIR_TOK_GEQ, ">="),
  TOK(FIR_TOK_EQ, "=="),
  TOK(FIR_TOK_NEQ, "!="),
  TOK(FIR_TOK_ARROW, "->"),

  /* 0x48 */
  TOK(FIR_TOK_INCREMENT, "++"),
  TOK(FIR_TOK_DECREMENT, "--"),
  TOK(FIR_TOK_LSHIFT, "<<"),
  TOK(FIR_TOK_RSHIFT, ">>"),
  TOK(FIR_TOK_MUL_ASSIGN, "*="),
  TOK(FIR_TOK_DIV_ASSIGN, "/="),
  TOK(FIR_TOK_MOD_ASSIGN, "%="),
  TOK(FIR_TOK_PLUS_ASSIGN, "+="),

  /* 0x50 */
  TOK(FIR_TOK_MINUS_ASSIGN, "-="),
  TOK(FIR_TOK_LSHIFT_ASSIGN, "<<="),
  TOK(FIR_TOK_RSHIFT_ASSIGN, ">>="),
  TOK(FIR_TOK_AND_ASSIGN, "&="),
  TOK(FIR_TOK_XOR_ASSIGN, "^="),
  TOK(FIR_TOK_OR_ASSIGN, "|="),
  TOK(FIR_TOK_TOKEN_PASTE, "##"),
  EMPTY,

  /* 0x58 */
  EMPTY,
  EMPTY,
  EMPTY,
  TOK(FIR_TOK_OPEN_BRACKET, "["),
  EMPTY,
  TOK(FIR_TOK_CLOSE_BRACKET, "]"),
  TOK(FIR_TOK_XOR, "^"),
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
  {FIR_TOK_NUMBER},
  {FIR_TOK_IDENTIFIER, 1, 1},
  {FIR_TOK_STRING},
  {FIR_TOK_PARAM},

  /* 0x78 */
  {FIR_TOK_EMPTY_ARG},
  {FIR_TOK_PREP_NUMBER},
  EMPTY,
  TOK(FIR_TOK_OPEN_CURLY, "{"),
  TOK(FIR_TOK_OR, "|"),
  TOK(FIR_TOK_CLOSE_CURLY, "}"),
  TOK(FIR_TOK_NEG, "~"),
  EMPTY
};
