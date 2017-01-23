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

namespace Jay {
  namespace Ast {
    struct Stmt : Node {};

    struct StmtExpr : Stmt {
      Expr *expr = nullptr;

      StmtExpr();
      StmtExpr(Expr *expr);

      virtual void accept(Node *scope) override;
      void generate(Gen::Generator *generator);
    };

    struct StmtLabel : Stmt {
      enum Kind {
        LABEL, CASE, DEFAULT
      } kind;
      std::string *id;
      Stmt *stmt = nullptr;
      Expr *cond = nullptr;

      StmtLabel(Stmt *stmt);
      StmtLabel(std::string *id, Stmt *stmt);
      StmtLabel(Expr *cond, Stmt *stmt);

      virtual void accept(Node *scope) override;
      void generate(Gen::Generator *generator);
    };

    struct StmtCompound : Stmt, Closure {
      Stmt *stmts = nullptr;

      StmtCompound();
      StmtCompound(Stmt *stmts);

      virtual void accept(Node *scope) override;
      void generate(Gen::Generator *generator);
    };

    struct StmtSelect : Stmt {
      enum Kind {
        IF, SWITCH
      } kind;
      Expr *cond = nullptr;
      Stmt *stmt = nullptr;
      Stmt *else_stmt = nullptr;

      StmtSelect();
      StmtSelect(Kind kind, Expr *cond, Stmt *stmt);
      StmtSelect(Expr *cond, Stmt *stmt, Stmt *else_stmt);

      virtual void accept(Node *scope) override;
      void generate(Gen::Generator *generator);
    };

    struct StmtIter : Stmt {
      enum Kind {
        WHILE, DO_WHILE, FOR
      } kind;
      Stmt *decls = nullptr;
      Expr *cond = nullptr;
      Closure *step = nullptr;
      Stmt *stmt = nullptr;

      StmtIter(Expr *cond, Stmt *stmt);
      StmtIter(Stmt *stmt, Expr *cond);
      StmtIter(Expr *cond, Closure *step, Stmt *stmt);
      StmtIter(Stmt *decls, Expr *cond, Closure *step, Stmt *stmt);

      virtual void accept(Node *scope) override;
      void generate(Gen::Generator *generator);
    };

    struct StmtJump : Stmt {
      enum Kind {
        GOTO, CONTINUE, BREAK, RETURN
      } kind;
      std::string *id;
      Expr *expr = nullptr;

      StmtJump(Kind kind);
      StmtJump(std::string *id);
      StmtJump(Expr *expr);

      virtual void accept(Node *scope) override;
      void generate(Gen::Generator *generator);
    };

    struct StmtDecl : Stmt {
      Decl *decls = nullptr;
      bool constant = false;

      StmtDecl();
      StmtDecl(Decl *decls);
      StmtDecl(Decl *decls, bool constant);

      virtual void accept(Node *scope) override;
      void generate(Gen::Generator *generator);
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