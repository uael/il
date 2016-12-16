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

#ifndef _AST_TYPE_H
#define _AST_TYPE_H

#include <vector>
#include "node.h"

namespace dyc {
  namespace ast {
    struct type_specifier_t : node_t {};

    struct type_callable_t : type_specifier_t {
      type_specifier_t *type = nullptr;
      type_specifier_t *args_types = nullptr;

      type_callable_t(type_specifier_t *type, type_specifier_t *args_types = nullptr);

      void accept(node_t *scope) override;
      void write(writer_t *writer) override;
    };

    struct type_array_t : type_specifier_t {
      type_specifier_t *type = nullptr;
      expr_t *fixed_size = nullptr;

      type_array_t(type_specifier_t *type, expr_t *fixed_size = nullptr);

      void accept(node_t *scope) override;
      void write(writer_t *writer) override;
    };

    struct type_t : type_specifier_t {};

    struct type_scalar_t : type_t {
      enum kind_t {
        VOID, BOOL, CHAR, INT, UINT, SINT, SHORT, USHORT,
        SSHORT, FLOAT, UFLOAT, SFLOAT, DOUBLE, UDOUBLE, SDOUBLE
      } kind;

      type_scalar_t(kind_t kind);

      void write(writer_t *writer) override;
    };

    struct type_userdef_t : type_t {
      std::string *id;

      type_userdef_t(std::string *id);

      void write(writer_t *writer) override;
    };
  }
}

#endif /* _AST_TYPE_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */