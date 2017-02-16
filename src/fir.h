#ifndef _JAYL_FIR_H_
#define _JAYL_FIR_H_

#include <p99.h>
#include "adt/deque.h"
#include "adt/string.h"

typedef enum {
  FIR_TYPE_VOID = 0,
  FIR_TYPE_CHAR,
  FIR_TYPE_SHORT,
  FIR_TYPE_INT,
  FIR_TYPE_LONG,
  FIR_TYPE_FLOAT,
  FIR_TYPE_DOUBLE,
  FIR_TYPE_LDOUBLE,
  FIR_TYPE_POINTER,
  FIR_TYPE_FUNCTION,
  FIR_TYPE_ARRAY,
  FIR_TYPE_STRUCT,
  FIR_TYPE_UNION,
  FIR_TYPE_NAMESPACE
} fir_type;

typedef enum {
  FIR_QUAL_UNSIGNED = 0,
  FIR_QUAL_CONST,
  FIR_QUAL_VOLATILE
} fir_qual;

typedef enum {
  FIR_SYM_DEFINITION = 0,
  FIR_SYM_TENTATIVE,
  FIR_SYM_DECLARATION,
  FIR_SYM_TYPEDEF,
  FIR_SYM_STRING_VALUE,
  FIR_SYM_CONSTANT,
  FIR_SYM_LABEL
} fir_sym;

/**
 * Visibility of external declarations, or FIR_LINK_NONE for other
 * symbols.
 */
typedef enum {
  FIR_LINK_NONE = 0,
  FIR_LINK_INTERN,
  FIR_LINK_EXTERN
} fir_link;

typedef enum {
  FIR_KIND_TU = 0,
  FIR_KIND_MEM,
  FIR_KIND_STMT,
  FIR_KIND_EXPR
} fir_kind;

/**
 * Map token type to corresponding numerical ascii value where possible,
 * and fit the remaining tokens in between.
 */
