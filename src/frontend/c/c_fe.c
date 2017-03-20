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
#define RULEDC(name) static bool RULEFN(name)(jl_frontend_t *self, jl_lexer_t *lexer, jl_program_t *out)
#define RULEDF(name) RULEDC(name)

#define PEEK jl_lexer_peek(lexer)
#define PEEKN(n) jl_lexer_peekn(lexer, n)
#define NEXT jl_lexer_next(lexer)
#define CONSUME(t) jl_lexer_consume(lexer, t)
#define RULE(name) RULEFN(name)(self, lexer, out)
#define TOKEN(name) (jl_lexer_peek(lexer).type == name)

RULEDC(primary_expression);
RULEDC(constant);
RULEDC(enumeration_constant);
RULEDC(string);
RULEDC(generic_selection);
RULEDC(generic_assoc_list);
RULEDC(generic_association);
RULEDC(postfix_expression);
RULEDC(argument_expression_list);
RULEDC(unary_expression);
RULEDC(unary_operator);
RULEDC(cast_expression);
RULEDC(multiplicative_expression);
RULEDC(additive_expression);
RULEDC(shift_expression);
RULEDC(relational_expression);
RULEDC(equality_expression);
RULEDC(and_expression);
RULEDC(exclusive_or_expression);
RULEDC(inclusive_or_expression);
RULEDC(logical_and_expression);
RULEDC(logical_or_expression);
RULEDC(conditional_expression);
RULEDC(assignment_expression);
RULEDC(assignment_operator);
RULEDC(expression);
RULEDC(constant_expression);
RULEDC(declaration);
RULEDC(declaration_specifiers);
RULEDC(init_declarator_list);
RULEDC(init_declarator);
RULEDC(storage_class_specifier);
RULEDC(type_specifier);
RULEDC(struct_or_union_specifier);
RULEDC(struct_or_union);
RULEDC(struct_declaration_list);
RULEDC(struct_declaration);
RULEDC(specifier_qualifier_list);
RULEDC(struct_declarator_list);
RULEDC(struct_declarator);
RULEDC(enum_specifier);
RULEDC(enumerator_list);
RULEDC(enumerator);
RULEDC(atomic_type_specifier);
RULEDC(type_qualifier);
RULEDC(function_specifier);
RULEDC(alignment_specifier);
RULEDC(declarator);
RULEDC(direct_declarator);
RULEDC(pointer);
RULEDC(type_qualifier_list);
RULEDC(parameter_type_list);
RULEDC(parameter_list);
RULEDC(parameter_declaration);
RULEDC(identifier_list);
RULEDC(type_name);
RULEDC(abstract_declarator);
RULEDC(direct_abstract_declarator);
RULEDC(initializer);
RULEDC(initializer_list);
RULEDC(designation);
RULEDC(designator_list);
RULEDC(designator);
RULEDC(static_assert_declaration);
RULEDC(statement);
RULEDC(labeled_statement);
RULEDC(compound_statement);
RULEDC(block_item_list);
RULEDC(block_item);
RULEDC(expression_statement);
RULEDC(selection_statement);
RULEDC(iteration_statement);
RULEDC(jump_statement);
RULEDC(translation_unit);
RULEDC(external_declaration);
RULEDC(function_definition);
RULEDC(declaration_list);

void c_fe_parse(jl_frontend_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_program_init(out);

  RULE(primary_expression);
}

typedef enum c_token_flag_n c_token_flag_n;
enum c_token_flag_n {
  C_TOKEN_FLAG_NONE = 0,
  C_TOKEN_FLAG_ENUMERATION_CONSTANT = 1 << 0,
};

static void scope(jl_frontend_t *self, jl_program_t *out, const char *id) {
  jl_symtab_t *symtab;
  int it;

  symtab = self->scope ? &self->scope->childs : &out->symtab;
  it = kh_get(jl_symtab, symtab, id);
  if (it == kh_end(symtab)) {
    puts("cannot scope on unrecognized entity");
    exit(1);
  }
  kh_value(symtab, it).parent = self->scope;
  self->scope = &kh_value(symtab, it);
}

static void unscope(jl_frontend_t *self) {
  self->scope = self->scope->parent;
}

jl_sym_t *sym_push(jl_frontend_t *self, jl_program_t *out, const char *id) {
  jl_symtab_t *symtab;
  jl_sym_t *sym;
  int it, r;

  symtab = self->scope ? &self->scope->childs : &out->symtab;
  it = kh_put(jl_symtab, symtab, id, &r);
  if (r == 0) {
    fprintf(stderr, "%s already defined", id);
    exit(1);
  }
  sym = &kh_value(symtab, it);
  sym->id = id;
  return sym;
}

jl_sym_t *sym_get(jl_frontend_t *self, jl_program_t *out, const char *id) {
  jl_symtab_t *symtab;
  int it;

  symtab = self->scope ? &self->scope->childs : &out->symtab;
  it = kh_get(jl_symtab, symtab, id);
  if (it == kh_end(symtab)) {
    return NULL;
  }
  return &kh_value(symtab, it);
}

RULEDF(primary_expression) {
  jl_token_t token;
  jl_sym_t *sym;

  if (TOKEN(C_TOK_IDENTIFIER)) {
    token = NEXT;
    if (!(sym = sym_get(self, out, token.s))) {
      fprintf(stderr, "%s undefined symbol", token.s);
      exit(1);
    }
    self->entity = sym->entity;
  }
  if (RULE(constant)) {
    return true;
  }
  if (TOKEN('(')) {
    NEXT;
    if (!RULE(expression)) {
      return false;
    }
    CONSUME(')');
    return true;
  }
  return false;
}

RULEDF(constant) {
  jl_token_t token;
  jl_sym_t *sym;

  if (TOKEN(C_TOK_NUMBER)) {
    token = NEXT;
    self->entity = jl_var_string(NULL, token.s);
    return true;
  }
  if (TOKEN(C_TOK_IDENTIFIER)) {
    token = NEXT;
    if (!(sym = sym_get(self, out, token.s))) {
      fprintf(stderr, "%s undefined symbol", token.s);
      exit(1);
    }
    if (!(sym->flags & C_TOKEN_FLAG_ENUMERATION_CONSTANT)) {
      return false;
    }
    self->entity = sym->entity;
    return true;
  }
  return false;
}

RULEDF(enumeration_constant) {
  jl_token_t token;
  jl_sym_t *sym;

  if (TOKEN(C_TOK_IDENTIFIER)) {
    token = NEXT;
    sym = sym_push(self, out, token.s);
    sym->flags |= C_TOKEN_FLAG_ENUMERATION_CONSTANT;
    return true;
  }
  return false;
}

RULEDF(string) {
  jl_token_t token;

  if (TOKEN(C_TOK_STRING)) {
    token = NEXT;
    self->entity = jl_var_string(NULL, token.s);
    return true;
  }
  if (TOKEN(C_TOK_FUNC_NAME)) {
    NEXT;
    if (!self->scope || !jl_entity_is_func(self->scope->entity)) {
      fprintf(stderr, "__func__ only available in function scope");
      exit(1);
    }
    self->entity = jl_var_string(NULL, jl_entity_func(self->scope->entity)->name);
    return true;
  }
  return false;
}

RULEDF(generic_selection) {
  if (TOKEN(C_TOK_GENERIC)) {
    CONSUME('(');
    if (!RULE(assignment_expression)) {
      return false;
    }
    CONSUME(',');
    if (!RULE(generic_assoc_list)) {
      return false;
    }
    return true;
  }
  return false;
}

RULEDF(generic_assoc_list) {
  bool result = false;

  next:
  if (RULE(generic_association)) {
    result = true;
    if (TOKEN(',')) {
      NEXT;
      goto next;
    }
  }

  return result;
}

RULEDF(generic_association) {
  if (TOKEN(C_TOK_DEFAULT)) {
    NEXT;
    CONSUME(':');
    return RULE(assignment_expression);
  }
  if (RULE(type_name)) {
    CONSUME(':');
    return RULE(assignment_expression);
  }
  return false;
}

