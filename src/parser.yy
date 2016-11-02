/*
 *  MIT License
 *
 * Copyright (c) 2016 .DRY (Don't Repeat Yourself)
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
%require "3.0"
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
  ddc::function_decl_t *_function_decl;
  ddc::function_t *_function;
  ddc::functions_t *_functions;
  ddc::arguments_t *_arguments;
  ddc::class_t *_class;
  std::string *_string;
}

%token END 0 "end of file"
%token EOL "end of line"
%token <_string> IDENTIFIER KCLASS
%token COLON SEMICOLON COMMA LPAR RPAR LBRA RBRA INLINE

%type <_identifier> identifier
%type <_type> type
%type <_signature> signature
%type <_declaration> declaration
%type <_function_decl> function_decl
%type <_function> function
%type <_arguments> arguments arguments_list
%type <_functions> functions
%type <_class> class
%type <_string> closure

%{
#include "driver.h"
#include "scanner.h"

#undef yylex
#define yylex driver.lexer->lex

#define MAKE(v, type, ...) v = new ddc::type##_t(__VA_ARGS__)
#define MAKE_FUNCTION_DECL(v, ...) MAKE(v, function_decl, __VA_ARGS__);
#define PUSH(v, arg) v->push_back(arg)
%}

%start program

%%

program
    : stmts
    ;

stmts
    : stmt
    | stmts stmt
    ;

stmt
    : function_decl SEMICOLON
        { driver.context.declarations.push_back($1); }
    | class
        { driver.context.classes.push_back($1); }
    ;

class
    : KCLASS identifier COLON signature LBRA functions RBRA
        { MAKE($$, class, *$2, *$4, *$6); }
    | KCLASS identifier LBRA functions RBRA
        { MAKE($$, class, *$2, *$4); }
    ;

functions
    : /* empty */
        { MAKE($$, functions); }
    | function
        { MAKE($$, functions); PUSH($$, $1); }
    | functions function
        { PUSH($1, $2); $$ = $1; }
    ;

function
    : function_decl closure
        { MAKE($$, function, *$1, *$2); }
    ;

closure
    : INLINE IDENTIFIER SEMICOLON
        { $$ = $2; }
    | LBRA IDENTIFIER RBRA
        { $$ = $2; }
    ;

function_decl
    : identifier arguments
        { MAKE($$, function_decl, *$1, *$2); }
    | identifier arguments COLON signature
        { MAKE($$, function_decl, *$1, *$4, *$2); }
    ;

arguments
    : LPAR RPAR
        { MAKE($$, arguments); }
    | LPAR arguments_list RPAR
        { $$ = $2; }

arguments_list
    : declaration
        { MAKE($$, arguments); PUSH($$, $1); }
    | arguments_list COMMA declaration
        { PUSH($1, $3); $$ = $1; }
    ;

declaration
    : identifier COLON signature
        { MAKE($$, declaration, *$1, *$3); }
    | function_decl
        { $$ = $1; }
    ;

signature
    : type { $$ = new ddc::signature_t(*$1); }
    ;

type
    : identifier { $$ = new ddc::type_t(*$1); }
    ;

identifier
    : IDENTIFIER { $$ = new ddc::identifier_t(*$1); }
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