typedef enum {
  FIR_TOK_END = 0,
  FIR_TOK_AUTO = FIR_TOK_END + 1,
  FIR_TOK_BREAK,
  FIR_TOK_CASE,
  FIR_TOK_CHAR,
  FIR_TOK_CONST,
  FIR_TOK_CONTINUE,
  FIR_TOK_DEFAULT,
  FIR_TOK_DO,
  FIR_TOK_DOUBLE,
  FIR_TOK_EOL = '\n',
  FIR_TOK_ELSE = FIR_TOK_EOL + 1,
  FIR_TOK_ENUM,
  FIR_TOK_EXTERN,
  FIR_TOK_FLOAT,
  FIR_TOK_FOR,
  FIR_TOK_GOTO,
  FIR_TOK_IF,
  FIR_TOK_INT,
  FIR_TOK_LONG,
  FIR_TOK_REGISTER,
  FIR_TOK_RETURN,
  FIR_TOK_SHORT,
  FIR_TOK_SIGNED,
  FIR_TOK_SIZEOF,
  FIR_TOK_STATIC,
  FIR_TOK_STRUCT,
  FIR_TOK_SWITCH,
  FIR_TOK_TYPEDEF,
  FIR_TOK_UNION,
  FIR_TOK_UNSIGNED,
  FIR_TOK_VOID,
  FIR_TOK_INLINE,

  FIR_TOK_NOT = '!',
  FIR_TOK_VOLATILE = FIR_TOK_NOT + 1,
  FIR_TOK_HASH = '#',
  FIR_TOK_WHILE = FIR_TOK_HASH + 1,
  FIR_TOK_MODULO = '%',
  FIR_TOK_AND = '&',
  FIR_TOK_INCLUDE = FIR_TOK_AND + 1,

  FIR_TOK_OPEN_PAREN = '(',
  FIR_TOK_CLOSE_PAREN = ')',
  FIR_TOK_STAR = '*',
  FIR_TOK_PLUS = '+',
  FIR_TOK_COMMA = ',',
  FIR_TOK_MINUS = '-',
  FIR_TOK_DOT = '.',
  FIR_TOK_SLASH = '/',
  FIR_TOK_USE = FIR_TOK_SLASH + 1,
  FIR_TOK_DCOLON, /* :: */
  FIR_TOK_DARROW, /* => */
  FIR_TOK_DQUESTION, /* ?? */
  FIR_TOK_NAMESPACE,

  FIR_TOK_COLON = ':',
  FIR_TOK_SEMICOLON = ';',
  FIR_TOK_LT = '<',
  FIR_TOK_ASSIGN = '=',
  FIR_TOK_GT = '>',
  FIR_TOK_QUESTION = '?',
  FIR_TOK_DOTS = FIR_TOK_QUESTION + 1,  /* ... */
  FIR_TOK_LOGICAL_OR,           /* || */
  FIR_TOK_LOGICAL_AND,          /* && */
  FIR_TOK_LEQ,                  /* <= */
  FIR_TOK_GEQ,                  /* >= */
  FIR_TOK_EQ,                   /* == */
  FIR_TOK_NEQ,                  /* != */
  FIR_TOK_ARROW,                /* -> */
  FIR_TOK_INCREMENT,            /* ++ */
  FIR_TOK_DECREMENT,            /* -- */
  FIR_TOK_LSHIFT,               /* << */
  FIR_TOK_RSHIFT,               /* >> */
  FIR_TOK_MUL_ASSIGN,           /* *= */
  FIR_TOK_DIV_ASSIGN,           /* /= */
  FIR_TOK_MOD_ASSIGN,           /* %= */
  FIR_TOK_PLUS_ASSIGN,          /* += */
  FIR_TOK_MINUS_ASSIGN,         /* -= */
  FIR_TOK_LSHIFT_ASSIGN,        /* <<= */
  FIR_TOK_RSHIFT_ASSIGN,        /* >>= */
  FIR_TOK_AND_ASSIGN,           /* &= */
  FIR_TOK_XOR_ASSIGN,           /* ^= */
  FIR_TOK_OR_ASSIGN,            /* |= */
  FIR_TOK_TOKEN_PASTE,          /* ## */

  FIR_TOK_OPEN_BRACKET = '[',
  FIR_TOK_CLOSE_BRACKET = ']',
  FIR_TOK_XOR = '^',
  FIR_TOK_OPEN_CURLY = '{',
  FIR_TOK_OR = '|',
  FIR_TOK_CLOSE_CURLY = '}',
  FIR_TOK_NEG = '~',

  /*
   * The remaining tokens do not correspond to any fixed string, and
   * are placed at arbitrary locations.
   */
  FIR_TOK_NUMBER = 116,
  FIR_TOK_IDENTIFIER,
  FIR_TOK_STRING,

  /*
   * Pseudo-token representing parameter substitution in macros. Has
   * an immediate integer value referring to the parameter index.
   */
  FIR_TOK_PARAM,

  /*
   * Pseudo-token representing empty argument for macro expansion in
   * preprocessor.
   */
  FIR_TOK_EMPTY_ARG,

  /*
   * Preprocessing token representing a number. Valid strings include
   * a superset of numeric constants in C. A conversion to NUMBER
   * token is done before handed to parser.
   */
  FIR_TOK_PREP_NUMBER
} fir_tok;

typedef enum {
  FIR_ST_PARAM = 0,    /* param (expr)             */
  FIR_ST_VA_START, /* va_start(expr);          */
  FIR_ST_EXPR,     /* (expr);                  */
  FIR_ST_LABEL,    /* label: right             */
  FIR_ST_CASE = FIR_TOK_CASE,     /* case(expr): right        */
  FIR_ST_DEFAULT = FIR_TOK_DEFAULT,  /* default: right           */
  FIR_ST_COMPOUND, /* { down }                 */
  FIR_ST_IF = FIR_TOK_IF,       /* if (expr) right          */
  FIR_ST_ELSE = FIR_TOK_ELSE,     /* else right               */
  FIR_ST_SWITCH = FIR_TOK_SWITCH,   /* switch (expr) right      */
  FIR_ST_WHILE = FIR_TOK_WHILE,    /* while (expr) right       */
  FIR_ST_DO = FIR_TOK_DO,       /* do right while (expr)    */
  FIR_ST_FOR = FIR_TOK_FOR,      /* for (right (IF_FOR_INIT) */
  IF_ST_FOR_INIT = FIR_ST_FOR + 1, /* down; right              */
  IF_ST_FOR_COND, /* down; expr) right        */
  FIR_ST_GOTO = FIR_TOK_GOTO,     /* goto name; right = label */
  FIR_ST_BREAK = FIR_TOK_BREAK,    /* break;                   */
  FIR_ST_CONTINUE = FIR_TOK_CONTINUE, /* continue;                */
  FIR_ST_RETURN = FIR_TOK_RETURN,   /* return expr;             */
  FIR_ST_VAR       /* var expr;                */
} fir_st;

