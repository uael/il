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
%token STRUCT ENUM IFACE
%token VOID BOOL CHAR INT UINT SINT SHORT USHORT SSHORT FLOAT UFLOAT SFLOAT DOUBLE UDOUBLE SDOUBLE
%token GT LT ADD SUB MUL DIV EQ NEQ LE GE
%token COLON SEMICOLON COMMA LPAR RPAR LBRA RBRA INLINE ASSIGN
%token COND MOD ACCESS AND INC DEC LS LSQU RS RSQU NOT DOT TID OR XOR LAND LOR
%token ELLIPSIS RIGHT_ASSIGN LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN
%token MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token AS CASE DEFAULT IF ELSE SWITCH WHILE FOR DO GOTO CONTINUE BREAK RETURN

%destructor { delete $$; } ID INT_CONST FLOAT_CONST STRING_CONST

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
  : /* empty */
  | program iface_decl
  | program struct_decl
  ;

/* ----------------------- TYPE ----------------------- */

ids
  : ID
  | ids COMMA ID
  ;

typed_ids
  : ids COLON type_spec
  ;

typed_ids_list
  : typed_ids
  | typed_ids_list COMMA typed_ids
  ;

braced_typed_ids
  : ids COLON braced_type_spec
  ;

braced_typed_ids_list
  : braced_typed_ids
  | braced_typed_ids_list COMMA braced_typed_ids
  ;

type_specs
  : type_spec
  | type_specs COMMA type_spec
  ;

type_spec
  : ID
  | scalar_spec
  | lambda_spec
  | struct_spec
  | enum_spec
  | braced_struct_spec
  ;

braced_type_spec
  : ID
  | braced_struct_spec
  ;

scalar_spec
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

struct_specs
  : struct_spec
  | struct_specs COMMA struct_spec
  ;

struct_spec
  : braced_struct_spec
  | ID
  | ID LT type_specs GT
  ;

braced_struct_spec
  : STRUCT LBRA struct_body RBRA
  | STRUCT COLON struct_specs LBRA struct_body RBRA
  | IFACE LBRA iface_body RBRA
  | IFACE COLON struct_specs LBRA iface_body RBRA
  ;

lambda_spec
  : type_spec LPAR RPAR
  | type_spec LT type_specs GT LPAR RPAR
  | type_spec LPAR type_specs RPAR
  | type_spec LT type_specs GT LPAR type_specs RPAR
  | LPAR RPAR
  | LT type_specs GT LPAR RPAR
  | LPAR type_specs RPAR
  | LT type_specs GT LPAR type_specs RPAR
  ;

signed_arguments_lambda_spec
  : type_spec LPAR RPAR
  | type_spec LT type_specs GT LPAR RPAR
  | type_spec LPAR typed_ids_list RPAR
  | type_spec LT type_specs GT LPAR typed_ids_list RPAR
  ;

enum_spec
  : ID
  | ENUM LBRA enum_body RBRA
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
  | LPAR type_spec RPAR cast_expr
  | cast_expr AS type_spec
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
  | expr COMMA assign_expr
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

const_expr
  : cond_expr
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
  ;

expr_stmt
  : SEMICOLON
  | expr SEMICOLON
  ;

label_stmt
  : ID COLON stmt
  | CASE const_expr COLON stmt
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
	| /* FOR LPAR decl expr_stmt RPAR stmt */
	| /* FOR LPAR decl expr_stmt expr RPAR stmt */
	;

jump_stmt
  : GOTO ID SEMICOLON
  | CONTINUE SEMICOLON
  | BREAK SEMICOLON
  | RETURN SEMICOLON
  | RETURN expr SEMICOLON
  ;

/* ----------------------- DECLARATIONS ----------------------- */

Ts_decl
  : /* empty */
  | LT Ts_decl_list GT
  ;

Ts_decl_list
  : T_decl
  | Ts_decl_list COMMA T_decl
  ;

T_decl
  : ids
  | typed_ids
  ;

iface_decl
  : IFACE ID Ts_decl LBRA iface_body RBRA
  | IFACE ID Ts_decl COLON struct_specs LBRA iface_body RBRA
  ;

struct_decl
  : STRUCT ID Ts_decl LBRA struct_body RBRA
  | STRUCT ID Ts_decl COLON struct_specs LBRA struct_body RBRA
  ;

iface_body
  : /* empty */
  | iface_body prop_proto
  | iface_body method_proto
  ;

struct_body
  : /* empty */
  | iface_body method_decl
  ;

enum_body
  :
  ;

prop_proto
  : ids COLON type_spec SEMICOLON
  | ids COLON braced_type_spec
  ;

method_proto
  : ids LPAR RPAR COLON type_spec SEMICOLON
  | ids LT type_specs GT LPAR RPAR COLON type_spec SEMICOLON
  | ids LPAR type_specs RPAR COLON type_spec SEMICOLON
  | ids LT type_specs GT LPAR type_specs RPAR COLON type_spec SEMICOLON
  ;

method_decl
  : ids LPAR RPAR COLON type_spec INLINE compound_stmt
  | ids LT type_specs GT LPAR RPAR COLON type_spec INLINE compound_stmt
  | ids LPAR type_specs RPAR COLON type_spec INLINE compound_stmt
  | ids LT type_specs GT LPAR type_specs RPAR COLON type_spec INLINE compound_stmt
  | ids LPAR RPAR INLINE compound_stmt
  | ids LT type_specs GT LPAR RPAR INLINE compound_stmt
  | ids LPAR type_specs RPAR INLINE compound_stmt
  | ids LT type_specs GT LPAR type_specs RPAR INLINE compound_stmt
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