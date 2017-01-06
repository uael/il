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

#include "decl.h"
#include "generic.h"
#include "type.h"

namespace Jay {
  namespace Ast {
    DeclInclude::DeclInclude(Identifier *includes) : includes(includes) {}

    void DeclInclude::accept(Node *scope) {
      ACCEPT(includes);
      Node::accept(scope);
    }

    DeclUse::DeclUse(Identifier *uses) : uses(uses) {}

    void DeclUse::accept(Node *scope) {
      ACCEPT(uses);
      Node::accept(scope);
    }

    DeclNested::DeclNested(Identifier *name, Decl *decls) : name(name), decls(decls) {}

    void DeclNested::accept(Node *scope) {
      ACCEPT(name);
      Node::accept(scope);
    }

    DeclMember::DeclMember(Identifier *ids, TypeSpecifier *type_specifier, Closure *closure)
      : ids(ids), type_specifier(type_specifier), closure(closure) {}

    void DeclMember::accept(Node *scope) {
      ACCEPT(ids);
      ACCEPT(type_specifier);
      ACCEPT(closure);
      Node::accept(scope);
    }

    DeclProperty::DeclProperty(
      Identifier *ids, TypeSpecifier *type_specifier, Closure *closure, bool assigned)
      : DeclMember(ids, type_specifier, closure), assigned(assigned) {}

    DeclFunction::DeclFunction(
      Identifier *ids, Generic *generics, Decl *args, TypeSpecifier *type_specifier, Closure *closure)
      : DeclMember(ids, type_specifier, closure), generics(generics), args(args) {}

    void DeclFunction::accept(Node *scope) {
      ACCEPT(generics);
      ACCEPT(args);
      DeclMember::accept(scope);
    }

    DeclCtor::DeclCtor(Decl *args, Closure *closure, const bool &poly)
      : DeclFunction(nullptr, nullptr, args, nullptr, closure), poly(poly) {}
    DeclCtor::DeclCtor(Identifier *props_args, Closure *closure, const bool &poly)
      : DeclFunction(nullptr, nullptr, nullptr, nullptr, closure), props_args(props_args), poly(poly) {}

    void DeclCtor::accept(Node *scope) {
      ACCEPT(props_args);
      DeclFunction::accept(scope);
    }

    DeclDtor::DeclDtor(Decl *args, Closure *closure, const bool &poly) : DeclCtor(args, closure, poly) {}
    DeclDtor::DeclDtor(Identifier *props_args, Closure *closure, const bool &poly)
      : DeclCtor(props_args, closure, poly) {}

    DeclFrame::DeclFrame(Identifier *name, Generic *generics, TypeSpecifier *type, Decl *decls)
      : DeclNested(name, decls), generics(generics), type(type)  {}

    void DeclFrame::accept(Node *scope) {
      ACCEPT(generics);
      ACCEPT(type);
      DeclNested::accept(scope);
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