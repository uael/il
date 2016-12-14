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

#ifndef _AST_EXPR_H
#define _AST_EXPR_H

#include <tuple>
#include <vector>
#include "node.h"
#include "closure.h"

namespace dyc {
  namespace ast {
    struct expr_t : node_t, closure_t {
      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_assign_t : expr_t {
      enum kind_t {
        NONE, MUL, DIV, MOD, ADD, SUB, LEFT, RIGHT, AND, XOR, OR
      };
      std::vector<std::tuple<kind_t, expr_prefix_t *>> assign_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_cond_t : expr_assign_t {
      std::vector<std::tuple<expr_lor_t *, expr_t *>> ternary_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_lor_t : expr_cond_t {
      std::vector<expr_land_t *> lor_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_land_t : expr_lor_t {
      std::vector<expr_or_t *> land_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_or_t : expr_land_t {
      std::vector<expr_xor_t *> or_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_xor_t : expr_or_t {
      std::vector<expr_and_t *> xor_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_and_t : expr_xor_t {
      std::vector<expr_equal_t *> and_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_equal_t : expr_and_t {
      std::vector<expr_relational_t *> eq_chain;
      std::vector<expr_relational_t *> neq_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_relational_t : expr_equal_t {
      std::vector<expr_shift_t *> lt_chain;
      std::vector<expr_shift_t *> gt_chain;
      std::vector<expr_shift_t *> le_chain;
      std::vector<expr_shift_t *> ge_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_shift_t : expr_relational_t {
      std::vector<expr_add_t *> ls_chain;
      std::vector<expr_add_t *> rs_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_add_t : expr_shift_t {
      std::vector<expr_mul_t *> add_chain;
      std::vector<expr_mul_t *> sub_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_mul_t : expr_add_t {
      std::vector<expr_cast_t *> mul_chain;
      std::vector<expr_cast_t *> div_chain;
      std::vector<expr_cast_t *> mod_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_cast_t : expr_mul_t {
      std::vector<type_specifier_t *> cast_chain;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_prefix_t : expr_cast_t {
      int inc_lvl;
      int dec_lvl;
      int and_lvl;
      int add_lvl;
      int sub_lvl;
      int mul_lvl;
      int not_lvl;
      int tid_lvl;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_postfix_t : expr_prefix_t {
      std::vector<expr_t *> position_chain;
      std::vector<expr_t *> call_chain;
      int post_inc_lvl = 0;
      int post_dec_lvl = 0;

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_primary_t : expr_postfix_t {
      expr_const_t *const_expr = nullptr;
      expr_t *expr = nullptr;

      expr_primary_t();
      expr_primary_t(expr_const_t *const_expr);
      expr_primary_t(expr_t *expr);

      bool write(generator_t::writer_t *writer, ast_t *ast) override;
    };

    struct expr_const_t : node_t {};
  }
}

#endif /* _AST_EXPR_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */