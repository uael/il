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
  namespace Gen {
    File::File(const std::string &filename, const std::string &ext) :
      filename(filename),
      basename(
        std::string(std::find_if(filename.rbegin(), filename.rend(), MatchPathSeparator()).base(), filename.end())) {
      std::string::const_reverse_iterator pivot = std::find(basename.rbegin(), basename.rend(), '.');
      name = (pivot == basename.rend() ? basename : std::string(basename.begin(), pivot.base() - 1)) + "." + ext;
    }

    void File::open() {
      remove(name.c_str());
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
          *generator << "  ";
        }
        *generator << child;
        if (all || child->next) {
          *generator << eot;
        }
      }
    }

    void IndentCursor::make(Generator *generator) {
      for (int i = 0; i < generator->indent_lvl; i++) {
        *generator << "  ";
      }
    }

    Closure::Closure(Ast::Closure *closure) : closure(closure) {}

    void Closure::make(Generator *generator) {
      if (closure) {
        Ast::Expr *expr;
        Ast::StmtCompound *compound;
        if ((expr = as(closure, Ast::Expr))) {
          Ast::DeclMember *member = as(expr->scope, Ast::DeclMember);
          if (!closure->macro) {
            Ast::StmtJump *return_stmt = new Ast::StmtJump(expr);
            Ast::StmtCompound *compound_stmt = new Ast::StmtCompound(return_stmt);
            *generator << op(Closure, compound_stmt);
            delete compound_stmt, return_stmt;
          } else
          if (member && member->is_void()) {
            *generator << "do {" << expr << "; } while (0)\n";
          } else {
            *generator << "(" << expr << ")\n";
          }
        } else
        if ((compound = as(closure, Ast::StmtCompound))) {
          Ast::DeclMember *member = as(compound->scope, Ast::DeclMember);
          if (closure->macro) {
            if (member->is_void()) {
              *generator << "do { ";
              if (compound->stmts->next) {
                *generator << "\\\n" << op(Indent);
              }
            } else {
              *generator << "({ ";
            }
            if (compound->stmts->next) {
              *generator << "\\\n" << op(Indent);
            }
          } else {
            *generator << "{\n";
          }
          Ast::StmtJump *jump = as(compound->stmts, Ast::StmtJump);
          if (closure->macro && jump && jump->kind == Ast::StmtJump::RETURN) {
            *generator << jump->expr << ";";
          } else {
            if (!closure->macro || member->is_void()) {
              *generator
                << op(Indent)
                << op(AllIndent, compound->stmts, closure->macro ? " \\\n" : "\n", !closure->macro)
                << op(IndentBack);
                if (!closure->macro) {
                  *generator << op(IndentCursor);
                }
            } else {
              Ast::List<Ast::StmtJump> return_stmts = compound->select<Ast::StmtJump>([=](Ast::StmtJump *j) {
                return j->kind == Ast::StmtJump::RETURN && j->next_scope<Ast::DeclMember>() == member;
              });
              if (!return_stmts.size) {
                *generator
                  << op(Indent)
                  << op(AllIndent, compound->stmts, " \\\n")
                  << op(IndentBack);
              } else
              if (return_stmts.size == 1 && !return_stmts.first()->next) {
                Ast::StmtExpr expr_stmt = Ast::StmtExpr(return_stmts.first()->expr);
                return_stmts.first()->replace(&expr_stmt);
                *generator
                  << op(Indent)
                  << op(AllIndent, compound->stmts, " \\\n")
                  << op(IndentBack);
                return_stmts.first()->replace(return_stmts.first());
              } else {
                generator->member = member;
                *generator << compound;
                generator->member = nullptr;
              }
            }
          }
          if (closure->macro) {
            if (compound->stmts->next) {
              *generator << " \\\n" << op(IndentBack) << op(IndentCursor);
            } else {
              *generator << " ";
            }
            if (member->is_void()) {
              *generator << "} while (0)\n" << op(IndentBack);
            } else {
              *generator << "})\n" << op(IndentBack);
            }
          } else {
            *generator << "}\n";
          }
        }
      }
    }

    AllName::AllName(Ast::Node *node, const std::string &eot, bool all) : All(node, eot, all) {}

    void AllName::make(Generator *generator) {
      foreach(child, node) {
        Ast::Named *named = as(child, Ast::Named);
        if (named) {
          foreach(id, named->ids) {
            *generator << id->value;
            if (all || child->next) {
              *generator << eot;
            }
          }
        }
      }
    }

    void StmtsNoReturn::make(Generator *generator) {
      Ast::StmtJump *return_stmt;
      std::string ret_prop_name = "__ret_" + *member->ids->value, ret_lbl_name = "__lbl_ret_" + *member->ids->value;

      *generator << op(Indent);
      foreach(stmt, stmts) {
        if (!stmt->prev && stmts->scope == member->closure->as_node()) {
          for (int i = 0; i < generator->indent_lvl; i++) {
            *generator << "  ";
          }
          Ast::Id ret_prop_id = Ast::Id(&ret_prop_name);
          Ast::DeclProperty ret_prop = Ast::DeclProperty(&ret_prop_id, member->type_specifier, nullptr, false);
          ret_prop.scope = stmt->scope;
          ret_prop.generate(generator);
          *generator << ";" << eot;
        }

        for (int i = 0; i < generator->indent_lvl; i++) {
          *generator << "  ";
        }

        if ((return_stmt = as(stmt, Ast::StmtJump)) && return_stmt->kind == Ast::StmtJump::RETURN) {
          *generator << ret_prop_name << " = " << return_stmt->expr << "; goto " << ret_lbl_name << ";";
        } else {
          *generator << stmt;
        }

        if (!stmt->next && stmts->scope == member->closure->as_node()) {
          *generator << eot;
          for (int i = 0; i < generator->indent_lvl; i++) {
            *generator << "  ";
          }
          *generator << ret_lbl_name << ":; " << ret_prop_name << ";";
        }

        if (all || stmt->next) {
          *generator << eot;
        }
      }
      *generator << op(IndentBack);
    }

    StmtsNoReturn::StmtsNoReturn(Ast::Stmt *stmts, Ast::DeclMember *member, const std::string &eot, bool all)
      : stmts(stmts), member(member), eot(eot), all(all) {}
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