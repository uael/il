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

#include <iostream>
#include "expr.h"
#include "type.h"

namespace dyc {
  namespace ast {
    expr_op_t::expr_op_t(expr_op_t::kind_t kind, expr_t *op1) : kind(kind), op1(op1) {}

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
          return "=";
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
          return "";
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

    void expr_op_t::write(writer_t *writer) {
      switch (kind) {
        case INC_PRE:
          *writer << "++" << op1;
          break;
        case DEC_PRE:
          *writer << "--" << op1;
          break;
        case AND_PRE:
          *writer << "&" << op1;
          break;
        case ADD_PRE:
          *writer << "+" << op1;
          break;
        case SUB_PRE:
          *writer << "-" << op1;
          break;
        case MUL_PRE:
          *writer << "*" << op1;
          break;
        case NOT_PRE:
          *writer << "!" << op1;
          break;
        case TID_PRE:
          *writer << "~" << op1;
          break;
        case INC_POST:
          *writer << op1 << "++";
          break;
        case DEC_POST:
          *writer << op1 << "--";
          break;
        case CONST:
          *writer << op1;
          break;
        case ENCLOSE:
          *writer << "(" << op1 << ")";
          break;
        default:
          break;
      }
    }

    expr_dop_t::expr_dop_t(expr_op_t::kind_t kind, expr_t *op1, expr_t *op2) : expr_op_t(kind, op1), op2(op2) {}

    void expr_dop_t::write(writer_t *writer) {
      switch (kind) {
        case POS:
          *writer << op1 << "[" << op2 << "]";
          break;
        case CALL:
          *writer << op1 << "(" << op2 << ")";
          break;
        default:
          *writer << "(" << op1 << op() << op2 << ")";
      }
    }

    expr_ternary_t::expr_ternary_t(expr_t *cond, expr_t *op1, expr_t *op2) : expr_dop_t(TERNARY, op1, op2),cond(cond) {}

    void expr_ternary_t::write(writer_t *writer) {
      *writer << "(" << cond << "?" << op1 << ":" << op2 << ")";
    }

    expr_cast_t::expr_cast_t(expr_t *op1, type_specifier_t *type) : expr_op_t(CAST, op1), type(type) {}

    void expr_cast_t::write(writer_t *writer) {
      *writer << "(" << type << ")" << op1;
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