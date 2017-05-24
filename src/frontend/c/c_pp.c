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

#include "c_pp.h"

void c_macro_init(c_macro_t *self) {
  *self = (c_macro_t) {C_MACRO_UNDEFINED};
}

void c_macro_dtor(c_macro_t *self) {
  adt_vector_dtor(self->params);
  adt_vector_dtor(self->replacement);
}

void c_macro_expand(c_macro_t *self, c_pp_t *pp, wulk_lexer_t *into) {
  wulk_token_t t;
  unsigned it;
  c_macro_t macro;

  adt_vector_foreach(self->replacement, t) {
    if (t.kind == WULK_TOKEN_IDENTIFIER) {
      it = kh_get(c_macro_ht, &pp->macros, t.value);
      if (it != kh_end(&pp->macros)) {
        macro = kh_value(&pp->macros, it);
        if (strcmp(macro.name, self->name) != 0) {
          c_macro_expand(&macro, pp, into);
          continue;
        }
      }
    }
    wulk_lexer_push(into, t);
  }
}


void c_pp_init(c_pp_t *self) {
  *self = (c_pp_t) {{0}};
}

void c_pp_dtor(c_pp_t *self) {
  c_macro_t macro;

  kh_foreach_value(&self->macros, macro, {
    c_macro_dtor(&macro);
  });
  c_macro_ht_dtor(&self->macros);
  wulk_lexer_dtor(&self->lexer, true);
}

void c_pp_parse_define(c_pp_t *self, wulk_lexer_t *lexer) {
  wulk_token_t t, pt;
  c_macro_t macro;
  int out;
  unsigned it;
  wulk_lexer_consume_id(lexer, "define");
  t = wulk_lexer_consume(lexer, C_TOK_IDENTIFIER);
  c_macro_init(&macro);
  macro.name = t.value;

  if (wulk_lexer_peek(lexer).type == '(') {
    macro.kind = C_MACRO_FUNC;
    lbl_parse_arg:
    adt_vector_push(macro.params, wulk_lexer_consume(lexer, C_TOK_IDENTIFIER));
    if (wulk_lexer_peek(lexer).type == ',') {
      wulk_lexer_consume(lexer, ',');
      goto lbl_parse_arg;
    }
    wulk_lexer_consume(lexer, ')');
  } else {
    macro.kind = C_MACRO_OBJECT;
  }

  while ((t = wulk_lexer_next(lexer)).type != 0) {
    if (t.type == '\n') {
      if (pt.type != '\\') break;
      else {
        wulk_token_dtor(&pt);
        wulk_token_dtor(&t);
        continue;
      }
    } else if (t.type != '\\') {
      adt_vector_push(macro.replacement, t);
    }
    pt = t;
  }
  it = kh_put(c_macro_ht, &self->macros, macro.name, &out);
  if (out == 0) {
    puts("macro redefined");
  }
  kh_value(&self->macros, it) = macro;
  wulk_token_dtor(&t);
}

void c_pp_parse_undef(c_pp_t *self, wulk_lexer_t *lexer) {
  wulk_token_t t;
  unsigned it;

  wulk_lexer_consume_id(lexer, "undef");
  t = wulk_lexer_consume(lexer, C_TOK_IDENTIFIER);
  it = kh_get(c_macro_ht, &self->macros, t.value);
  if (it != kh_end(&self->macros)) {
    kh_del(c_macro_ht, &self->macros, it);
  }
  wulk_token_dtor(&t);
}


bool c_pp_on_push_callback(wulk_lexer_event_t *self, void *arg) {
  wulk_token_t *token, t;
  c_pp_t *pp;
  unsigned it;

  token = (wulk_token_t *) arg;
  pp = (c_pp_t *) self->data;

  if (token->type == '#') {
    wulk_lexer_fork(&pp->lexer, self->lexer);
    if ((t = wulk_lexer_peek(&pp->lexer)).kind == WULK_TOKEN_IDENTIFIER) {
      if (strcmp("define", t.value) == 0) {
        c_pp_parse_define(pp, &pp->lexer);
        wulk_lexer_join(&pp->lexer);
        return false;
      } else if (strcmp("undef", t.value) == 0) {
        c_pp_parse_undef(pp, &pp->lexer);
        wulk_lexer_join(&pp->lexer);

        return false;
      }
    }
  } else if (token->kind == WULK_TOKEN_IDENTIFIER) {
    it = kh_get(c_macro_ht, &pp->macros, token->value);
    if (it != kh_end(&pp->macros)) {
      c_macro_expand(&kh_value(&pp->macros, it), pp, self->lexer);
      return false;
    }
  }

  return true;
}

void c_pp_on_push_dtor(wulk_lexer_event_t *self) {
  if (self->data) {
    c_pp_dtor(self->data);
    xfree(self->data);
    self->data = NULL;
  }
}


KHASH_MAP_IMPL_STR(c_macro_ht, c_macro_t)
