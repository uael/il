/*
 * libil - Intermediate Language cross-platform c library
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
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

static il_expr_t primary_expression(il_parser_t *self, il_program_t *out);
static il_expr_t postfix_expression(il_parser_t *self, il_program_t *out);
static il_expr_t unary_expression(il_parser_t *self, il_program_t *out);
static il_expr_t cast_expression(il_parser_t *self, il_program_t *out);
static il_expr_t multiplicative_expression(il_parser_t *self, il_program_t *out);
static il_expr_t additive_expression(il_parser_t *self, il_program_t *out);
static il_expr_t shift_expression(il_parser_t *self, il_program_t *out);
static il_expr_t relational_expression(il_parser_t *self, il_program_t *out);
static il_expr_t equality_expression(il_parser_t *self, il_program_t *out);
static il_expr_t and_expression(il_parser_t *self, il_program_t *out);
static il_expr_t exclusive_or_expression(il_parser_t *self, il_program_t *out);
static il_expr_t inclusive_or_expression(il_parser_t *self, il_program_t *out);
static il_expr_t logical_and_expression(il_parser_t *self, il_program_t *out);
static il_expr_t logical_or_expression(il_parser_t *self, il_program_t *out);
static il_expr_t conditional_expression(il_parser_t *self, il_program_t *out);
static il_expr_t assignment_expression(il_parser_t *self, il_program_t *out);
static il_expr_t expression(il_parser_t *self, il_program_t *out);
static il_expr_t constant_expression(il_parser_t *self, il_program_t *out);
//static il_entity_t declaration(il_parser_t *self, il_program_t *out);
static il_type_t declaration_specifiers(il_parser_t *self, il_program_t *out);
/*static il_entity_t init_declarator_list(il_parser_t *self, il_program_t *out);
static il_entity_t init_declarator(il_parser_t *self, il_program_t *out);*/
static il_type_t type_specifier(il_parser_t *self, il_program_t *out);
/*static il_entity_t struct_or_union_specifier(il_parser_t *self, il_program_t *out);
static il_entity_t struct_or_union(il_parser_t *self, il_program_t *out);
static il_entity_t struct_declaration_list(il_parser_t *self, il_program_t *out);
static il_entity_t struct_declaration(il_parser_t *self, il_program_t *out);
static il_entity_t specifier_qualifier_list(il_parser_t *self, il_program_t *out);
static il_entity_t struct_declarator_list(il_parser_t *self, il_program_t *out);
static il_entity_t struct_declarator(il_parser_t *self, il_program_t *out);
static il_entity_t enum_specifier(il_parser_t *self, il_program_t *out);
static il_entity_t enumerator_list(il_parser_t *self, il_program_t *out);
static il_entity_t enumerator(il_parser_t *self, il_program_t *out);
static il_entity_t atomic_type_specifier(il_parser_t *self, il_program_t *out);
static il_entity_t type_qualifier(il_parser_t *self, il_program_t *out);
static il_entity_t function_specifier(il_parser_t *self, il_program_t *out);
static il_entity_t alignment_specifier(il_parser_t *self, il_program_t *out);
static il_entity_t declarator(il_parser_t *self, il_program_t *out);
static il_entity_t direct_declarator(il_parser_t *self, il_program_t *out);
static il_entity_t pointer(il_parser_t *self, il_program_t *out);
static il_entity_t type_qualifier_list(il_parser_t *self, il_program_t *out);
static il_entity_t parameter_type_list(il_parser_t *self, il_program_t *out);
static il_entity_t parameter_list(il_parser_t *self, il_program_t *out);
static il_entity_t parameter_declaration(il_parser_t *self, il_program_t *out);
static il_entity_t identifier_list(il_parser_t *self, il_program_t *out);*/
static il_type_t type_name(il_parser_t *self, il_program_t *out);
/*static il_entity_t abstract_declarator(il_parser_t *self, il_program_t *out);
static il_entity_t direct_abstract_declarator(il_parser_t *self, il_program_t *out);
static il_entity_t initializer(il_parser_t *self, il_program_t *out);
static il_entity_t initializer_list(il_parser_t *self, il_program_t *out);
static il_entity_t designation(il_parser_t *self, il_program_t *out);
static il_entity_t designator_list(il_parser_t *self, il_program_t *out);
static il_entity_t designator(il_parser_t *self, il_program_t *out);
static il_entity_t static_assert_declaration(il_parser_t *self, il_program_t *out);
static il_entity_t statement(il_parser_t *self, il_program_t *out);
static il_entity_t labeled_statement(il_parser_t *self, il_program_t *out);
static il_entity_t compound_statement(il_parser_t *self, il_program_t *out);
static il_entity_t block_item_list(il_parser_t *self, il_program_t *out);
static il_entity_t block_item(il_parser_t *self, il_program_t *out);
static il_entity_t expression_statement(il_parser_t *self, il_program_t *out);
static il_entity_t selection_statement(il_parser_t *self, il_program_t *out);
static il_entity_t iteration_statement(il_parser_t *self, il_program_t *out);
static il_entity_t jump_statement(il_parser_t *self, il_program_t *out);
static il_entity_t translation_unit(il_parser_t *self, il_program_t *out);
static il_entity_t external_declaration(il_parser_t *self, il_program_t *out);
static il_entity_t function_definition(il_parser_t *self, il_program_t *out);
static il_entity_t declaration_list(il_parser_t *self, il_program_t *out);*/

