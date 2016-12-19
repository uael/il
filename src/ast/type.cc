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
    type_callable_t::type_callable_t(type_specifier_t *type, type_specifier_t *args_types)
      : type(type), args_types(args_types) {}

    void type_callable_t::accept(node_t *scope) {
      ACCEPT(type);
      ACCEPT(args_types);
      node_t::accept(scope);
    }

    void type_callable_t::write(writer_t *writer) {
      node_t::write(writer);
    }

    type_ptr_t::type_ptr_t(type_specifier_t *type) : type(type) {}

    void type_ptr_t::accept(node_t *scope) {
      ACCEPT(type);
      node_t::accept(scope);
    }

    void type_ptr_t::write(writer_t *writer) {
      node_t::write(writer);
    }

    type_array_t::type_array_t(type_specifier_t *type, expr_t *fixed_size) : type(type), fixed_size(fixed_size) {}

    void type_array_t::accept(node_t *scope) {
      ACCEPT(type);
      ACCEPT(fixed_size);
      node_t::accept(scope);
    }

    void type_array_t::write(writer_t *writer) {
      node_t::write(writer);
    }

    type_internal_t::type_internal_t(type_internal_t::kind_t kind) : kind(kind) {}

    void type_internal_t::write(writer_t *writer) {
      type_t::write(writer);
    }

    type_userdef_t::type_userdef_t(identifier_t *id) : id(id) {}

    void type_userdef_t::accept(node_t *scope) {
      ACCEPT(id);
      node_t::accept(scope);
    }

    void type_userdef_t::write(writer_t *writer) {
      type_t::write(writer);
    }

    type_generic_t::type_generic_t(identifier_t *id, type_specifier_t *types) : type_userdef_t(id), types(types) {}

    void type_generic_t::accept(node_t *scope) {
      ACCEPT(types);
      type_userdef_t::accept(scope);
    }

    void type_generic_t::write(writer_t *writer) {
      type_userdef_t::write(writer);
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