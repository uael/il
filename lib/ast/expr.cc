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

#include "expr.h"
#include "id.h"
#include "type.h"

namespace Jay {
  namespace Ast {
    ExprOp::ExprOp(ExprOp::Kind kind, Expr *op1) : kind(kind), op1(op1) {}

    std::string ExprOp::op_str() {
      switch (kind) {
        case INC_PRE:
        case INC_POST:
          return "++";
        case DEC_PRE:
        case DEC_POST:
          return "--";
        case AND_PRE:
        case AND:
          return "&";
        case ADD_PRE:
        case ADD:
          return "+";
        case SUB_PRE:
        case SUB:
          return "-";
        case MUL_PRE:
        case MUL:
          return "*";
        case NOT_PRE:
          return "!";
        case TID_PRE:
          return "~";
        case ASSIGN:
          return "=";
        case MUL_ASSIGN:
          return "*=";
        case DIV_ASSIGN:
          return "/=";
        case MOD_ASSIGN:
          return "%=";
        case ADD_ASSIGN:
          return "+=";
        case SUB_ASSIGN:
          return "-=";
        case LEFT_ASSIGN:
          return "<<=";
        case RIGHT_ASSIGN:
          return ">>=";
        case AND_ASSIGN:
          return "&=";
        case XOR_ASSIGN:
          return "^=";
        case OR_ASSIGN:
          return "|=";
        case DIV:
          return "/";
        case MOD:
          return "%";
        case LEFT:
          return "<<";
        case RIGHT:
          return ">>";
        case XOR:
          return "^";
        case OR:
          return "|";
        case LAND:
          return "&&";
        case LOR:
          return "||";
        case EQ:
          return "==";
        case NEQ:
          return "!=";
        case LT:
          return "<";
        case GT:
          return ">";
        case LTE:
          return "<=";
        case GTE:
          return ">=";
        default:
          return "";
      }
    }

    void ExprOp::accept(Node *scope) {
      ACCEPT(op1);
      Node::accept(scope);
    }

    ExprDop::ExprDop(ExprOp::Kind kind, Expr *op1, Expr *op2) : ExprOp(kind, op1), op2(op2) {}

    void ExprDop::accept(Node *scope) {
      ACCEPT(op2);
      ExprOp::accept(scope);
    }

    ExprTernary::ExprTernary(Expr *cond, Expr *op1, Expr *op2) : ExprDop(TERNARY, op1, op2),cond(cond) {}

    void ExprTernary::accept(Node *scope) {
      ACCEPT(cond);
      ExprDop::accept(scope);
    }

    ExprCast::ExprCast(Expr *op1, TypeSpecifier *type) : ExprOp(CAST, op1), type(type) {}

    void ExprCast::accept(Node *scope) {
      ACCEPT(type);
      ExprOp::accept(scope);
    }

    ExprCall::ExprCall(Expr *op1, Expr *op2) : ExprDop(CALL, op1, op2) {}

    ExprPos::ExprPos(Expr *op1, Expr *op2) : ExprDop(POS, op1, op2) {}

    ExprUnary::ExprUnary(ExprOp::Kind kind, Expr *op1) : ExprOp(kind, op1) {}

    ExprPostfix::ExprPostfix(ExprOp::Kind kind, Expr *op1) : ExprOp(kind, op1) {}

    ExprNested::ExprNested(Expr *op1, Id *id) : ExprOp(NESTED, op1), id(id) {}

    void ExprNested::accept(Node *scope) {
      ACCEPT(id);
      ExprOp::accept(scope);
    }

    ExprSizeof::ExprSizeof(TypeSpecifier *type) : ExprOp(SIZEOF, nullptr), type(type) {}
    ExprSizeof::ExprSizeof(Expr *op1) : ExprOp(SIZEOF, op1) {}

    void ExprSizeof::accept(Node *scope) {
      ACCEPT(type);
      Node::accept(scope);
    }

    ExprPrimary::ExprPrimary(ExprOp::Kind kind, Expr *op1) : ExprOp(kind, op1) {}

    ExprKvp::ExprKvp(Expr *op1, Expr *op2) : ExprDop(KVP, op1, op2) {}

    ExprAccess::ExprAccess(Expr *op1, Id *id) : ExprNested(op1, id) {}

    void ExprAccess::accept(Node *scope) {
      ExprNested::accept(scope);
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