void c_parser_parse(il_parser_t *self, il_program_t *out) {
  int r;

  il_fe_scope(self);
  self->scope->sym = il_sym_put(self->scope, "foo", &r);
  il_fe_unscope(self);

  il_expr_t expr = expression(self, out);
  il_expr_dtor(&expr);

  while (il_lexer_next(self->lexer).type != 0);
}

enum {
  C_TOKEN_FLAG_NONE = 0,
  C_TOKEN_FLAG_ENUMERATION_CONSTANT = 1 << 0
};

static il_expr_t primary_expression(il_parser_t *self, il_program_t *out) {
  int r;
  il_sym_t *symbol;
  il_token_t token;
  il_expr_t r1, r2;
  il_lloc_t lloc = il_lloc_begin(self->lexer);

  switch ((token = il_lexer_peek(self->lexer)).type) {
    case C_TOK_IDENTIFIER:
      il_lexer_next(self->lexer);
      symbol = il_sym_get(self->scope, token.value, &r);
      if (r) {
        il_parse_err(self->compiler, token.loc, "Undefined symbol '%s'", token.value);
      }
      if (symbol->flags & C_TOKEN_FLAG_ENUMERATION_CONSTANT) {
        return symbol->entity.variable.initializer;
      }
      return il_id(il_lloc_end(lloc), symbol->id, symbol->entity.type);
    case C_TOK_NUMBER:
      il_lexer_next(self->lexer);
      switch (il_const_parse(il_lloc_end(lloc), token.value, token.length, &r2)) {
        case ERANGE:
          il_parse_err(self->compiler, token.loc, "Numeric literal '%s' is out of range", token.value);
        case 1:
          il_parse_err(self->compiler, token.loc, "Invalid numeric literal '%s'", token.value);
        default:
          break;
      }
      return r2;
    case C_TOK_STRING:
      il_lexer_next(self->lexer);
      return il_const_string(il_lloc_end(lloc), token.value);
    case C_TOK_FUNC_NAME:
      if (!(symbol = self->scope->sym) || !il_is(symbol->entity, IL_ENTITY_FUNC)) {
        il_parse_err(self->compiler, token.loc,
          "access of __func__ outside of function"
        );
      }
      il_lexer_next(self->lexer);
      return il_const_string(il_lloc_end(lloc), symbol->entity.name);
    case '(':
      il_lexer_next(self->lexer);
      r1 = expression(self, out);
      il_lexer_consume(self->lexer, ')');
      return il_unary(il_lloc_end(lloc), IL_OP_EN, r1);
    default:
      il_parse_err(self->compiler, token.loc,
        "Unexpected '%s', not a valid primary expression",
        token.value ? token.value : token.name
      );
  }
}

static il_expr_t postfix_expression(il_parser_t *self, il_program_t *out) {
  il_token_t token;
  il_expr_t r1, r2;
  il_lloc_t lloc;

  r1 = primary_expression(self, out);
  while (true) {
    lloc = il_lloc_begin(self->lexer);
    switch ((token = il_lexer_peek(self->lexer)).type) {
      case '[':
        if (!il_type_is_ref(r1.type)) {
          il_parse_err(self->compiler, token.loc,
            "Array access on non array element"
          );
        }
        il_lexer_next(self->lexer);
        r2 = expression(self, out);
        il_lexer_consume(self->lexer, ']');
        r1 = il_array_read(r1, il_lloc_end(lloc), r2);
        break;
      case '(': {
        unsigned i;
        il_entity_t param;
        il_func_t *func;
        il_expr_r args = (il_expr_r) {0};
        il_type_t type = r1.type;

        if (il_is(type, IL_TYPE_POINTER) && il_type_is_func(*type.pointer.of)) {
          type = *type.pointer.of;
        } else if (!il_type_is_func(type)) {
          il_parse_err(self->compiler, token.loc,
            "Expression must have type pointer to function"
          );
        }
        func = &type.compound.entity->function;
        il_lexer_next(self->lexer);
        for (i = 0; i < adt_vector_size(func->params); ++i) {
          if (il_lexer_peek(self->lexer).type == ')') {
            il_parse_err(self->compiler, token.loc,
              "Too few arguments, expected %zu but got %d",
              adt_vector_size(func->params), i
            );
          }
          param = adt_vector_at(func->params, i);
          r2 = assignment_expression(self, out);
          if (!il_type_equals(r2.type, param.type)) {
            r2 = il_cast(r2.lloc, param.type, r2);
          }
          adt_vector_push(args, r2);
          if (i < adt_vector_size(func->params) - 1) {
            il_lexer_consume(self->lexer, ',');
          }
        }
        il_lexer_consume(self->lexer, ')');
        adt_vector_push(args, il_expr_undefined());
        r1 = il_call(r1, il_lloc_end(lloc), il_exprs(args.data).exprs);
        break;
      }
      case '.': {
        il_field_t *field;

        il_lexer_next(self->lexer);
        token = il_lexer_consume(self->lexer, C_TOK_IDENTIFIER);
        field = il_field_lookup(r1.type, token.value);
        if (!field) {
          il_parse_err(self->compiler, token.loc,
            "Invalid access, no member named '%s'",
            token.value
          );
        }
        r1 = il_field_read(r1, il_id(il_lloc_end(lloc), field->name, field->type).id);
        break;
      }
      case C_TOK_PTR_OP: {
        il_type_t type;
        il_field_t *field;

        il_lexer_next(self->lexer);
        if (!il_is(type = r1.type, IL_TYPE_POINTER)) {
          il_parse_err(self->compiler, token.loc,
            "Invalid access on non pointer element '%s'",
            token.value
          );
        }
        token = il_lexer_consume(self->lexer, C_TOK_IDENTIFIER);
        field = il_field_lookup(il_type_deref(type), token.value);
        if (!field) {
          il_parse_err(self->compiler, token.loc,
            "Invalid access, no member named '%s'",
            token.value
          );
        }
        r1 = il_field_read(r1, il_id(il_lloc_end(lloc), field->name, field->type).id);
        break;
      }
      default:
        return r1;
    }
  }
}

static il_expr_t unary_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;
  enum il_op_n op;
  il_lloc_t lloc = il_lloc_begin(self->lexer);

  switch (il_lexer_peek(self->lexer).type) {
    case '&':
      op = IL_OP_AND;
      break;
    case '*':
      op = IL_OP_AND;
      break;
    case '-':
      op = IL_OP_SUB;
      break;
    case '~':
      op = IL_OP_NEG;
      break;
    case '!':
      op = IL_OP_NOT;
      break;
    case C_TOK_INCREMENT:
      op = IL_OP_ADD;
      goto self_assign;
    case C_TOK_DECREMENT:
      op = IL_OP_SUB;
      goto self_assign;
    default:
      return postfix_expression(self, out);
    self_assign:
      il_lexer_next(self->lexer);
      lloc = il_lloc_end(lloc);
      r1 = unary_expression(self, out);
      return il_unary(lloc, IL_OP_EN, il_binary(IL_OP_ASSIGN, r1, il_binary(op, r1, il_const_int(r1.lloc, 1))));
  }
  il_lexer_next(self->lexer);
  return il_unary(il_lloc_end(lloc), op, cast_expression(self, out));
}

static il_expr_t cast_expression(il_parser_t *self, il_program_t *out) {
  il_type_t type;
  il_token_t token;
  il_lloc_t lloc = il_lloc_begin(self->lexer);

  if ((token = il_lexer_peek(self->lexer)).type == '(') {
    il_lexer_next(self->lexer);
    if (!il_defined(type = type_name(self, out))) {
      il_lexer_undo(self->lexer, token);
      return unary_expression(self, out);
    }
    il_lexer_consume(self->lexer, ')');
    return il_cast(il_lloc_end(lloc), type, cast_expression(self, out));
  }

  return unary_expression(self, out);
}

static il_expr_t multiplicative_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = cast_expression(self, out);
  while (true) {
    switch (il_lexer_peek(self->lexer).type) {
      case '*':
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_MUL, r1, cast_expression(self, out));
        break;
      case '/':
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_DIV, r1, cast_expression(self, out));
        break;
      case '%':
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_MOD, r1, cast_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static il_expr_t additive_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = multiplicative_expression(self, out);
  while (true) {
    switch (il_lexer_peek(self->lexer).type) {
      case '+':
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_ADD, r1, multiplicative_expression(self, out));
        break;
      case '-':
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_SUB, r1, multiplicative_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static il_expr_t shift_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = additive_expression(self, out);
  while (true) {
    switch (il_lexer_peek(self->lexer).type) {
      case C_TOK_LEFT_OP:
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_SHL, r1, additive_expression(self, out));
        break;
      case C_TOK_RIGHT_OP:
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_SHR, r1, additive_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static il_expr_t relational_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = shift_expression(self, out);
  while (true) {
    switch (il_lexer_peek(self->lexer).type) {
      case '<':
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_LT, r1, shift_expression(self, out));
        break;
      case C_TOK_LE_OP:
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_LE, r1, shift_expression(self, out));
        break;
      case '>':
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_GT, r1, shift_expression(self, out));
        break;
      case C_TOK_GE_OP:
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_GE, r1, shift_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static il_expr_t equality_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = relational_expression(self, out);
  while (true) {
    switch (il_lexer_peek(self->lexer).type) {
      case C_TOK_EQ_OP:
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_EQ, r1, relational_expression(self, out));
        break;
      case C_TOK_NE_OP:
        il_lexer_next(self->lexer);
        r1 = il_binary(IL_OP_NE, r1, relational_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static il_expr_t and_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = equality_expression(self, out);
  while (true) {
    if (il_lexer_peek(self->lexer).type != '&') {
      return r1;
    }
    il_lexer_next(self->lexer);
    r1 = il_binary(IL_OP_AND, r1, equality_expression(self, out));
  }
}

static il_expr_t exclusive_or_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = and_expression(self, out);
  while (true) {
    if (il_lexer_peek(self->lexer).type != '^') {
      return r1;
    }
    il_lexer_next(self->lexer);
    r1 = il_binary(IL_OP_XOR, r1, and_expression(self, out));
  }
}

static il_expr_t inclusive_or_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = exclusive_or_expression(self, out);
  while (true) {
    if (il_lexer_peek(self->lexer).type != '|') {
      return r1;
    }
    il_lexer_next(self->lexer);
    r1 = il_binary(IL_OP_OR, r1, exclusive_or_expression(self, out));
  }
}

static il_expr_t logical_and_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = inclusive_or_expression(self, out);
  while (true) {
    if (il_lexer_peek(self->lexer).type != C_TOK_LOGICAL_AND) {
      return r1;
    }
    il_lexer_next(self->lexer);
    r1 = il_binary(IL_OP_LAND, r1, inclusive_or_expression(self, out));
  }
}

static il_expr_t logical_or_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = logical_and_expression(self, out);
  while (true) {
    if (il_lexer_peek(self->lexer).type != C_TOK_LOGICAL_OR) {
      return r1;
    }
    il_lexer_next(self->lexer);
    r1 = il_binary(IL_OP_LOR, r1, logical_and_expression(self, out));
  }
}

static il_expr_t conditional_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1, r2;

  r1 = logical_or_expression(self, out);
  if (il_lexer_peek(self->lexer).type != '?') {
    return r1;
  }
  il_lexer_next(self->lexer);
  r2 = expression(self, out);
  il_lexer_consume(self->lexer, ':');
  return il_ternary(r1, r2, conditional_expression(self, out));
}

static il_expr_t assignment_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;
  enum il_op_n op;

  r1 = conditional_expression(self, out);
  switch (il_lexer_peek(self->lexer).type) {
    case C_TOK_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_ASSIGN;
      break;
    case C_TOK_MUL_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_MUL;
      break;
    case C_TOK_DIV_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_DIV;
      break;
    case C_TOK_MOD_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_MOD;
      break;
    case C_TOK_PLUS_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_ADD;
      break;
    case C_TOK_MINUS_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_SUB;
      break;
    case C_TOK_AND_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_AND;
      break;
    case C_TOK_OR_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_OR;
      break;
    case C_TOK_XOR_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_XOR;
      break;
    case C_TOK_RSHIFT_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_SHR;
      break;
    case C_TOK_LSHIFT_ASSIGN:
      il_lexer_next(self->lexer);
      op = IL_OP_SHL;
      break;
    default:
      return r1;
  }
  if (il_is(r1, IL_EXPR_BINARY)) {
    il_parse_err(self->compiler, il_lexer_peek(self->lexer).loc,
      "Unexpected left assign operand"
    );
  }
  return il_binary(IL_OP_ASSIGN, r1, op != IL_OP_ASSIGN
    ? il_binary(op, r1, assignment_expression(self, out))
    : assignment_expression(self, out)
  );
}

static il_expr_t expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = assignment_expression(self, out);
  while (true) {
    if (il_lexer_peek(self->lexer).type != ',') {
      return r1;
    }
    il_lexer_next(self->lexer);
    if (!il_is(r1, IL_EXPR_EXPRS)) {
      r1 = il_exprs_start(r1);
    }
    adt_vector_push(r1.exprs.vector, assignment_expression(self, out));
  }
}

static UNUSED il_expr_t constant_expression(il_parser_t *self, il_program_t *out) {
  il_expr_t r1;

  r1 = conditional_expression(self, out);
  if (!il_expr_is_constant(r1)) {
    il_parse_err(self->compiler, il_lexer_peek(self->lexer).loc,
      "Constant expression must be computable at compile time"
    );
  }
  return r1;
}

static UNUSED il_type_t declaration_specifiers(il_parser_t *self, UNUSED il_program_t *out) {
  il_token_t token;
  il_type_t type = il_type_undefined();

  while (true) {
    switch ((token = il_lexer_peek(self->lexer)).type) {
      case C_TOK_TYPEDEF:
        il_lexer_next(self->lexer);
        type.specifiers |= IL_TYPE_SPECIFIER_TYPEDEF;
        break;
      case C_TOK_EXTERN:
        il_lexer_next(self->lexer);
        type.specifiers |= IL_TYPE_SPECIFIER_EXTERN;
        break;
      case C_TOK_STATIC:
        il_lexer_next(self->lexer);
        type.specifiers |= IL_TYPE_SPECIFIER_STATIC;
        break;
      case C_TOK_THREAD_LOCAL:
        il_lexer_next(self->lexer);
        type.specifiers |= IL_TYPE_SPECIFIER_THREAD_LOCAL;
        break;
      case C_TOK_AUTO:
        il_lexer_next(self->lexer);
        type.specifiers |= IL_TYPE_SPECIFIER_AUTO;
        break;
      case C_TOK_REGISTER:
        il_lexer_next(self->lexer);
        type.specifiers |= IL_TYPE_SPECIFIER_REGISTER;
        break;
      case C_TOK_INLINE:
        il_lexer_next(self->lexer);
        type.specifiers |= IL_FUNC_SPECIFIER_INLINE;
        break;
      case C_TOK_NORETURN:
        il_lexer_next(self->lexer);
        type.specifiers |= IL_FUNC_SPECIFIER_NORETURN;
        break;
      case C_TOK_CONST:
        il_lexer_next(self->lexer);
        type.qualifiers |= IL_TYPE_QUALIFIER_CONST;
        break;
      case C_TOK_RESTRICT:
        il_lexer_next(self->lexer);
        type.qualifiers |= IL_TYPE_QUALIFIER_RESTRICT;
        break;
      case C_TOK_VOLATILE:
        il_lexer_next(self->lexer);
        type.qualifiers |= IL_TYPE_QUALIFIER_VOLATILE;
        break;
      case C_TOK_ATOMIC:
        il_lexer_next(self->lexer);
        type.qualifiers |= IL_TYPE_QUALIFIER_ATOMIC;
        break;
      case C_TOK_ALIGNAS:
        il_parse_err(self->compiler, token.loc,
          "_Alignas still unsupported"
        );
      default:
        return type;
    }
  }
}

static UNUSED il_type_t type_specifier(il_parser_t *self, UNUSED il_program_t *out) {
  il_token_t token;
  il_type_t type = il_type_undefined();
  //enum il_type_n type_kind;

  while (true) {
    switch ((token = il_lexer_peek(self->lexer)).type) {
      case C_TOK_VOID:
        //type_kind = IL_TYPE_VOID;
        goto dft_literal;
      case C_TOK_CHAR:
        //type_kind = IL_TYPE_CHAR;
        goto dft_literal;
      default:
        return type;
      dft_literal:
        if (il_defined(type)) {
          il_parse_err(self->compiler, token.loc,
            "Two or more data types in declaration specifiers"
          );
        }
        il_lexer_next(self->lexer);
        break;
    }
  }
}

static il_type_t type_name(UNUSED il_parser_t *self, UNUSED il_program_t *out) {
  return il_type_undefined();
}
