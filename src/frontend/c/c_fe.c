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

#include <stdlib.h>

#include "c_fe.h"
#include "c_lexer.h"
#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

#define RULEFN(name) parse_ ## name
#define RULEDEF(name) static bool RULEFN(name)(jl_frontend_t *self, jl_lexer_t *lexer, jl_program_t *out)
#define RULE(name) RULEFN(name)(self, lexer, out)
#define TOK(tok) (jl_lexer_peek(lexer).type == tok)

RULEDEF(primary_expression);
RULEDEF(constant);
RULEDEF(enumeration_constant);
RULEDEF(string);
RULEDEF(generic_selection);
RULEDEF(generic_assoc_list);
RULEDEF(generic_association);
RULEDEF(postfix_expression);
RULEDEF(argument_expression_list);
RULEDEF(unary_expression);
RULEDEF(unary_operator);
RULEDEF(cast_expression);
RULEDEF(multiplicative_expression);
RULEDEF(additive_expression);
RULEDEF(shift_expression);
RULEDEF(relational_expression);
RULEDEF(equality_expression);
RULEDEF(and_expression);
RULEDEF(exclusive_or_expression);
RULEDEF(inclusive_or_expression);
RULEDEF(logical_and_expression);
RULEDEF(logical_or_expression);
RULEDEF(conditional_expression);
RULEDEF(assignment_expression);
RULEDEF(assignment_operator);
RULEDEF(expression);
RULEDEF(constant_expression);
RULEDEF(declaration);
RULEDEF(declaration_specifiers);
RULEDEF(init_declarator_list);
RULEDEF(init_declarator);
RULEDEF(storage_class_specifier);
RULEDEF(type_specifier);
RULEDEF(struct_or_union_specifier);
RULEDEF(struct_or_union);
RULEDEF(struct_declaration_list);
RULEDEF(struct_declaration);
RULEDEF(specifier_qualifier_list);
RULEDEF(struct_declarator_list);
RULEDEF(struct_declarator);
RULEDEF(enum_specifier);
RULEDEF(enumerator_list);
RULEDEF(enumerator);
RULEDEF(atomic_type_specifier);
RULEDEF(type_qualifier);
RULEDEF(function_specifier);
RULEDEF(alignment_specifier);
RULEDEF(declarator);
RULEDEF(direct_declarator);
RULEDEF(pointer);
RULEDEF(type_qualifier_list);
RULEDEF(parameter_type_list);
RULEDEF(parameter_list);
RULEDEF(parameter_declaration);
RULEDEF(identifier_list);
RULEDEF(type_name);
RULEDEF(abstract_declarator);
RULEDEF(direct_abstract_declarator);
RULEDEF(initializer);
RULEDEF(initializer_list);
RULEDEF(designation);
RULEDEF(designator_list);
RULEDEF(designator);
RULEDEF(static_assert_declaration);
RULEDEF(statement);
RULEDEF(labeled_statement);
RULEDEF(compound_statement);
RULEDEF(block_item_list);
RULEDEF(block_item);
RULEDEF(expression_statement);
RULEDEF(selection_statement);
RULEDEF(iteration_statement);
RULEDEF(jump_statement);
RULEDEF(translation_unit);
RULEDEF(external_declaration);
RULEDEF(function_definition);
RULEDEF(declaration_list);

void c_fe_parse(jl_frontend_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_entity_t entity;

  jl_program_init(out);
}

RULEDEF(primary_expression) {
  return false;
}

RULEDEF(constant) {
  return false;
}

RULEDEF(enumeration_constant) {
  return false;
}

RULEDEF(string) {
  return false;
}

RULEDEF(generic_selection) {
  return false;
}

RULEDEF(generic_assoc_list) {
  return false;
}

RULEDEF(generic_association) {
  return false;
}

RULEDEF(postfix_expression) {
  return false;
}

RULEDEF(argument_expression_list) {
  return false;
}

