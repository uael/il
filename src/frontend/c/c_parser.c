/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <errno.h>

#include "c_parser.h"

#include "c_lexer.h"
#include "compiler.h"
#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

static wulk_expr_t primary_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t postfix_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t unary_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t cast_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t multiplicative_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t additive_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t shift_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t relational_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t equality_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t and_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t exclusive_or_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t inclusive_or_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t logical_and_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t logical_or_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t conditional_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t assignment_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_expr_t constant_expression(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t declaration(wulk_parser_t *self, wulk_program_t *out);
static wulk_type_t declaration_specifiers(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t init_declarator_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t init_declarator(wulk_parser_t *self, wulk_program_t *out);
static wulk_type_t type_specifier(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t struct_or_union_specifier(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t struct_or_union(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t struct_declaration_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t struct_declaration(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t specifier_qualifier_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t struct_declarator_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t struct_declarator(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t enum_specifier(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t enumerator_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t enumerator(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t atomic_type_specifier(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t type_qualifier(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t function_specifier(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t alignment_specifier(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t declarator(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t direct_declarator(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t pointer(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t type_qualifier_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t parameter_type_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t parameter_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t parameter_declaration(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t identifier_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_type_t type_name(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t abstract_declarator(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t direct_abstract_declarator(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t initializer(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t initializer_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t designation(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t designator_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t designator(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t static_assert_declaration(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t statement(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t labeled_statement(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t compound_statement(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t block_item_list(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t block_item(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t expression_statement(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t selection_statement(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t iteration_statement(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t jump_statement(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t translation_unit(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t external_declaration(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t function_definition(wulk_parser_t *self, wulk_program_t *out);
static wulk_entity_t declaration_list(wulk_parser_t *self, wulk_program_t *out);

void c_parser_parse(wulk_parser_t *self, wulk_program_t *out) {
  int r;

  wulk_fe_scope(self);
  self->scope->sym = wulk_sym_put(self->scope, "foo", &r);
  wulk_fe_unscope(self);

  wulk_expr_t expr = expression(self, out);
  wulk_expr_dtor(&expr);

  while (wulk_lexer_next(self->lexer).type != 0);
}

enum {
  C_TOKEN_FLAG_NONE = 0,
  C_TOKEN_FLAG_ENUMERATION_CONSTANT = 1 << 0
};

static wulk_expr_t primary_expression(wulk_parser_t *self, wulk_program_t *out) {
  int r;
  wulk_sym_t *symbol;
  wulk_token_t token;
  wulk_expr_t r1, r2;
  wulk_lloc_t lloc = wulk_lloc_begin(self->lexer);

  switch ((token = wulk_lexer_peek(self->lexer)).type) {
    case C_TOK_IDENTIFIER:
      wulk_lexer_next(self->lexer);
      symbol = wulk_sym_get(self->scope, token.value, &r);
      if (r) {
        wulk_parse_err(self->compiler, token.loc, "Undefined symbol '%s'", token.value);
      }
      if (symbol->flags & C_TOKEN_FLAG_ENUMERATION_CONSTANT) {
        return symbol->entity.variable.initializer;
      }
      return wulk_id(wulk_lloc_end(lloc), symbol->id, symbol->entity.type);
    case C_TOK_NUMBER:
      wulk_lexer_next(self->lexer);
      switch (wulk_const_parse(wulk_lloc_end(lloc), token.value, token.length, &r2)) {
        case ERANGE:
          wulk_parse_err(self->compiler, token.loc, "Numeric literal '%s' is out of range", token.value);
        case 1:
          wulk_parse_err(self->compiler, token.loc, "Invalid numeric literal '%s'", token.value);
        default:
          break;
      }
      return r2;
    case C_TOK_STRING:
      wulk_lexer_next(self->lexer);
      return wulk_const_string(wulk_lloc_end(lloc), token.value);
    case C_TOK_FUNC_NAME:
      if (!(symbol = self->scope->sym) || !wulk_is(symbol->entity, WULK_ENTITY_FUNC)) {
        wulk_parse_err(self->compiler, token.loc,
          "access of __func__ outside of function"
        );
      }
      wulk_lexer_next(self->lexer);
      return wulk_const_string(wulk_lloc_end(lloc), symbol->entity.name);
    case '(':
      wulk_lexer_next(self->lexer);
      r1 = expression(self, out);
      wulk_lexer_consume(self->lexer, ')');
      return wulk_unary(wulk_lloc_end(lloc), WULK_OP_EN, r1);
    default:
      wulk_parse_err(self->compiler, token.loc,
        "Unexpected '%s', not a valid primary expression",
        token.value ? token.value : token.name
      );
  }
}

static wulk_expr_t postfix_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_token_t token;
  wulk_expr_t r1, r2;
  wulk_lloc_t lloc;

  r1 = primary_expression(self, out);
  while (true) {
    lloc = wulk_lloc_begin(self->lexer);
    switch ((token = wulk_lexer_peek(self->lexer)).type) {
      case '[':
        if (!wulk_type_is_ref(r1.type)) {
          wulk_parse_err(self->compiler, token.loc,
            "Array access on non array element"
          );
        }
        wulk_lexer_next(self->lexer);
        r2 = expression(self, out);
        wulk_lexer_consume(self->lexer, ']');
        r1 = wulk_array_read(r1, wulk_lloc_end(lloc), r2);
        break;
      case '(': {
        unsigned i;
        wulk_entity_t param;
        wulk_func_t *func;
        wulk_expr_r args = (wulk_expr_r) {0};
        wulk_type_t type = r1.type;

        if (wulk_is(type, WULK_TYPE_POINTER) && wulk_type_is_func(*type.pointer.of)) {
          type = *type.pointer.of;
        } else if (!wulk_type_is_func(type)) {
          wulk_parse_err(self->compiler, token.loc,
            "Expression must have type pointer to function"
          );
        }
        func = &type.compound.entity->function;
        wulk_lexer_next(self->lexer);
        for (i = 0; i < adt_vector_size(func->params); ++i) {
          if (wulk_lexer_peek(self->lexer).type == ')') {
            wulk_parse_err(self->compiler, token.loc,
              "Too few arguments, expected %zu but got %d",
              adt_vector_size(func->params), i
            );
          }
          param = adt_vector_at(func->params, i);
          r2 = assignment_expression(self, out);
          if (!wulk_type_equals(r2.type, param.type)) {
            r2 = wulk_cast(r2.lloc, param.type, r2);
          }
          adt_vector_push(args, r2);
          if (i < adt_vector_size(func->params) - 1) {
            wulk_lexer_consume(self->lexer, ',');
          }
        }
        wulk_lexer_consume(self->lexer, ')');
        adt_vector_push(args, wulk_expr_undefined());
        r1 = wulk_call(r1, wulk_lloc_end(lloc), wulk_exprs(args.data).exprs);
        break;
      }
      case '.': {
        wulk_field_t *field;

        wulk_lexer_next(self->lexer);
        token = wulk_lexer_consume(self->lexer, C_TOK_IDENTIFIER);
        field = wulk_field_lookup(r1.type, token.value);
        if (!field) {
          wulk_parse_err(self->compiler, token.loc,
            "Invalid access, no member named '%s'",
            token.value
          );
        }
        r1 = wulk_field_read(r1, wulk_id(wulk_lloc_end(lloc), field->name, field->type).id);
        break;
      }
      case C_TOK_PTR_OP: {
        wulk_type_t type;
        wulk_field_t *field;

        wulk_lexer_next(self->lexer);
        if (!wulk_is(type = r1.type, WULK_TYPE_POINTER)) {
          wulk_parse_err(self->compiler, token.loc,
            "Invalid access on non pointer element '%s'",
            token.value
          );
        }
        token = wulk_lexer_consume(self->lexer, C_TOK_IDENTIFIER);
        field = wulk_field_lookup(wulk_type_deref(type), token.value);
        if (!field) {
          wulk_parse_err(self->compiler, token.loc,
            "Invalid access, no member named '%s'",
            token.value
          );
        }
        r1 = wulk_field_read(r1, wulk_id(wulk_lloc_end(lloc), field->name, field->type).id);
        break;
      }
      default:
        return r1;
    }
  }
}

static wulk_expr_t unary_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;
  enum wulk_op_n op;
  wulk_lloc_t lloc = wulk_lloc_begin(self->lexer);

  switch (wulk_lexer_peek(self->lexer).type) {
    case '&':
      op = WULK_OP_AND;
      break;
    case '*':
      op = WULK_OP_AND;
      break;
    case '-':
      op = WULK_OP_SUB;
      break;
    case '~':
      op = WULK_OP_NEG;
      break;
    case '!':
      op = WULK_OP_NOT;
      break;
    case C_TOK_INCREMENT:
      op = WULK_OP_ADD;
      goto self_assign;
    case C_TOK_DECREMENT:
      op = WULK_OP_SUB;
      goto self_assign;
    default:
      return postfix_expression(self, out);
    self_assign:
      wulk_lexer_next(self->lexer);
      lloc = wulk_lloc_end(lloc);
      r1 = unary_expression(self, out);
      return wulk_unary(lloc, WULK_OP_EN, wulk_binary(WULK_OP_ASSIGN, r1, wulk_binary(op, r1, wulk_const_int(r1.lloc, 1))));
  }
  wulk_lexer_next(self->lexer);
  return wulk_unary(wulk_lloc_end(lloc), op, cast_expression(self, out));
}

static wulk_expr_t cast_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_type_t type;
  wulk_token_t token;
  wulk_lloc_t lloc = wulk_lloc_begin(self->lexer);

  if ((token = wulk_lexer_peek(self->lexer)).type == '(') {
    wulk_lexer_next(self->lexer);
    if (!wulk_defined(type = type_name(self, out))) {
      wulk_lexer_undo(self->lexer, token);
      return unary_expression(self, out);
    }
    wulk_lexer_consume(self->lexer, ')');
    return wulk_cast(wulk_lloc_end(lloc), type, cast_expression(self, out));
  }

  return unary_expression(self, out);
}

static wulk_expr_t multiplicative_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = cast_expression(self, out);
  while (true) {
    switch (wulk_lexer_peek(self->lexer).type) {
      case '*':
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_MUL, r1, cast_expression(self, out));
        break;
      case '/':
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_DIV, r1, cast_expression(self, out));
        break;
      case '%':
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_MOD, r1, cast_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static wulk_expr_t additive_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = multiplicative_expression(self, out);
  while (true) {
    switch (wulk_lexer_peek(self->lexer).type) {
      case '+':
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_ADD, r1, multiplicative_expression(self, out));
        break;
      case '-':
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_SUB, r1, multiplicative_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static wulk_expr_t shift_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = additive_expression(self, out);
  while (true) {
    switch (wulk_lexer_peek(self->lexer).type) {
      case C_TOK_LEFT_OP:
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_SHL, r1, additive_expression(self, out));
        break;
      case C_TOK_RIGHT_OP:
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_SHR, r1, additive_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static wulk_expr_t relational_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = shift_expression(self, out);
  while (true) {
    switch (wulk_lexer_peek(self->lexer).type) {
      case '<':
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_LT, r1, shift_expression(self, out));
        break;
      case C_TOK_LE_OP:
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_LE, r1, shift_expression(self, out));
        break;
      case '>':
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_GT, r1, shift_expression(self, out));
        break;
      case C_TOK_GE_OP:
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_GE, r1, shift_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static wulk_expr_t equality_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = relational_expression(self, out);
  while (true) {
    switch (wulk_lexer_peek(self->lexer).type) {
      case C_TOK_EQ_OP:
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_EQ, r1, relational_expression(self, out));
        break;
      case C_TOK_NE_OP:
        wulk_lexer_next(self->lexer);
        r1 = wulk_binary(WULK_OP_NE, r1, relational_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static wulk_expr_t and_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = equality_expression(self, out);
  while (true) {
    if (wulk_lexer_peek(self->lexer).type != '&') {
      return r1;
    }
    wulk_lexer_next(self->lexer);
    r1 = wulk_binary(WULK_OP_AND, r1, equality_expression(self, out));
  }
}

static wulk_expr_t exclusive_or_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = and_expression(self, out);
  while (true) {
    if (wulk_lexer_peek(self->lexer).type != '^') {
      return r1;
    }
    wulk_lexer_next(self->lexer);
    r1 = wulk_binary(WULK_OP_XOR, r1, and_expression(self, out));
  }
}

static wulk_expr_t inclusive_or_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = exclusive_or_expression(self, out);
  while (true) {
    if (wulk_lexer_peek(self->lexer).type != '|') {
      return r1;
    }
    wulk_lexer_next(self->lexer);
    r1 = wulk_binary(WULK_OP_OR, r1, exclusive_or_expression(self, out));
  }
}

static wulk_expr_t logical_and_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = inclusive_or_expression(self, out);
  while (true) {
    if (wulk_lexer_peek(self->lexer).type != C_TOK_LOGICAL_AND) {
      return r1;
    }
    wulk_lexer_next(self->lexer);
    r1 = wulk_binary(WULK_OP_LAND, r1, inclusive_or_expression(self, out));
  }
}

static wulk_expr_t logical_or_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = logical_and_expression(self, out);
  while (true) {
    if (wulk_lexer_peek(self->lexer).type != C_TOK_LOGICAL_OR) {
      return r1;
    }
    wulk_lexer_next(self->lexer);
    r1 = wulk_binary(WULK_OP_LOR, r1, logical_and_expression(self, out));
  }
}

static wulk_expr_t conditional_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1, r2;

  r1 = logical_or_expression(self, out);
  if (wulk_lexer_peek(self->lexer).type != '?') {
    return r1;
  }
  wulk_lexer_next(self->lexer);
  r2 = expression(self, out);
  wulk_lexer_consume(self->lexer, ':');
  return wulk_ternary(r1, r2, conditional_expression(self, out));
}

static wulk_expr_t assignment_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;
  enum wulk_op_n op;

  r1 = conditional_expression(self, out);
  switch (wulk_lexer_peek(self->lexer).type) {
    case C_TOK_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_ASSIGN;
      break;
    case C_TOK_MUL_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_MUL;
      break;
    case C_TOK_DIV_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_DIV;
      break;
    case C_TOK_MOD_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_MOD;
      break;
    case C_TOK_PLUS_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_ADD;
      break;
    case C_TOK_MINUS_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_SUB;
      break;
    case C_TOK_AND_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_AND;
      break;
    case C_TOK_OR_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_OR;
      break;
    case C_TOK_XOR_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_XOR;
      break;
    case C_TOK_RSHIFT_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_SHR;
      break;
    case C_TOK_LSHIFT_ASSIGN:
      wulk_lexer_next(self->lexer);
      op = WULK_OP_SHL;
      break;
    default:
      return r1;
  }
  if (wulk_is(r1, WULK_EXPR_BINARY)) {
    wulk_parse_err(self->compiler, wulk_lexer_peek(self->lexer).loc,
      "Unexpected left assign operand"
    );
  }
  return wulk_binary(WULK_OP_ASSIGN, r1, op != WULK_OP_ASSIGN
    ? wulk_binary(op, r1, assignment_expression(self, out))
    : assignment_expression(self, out)
  );
}

static wulk_expr_t expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = assignment_expression(self, out);
  while (true) {
    if (wulk_lexer_peek(self->lexer).type != ',') {
      return r1;
    }
    wulk_lexer_next(self->lexer);
    if (!wulk_is(r1, WULK_EXPR_EXPRS)) {
      r1 = wulk_exprs_start(r1);
    }
    adt_vector_push(r1.exprs.vector, assignment_expression(self, out));
  }
}

static wulk_expr_t constant_expression(wulk_parser_t *self, wulk_program_t *out) {
  wulk_expr_t r1;

  r1 = conditional_expression(self, out);
  if (!wulk_expr_is_constant(r1)) {
    wulk_parse_err(self->compiler, wulk_lexer_peek(self->lexer).loc,
      "Constant expression must be computable at compile time"
    );
  }
  return r1;
}

static wulk_type_t declaration_specifiers(wulk_parser_t *self, wulk_program_t *out) {
  wulk_token_t token;
  wulk_type_t type = wulk_type_undefined();

  while (true) {
    switch ((token = wulk_lexer_peek(self->lexer)).type) {
      case C_TOK_TYPEDEF:
        wulk_lexer_next(self->lexer);
        type.specifiers |= WULK_TYPE_SPECIFIER_TYPEDEF;
        break;
      case C_TOK_EXTERN:
        wulk_lexer_next(self->lexer);
        type.specifiers |= WULK_TYPE_SPECIFIER_EXTERN;
        break;
      case C_TOK_STATIC:
        wulk_lexer_next(self->lexer);
        type.specifiers |= WULK_TYPE_SPECIFIER_STATIC;
        break;
      case C_TOK_THREAD_LOCAL:
        wulk_lexer_next(self->lexer);
        type.specifiers |= WULK_TYPE_SPECIFIER_THREAD_LOCAL;
        break;
      case C_TOK_AUTO:
        wulk_lexer_next(self->lexer);
        type.specifiers |= WULK_TYPE_SPECIFIER_AUTO;
        break;
      case C_TOK_REGISTER:
        wulk_lexer_next(self->lexer);
        type.specifiers |= WULK_TYPE_SPECIFIER_REGISTER;
        break;
      case C_TOK_INLINE:
        wulk_lexer_next(self->lexer);
        type.specifiers |= WULK_FUNC_SPECIFIER_INLINE;
        break;
      case C_TOK_NORETURN:
        wulk_lexer_next(self->lexer);
        type.specifiers |= WULK_FUNC_SPECIFIER_NORETURN;
        break;
      case C_TOK_CONST:
        wulk_lexer_next(self->lexer);
        type.qualifiers |= WULK_TYPE_QUALIFIER_CONST;
        break;
      case C_TOK_RESTRICT:
        wulk_lexer_next(self->lexer);
        type.qualifiers |= WULK_TYPE_QUALIFIER_RESTRICT;
        break;
      case C_TOK_VOLATILE:
        wulk_lexer_next(self->lexer);
        type.qualifiers |= WULK_TYPE_QUALIFIER_VOLATILE;
        break;
      case C_TOK_ATOMIC:
        wulk_lexer_next(self->lexer);
        type.qualifiers |= WULK_TYPE_QUALIFIER_ATOMIC;
        break;
      case C_TOK_ALIGNAS:
        wulk_parse_err(self->compiler, token.loc,
          "_Alignas still unsupported"
        );
      default:
        return type;
    }
  }
}

static wulk_type_t type_specifier(wulk_parser_t *self, wulk_program_t *out) {
  wulk_token_t token;
  wulk_type_t type = wulk_type_undefined();
  //enum wulk_type_n type_kind;

  while (true) {
    switch ((token = wulk_lexer_peek(self->lexer)).type) {
      case C_TOK_VOID:
        //type_kind = WULK_TYPE_VOID;
        goto dft_literal;
      case C_TOK_CHAR:
        //type_kind = WULK_TYPE_CHAR;
        goto dft_literal;
      default:
        return type;
      dft_literal:
        if (wulk_defined(type)) {
          wulk_parse_err(self->compiler, token.loc,
            "Two or more data types in declaration specifiers"
          );
        }
        wulk_lexer_next(self->lexer);
        break;
    }
  }
}

static wulk_type_t type_name(wulk_parser_t *self, wulk_program_t *out) {
  return wulk_type_undefined();
}
