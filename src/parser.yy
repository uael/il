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
%token <_string> ID GENERIC INT_CONST FLOAT_CONST STRING_CONST
%token STRUCT ENUM INTERFACE CLASS
%token VOID BOOL CHAR INT UINT SINT SHORT USHORT SSHORT FLOAT UFLOAT SFLOAT DOUBLE UDOUBLE SDOUBLE
%token GT LT ADD SUB MUL DIV EQ NEQ LE GE
%token COLON DOUBLE_COLON SEMICOLON COMMA LPAR RPAR LBRA RBRA ARROW ASSIGN
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

%start decl_list

%%

id
  : ID
  ;

id_list
  : id
  | id_list COMMA id
  ;
  
generic
  : GENERIC
  | GENERIC COLON type_specifier
  ;
  
generic_list
  : generic
  | generic_list COMMA generic
  ;

generics
  : /* empty */
  | LT generic_list GT
  ;

decl_list
  : /* empty */
  | decl_list decl
  ;

decl
  : decl_property_expr SEMICOLON
  | decl_property_compound
  | decl_function_expr SEMICOLON
  | decl_function_compound
  ;

decl_property_expr
  : id_list COLON type_specifier
  | id_list ASSIGN expr_cond
  | id_list COLON type_specifier ASSIGN expr_cond
  | id_list COLON type_specifier ARROW expr_cond
  ;

decl_property_compound
  : id_list ARROW stmt_compound
  | id_list COLON type_specifier ARROW stmt_compound
  ;

decl_function_expr
  : id_list generics decl_args ARROW expr_cond
  | id_list generics decl_args COLON type_specifier ARROW expr_cond
  ;

decl_function_compound
  : id_list generics decl_args ARROW stmt_compound
  | id_list generics decl_args COLON type_specifier ARROW stmt_compound
  ;

decl_var
  : decl_property_expr
  | decl_property_compound
  | decl_function_expr
  | decl_function_compound
  ;

decl_var_list
  : /* empty */
  | decl_var
  | decl_var_list COMMA decl_var
  ;

decl_args
  : LPAR decl_var_list RPAR
  ;

type_specifier
  : type
  | MUL type_specifier
  | type_specifier LSQU RSQU
  | type_specifier LPAR type_specifier_list RPAR
  | type_specifier LBRA decl_list RBRA
  ;

type_specifier_list
  : /* empty */
  | type_specifier
  | type_specifier_list COMMA type_specifier
  ;

type
  : type_scalar
  | type_generic
  ;

type_scalar
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

type_generic
  : GENERIC
  ;

stmt_list
  : /* empty */
  | stmt
  | stmt_list stmt
  ;

stmt
	: stmt_expr
	| stmt_stmt
	| stmt_compound
	| stmt_select
	| stmt_iter
	| stmt_jump
	| stmt_decl
	;

stmt_expr
  : SEMICOLON
  | expr SEMICOLON
  ;

stmt_stmt
  : id COLON stmt
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
	| FOR LPAR stmt_decl stmt_expr RPAR stmt
	| FOR LPAR stmt_decl stmt_expr expr RPAR stmt
	| FOR LPAR stmt_decl stmt_expr stmt_compound RPAR stmt
	;

stmt_jump
  : GOTO id SEMICOLON
  | CONTINUE SEMICOLON
  | BREAK SEMICOLON
  | RETURN SEMICOLON
  | RETURN expr SEMICOLON
  ;

stmt_decl
  : VAR decl_var_list SEMICOLON
  | VAR LBRA decl_list RBRA
  ;

expr_list
  : /* empty */
  | expr
  | expr_list COMMA expr
  ;

expr_const
  : INT_CONST
  | FLOAT_CONST
  | STRING_CONST
  | LPAR id_list RPAR ARROW expr
  | LPAR id_list RPAR ARROW stmt_compound
  ;

expr_primary
  : id
  | expr_const
  | LPAR expr RPAR
  ;

expr_postfix
  : expr_primary
  | expr_postfix LSQU RSQU
  | expr_postfix LSQU expr RSQU
  | expr_postfix LPAR expr_list RPAR
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
  | LPAR type_specifier RPAR expr_cast
  | expr_cast AS type_specifier
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

expr
  : expr_assign
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