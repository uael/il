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
      enum kind_t {
        INC_PRE, DEC_PRE, AND_PRE, ADD_PRE, SUB_PRE, MUL_PRE, NOT_PRE, TID_PRE, POS, CALL, INC_POST, DEC_POST, CONST,
        ENCLOSE, ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, ADD_ASSIGN, SUB_ASSIGN, LEFT_ASSIGN, RIGHT_ASSIGN,
        AND_ASSIGN, XOR_ASSIGN, OR_ASSIGN, MUL, DIV, MOD, ADD, SUB, LEFT, RIGHT, AND, XOR, OR, LAND, LOR, EQ, NEQ, LT,
        GT, LTE, GTE, TERNARY, CAST, KVP
      };
    };

    struct expr_op_t : expr_t {
      kind_t kind;
      expr_t *op1 = nullptr;

      expr_op_t(kind_t kind, expr_t *op1);

      std::string op();
      void accept(node_t *scope) override;
    };

    struct expr_dop_t : expr_op_t {
      expr_t *op2 = nullptr;

      expr_dop_t(kind_t kind, expr_t *op1, expr_t *op2);

      void write(writer_t *writer) override;
      void accept(node_t *scope) override;
    };

    struct expr_ternary_t : expr_dop_t {
      expr_t *cond = nullptr;

      expr_ternary_t(expr_t *cond, expr_t *op1, expr_t *op2);

      void write(writer_t *writer) override;
      void accept(node_t *scope) override;
    };

    struct expr_cast_t : expr_op_t {
      type_specifier_t *type = nullptr;

      expr_cast_t(expr_t *op1, type_specifier_t *type);

      void write(writer_t *writer) override;
      void accept(node_t *scope) override;
    };

    struct expr_call_t : expr_dop_t {
      expr_call_t(expr_t *op1, expr_t *op2);

      void write(writer_t *writer) override;
    };

    struct expr_pos_t : expr_dop_t {
      expr_pos_t(expr_t *op1, expr_t *op2);

      void write(writer_t *writer) override;
    };

    struct expr_prefix_t : expr_op_t {
      expr_prefix_t(kind_t kind, expr_t *op1);

      void write(writer_t *writer) override;
    };

    struct expr_postfix_t : expr_op_t {
      expr_postfix_t(kind_t kind, expr_t *op1);

      void write(writer_t *writer) override;
    };

    struct expr_primary_t : expr_op_t {
      expr_primary_t(kind_t kind, expr_t *op1);

      void write(writer_t *writer) override;
    };

    struct expr_kvp_t : expr_dop_t {
      expr_kvp_t(expr_t *op1, expr_t *op2);
    };

    struct expr_const_t : expr_t {};
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