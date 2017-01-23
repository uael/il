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

#include "generator.h"

namespace Jay {
  namespace Ast {
    void ExprCast::generate(Gen::Generator *generator) {
      *generator << "(" << type << ")" << op1;
    }

    void ExprPos::generate(Gen::Generator *generator) {
      *generator << op1 << "[" << op2 << "]";
    }

    void ExprKvp::generate(Gen::Generator *generator) {

    }

    void ExprCall::generate(Gen::Generator *generator) {
      *generator << op1 << "(" << op(All, op2, ", ") << ")";
    }

    void ExprNested::generate(Gen::Generator *generator) {
      *generator << op1 << "_" << id;
    }

    void ExprAccess::generate(Gen::Generator *generator) {
      *generator << op1 << "->" << id;
    }

    void ExprPostfix::generate(Gen::Generator *generator) {
      *generator << op1 << op_str();
    }

    void ExprSizeof::generate(Gen::Generator *generator) {
      *generator << op1 << op_str();
    }

    void ExprDop::generate(Gen::Generator *generator) {
      *generator << op1 << op_str() << op2;
    }

    void ExprPrimary::generate(Gen::Generator *generator) {
      if (kind == Ast::Expr::Kind::ENCLOSE) {
        *generator << "(" << op1 << ")";
      } else {
        *generator << op1;
      }
    }

    void ExprUnary::generate(Gen::Generator *generator) {
      *generator << op_str() << op1;
    }

    void ExprTernary::generate(Gen::Generator *generator) {
      *generator << cond << "?" << op1 << ":" << op2;
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