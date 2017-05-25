/*
 * Wulk - Wu uniform language kit
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

#ifndef   WULK_IR_H__
# define  WULK_IR_H__

#include <adt/vector.h>

#include "token.h"

struct wulk_entity;
struct wulk_expr;
struct wulk_stmt;
struct wulk_type;

typedef struct wulk_type wulk_type_t;
typedef struct wulk_literal wulk_literal_t;
typedef struct wulk_pointer wulk_pointer_t;
typedef struct wulk_array wulk_array_t;
typedef struct wulk_compound wulk_compound_t;

enum wulk_type_n {
  WULK_TYPE_UNDEFINED = 0,
  WULK_TYPE_VOID,
  WULK_TYPE_POINTER,
  WULK_TYPE_ARRAY,
  WULK_TYPE_COMPOUND,
  WULK_TYPE_BOOL,
  WULK_TYPE_CHAR,
  WULK_TYPE_SHORT,
  WULK_TYPE_INT,
  WULK_TYPE_LONG,
  WULK_TYPE_FLOAT,
  WULK_TYPE_DOUBLE,
  WULK_TYPE_LONG_LONG,
  WULK_TYPE_LONG_DOUBLE
};

enum wulk_type_specifier_n {
  WULK_TYPE_SPECIFIER_NONE = 0,
  WULK_TYPE_SPECIFIER_SIGNED = 1 << 0,
  WULK_TYPE_SPECIFIER_UNSIGNED = 1 << 1,
  WULK_TYPE_SPECIFIER_COMPLEX = 1 << 2,
  WULK_TYPE_SPECIFIER_IMAGINARY = 1 << 3,
  WULK_TYPE_SPECIFIER_TYPEDEF = 1 << 4,
  WULK_TYPE_SPECIFIER_EXTERN = 1 << 5,
  WULK_TYPE_SPECIFIER_STATIC = 1 << 6,
  WULK_TYPE_SPECIFIER_THREAD_LOCAL = 1 << 7,
  WULK_TYPE_SPECIFIER_AUTO = 1 << 8,
  WULK_TYPE_SPECIFIER_REGISTER = 1 << 9,
  WULK_FUNC_SPECIFIER_INLINE = 1 << 10,
  WULK_FUNC_SPECIFIER_NORETURN = 1 << 11
};

enum wulk_type_qualifier_n {
  WULK_TYPE_QUALIFIER_NONE = 0,
  WULK_TYPE_QUALIFIER_CONST = 1 << 0,
  WULK_TYPE_QUALIFIER_VOLATILE = 1 << 1,
  WULK_TYPE_QUALIFIER_RESTRICT = 1 << 2,
  WULK_TYPE_QUALIFIER_ATOMIC = 1 << 3
};

struct wulk_literal {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t *next;
  enum wulk_type_specifier_n specifiers;
  enum wulk_type_qualifier_n qualifiers;
};

struct wulk_pointer {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t *of;
  enum wulk_type_specifier_n specifiers;
  enum wulk_type_qualifier_n qualifiers;
};

struct wulk_array {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t *of;
  enum wulk_type_specifier_n specifiers;
  enum wulk_type_qualifier_n qualifiers;

  struct wulk_expr *length;
};

struct wulk_compound {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t *next;
  enum wulk_type_specifier_n specifiers;
  enum wulk_type_qualifier_n qualifiers;

  struct wulk_entity *entity;
};

struct wulk_type {
  union {
    struct {
      wulk_lloc_t lloc;
      size_t size, align;
      wulk_type_t *next;
      enum wulk_type_specifier_n specifiers;
      enum wulk_type_qualifier_n qualifiers;
    };
    wulk_literal_t literal;
    wulk_pointer_t pointer;
    wulk_array_t array;
    wulk_compound_t compound;
  };
  enum wulk_type_n kind;
};

struct wulk_field;

typedef struct wulk_val wulk_val_t;
typedef struct wulk_expr wulk_expr_t;
typedef struct wulk_exprs wulk_exprs_t;
typedef struct wulk_id wulk_id_t;
typedef struct wulk_const wulk_const_t;
typedef struct wulk_unary wulk_unary_t;
typedef struct wulk_binary wulk_binary_t;
typedef struct wulk_ternary wulk_ternary_t;
typedef struct wulk_array_read wulk_array_read_t;
typedef struct wulk_array_write wulk_array_write_t;
typedef struct wulk_field_read wulk_field_read_t;
typedef struct wulk_field_write wulk_field_write_t;
typedef struct wulk_call wulk_call_t;

typedef adt_vector_of(wulk_expr_t) wulk_expr_r;

enum wulk_expr_n {
  WULK_EXPR_UNDEFINED = 0,
  WULK_EXPR_EXPRS,
  WULK_EXPR_ID,
  WULK_EXPR_CONST,
  WULK_EXPR_UNARY,
  WULK_EXPR_BINARY,
  WULK_EXPR_TERNARY,
  WULK_EXPR_ARRAY_READ,
  WULK_EXPR_ARRAY_WRITE,
  WULK_EXPR_FIELD_READ,
  WULK_EXPR_FIELD_WRITE,
  WULK_EXPR_CALL
};

enum wulk_op_n {
  WULK_OP_CAST,   /* (T) l        */
  WULK_OP_EN,     /* (l)          */
  WULK_OP_CALL,   /* l()          */
  WULK_OP_VA_ARG, /* va_arg(l, T) */
  WULK_OP_NOT,    /* !l           */
  WULK_OP_ASSIGN, /* l = r        */
  WULK_OP_ADD,    /* l + r        */
  WULK_OP_SUB,    /* l - r        */
  WULK_OP_MUL,    /* l * r        */
  WULK_OP_DIV,    /* l / r        */
  WULK_OP_MOD,    /* l % r        */
  WULK_OP_AND,    /* l & r        */
  WULK_OP_LAND,    /* l && r        */
  WULK_OP_OR,     /* l | r        */
  WULK_OP_LOR,     /* l || r        */
  WULK_OP_XOR,    /* l ^ r        */
  WULK_OP_SHL,    /* l << r       */
  WULK_OP_SHR,    /* l >> r       */
  WULK_OP_EQ,     /* l == r       */
  WULK_OP_NE,     /* l != r       */
  WULK_OP_GE,     /* l >= r       */
  WULK_OP_GT,     /* l > r        */
  WULK_OP_LE,     /* l <= r       */
  WULK_OP_LT,     /* l < r        */
  WULK_OP_NEG     /* l ~ r        */
};

struct wulk_val {
  wulk_type_t type;
  union {
    const char *s;
    float f;
    double d;
    long double ld;
    unsigned long int ul;
  };
};

struct wulk_exprs {
  wulk_lloc_t lloc;
  wulk_type_t type;

  wulk_expr_r vector;
};

struct wulk_id {
  wulk_lloc_t lloc;
  wulk_type_t type;

  const char *id;
  bool is_keyword;
};

struct wulk_const {
  wulk_lloc_t lloc;

  union {
    wulk_val_t value;

    /* only accessed, value field must be set. */
    struct {
      wulk_type_t type;
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

struct wulk_unary {
  wulk_lloc_t lloc;
  wulk_type_t type;

  enum wulk_op_n op;
  wulk_expr_t *operand;
};

struct wulk_binary {
  wulk_lloc_t lloc;
  wulk_type_t type;

  enum wulk_op_n op;
  wulk_expr_t *lhs, *rhs;
};

struct wulk_ternary {
  wulk_lloc_t lloc;
  wulk_type_t type;

  wulk_expr_t *lhs, *mhs, *rhs;
};

struct wulk_array_read {
  wulk_lloc_t lloc;
  wulk_type_t type;

  wulk_expr_t *lhs, *pos;
};

struct wulk_array_write {
  union {
    wulk_array_read_t read;

    /* only accessed, array_read field must be set. */
    struct {
      wulk_lloc_t lloc;
      wulk_type_t type;

      wulk_expr_t *lhs, *pos;
    };
  };

  wulk_expr_t *rhs;
};

struct wulk_field_read {
  wulk_lloc_t lloc;
  wulk_type_t type;

  wulk_expr_t *lhs;
  wulk_id_t id;
};

struct wulk_field_write {
  union {
    wulk_field_read_t read;

    /* only accessed, field_read field must be set. */
    struct {
      wulk_lloc_t lloc;
      wulk_type_t type;

      wulk_expr_t *lhs;
      wulk_id_t id;
    };
  };

  wulk_expr_t *rhs;
};

struct wulk_call {
  wulk_lloc_t lloc;
  wulk_type_t type;

  wulk_expr_t *lhs;
  wulk_exprs_t args;
};

struct wulk_expr {
  union {
    struct {
      wulk_lloc_t lloc;
      wulk_type_t type;
    };
    wulk_exprs_t exprs;
    wulk_id_t id;
    wulk_const_t constant;
    wulk_unary_t unary;
    wulk_binary_t binary;
    wulk_ternary_t ternary;
    wulk_array_read_t array_read;
    wulk_array_write_t array_write;
    wulk_field_read_t field_read;
    wulk_field_write_t field_write;
    wulk_call_t call;
  };
  enum wulk_expr_n kind;
};

typedef struct wulk_stmt wulk_stmt_t;
typedef struct wulk_stmt_expr wulk_stmt_expr_t;
typedef struct wulk_stmt_label wulk_stmt_label_t;
typedef struct wulk_stmt_case wulk_stmt_case_t;
typedef struct wulk_stmt_default wulk_stmt_default_t;
typedef struct wulk_stmt_compound wulk_stmt_compound_t;
typedef struct wulk_stmt_if wulk_stmt_if_t;
typedef struct wulk_stmt_switch wulk_stmt_switch_t;
typedef struct wulk_stmt_while wulk_stmt_while_t;
typedef struct wulk_stmt_do wulk_stmt_do_t;
typedef struct wulk_stmt_for wulk_stmt_for_t;
typedef struct wulk_stmt_goto wulk_stmt_goto_t;
typedef struct wulk_stmt_break wulk_stmt_break_t;
typedef struct wulk_stmt_continue wulk_stmt_continue_t;
typedef struct wulk_stmt_return wulk_stmt_return_t;
typedef struct wulk_stmt_decl wulk_stmt_decl_t;

typedef adt_vector_of(wulk_stmt_t) wulk_stmt_r;

enum wulk_stmt_n {
  WULK_STMT_UNDEFINED = 0,
  WULK_STMT_EXPR,
  WULK_STMT_LABEL,
  WULK_STMT_CASE,
  WULK_STMT_DEFAULT,
  WULK_STMT_COMPOUND,
  WULK_STMT_IF,
  WULK_STMT_SWITCH,
  WULK_STMT_WHILE,
  WULK_STMT_DO,
  WULK_STMT_FOR,
  WULK_STMT_GOTO,
  WULK_STMT_BREAK,
  WULK_STMT_CONTINUE,
  WULK_STMT_RETURN,
  WULK_STMT_DECL
};

struct wulk_stmt_expr {
  wulk_lloc_t lloc;

  wulk_expr_t expr;
};

struct wulk_stmt_label {
  wulk_lloc_t lloc;

  const char *name;
};

struct wulk_stmt_case {
  wulk_lloc_t lloc;

  wulk_expr_t cond;
  wulk_stmt_t *body;
};

struct wulk_stmt_default {
  wulk_lloc_t lloc;

  wulk_stmt_t *body;
};

struct wulk_stmt_compound {
  wulk_lloc_t lloc;

  wulk_stmt_r stmts;
};

struct wulk_stmt_if {
  wulk_lloc_t lloc;

  wulk_expr_t cond;
  wulk_stmt_t *then_body, *else_body;
};

struct wulk_stmt_switch {
  wulk_lloc_t lloc;

  wulk_expr_t cond;
  wulk_stmt_t *body;
};

struct wulk_stmt_while {
  wulk_lloc_t lloc;

  wulk_expr_t cond;
  wulk_stmt_t *body;
};

struct wulk_stmt_do {
  wulk_lloc_t lloc;

  wulk_expr_t cond;
  wulk_stmt_t *body;
};

struct wulk_stmt_for {
  wulk_lloc_t lloc;

  wulk_stmt_expr_t lhs, mhs;
  wulk_expr_t rhs;
  wulk_stmt_t *body;
};

struct wulk_stmt_goto {
  wulk_lloc_t lloc;

  const char *label;
};

struct wulk_stmt_break {
  wulk_lloc_t lloc;
};

struct wulk_stmt_continue {
  wulk_lloc_t lloc;
};

struct wulk_stmt_return {
  wulk_lloc_t lloc;

  wulk_expr_t expr;
};

struct wulk_stmt_decl {
  wulk_lloc_t lloc;

  struct wulk_entity *entity;
};

struct wulk_stmt {
  union {
    wulk_lloc_t lloc;
    wulk_stmt_expr_t expr;
    wulk_stmt_label_t label;
    wulk_stmt_case_t case_stmt;
    wulk_stmt_default_t default_stmt;
    wulk_stmt_compound_t compound;
    wulk_stmt_if_t if_stmt;
    wulk_stmt_switch_t switch_stmt;
    wulk_stmt_while_t while_stmt;
    wulk_stmt_do_t do_stmt;
    wulk_stmt_for_t for_stmt;
    wulk_stmt_goto_t goto_stmt;
    wulk_stmt_break_t break_stmt;
    wulk_stmt_continue_t continue_stmt;
    wulk_stmt_return_t return_stmt;
    wulk_stmt_decl_t decl;
  };
  enum wulk_stmt_n kind;
};


typedef struct wulk_entity wulk_entity_t;
typedef struct wulk_field wulk_field_t;
typedef struct wulk_var wulk_var_t;
typedef struct wulk_param wulk_param_t;
typedef struct wulk_func wulk_func_t;
typedef struct wulk_enum wulk_enum_t;
typedef struct wulk_struct wulk_struct_t;
typedef struct wulk_union wulk_union_t;
typedef struct wulk_label wulk_label_t;

typedef adt_vector_of(wulk_entity_t) wulk_entity_r;

enum wulk_entity_n {
  WULK_ENTITY_UNDEFINED = 0,
  WULK_ENTITY_FIELD,
  WULK_ENTITY_VAR,
  WULK_ENTITY_PARAM,
  WULK_ENTITY_FUNC,
  WULK_ENTITY_ENUM,
  WULK_ENTITY_STRUCT,
  WULK_ENTITY_UNION,
  WULK_ENTITY_LABEL
};

struct wulk_field {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t type;
  const char *name;

  size_t offset;
  unsigned short field_width, field_offset;
};

struct wulk_var {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t type;
  const char *name;

  wulk_expr_t initializer;
};

struct wulk_param {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t type;
  const char *name;

  unsigned position;
  wulk_expr_t initializer;
};

struct wulk_func {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t return_type;
  const char *name;

  wulk_entity_r params;
  wulk_stmt_t body;
};

struct wulk_enum {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t type;
  const char *name;

  wulk_entity_r vars;
};

struct wulk_struct {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t type;
  const char *name;

  wulk_entity_r fields;
};

struct wulk_union {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t type;
  const char *name;

  wulk_entity_r fields;
};

struct wulk_label {
  wulk_lloc_t lloc;
  size_t size, align;
  wulk_type_t type;
  const char *name;

  wulk_stmt_t next;
};

struct wulk_entity {
  union {
    struct {
      wulk_lloc_t lloc;
      size_t size, align;
      wulk_type_t type;
      const char *name;
    };
    wulk_field_t field;
    wulk_var_t variable;
    wulk_param_t parameter;
    wulk_func_t function;
    wulk_enum_t enumerable;
    wulk_struct_t structure;
    wulk_union_t u_structure;
    wulk_label_t label;
  };
  enum wulk_entity_n kind;
};


typedef struct wulk_node wulk_node_t;

enum wulk_node_n {
  WULK_NODE_UNDEFINED = 0,
  WULK_NODE_ENTITY,
  WULK_NODE_EXPR,
  WULK_NODE_STMT,
  WULK_NODE_TYPE
};

struct wulk_node {
  union {
    wulk_lloc_t lloc;
    wulk_entity_t entity;
    wulk_expr_t expr;
    wulk_stmt_t stmt;
    wulk_type_t type;
  };
  enum wulk_node_n kind;
};

#endif /* WULK_IR_H__ */
