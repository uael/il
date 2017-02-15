#ifndef _JAYL_IR_H_
#define _JAYL_IR_H_

#include <p99.h>
#include "deque.h"
#include "string.h"

typedef enum {
  IR_TYPE_VOID = 0,
  IR_TYPE_CHAR,
  IR_TYPE_SHORT,
  IR_TYPE_INT,
  IR_TYPE_LONG,
  IR_TYPE_FLOAT,
  IR_TYPE_DOUBLE,
  IR_TYPE_LDOUBLE,
  IR_TYPE_POINTER,
  IR_TYPE_FUNCTION,
  IR_TYPE_ARRAY,
  IR_TYPE_STRUCT,
  IR_TYPE_UNION,
  IR_TYPE_NAMESPACE
} ir_type;

typedef enum {
  IR_QUAL_UNSIGNED = 0,
  IR_QUAL_CONST,
  IR_QUAL_VOLATILE
} ir_qual;

typedef enum {
  IR_SYM_DEFINITION = 0,
  IR_SYM_TENTATIVE,
  IR_SYM_DECLARATION,
  IR_SYM_TYPEDEF,
  IR_SYM_STRING_VALUE,
  IR_SYM_CONSTANT,
  IR_SYM_LABEL
} ir_sym;

/**
 * Visibility of external declarations, or IR_LINK_NONE for other
 * symbols.
 */
typedef enum {
  IR_LINK_NONE = 0,
  IR_LINK_INTERN,
  IR_LINK_EXTERN
} ir_link;

typedef enum {
  IR_KIND_TU = 0,
  IR_KIND_MEM,
  IR_KIND_STMT,
  IR_KIND_EXPR
} ir_kind;

/**
 * Map token type to corresponding numerical ascii value where possible,
 * and fit the remaining tokens in between.
 */
typedef enum {
  IR_TOK_END = 0,
  IR_TOK_AUTO = IR_TOK_END + 1,
  IR_TOK_BREAK,
  IR_TOK_CASE,
  IR_TOK_CHAR,
  IR_TOK_CONST,
  IR_TOK_CONTINUE,
  IR_TOK_DEFAULT,
  IR_TOK_DO,
  IR_TOK_DOUBLE,
  IR_TOK_EOL = '\n',
  IR_TOK_ELSE = IR_TOK_EOL + 1,
  IR_TOK_ENUM,
  IR_TOK_EXTERN,
  IR_TOK_FLOAT,
  IR_TOK_FOR,
  IR_TOK_GOTO,
  IR_TOK_IF,
  IR_TOK_INT,
  IR_TOK_LONG,
  IR_TOK_REGISTER,
  IR_TOK_RETURN,
  IR_TOK_SHORT,
  IR_TOK_SIGNED,
  IR_TOK_SIZEOF,
  IR_TOK_STATIC,
  IR_TOK_STRUCT,
  IR_TOK_SWITCH,
  IR_TOK_TYPEDEF,
  IR_TOK_UNION,
  IR_TOK_UNSIGNED,
  IR_TOK_VOID,
  IR_TOK_INLINE,

  IR_TOK_NOT = '!',
  IR_TOK_VOLATILE = IR_TOK_NOT + 1,
  IR_TOK_HASH = '#',
  IR_TOK_WHILE = IR_TOK_HASH + 1,
  IR_TOK_MODULO = '%',
  IR_TOK_AND = '&',
  IR_TOK_INCLUDE = IR_TOK_AND + 1,

  IR_TOK_OPEN_PAREN = '(',
  IR_TOK_CLOSE_PAREN = ')',
  IR_TOK_STAR = '*',
  IR_TOK_PLUS = '+',
  IR_TOK_COMMA = ',',
  IR_TOK_MINUS = '-',
  IR_TOK_DOT = '.',
  IR_TOK_SLASH = '/',
  IR_TOK_USE = IR_TOK_SLASH + 1,
  IR_TOK_DCOLON, /* :: */
  IR_TOK_DARROW, /* => */
  IR_TOK_DQUESTION, /* ?? */
  IR_TOK_NAMESPACE,

  IR_TOK_COLON = ':',
  IR_TOK_SEMICOLON = ';',
  IR_TOK_LT = '<',
  IR_TOK_ASSIGN = '=',
  IR_TOK_GT = '>',
  IR_TOK_QUESTION = '?',
  IR_TOK_DOTS = IR_TOK_QUESTION + 1,  /* ... */
  IR_TOK_LOGICAL_OR,           /* || */
  IR_TOK_LOGICAL_AND,          /* && */
  IR_TOK_LEQ,                  /* <= */
  IR_TOK_GEQ,                  /* >= */
  IR_TOK_EQ,                   /* == */
  IR_TOK_NEQ,                  /* != */
  IR_TOK_ARROW,                /* -> */
  IR_TOK_INCREMENT,            /* ++ */
  IR_TOK_DECREMENT,            /* -- */
  IR_TOK_LSHIFT,               /* << */
  IR_TOK_RSHIFT,               /* >> */
  IR_TOK_MUL_ASSIGN,           /* *= */
  IR_TOK_DIV_ASSIGN,           /* /= */
  IR_TOK_MOD_ASSIGN,           /* %= */
  IR_TOK_PLUS_ASSIGN,          /* += */
  IR_TOK_MINUS_ASSIGN,         /* -= */
  IR_TOK_LSHIFT_ASSIGN,        /* <<= */
  IR_TOK_RSHIFT_ASSIGN,        /* >>= */
  IR_TOK_AND_ASSIGN,           /* &= */
  IR_TOK_XOR_ASSIGN,           /* ^= */
  IR_TOK_OR_ASSIGN,            /* |= */
  IR_TOK_TOKEN_PASTE,          /* ## */

  IR_TOK_OPEN_BRACKET = '[',
  IR_TOK_CLOSE_BRACKET = ']',
  IR_TOK_XOR = '^',
  IR_TOK_OPEN_CURLY = '{',
  IR_TOK_OR = '|',
  IR_TOK_CLOSE_CURLY = '}',
  IR_TOK_NEG = '~',

  /*
   * The remaining tokens do not correspond to any fixed string, and
   * are placed at arbitrary locations.
   */
  IR_TOK_NUMBER = 116,
  IR_TOK_IDENTIFIER,
  IR_TOK_STRING,

  /*
   * Pseudo-token representing parameter substitution in macros. Has
   * an immediate integer value referring to the parameter index.
   */
  IR_TOK_PARAM,

  /*
   * Pseudo-token representing empty argument for macro expansion in
   * preprocessor.
   */
  IR_TOK_EMPTY_ARG,

  /*
   * Preprocessing token representing a number. Valid strings include
   * a superset of numeric constants in C. A conversion to NUMBER
   * token is done before handed to parser.
   */
  IR_TOK_PREP_NUMBER
} ir_tok;

typedef enum {
  IR_ST_PARAM = 0,    /* param (expr)             */
  IR_ST_VA_START, /* va_start(expr);          */
  IR_ST_EXPR,     /* (expr);                  */
  IR_ST_LABEL,    /* label: right             */
  IR_ST_CASE = IR_TOK_CASE,     /* case(expr): right        */
  IR_ST_DEFAULT = IR_TOK_DEFAULT,  /* default: right           */
  IR_ST_COMPOUND, /* { down }                 */
  IR_ST_IF = IR_TOK_IF,       /* if (expr) right          */
  IR_ST_ELSE = IR_TOK_ELSE,     /* else right               */
  IR_ST_SWITCH = IR_TOK_SWITCH,   /* switch (expr) right      */
  IR_ST_WHILE = IR_TOK_WHILE,    /* while (expr) right       */
  IR_ST_DO = IR_TOK_DO,       /* do right while (expr)    */
  IR_ST_FOR = IR_TOK_FOR,      /* for (right (IF_FOR_INIT) */
  IF_ST_FOR_INIT = IR_ST_FOR + 1, /* down; right              */
  IF_ST_FOR_COND, /* down; expr) right        */
  IR_ST_GOTO = IR_TOK_GOTO,     /* goto name; right = label */
  IR_ST_BREAK = IR_TOK_BREAK,    /* break;                   */
  IR_ST_CONTINUE = IR_TOK_CONTINUE, /* continue;                */
  IR_ST_RETURN = IR_TOK_RETURN,   /* return expr;             */
  IR_ST_VAR       /* var expr;                */
} ir_st;

