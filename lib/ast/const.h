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

#ifndef _AST_CONST_H
#define _AST_CONST_H

#include "expr.h"

namespace Jay {
  namespace Ast {
    struct ConstThis : ExprConst {
    };

    struct ConstValue : ExprConst {
      enum Kind {
        INT, FLOAT, STRING
      } kind;
      std::string *value;

      ConstValue(Kind kind, std::string *value);
    };

    struct ConstLambda : ExprConst {
      Identifier *args = nullptr;
      Closure *closure = nullptr;

      ConstLambda(Identifier *args, Closure *closure);

      void accept(Node *scope) override;
    };

    struct ConstInitializer : ExprConst {
      Expr *exprs = nullptr;

      ConstInitializer(Expr *exprs);

      void accept(Node *scope) override;
    };

    struct ConstNew : ConstInitializer {
      TypeUserdef *type;

      ConstNew(TypeUserdef *type, Expr *exprs = nullptr);

      void accept(Node *scope) override;
    };
  }
}

#endif /* _AST_CONST_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */