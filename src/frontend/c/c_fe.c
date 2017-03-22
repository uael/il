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

#include <fval.h>
#include "c_fe.h"

#include "c_lexer.h"
#include "program.h"
#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

FRULE_DECL(primary_expression);
FRULE_DECL(constant);
FRULE_DECL(enumeration_constant);
FRULE_DECL(string);
FRULE_DECL(generic_selection);
FRULE_DECL(generic_assoc_list);
FRULE_DECL(generic_association);
FRULE_DECL(postfix_expression);
FRULE_DECL(argument_expression_list);
FRULE_DECL(unary_expression);
FRULE_DECL(unary_operator);
FRULE_DECL(cast_expression);
FRULE_DECL(multiplicative_expression);
FRULE_DECL(additive_expression);
FRULE_DECL(shift_expression);
FRULE_DECL(relational_expression);
FRULE_DECL(equality_expression);
FRULE_DECL(and_expression);
FRULE_DECL(exclusive_or_expression);
FRULE_DECL(inclusive_or_expression);
FRULE_DECL(logical_and_expression);
FRULE_DECL(logical_or_expression);
FRULE_DECL(conditional_expression);
FRULE_DECL(assignment_expression);
FRULE_DECL(assignment_operator);
FRULE_DECL(expression);
FRULE_DECL(constant_expression);
FRULE_DECL(declaration);
FRULE_DECL(declaration_specifiers);
FRULE_DECL(init_declarator_list);
FRULE_DECL(init_declarator);
FRULE_DECL(storage_class_specifier);
FRULE_DECL(type_specifier);
FRULE_DECL(struct_or_union_specifier);
FRULE_DECL(struct_or_union);
FRULE_DECL(struct_declaration_list);
FRULE_DECL(struct_declaration);
FRULE_DECL(specifier_qualifier_list);
FRULE_DECL(struct_declarator_list);
FRULE_DECL(struct_declarator);
FRULE_DECL(enum_specifier);
FRULE_DECL(enumerator_list);
FRULE_DECL(enumerator);
FRULE_DECL(atomic_type_specifier);
FRULE_DECL(type_qualifier);
FRULE_DECL(function_specifier);
FRULE_DECL(alignment_specifier);
FRULE_DECL(declarator);
FRULE_DECL(direct_declarator);
FRULE_DECL(pointer);
FRULE_DECL(type_qualifier_list);
FRULE_DECL(parameter_type_list);
FRULE_DECL(parameter_list);
FRULE_DECL(parameter_declaration);
FRULE_DECL(identifier_list);
FRULE_DECL(type_name);
FRULE_DECL(abstract_declarator);
FRULE_DECL(direct_abstract_declarator);
FRULE_DECL(initializer);
FRULE_DECL(initializer_list);
FRULE_DECL(designation);
FRULE_DECL(designator_list);
FRULE_DECL(designator);
FRULE_DECL(static_assert_declaration);
FRULE_DECL(statement);
FRULE_DECL(labeled_statement);
FRULE_DECL(compound_statement);
FRULE_DECL(block_item_list);
FRULE_DECL(block_item);
FRULE_DECL(expression_statement);
FRULE_DECL(selection_statement);
FRULE_DECL(iteration_statement);
FRULE_DECL(jump_statement);
FRULE_DECL(translation_unit);
FRULE_DECL(external_declaration);
FRULE_DECL(function_definition);
FRULE_DECL(declaration_list);

enum {
  C_TOKEN_FLAG_NONE = 0,
  C_TOKEN_FLAG_ENUMERATION_CONSTANT = 1 << 0,
};

void c_fe_parse(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_program_init(out);

  jl_fval_t fval;
  postfix_expression(&fval, self, lexer, out);
}

#define SYM sym
#define SYM_GET(id) (SYM = jl_sym_get(fe->scope ? &fe->scope->childs : &out->symtab, id))
#define SYM_PUT(id) (SYM = jl_sym_put(fe->scope ? &fe->scope->childs : &out->symtab, id))

FRULE_DEF(primary_expression) {
  FRULE_BODY_BEGIN;

  FE_MATCHT(1, C_TOK_IDENTIFIER) {
    jl_fval_string(_0, _1.u.token.u.s);
  }
  FRULE_OR
  FE_MATCHR(1, constant, JL_FVAL_STRING | JL_FVAL_INT | JL_FVAL_FLOAT) {
    switch (_1.kind) {
      case JL_FVAL_STRING:
        jl_fval_expr(_0, jl_const_string(_1.u.s));
        break;
      case JL_FVAL_INT:
        jl_fval_expr(_0, jl_const_int(_1.u.d));
        break;
      case JL_FVAL_FLOAT:
        jl_fval_expr(_0, jl_const_float(_1.u.f));
        break;
      default:
        break;
    }
  }
  FRULE_OR
  FE_MATCHR(1, string, JL_FVAL_STRING) {
    jl_fval_string(_0, _1.u.s);
  }
  FRULE_OR
  FE_MATCHT(1, '(') {
    FE_MATCHR(2, primary_expression, JL_FVAL_EXPR) {
      FE_CONSUME(')');
      jl_fval_expr(_0, jl_unary(JL_OP_EN, _2.u.expr));
    }
  }
  FRULE_OR
  FE_MATCHR(1, generic_selection, JL_FVAL_UNDEFINED) {
    /* todo generic_selection */
  }

  FRULE_BODY_END;
}

FRULE_DEF(constant) {
  FRULE_BODY_BEGIN;

  FE_MATCHT(1, C_TOK_NUMBER) {
    jl_fval_string(_0, _1.u.token.u.s);
  }
  FRULE_OR
  FE_MATCHT(1, C_TOK_IDENTIFIER) {
    if (SYM_GET(_1.u.token.u.s) && jl_sym_has_flag(sym, C_TOKEN_FLAG_ENUMERATION_CONSTANT)) {
      jl_fval_string(_0, _1.u.token.u.s);
    }
  }

  FRULE_BODY_END;
}

FRULE_DEF(enumeration_constant) {
  FRULE_BODY_BEGIN;

  FE_MATCHT(1, C_TOK_IDENTIFIER) {
    if (SYM_GET(_1.u.token.u.s)) {
      fprintf(stderr, "duplicate enumeration constant %s", _1.u.token.u.s);
      exit(1);
    }
    SYM_PUT(_1.u.token.u.s)->flags |= C_TOKEN_FLAG_ENUMERATION_CONSTANT;
    jl_fval_string(_0, _1.u.token.u.s);
  }

  FRULE_BODY_END;
}

FRULE_DEF(string) {
  FRULE_BODY_BEGIN;

  FE_MATCHT(1, C_TOK_STRING) {
    if (SYM_GET(_1.u.token.u.s)) {
      fprintf(stderr, "duplicate enumeration constant %s", _1.u.token.u.s);
      exit(1);
    }
    SYM_PUT(_1.u.token.u.s)->flags |= C_TOKEN_FLAG_ENUMERATION_CONSTANT;
    jl_fval_string(_0, _1.u.token.u.s);
  }
  FRULE_OR
  FE_MATCHT(1, C_TOK_FUNC_NAME) {
    if (!fe->scope || !jl_entity_is_func(fe->scope->entity)) {
      fprintf(stderr, "access of __func__ outside of one");
      exit(1);
    }
    jl_fval_string(_0, jl_entity_func(fe->scope->entity)->name);
  }

  FRULE_BODY_END;
}

