/*
 * libil - Intermediate Language cross-platform c library
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#ifndef   IL_IR_H__
# define  IL_IR_H__

#include <adt/vector.h>

#include "token.h"

struct il_entity;
struct il_expr;
struct il_stmt;
struct il_type;

typedef struct il_type il_type_t;
typedef struct il_literal il_literal_t;
typedef struct il_pointer il_pointer_t;
typedef struct il_array il_array_t;
typedef struct il_compound il_compound_t;

enum il_type_n {
  IL_TYPE_UNDEFINED = 0,
  IL_TYPE_VOID,
  IL_TYPE_POINTER,
  IL_TYPE_ARRAY,
  IL_TYPE_COMPOUND,
  IL_TYPE_BOOL,
  IL_TYPE_CHAR,
  IL_TYPE_SHORT,
  IL_TYPE_INT,
  IL_TYPE_LONG,
  IL_TYPE_FLOAT,
  IL_TYPE_DOUBLE,
  IL_TYPE_LONG_LONG,
  IL_TYPE_LONG_DOUBLE
};

enum il_type_specifier_n {
  IL_TYPE_SPECIFIER_NONE = 0,
  IL_TYPE_SPECIFIER_SIGNED = 1 << 0,
  IL_TYPE_SPECIFIER_UNSIGNED = 1 << 1,
  IL_TYPE_SPECIFIER_COMPLEX = 1 << 2,
  IL_TYPE_SPECIFIER_IMAGINARY = 1 << 3,
  IL_TYPE_SPECIFIER_TYPEDEF = 1 << 4,
  IL_TYPE_SPECIFIER_EXTERN = 1 << 5,
  IL_TYPE_SPECIFIER_STATIC = 1 << 6,
  IL_TYPE_SPECIFIER_THREAD_LOCAL = 1 << 7,
  IL_TYPE_SPECIFIER_AUTO = 1 << 8,
  IL_TYPE_SPECIFIER_REGISTER = 1 << 9,
  IL_FUNC_SPECIFIER_INLINE = 1 << 10,
  IL_FUNC_SPECIFIER_NORETURN = 1 << 11
};

enum il_type_qualifier_n {
  IL_TYPE_QUALIFIER_NONE = 0,
  IL_TYPE_QUALIFIER_CONST = 1 << 0,
  IL_TYPE_QUALIFIER_VOLATILE = 1 << 1,
  IL_TYPE_QUALIFIER_RESTRICT = 1 << 2,
  IL_TYPE_QUALIFIER_ATOMIC = 1 << 3
};

struct il_literal {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t *next;
  enum il_type_specifier_n specifiers;
  enum il_type_qualifier_n qualifiers;
};

struct il_pointer {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t *of;
  enum il_type_specifier_n specifiers;
  enum il_type_qualifier_n qualifiers;
};

struct il_array {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t *of;
  enum il_type_specifier_n specifiers;
  enum il_type_qualifier_n qualifiers;

  struct il_expr *length;
};

struct il_compound {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t *next;
  enum il_type_specifier_n specifiers;
  enum il_type_qualifier_n qualifiers;

  struct il_entity *entity;
};

struct il_type {
  union {
    struct {
      il_lloc_t lloc;
      size_t size, align;
      il_type_t *next;
      enum il_type_specifier_n specifiers;
      enum il_type_qualifier_n qualifiers;
    };
    il_literal_t literal;
    il_pointer_t pointer;
    il_array_t array;
    il_compound_t compound;
  };
  enum il_type_n kind;
};

struct il_field;

typedef struct il_val il_val_t;
typedef struct il_expr il_expr_t;
typedef struct il_exprs il_exprs_t;
typedef struct il_id il_id_t;
typedef struct il_const il_const_t;
typedef struct il_unary il_unary_t;
typedef struct il_binary il_binary_t;
typedef struct il_ternary il_ternary_t;
typedef struct il_array_read il_array_read_t;
typedef struct il_array_write il_array_write_t;
typedef struct il_field_read il_field_read_t;
typedef struct il_field_write il_field_write_t;
typedef struct il_call il_call_t;

typedef adt_vector_of(il_expr_t) il_expr_r;

enum il_expr_n {
  IL_EXPR_UNDEFINED = 0,
  IL_EXPR_EXPRS,
  IL_EXPR_ID,
  IL_EXPR_CONST,
  IL_EXPR_UNARY,
  IL_EXPR_BINARY,
  IL_EXPR_TERNARY,
  IL_EXPR_ARRAY_READ,
  IL_EXPR_ARRAY_WRITE,
  IL_EXPR_FIELD_READ,
  IL_EXPR_FIELD_WRITE,
  IL_EXPR_CALL
};

enum il_op_n {
  IL_OP_CAST,   /* (T) l        */
  IL_OP_EN,     /* (l)          */
  IL_OP_CALL,   /* l()          */
  IL_OP_VA_ARG, /* va_arg(l, T) */
  IL_OP_NOT,    /* !l           */
  IL_OP_ASSIGN, /* l = r        */
  IL_OP_ADD,    /* l + r        */
  IL_OP_SUB,    /* l - r        */
  IL_OP_MUL,    /* l * r        */
  IL_OP_DIV,    /* l / r        */
  IL_OP_MOD,    /* l % r        */
  IL_OP_AND,    /* l & r        */
  IL_OP_LAND,    /* l && r        */
  IL_OP_OR,     /* l | r        */
  IL_OP_LOR,     /* l || r        */
  IL_OP_XOR,    /* l ^ r        */
  IL_OP_SHL,    /* l << r       */
  IL_OP_SHR,    /* l >> r       */
  IL_OP_EQ,     /* l == r       */
  IL_OP_NE,     /* l != r       */
  IL_OP_GE,     /* l >= r       */
  IL_OP_GT,     /* l > r        */
  IL_OP_LE,     /* l <= r       */
  IL_OP_LT,     /* l < r        */
  IL_OP_NEG     /* l ~ r        */
};

struct il_val {
  il_type_t type;
  union {
    const char *s;
    float f;
    double d;
    long double ld;
    unsigned long int ul;
  };
};

struct il_exprs {
  il_lloc_t lloc;
  il_type_t type;

  il_expr_r vector;
};

struct il_id {
  il_lloc_t lloc;
  il_type_t type;

  const char *id;
  bool is_keyword;
};

struct il_const {
  il_lloc_t lloc;

  union {
    il_val_t value;

    /* only accessed, value field must be set. */
    struct {
      il_type_t type;
      union {
        const char *s;
        float f;
        double d;
        long double ld;
        unsigned long int ul;
      };
    };
  };
};

struct il_unary {
  il_lloc_t lloc;
  il_type_t type;

  enum il_op_n op;
  il_expr_t *operand;
};

struct il_binary {
  il_lloc_t lloc;
  il_type_t type;

  enum il_op_n op;
  il_expr_t *lhs, *rhs;
};

struct il_ternary {
  il_lloc_t lloc;
  il_type_t type;

  il_expr_t *lhs, *mhs, *rhs;
};

struct il_array_read {
  il_lloc_t lloc;
  il_type_t type;

  il_expr_t *lhs, *pos;
};

struct il_array_write {
  union {
    il_array_read_t read;

    /* only accessed, array_read field must be set. */
    struct {
      il_lloc_t lloc;
      il_type_t type;

      il_expr_t *lhs, *pos;
    };
  };

  il_expr_t *rhs;
};

struct il_field_read {
  il_lloc_t lloc;
  il_type_t type;

  il_expr_t *lhs;
  il_id_t id;
};

struct il_field_write {
  union {
    il_field_read_t read;

    /* only accessed, field_read field must be set. */
    struct {
      il_lloc_t lloc;
      il_type_t type;

      il_expr_t *lhs;
      il_id_t id;
    };
  };

  il_expr_t *rhs;
};

struct il_call {
  il_lloc_t lloc;
  il_type_t type;

  il_expr_t *lhs;
  il_exprs_t args;
};

struct il_expr {
  union {
    struct {
      il_lloc_t lloc;
      il_type_t type;
    };
    il_exprs_t exprs;
    il_id_t id;
    il_const_t constant;
    il_unary_t unary;
    il_binary_t binary;
    il_ternary_t ternary;
    il_array_read_t array_read;
    il_array_write_t array_write;
    il_field_read_t field_read;
    il_field_write_t field_write;
    il_call_t call;
  };
  enum il_expr_n kind;
};

typedef struct il_stmt il_stmt_t;
typedef struct il_stmt_expr il_stmt_expr_t;
typedef struct il_stmt_label il_stmt_label_t;
typedef struct il_stmt_case il_stmt_case_t;
typedef struct il_stmt_default il_stmt_default_t;
typedef struct il_stmt_compound il_stmt_compound_t;
typedef struct il_stmt_if il_stmt_if_t;
typedef struct il_stmt_switch il_stmt_switch_t;
typedef struct il_stmt_while il_stmt_while_t;
typedef struct il_stmt_do il_stmt_do_t;
typedef struct il_stmt_for il_stmt_for_t;
typedef struct il_stmt_goto il_stmt_goto_t;
typedef struct il_stmt_break il_stmt_break_t;
typedef struct il_stmt_continue il_stmt_continue_t;
typedef struct il_stmt_return il_stmt_return_t;
typedef struct il_stmt_decl il_stmt_decl_t;

typedef adt_vector_of(il_stmt_t) il_stmt_r;

enum il_stmt_n {
  IL_STMT_UNDEFINED = 0,
  IL_STMT_EXPR,
  IL_STMT_LABEL,
  IL_STMT_CASE,
  IL_STMT_DEFAULT,
  IL_STMT_COMPOUND,
  IL_STMT_IF,
  IL_STMT_SWITCH,
  IL_STMT_WHILE,
  IL_STMT_DO,
  IL_STMT_FOR,
  IL_STMT_GOTO,
  IL_STMT_BREAK,
  IL_STMT_CONTINUE,
  IL_STMT_RETURN,
  IL_STMT_DECL
};