typedef enum {
  IR_OP_CAST,   /* (T) l        */
  IR_OP_CALL,   /* l()          */
  IR_OP_VA_ARG, /* va_arg(l, T) */
  IR_OP_NOT,    /* !l           */
  IR_OP_ASSIGN, /* l = r        */
  IR_OP_ADD,    /* l + r        */
  IR_OP_SUB,    /* l - r        */
  IR_OP_MUL,    /* l * r        */
  IR_OP_DIV,    /* l / r        */
  IR_OP_MOD,    /* l % r        */
  IR_OP_AND,    /* l & r        */
  IR_OP_OR,     /* l | r        */
  IR_OP_XOR,    /* l ^ r        */
  IR_OP_SHL,    /* l << r       */
  IR_OP_SHR,    /* l >> r       */
  IR_OP_EQ,     /* l == r       */
  IR_OP_NE,     /* l != r       */
  IR_OP_GE,     /* l >= r       */
  IR_OP_GT      /* l > r        */
} ir_op;

struct _ir_visitor_t;
struct _ir_stmt_t;

/**
 * Internal representation of a type.
 *
 * Optimize for space and treat as a value type. This should fit in 32
 * bits, and be able to encode basic types such as:
 *
 *     int
 *     const int * volatile
 *     unsigned short const * const.
 *
 * For function, array, aggregate, and deeper pointer types, the type is
 * encoded in an opaque structure referenced by ref. All other types are
 * completely represented by this object, and have ref value 0.
 */
typedef struct {
  ir_type type : 8;
  ir_qual qualifiers : 8;
  int ref : 16;
} ir_sign_t;

/**
 * The type ir_value_t
 *
 * Hold an immediate numeric value. Associated type is used to determine
 * which element is valid.
 */
typedef union {
  ulong u;
  slong i;
  float f;
  double d;
  ldouble ld;
} ir_value_t;

typedef struct {
  const char *filename;
  int col, line;
} ir_loc_t;

/**
 * The type ir_token_t
 *
 * Representation of token; used in preprocessing, and interface to
 * parser.
 *
 * Tokens keep track of typed numbers, to capture difference between
 * literals like 1 and 1ul. Type should always correspond to one of the
 * basic integer types.
 */
typedef struct {
  ir_tok kind;
  uint leading_whitespace : 16;
  uint is_ident : 1;
  uint is_char_literal : 1;
  uint disable_expand : 1;
  ir_sign_t type;
  union {
    string_t string;
    ir_value_t val;
  } d;
  ir_loc_t loc;
} ir_tok_t;

typedef deque_of(ir_tok_t) ir_toks_t;

/**
 * A ir_sym_t represents declarations that may have a storage location at
 * runtime, such as functions, static and local variables. Store offset
 * to base pointer for automatic variables and function arguments.
 */
typedef struct {
  ir_sym sym;
  string_t name;
  ir_sign_t sign;
  ir_link linkage;
  union {
    ir_value_t constant;
    string_t string;
    struct _ir_stmt_t *label;
  } value;
} ir_sym_t;

#define __IR_NODE_PROPERTIES \
  struct _ir_node_t *parent; \
  void (*accept)(struct _ir_node_t *node, struct _ir_visitor_t *visitor); \
  ir_kind kind \

typedef struct _ir_node_t {
  __IR_NODE_PROPERTIES;
} ir_node_t;

typedef struct _ir_tu_t {
  __IR_NODE_PROPERTIES;
  const ir_sym_t *sym;
  struct _ir_stmt_t *body;
} ir_tu_t;

typedef struct _ir_expr_t {
  __IR_NODE_PROPERTIES;
  ir_op op;
  ir_sign_t sign;
  struct _ir_expr_t *left, *right;
} ir_expr_t;

typedef struct _ir_stmt_t {
  __IR_NODE_PROPERTIES;
  ir_st st;
  string_t label;
  ir_expr_t *expr;
  struct _ir_stmt_t *down, *right;
} ir_stmt_t;

/**
 * @brief Get the number of struct or union members, or function parameters.
 * @param sign The signature
 * @return The number of members
 */
unsigned ir_nmem(ir_sign_t sign);
ir_tu_t *ir_mem(ir_sign_t sign, int n);
ir_sign_t ir_next(ir_sign_t sign);
bool is_vararg(ir_sign_t sign);
size_t size_of(ir_sign_t sign);
size_t type_alignment(ir_sign_t sign);
bool type_equal(ir_sign_t l, ir_sign_t r);
ir_sign_t usual_arithmetic_conversion(ir_sign_t t1, ir_sign_t t2);

#endif /* _JAYL_IR_H_ */
