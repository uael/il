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

#include "type.h"
#include "decl.h"

namespace dyc {
  namespace ast {
    type_specifier_t::type_specifier_t(type_t *type) : type(type) {}

    void type_specifier_t::accept(node_t *scope) {
      ACCEPT(type);
      for (auto &t : call_chain) {
        ACCEPT(t);
      }
      node_t::accept(scope);
    }

    void type_specifier_t::write(writer_t *writer) {
      node_t::write(writer);
    }

    type_scalar_t::type_scalar_t(type_scalar_t::kind_t kind) : kind(kind) {}

    void type_scalar_t::write(writer_t *writer) {
      type_t::write(writer);
    }

    type_generic_t::type_generic_t(std::string *id) : id(id) {}

    void type_generic_t::write(writer_t *writer) {
      type_t::write(writer);
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