FRULE_DEF(generic_selection) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(generic_assoc_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(generic_association) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

static _Bool postfix_expression(jl_fval_t*fval,jl_fe_t*fe,jl_lexer_t*lexer,jl_program_t*out) {
  jl_fval_t _1,_2,_3,_4,_5,_6,_7,_8,_9;jl_sym_t*sym;*fval=(jl_fval_t){JL_FVAL_UNDEFINED};

  if(_1=(jl_fval_t){JL_FVAL_UNDEFINED},jl_frule_validate((jl_frule_t){JL_FVAL_EXPR,primary_expression},&_1,fe,lexer,out)) {
    jl_fval_expr(_0, _1.u.expr);
    goto lbl;
  }
  FRULE_OR
  FE_MATCHR(1, postfix_expression, JL_FVAL_EXPR) {
    lbl:;
    jl_token_t t = FE_PEEK();
    char tt = C_TOK_INCREMENT;
    FE_MATCHT(2, '[') {
      FE_MATCHR(3, expression , JL_FVAL_EXPR) {
        jl_fval_expr(_0, jl_array_read(_1.u.expr, _3.u.expr));
      }
      FE_CONSUME(']');
    }
    FRULE_OR
    FE_MATCHT(2, '(') {
      FE_MATCHR(3, argument_expression_list , JL_FVAL_EXPR) {
        jl_fval_expr(_0, jl_call(_1.u.expr, _3.u.expr));
      }
      FE_CONSUME(')');
    }
    FRULE_OR
    FE_MATCHT(2, '.') {
      FE_MATCHT(3, C_TOK_IDENTIFIER) {
        jl_fval_expr(_0, jl_field_read(_1.u.expr, false, jl_id(_3.u.token.u.s, _3.u.token.kind == JL_TOKEN_KEYWORD)));
      }
    }
    FRULE_OR
    FE_MATCHT(2, C_TOK_ARROW) {
      FE_MATCHT(3, C_TOK_IDENTIFIER) {
        jl_fval_expr(_0, jl_field_read(_1.u.expr, true, jl_id(_3.u.token.u.s, _3.u.token.kind == JL_TOKEN_KEYWORD)));
      }
    }
    FRULE_OR
    FE_MATCHT(2, C_TOK_INCREMENT) {
      fprintf(stderr, "unsupported post increment post operator");
      exit(1);
    }
    FRULE_OR
    FE_MATCHT(2, C_TOK_DECREMENT) {
      fprintf(stderr, "unsupported post decrement post operator");
      exit(1);
    }
    FRULE_OR {
      FE_UNDO(_1.begin);
    }
  }
  FRULE_OR
  FE_MATCHT(1, '(') {
    FE_MATCHR(2, type_name , JL_FVAL_TYPE) {
      FE_CONSUME(')');
      FE_MATCHT(3, '{') {
        FE_MATCHR(4, initializer_list , JL_FVAL_TYPE) {
          jl_fval_expr(_0, jl_expr_undefined());
        }
        if (FE_PEEK().type == ',') {
          FE_NEXT();
        }
        FE_CONSUME('}');
      }
    }
  }

  FRULE_BODY_END;
}

FRULE_DEF(argument_expression_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(unary_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(unary_operator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(cast_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(multiplicative_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(additive_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(shift_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(relational_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(equality_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(and_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(exclusive_or_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(inclusive_or_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(logical_and_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(logical_or_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(conditional_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(assignment_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(assignment_operator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(constant_expression) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(declaration) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(declaration_specifiers) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(init_declarator_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(init_declarator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(storage_class_specifier) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(type_specifier) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(struct_or_union_specifier) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(struct_or_union) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(struct_declaration_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(struct_declaration) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(specifier_qualifier_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(struct_declarator_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(struct_declarator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(enum_specifier) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(enumerator_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(enumerator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(atomic_type_specifier) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(type_qualifier) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(function_specifier) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(alignment_specifier) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(declarator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(direct_declarator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(pointer) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(type_qualifier_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(parameter_type_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(parameter_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(parameter_declaration) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(identifier_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(type_name) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(abstract_declarator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(direct_abstract_declarator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(initializer) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(initializer_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(designation) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(designator_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(designator) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(static_assert_declaration) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(statement) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(labeled_statement) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(compound_statement) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(block_item_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(block_item) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(expression_statement) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(selection_statement) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(iteration_statement) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(jump_statement) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(translation_unit) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(external_declaration) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(function_definition) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}

FRULE_DEF(declaration_list) {
  FRULE_BODY_BEGIN;
  FRULE_BODY_END;
}
