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

#include <string>
#include <vector>
#include "ddc.h"

namespace ddc {
  using namespace std;

  typedef class {
  } node_t;

  typedef class _identifier_t : public node_t {
  public:
    string id;

    _identifier_t(const string &id);

    virtual string to_string() const {
      return id;
    }
  } identifier_t;

  typedef class _type_t : public node_t {
  public:
    identifier_t id;

    _type_t(const identifier_t &id);

    virtual string to_string() const {
      return id.to_string();
    }
  } type_t;

  typedef class _signature_t : public node_t {
  public:
    type_t type;
    _signature_t *parent;
    vector<node_t> context;

    _signature_t(const type_t &type);

    virtual string to_string() const {
      return type.to_string();
    }
  } signature_t;

  typedef class _declaration_t : public node_t {
  public:
    identifier_t identifier;
    signature_t signature;

    _declaration_t(const identifier_t &identifier, const signature_t &signature);

    virtual string to_string() const {
      return signature.to_string() + " " + identifier.to_string();
    }
  } declaration_t;

  typedef vector<declaration_t *> arguments_t;

  typedef class _function_decl_t : public declaration_t {
  public:
    arguments_t arguments;

    _function_decl_t(
      const identifier_t &identifier,
      const arguments_t &arguments
    );

    _function_decl_t(
      const identifier_t &identifier,
      const signature_t &signature,
      const arguments_t &arguments
    );

    virtual string to_string() const {
      string ret = signature.to_string() + " " + identifier.to_string() + "(";
      for(auto it = this->arguments.begin(); it != this->arguments.end(); ++it) {
        if (it != this->arguments.begin()) {
          ret += ", ";
        }
        ret += (*it)->to_string();
      }
      return ret + ")";
    }
  } function_decl_t;

  typedef vector<_function_decl_t *> functions_t;

  typedef class _function_t : public _function_decl_t {
  public:
    string code;

    _function_t(
      const _function_decl_t &function_decl,
      const string &code
    ) : _function_decl_t(function_decl.identifier, function_decl.signature, function_decl.arguments),
      code(code) { }

    virtual string to_string() const {
      string ret = signature.to_string() + " " + identifier.to_string() + "(";
      for(auto it = this->arguments.begin(); it != this->arguments.end(); ++it) {
        if (it != this->arguments.begin()) {
          ret += ", ";
        }
        ret += (*it)->to_string();
      }
      return _function_decl_t::to_string() + "{ }";
    }
  } function_t;

  typedef class _class_t : public declaration_t {
  public:
    functions_t functions;

    _class_t(
      const identifier_t &identifier,
      const signature_t &signature,
      const functions_t &functions
    ) : _declaration_t(identifier, signature), functions(functions) { }

    _class_t(
      const identifier_t &identifier,
      const functions_t &functions
    ) : _declaration_t(identifier, _signature_t(_type_t(_identifier_t("void")))), functions(functions) { }

    virtual string to_string() const {
      string ret = "class " + identifier.to_string() + "{\n";
      for(auto it = this->functions.begin(); it != this->functions.end(); ++it) {
        ret += "  " + (*it)->to_string() + "\n";
      }
      return ret + "}";
    }
  } class_t;
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