RULEDF(postfix_expression) {
  if (RULE(primary_expression)) {
    return true;
  }
  return false;
}

RULEDF(argument_expression_list) {
  return false;
}

RULEDF(unary_expression) {
  return false;
}

RULEDF(unary_operator) {
  return false;
}

RULEDF(cast_expression) {
  return false;
}

RULEDF(multiplicative_expression) {
  return false;
}

RULEDF(additive_expression) {
  return false;
}

RULEDF(shift_expression) {
  return false;
}

RULEDF(relational_expression) {
  return false;
}

RULEDF(equality_expression) {
  return false;
}

RULEDF(and_expression) {
  return false;
}

RULEDF(exclusive_or_expression) {
  return false;
}

RULEDF(inclusive_or_expression) {
  return false;
}

RULEDF(logical_and_expression) {
  return false;
}

RULEDF(logical_or_expression) {
  return false;
}

RULEDF(conditional_expression) {
  return false;
}

RULEDF(assignment_expression) {
  return false;
}

RULEDF(assignment_operator) {
  return false;
}

RULEDF(expression) {
  return false;
}

RULEDF(constant_expression) {
  return false;
}

RULEDF(declaration) {
  return false;
}

RULEDF(declaration_specifiers) {
  return false;
}

RULEDF(init_declarator_list) {
  return false;
}

RULEDF(init_declarator) {
  return false;
}

RULEDF(storage_class_specifier) {
  return false;
}

RULEDF(type_specifier) {
  return false;
}

RULEDF(struct_or_union_specifier) {
  return false;
}

RULEDF(struct_or_union) {
  return false;
}

RULEDF(struct_declaration_list) {
  return false;
}

RULEDF(struct_declaration) {
  return false;
}

RULEDF(specifier_qualifier_list) {
  return false;
}

RULEDF(struct_declarator_list) {
  return false;
}

RULEDF(struct_declarator) {
  return false;
}

RULEDF(enum_specifier) {
  return false;
}

RULEDF(enumerator_list) {
  return false;
}

RULEDF(enumerator) {
  return false;
}

RULEDF(atomic_type_specifier) {
  return false;
}

RULEDF(type_qualifier) {
  return false;
}

RULEDF(function_specifier) {
  return false;
}

RULEDF(alignment_specifier) {
  return false;
}

RULEDF(declarator) {
  return false;
}

RULEDF(direct_declarator) {
  return false;
}

RULEDF(pointer) {
  return false;
}

RULEDF(type_qualifier_list) {
  return false;
}

RULEDF(parameter_type_list) {
  return false;
}

RULEDF(parameter_list) {
  return false;
}

RULEDF(parameter_declaration) {
  return false;
}

RULEDF(identifier_list) {
  return false;
}

RULEDF(type_name) {
  return false;
}

RULEDF(abstract_declarator) {
  return false;
}

RULEDF(direct_abstract_declarator) {
  return false;
}

RULEDF(initializer) {
  return false;
}

RULEDF(initializer_list) {
  return false;
}

RULEDF(designation) {
  return false;
}

RULEDF(designator_list) {
  return false;
}

RULEDF(designator) {
  return false;
}

RULEDF(static_assert_declaration) {
  return false;
}

RULEDF(statement) {
  return false;
}

RULEDF(labeled_statement) {
  return false;
}

RULEDF(compound_statement) {
  return false;
}

RULEDF(block_item_list) {
  return false;
}

RULEDF(block_item) {
  return false;
}

RULEDF(expression_statement) {
  return false;
}

RULEDF(selection_statement) {
  return false;
}

RULEDF(iteration_statement) {
  return false;
}

RULEDF(jump_statement) {
  return false;
}

RULEDF(translation_unit) {
  return false;
}

RULEDF(external_declaration) {
  return false;
}

RULEDF(function_definition) {
  return false;
}

RULEDF(declaration_list) {
  return false;
}