RULEDEF(unary_expression) {
  return false;
}

RULEDEF(unary_operator) {
  return false;
}

RULEDEF(cast_expression) {
  return false;
}

RULEDEF(multiplicative_expression) {
  return false;
}

RULEDEF(additive_expression) {
  return false;
}

RULEDEF(shift_expression) {
  return false;
}

RULEDEF(relational_expression) {
  return false;
}

RULEDEF(equality_expression) {
  return false;
}

RULEDEF(and_expression) {
  return false;
}

RULEDEF(exclusive_or_expression) {
  return false;
}

RULEDEF(inclusive_or_expression) {
  return false;
}

RULEDEF(logical_and_expression) {
  return false;
}

RULEDEF(logical_or_expression) {
  return false;
}

RULEDEF(conditional_expression) {
  return false;
}

RULEDEF(assignment_expression) {
  return false;
}

RULEDEF(assignment_operator) {
  return false;
}

RULEDEF(expression) {
  return false;
}

RULEDEF(constant_expression) {
  return false;
}

RULEDEF(declaration) {
  return false;
}

RULEDEF(declaration_specifiers) {
  return false;
}

RULEDEF(init_declarator_list) {
  return false;
}

RULEDEF(init_declarator) {
  return false;
}

RULEDEF(storage_class_specifier) {
  return false;
}

RULEDEF(type_specifier) {
  return false;
}

RULEDEF(struct_or_union_specifier) {
  return false;
}

RULEDEF(struct_or_union) {
  return false;
}

RULEDEF(struct_declaration_list) {
  return false;
}

RULEDEF(struct_declaration) {
  return false;
}

RULEDEF(specifier_qualifier_list) {
  return false;
}

RULEDEF(struct_declarator_list) {
  return false;
}

RULEDEF(struct_declarator) {
  return false;
}

RULEDEF(enum_specifier) {
  return false;
}

RULEDEF(enumerator_list) {
  return false;
}

RULEDEF(enumerator) {
  return false;
}

RULEDEF(atomic_type_specifier) {
  return false;
}

RULEDEF(type_qualifier) {
  return false;
}

RULEDEF(function_specifier) {
  return false;
}

RULEDEF(alignment_specifier) {
  return false;
}

RULEDEF(declarator) {
  return false;
}

RULEDEF(direct_declarator) {
  return false;
}

RULEDEF(pointer) {
  return false;
}

RULEDEF(type_qualifier_list) {
  return false;
}

RULEDEF(parameter_type_list) {
  return false;
}

RULEDEF(parameter_list) {
  return false;
}

RULEDEF(parameter_declaration) {
  return false;
}

RULEDEF(identifier_list) {
  return false;
}

RULEDEF(type_name) {
  return false;
}

RULEDEF(abstract_declarator) {
  return false;
}

RULEDEF(direct_abstract_declarator) {
  return false;
}

RULEDEF(initializer) {
  return false;
}

RULEDEF(initializer_list) {
  return false;
}

RULEDEF(designation) {
  return false;
}

RULEDEF(designator_list) {
  return false;
}

RULEDEF(designator) {
  return false;
}

RULEDEF(static_assert_declaration) {
  return false;
}

RULEDEF(statement) {
  return false;
}

RULEDEF(labeled_statement) {
  return false;
}

RULEDEF(compound_statement) {
  return false;
}

RULEDEF(block_item_list) {
  return false;
}

RULEDEF(block_item) {
  return false;
}

RULEDEF(expression_statement) {
  return false;
}

RULEDEF(selection_statement) {
  return false;
}

RULEDEF(iteration_statement) {
  return false;
}

RULEDEF(jump_statement) {
  return false;
}

RULEDEF(translation_unit) {
  return false;
}

RULEDEF(external_declaration) {
  return false;
}

RULEDEF(function_definition) {
  return false;
}

RULEDEF(declaration_list) {
  return false;
}
