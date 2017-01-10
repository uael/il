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
#include "type.h"
#include "const.h"
#include "id.h"

namespace Jay {
  namespace Gen {
    Expr::Expr(Ast::Program *program, Ast::Expr *node) : CGen(program, node) {}

    void Expr::generate(File *file) {
      TRY_CGEN(Const);
      TRY_CGEN(ExprDop);
      TRY_CGEN(ExprOp);
    }

    ExprCast::ExprCast(Ast::Program *program, Ast::ExprCast *expr) : CGen(program, expr) {}

    void ExprCast::generate(File *file) {
      cursor = "(" + CGEN(TypeSpecifier, node->type) + ")" + CGEN(Expr, node->op1);
    }

    ExprPos::ExprPos(Ast::Program *program, Ast::ExprPos *expr) : CGen(program, expr) {}

    void ExprPos::generate(File *file) {
      cursor = CGEN(Expr, node->op1) + "[" + CGEN(Expr, node->op2) +  "]";
    }

    ExprKvp::ExprKvp(Ast::Program *program, Ast::ExprKvp *expr) : CGen(program, expr) {}

    void ExprKvp::generate(File *file) {
      CGen::generate(file);
    }

    ExprCall::ExprCall(Ast::Program *program, Ast::ExprCall *expr) : CGen(program, expr) {}

    void ExprCall::generate(File *file) {
      cursor = CGEN(Expr, node->op1) + "(" + CGEN_ALL(Expr, node->op2, ", ") + ")";
    }

    ExprNested::ExprNested(Ast::Program *program, Ast::ExprNested *expr) : CGen(program, expr) {}

    void ExprNested::generate(File *file) {
      cursor = CGEN(Expr, node->op1) + "->" + CGEN(Id, node->id);
    }

    ExprOp::ExprOp(Ast::Program *program, Ast::ExprOp *expr) : CGen(program, expr) {}

    void ExprOp::generate(File *file) {
      TRY_CGEN(ExprCast);
      TRY_CGEN(ExprUnary);
      TRY_CGEN(ExprPostfix);
      TRY_CGEN(ExprNested);
      TRY_CGEN(ExprSizeof);
      TRY_CGEN(ExprPrimary);
    }

    ExprPostfix::ExprPostfix(Ast::Program *program, Ast::ExprPostfix *expr) : CGen(program, expr) {}

    void ExprPostfix::generate(File *file) {
      CGen::generate(file);
    }

    ExprSizeof::ExprSizeof(Ast::Program *program, Ast::ExprSizeof *expr) : CGen(program, expr) {}

    void ExprSizeof::generate(File *file) {
      cursor = CGEN(Expr, node->op1) + node->op();
    }

    ExprDop::ExprDop(Ast::Program *program, Ast::ExprDop *expr) : CGen(program, expr) {}

    void ExprDop::generate(File *file) {
      TRY_CGEN(ExprTernary);
      TRY_CGEN(ExprCall);
      TRY_CGEN(ExprPos);
      TRY_CGEN(ExprKvp);
      cursor = CGEN(Expr, node->op1) + node->op() + CGEN(Expr, node->op2);
    }

    ExprPrimary::ExprPrimary(Ast::Program *program, Ast::ExprPrimary *expr) : CGen(program, expr) {}

    void ExprPrimary::generate(File *file) {
      if (node->kind == Ast::Expr::Kind::ENCLOSE) {
        cursor = "(" + CGEN(Expr, node->op1) + ")";
      } else {
        cursor = CGEN(Expr, node->op1);
      }
    }

    ExprUnary::ExprUnary(Ast::Program *program, Ast::ExprUnary *expr) : CGen(program, expr) {}

    void ExprUnary::generate(File *file) {
      cursor = node->op() + CGEN(Expr, node->op1);
    }

    ExprTernary::ExprTernary(Ast::Program *program, Ast::ExprTernary *expr) : CGen(program, expr) {}

    void ExprTernary::generate(File *file) {
      cursor = CGEN(Expr, node->cond) + "?";
      cursor += CGEN(Expr, node->op1) + ":" + CGEN(Expr, node->op2);
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