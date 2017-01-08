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

#include "type.h"
#include "decl.h"

namespace Jay {
  namespace Ast {
    TypeCallable::TypeCallable(TypeSpecifier *type, TypeSpecifier *args_types)
      : type(type), args_types(args_types) {}

    void TypeCallable::accept(Node *scope) {
      ACCEPT(type);
      ACCEPT(args_types);
      Node::accept(scope);
    }

    TypePtr::TypePtr(TypeSpecifier *type) : type(type) {}

    void TypePtr::accept(Node *scope) {
      ACCEPT(type);
      Node::accept(scope);
    }

    TypeArray::TypeArray(TypeSpecifier *type, Expr *fixed_size) : type(type), fixed_size(fixed_size) {}

    void TypeArray::accept(Node *scope) {
      ACCEPT(type);
      ACCEPT(fixed_size);
      Node::accept(scope);
    }

    TypeInternal::TypeInternal(TypeInternal::Kind kind) : kind(kind) {}

    TypeUserdef::TypeUserdef(Id *id) : id(id) {}

    void TypeUserdef::accept(Node *scope) {
      ACCEPT(id);
      Node::accept(scope);
    }

    TypeGeneric::TypeGeneric(Id *id, TypeSpecifier *types) : TypeUserdef(id), types(types) {}

    void TypeGeneric::accept(Node *scope) {
      ACCEPT(types);
      TypeUserdef::accept(scope);
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