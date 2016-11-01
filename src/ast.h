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

namespace ddc {
  using namespace std;

  typedef class {
    int token;
  } node_t;

  typedef class _identifier_t : public node_t {
  public:
    string id;

    _identifier_t(const string &id);
  } identifier_t;

  typedef class _type_t : public node_t {
  public:
    identifier_t id;

    _type_t(const identifier_t &id);
  } type_t;

  typedef class _signature_t : public node_t {
  public:
    type_t type;
    _signature_t *parent;
    vector<node_t> context;

    _signature_t(const type_t &type);
  } signature_t;

  typedef class _declaration_t : public node_t {
  public:
    identifier_t identifier;
    signature_t signature;

    _declaration_t(const identifier_t &identifier, const signature_t &signature);
  } declaration_t;
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