/*
 * MIT License
 *
 * Copyright (c) 2016-2017 uael <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "c_fe.h"

#include "c_lexer.h"
#include "program.h"
#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

JL_RULEDC(primary_expression);
JL_RULEDC(constant);
JL_RULEDC(enumeration_constant);
JL_RULEDC(string);
JL_RULEDC(generic_selection);
JL_RULEDC(generic_assoc_list);
JL_RULEDC(generic_association);
JL_RULEDC(postfix_expression);
JL_RULEDC(argument_expression_list);
JL_RULEDC(unary_expression);
JL_RULEDC(unary_operator);
JL_RULEDC(cast_expression);
JL_RULEDC(multiplicative_expression);
JL_RULEDC(additive_expression);
JL_RULEDC(shift_expression);
JL_RULEDC(relational_expression);
JL_RULEDC(equality_expression);
JL_RULEDC(and_expression);
JL_RULEDC(exclusive_or_expression);
JL_RULEDC(inclusive_or_expression);
JL_RULEDC(logical_and_expression);
JL_RULEDC(logical_or_expression);
JL_RULEDC(conditional_expression);
JL_RULEDC(assignment_expression);
JL_RULEDC(assignment_operator);
JL_RULEDC(expression);
JL_RULEDC(constant_expression);
JL_RULEDC(declaration);
JL_RULEDC(declaration_specifiers);
JL_RULEDC(init_declarator_list);
JL_RULEDC(init_declarator);
JL_RULEDC(storage_class_specifier);
JL_RULEDC(type_specifier);
JL_RULEDC(struct_or_union_specifier);
JL_RULEDC(struct_or_union);
JL_RULEDC(struct_declaration_list);
JL_RULEDC(struct_declaration);
JL_RULEDC(specifier_qualifier_list);
JL_RULEDC(struct_declarator_list);
JL_RULEDC(struct_declarator);
JL_RULEDC(enum_specifier);
JL_RULEDC(enumerator_list);
JL_RULEDC(enumerator);
JL_RULEDC(atomic_type_specifier);
JL_RULEDC(type_qualifier);
JL_RULEDC(function_specifier);
JL_RULEDC(alignment_specifier);
JL_RULEDC(declarator);
JL_RULEDC(direct_declarator);
JL_RULEDC(pointer);
JL_RULEDC(type_qualifier_list);
JL_RULEDC(parameter_type_list);
JL_RULEDC(parameter_list);
JL_RULEDC(parameter_declaration);
JL_RULEDC(identifier_list);
JL_RULEDC(type_name);
JL_RULEDC(abstract_declarator);
JL_RULEDC(direct_abstract_declarator);
JL_RULEDC(initializer);
JL_RULEDC(initializer_list);
JL_RULEDC(designation);
JL_RULEDC(designator_list);
JL_RULEDC(designator);
JL_RULEDC(static_assert_declaration);
JL_RULEDC(statement);
JL_RULEDC(labeled_statement);
JL_RULEDC(compound_statement);
JL_RULEDC(block_item_list);
JL_RULEDC(block_item);
JL_RULEDC(expression_statement);
JL_RULEDC(selection_statement);
JL_RULEDC(iteration_statement);
JL_RULEDC(jump_statement);
JL_RULEDC(translation_unit);
JL_RULEDC(external_declaration);
JL_RULEDC(function_definition);
JL_RULEDC(declaration_list);

enum {
  C_TOKEN_FLAG_NONE = 0,
  C_TOKEN_FLAG_ENUMERATION_CONSTANT = 1 << 0,
};

void c_fe_parse(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_program_init(out);

  jl_fval_t fval;
  primary_expression(&fval, self, lexer, out);
}

#define SYM_GET(id) (sym = jl_sym_get(fe->scope ? &fe->scope->childs : &out->symtab, id))

JL_RULEDF(primary_expression) {
  Jl_RULEBG;

  JL_MATCHT(1, C_TOK_IDENTIFIER) {
    $$ = jl_fval_string($1.token.s);
  }
  else
  JL_MATCHR(1, constant, JL_FVAL_STRING | JL_FVAL_INT | JL_FVAL_FLOAT) {
    switch ($1.kind) {
      case JL_FVAL_STRING:
        $$ = jl_fval_expr(jl_const_string($1.s));
      case JL_FVAL_INT:
        $$ = jl_fval_expr(jl_const_int($1.d));
      case JL_FVAL_FLOAT:
        $$ = jl_fval_expr(jl_const_float($1.f));
      default:
        break;
    }
  }
  else
  JL_MATCHR(1, expression, JL_FVAL_STRING) {
    $$ = jl_fval_string($1.s);
  }
  else
  JL_MATCHT(1, '(') JL_MATCHR(2, expression, JL_FVAL_EXPR) {

    $$ = jl_fval_expr(jl_unary(JL_OP_EN, $2.expr));
  }
  /*else todo
  JL_MATCHR(1, generic_selection, JL_FVAL_EXPR) {
    $$ = jl_fval_string($1.s);
  }*/
  Jl_RULEED;
}

JL_RULEDF(constant) {
  Jl_RULEBG;

  JL_MATCHT(1, C_TOK_NUMBER) {
    $$ = jl_fval_string($1.token.s);
  }
  else
  JL_MATCHT(1, C_TOK_IDENTIFIER) {
    if (SYM_GET($1.token.s) && jl_sym_has_flag(sym, C_TOKEN_FLAG_ENUMERATION_CONSTANT)) {
      $$ = jl_fval_string($1.token.s);
    }
  }
  Jl_RULEED;
}

JL_RULEDF(enumeration_constant) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(string) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(generic_selection) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(generic_assoc_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(generic_association) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(postfix_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(argument_expression_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(unary_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(unary_operator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(cast_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(multiplicative_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(additive_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(shift_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(relational_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(equality_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(and_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(exclusive_or_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(inclusive_or_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(logical_and_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(logical_or_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(conditional_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(assignment_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(assignment_operator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(constant_expression) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(declaration) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(declaration_specifiers) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(init_declarator_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(init_declarator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(storage_class_specifier) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(type_specifier) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(struct_or_union_specifier) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(struct_or_union) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(struct_declaration_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(struct_declaration) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(specifier_qualifier_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(struct_declarator_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(struct_declarator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(enum_specifier) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(enumerator_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(enumerator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(atomic_type_specifier) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(type_qualifier) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(function_specifier) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(alignment_specifier) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(declarator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(direct_declarator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(pointer) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(type_qualifier_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(parameter_type_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(parameter_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(parameter_declaration) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(identifier_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(type_name) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(abstract_declarator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(direct_abstract_declarator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(initializer) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(initializer_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(designation) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(designator_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(designator) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(static_assert_declaration) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(statement) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(labeled_statement) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(compound_statement) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(block_item_list) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(block_item) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(expression_statement) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(selection_statement) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(iteration_statement) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(jump_statement) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(translation_unit) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(external_declaration) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(function_definition) {
  Jl_RULEBG;
  Jl_RULEED;
}

JL_RULEDF(declaration_list) {
  Jl_RULEBG;
  Jl_RULEED;
}
