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

%start program

%%

program
  : /* empty */
  | program decl_function
  | program decl_enum
  | program decl_interface
  | program decl_struct
  | program decl_class
  ;

/* ----------------------- COMMON ----------------------- */

id_list
  : ID
  | id_list COMMA ID
  ;

/* ----------------------- QUALIFIER ----------------------- */

qualifier_modifier
  : /* empty */
  | PRIVATE
  | PROTECTED
  ;

qualifier_struct
  : /* empty */
  | FINAL
  | ABSTRACT
  ;

qualifier_class
  : qualifier_modifier qualifier_struct
  ;

qualifier_type
  : /* empty */
  | qualifier_type FINAL
  | qualifier_type VOLATILE
  ;

qualifier_function
  : /* empty */
  | qualifier_type
  | qualifier_function INLINE
  ;

qualifier_method
  : /* empty */
  | qualifier_function
  | qualifier_method ABSTRACT
  | qualifier_method STATIC
  ;

/* ----------------------- SPECIFIER ----------------------- */

signed_specifier
  : ID
  | specifier_scalar
  | specifier_in_struct
  | specifier_type
  | specifier_lambda
  | specifier_nested
  ;

specifier
  : signed_specifier
  | specifier_enum
  | specifier_interface
  | specifier_struct
  | specifier_class
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

specifier_in_struct
  : STATIC
  | SELF
  ;

specifier_enum
  : ID
  | ENUM LBRA body_enum RBRA
  ;

specifier_type
  : ID
  | ID LT specifier_list GT
  ;

specifier_interface
  : specifier_type
  | INTERFACE
  | INTERFACE LBRA body_interface RBRA
  | INTERFACE COLON specifier_interface_list LBRA body_interface RBRA
  ;

specifier_struct
  : specifier_type
  | STRUCT
  | STRUCT LBRA body_struct RBRA
  | STRUCT COLON specifier_struct_list LBRA body_struct RBRA
  ;

specifier_class
  : specifier_type
  | CLASS
  | CLASS LBRA body_class RBRA
  | CLASS COLON specifier_class_list LBRA body_class RBRA
  ;

specifier_lambda
  : decl_generics_or_empty LPAR RPAR
  | decl_generics_or_empty LPAR specifier_list RPAR
  | specifier decl_generics_or_empty LPAR RPAR
  | specifier decl_generics_or_empty LPAR specifier_list RPAR
  ;

specifier_nested
  : signed_specifier DOUBLE_COLON signed_specifier
  | specifier_nested DOUBLE_COLON signed_specifier
  ;
  
specifier_list
  : specifier
  | specifier_list COMMA specifier
  ;

specifier_interface_list
  : specifier_interface
  | specifier_interface_list COMMA specifier_interface
  ;

specifier_struct_list
  : specifier_struct
  | specifier_struct_list COMMA specifier_struct
  ;

specifier_class_list
  : specifier_class
  | specifier_class_list COMMA specifier_class
  | specifier_class_list COMMA specifier_struct
  ;

/* ----------------------- SIGNATURE ----------------------- */

signature
  : COLON specifier
  | COLON signed_specifier LBRA RBRA
  | COLON signed_specifier LBRA body_class RBRA
  ;

signature_or_empty
  : /* empty */
  | signature
  ;

/* ----------------------- EXPRESSION ----------------------- */

expr_const
  : INT_CONST
  | FLOAT_CONST
  | STRING_CONST
  | NEW signed_specifier
  | prototype_typed_or_not_lambda expr_closure
  ;

expr_primary
  : ID
  | THIS
  | expr_const
  | LPAR expr RPAR
  ;

expr_postfix
  : expr_primary
  | expr_postfix DOT expr_postfix
  | expr_postfix ACCESS expr_postfix
  | expr_postfix LSQU RSQU
  | expr_postfix LSQU expr RSQU
  | expr_postfix LPAR expr_comma_list RPAR
  | expr_postfix LT specifier_list GT LPAR expr_comma_list RPAR
  | expr_postfix LBRA body_class RBRA
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
  | LPAR specifier RPAR expr_cast
  | expr_cast AS specifier
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
  | ARROW stmt_compound
  ;

expr
  : expr_assign
  ;
  
expr_comma_list
  : /* empty */
  | expr
  | expr_comma_list COMMA expr
  ;
  
/* ----------------------- STATEMENTS ----------------------- */
  
stmt
	: stmt_expr
	| stmt_label
	| stmt_compound
	| stmt_select
	| stmt_iter
	| stmt_jump
	| stmt_var
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
	| FOR LPAR stmt_var stmt_expr RPAR stmt
	| FOR LPAR stmt_var stmt_expr expr RPAR stmt
	| FOR LPAR stmt_var stmt_expr stmt_compound RPAR stmt
	;

stmt_jump
  : GOTO ID SEMICOLON
  | CONTINUE SEMICOLON
  | BREAK SEMICOLON
  | RETURN SEMICOLON
  | RETURN expr SEMICOLON
  ;

stmt_var
  : VAR decl_var_list SEMICOLON
  ;

stmt_closure
  : ARROW expr SEMICOLON
  | ARROW stmt_compound
  ;

stmt_list
  : /* empty */
  | stmt
  | stmt_list stmt
  ;

/* ----------------------- PROTOTYPE ----------------------- */

prototype_typed_or_not_lambda
  : prototype_typed_lambda
  | LPAR RPAR
  | LPAR id_list RPAR
  ;

prototype_typed_lambda
  : decl_generics_or_empty decl_signed_args signature_or_empty
  ;

prototype_property
  : id_list signature
  ;

prototype_function
  : id_list decl_generics_or_empty decl_signed_or_not_args signature_or_empty
  ;

/* ----------------------- DECLARATION ----------------------- */

decl_generic
  : ID signature_or_empty
  ;
  
decl_generic_list
  : decl_generic
  | decl_generic_list decl_generic
  ;
  
decl_generics
  : LT decl_generic_list GT
  ;

decl_generics_or_empty
  : /* empty */
  | decl_generics
  ;

decl_var
  : qualifier_type id_list signature
  | qualifier_type id_list signature_or_empty ASSIGN expr_cond
  ;

decl_var_list
  : decl_var
  | decl_var_list COMMA decl_var
  ;

decl_signed_or_not_args
  : decl_signed_args
  | LPAR specifier_list RPAR
  ;

decl_signed_args
  : LPAR RPAR
  | LPAR decl_var_list RPAR
  ;

decl_enum
  : ENUM ID LBRA body_enum RBRA
  ;

decl_interface
  : INTERFACE ID decl_generics_or_empty SEMICOLON
  | INTERFACE ID decl_generics_or_empty LBRA body_interface RBRA
  | INTERFACE ID decl_generics_or_empty COLON specifier_interface_list SEMICOLON
  | INTERFACE ID decl_generics_or_empty COLON specifier_interface_list LBRA body_interface RBRA
  ;

decl_struct
  : qualifier_struct STRUCT ID decl_generics_or_empty SEMICOLON
  | qualifier_struct STRUCT ID decl_generics_or_empty LBRA body_struct RBRA
  | qualifier_struct STRUCT ID decl_generics_or_empty COLON specifier_struct_list SEMICOLON
  | qualifier_struct STRUCT ID decl_generics_or_empty COLON specifier_struct_list LBRA body_struct RBRA
  ;

decl_class
  : qualifier_class CLASS ID decl_generics_or_empty SEMICOLON
  | qualifier_class CLASS ID decl_generics_or_empty LBRA body_class RBRA
  | qualifier_class CLASS ID decl_generics_or_empty COLON specifier_class_list SEMICOLON
  | qualifier_class CLASS ID decl_generics_or_empty COLON specifier_class_list LBRA body_class RBRA
  ;

decl_property
  : prototype_property SEMICOLON
  | prototype_property ASSIGN stmt_expr
  ;

decl_function
  : qualifier_function id_list prototype_typed_lambda stmt_closure
  ;

decl_method
  : qualifier_method decl_function
  ;

decl_method_override
  : qualifier_method id_list stmt_closure
  ;

decl_property_override
  : id_list ASSIGN stmt_expr
  ;

/* ----------------------- BODY ----------------------- */

body_interface
  : /* empty */
  | body_interface prototype_property SEMICOLON
  | body_interface prototype_function SEMICOLON
  ;

body_struct
  : /* empty */
  | body_struct decl_property
  | body_struct decl_property_override
  | body_struct decl_method
  | body_struct decl_method_override
  ;

body_class
  : /* empty */
  | body_class qualifier_modifier decl_method
  | body_class qualifier_modifier decl_method_override
  | body_class qualifier_modifier decl_property
  | body_class qualifier_modifier decl_property_override
  ;

body_enum
  : /* empty */
  | body_enum ID SEMICOLON
  | body_enum ID ASSIGN stmt_expr
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