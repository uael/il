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

#include "stmt.h"
#include "expr.h"
#include "decl.h"

namespace dyc {
  namespace ast {
    stmt_expr_t::stmt_expr_t() {}
    stmt_expr_t::stmt_expr_t(expr_t *expr) : expr(expr) {}

    void stmt_expr_t::accept(node_t *scope) {
      node_t::accept(scope);
      ACCEPT(expr);
    }

    bool stmt_expr_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return stmt_t::write(writer, ast);
    }

    stmt_label_t::stmt_label_t(stmt_t *stmt) : kind(DEFAULT), stmt(stmt) {}
    stmt_label_t::stmt_label_t(std::string *id, stmt_t *stmt) : kind(LABEL), id(id), stmt(stmt) {}
    stmt_label_t::stmt_label_t(expr_cond_t *cond, stmt_t *stmt) : kind(CASE), stmt(stmt), cond(cond) {}

    void stmt_label_t::accept(node_t *scope) {
      node_t::accept(scope);
      ACCEPT(stmt);
      ACCEPT(cond);
    }

    bool stmt_label_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return stmt_t::write(writer, ast);
    }

    stmt_compound_t::stmt_compound_t(stmt_t *stmts) : stmts(stmts) {}
    stmt_compound_t::stmt_compound_t() {}

    void stmt_compound_t::accept(node_t *scope) {
      node_t::accept(scope);
      if (stmts) stmts->accept(this);
    }

    bool stmt_compound_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return stmt_t::write(writer, ast);
    }

    stmt_select_t::stmt_select_t() {}
    stmt_select_t::stmt_select_t(stmt_select_t::kind_t kind, expr_t *cond, stmt_t *stmt)
      : kind(kind), cond(cond), stmt(stmt) {}
    stmt_select_t::stmt_select_t(expr_t *cond, stmt_t *stmt, stmt_t *else_stmt)
      : kind(IF), cond(cond), stmt(stmt), else_stmt(else_stmt) {}

    void stmt_select_t::accept(node_t *scope) {
      node_t::accept(scope);
      ACCEPT(cond);
      ACCEPT(stmt);
      ACCEPT(else_stmt);
    }

    bool stmt_select_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return stmt_t::write(writer, ast);
    }

    stmt_iter_t::stmt_iter_t(expr_t *cond, stmt_t *stmt) : kind(WHILE), cond(cond), stmt(stmt) {}
    stmt_iter_t::stmt_iter_t(stmt_t *stmt, expr_t *cond) : kind(DO_WHILE), cond(cond), stmt(stmt) {}
    stmt_iter_t::stmt_iter_t(expr_t *cond, closure_t *step, stmt_t *stmt)
      : kind(FOR), cond(cond), step(step), stmt(stmt) {}
    stmt_iter_t::stmt_iter_t(stmt_t *decls, expr_t *cond, closure_t *step, stmt_t *stmt)
      : kind(FOR), decls(decls), cond(cond), step(step), stmt(stmt) {}

    void stmt_iter_t::accept(node_t *scope) {
      node_t::accept(scope);
      ACCEPT(decls);
      ACCEPT(cond);
      ACCEPT((node_t *) step);
      ACCEPT(stmt);
    }

    bool stmt_iter_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return stmt_t::write(writer, ast);
    }

    stmt_jump_t::stmt_jump_t(stmt_jump_t::kind_t kind) : kind(kind) {}
    stmt_jump_t::stmt_jump_t(std::string *id) : kind(GOTO), id(id) {}
    stmt_jump_t::stmt_jump_t(expr_t *expr) : kind(RETURN), expr(expr) {}

    void stmt_jump_t::accept(node_t *scope) {
      node_t::accept(scope);
      ACCEPT(expr);
    }

    bool stmt_jump_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return stmt_t::write(writer, ast);
    }

    stmt_decl_t::stmt_decl_t() {}
    stmt_decl_t::stmt_decl_t(decl_t *decls) : decls(decls) {}

    void stmt_decl_t::accept(node_t *scope) {
      node_t::accept(scope);
      ACCEPT(decls);
    }

    bool stmt_decl_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return stmt_t::write(writer, ast);
    }
  }
}

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */