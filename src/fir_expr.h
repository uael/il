#ifndef _JAYL_FIR_EXPR_H_
#define _JAYL_FIR_EXPR_H_

#include "fir_type.h"

TYPEDEF_ENUMS(
  fir_expr_kind_t,
  fir_op_t
);

TYPEDEF_UNIONS(
  fir_expr_t
);

TYPEDEF_STRUCTS(
  fir_expr_const_string_t,
  fir_expr_const_float_t,
  fir_expr_const_int_t,
  fir_expr_primary_t,
  fir_expr_frame_t,
  fir_expr_cast_t,
  fir_expr_sizeof_t,
  fir_expr_typeof_t,
  fir_expr_call_t,
  fir_expr_array_access_t,
  fir_expr_sign_access_t,
  fir_expr_mem_access_t,
  fir_expr_unary_t,
  fir_expr_binary_t,
  fir_expr_ternary_t,
  fir_expr_lambda_t
);

#define FIR_EXPR_BASE \
  fir_expr_kind_t kind; \
  fir_expr_t *next; \
  struct _fir_type_t *type; \
  bool lowered

enum _fir_expr_kind_t {
  FIR_EXPR_CONST_STRING,
  FIR_EXPR_CONST_FLOAT,
  FIR_EXPR_CONST_INT,
  FIR_EXPR_PRIMARY,
  FIR_EXPR_FRAME,
  FIR_EXPR_CAST,
  FIR_EXPR_SIZEOF,
  FIR_EXPR_TYPEOF,
  FIR_EXPR_CALL,
  FIR_EXPR_ARRAY_ACCESS,
  FIR_EXPR_SIGN_ACCESS,
  FIR_EXPR_MEM_ACCESS,
  FIR_EXPR_UNARY,
  FIR_EXPR_TERNARY,
  FIR_EXPR_LAMBDA
};

enum _fir_op_t {
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
  FIR_OP_GT,     /* l > r        */
  FIR_OP_DQ      /* l ?? r       */
};

struct _fir_expr_const_string_t {
  FIR_EXPR_BASE;
  const char *value;
};

struct _fir_expr_const_float_t {
  FIR_EXPR_BASE;
  float value;
};

struct _fir_expr_const_int_t {
  FIR_EXPR_BASE;
  int value;
};

struct _fir_expr_frame_t {
  FIR_EXPR_BASE;
  fir_expr_t *expr;
};

struct _fir_expr_cast_t {
  FIR_EXPR_BASE;
  fir_expr_t *expr;
};

struct _fir_expr_sizeof_t {
  FIR_EXPR_BASE;
  fir_expr_t *of_expr;
  struct _fir_type_t *of_type;
};

struct _fir_expr_typeof_t {
  FIR_EXPR_BASE;
  fir_expr_t *of_expr;
  struct _fir_type_t *of_type;
};

struct _fir_expr_call_t {
  FIR_EXPR_BASE;
  fir_expr_t *expr, *args;
};

struct _fir_expr_array_access_t {
  FIR_EXPR_BASE;
  fir_expr_t *expr, *at;
};

struct _fir_expr_sign_access_t {
  FIR_EXPR_BASE;
  struct _fir_symbol_t *symbol;
  fir_expr_t *expr;
};

struct _fir_expr_mem_access_t {
  FIR_EXPR_BASE;
  struct _fir_symbol_t *symbol;
  fir_expr_t *expr;
};

struct _fir_expr_unary_t {
  FIR_EXPR_BASE;
  fir_op_t op;
  fir_expr_t *right;
};

struct _fir_expr_binary_t {
  FIR_EXPR_BASE;
  fir_op_t op;
  fir_expr_t *left, *right;
};

struct _fir_expr_ternary_t {
  FIR_EXPR_BASE;
  fir_expr_t *expr, *left, *right;
};

struct _fir_expr_lambda_t {
  FIR_EXPR_BASE;
};

union _fir_expr_t {
  fir_expr_kind_t kind;
  fir_expr_const_string_t const_string;
  fir_expr_const_float_t const_float;
  fir_expr_const_int_t const_int;
  fir_expr_frame_t frame;
  fir_expr_cast_t cast;
  fir_expr_sizeof_t size_of;
  fir_expr_typeof_t type_of;
  fir_expr_call_t call;
  fir_expr_array_access_t array_access;
  fir_expr_sign_access_t sign_access;
  fir_expr_mem_access_t mem_access;
  fir_expr_unary_t unary;
  fir_expr_binary_t binary;
  fir_expr_ternary_t ternary;
  fir_expr_lambda_t lambda;
};

#endif /* _JAYL_FIR_EXPR_H_ */
