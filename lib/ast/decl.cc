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
    DeclInclude::DeclInclude(Id *includes) : Decl(includes) {}

    void DeclInclude::accept(Node *scope) {
      ACCEPT(ids);
      Node::accept(scope);
    }

    DeclUse::DeclUse(ConstPath *path) : Decl(path->ids), path(path) {}

    void DeclUse::accept(Node *scope) {
      ACCEPT(path);
      Node::accept(scope);
    }

    DeclNested::DeclNested(Id *name, Decl *decls) : Decl(name), decls(decls) {}

    void DeclNested::accept(Node *scope) {
      ACCEPT(ids);
      ACCEPT(decls);
      Node::accept(scope);
    }

    DeclMember::DeclMember(Id *ids, TypeSpecifier *type_specifier, Closure *closure)
      : Decl(ids), type_specifier(type_specifier), closure(closure) {}

    void DeclMember::accept(Node *scope) {
      ACCEPT(ids);
      ACCEPT(type_specifier);
      ACCEPT(closure);
      Node::accept(scope);
    }

    DeclProperty::DeclProperty(Id *ids, TypeSpecifier *type_specifier, Closure *closure, bool assigned)
      : DeclMember(ids, type_specifier, closure), assigned(assigned) {}

    DeclFunction::DeclFunction(Id *ids, Generic *generics, Decl *args, TypeSpecifier *type_specifier, Closure *closure)
      : DeclMember(ids, type_specifier, closure), generics(generics), args(args) {}

    void DeclFunction::accept(Node *scope) {
      ACCEPT(generics);
      ACCEPT(args);
      DeclMember::accept(scope);
    }

    DeclFunction::~DeclFunction() {
      delete uses;
    }

    bool DeclMember::is_void() {
      TypeInternal *type = as(type_specifier, TypeInternal);
      return type && type->kind == TypeInternal::VOID;
    }

    DeclCtor::DeclCtor(Decl *args, Closure *closure, const bool &poly)
      : DeclFunction(nullptr, nullptr, args, nullptr, closure), poly(poly) {}
    DeclCtor::DeclCtor(Id *props_args, Closure *closure, const bool &poly)
      : DeclFunction(nullptr, nullptr, nullptr, nullptr, closure), props_args(props_args), poly(poly) {}

    void DeclCtor::accept(Node *scope) {
      ACCEPT(props_args);
      DeclFunction::accept(scope);
    }

    DeclDtor::DeclDtor(Decl *args, Closure *closure, const bool &poly) : DeclCtor(args, closure, poly) {}
    DeclDtor::DeclDtor(Id *props_args, Closure *closure, const bool &poly)
      : DeclCtor(props_args, closure, poly) {}

    DeclFrame::DeclFrame(Id *name, Generic *generics, TypeSpecifier *type, Decl *decls)
      : DeclNested(name, decls), generics(generics), type(type)  {}

    void DeclFrame::accept(Node *scope) {
      ACCEPT(generics);
      ACCEPT(type);
      DeclNested::accept(scope);
    }

    Decl::Decl(Id *ids) : Named(ids) {}

    Use::Use(Decl *node) : node(node) {}

    Use::~Use() {
      if (next) {
        delete next;
      }
    }

    DeclNamespace::DeclNamespace(ConstPath *path, Decl *decls) : DeclNested(path->ids, decls), path(path) {}

    void DeclNamespace::accept(Node *scope) {
      ACCEPT(path);
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