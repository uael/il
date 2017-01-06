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

namespace Jay {
  namespace Ast {
    struct Decl : Node {};

    struct DeclInclude : Decl {
      Identifier *includes = nullptr;

      DeclInclude(Identifier *includes);

      void accept(Node *scope) override;
    };

    struct DeclUse : Decl {
      Identifier *uses = nullptr;

      DeclUse(Identifier *uses);

      void accept(Node *scope) override;
    };

    struct DeclNested : Decl {
      Identifier *name = nullptr;
      Decl *decls = nullptr;

      DeclNested(Identifier *name, Decl *decls);

      void accept(Node *scope) override;
    };

    struct DeclMember : Decl {
      Identifier *ids = nullptr;
      TypeSpecifier *type_specifier = nullptr;
      Closure *closure = nullptr;

      DeclMember(Identifier *ids, TypeSpecifier *type_specifier, Closure *closure);

      virtual void accept(Node *scope) override;
    };

    struct DeclProperty : DeclMember {
      bool assigned;

      DeclProperty(Identifier *ids, TypeSpecifier *type_specifier, Closure *closure, bool assigned);
    };

    struct DeclFunction : DeclMember {
      Generic *generics = nullptr;
      Decl *args = nullptr;

      DeclFunction(Identifier *ids, Generic *generics, Decl *args, TypeSpecifier *type_specifier,
                      Closure *closure);

      virtual void accept(Node *scope) override;
    };

    struct DeclCtor : DeclFunction {
      Identifier *props_args = nullptr;
      bool poly = true;
      bool dtor = false;

      DeclCtor(Decl *args, Closure *closure, const bool &poly = true);
      DeclCtor(Identifier *props_args, Closure *closure, const bool &poly = true);

      void accept(Node *scope) override;
    };

    struct DeclDtor : DeclCtor {
      DeclDtor(Decl *args, Closure *closure, const bool &poly);

      DeclDtor(Identifier *props_args, Closure *closure, const bool &poly);
    };

    struct DeclFrame : DeclNested {
      Generic *generics = nullptr;
      TypeSpecifier *type = nullptr;

      DeclFrame(Identifier *name, Generic *generics, TypeSpecifier *type, Decl *decls);

      void accept(Node *scope) override;
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