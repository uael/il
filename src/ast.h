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

#ifndef _AST_H
#define _AST_H

#include <vector>
#include <string>
#include <tuple>
#include <map>
#include "generator.h"

using namespace std;

namespace dyc {
  namespace ast {
    struct identifier_t;
    typedef vector<identifier_t *> id_list_t;

    struct generic_t;
    typedef vector<generic_t *> generic_list_t;
    struct closure_t;

    struct decl_t;
    typedef vector<decl_t *> decl_list_t;
    struct decl_property_t;
    struct decl_function_t;

    struct type_specifier_t;
    typedef vector<type_specifier_t *> type_specifier_list_t;
    struct type_t;
    struct type_scalar_t;
    struct type_generic_t;

    struct stmt_t;
    typedef vector<stmt_t *> stmt_list_t;
    struct stmt_expr_t;
    struct stmt_label_t;
    struct stmt_compound_t;
    struct stmt_select_t;
    struct stmt_iter_t;
    struct stmt_jump_t;
    struct stmt_decl_t;

    struct expr_t;
    struct expr_assign_t;
    struct expr_cond_t;
    struct expr_lor_t;
    struct expr_land_t;
    struct expr_or_t;
    struct expr_xor_t;
    struct expr_and_t;
    struct expr_relational_t;
    struct expr_add_t;
    struct expr_shift_t;
    struct expr_mul_t;
    struct expr_equal_t;
    struct expr_cast_t;
    struct expr_prefix_t;
    struct expr_postfix_t;
    struct expr_primary_t;
    struct expr_const_t;

    struct const_value_t;
    struct const_lambda_t;
    struct const_initializer_t;

    typedef map<expr_cond_t *, expr_t *> ds_map_t;

    struct ast_t {
      decl_list_t *decls;
      ast_t() {};
      ast_t(decl_list_t *decls) : decls(decls) {};
    };

    struct node_t {
      virtual ~node_t() {}

      virtual bool write(generator_t::writer_t *writer, ast_t *ast) {
        return false;
      };
    };

    struct generic_t : node_t {
      string *value;
      type_specifier_t *type_specifier;

      generic_t(string *value, type_specifier_t *type_specifier)
        : value(value), type_specifier(type_specifier) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return node_t::write(writer, ast);
      }
    };

    struct closure_t : node_t {};

    struct decl_t : node_t {
      id_list_t *ids;
      type_specifier_t *type_specifier;
      closure_t *closure;

      decl_t(id_list_t *ids, type_specifier_t *type_specifier, closure_t *closure)
        : ids(ids), type_specifier(type_specifier), closure(closure) {}
    };

    struct decl_property_t : decl_t {
      bool assigned;

