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

#include "generator.h"

namespace Jay {
  namespace Ast {
    void StmtCompound::generate(Gen::Generator *generator) {
      if (generator->member) {
        *generator << op(StmtsNoReturn, stmts, generator->member, " \\\n");
      } else {
        *generator
          << "{\n"
          << op(Indent)
          << op(AllIndent, stmts, "\n", true)
          << op(IndentBack)
          << op(IndentCursor)
          << "}";
      }
    }

    void StmtSelect::generate(Gen::Generator *generator) {
      switch (kind) {
        case IF:
          *generator << "if (" << cond << ")" << stmt;
          if (else_stmt) {
            *generator << "else" << else_stmt;
          }
          break;
        case SWITCH:
          *generator << "switch (" << cond << ")" << stmt;
          break;
      }
    }

    void StmtExpr::generate(Gen::Generator *generator) {
      *generator << expr << ";";
    }

    void StmtJump::generate(Gen::Generator *generator) {
      switch (kind) {
        case Ast::StmtJump::GOTO:
          *generator << "goto " << id << ";";
          break;
        case Ast::StmtJump::CONTINUE:
          *generator << "continue;";
          break;
        case Ast::StmtJump::BREAK:
          *generator << "break;";
          break;
        case Ast::StmtJump::RETURN:
          *generator << "return " << expr << ";";
          break;
      }
    }

    void StmtLabel::generate(Gen::Generator *generator) {
      switch (kind) {
        case LABEL:
          *generator << id << ": " << stmt;
          break;
        case CASE:
          *generator << "case(" << cond << ")" << ": " << stmt;
          break;
        case DEFAULT:
          *generator << "default: " << stmt;
          break;
      }
    }

    void StmtIter::generate(Gen::Generator *generator) {
      switch (kind) {
        case WHILE:
          *generator << "while (" << cond << ")" << stmt;
          break;
        case DO_WHILE:
          *generator << "do " << stmt << " while (" << cond << ")";
          break;
        case FOR:
          *generator << "for (" << decls << " " << cond;
          if (step) {
            *generator << "; ";
            if (as(step, StmtCompound)) {
              *generator << op(Closure, step);
            } else {
              *generator << as(step, Expr);
            }
          }
          *generator << ")" << stmt;
          break;
      }
    }

    void StmtDecl::generate(Gen::Generator *generator) {
      foreach(decl, decls) {
        if (constant && as(decl, DeclFunction)) {
          std::string _c = generator->cursor;
          *generator << decl;
          generator->cursor = _c;
        }
        if (as(decl, DeclProperty)) {
          *generator << decl << ";";
        }
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