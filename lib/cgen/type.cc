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

namespace Jay {
  namespace Gen {
    Type::Type(Ast::Program *program, Ast::Type *node) : CGen(program, node) {}

    void Type::generate(File *file) {
      CGen::generate(file);
    }

    TypeCallable::TypeCallable(Ast::Program *program, Ast::TypeCallable *node) : CGen(program, node) {}

    void TypeCallable::generate(File *file) {
      CGen::generate(file);
    }

    TypeGeneric::TypeGeneric(Ast::Program *program, Ast::TypeGeneric *node) : CGen(program, node) {}

    void TypeGeneric::generate(File *file) {
      CGen::generate(file);
    }

    TypePtr::TypePtr(Ast::Program *program, Ast::TypePtr *node) : CGen(program, node) {}

    void TypePtr::generate(File *file) {
      CGen::generate(file);
    }

    TypeUserdef::TypeUserdef(Ast::Program *program, Ast::TypeUserdef *node) : CGen(program, node) {}

    void TypeUserdef::generate(File *file) {
      CGen::generate(file);
    }

    TypeArray::TypeArray(Ast::Program *program, Ast::TypeArray *node) : CGen(program, node) {}

    void TypeArray::generate(File *file) {
      CGen::generate(file);
    }

    TypeInternal::TypeInternal(Ast::Program *program, Ast::TypeInternal *node) : CGen(program, node) {}

    void TypeInternal::generate(File *file) {
      CGen::generate(file);
    }

    TypeSpecifier::TypeSpecifier(Ast::Program *program, Ast::TypeSpecifier *node) : CGen(program, node) {}

    void TypeSpecifier::generate(File *file) {
      CGen::generate(file);
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