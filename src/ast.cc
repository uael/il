/*
 *  MIT License
 *
 * Copyright (c) 2016 .DRY (Don't Repeat Yourself)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* $Id$ */

#include "ast.h"

namespace ddc {
  namespace ast {
    struct id_t {
      string value;
    };

    struct generic_t {
      string value;
      type_specifier_t *type_specifier;
    };

    struct closure_t {};

    struct decl_t {
      id_list_t *ids;
      type_specifier_t *type_specifier;
      closure_t *closure;
    };

    struct decl_property_t : decl_t {
      bool assigned;
    };

    struct decl_function_t : decl_t {
      generic_list_t *generics;
      decl_list_t *args;
    };

    struct type_specifier_t {
      type_t *type;
      type_specifier_list_t *args;
      decl_list_t *decls;
      int ptr_lvl;
      int array_lvl;
    };
    
    struct type_t {};
    
    struct type_scalar_t : type_t {
      enum {
        VOID, BOOL, CHAR, INT, UINT, SINT, SHORT, USHORT,
        SSHORT, FLOAT, UFLOAT, SFLOAT, DOUBLE, UDOUBLE, SDOUBLE
      } kind;
    };

    struct type_generic_t {
      string id;
    };

    struct stmt_t {};

    struct stmt_expr_t : stmt_t {
      expr_t *expr;
    };

    struct stmt_stmt_t : stmt_t {
      stmt_t *stmt;
    };

    struct stmt_label_t : stmt_stmt_t {
      id_t *id;
    };

    struct stmt_case_t : stmt_stmt_t {
      expr_cond_t *cond;
    };

    struct stmt_default_t : stmt_stmt_t {};

    struct stmt_compound_t : stmt_t, closure_t {
      stmt_list_t *stmts;
    };

    struct stmt_select_t : stmt_t {
      expr_t *cond;
      stmt_t *stmt;
    };

    struct stmt_if_t : stmt_select_t {
      stmt_t *else_stmt;
    };

    struct stmt_switch_t : stmt_select_t {};

    struct stmt_iter_t : stmt_t {
      stmt_t *stmt;
    };

    struct stmt_while_t : stmt_iter_t {
      expr_t *cond;
    };

    struct stmt_do_while_t : stmt_iter_t {
      expr_t *cond;
    };

    struct stmt_for_t : stmt_iter_t {
      stmt_decl_t *decl;
      stmt_expr_t *cond;
      closure_t *step;
    };

    struct stmt_jump_t : stmt_t {};

    struct stmt_goto_t : stmt_jump_t {
      id_t *id;
    };

    struct stmt_continue_t : stmt_jump_t {};

    struct stmt_break_t : stmt_jump_t {};

    struct stmt_return_t : stmt_jump_t {
      expr_t *expr;
    };

    struct stmt_decl_t : stmt_t {
      decl_list_t *decls;
    };

    struct expr_t {};

    struct expr_assign_t : expr_t {
      enum {
        ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        ADD_ASSIGN,
        SUB_ASSIGN,
        LEFT_ASSIGN,
        RIGHT_ASSIGN,
        AND_ASSIGN,
        XOR_ASSIGN,
        OR_ASSIGN
      } kind;
      expr_prefix_t *left;
      expr_assign_t *right;
    };

    struct expr_cond_t : expr_assign_t {
      expr_lor_t *cond;
      expr_t *first;
      expr_cond_t *second;
    };

    struct expr_lor_t : expr_cond_t {
      expr_lor_t *left;
      expr_land_t *right;
    };

    struct expr_land_t : expr_lor_t {
      expr_land_t *left;
      expr_or_t *right;
    };

    struct expr_or_t : expr_land_t {
      expr_or_t *left;
      expr_xor_t *right;
    };

    struct expr_xor_t : expr_or_t {
      expr_xor_t *left;
      expr_and_t *right;
    };

    struct expr_and_t : expr_xor_t {
      expr_and_t *left;
      expr_equal_t *right;
    };

    struct expr_equal_t : expr_and_t {
      enum {
        EQ, NEQ
      } kind;
      expr_equal_t *left;
      expr_relational_t *right;
    };

    struct expr_relational_t : expr_equal_t {
      enum {
        LT, GT, LE, GE
      } kind;
      expr_relational_t *left;
      expr_shift_t *right;
    };

    struct expr_shift_t : expr_relational_t {
      enum {
        LS, RS
      } kind;
      expr_shift_t *left;
      expr_add_t *right;
    };

    struct expr_add_t : expr_shift_t {
      enum {
        ADD, SUB
      } kind;
      expr_add_t *left;
      expr_mul_t *right;
    };

    struct expr_mul_t : expr_add_t {
      enum {
        MUL, DIV, MOD
      } kind;
      expr_mul_t *left;
      expr_cast_t *right;
    };

    struct expr_cast_t : expr_mul_t {
      type_specifier_t *type_specifier;
    };

    struct expr_prefix_t : expr_cast_t {
      expr_prefix_t *prefix;
      int inc_lvl;
      int dec_lvl;
      int and_lvl;
      int add_lvl;
      int sub_lvl;
      int mul_lvl;
      int not_lvl;
      int tid_lvl;
    };

    struct expr_postfix_t : expr_prefix_t {
      expr_postfix_t *postfix;
      int inc_lvl;
      int dec_lvl;
    };

    struct expr_array_access_t : expr_postfix_t {
      expr_t *position;
    };

    struct expr_call_t : expr_postfix_t {
      expr_list_t *args;
    };

    struct expr_primary_t : expr_postfix_t {
      id_t *id;
      expr_t *expr;
    };

    struct expr_const_t : expr_primary_t {
      enum {
        INT, FLOAT, STRING, LAMBDA
      } kind;
    };
    struct expr_const_value_t : expr_const_t {
      string value;
    };
    struct expr_lambda_t : expr_const_t {
      id_list_t *args;
      closure_t *closure;
    };
  }
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */