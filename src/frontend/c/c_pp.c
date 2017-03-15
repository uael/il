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

#include "c_pp.h"
#include "c_lexer.h"

void c_pp_init(c_pp_t *self, jl_lexer_t *lexer) {}

void c_pp_dtor(c_pp_t *self) {
  c_macro_ht_dtor(&self->macros);
  self->pp_lexer.buffer = NULL;
  jl_lexer_dtor(&self->pp_lexer);
}

void c_pp_parse_define(c_pp_t *self, c_macro_t *macro);


bool c_pp_op_push_callback(jl_lexer_event_t *self, void *arg) {
  jl_token_t *token, tok;
  c_pp_t *pp;
  c_macro_t macro = {C_MACRO_UNDEFINED};
  unsigned it;
  int out;

  token = (jl_token_t *) arg;
  pp = (c_pp_t *) self->data;

  if (token->type == '#') {
    jl_lexer_fork(&pp->pp_lexer, self->lexer);
    jl_lexer_skip(&pp->pp_lexer, token->length);
    if ((tok = jl_lexer_peek(&pp->pp_lexer)).kind == Jl_TOKEN_IDENTIFIER) {
      if (strcmp("define", tok.s) == 0) {
        c_pp_parse_define(pp, &macro);
        it = kh_put(c_macro_ht, &pp->macros, macro.name, &out);
        if (out == 0) {
          puts("macro redefined");
        }
        kh_value(&pp->macros, it) = macro;
        jl_lexer_join(self->lexer, &pp->pp_lexer);
        jl_token_dtor(&tok);

        return false;
      } else if (strcmp("undef", tok.s) == 0) {
        tok = jl_lexer_consume(&pp->pp_lexer, C_TOK_IDENTIFIER);
        it = kh_get(c_macro_ht, &pp->macros, tok.s);
        if (it != kh_end(&pp->macros)) {
          kh_del(c_macro_ht, &pp->macros, it);
        }
        jl_lexer_join(self->lexer, &pp->pp_lexer);
        jl_token_dtor(&tok);

        return false;
      }
    }
  } else if (token->kind == Jl_TOKEN_IDENTIFIER) {
    it = kh_get(c_macro_ht, &pp->macros, token->s);
    if (it != kh_end(&pp->macros)) {
      macro = kh_value(&pp->macros, it);
      jl_vector_foreach(macro.replacement, tok) {
        jl_deque_push(self->lexer->queue, tok);
      }
      return false;
    }
  }

  return true;
}

void c_pp_op_push_dtor(jl_lexer_event_t *self) {
  if (self->data) {
    c_pp_dtor(self->data);
    free(self->data);
    self->data = NULL;
  }
}

void c_pp_parse_define(c_pp_t *self, c_macro_t *macro) {
  jl_token_t token, prev_token;

  token = jl_lexer_consume(&self->pp_lexer, C_TOK_IDENTIFIER);
  macro->name = token.s;

  if (jl_lexer_peek(&self->pp_lexer).type == '(') {
    macro->kind = C_MACRO_FUNC;
    lbl_parse_arg:
    jl_vector_push(macro->params, jl_lexer_consume(&self->pp_lexer, C_TOK_IDENTIFIER));
    if (jl_lexer_peek(&self->pp_lexer).type == ',') {
      jl_lexer_consume(&self->pp_lexer, ',');
      goto lbl_parse_arg;
    }
    jl_lexer_consume(&self->pp_lexer, ')');
  } else {
    macro->kind = C_MACRO_OBJECT;
  }

  while ((token = jl_lexer_next(&self->pp_lexer)).type != 0) {
    if (token.type == '\n') {
      if (prev_token.type != '\\') break;
      else {
        jl_token_dtor(&token);
        continue;
      }
    }
    jl_vector_push(macro->replacement, token);
    prev_token = token;
  }
}


KHASH_MAP_IMPL_STR(c_macro_ht, c_macro_t);
