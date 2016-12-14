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

#include "expr.h"

namespace dyc {
  namespace ast {
    bool expr_assign_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_t::write(writer, ast);
    }

    bool expr_cond_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_assign_t::write(writer, ast);
    }

    bool expr_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return node_t::write(writer, ast);
    }

    bool expr_lor_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_cond_t::write(writer, ast);
    }

    bool expr_land_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_lor_t::write(writer, ast);
    }

    bool expr_or_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_land_t::write(writer, ast);
    }

    bool expr_xor_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_or_t::write(writer, ast);
    }

    bool expr_and_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_xor_t::write(writer, ast);
    }

    bool expr_equal_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_and_t::write(writer, ast);
    }

    bool expr_relational_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_equal_t::write(writer, ast);
    }

    bool expr_shift_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_relational_t::write(writer, ast);
    }

    bool expr_add_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_shift_t::write(writer, ast);
    }

    bool expr_mul_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_add_t::write(writer, ast);
    }

    bool expr_cast_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_mul_t::write(writer, ast);
    }

    bool expr_prefix_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_cast_t::write(writer, ast);
    }

    bool expr_postfix_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_prefix_t::write(writer, ast);
    }

    expr_primary_t::expr_primary_t() {}

    expr_primary_t::expr_primary_t(expr_const_t *const_expr) : const_expr(const_expr) {}

    expr_primary_t::expr_primary_t(expr_t *expr) : expr(expr) {}

    bool expr_primary_t::write(generator_t::writer_t *writer, ast_t *ast) {
      return expr_postfix_t::write(writer, ast);
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