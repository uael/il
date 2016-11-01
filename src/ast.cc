/*
 * Copyright (c) 2016, Abel Lucas <ylilt71@gmail.com>
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

#include <cstdio>
#include <iostream>
#include "ast.h"

namespace ddc {

  _type_t::_type_t(const identifier_t &id)
    : id(id) { std::cout << "type : " << id.id << std::endl; }

  _signature_t::_signature_t(const type_t &type)
    : type(type) { std::cout << "signature : " << type.id.id << std::endl; }

  _identifier_t::_identifier_t(const std::string &id)
    : id(id) { std::cout << "identifier : " << id << std::endl; }

  _declaration_t::_declaration_t(const identifier_t &identifier, const signature_t &signature)
    : identifier(identifier), signature(signature) {
    std::cout << "declaration : " << identifier.id << " " << signature.type.id.id << std::endl;
  }
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
