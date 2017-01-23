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
#include <vector>
#include <functional>

#define foreach(v, values) for (__typeof__(*values) *v = values; v; v = (__typeof__(values)) v->next)
#define rforeach(v, values) for (__typeof__(*values) *v = values; v; v = (__typeof__(values)) v->prev)
#define as(node, type) dynamic_cast<type *>(node)
#define cast(node, dnode) (dnode = dynamic_cast<__typeof__(dnode)>(node))
#define ACCEPT(node) if ((node)) { \
    Jay::Ast::Node * __nptr = dynamic_cast<Jay::Ast::Node *>(node); \
    if (__nptr) { \
      __nptr->accept(this); \
      childs.push(__nptr); \
    } \
  }


#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({            \
 const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
 (type *)( (char *)__mptr - offsetof(type,member) );})

#define list_foreach(v, list) for ( \
    __typeof__((list)->next) v = (list)->next; \
    v != (list); \
    v = v->next \
  )

namespace Jay {
  class location;

  namespace Gen {
    struct Generator;
  }

  namespace Ast {
    struct File;
    struct Id;
    struct Generic;
    struct Closure;
    struct Use;

    struct Decl;
    struct DeclInclude;
    struct DeclUse;
    struct DeclNested;
    struct DeclMember;
    struct DeclProperty;
    struct DeclFunction;
    struct DeclCtor;
    struct DeclDtor;
    struct DeclFrame;

    struct TypeSpecifier;
    struct TypeCallable;
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

    struct Const;
    struct ConstThis;
    struct ConstValue;
    struct ConstLambda;
    struct ConstInitializer;
    struct ConstNull;

    struct Named {
      Id *ids = nullptr;

      Named(Id *ids);

      Id *find_id(std::string value);
    };

    template <typename T>
    struct ListNode {
      T *value = nullptr;
      ListNode<T> *prev, *next;

      ListNode() {};
      ListNode(T *value) : value(value) {}
      ListNode(ListNode<T> *prev, ListNode<T> *next) : prev(prev), next(next) {}

      virtual ~ListNode() {}
    };

    template <typename T>
    struct List : ListNode<T> {
      int size = 0;

      List() : ListNode<T>(this, this) {};

      virtual ~List() {
        clear();
      };

      ListNode<T> *link(ListNode<T> *node, ListNode<T> *prev, ListNode<T> *next) {
        size++;
        node->next = prev->next;
        node->prev = next->prev;
        prev->next = node;
        next->prev = node;
        return node;
      }

      T *first() {
        return ListNode<T>::next->value;
      }

      T *last() {
        return ListNode<T>::next->prev;
      }

      void clear() {
        size = 0;
        list_foreach(item, this) {
          delete item;
        }
      }

      void push(T *value) {
        ListNode<T> *node = new ListNode<T>(value);
        link(node, ListNode<T>::prev, this);
      }

      void unshift(T *value) {
        ListNode<T> *node = new ListNode<T>(value);
        link(node, this, ListNode<T>::next);
      }

      void push(List<T> *list) {
        list_foreach(item, list) {
          push(item->value);
        }
      }

      ListNode<T> *find(T *value) {
        list_foreach(item, this) {
          if (item->value == value) {
            return item;
          }
        }
        return nullptr;
      }
    };

    struct Node {
      Node *scope = nullptr;
      Node *prev = nullptr;
      Node *next = nullptr;
      location *loc = nullptr;
      List<Node> childs;

      virtual ~Node();
      virtual void accept(Node *scope);

      int count() {
        Node *n = this;
        int result = 0;
        while (n) { n = n->next; result++; }
        return result;
      }

      Node *last() {
        Node *last = this;
        while (last->next) {
          last = last->next;
        }
        return next;
      }

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

      Node *replace(Node *by) {
        by->prev = prev;
        if (prev) {
          prev->next = by;
        }
        by->next = next;
        if (next) {
          next->prev = by;
        }
        return by;
      }

      void insert_after(Node *node) {
        if (next) {
          next->prev = node;
        }
        node->next = next;
        node->prev = this;
        next = node;
      }

      void insert_before(Node *node) {
        if (prev) {
          prev->next = node;
        }
        node->prev = prev;
        node->next = this;
        prev = node;
      }

      template<typename T>
      T *unshift(T *node) {
        node->next = this;
        this->prev = node;
        return node;
      }

      template <typename T>
      List<T> select(std::function<bool(T *)> predicate = [](T *i) { return true; }) {
        List<T> result;
        T *match;
        list_foreach(item, &childs) {
          foreach(node, item->value) {
            if ((match = as(node, T))) {
              if (!predicate || predicate(match)) {
                result.push(match);
              }
            }
            List<T> matches = node->select<T>(predicate);
            result.push(&matches);
          }
        }
        return result;
      }

      template <typename T>
      T *find(std::string value) {
        List<Named> list = this->select<Named>();
        list_foreach(item, &list) {
          if (item->value->find_id(value)) {
            return as(item->value, T);
          }
        }
        return nullptr;
      }

      template <typename T>
      T *next_scope() {
        T *result = nullptr;
        Node *s = scope;
        while (s) {
          if ((result = as(s, T))) {
            return result;
          }
          s = s->scope;
        }
        return nullptr;
      }

      template <typename T>
      T *findup(std::string name) {
        T *match = nullptr;
        Node *s = scope;
        while (s) {
          if ((match = s->find<T>(name))) {
            break;
          }
          s = s->next_scope<Node>();
        }
        return match;
      }

      virtual void generate(Gen::Generator *generator) {};
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
