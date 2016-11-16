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

#define MAKE(v, type, ...) v = new ddc::type##_t(__VA_ARGS__)
#define MAKE_FUNCTION_DECL(v, ...) MAKE(v, function_declaration, __VA_ARGS__);
#define PUSH(v, arg) v->push_back(arg)
%}

%start program

%%

program
  : /* empty */
  | program interface_declaration
  | program struct_declaration
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

type_qualifier
  : /* empty */
  | type_qualifier CONST
  | type_qualifier VOLATILE
  ;

struct_qualifier
  : /* empty */
  | FINAL
  | ABSTRACT
  ;

meth_qualifier
  : meth_qualifier func_qualifier
  | meth_qualifier ABSTRACT
  | meth_qualifier STATIC
  | meth_qualifier VIRTUAL
  | meth_qualifier FINAL
  ;

func_qualifier
  : /* empty */
  | INLINE
  ;

/* ----------------------- TYPE ----------------------- */

signature
  : COLON type_specifier
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

type_specifiers
  : type_specifier
  | type_specifiers COMMA type_specifier
  ;

type_specifier
  : ID
  | scalar_specifier
  | lambda_specifier
  | struct_specifier
  | enum_specifier
  | braced_struct_specifier
  ;

braced_type_specifier
  : braced_struct_specifier
  ;

scalar_specifier
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

struct_specifiers
  : struct_specifier
  | struct_specifiers COMMA struct_specifier
  ;

struct_specifier
  : braced_struct_specifier
  | ID
  | ID LT type_specifiers GT
  ;

braced_struct_specifier
  : STRUCT LBRA struct_body RBRA
  | STRUCT COLON struct_specifiers LBRA struct_body RBRA
  | INTERFACE LBRA interface_body RBRA
  | INTERFACE COLON struct_specifiers LBRA interface_body RBRA
  ;

enum_specifier
  : ID
  | ENUM LBRA enum_body RBRA
  ;

lambda_specifier
  : generics_declaration_or_empty LPAR RPAR
  | generics_declaration_or_empty LPAR type_specifiers RPAR
  | type_specifier generics_declaration_or_empty LPAR RPAR
  | type_specifier generics_declaration_or_empty LPAR type_specifiers RPAR
  ;

/* ----------------------- EXPRESSIONS ----------------------- */

comma_exprs
  : /* empty */
  | expr
  | comma_exprs COMMA expr
  ;

const
  : INT_CONST
  | FLOAT_CONST
  | STRING_CONST
  | typed_or_not_lambda_const
  ;

typed_or_not_lambda_const
  : typed_lambda_const
  | LPAR RPAR ARROW closure_expr
  | LPAR ids RPAR ARROW closure_expr
  ;

typed_lambda_const
  : generics_declaration_or_empty signed_args_declaration signature_or_empty ARROW closure_expr
  ;

prim_expr
  : ID
  | const
  | LPAR expr RPAR
  ;

post_expr
  : prim_expr
  | post_expr DOT ID /* namespace access */
  | post_expr ACCESS ID /* access */
  | post_expr LSQU RSQU /* array add */
  | post_expr LSQU expr RSQU /* array access */
  | post_expr LPAR comma_exprs RPAR /* func call */
  | post_expr LT type_specifiers GT LPAR comma_exprs RPAR /* func call generic */
  | post_expr INC
  | post_expr DEC
  ;

pre_expr
  : post_expr
  | INC pre_expr
  | DEC pre_expr
  | AND pre_expr
  | ADD pre_expr
  | SUB pre_expr
  | MUL pre_expr
  | NOT pre_expr
  | TID pre_expr
  ;

cast_expr
  : pre_expr
  | LPAR type_specifier RPAR cast_expr
  | cast_expr AS type_specifier
  ;

mul_expr
  : cast_expr
  | mul_expr MUL cast_expr
  | mul_expr DIV cast_expr
  | mul_expr MOD cast_expr
  ;

add_expr
  : mul_expr
  | add_expr ADD mul_expr
  | add_expr SUB mul_expr
  ;

shift_expr
  : add_expr
  | shift_expr LS add_expr
  | shift_expr RS add_expr
  ;

rel_expr
  : shift_expr
  | rel_expr LT shift_expr
  | rel_expr GT shift_expr
  | rel_expr LE shift_expr
  | rel_expr GE shift_expr
  ;

equal_expr
  : rel_expr
  | equal_expr EQ rel_expr
  | equal_expr NEQ rel_expr
  ;

and_expr
  : equal_expr
  | and_expr AND equal_expr
  ;

xor_expr
  : and_expr
  | xor_expr XOR and_expr
  ;

or_expr
  : xor_expr
  | or_expr OR xor_expr
  ;

land_expr
  : or_expr
  | land_expr LAND or_expr
  ;

lor_expr
  : land_expr
  | lor_expr LOR land_expr
  ;

cond_expr
  : lor_expr
  | lor_expr COND expr COLON cond_expr
  ;

assign_expr
  : cond_expr
  | pre_expr ASSIGN assign_expr
  | pre_expr MUL_ASSIGN assign_expr
  | pre_expr DIV_ASSIGN assign_expr
  | pre_expr MOD_ASSIGN assign_expr
  | pre_expr ADD_ASSIGN assign_expr
  | pre_expr SUB_ASSIGN assign_expr
  | pre_expr LEFT_ASSIGN assign_expr
  | pre_expr RIGHT_ASSIGN assign_expr
  | pre_expr AND_ASSIGN assign_expr
  | pre_expr XOR_ASSIGN assign_expr
  | pre_expr OR_ASSIGN assign_expr
  ;

closure_expr
  : expr
  | compound_stmt
  ;

expr
  : assign_expr
  ;
  
/* ----------------------- STATEMENTS ----------------------- */
  
stmt
	: expr_stmt
	| label_stmt
	| compound_stmt
	| select_stmt
	| iter_stmt
	| jump_stmt
	;

stmts
  : stmt
  | stmts stmt
  | stmts var_declaration
  ;

expr_stmt
  : SEMICOLON
  | expr SEMICOLON
  ;

label_stmt
  : ID COLON stmt
  | CASE cond_expr COLON stmt
  | DEFAULT COLON stmt
  ;

compound_stmt
  : LBRA RBRA
  | LBRA stmts RBRA
  ;

select_stmt
  : IF LPAR expr RPAR stmt ELSE stmt
  | IF LPAR expr RPAR stmt
  | SWITCH LPAR expr RPAR stmt
  ;

iter_stmt
	: WHILE LPAR expr RPAR stmt
	| DO stmt WHILE LPAR expr RPAR SEMICOLON
	| FOR LPAR expr_stmt expr_stmt RPAR stmt
	| FOR LPAR expr_stmt expr_stmt expr RPAR stmt
	| /* FOR LPAR declaration expr_stmt RPAR stmt */
	| /* FOR LPAR declaration expr_stmt expr RPAR stmt */
	;

jump_stmt
  : GOTO ID SEMICOLON
  | CONTINUE SEMICOLON
  | BREAK SEMICOLON
  | RETURN SEMICOLON
  | RETURN expr SEMICOLON
  ;

/* ----------------------- DECLARATIONS ----------------------- */

signed_or_not_args_declaration
  : signed_args_declaration
  | LPAR type_specifiers RPAR
  ;

signed_args_declaration
  : LPAR RPAR
  | LPAR typed_ids_list RPAR
  ;

generics_declaration
  : LT typed_or_not_ids_list GT
  ;

generics_declaration_or_empty
  : /* empty */
  | generics_declaration
  ;

interface_declaration
  : INTERFACE ID generics_declaration_or_empty LBRA interface_body RBRA
  | INTERFACE ID generics_declaration_or_empty COLON struct_specifiers LBRA interface_body RBRA
  ;

struct_declaration
  : struct_qualifier STRUCT ID generics_declaration_or_empty LBRA struct_body RBRA
  | struct_qualifier STRUCT ID generics_declaration_or_empty COLON struct_specifiers LBRA struct_body RBRA
  ;

interface_body
  : /* empty */
  | interface_body ids signature_or_empty SEMICOLON
  | interface_body ids COLON braced_type_specifier
  | interface_body ids generics_declaration_or_empty signed_or_not_args_declaration signature_or_empty SEMICOLON
  ;

struct_body
  : /* empty */
  | struct_body meth_declaration
  | struct_body ids signature_or_empty
  | struct_body ids signature_or_empty ASSIGN expr_stmt
  ;

meth_declaration
  : ids typed_lambda_const
  ;

var_declaration
  : VAR var_declarators SEMICOLON
  ;

var_declarators
  : var_declarator
  | var_declarators COMMA var_declarator
  ;

var_declarator
  : ID signature
  | ID signature_or_empty ASSIGN cond_expr
  ;

enum_body
  :
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