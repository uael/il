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

namespace Jay {
  namespace Gen {
    Decl::Decl(Ast::Program *program, Ast::Decl *node) : CGen(program, node) {}

    void Decl::generate(File *file) {
      TRY_CGEN(DeclCtor);
      TRY_CGEN(DeclDtor);
      TRY_CGEN(DeclFrame);
      TRY_CGEN(DeclFunction);
      TRY_CGEN(DeclInclude);
      TRY_CGEN(DeclProperty);
    }

    DeclUse::DeclUse(Ast::Program *program, Ast::DeclUse *node) : CGen(program, node) {}

    void DeclUse::generate(File *file) {
      CGen::generate(file);
    }

    DeclInclude::DeclInclude(Ast::Program *program, Ast::DeclInclude *node) : CGen(program, node) {}

    void DeclInclude::generate(File *file) {
      foreach(include, node->includes) {
        file->includes += "#include \"" + *include->value + ".h\"\n";
      }
    }

    DeclCtor::DeclCtor(Ast::Program *program, Ast::DeclCtor *node) : CGen(program, node) {}

    void DeclCtor::generate(File *file) {
      CGen::generate(file);
    }

    DeclFrame::DeclFrame(Ast::Program *program, Ast::DeclFrame *node) : CGen(program, node) {}

    void DeclFrame::generate(File *file) {
      CGen::generate(file);
    }

    DeclFunction::DeclFunction(Ast::Program *program, Ast::DeclFunction *node) : CGen(program, node) {}

    void DeclFunction::generate(File *file) {
      if (node->ids->find("main") && as(node->scope, Ast::File)) {
        this->cursor += "int main(";
        if (node->args) {
          foreach(arg, node->args) {
            this->cursor += CGEN(Decl, arg)->cursor;
          }
        }
        this->cursor += ") {\n";
        file->declarations += "\n" + this->cursor + "\n";

      } else if (as(node->scope, Ast::DeclFunction)) {

      } else {

      }
      this->cursor = "";
    }

    DeclDtor::DeclDtor(Ast::Program *program, Ast::DeclDtor *node) : CGen(program, node) {}

    void DeclDtor::generate(File *file) {
      CGen::generate(file);
    }

    DeclNested::DeclNested(Ast::Program *program, Ast::DeclNested *node) : CGen(program, node) {}

    void DeclNested::generate(File *file) {
      program->ns = node;
    }

    DeclProperty::DeclProperty(Ast::Program *program, Ast::DeclProperty *node) : CGen(program, node) {}

    void DeclProperty::generate(File *file) {
      if (node->type_specifier) {

      }
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