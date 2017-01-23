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

#include <iostream>
#include "generator.h"

namespace Jay {
  namespace Ast {
    void DeclInclude::generate(Gen::Generator *generator) {
      foreach(include, ids) {
        *generator << "#include \"" << include->value << ".h\"\n";
      }
      generator->source->header += generator->cursor;
    }

    void DeclUse::generate(Gen::Generator *generator) {

    }

    void DeclNested::generate(Gen::Generator *generator) {
      *generator << op(AllIndent, decls, "\n");
    }

    void DeclNamespace::generate(Gen::Generator *generator) {
      *generator << op(AllIndent, decls, "\n");
    }

    void DeclFunction::generate(Gen::Generator *generator) {
      generator->reset();
      if (as(scope, File) && find_id("main")) {
        *generator << op(IndentCursor) << type_specifier << " " << ids->value << "(" << op(All, args, ", ") << ")";
        generator->header->body += "\n" + generator->cursor + ";";
        if (closure) {
          closure->macro = false;
          *generator << " " << op(Closure, closure);
        }
      } else {
        foreach(id, ids) {
          *generator << "#define " << id << "(" << op(AllName, args, ", ") << ")";
          if (closure) {
            *generator << " " << op(Closure, closure);
          }
        }
      }
      generator->source->body += "\n" + generator->cursor;
    }

    void Use::generate(Gen::Generator *generator) {
      Node *s = node->scope;
      node->scope = scope;
      *generator << node;
      node->scope = s;
    }

    void DeclProperty::generate(Gen::Generator *generator) {
      if (as(scope, DeclFunction) || as(scope, StmtDecl) || as(scope, StmtCompound)) {
        *generator << type_specifier << " " << ids;
        TypeArray *arr;
        if ((arr = as(type_specifier, TypeArray))) {
          *generator << "[" << arr->fixed_size << "]";
        }
        if (closure && !as(scope, DeclFunction)) {
          *generator << " = " << closure->as_node();
        }
      }
    }

    void DeclCtor::generate(Gen::Generator *generator) {

    }

    void DeclDtor::generate(Gen::Generator *generator) {

    }

    void DeclFrame::generate(Gen::Generator *generator) {

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