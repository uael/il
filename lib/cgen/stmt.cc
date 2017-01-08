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

#include "stmt.h"

namespace Jay {
  namespace Gen {
    Stmt::Stmt(Ast::Program *program, Ast::Stmt *node) : CGen(program, node) {}

    void Stmt::generate(File *file) {
      CGen::generate(file);
    }

    StmtIter::StmtIter(Ast::Program *program, Ast::StmtIter *node) : CGen(program, node) {}

    void StmtIter::generate(File *file) {
      CGen::generate(file);
    }

    StmtSelect::StmtSelect(Ast::Program *program, Ast::StmtSelect *node) : CGen(program, node) {}

    void StmtSelect::generate(File *file) {
      CGen::generate(file);
    }

    StmtDecl::StmtDecl(Ast::Program *program, Ast::StmtDecl *node) : CGen(program, node) {}

    void StmtDecl::generate(File *file) {
      CGen::generate(file);
    }

    StmtExpr::StmtExpr(Ast::Program *program, Ast::StmtExpr *node) : CGen(program, node) {}

    void StmtExpr::generate(File *file) {
      CGen::generate(file);
    }

    StmtCompound::StmtCompound(Ast::Program *program, Ast::StmtCompound *node) : CGen(program, node) {}

    void StmtCompound::generate(File *file) {
      CGen::generate(file);
    }

    StmtJump::StmtJump(Ast::Program *program, Ast::StmtJump *node) : CGen(program, node) {}

    void StmtJump::generate(File *file) {
      CGen::generate(file);
    }

    StmtLabel::StmtLabel(Ast::Program *program, Ast::StmtLabel *node) : CGen(program, node) {}

    void StmtLabel::generate(File *file) {
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