      decl_property_t(id_list_t *ids, type_specifier_t *type_specifier, closure_t *closure, bool assigned)
        : decl_t(ids, type_specifier, closure), assigned(assigned) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return decl_t::write(writer, ast);
      }
    };

    struct decl_function_t : decl_t {
      generic_list_t *generics;
      decl_list_t *args;

      decl_function_t(id_list_t *ids, generic_list_t *generics, decl_list_t *args, type_specifier_t *type_specifier,
                      closure_t *closure)
        : decl_t(ids, type_specifier, closure), generics(generics), args(args) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return decl_t::write(writer, ast);
      }
    };

    struct type_specifier_t : node_t {
      type_t *type;
      vector<type_specifier_list_t *> call_chain;
      decl_list_t *decls;
      type_specifier_t *next;
      int ptr_lvl;
      int array_lvl;

      type_specifier_t(type_t *type)
        : type(type) {}

      type_specifier_t(type_t *type, decl_list_t *decls)
        : type(type), decls(decls) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return node_t::write(writer, ast);
      }
    };

    struct type_t : node_t {};

    struct type_scalar_t : type_t {
      enum kind_t {
        VOID, BOOL, CHAR, INT, UINT, SINT, SHORT, USHORT,
        SSHORT, FLOAT, UFLOAT, SFLOAT, DOUBLE, UDOUBLE, SDOUBLE
      } kind;

      type_scalar_t(kind_t kind)
        : kind(kind) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return type_t::write(writer, ast);
      }
    };

    struct type_generic_t : type_t {
      string *id;

      type_generic_t(string *id)
        : id(id) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return type_t::write(writer, ast);
      }
    };

    struct stmt_t : node_t {};

    struct stmt_expr_t : stmt_t {
      expr_t *expr;

      stmt_expr_t() {}
      stmt_expr_t(expr_t *expr)
        : expr(expr) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return stmt_t::write(writer, ast);
      }
    };

    struct stmt_label_t : stmt_t {
      enum kind_t {
        LABEL, CASE, DEFAULT
      } kind;
      string *id;
      stmt_t *stmt;
      expr_cond_t *cond;

      stmt_label_t(stmt_t *stmt)
        : kind(DEFAULT), stmt(stmt) {}
      stmt_label_t(string *id, stmt_t *stmt)
        : kind(LABEL), id(id), stmt(stmt) {}
      stmt_label_t(expr_cond_t *cond, stmt_t *stmt)
        : kind(CASE), stmt(stmt), cond(cond) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return stmt_t::write(writer, ast);
      }
    };

    struct stmt_compound_t : stmt_t, closure_t {
      stmt_list_t *stmts;

      stmt_compound_t() {}
      stmt_compound_t(stmt_list_t *stmts)
        : stmts(stmts) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return stmt_t::write(writer, ast);
      }
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
        : kind(kind), cond(cond), stmt(stmt) {}
      stmt_select_t(expr_t *cond, stmt_t *stmt, stmt_t *else_stmt)
        : kind(IF), cond(cond), stmt(stmt), else_stmt(else_stmt) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return stmt_t::write(writer, ast);
      }
    };

    struct stmt_iter_t : stmt_t {
      enum kind_t {
        WHILE, DO_WHILE, FOR
      } kind;
      stmt_t *decls;
      expr_t *cond;
      closure_t *step;
      stmt_t *stmt;

      stmt_iter_t(expr_t *cond, stmt_t *stmt)
        : kind(WHILE), cond(cond), stmt(stmt) {}
      stmt_iter_t(stmt_t *stmt, expr_t *cond)
        : kind(DO_WHILE), cond(cond), stmt(stmt) {}
      stmt_iter_t(expr_t *cond, closure_t *step, stmt_t *stmt)
        : kind(FOR), cond(cond), step(step), stmt(stmt) {}
      stmt_iter_t(stmt_t *decls, expr_t *cond, closure_t *step, stmt_t *stmt)
        : kind(FOR), decls(decls), cond(cond), step(step), stmt(stmt) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return stmt_t::write(writer, ast);
      }
    };

    struct stmt_jump_t : stmt_t {
      enum kind_t {
        GOTO, CONTINUE, BREAK, RETURN
      } kind;
      string *id;
      expr_t *expr;

      stmt_jump_t(kind_t kind)
        : kind(kind) {}
      stmt_jump_t(string *id)
        : kind(GOTO), id(id) {}
      stmt_jump_t(expr_t *expr)
        : kind(RETURN), expr(expr) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return stmt_t::write(writer, ast);
      }
    };

    struct stmt_decl_t : stmt_t {
      decl_list_t *decls;

      stmt_decl_t() {}
      stmt_decl_t(decl_list_t *decls)
        : decls(decls) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return stmt_t::write(writer, ast);
      }
    };

    struct expr_t : closure_t {
      expr_t *next;

      expr_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return closure_t::write(writer, ast);
      }
    };

    struct expr_assign_t : expr_t {
      vector<expr_prefix_t *> assign_chain;
      vector<expr_prefix_t *> mul_assign_chain;
      vector<expr_prefix_t *> div_assign_chain;
      vector<expr_prefix_t *> mod_assign_chain;
      vector<expr_prefix_t *> add_assign_chain;
      vector<expr_prefix_t *> sub_assign_chain;
      vector<expr_prefix_t *> left_assign_chain;
      vector<expr_prefix_t *> right_assign_chain;
      vector<expr_prefix_t *> and_assign_chain;
      vector<expr_prefix_t *> xor_assign_chain;
      vector<expr_prefix_t *> or_assign_chain;

      expr_assign_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_t::write(writer, ast);
      }
    };

    struct expr_cond_t : expr_assign_t {
      vector<tuple<expr_lor_t *, expr_t *>> ternary_chain;

      expr_cond_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_assign_t::write(writer, ast);
      }
    };

    struct expr_lor_t : expr_cond_t {
      vector<expr_land_t *> lor_chain;

      expr_lor_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_cond_t::write(writer, ast);
      }
    };

    struct expr_land_t : expr_lor_t {
      vector<expr_or_t *> land_chain;

      expr_land_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_lor_t::write(writer, ast);
      }
    };

    struct expr_or_t : expr_land_t {
      vector<expr_xor_t *> or_chain;

      expr_or_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_land_t::write(writer, ast);
      }
    };

    struct expr_xor_t : expr_or_t {
      vector<expr_and_t *> xor_chain;

      expr_xor_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_or_t::write(writer, ast);
      }
    };

    struct expr_and_t : expr_xor_t {
      vector<expr_equal_t *> and_chain;

      expr_and_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_xor_t::write(writer, ast);
      }
    };

    struct expr_equal_t : expr_and_t {
      vector<expr_relational_t *> eq_chain;
      vector<expr_relational_t *> neq_chain;

      expr_equal_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_and_t::write(writer, ast);
      }
    };

    struct expr_relational_t : expr_equal_t {
      vector<expr_shift_t *> lt_chain;
      vector<expr_shift_t *> gt_chain;
      vector<expr_shift_t *> le_chain;
      vector<expr_shift_t *> ge_chain;

      expr_relational_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_equal_t::write(writer, ast);
      }
    };

    struct expr_shift_t : expr_relational_t {
      vector<expr_add_t *> ls_chain;
      vector<expr_add_t *> rs_chain;

      expr_shift_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_relational_t::write(writer, ast);
      }
    };

    struct expr_add_t : expr_shift_t {
      vector<expr_mul_t *> add_chain;
      vector<expr_mul_t *> sub_chain;

      expr_add_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_shift_t::write(writer, ast);
      }
    };

    struct expr_mul_t : expr_add_t {
      vector<expr_cast_t *> mul_chain;
      vector<expr_cast_t *> div_chain;
      vector<expr_cast_t *> mod_chain;

      expr_mul_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_add_t::write(writer, ast);
      }
    };

    struct expr_cast_t : expr_mul_t {
      vector<type_specifier_t *> cast_chain;

      expr_cast_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_mul_t::write(writer, ast);
      }
    };

    struct expr_prefix_t : expr_cast_t {
      int inc_lvl;
      int dec_lvl;
      int and_lvl;
      int add_lvl;
      int sub_lvl;
      int mul_lvl;
      int not_lvl;
      int tid_lvl;

      expr_prefix_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_cast_t::write(writer, ast);
      }
    };

    struct expr_postfix_t : expr_prefix_t {
      vector<expr_t *> position_chain;
      vector<expr_t *> call_chain;
      int post_inc_lvl = 0;
      int post_dec_lvl = 0;

      expr_postfix_t() {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_prefix_t::write(writer, ast);
      }
    };

    struct expr_primary_t : expr_postfix_t {
      expr_const_t *const_expr;
      expr_t *expr;

      expr_primary_t() {}
      expr_primary_t(expr_const_t *const_expr)
        : const_expr(const_expr) {}
      expr_primary_t(expr_t *expr)
        : expr(expr) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_postfix_t::write(writer, ast);
      }
    };

    struct expr_const_t : node_t {};

    struct const_value_t : expr_const_t {
      enum kind_t {
        INT, FLOAT, STRING
      } kind;
      string *value;

      const_value_t(kind_t kind, string *value)
       : kind(kind), value(value) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_const_t::write(writer, ast);
      }
    };

    struct const_lambda_t : expr_const_t {
      identifier_t *arg;
      id_list_t *args;
      closure_t *closure;

      const_lambda_t(id_list_t *args, closure_t *closure)
        : args(args), closure(closure) {}
      const_lambda_t(identifier_t *arg, closure_t *closure)
        : arg(arg), closure(closure) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_const_t::write(writer, ast);
      }
    };

    struct const_initializer_t : expr_const_t {
      expr_t *list;
      ds_map_t *map;

      const_initializer_t(expr_t *list)
        : list(list) {}
      const_initializer_t(ds_map_t *map)
        : map(map) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return expr_const_t::write(writer, ast);
      }
    };

    struct identifier_t : expr_const_t {
      string *value;
      string uk_value;

      identifier_t(string *value) : value(value) {}

      bool write(generator_t::writer_t *writer, ast_t *ast) override {
        return node_t::write(writer, ast);
      }
    };
  }
}

#endif /* _AST_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */