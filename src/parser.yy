/*
 * Copyright (c) 2016, Abel Lucas <ylilt71@gmail.com>
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

%{
#include "ast.h"

#include <cstdio>
#include <cstdlib>
%}

%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {ddc}
%define parser_class_name {parser}

%parse-param { class driver &driver }

%locations

%union {
  ddc::identifier_t *_identifier;
  ddc::type_t *_type;
  ddc::signature_t *_signature;
  ddc::declaration_t *_declaration;
  std::string *_string;
}

%token END 0 "end of file"
%token <_string> TOKEN_IDENTIFIER TOKEN_TYPE
%token <_token> TOKEN_COLON

%type <_identifier> identifier
%type <_type> type
%type <_signature> signature
%type <_declaration> declaration

%start declaration

%{
#include "driver.h"
#include "scanner.h"

#undef yylex
#define yylex driver.lexer->lex
%}

%%

declaration :
        identifier TOKEN_COLON signature {
            $$ = new ddc::declaration_t(*$1, *$3); delete($1); delete($3);
            driver.context.declarations.push_back($$);
        }
        ;

signature :
        type { $$ = new ddc::signature_t(*$1); delete($1); }
        ;

type :
        identifier { $$ = new ddc::type_t(*$1); delete($1); }
        ;

identifier :
        TOKEN_IDENTIFIER { $$ = new ddc::identifier_t(*$1); delete($1); }
        ;

%%

void ddc::parser::error(const ddc::parser::location_type& l, const std::string& m) {
    driver.error(l, m);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */