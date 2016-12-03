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

      id_t(const string &value)
        : value(value) {}
    };

    struct generic_t {
      string value;
      type_specifier_t *type_specifier;

      generic_t(const string &value, type_specifier_t *type_specifier)
        : value(value), type_specifier(type_specifier) {}
    };

    struct closure_t {};

    struct decl_t {
      id_list_t ids;
      type_specifier_t *type_specifier;
      closure_t *closure;

      decl_t(id_list_t ids, type_specifier_t *type_specifier, closure_t *closure) : ids(ids), type_specifier(
        type_specifier), closure(closure) {}
    };

    struct decl_property_t : decl_t {
      bool assigned;

      decl_property_t(id_list_t ids, type_specifier_t *type_specifier, closure_t *closure, bool assigned)
        : decl_t(ids, type_specifier, closure), assigned(assigned) {}
    };

    struct decl_function_t : decl_t {
      generic_list_t generics;
      decl_list_t args;

      decl_function_t(id_list_t ids, type_specifier_t *type_specifier, closure_t *closure,
                      generic_list_t generics, decl_list_t args)
        : decl_t(ids, type_specifier, closure), generics(generics), args(args) {}
    };

    struct type_specifier_t {
      type_t *type;
      vector<type_specifier_list_t *> call_chain;
      decl_list_t decls;
      int ptr_lvl;
      int array_lvl;

      type_specifier_t(type_t *type)
        : type(type) {}
    };
    
    struct type_t {};
    
    struct type_scalar_t : type_t {
      enum kind_t {
        VOID, BOOL, CHAR, INT, UINT, SINT, SHORT, USHORT,
        SSHORT, FLOAT, UFLOAT, SFLOAT, DOUBLE, UDOUBLE, SDOUBLE
      } kind;

      type_scalar_t(kind_t kind)
        : kind(kind) {}
    };

    struct type_generic_t {
      string id;

      type_generic_t(const string &id)
        : id(id) {}
    };

    struct stmt_t {};

    struct stmt_expr_t : stmt_t {
      expr_t *expr;

      stmt_expr_t() {}
      stmt_expr_t(expr_t *expr)
        : expr(expr) {}
    };

    struct stmt_label_t : stmt_t {
      enum kind_t {
        LABEL, CASE, DEFAULT
      } kind;
      id_t *id;
      stmt_t *stmt;
      expr_cond_t *cond;

      stmt_label_t(stmt_t *stmt)
        : stmt(stmt), kind(DEFAULT) {}
      stmt_label_t(id_t *id, stmt_t *stmt)
        : id(id), stmt(stmt), kind(LABEL) {}
      stmt_label_t(expr_cond_t *cond, stmt_t *stmt)
        : stmt(stmt), cond(cond), kind(CASE) {}
    };

    struct stmt_compound_t : stmt_t, closure_t {
      stmt_list_t stmts;

      stmt_compound_t(stmt_list_t stmts)
        : stmts(stmts) {}
    };

    struct stmt_select_t : stmt_t {
      enum kind_t {
        IF, SWITCH
      } kind;
      expr_t *cond;
      stmt_t *stmt;
      stmt_t *else_stmt;

      stmt_select_t() {}
      stmt_select_t(kind_t kind, expr_t *cond, stmt_t *stmt)
        : cond(cond), stmt(stmt), kind(kind) {}
      stmt_select_t(expr_t *cond, stmt_t *stmt, stmt_t *else_stmt)
        : cond(cond), stmt(stmt), else_stmt(else_stmt), kind(IF) {}
    };

    struct stmt_iter_t : stmt_t {
      enum kind_t {
        WHILE, DO_WHILE, FOR
      } kind;
      decl_list_t decls;
      expr_t *cond;
      stmt_t *stmt;
      closure_t *step;

      stmt_iter_t(expr_t *cond, stmt_t *stmt)
        : cond(cond), stmt(stmt), kind(WHILE) {}
      stmt_iter_t(stmt_t *stmt, expr_t *cond)
        : stmt(stmt), cond(cond), kind(DO_WHILE) {}
      stmt_iter_t(expr_t *cond, closure_t *step, stmt_t *stmt)
        : cond(cond), step(step), stmt(stmt) {}
      stmt_iter_t(decl_list_t decls, expr_t *cond, closure_t *step, stmt_t *stmt)
        : decls(decls), cond(cond), step(step), stmt(stmt) {}
    };

    struct stmt_jump_t : stmt_t {
      enum kind_t {
        GOTO, CONTINUE, BREAK, RETURN
      } kind;
      id_t *id;
      expr_t *expr;

      stmt_jump_t(kind_t kind)
        : kind(kind) {}
      stmt_jump_t(id_t *id)
        : id(id), kind(GOTO) {}
      stmt_jump_t(expr_t *expr)
        : expr(expr), kind(RETURN) {}
    };

    struct stmt_decl_t : stmt_t {
      decl_list_t decls;

      stmt_decl_t() {}
      stmt_decl_t(decl_list_t decls)
        : decls(decls) {}
    };

    struct expr_t {};

    struct expr_assign_t : expr_t {
      enum kind_t {
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

      expr_assign_t() {}
      expr_assign_t(kind_t kind, expr_prefix_t *left, expr_assign_t *right)
        : kind(kind), left(left), right(right) {}
    };

    struct expr_cond_t : expr_assign_t, closure_t {
      expr_lor_t *cond;
      expr_t *first;
      expr_cond_t *second;

      expr_cond_t() {}
      expr_cond_t(expr_lor_t *cond, expr_t *first, expr_cond_t *second)
        : cond(cond), first(first), second(second) {}
    };

    struct expr_lor_t : expr_cond_t {
      expr_lor_t *left;
      expr_land_t *right;

      expr_lor_t() {}
      expr_lor_t(expr_lor_t *left, expr_land_t *right)
        : left(left), right(right) {}
    };

    struct expr_land_t : expr_lor_t {
      expr_land_t *left;
      expr_or_t *right;

      expr_land_t() {}
      expr_land_t(expr_land_t *left, expr_or_t *right)
        : left(left), right(right) {}
    };

    struct expr_or_t : expr_land_t {
      expr_or_t *left;
      expr_xor_t *right;

      expr_or_t() {}
      expr_or_t(expr_or_t *left, expr_xor_t *right)
        : left(left), right(right) {}
    };

    struct expr_xor_t : expr_or_t {
      expr_xor_t *left;
      expr_and_t *right;

      expr_xor_t() {}
      expr_xor_t(expr_xor_t *left, expr_and_t *right)
        : left(left), right(right) {}
    };

    struct expr_and_t : expr_xor_t {
      expr_and_t *left;
      expr_equal_t *right;

      expr_and_t() {}
      expr_and_t(expr_and_t *left, expr_equal_t *right)
        : left(left), right(right) {}
    };

    struct expr_equal_t : expr_and_t {
      enum kind_t {
        EQ, NEQ
      } kind;
      expr_equal_t *left;
      expr_relational_t *right;

      expr_equal_t() {}
      expr_equal_t(kind_t kind, expr_equal_t *left, expr_relational_t *right)
        : kind(kind), left(left), right(right) {}
    };

    struct expr_relational_t : expr_equal_t {
      enum kind_t {
        LT, GT, LE, GE
      } kind;
      expr_relational_t *left;
      expr_shift_t *right;

      expr_relational_t() {}
      expr_relational_t(kind_t kind, expr_relational_t *left, expr_shift_t *right)
        : kind(kind), left(left), right(right) {}
    };

    struct expr_shift_t : expr_relational_t {
      enum kind_t {
        LS, RS
      } kind;
      expr_shift_t *left;
      expr_add_t *right;

      expr_shift_t() {}
      expr_shift_t(kind_t kind, expr_shift_t *left, expr_add_t *right)
        : kind(kind), left(left), right(right) {}
    };

    struct expr_add_t : expr_shift_t {
      enum kind_t {
        ADD, SUB
      } kind;
      expr_add_t *left;
      expr_mul_t *right;

      expr_add_t() {}
      expr_add_t(kind_t kind, expr_add_t *left, expr_mul_t *right)
        : kind(kind), left(left), right(right) {}
    };

    struct expr_mul_t : expr_add_t {
      enum kind_t {
        MUL, DIV, MOD
      } kind;
      expr_mul_t *left;
      expr_cast_t *right;

      expr_mul_t() {}
      expr_mul_t(kind_t kind, expr_mul_t *left, expr_cast_t *right)
        : kind(kind), left(left), right(right) {}
    };

    struct expr_cast_t : expr_mul_t {
      expr_prefix_t *prefix;
      vector<type_specifier_t *> type_specifier_chain;

      expr_cast_t() {}
      expr_cast_t(expr_prefix_t *prefix)
        : prefix(prefix) {}
    };

    struct expr_prefix_t : expr_cast_t {
      expr_postfix_t *postfix;
      int inc_lvl;
      int dec_lvl;
      int and_lvl;
      int add_lvl;
      int sub_lvl;
      int mul_lvl;
      int not_lvl;
      int tid_lvl;

      expr_prefix_t() {}
      expr_prefix_t(expr_postfix_t *postfix)
        : postfix(postfix) {}
    };

    struct expr_postfix_t : expr_prefix_t {
      expr_primary_t *primary;
      vector<expr_t *> position_chain;
      vector<expr_list_t *> call_chain;
      int inc_lvl;
      int dec_lvl;

      expr_postfix_t() {}
      expr_postfix_t(expr_primary_t *primary)
        : primary(primary) {}
    };

    struct expr_primary_t : expr_postfix_t {
      enum kind_t {
        ID, CONST, ENCLOSED_EXPR
      } kind;
      id_t *id;
      expr_const_t *const_expr;
      expr_t *expr;

      expr_primary_t() {}
      expr_primary_t(id_t *id)
        : id(id), kind(ID) {}
      expr_primary_t(expr_const_t *const_expr)
        : const_expr(const_expr), kind(CONST) {}
      expr_primary_t(expr_t *expr)
        : expr(expr), kind(ENCLOSED_EXPR) {}
    };

    struct expr_const_t : expr_primary_t {
      enum kind_t {
        INT, FLOAT, STRING, LAMBDA
      } kind;
      string value;
      id_list_t args;
      closure_t *closure;

      expr_const_t() {}
      expr_const_t(kind_t kind, const string &value)
        : kind(kind), value(value) {}
      expr_const_t(id_list_t args, closure_t *closure)
        : args(args), closure(closure), kind(LAMBDA) {}
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