struct il_stmt_expr {
  il_lloc_t lloc;

  il_expr_t expr;
};

struct il_stmt_label {
  il_lloc_t lloc;

  const char *name;
};

struct il_stmt_case {
  il_lloc_t lloc;

  il_expr_t cond;
  il_stmt_t *body;
};

struct il_stmt_default {
  il_lloc_t lloc;

  il_stmt_t *body;
};

struct il_stmt_compound {
  il_lloc_t lloc;

  il_stmt_r stmts;
};

struct il_stmt_if {
  il_lloc_t lloc;

  il_expr_t cond;
  il_stmt_t *then_body, *else_body;
};

struct il_stmt_switch {
  il_lloc_t lloc;

  il_expr_t cond;
  il_stmt_t *body;
};

struct il_stmt_while {
  il_lloc_t lloc;

  il_expr_t cond;
  il_stmt_t *body;
};

struct il_stmt_do {
  il_lloc_t lloc;

  il_expr_t cond;
  il_stmt_t *body;
};

struct il_stmt_for {
  il_lloc_t lloc;

  il_stmt_expr_t lhs, mhs;
  il_expr_t rhs;
  il_stmt_t *body;
};

struct il_stmt_goto {
  il_lloc_t lloc;

  const char *label;
};

struct il_stmt_break {
  il_lloc_t lloc;
};

struct il_stmt_continue {
  il_lloc_t lloc;
};

struct il_stmt_return {
  il_lloc_t lloc;

  il_expr_t expr;
};

struct il_stmt_decl {
  il_lloc_t lloc;

  struct il_entity *entity;
};

struct il_stmt {
  union {
    il_lloc_t lloc;
    il_stmt_expr_t expr;
    il_stmt_label_t label;
    il_stmt_case_t case_stmt;
    il_stmt_default_t default_stmt;
    il_stmt_compound_t compound;
    il_stmt_if_t if_stmt;
    il_stmt_switch_t switch_stmt;
    il_stmt_while_t while_stmt;
    il_stmt_do_t do_stmt;
    il_stmt_for_t for_stmt;
    il_stmt_goto_t goto_stmt;
    il_stmt_break_t break_stmt;
    il_stmt_continue_t continue_stmt;
    il_stmt_return_t return_stmt;
    il_stmt_decl_t decl;
  };
  enum il_stmt_n kind;
};


typedef struct il_entity il_entity_t;
typedef struct il_field il_field_t;
typedef struct il_var il_var_t;
typedef struct il_param il_param_t;
typedef struct il_func il_func_t;
typedef struct il_enum il_enum_t;
typedef struct il_struct il_struct_t;
typedef struct il_union il_union_t;
typedef struct il_label il_label_t;

typedef adt_vector_of(il_entity_t) il_entity_r;

enum il_entity_n {
  IL_ENTITY_UNDEFINED = 0,
  IL_ENTITY_FIELD,
  IL_ENTITY_VAR,
  IL_ENTITY_PARAM,
  IL_ENTITY_FUNC,
  IL_ENTITY_ENUM,
  IL_ENTITY_STRUCT,
  IL_ENTITY_UNION,
  IL_ENTITY_LABEL
};

struct il_field {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t type;
  const char *name;

  size_t offset;
  unsigned short field_width, field_offset;
};

struct il_var {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t type;
  const char *name;

  il_expr_t initializer;
};

struct il_param {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t type;
  const char *name;

  unsigned position;
  il_expr_t initializer;
};

struct il_func {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t return_type;
  const char *name;

  il_entity_r params;
  il_stmt_t body;
};

struct il_enum {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t type;
  const char *name;

  il_entity_r vars;
};

struct il_struct {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t type;
  const char *name;

  il_entity_r fields;
};

struct il_union {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t type;
  const char *name;

  il_entity_r fields;
};

struct il_label {
  il_lloc_t lloc;
  size_t size, align;
  il_type_t type;
  const char *name;

  il_stmt_t next;
};

struct il_entity {
  union {
    struct {
      il_lloc_t lloc;
      size_t size, align;
      il_type_t type;
      const char *name;
    };
    il_field_t field;
    il_var_t variable;
    il_param_t parameter;
    il_func_t function;
    il_enum_t enumerable;
    il_struct_t structure;
    il_union_t u_structure;
    il_label_t label;
  };
  enum il_entity_n kind;
};


typedef struct il_node il_node_t;

enum il_node_n {
  IL_NODE_UNDEFINED = 0,
  IL_NODE_ENTITY,
  IL_NODE_EXPR,
  IL_NODE_STMT,
  IL_NODE_TYPE
};

struct il_node {
  union {
    il_lloc_t lloc;
    il_entity_t entity;
    il_expr_t expr;
    il_stmt_t stmt;
    il_type_t type;
  };
  enum il_node_n kind;
};

#endif /* IL_IR_H__ */
