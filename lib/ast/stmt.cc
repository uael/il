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

namespace Jay {
  namespace Ast {
    StmtExpr::StmtExpr() {}
    StmtExpr::StmtExpr(Expr *expr) : expr(expr) {}

    void StmtExpr::accept(Node *scope) {
      Node::accept(scope);
      ACCEPT(expr);
    }

    StmtLabel::StmtLabel(Stmt *stmt) : kind(DEFAULT), stmt(stmt) {}
    StmtLabel::StmtLabel(std::string *id, Stmt *stmt) : kind(LABEL), id(id), stmt(stmt) {}
    StmtLabel::StmtLabel(Expr *cond, Stmt *stmt) : kind(CASE), stmt(stmt), cond(cond) {}

    void StmtLabel::accept(Node *scope) {
      ACCEPT(stmt);
      ACCEPT(cond);
      Node::accept(scope);
    }

    StmtCompound::StmtCompound(Stmt *stmts) : stmts(stmts) {}
    StmtCompound::StmtCompound() {}

    void StmtCompound::accept(Node *scope) {
      ACCEPT(stmts);
      Node::accept(scope);
    }

    StmtSelect::StmtSelect() {}
    StmtSelect::StmtSelect(StmtSelect::Kind kind, Expr *cond, Stmt *stmt)
      : kind(kind), cond(cond), stmt(stmt) {}
    StmtSelect::StmtSelect(Expr *cond, Stmt *stmt, Stmt *else_stmt)
      : kind(IF), cond(cond), stmt(stmt), else_stmt(else_stmt) {}

    void StmtSelect::accept(Node *scope) {
      ACCEPT(cond);
      ACCEPT(stmt);
      ACCEPT(else_stmt);
      Node::accept(scope);
    }

    StmtIter::StmtIter(Expr *cond, Stmt *stmt) : kind(WHILE), cond(cond), stmt(stmt) {}
    StmtIter::StmtIter(Stmt *stmt, Expr *cond) : kind(DO_WHILE), cond(cond), stmt(stmt) {}
    StmtIter::StmtIter(Expr *cond, Closure *step, Stmt *stmt)
      : kind(FOR), cond(cond), step(step), stmt(stmt) {}
    StmtIter::StmtIter(Stmt *decls, Expr *cond, Closure *step, Stmt *stmt)
      : kind(FOR), decls(decls), cond(cond), step(step), stmt(stmt) {}

    void StmtIter::accept(Node *scope) {
      ACCEPT(decls);
      ACCEPT(cond);
      ACCEPT(step);
      ACCEPT(stmt);
      Node::accept(scope);
    }

    StmtJump::StmtJump(StmtJump::Kind kind) : kind(kind) {}
    StmtJump::StmtJump(std::string *id) : kind(GOTO), id(id) {}
    StmtJump::StmtJump(Expr *expr) : kind(RETURN), expr(expr) {}

    void StmtJump::accept(Node *scope) {
      ACCEPT(expr);
      Node::accept(scope);
    }

    StmtDecl::StmtDecl() {}
    StmtDecl::StmtDecl(Decl *decls) : decls(decls) {}
    StmtDecl::StmtDecl(Decl *decls, bool constant) : decls(decls), constant(constant) {}

    void StmtDecl::accept(Node *scope) {
      ACCEPT(decls);
      Node::accept(scope);
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