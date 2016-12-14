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

#include "const.h"
#include "closure.h"

namespace dyc {
  namespace ast {
    const_value_t::const_value_t(const_value_t::kind_t kind, std::string *value) : kind(kind), value(value) {}

    bool const_value_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_const_t::write(writer, ast);
    }

    const_lambda_t::const_lambda_t(identifier_t *args, closure_t *closure) : args(args), closure(closure) {}

    bool const_lambda_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_const_t::write(writer, ast);
    }

    const_initializer_t::const_initializer_t(expr_t *list) : list(list) {}
    const_initializer_t::const_initializer_t(ds_map_t *map) : map(map) {}

    bool const_initializer_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_const_t::write(writer, ast);
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