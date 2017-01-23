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

#ifndef _CGEN_GENERATOR_H
#define _CGEN_GENERATOR_H

#include <fstream>
#include <algorithm>
#include "ast/ast.h"

#define op(type, ...) (new Jay::Gen::type(__VA_ARGS__))

namespace Jay {
  namespace Gen {
    struct File {
      struct MatchPathSeparator {
        bool operator()(char ch) const {
          return ch == '/';
        }
      };

      const std::string &filename;
      const std::string &basename;
      std::string name;
      std::ofstream ofstream;

      std::string header;
      std::string body;
      std::string footer;

      File(const std::string &filename, const std::string &ext);

      virtual ~File();

      void open();
      void close();
      void write();
    };

    struct Op {
      virtual ~Op();
      virtual void make(Generator *generator)= 0;
    };

    struct All : Op {
      Ast::Node *node;
      const std::string &eot;
      bool all = false;

      All(Ast::Node *node, const std::string &eot = "", bool all = false);

      void make(Generator *generator) override;
    };

    struct AllName : All {
      AllName(Ast::Node *node, const std::string &eot, bool all = false);

      void make(Generator *generator) override;
    };

    struct AllIndent : All {
      AllIndent(Ast::Node *node, const std::string &eot, bool all = false);

      void make(Generator *generator) override;
    };

    struct StmtsNoReturn : Op {
      Ast::Stmt *stmts;
      Ast::DeclMember *member;
      const std::string &eot;
      bool all = false;

      StmtsNoReturn(Ast::Stmt *stmts, Ast::DeclMember *member, const std::string &eot, bool all = false);

      void make(Generator *generator) override;
    };

    struct Indent : Op {
      void make(Generator *generator) override;
    };

    struct IndentBack : Op {
      void make(Generator *generator) override;
    };

    struct IndentCursor : Op {
      void make(Generator *generator) override;
    };

    struct Closure : Op {
      Ast::Closure *closure;

      Closure(Ast::Closure *closure);

      void make(Generator *generator) override;
    };

    struct Generator {
      Ast::Program *program;
      File *source;
      File *header;
      std::string cursor;
      int indent_lvl;
      Ast::DeclMember *member = nullptr;

      Generator(Ast::Program *program);

      void reset() {
        cursor = "";
      };

      Generator &operator <<(std::string *str) {
        if (str) cursor += *str;
        return *this;
      }

      Generator &operator <<(std::string str) {
        cursor += str;
        return *this;
      }

      Generator &operator <<(Ast::Node *node) {
        if (node) node->generate(this);
        return *this;
      }

      Generator &operator <<(Gen::Op *op) {
        if (op) op->make(this);
        delete op;
        return *this;
      }
    };
  }
}

#endif /* _CGEN_GENERATOR_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */