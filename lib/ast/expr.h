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

#ifndef _AST_EXPR_H
#define _AST_EXPR_H

#include <tuple>
#include <vector>
#include "node.h"
#include "closure.h"

namespace Jay {
  namespace Ast {
    struct Expr : Node, Closure {
      enum Kind {
        INC_PRE, DEC_PRE, AND_PRE, ADD_PRE, SUB_PRE, MUL_PRE, NOT_PRE, TID_PRE, POS, CALL, INC_POST, DEC_POST, CONST,
        ENCLOSE, ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, ADD_ASSIGN, SUB_ASSIGN, LEFT_ASSIGN, RIGHT_ASSIGN,
        AND_ASSIGN, XOR_ASSIGN, OR_ASSIGN, MUL, DIV, MOD, ADD, SUB, LEFT, RIGHT, AND, XOR, OR, LAND, LOR, EQ, NEQ, LT,
        GT, LTE, GTE, TERNARY, CAST, KVP, SIZEOF, NESTED
      };
    };

    struct ExprOp : Expr {
      Kind kind;
      Expr *op1 = nullptr;

      ExprOp(Kind kind, Expr *op1);

      std::string op();
      void accept(Node *scope) override;
    };

    struct ExprDop : ExprOp {
      Expr *op2 = nullptr;

      ExprDop(Kind kind, Expr *op1, Expr *op2);

      void accept(Node *scope) override;
    };

    struct ExprTernary : ExprDop {
      Expr *cond = nullptr;

      ExprTernary(Expr *cond, Expr *op1, Expr *op2);

      void accept(Node *scope) override;
    };

    struct ExprCast : ExprOp {
      TypeSpecifier *type = nullptr;

      ExprCast(Expr *op1, TypeSpecifier *type);

      void accept(Node *scope) override;
    };

    struct ExprCall : ExprDop {
      ExprCall(Expr *op1, Expr *op2);
    };

    struct ExprPos : ExprDop {
      ExprPos(Expr *op1, Expr *op2);
    };

    struct ExprUnary : ExprOp {
      ExprUnary(Kind kind, Expr *op1);
    };

    struct ExprPostfix : ExprOp {
      ExprPostfix(Kind kind, Expr *op1);
    };

    struct ExprNested : ExprOp {
      Identifier *id = nullptr;

      ExprNested(Expr *op1, Identifier *id);

      void accept(Node *scope) override;
    };

    struct ExprSizeof : ExprOp {
      TypeSpecifier *type = nullptr;

      ExprSizeof(TypeSpecifier *type);
      ExprSizeof(Expr *op1);

      void accept(Node *scope) override;
    };

    struct ExprPrimary : ExprOp {
      ExprPrimary(Kind kind, Expr *op1);
    };

    struct ExprKvp : ExprDop {
      ExprKvp(Expr *op1, Expr *op2);
    };

    struct ExprConst : Expr {};
  }
}

#endif /* _AST_EXPR_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */