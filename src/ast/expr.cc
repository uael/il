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
#include "type.h"

namespace dyc {
  namespace ast {
    expr_op_t::expr_op_t(expr_op_t::kind_t kind, expr_t *op1) : kind(kind), op1(op1) {}

    void expr_op_t::accept(node_t *scope) {
      node_t::accept(scope);
    }

    std::string expr_op_t::op() {
      switch (kind) {
        case INC_PRE:
        case INC_POST:
          return "++";
        case DEC_PRE:
        case DEC_POST:
          return "--";
        case AND_PRE:
        case AND:
          return "&";
        case ADD_PRE:
        case ADD:
          return "+";
        case SUB_PRE:
        case SUB:
          return "-";
        case MUL_PRE:
        case MUL:
          return "*";
        case NOT_PRE:
          return "!";
        case TID_PRE:
          return "~";
        case ASSIGN:
          return "=";
        case MUL_ASSIGN:
          return "*=";
        case DIV_ASSIGN:
          return "/=";
        case MOD_ASSIGN:
          return "%=";
        case ADD_ASSIGN:
          return "+=";
        case SUB_ASSIGN:
          return "-=";
        case LEFT_ASSIGN:
          return "<<=";
        case RIGHT_ASSIGN:
          return ">>=";
        case AND_ASSIGN:
          return "&=";
        case XOR_ASSIGN:
          return "^=";
        case OR_ASSIGN:
          return "|=";
        case DIV:
          return "/";
        case MOD:
          return "%";
        case LEFT:
          return "<<";
        case RIGHT:
          return ">>";
        case XOR:
          return "^";
        case OR:
          return "|";
        case LAND:
          return "&&";
        case LOR:
          return "||";
        case EQ:
          return "==";
        case NEQ:
          return "!=";
        case LT:
          return "<";
        case GT:
          return ">";
        case LTE:
          return "<=";
        case GTE:
          return ">=";
        default:
          return "";
      }
    }

    expr_dop_t::expr_dop_t(expr_op_t::kind_t kind, expr_t *op1, expr_t *op2) : expr_op_t(kind, op1), op2(op2) {}

    void expr_dop_t::write(writer_t *writer) {
      *writer << "(" << op1 << op() << op2 << ")";
    }

    expr_ternary_t::expr_ternary_t(expr_t *cond, expr_t *op1, expr_t *op2) : expr_dop_t(TERNARY, op1, op2),cond(cond) {}

    void expr_ternary_t::write(writer_t *writer) {
      *writer << "(" << cond << "?" << op1 << ":" << op2 << ")";
    }

    expr_cast_t::expr_cast_t(expr_t *op1, type_specifier_t *type) : expr_op_t(CAST, op1), type(type) {}

    void expr_cast_t::write(writer_t *writer) {
      *writer << "(" << type << ")" << op1;
    }

    expr_call_t::expr_call_t(expr_t *op1, expr_t *op2) : expr_dop_t(CALL, op1, op2) {}

    void expr_call_t::write(writer_t *writer) {
      *writer << op1 << "(" << op2 << ")";
    }

    expr_pos_t::expr_pos_t(expr_t *op1, expr_t *op2) : expr_dop_t(POS, op1, op2) {}

    void expr_pos_t::write(writer_t *writer) {
      *writer << op1 << "[" << op2 << "]";
    }

    expr_prefix_t::expr_prefix_t(expr_op_t::kind_t kind, expr_t *op1) : expr_op_t(kind, op1) {}

    void expr_prefix_t::write(writer_t *writer) {
      *writer << op() << op1;
    }

    expr_postfix_t::expr_postfix_t(expr_op_t::kind_t kind, expr_t *op1) : expr_op_t(kind, op1) {}

    void expr_postfix_t::write(writer_t *writer) {
      *writer  << op1 << op();
    }

    expr_primary_t::expr_primary_t(expr_op_t::kind_t kind, expr_t *op1) : expr_op_t(kind, op1) {}

    void expr_primary_t::write(writer_t *writer) {
      if (kind == ENCLOSE) {
        *writer << "(" << op1 << ")";
      } else {
        *writer << op1;
      }
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