typedef enum {
  FIR_OP_CAST,   /* (T) l        */
  FIR_OP_CALL,   /* l()          */
  FIR_OP_VA_ARG, /* va_arg(l, T) */
  FIR_OP_NOT,    /* !l           */
  FIR_OP_ASSIGN, /* l = r        */
  FIR_OP_ADD,    /* l + r        */
  FIR_OP_SUB,    /* l - r        */
  FIR_OP_MUL,    /* l * r        */
  FIR_OP_DIV,    /* l / r        */
  FIR_OP_MOD,    /* l % r        */
  FIR_OP_AND,    /* l & r        */
  FIR_OP_OR,     /* l | r        */
  FIR_OP_XOR,    /* l ^ r        */
  FIR_OP_SHL,    /* l << r       */
  FIR_OP_SHR,    /* l >> r       */
  FIR_OP_EQ,     /* l == r       */
  FIR_OP_NE,     /* l != r       */
  FIR_OP_GE,     /* l >= r       */
  FIR_OP_GT      /* l > r        */
} fir_op;

struct _fir_visitor_t;
struct _fir_stmt_t;

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
  fir_type type : 8;
  fir_qual qualifiers : 8;
  int ref : 16;
} fir_sign_t;

/**
 * The type fir_value_t
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
} fir_value_t;

typedef struct {
  const char *filename;
  int col, line;
} fir_loc_t;

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
  fir_tok kind;
  uint leading_whitespace : 16;
  uint is_id_or_kw : 1;
  uint is_char_literal : 1;
  uint disable_expand : 1;
  fir_sign_t type;
  union {
    string_t string;
    fir_value_t val;
  } d;
  fir_loc_t loc;
} fir_tok_t;

/**
 * Table is indexed by ASCII value, which is also assigned to
 * corresponding token type. To get a token of a particular type,
 * access tokens[ir_tok].
 */
extern const fir_tok_t tokens[128];

typedef deque_of(fir_tok_t) fir_toks_t;

/**
 * A fir_sym_t represents declarations that may have a storage location at
 * runtime, such as functions, static and local variables. Store offset
 * to base pointer for automatic variables and function arguments.
 */
typedef struct {
  fir_sym sym;
  string_t name;
  fir_sign_t sign;
  fir_link linkage;
  union {
    fir_value_t constant;
    string_t string;
    struct _fir_stmt_t *label;
  } value;
} fir_sym_t;

#define __FIR_NODE_PROPERTIES \
  struct _fir_node_t *parent; \
  void (*accept)(struct _fir_node_t *node, struct _fir_visitor_t *visitor); \
  fir_kind kind \

typedef struct _fir_node_t {
  __FIR_NODE_PROPERTIES;
} fir_node_t;

typedef struct _fir_tu_t {
  __FIR_NODE_PROPERTIES;
  const fir_sym_t *sym;
  struct _fir_stmt_t *body;
} fir_tu_t;

typedef deque_of(fir_tu_t) fir_tus_t;

typedef struct _fir_expr_t {
  __FIR_NODE_PROPERTIES;
  fir_op op;
  fir_sign_t sign;
  struct _fir_expr_t *left, *right;
} fir_expr_t;

typedef struct _fir_stmt_t {
  __FIR_NODE_PROPERTIES;
  fir_st st;
  string_t label;
  fir_expr_t *expr;
  struct _fir_stmt_t *down, *right;
} fir_stmt_t;

/**
 * @brief Get the number of struct or union members, or function parameters.
 * @param sign The signature
 * @return The number of members
 */
unsigned fir_nmem(fir_sign_t sign);
fir_tu_t *fir_mem(fir_sign_t sign, int n);
fir_sign_t fir_next(fir_sign_t sign);
bool fis_vararg(fir_sign_t sign);
size_t size_of(fir_sign_t sign);
size_t type_alignment(fir_sign_t sign);
bool type_equal(fir_sign_t l, fir_sign_t r);
fir_sign_t usual_arithmetic_conversion(fir_sign_t t1, fir_sign_t t2);

#endif /* _JAYL_FIR_H_ */
