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

namespace dyc {
  namespace ast {
    struct const_this_t : expr_const_t {
      void write(writer_t *writer) override;
    };

    struct const_value_t : expr_const_t {
      enum kind_t {
        INT, FLOAT, STRING
      } kind;
      std::string *value;

      const_value_t(kind_t kind, std::string *value);

      void write(writer_t *writer) override;
    };

    struct const_lambda_t : expr_const_t {
      identifier_t *args = nullptr;
      closure_t *closure = nullptr;

      const_lambda_t(identifier_t *args, closure_t *closure);

      void write(writer_t *writer) override;
      void accept(node_t *scope) override;
    };

    struct const_initializer_t : expr_const_t {
      expr_t *exprs = nullptr;

      const_initializer_t(expr_t *exprs);

      void write(writer_t *writer) override;
      void accept(node_t *scope) override;
    };

    struct const_new_t : const_initializer_t {
      identifier_t *id;

      const_new_t(identifier_t *id, expr_t *exprs);

      void accept(node_t *scope) override;
      void write(writer_t *writer) override;
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