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

#ifndef JAY_LANG_H
#define JAY_LANG_H

#include <string>

#define list_foreach(v, list) for ( \
    __typeof__((list)->next) v = (list)->next; \
    v != (list); \
    v = v->next \
  )

namespace Jayl {
  template <typename T>
  struct ListNode {
    T value = nullptr;
    ListNode<T> *prev, *next;

    ListNode() {};
    ListNode(T value) : value(value) {}
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

    T first() {
      return ListNode<T>::next->value;
    }

    T last() {
      return ListNode<T>::next->prev;
    }

    void clear() {
      size = 0;
      list_foreach(item, this) {
        delete item;
      }
    }

    void push(T value) {
      ListNode<T> *node = new ListNode<T>(value);
      link(node, ListNode<T>::prev, this);
    }

    void unshift(T value) {
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

  struct Sentence;
  struct Word;
  struct Id;
  struct Expr;
  struct Stmt;
  struct Type;

  struct Word {
    virtual ~Word() {}
  };

  struct Id : Word {
    std::string value;

    Id(const std::string &value) : value(value) {}
  };

  struct Expr : Word {
    Type *type;
  };

  enum Operator {

  };

  struct OperationExpr : Expr {
    Operator op;
    Expr *left, right;
  };

  struct TernaryExpr : Expr {
    Expr *condition;
    Expr *left, right;
  };

  struct CastExpr : Expr {
    Expr *expr;
  };

  struct CallExpr : Expr {
    Expr *expr;
    List<Expr *> args;
  };

  struct PositionExpr {
    Expr *expr;
    Expr *position;
  };

  struct UnaryExpr : Expr {
    Operator op;
    Expr *expr;
  };

  struct PostfixExpr : Expr {
    Operator op;
    Expr *expr;
  };

  struct AccessExpr : Expr {
    PostfixExpr *expr;
    UserType *to;
  };

  struct SizeofExpr : Expr {
    Expr *expr;
    Type *type;
  };

  struct TypeofExpr : Expr {
    Expr *expr;
    Type *type;
  };

  struct ConstExpr : Expr {};

  struct FrameExpr : Expr {
    Expr *expr;
  };

  struct Stmt : Word {

  };

  struct Type : Word {

  };

  template <typename TOfType = Type *>
  struct TypeSpecifier : Type {
    TOfType type;
  };

  template <typename TOfType>
  struct TypePtr : TypeSpecifier<TOfType> {

  };

  template <typename TOfType>
  struct TypeArray : TypeSpecifier<TOfType> {
    Expr size;
  };

  template <typename TOfType>
  struct TypeCallable : TypeSpecifier<TOfType> {
    List<Arg *> args;
  };

  template <typename TOfType>
  struct TypeGeneric : TypeSpecifier<TOfType> {
    List<Type *> generics;
  };

  struct Scalar : Type {
    enum Kind {

    };
  };

  struct UserType : Type {
    Id name;
  };


  struct Generic : UserType {
    List<Type *> constraints;
  };

  struct Namespace : UserType {
    List<UserType *> user_types;
  };

  struct Var : UserType {
    Type *type;
    Expr *value;
  };

  struct Const : Var {};

  struct Arg : Var {};

  struct Property : Var {};

  struct Use : UserType {
    List<UserType *> types;
  };

  struct Function : UserType {
    Type *return_type;
    List<Arg *> args;
    List<Generic *> generics;
    List<Stmt *> stmts;
  };

  struct Method : Function {
    UserType *scope;
  };

  struct Interface : UserType {
    List<Generic *> generics;
    List<TypeGeneric<Interface *> *> extends;
    List<Const *> constants;
    List<Property *> properties;
    List<Method *> ctors;
    List<Method *> dtors;
    List<Function *> functions;
  };

  struct Struct : UserType {
    List<Generic *> generics;
    TypeGeneric<Struct *> *extends;
    List<TypeGeneric<Interface *> *> implements;
    List<Const *> constants;
    List<Property *> properties;
    List<Method *> ctors;
    List<Method *> dtors;
    List<Function *> functions;
  };

  struct Frame : Type {
    Id name;
    TypeGeneric<Type *> *extends;
    List<TypeGeneric<Interface> *> implements;
    List<Const *> constants;
    List<Property *> properties;
    List<Method *> ctors;
    List<Method *> dtors;
    List<Function *> functions;
  };
}

#endif /* JAY_LANG_H */
