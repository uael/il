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
#include <entities.h>
#include <exprs.h>

#include "c_fe.h"
#include "c_lexer.h"

static jl_entity_t parse_entity(jl_frontend_t *self, jl_lexer_t *lexer);
static jl_entity_t parse_enum(jl_frontend_t *self, jl_lexer_t *lexer);

void c_fe_parse(jl_frontend_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_entity_t entity;

  jl_program_init(out);
  while (jl_lexer_peek(lexer).type != 0) {
    entity = parse_entity(self, lexer);
    if (jl_entity_is_defined(entity)) {
      jl_vector_push(out->entities, entity);
    }
  }
}

static jl_entity_t parse_entity(jl_frontend_t *self, jl_lexer_t *lexer) {
  jl_entity_t undefined = {JL_ENTITY_UNDEFINED};

  switch (jl_lexer_peek(lexer).type) {
    case C_TOK_ENUM:
      return parse_enum(self, lexer);
    default:
      jl_lexer_next(lexer);
      return undefined;
  }
}

static jl_entity_t parse_enum(jl_frontend_t *self, jl_lexer_t *lexer) {
  jl_entity_t entity, var;
  jl_token_t token;
  const char *name, *var_name;
  jl_var_r vars = (jl_var_r) {0};
  jl_type_t type;
  jl_expr_t expr;

  jl_literal_init_i(&type, 0, JL_QUALIFIER_NONE);

  jl_lexer_consume(lexer, C_TOK_ENUM);
  token = jl_lexer_consume(lexer, C_TOK_IDENTIFIER);
  name = token.s;

  jl_lexer_consume(lexer, C_TOK_OPEN_CURLY);
  lbl_parse_var:
  token = jl_lexer_consume(lexer, C_TOK_IDENTIFIER);
  var_name = token.s;
  if (jl_lexer_peek(lexer).type == '=') {
    jl_lexer_next(lexer);
    token = jl_lexer_consume(lexer, C_TOK_NUMBER);
    jl_expr_const_init(&expr, type);
  } else {
    jl_expr_undefined(&expr);
  }
  jl_var_init(&var, var_name, type, expr, 0);
  jl_vector_push(vars, var);
  if (jl_lexer_peek(lexer).type == ',') {
    jl_lexer_next(lexer);
    goto lbl_parse_var;
  }

  jl_lexer_consume(lexer, C_TOK_CLOSE_CURLY);
  jl_enum_init(&entity, name, type, vars, 0);

  return entity;
}
