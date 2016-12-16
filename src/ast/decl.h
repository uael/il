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

#ifndef _AST_DECL_H
#define _AST_DECL_H

#include "node.h"
#include "ident.h"

namespace dyc {
  namespace ast {
    struct decl_t : node_t {};

    struct decl_namespace_t : decl_t {
      identifier_t *name = nullptr;
      decl_t *decls = nullptr;

      decl_namespace_t(identifier_t *name, decl_t *decls);

      void accept(node_t *scope) override;
    };

    struct decl_member_t : decl_t {
      identifier_t *ids = nullptr;
      type_specifier_t *type_specifier = nullptr;
      closure_t *closure = nullptr;

      decl_member_t(identifier_t *ids, type_specifier_t *type_specifier, closure_t *closure);

      virtual void accept(node_t *scope) override;
    };

    struct decl_property_t : decl_member_t {
      bool assigned;

      decl_property_t(identifier_t *ids, type_specifier_t *type_specifier, closure_t *closure, bool assigned);

      void write(writer_t *writer) override;
    };

    struct decl_function_t : decl_member_t {
      generic_t *generics = nullptr;
      decl_t *args = nullptr;

      decl_function_t(identifier_t *ids, generic_t *generics, decl_t *args, type_specifier_t *type_specifier,
                      closure_t *closure);

      virtual void accept(node_t *scope) override;
      void write(writer_t *writer) override;
    };
  }
}

#endif /* _AST_DECL_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */