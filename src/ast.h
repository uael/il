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

#ifndef _AST_H
#define _AST_H

#include <vector>
#include <string>

using namespace std;

#define DECLARE_SIGN(x) \
  struct x##_t; \
  typedef vector<x##_t *> x##s_t; \
  struct x##_decl_t; \
  typedef vector<x##_decl_t *> x##_decls_t

#define DECLARE_SIGN_CONTAINER(x) \
  struct i##x##_t {}; \
  template <typename TSign = sign_t> \
  struct x##_t; \
  typedef vector<i##x##_t *> x##s_t; \
  struct i##x##_decl_t : decl_t {}; \
  template <typename TSign = sign_t> \
  struct x##_decl_t; \
  typedef vector<i##x##_decl_t *> x##_decls_t

namespace ddc {
  namespace ast {
    typedef string id_t;
    typedef vector<id_t *> ids_t;

    struct modifier {
      enum modifier_type {
        PUBLIC,
        PRIVATE,
        PROTECTED
      };
    };
    typedef modifier::modifier_type modifier_t;

    struct op {
      enum op_type {
        ADD,
        SUB,
        DIV,
        MUL,
        POW
      };
    };
    typedef op::op_type op_t;

    struct cmp {
      enum cmp_type {
        EQ,
        NEQ,
        LT,
        LE,
        GT,
        GE
      };
    };
    typedef cmp::cmp_type cmp_t;

    struct node_t {};
    typedef vector<node_t *> nodes_t;

    struct stmt_t : node_t {};
    typedef vector<stmt_t *> stmts_t;

    struct expr_t : stmt_t {};
    typedef vector<expr_t *> exprs_t;

    struct decl_t : stmt_t {};
    typedef vector<decl_t *> decls_t;

    struct sign_t;
    typedef vector<sign_t *> signs_t;

    struct ival_t : expr_t {};
    typedef vector<ival_t *> vals_t;

    template<typename TSign = sign_t>
    struct sign_container_t;

    template<typename TSign = sign_t>
    struct val_t;

    struct closure_stmt_t;
    struct ret_stmt_t;
    struct if_stmt_t;
    struct while_stmt_t;
    struct for_stmt_t;

    struct assign_expr_t;
    struct cmp_expr_t;
    struct op_expr_t;

    template<typename TSign>
    struct val_t : ival_t {
      static_assert(is_base_of<sign_t, TSign>::value, "TSign must inherit from sign_t");
      const TSign *type;
      const string &value;

      val_t(const TSign *type, const string &value = "none")
        : type(type), value(value) {}
    };

    struct sign_t {
      const id_t &id;
      const string &dfltval;
      const sign_t *parent;

      sign_t(const id_t &id, const sign_t *parent = nullptr)
        : id(id), dfltval("none"), parent(parent) {}

      sign_t(const id_t &id, const string &dfltval, const sign_t *parent = nullptr)
        : id(id), dfltval(dfltval), parent(parent) {}
    };

    template<typename TVar>
    struct assign_expr_t : expr_t {
      const TVar *var;
      const ival_t &value;
    };

    DECLARE_SIGN(scalar);
    DECLARE_SIGN(class);
    DECLARE_SIGN(iface);
    DECLARE_SIGN_CONTAINER(var);
    DECLARE_SIGN_CONTAINER(const);
    DECLARE_SIGN_CONTAINER(arg);
    DECLARE_SIGN_CONTAINER(func);
    DECLARE_SIGN_CONTAINER(meth);
    DECLARE_SIGN_CONTAINER(prop);

    template<typename TSign>
    struct sign_container_t : sign_t {
      static_assert(is_base_of<sign_t, TSign>::value, "TSign must inherit from sign_t");
      const TSign *child;

      sign_container_t(const id_t &id, const TSign *child, const sign_t *parent = nullptr)
        : sign_t(id, parent), child(child) {}
    };

    struct scalar_t : sign_t {
      scalar_t(const id_t &id, const string &dfltval)
        : sign_t(id, dfltval) {}
    };

    template<typename TSign>
    struct var_t : ivar_t, sign_container_t<TSign> {
      const val_t<TSign> &value;

      var_t(const id_t &id, const TSign *type, const string &value, const var_t<TSign> *parent = nullptr)
        : sign_container_t<TSign>(id, type, parent), value(val_t<TSign>(type, value)) {}

      var_t(const id_t &id, const TSign *type, const val_t<TSign> &value, const var_t<TSign> *parent = nullptr)
        : sign_container_t<TSign>(id, type, parent), value(value) {}

      var_t(const id_t &id, const TSign *type, const var_t<TSign> *parent = nullptr)
        : sign_container_t<TSign>(id, type, parent), value(val_t<TSign>(type)) {}
    };

    template<typename TSign>
    struct var_decl_t : decl_t, var_t<TSign> {
      var_decl_t(const id_t &id, const TSign *type, const val_t<TSign> &value, const var_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, value, parent) {}
    };

    template<typename TSign>
    struct const_t : iconst_t, var_t<TSign> {
      const modifier_t &modifier;

      const_t(const id_t &id, const TSign *type, const string &value, const modifier_t &modifier = modifier_t::PUBLIC,
              const const_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, value, parent), modifier(modifier) {}

      const_t(const id_t &id, const TSign *type, const val_t<TSign> &value,
              const modifier_t &modifier = modifier_t::PUBLIC, const const_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, value, parent), modifier(modifier) {}
    };

    template<typename TSign>
    struct const_decl_t : decl_t, const_t<TSign> {
      const_decl_t(const id_t &id, const TSign *type, const val_t<TSign> &value, const modifier_t &modifier,
                   const const_t<TSign> *parent)
        : const_t<TSign>(id, type, value, modifier, parent) {}
    };

    template<typename TSign>
    struct arg_t : iarg_t, var_t<TSign> {
      arg_t(const id_t &id, const TSign *type, const arg_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, parent) {}

      arg_t(const id_t &id, const TSign *type, const string &value, const arg_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, value, parent) {}

      arg_t(const id_t &id, const TSign *type, const val_t<TSign> &value, const arg_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, value, parent) {}
    };

    template<typename TSign>
    struct arg_decl_t : decl_t, arg_t<TSign> {
      arg_decl_t(const id_t &id, const TSign *type, const val_t<TSign> &value, const arg_t<TSign> *parent)
        : arg_t<TSign>(id, type, value, parent) {}
    };

    template<typename TSign>
    struct func_t : ifunc_t, sign_container_t<TSign> {
      const modifier_t &modifier;
      const args_t &args;

      func_t(const id_t &id, const TSign *return_type, const args_t &args = args_t(),
             const modifier_t &modifier = modifier_t::PUBLIC, const func_t<TSign> *parent = nullptr)
        : sign_container_t<TSign>(id, return_type, parent), modifier(modifier), args(args) {}
    };

    template<typename TSign>
    struct func_decl_t : decl_t, func_t<TSign> {
      func_decl_t(const id_t &id, const TSign *return_type, const args_t &args, const modifier_t &modifier,
                  const func_t<TSign> *parent)
        : func_t<TSign>(id, return_type, args, modifier, parent) {}
    };

    template<typename TSign>
    struct meth_t : imeth_t, func_t<TSign> {
      class_t *scope;

      meth_t(const id_t &id, const TSign *return_type, const args_t &args = args_t(),
             const modifier_t &modifier = modifier_t::PUBLIC, const meth_t<TSign> *parent = nullptr)
        : func_t<TSign>(id, return_type, args, modifier, parent) {}

      meth_t(const id_t &id, const TSign *return_type, class_t *scope, const args_t &args = args_t(),
             const modifier_t &modifier = modifier_t::PUBLIC, const meth_t<TSign> *parent = nullptr)
        : func_t<TSign>(id, return_type, args, modifier, parent), scope(scope) {}
    };

    template<typename TSign>
    struct meth_decl_t : decl_t, meth_t<TSign> {
      meth_decl_t(const id_t &id, const TSign *return_type, class_t *scope, const args_t &args,
                  const modifier_t &modifier, const meth_t<TSign> *parent)
        : meth_t<TSign>(id, return_type, scope, args, modifier, parent) {}
    };

    template<typename TSign>
    struct prop_t : iprop_t, var_t<TSign> {
      const modifier_t &modifier;

      prop_t(const id_t &id, const TSign *type, const modifier_t &modifier = modifier_t::PUBLIC,
             const prop_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, parent), modifier(modifier) {}

      prop_t(const id_t &id, const TSign *type, const string &value, const modifier_t &modifier = modifier_t::PUBLIC,
             const prop_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, value, parent), modifier(modifier) {}

      prop_t(const id_t &id, const TSign *type, const val_t<TSign> &value,
             const modifier_t &modifier = modifier_t::PUBLIC, const prop_t<TSign> *parent = nullptr)
        : var_t<TSign>(id, type, value, parent), modifier(modifier) {}
    };

    template<typename TSign>
    struct prop_decl_t : decl_t, prop_t<TSign> {
      prop_decl_t(const id_t &id, const TSign *type, const val_t<TSign> &value, const modifier_t &modifier,
                  const prop_t<TSign> *parent)
        : prop_t<TSign>(id, type, value, modifier, parent) {}
    };

    struct class_t : sign_t {
      const modifier_t &modifier;
      const classs_t &implements;
      const consts_t &consts;
      const props_t &props;
      const meths_t &meths;

      class_t(const id_t &id,
              const classs_t &implements, const class_t *parent = nullptr,
              const modifier_t &modifier = modifier_t::PUBLIC)
        : sign_t(id, parent), implements(implements), consts(consts_t()), props(props_t()), meths(meths_t()),
          modifier(modifier) {}

      class_t(const id_t &id,
              const class_t *parent = nullptr, const classs_t &implements = classs_t(),
              const modifier_t &modifier = modifier_t::PUBLIC)
        : sign_t(id, parent), implements(implements), consts(consts_t()), props(props_t()), meths(meths_t()),
          modifier(modifier) {}

      class_t(const id_t &id,
              const meths_t &meths,
              const classs_t &implements, const class_t *parent = nullptr,
              const modifier_t &modifier = modifier_t::PUBLIC)
        : sign_t(id, parent), implements(implements), consts(consts_t()), props(props_t()), meths(meths),
          modifier(modifier) {}

      class_t(const id_t &id,
              const meths_t &meths,
              const class_t *parent = nullptr, const classs_t &implements = classs_t(),
              const modifier_t &modifier = modifier_t::PUBLIC)
        : sign_t(id, parent), implements(implements), consts(consts_t()), props(props_t()), meths(meths),
          modifier(modifier) {}

      class_t(const id_t &id,
              const props_t &props, const meths_t &meths,
              const classs_t &implements, const class_t *parent = nullptr,
              const modifier_t &modifier = modifier_t::PUBLIC)
        : sign_t(id, parent), implements(implements), consts(consts_t()), props(props), meths(meths),
          modifier(modifier) {}

      class_t(const id_t &id,
              const props_t &props, const meths_t &meths,
              const class_t *parent = nullptr, const classs_t &implements = classs_t(),
              const modifier_t &modifier = modifier_t::PUBLIC)
        : sign_t(id, parent), implements(implements), consts(consts_t()), props(props), meths(meths),
          modifier(modifier) {}

      class_t(const id_t &id,
              const consts_t &consts, const props_t &props, const meths_t &meths,
              const classs_t &implements, const class_t *parent = nullptr,
              const modifier_t &modifier = modifier_t::PUBLIC)
        : sign_t(id, parent), implements(implements), consts(consts), props(props), meths(meths),
          modifier(modifier) {}

      class_t(const id_t &id,
              const consts_t &consts, const props_t &props, const meths_t &meths,
              const class_t *parent = nullptr, const classs_t &implements = classs_t(),
              const modifier_t &modifier = modifier_t::PUBLIC)
        : sign_t(id, parent), implements(implements), consts(consts), props(props), meths(meths),
          modifier(modifier) {}
    };

    struct class_decl_t : decl_t, class_t {
      class_decl_t(const id_t &id, const consts_t &consts, const props_t &props, const meths_t &meths,
                   const class_t *parent, const classs_t &implements, const modifier_t &modifier)
        : class_t(id, consts, props, meths, parent, implements, modifier) {}
    };

    namespace scalar {
      const static scalar_t INT = scalar_t("int", "0");
      const static scalar_t VOID = scalar_t("void", "NULL");
    };

    struct program_t : node_t {
      const funcs_t &funcs;
      const classs_t &classes;

      program_t(const funcs_t &funcs, const classs_t &classes)
        : funcs(funcs), classes(classes) {}
    };
  }
}

#endif /* _AST_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */