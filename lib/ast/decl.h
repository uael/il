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
#include "id.h"

namespace Jay {
  namespace Ast {
    struct Decl : Node {};

    struct DeclInclude : Decl {
      Id *includes = nullptr;

      DeclInclude(Id *includes);

      void accept(Node *scope) override;

      void generate(Gen::Generator *generator);
    };

    struct DeclUse : Decl {
      Id *uses = nullptr;

      DeclUse(Id *uses);

      void accept(Node *scope) override;
    };

    struct DeclNested : Decl {
      Id *name = nullptr;
      Decl *decls = nullptr;

      DeclNested(Id *name, Decl *decls);

      void accept(Node *scope) override;
    };

    struct DeclMember : Decl {
      Id *ids = nullptr;
      TypeSpecifier *type_specifier = nullptr;
      Closure *closure = nullptr;

      DeclMember(Id *ids, TypeSpecifier *type_specifier, Closure *closure);

      virtual void accept(Node *scope) override;
    };

    struct DeclProperty : DeclMember {
      bool assigned;

      DeclProperty(Id *ids, TypeSpecifier *type_specifier, Closure *closure, bool assigned);

      void generate(Gen::Generator *generator);
    };

    struct DeclFunction : DeclMember {
      Generic *generics = nullptr;
      Decl *args = nullptr;

      DeclFunction(Id *ids, Generic *generics, Decl *args, TypeSpecifier *type_specifier,
                      Closure *closure);

      virtual void accept(Node *scope) override;

      void generate(Gen::Generator *generator);
    };

    struct DeclCtor : DeclFunction {
      Id *props_args = nullptr;
      bool poly = true;
      bool dtor = false;

      DeclCtor(Decl *args, Closure *closure, const bool &poly = true);
      DeclCtor(Id *props_args, Closure *closure, const bool &poly = true);

      void accept(Node *scope) override;
    };

    struct DeclDtor : DeclCtor {
      DeclDtor(Decl *args, Closure *closure, const bool &poly);

      DeclDtor(Id *props_args, Closure *closure, const bool &poly);
    };

    struct DeclFrame : DeclNested {
      Generic *generics = nullptr;
      TypeSpecifier *type = nullptr;

      DeclFrame(Id *name, Generic *generics, TypeSpecifier *type, Decl *decls);

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