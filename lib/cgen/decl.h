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

#ifndef _CGEN_DECL_H
#define _CGEN_DECL_H

#include "cgen.h"

namespace Jay {
  namespace Gen {
    struct Decl : CGen<Ast::Decl> {
      Decl(Ast::Program *program, Ast::Decl *node);

      void generate(File *file) override;
    };

    struct DeclInclude : CGen<Ast::DeclInclude> {
      DeclInclude(Ast::Program *program, Ast::DeclInclude *node);

      void generate(File *file) override;
    };

    struct DeclUse : CGen<Ast::DeclUse> {
      DeclUse(Ast::Program *program, Ast::DeclUse *node);

      void generate(File *file) override;
    };

    struct DeclNested : CGen<Ast::DeclNested> {
      DeclNested(Ast::Program *program, Ast::DeclNested *node);

      void generate(File *file) override;
    };

    struct DeclProperty : CGen<Ast::DeclProperty> {
      DeclProperty(Ast::Program *program, Ast::DeclProperty *node);

      void generate(File *file) override;
    };

    struct DeclFunction : CGen<Ast::DeclFunction> {
      DeclFunction(Ast::Program *program, Ast::DeclFunction *node);

      void generate(File *file) override;
    };

    struct DeclCtor : CGen<Ast::DeclCtor> {
      DeclCtor(Ast::Program *program, Ast::DeclCtor *node);

      void generate(File *file) override;
    };

    struct DeclDtor : CGen<Ast::DeclDtor> {
      DeclDtor(Ast::Program *program, Ast::DeclDtor *node);

      void generate(File *file) override;
    };

    struct DeclFrame : CGen<Ast::DeclFrame> {
      DeclFrame(Ast::Program *program, Ast::DeclFrame *node);

      void generate(File *file) override;
    };
  }
}

#endif /* _CGEN_DECL_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */
