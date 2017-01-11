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

#define TAB "  "

namespace Jay {
  namespace Gen {
    File::File(const std::string &filename, const std::string &ext) :
      filename(filename),
      basename(
        std::string(std::find_if(filename.rbegin(), filename.rend(), MatchPathSeparator()).base(), filename.end())) {
      std::string::const_reverse_iterator pivot = std::find(basename.rbegin(), basename.rend(), '.');
      name = (pivot == basename.rend() ? basename : std::string(basename.begin(), pivot.base() - 1)) + "." + ext;
    }

    void File::open() {
      std::fstream fstream(name);
      if (fstream.good()) {
        if (fstream.is_open()) {
          fstream.close();
        }
        remove(name.c_str());
      }
      if (!ofstream.is_open()) {
        ofstream.open(name);
      }
    }

    void File::close() {
      if (!ofstream.good()) {
        throw new std::runtime_error("File is busy !");
      }
      if (ofstream.is_open()) {
        ofstream.close();
      }
    }

    File::~File() {
      close();
    }

    void File::write() {
      open();
      ofstream << header;
      ofstream << body;
      ofstream << footer;
      close();
    }

    Generator::Generator(Ast::Program *program) : program(program) {}

    All::All(Ast::Node *node, const std::string &eot, bool all) : node(node), eot(eot), all(all) {}

    Op::~Op() {}

    void All::make(Gen::Generator *generator) {
      foreach(child, node) {
        *generator << child;
        if (all || child->next) {
          *generator << eot;
        }
      }
    }

    void Indent::make(Generator *generator) {
      generator->indent_lvl++;
    }

    void IndentBack::make(Generator *generator) {
      generator->indent_lvl--;
    }

    AllIndent::AllIndent(Ast::Node *node, const std::string &eot, bool all) : All(node, eot, all) {}

    void AllIndent::make(Generator *generator) {
      foreach(child, node) {
        for (int i = 0; i < generator->indent_lvl; i++) {
          *generator << TAB;
        }
        *generator << child;
        if (all || child->next) {
          *generator << eot;
        }
      }
    }

    void IndentCursor::make(Generator *generator) {
      for (int i = 0; i < generator->indent_lvl; i++) {
        *generator << TAB;
      }
    }
  }

  namespace Ast {
    void Program::generate(Gen::Generator *generator) {
      *generator << op(All, files);
    }

    void File::generate(Gen::Generator *generator) {
      generator->header = new Gen::File(filename, "h");
      generator->source = new Gen::File(filename, "c");
      generator->source->header += "#include \"" + generator->header->name + "\"\n";
      *generator << op(All, decls, "\n", true);
      generator->header->write();
      generator->source->write();
      delete generator->header;
      delete generator->source;
    }

    void DeclFunction::generate(Gen::Generator *generator) {
      generator->reset();
      *generator << op(IndentCursor) << type_specifier << " " << ids->value << "(" << op(All, args, ", ") << ")";
      generator->header->body += "\n" + generator->cursor + ";";
      if (closure) {
        *generator << " " << closure;
        generator->source->body += "\n" + generator->cursor;
      }
    }

    void DeclProperty::generate(Gen::Generator *generator) {
      if (as(scope, DeclFunction)) {
        *generator << type_specifier << " " << ids;
        TypeArray *arr;
        if ((arr = as(type_specifier, TypeArray))) {
          *generator << "[" << arr->fixed_size << "]";
        }
        if (closure) {
          *generator << " = " << closure;
        }
      }
    }

    void TypeInternal::generate(Gen::Generator *generator) {
      switch (kind) {
        case SELF: *generator << ""; break;
        case STATIC: *generator << ""; break;
        case VOID: *generator << "void"; break;
        case BOOL: *generator << "int"; break;
        case CHAR: *generator << "char"; break;
        case INT: *generator << "int"; break;
        case UINT: *generator << "unsigned int"; break;
        case SINT: *generator << "signed int"; break;
        case SHORT: *generator << "short"; break;
        case USHORT: *generator << "unsigned short"; break;
        case STRING: *generator << "char *"; break;
        case SSHORT: *generator << "signed short"; break;
        case FLOAT: *generator << "float"; break;
        case UFLOAT: *generator << "unsigned float"; break;
        case SFLOAT: *generator << "signed float"; break;
        case DOUBLE: *generator << "double"; break;
        case UDOUBLE: *generator << "unsigned double"; break;
        case SDOUBLE: *generator << "signed double"; break;
      }
    }

    void TypePtr::generate(Gen::Generator *generator) {
      *generator << type << "*";
    }

    void TypeArray::generate(Gen::Generator *generator) {
      *generator << type;
    }

    void DeclInclude::generate(Gen::Generator *generator) {
      *generator << "#include \"" << includes->value << ".h\"\n" << includes->next;
      generator->source->header += generator->cursor;
    }

    void StmtCompound::generate(Gen::Generator *generator) {
      *generator
        << "{\n"
        << op(Indent)
        << op(AllIndent, stmts, "\n", true)
        << op(IndentBack)
        << op(IndentCursor)
        << "}";
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

    void ExprCast::generate(Gen::Generator *generator) {
      *generator << "(" << type << ")" << op1;
    }

    void ExprPos::generate(Gen::Generator *generator) {
      *generator << op1 << "[" << op2 << "]";
    }

    void ExprKvp::generate(Gen::Generator *generator) {}

    void ExprCall::generate(Gen::Generator *generator) {
      *generator << op1 << "(" << op(All, op2, ", ") << ")";
    }

    void ExprNested::generate(Gen::Generator *generator) {
      *generator << op1 << "->" << id;
    }

    void ExprPostfix::generate(Gen::Generator *generator) {}

    void ExprSizeof::generate(Gen::Generator *generator) {
      *generator << op1 << op_str();
    }

    void ExprDop::generate(Gen::Generator *generator) {
      *generator << op1 << op_str() << op2;
    }

    void ExprPrimary::generate(Gen::Generator *generator) {
      if (kind == Ast::Expr::Kind::ENCLOSE) {
        *generator << "(" << op1 << ")";
      } else {
        *generator << op1;
      }
    }

    void ExprUnary::generate(Gen::Generator *generator) {
      *generator << op_str() << op1;
    }

    void ExprTernary::generate(Gen::Generator *generator) {
      *generator << cond << "?" << op1 << ":" << op2;
    }

    void Id::generate(Gen::Generator *generator) {
      *generator << value;
    }

    void ConstValue::generate(Gen::Generator *generator) {
      *generator << value;
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