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
#include "ddc.h"

using namespace ddc::ast;

#include <cstdio>
#include <cstdlib>
%}

%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {ddc}
%define parser_class_name {parser}

%parse-param {class driver &driver}

%locations

%union {
  std::string *_string;
}

%token END 0 "end of file"
%token EOL "end of line"
%token <_string> ID INT_CONST FLOAT_CONST STRING_CONST
%token STRUCT ENUM INTERFACE
%token VOID BOOL CHAR INT UINT SINT SHORT USHORT SSHORT FLOAT UFLOAT SFLOAT DOUBLE UDOUBLE SDOUBLE
%token GT LT ADD SUB MUL DIV EQ NEQ LE GE
%token COLON SEMICOLON COMMA LPAR RPAR LBRA RBRA ARROW ASSIGN
%token COND MOD ACCESS AND INC DEC LS LSQU RS RSQU NOT DOT TID OR XOR LAND LOR
%token ELLIPSIS RIGHT_ASSIGN LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN
%token MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token AS CASE DEFAULT IF ELSE SWITCH WHILE FOR DO GOTO CONTINUE BREAK RETURN
%token PRIVATE PROTECTED CONST VOLATILE ABSTRACT STATIC VIRTUAL FINAL INLINE
%token VAR NEW SIZEOF TYPEOF ASSERT TRY CATCH SELF THIS

%destructor { delete $$; } ID INT_CONST FLOAT_CONST STRING_CONST

%{
#include "driver.h"
#include "scanner.h"

#undef yylex
#define yylex driver.lexer->lex
%}

%start program

%%

program
  : /* empty */
  | program decl_interface
  | program decl_struct
  | program decl_function
  ;

/* ----------------------- COMMON ----------------------- */

ids
  : ID
  | ids COMMA ID
  ;

/* ----------------------- QUALIFIER ----------------------- */

modifier
  : /* empty */
  | PRIVATE
  | PROTECTED
  ;

qualifier_type
  : /* empty */
  | qualifier_type CONST
  | qualifier_type VOLATILE
  ;

qualifier_struct
  : /* empty */
  | FINAL
  | ABSTRACT
  ;

qualifier_meth
  : qualifier_meth qualifier_function
  | qualifier_meth ABSTRACT
  | qualifier_meth STATIC
  | qualifier_meth VIRTUAL
  | qualifier_meth FINAL
  ;

qualifier_function
  : /* empty */
  | INLINE
  ;

/* ----------------------- TYPE ----------------------- */

signature
  : COLON specifier_type
  ;

signature_or_empty
  : /* empty */
  | signature
  ;

typed_ids_list
  : ids signature
  | typed_ids_list COMMA ids signature
  ;

typed_or_not_ids_list
  : ID signature_or_empty
  | typed_or_not_ids_list COMMA ID signature_or_empty
  ;

specifier_types
  : specifier_type
  | specifier_types COMMA specifier_type
  ;

specifier_type
  : ID
  | specifier_scalar
  | specifier_lambda
  | specifier_struct
  | specifier_enum
  | specifier_braced_struct
  ;

specifier_braced_type
  : specifier_braced_struct
  ;

specifier_scalar
  : VOID
  | BOOL
  | CHAR
  | INT
  | UINT
  | SINT
  | SHORT
  | USHORT
  | SSHORT
  | FLOAT
  | UFLOAT
  | SFLOAT
  | DOUBLE
  | UDOUBLE
  | SDOUBLE
  ;

specifier_structs
  : specifier_struct
  | specifier_structs COMMA specifier_struct
  ;

specifier_struct
  : specifier_braced_struct
  | ID
  | ID LT specifier_types GT
  ;

specifier_braced_struct
  : STRUCT LBRA body_struct RBRA
  | STRUCT COLON specifier_structs LBRA body_struct RBRA
  | INTERFACE LBRA body_interface RBRA
  | INTERFACE COLON specifier_structs LBRA body_interface RBRA
  ;

specifier_enum
  : ID
  | ENUM LBRA body_enum RBRA
  ;

specifier_lambda
  : decl_generics_or_empty LPAR RPAR
  | decl_generics_or_empty LPAR specifier_types RPAR
  | specifier_type decl_generics_or_empty LPAR RPAR
  | specifier_type decl_generics_or_empty LPAR specifier_types RPAR
  ;

/* ----------------------- EXPRESSIONS ----------------------- */

expr_comma_list
  : /* empty */
  | expr
  | expr_comma_list COMMA expr
  ;

const
  : INT_CONST
  | FLOAT_CONST
  | STRING_CONST
  | typed_or_not_lambda_const
  ;

prototype_typed_or_not_lambda
  : prototype_typed_lambda
  | LPAR RPAR
  | LPAR ids RPAR
  ;

prototype_typed_lambda
  : decl_generics_or_empty decl_signed_args signature_or_empty
  ;

typed_or_not_lambda_const
  : prototype_typed_or_not_lambda expr_closure
  ;

typed_lambda_const
  : prototype_typed_lambda expr_closure
  ;

expr_primary
  : ID
  | const
  | LPAR expr RPAR
  ;

expr_postfix
  : expr_primary
  | expr_postfix DOT ID /* namespace access */
  | expr_postfix ACCESS ID /* access */
  | expr_postfix LSQU RSQU /* array add */
  | expr_postfix LSQU expr RSQU /* array access */
  | expr_postfix LPAR expr_comma_list RPAR /* function call */
  | expr_postfix LT specifier_types GT LPAR expr_comma_list RPAR /* function call generic */
  | expr_postfix INC
  | expr_postfix DEC
  ;

expr_prefix
  : expr_postfix
  | INC expr_prefix
  | DEC expr_prefix
  | AND expr_prefix
  | ADD expr_prefix
  | SUB expr_prefix
  | MUL expr_prefix
  | NOT expr_prefix
  | TID expr_prefix
  ;

expr_cast
  : expr_prefix
  | LPAR specifier_type RPAR expr_cast
  | expr_cast AS specifier_type
  ;

expr_mul
  : expr_cast
  | expr_mul MUL expr_cast
  | expr_mul DIV expr_cast
  | expr_mul MOD expr_cast
  ;

expr_add
  : expr_mul
  | expr_add ADD expr_mul
  | expr_add SUB expr_mul
  ;

expr_shift
  : expr_add
  | expr_shift LS expr_add
  | expr_shift RS expr_add
  ;

expr_relational
  : expr_shift
  | expr_relational LT expr_shift
  | expr_relational GT expr_shift
  | expr_relational LE expr_shift
  | expr_relational GE expr_shift
  ;

expr_equal
  : expr_relational
  | expr_equal EQ expr_relational
  | expr_equal NEQ expr_relational
  ;

expr_and
  : expr_equal
  | expr_and AND expr_equal
  ;

expr_xor
  : expr_and
  | expr_xor XOR expr_and
  ;

expr_or
  : expr_xor
  | expr_or OR expr_xor
  ;

expr_land
  : expr_or
  | expr_land LAND expr_or
  ;

expr_lor
  : expr_land
  | expr_lor LOR expr_land
  ;

expr_cond
  : expr_lor
  | expr_lor COND expr COLON expr_cond
  ;

expr_assign
  : expr_cond
  | expr_prefix ASSIGN expr_assign
  | expr_prefix MUL_ASSIGN expr_assign
  | expr_prefix DIV_ASSIGN expr_assign
  | expr_prefix MOD_ASSIGN expr_assign
  | expr_prefix ADD_ASSIGN expr_assign
  | expr_prefix SUB_ASSIGN expr_assign
  | expr_prefix LEFT_ASSIGN expr_assign
  | expr_prefix RIGHT_ASSIGN expr_assign
  | expr_prefix AND_ASSIGN expr_assign
  | expr_prefix XOR_ASSIGN expr_assign
  | expr_prefix OR_ASSIGN expr_assign
  ;

expr_closure
  : ARROW expr
  | stmt_compound
  | ARROW stmt_compound
  ;

expr
  : expr_assign
  ;
  
/* ----------------------- STATEMENTS ----------------------- */
  
stmt
	: stmt_expr
	| stmt_label
	| stmt_compound
	| stmt_select
	| stmt_iter
	| stmt_jump
	;

stmt_list
  : /* empty */
  | stmt
  | stmt_list stmt
  | stmt_list decl_var
  ;

stmt_expr
  : SEMICOLON
  | expr SEMICOLON
  ;

stmt_label
  : ID COLON stmt
  | CASE expr_cond COLON stmt
  | DEFAULT COLON stmt
  ;

stmt_compound
  : LBRA RBRA
  | LBRA stmt_list RBRA
  ;

stmt_select
  : IF LPAR expr RPAR stmt ELSE stmt
  | IF LPAR expr RPAR stmt
  | SWITCH LPAR expr RPAR stmt
  ;

stmt_iter
	: WHILE LPAR expr RPAR stmt
	| DO stmt WHILE LPAR expr RPAR SEMICOLON
	| FOR LPAR stmt_expr stmt_expr RPAR stmt
	| FOR LPAR stmt_expr stmt_expr expr RPAR stmt
	| FOR LPAR stmt_expr stmt_expr stmt_compound RPAR stmt
	| FOR LPAR decl_var stmt_expr RPAR stmt
	| FOR LPAR decl_var stmt_expr expr RPAR stmt
	| FOR LPAR decl_var stmt_expr stmt_compound RPAR stmt
	;

stmt_jump
  : GOTO ID SEMICOLON
  | CONTINUE SEMICOLON
  | BREAK SEMICOLON
  | RETURN SEMICOLON
  | RETURN expr SEMICOLON
  ;

stmt_closure
  : ARROW expr SEMICOLON
  | stmt_compound
  | ARROW stmt_compound
  ;

/* ----------------------- DECLARATIONS ----------------------- */

decl_signed_or_not_args
  : decl_signed_args
  | LPAR specifier_types RPAR
  ;

decl_signed_args
  : LPAR RPAR
  | LPAR typed_ids_list RPAR
  ;

decl_generics
  : LT typed_or_not_ids_list GT
  ;

decl_generics_or_empty
  : /* empty */
  | decl_generics
  ;

decl_interface
  : INTERFACE ID decl_generics_or_empty LBRA body_interface RBRA
  | INTERFACE ID decl_generics_or_empty COLON specifier_structs LBRA body_interface RBRA
  ;

decl_struct
  : qualifier_struct STRUCT ID decl_generics_or_empty LBRA body_struct RBRA
  | qualifier_struct STRUCT ID decl_generics_or_empty COLON specifier_structs LBRA body_struct RBRA
  ;

body_interface
  : /* empty */
  | body_interface ids signature SEMICOLON
  | body_interface ids COLON specifier_braced_type
  | body_interface ids decl_generics_or_empty decl_signed_or_not_args signature_or_empty SEMICOLON
  ;

body_struct
  : /* empty */
  | body_struct decl_function
  | body_struct ids signature
  | body_struct ids signature ASSIGN stmt_expr
  | body_struct ids signature ASSIGN prototype_typed_or_not_lambda stmt_closure
  ;

decl_function
  : ids prototype_typed_lambda stmt_closure
  ;

decl_var
  : VAR var_declarators SEMICOLON
  ;

var_declarators
  : var_declarator
  | var_declarators COMMA var_declarator
  ;

var_declarator
  : ID signature
  | ID signature_or_empty ASSIGN expr_cond
  ;

body_enum
  :
  ;

%%

void ddc::parser::error(const ddc::parser::location_type& l, const std::string& m) {
  driver.error(l, m);
}

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */