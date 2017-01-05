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

#ifndef _AST_NODE_H
#define _AST_NODE_H

#include <map>

#define foreach(v, values) for (__typeof__(values) v = values; v; v = (__typeof__(values)) v->next)
#define rforeach(v, values) for (__typeof__(values) v = values; v; v = (__typeof__(values)) v->prev)
#define as(node, type) dynamic_cast<type *>(node)
#define cast(node, dnode) (dnode = dynamic_cast<__typeof__(dnode)>(node))
#define ACCEPT(node) if ((node)) (dynamic_cast<Jay::Ast::Node *>(node))->accept(this)

namespace Jay {
  class location;

  namespace Ast {
    struct File;
    struct Identifier;
    struct Generic;
    struct Closure;

    struct Decl;
    struct DeclInclude;
    struct DeclUse;
    struct DeclNested;
    struct DeclNested;
    struct DeclProperty;
    struct DeclFunction;
    struct DeclCtor;
    struct DeclFrame;

    struct TypeSpecifier;
    struct type_callable_t;
    struct TypePtr;
    struct TypeArray;
    struct Type;
    struct TypeInternal;
    struct TypeUserdef;
    struct TypeGeneric;

    struct Stmt;
    struct StmtExpr;
    struct StmtLabel;
    struct StmtCompound;
    struct StmtSelect;
    struct StmtIter;
    struct StmtJump;
    struct StmtDecl;

    struct Expr;
    struct ExprOp;
    struct ExprDop;
    struct ExprTernary;
    struct ExprCast;
    struct ExprCall;
    struct ExprPos;
    struct ExprUnary;
    struct ExprPostfix;
    struct ExprNested;
    struct ExprSizeof;
    struct ExprPrimary;
    struct ExprKvp;
    struct ExprConst;

    struct ConstThis;
    struct ConstValue;
    struct ConstLambda;
    struct ConstInitializer;

    struct Node {
      Node *scope = nullptr;
      Node *prev = nullptr;
      Node *next = nullptr;
      location *loc = nullptr;

      virtual ~Node();
      virtual void accept(Node *scope);

      template<typename T>
      T *push(T *node) {
        foreach(next, this) {
          if (!next->next) {
            next->next = node;
            node->prev = next;
            break;
          }
        }
        return dynamic_cast<T *>(this);
      }
    };
  }
}

#endif /* _AST_NODE_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */
