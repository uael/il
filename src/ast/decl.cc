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

#include "decl.h"
#include "generic.h"
#include "type.h"

namespace dyc {
  namespace ast {
    decl_include_t::decl_include_t(identifier_t *includes) : includes(includes) {}

    void decl_include_t::accept(node_t *scope) {
      ACCEPT(includes);
      node_t::accept(scope);
    }

    decl_use_t::decl_use_t(identifier_t *uses) : uses(uses) {}

    void decl_use_t::accept(node_t *scope) {
      ACCEPT(uses);
      node_t::accept(scope);
    }

    decl_nested_t::decl_nested_t(identifier_t *name, decl_t *decls) : name(name), decls(decls) {}

    void decl_nested_t::accept(node_t *scope) {
      ACCEPT(name);
      node_t::accept(scope);
    }

    decl_member_t::decl_member_t(identifier_t *ids, type_specifier_t *type_specifier, closure_t *closure)
      : ids(ids), type_specifier(type_specifier), closure(closure) {}

    void decl_member_t::accept(node_t *scope) {
      ACCEPT(ids);
      ACCEPT(type_specifier);
      ACCEPT(closure);
      node_t::accept(scope);
    }

    decl_property_t::decl_property_t(
      identifier_t *ids, type_specifier_t *type_specifier, closure_t *closure, bool assigned)
      : decl_member_t(ids, type_specifier, closure), assigned(assigned) {}

    decl_function_t::decl_function_t(
      identifier_t *ids, generic_t *generics, decl_t *args, type_specifier_t *type_specifier, closure_t *closure)
      : decl_member_t(ids, type_specifier, closure), generics(generics), args(args) {}

    void decl_function_t::accept(node_t *scope) {
      ACCEPT(generics);
      ACCEPT(args);
      decl_member_t::accept(scope);
    }

    decl_ctor_t::decl_ctor_t(decl_t *args, closure_t *closure, const bool &poly)
      : decl_function_t(nullptr, nullptr, args, nullptr, closure), poly(poly) {}
    decl_ctor_t::decl_ctor_t(identifier_t *props_args, closure_t *closure, const bool &poly)
      : decl_function_t(nullptr, nullptr, nullptr, nullptr, closure), props_args(props_args), poly(poly) {}

    void decl_ctor_t::accept(node_t *scope) {
      ACCEPT(props_args);
      decl_function_t::accept(scope);
    }

    decl_dtor_t::decl_dtor_t(decl_t *args, closure_t *closure, const bool &poly) : decl_ctor_t(args, closure, poly) {}
    decl_dtor_t::decl_dtor_t(identifier_t *props_args, closure_t *closure, const bool &poly)
      : decl_ctor_t(props_args, closure, poly) {}

    decl_frame_t::decl_frame_t(identifier_t *name, generic_t *generics, type_specifier_t *type, decl_t *decls)
      : decl_nested_t(name, decls), generics(generics), type(type)  {}

    void decl_frame_t::accept(node_t *scope) {
      ACCEPT(generics);
      ACCEPT(type);
      decl_nested_t::accept(scope);
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