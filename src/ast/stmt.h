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

#ifndef _AST_STMT_H
#define _AST_STMT_H

#include "node.h"
#include "closure.h"

namespace dyc {
  namespace ast {
    struct stmt_t : node_t {};

    struct stmt_expr_t : stmt_t {
      expr_t *expr = nullptr;

      stmt_expr_t();
      stmt_expr_t(expr_t *expr);

      virtual void accept(node_t *scope) override;
      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct stmt_label_t : stmt_t {
      enum kind_t {
        LABEL, CASE, DEFAULT
      } kind;
      std::string *id;
      stmt_t *stmt = nullptr;
      expr_cond_t *cond = nullptr;

      stmt_label_t(stmt_t *stmt);
      stmt_label_t(std::string *id, stmt_t *stmt);
      stmt_label_t(expr_cond_t *cond, stmt_t *stmt);

      virtual void accept(node_t *scope) override;
      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct stmt_compound_t : stmt_t, closure_t {
      stmt_t *stmts = nullptr;

      stmt_compound_t();
      stmt_compound_t(stmt_t *stmts);

      virtual void accept(node_t *scope) override;
      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct stmt_select_t : stmt_t {
      enum kind_t {
        IF, SWITCH
      } kind;
      expr_t *cond = nullptr;
      stmt_t *stmt = nullptr;
      stmt_t *else_stmt = nullptr;

      stmt_select_t();
      stmt_select_t(kind_t kind, expr_t *cond, stmt_t *stmt);
      stmt_select_t(expr_t *cond, stmt_t *stmt, stmt_t *else_stmt);

      virtual void accept(node_t *scope) override;
      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct stmt_iter_t : stmt_t {
      enum kind_t {
        WHILE, DO_WHILE, FOR
      } kind;
      stmt_t *decls = nullptr;
      expr_t *cond = nullptr;
      closure_t *step = nullptr;
      stmt_t *stmt = nullptr;

      stmt_iter_t(expr_t *cond, stmt_t *stmt);
      stmt_iter_t(stmt_t *stmt, expr_t *cond);
      stmt_iter_t(expr_t *cond, closure_t *step, stmt_t *stmt);
      stmt_iter_t(stmt_t *decls, expr_t *cond, closure_t *step, stmt_t *stmt);

      virtual void accept(node_t *scope) override;
      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct stmt_jump_t : stmt_t {
      enum kind_t {
        GOTO, CONTINUE, BREAK, RETURN
      } kind;
      std::string *id;
      expr_t *expr = nullptr;

      stmt_jump_t(kind_t kind);
      stmt_jump_t(std::string *id);
      stmt_jump_t(expr_t *expr);

      virtual void accept(node_t *scope) override;
      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct stmt_decl_t : stmt_t {
      decl_t *decls = nullptr;

      stmt_decl_t();
      stmt_decl_t(decl_t *decls);

      virtual void accept(node_t *scope) override;
      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };
  }
}

#endif /* _AST_STMT_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */