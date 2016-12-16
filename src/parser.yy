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
#include "dyc.h"

#include <cstdio>
#include <cstdlib>
%}

%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {dyc}
%define parser_class_name {parser}

%parse-param {class driver &driver}

%locations

%union {
  std::string *_string;

  dyc::ast::identifier_t *_id;

  dyc::ast::generic_t *_generic;

  dyc::ast::decl_t *_decl;
  dyc::ast::decl_property_t *_decl_property;
  dyc::ast::decl_function_t *_decl_function;

  dyc::ast::type_specifier_t *_type_specifier;
  dyc::ast::type_t *_type;
  dyc::ast::type_scalar_t *_type_scalar;
  dyc::ast::type_generic_t *_type_generic;

  dyc::ast::stmt_t *_stmt;
  dyc::ast::stmt_expr_t *_stmt_expr;
  dyc::ast::stmt_label_t *_stmt_label;
  dyc::ast::stmt_compound_t *_stmt_compound;
  dyc::ast::stmt_select_t *_stmt_select;
  dyc::ast::stmt_iter_t *_stmt_iter;
  dyc::ast::stmt_jump_t *_stmt_jump;
  dyc::ast::stmt_decl_t *_stmt_decl;

  dyc::ast::expr_t *_expr;

  dyc::ast::const_value_t *_const_value;
  dyc::ast::const_lambda_t *_const_lambda;
  dyc::ast::const_initializer_t *_const_initializer;
}

%token END 0 "end of file"
%token EOL "end of line"
%token <_string> ID GENERIC INT_CONST FLOAT_CONST STRING_CONST
%token TUPLE ENUM STRUCT INTERFACE CLASS
%token VOID BOOL CHAR INT UINT SINT SHORT USHORT SSHORT FLOAT UFLOAT SFLOAT DOUBLE UDOUBLE SDOUBLE
%token GT LT ADD SUB MUL DIV EQ NEQ LE GE
%token COLON DOUBLE_COLON SEMICOLON COMMA LPAR RPAR LBRA RBRA ARROW ASSIGN
%token COND MOD ACCESS AND INC DEC LS LSQU RS RSQU NOT DOT TID OR XOR LAND LOR
%token ELLIPSIS RIGHT_ASSIGN LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN
%token MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token AS CASE DEFAULT IF ELSE SWITCH WHILE FOR DO GOTO CONTINUE BREAK RETURN
%token PRIVATE PROTECTED CONST VOLATILE ABSTRACT STATIC VIRTUAL FINAL INLINE
%token VAR NEW SIZEOF TYPEOF ASSERT TRY CATCH SELF THIS

%destructor { if ($$) delete $$; $$ = nullptr; } ID INT_CONST FLOAT_CONST STRING_CONST

%type <_id> id id_list

%type <_generic> generic generic_list generics

%type <_decl> decl decl_var decl_list decl_args decl_comma_list
%type <_decl_property> decl_property_expr decl_property_compound
%type <_decl_function> decl_function_expr decl_function_compound

%type <_type_specifier> type_specifier type_specifier_list type_specifier_unit
%type <_type> type
%type <_type_scalar> type_scalar
%type <_type_generic> type_generic

%type <_stmt> stmt stmt_list
%type <_stmt_expr> stmt_expr
%type <_stmt_label> stmt_label
%type <_stmt_compound> stmt_compound
%type <_stmt_select> stmt_select
%type <_stmt_iter> stmt_iter
%type <_stmt_jump> stmt_jump
%type <_stmt_decl> stmt_decl

%type <_expr> expr expr_list
%type <_expr> expr_assign
%type <_expr> expr_cond
%type <_expr> expr_lor
%type <_expr> expr_land
%type <_expr> expr_or
%type <_expr> expr_xor
%type <_expr> expr_and
%type <_expr> expr_equal
%type <_expr> expr_relational
%type <_expr> expr_shift
%type <_expr> expr_add
%type <_expr> expr_mul
%type <_expr> expr_cast
%type <_expr> expr_prefix
%type <_expr> expr_postfix
%type <_expr> expr_primary
%type <_expr> expr_kvp expr_kvp_list
%type <_expr> expr_const

%type <_const_value> const_value
%type <_const_lambda> const_lambda
%type <_const_initializer> const_initializer

%{
#include "driver.h"
#include "scanner.h"

using namespace std;
using namespace dyc::ast;

#undef yylex
#define yylex driver.lexer->lex

#define MAKE(n, l, t, ...) do { n = new t(__VA_ARGS__); n->loc = &l; } while(0)
%}

%destructor { if ($$) delete $$; $$ = nullptr; } id id_list

%destructor { if ($$) delete $$; $$ = nullptr; } generic generic_list generics

%destructor { if ($$) delete $$; $$ = nullptr; } decl decl_var decl_list decl_args decl_comma_list
%destructor { if ($$) delete $$; $$ = nullptr; } decl_property_expr decl_property_compound
%destructor { if ($$) delete $$; $$ = nullptr; } decl_function_expr decl_function_compound

%destructor { if ($$) delete $$; $$ = nullptr; } type_specifier type_specifier_list type_specifier_unit
%destructor { if ($$) delete $$; $$ = nullptr; } type
%destructor { if ($$) delete $$; $$ = nullptr; } type_scalar
%destructor { if ($$) delete $$; $$ = nullptr; } type_generic

%destructor { if ($$) delete $$; $$ = nullptr; } stmt stmt_list
%destructor { if ($$) delete $$; $$ = nullptr; } stmt_expr
%destructor { if ($$) delete $$; $$ = nullptr; } stmt_label
%destructor { if ($$) delete $$; $$ = nullptr; } stmt_compound
%destructor { if ($$) delete $$; $$ = nullptr; } stmt_select
%destructor { if ($$) delete $$; $$ = nullptr; } stmt_iter
%destructor { if ($$) delete $$; $$ = nullptr; } stmt_jump
%destructor { if ($$) delete $$; $$ = nullptr; } stmt_decl

%destructor { if ($$) delete $$; $$ = nullptr; } expr expr_list
%destructor { if ($$) delete $$; $$ = nullptr; } expr_assign
%destructor { if ($$) delete $$; $$ = nullptr; } expr_cond
%destructor { if ($$) delete $$; $$ = nullptr; } expr_lor
%destructor { if ($$) delete $$; $$ = nullptr; } expr_land
%destructor { if ($$) delete $$; $$ = nullptr; } expr_or
%destructor { if ($$) delete $$; $$ = nullptr; } expr_xor
%destructor { if ($$) delete $$; $$ = nullptr; } expr_and
%destructor { if ($$) delete $$; $$ = nullptr; } expr_equal
%destructor { if ($$) delete $$; $$ = nullptr; } expr_relational
%destructor { if ($$) delete $$; $$ = nullptr; } expr_shift
%destructor { if ($$) delete $$; $$ = nullptr; } expr_add
%destructor { if ($$) delete $$; $$ = nullptr; } expr_mul
%destructor { if ($$) delete $$; $$ = nullptr; } expr_cast
%destructor { if ($$) delete $$; $$ = nullptr; } expr_prefix
%destructor { if ($$) delete $$; $$ = nullptr; } expr_postfix
%destructor { if ($$) delete $$; $$ = nullptr; } expr_primary
%destructor { if ($$) delete $$; $$ = nullptr; } expr_const

%destructor { if ($$) delete $$; $$ = nullptr; } const_value
%destructor { if ($$) delete $$; $$ = nullptr; } const_lambda
%destructor { if ($$) delete $$; $$ = nullptr; } const_initializer

%destructor { if ($$) delete $$; $$ = nullptr; } expr_kvp expr_kvp_list

%start program

%%

program
  : decl_list {
      driver.ast = dyc::ast_t($1);
    }
  ;

id
  : ID {
      MAKE($$, @$, identifier_t, $1);
    }
  ;

id_list
  : id {
      $$ = $1;
    }
  | id_list COMMA id {
      $$ = $1->push($3);
    }
  ;
  
generic
  : GENERIC {
      MAKE($$, @$, generic_t, $1, nullptr);
    }
  | GENERIC COLON type_specifier {
      MAKE($$, @$, generic_t, $1, $3);
    }
  ;
  
generic_list
  : generic {
      $$ = $1;
    }
  | generic_list COMMA generic {
      $$ = $1->push($3);
    }
  ;

generics
  : /* empty */ {
      $$ = nullptr;
    }
  | LT generic_list GT {
      $$ = $2;
    }
  ;

decl
  : decl_property_expr SEMICOLON {
      $$ = $1;
    }
  | decl_property_compound {
      $$ = $1;
    }
  | decl_function_expr SEMICOLON {
      $$ = $1;
    }
  | decl_function_compound {
      $$ = $1;
    }
  ;

decl_list
  : /* empty */ {
      $$ = nullptr;
    }
  | decl {
      $$ = $1;
    }
  | decl_list decl {
      $$ = $1->push($2);
    }
  ;

decl_property_expr
  : id_list COLON type_specifier {
      MAKE($$, @$, decl_property_t, $1, $3, nullptr, false);
    }
  | id_list ASSIGN expr_cond {
      MAKE($$, @$, decl_property_t, $1, nullptr, $3, true);
    }
  | id_list COLON type_specifier ASSIGN expr_cond {
      MAKE($$, @$, decl_property_t, $1, $3, $5, true);
    }
  | id_list COLON type_specifier ARROW expr_cond {
      MAKE($$, @$, decl_property_t, $1, $3, $5, false);
    }
  ;

decl_property_compound
  : id_list ARROW stmt_compound {
      MAKE($$, @$, decl_property_t, $1, nullptr, $3, false);
    }
  | id_list COLON type_specifier_list ARROW stmt_compound {
      MAKE($$, @$, decl_property_t, $1, $3, $5, false);
    }
  ;

decl_function_expr
  : id_list generics decl_args ARROW expr_cond {
      MAKE($$, @$, decl_function_t, $1, $2, $3, nullptr, $5);
    }
  | id_list generics decl_args COLON type_specifier_list ARROW expr_cond {
      MAKE($$, @$, decl_function_t, $1, $2, $3, $5, $7);
    }
  ;

decl_function_compound
  : id_list generics decl_args ARROW stmt_compound {
      MAKE($$, @$, decl_function_t, $1, $2, $3, nullptr, $5);
    }
  | id_list generics decl_args COLON type_specifier_list ARROW stmt_compound {
      MAKE($$, @$, decl_function_t, $1, $2, $3, $5, $7);
    }
  ;

decl_var
  : decl_property_expr {
      $$ = $1;
    }
  | decl_property_compound {
      $$ = $1;
    }
  | decl_function_expr {
      $$ = $1;
    }
  | decl_function_compound {
      $$ = $1;
    }
  ;

decl_comma_list
  : /* empty */ {
      $$ = nullptr;
    }
  | decl_var {
      $$ = $1;
    }
  | decl_comma_list COMMA decl_var {
      $$ = $1->push($3);
    }
  ;

decl_args
  : LPAR decl_comma_list RPAR {
      $$ = $2;
    }
  ;

type_specifier
  : type_specifier_unit {
      $$ = $1;
    }
  | TUPLE LT type_specifier_list GT {
      $$ = $3;
    }
  ;

type_specifier_list
  : type_specifier {
      $$ = $1;
    }
  | type_specifier_list COMMA type_specifier {
      $$ = $1->push($3);
    }
  ;

type_specifier_unit
  : type {
      $$ = $1;
    }
  | type_specifier_unit LSQU RSQU {
      MAKE($$, @$, type_array_t, $1);
    }
  | type_specifier_unit LSQU expr_cond RSQU {
      MAKE($$, @$, type_array_t, $1, $3);
    }
  | type_specifier_unit LPAR RPAR {
      MAKE($$, @$, type_callable_t, $1);
    }
  | type_specifier_unit LPAR type_specifier_list RPAR {
      MAKE($$, @$, type_callable_t, $1, $3);
    }
  ;

type
  : type_scalar {
      $$ = $1;
    }
  | type_generic {
      $$ = $1;
    }
  ;

type_scalar
  : VOID {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::VOID);
    }
  | BOOL {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::BOOL);
    }
  | CHAR {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::CHAR);
    }
  | INT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::INT);
    }
  | UINT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::UINT);
    }
  | SINT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::SINT);
    }
  | SHORT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::SHORT);
    }
  | USHORT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::USHORT);
    }
  | SSHORT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::SSHORT);
    }
  | FLOAT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::FLOAT);
    }
  | UFLOAT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::UFLOAT);
    }
  | SFLOAT {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::SFLOAT);
    }
  | DOUBLE {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::DOUBLE);
    }
  | UDOUBLE {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::UDOUBLE);
    }
  | SDOUBLE {
      MAKE($$, @$, type_scalar_t, type_scalar_t::kind_t::SDOUBLE);
    }
  ;

type_generic
  : GENERIC {
      MAKE($$, @$, type_generic_t, $1);
    }
  ;

stmt
  : stmt_expr {
      $$ = $1;
    }
  | stmt_label {
      $$ = $1;
    }
  | stmt_compound {
      $$ = $1;
    }
  | stmt_select {
      $$ = $1;
    }
  | stmt_iter {
      $$ = $1;
    }
  | stmt_jump {
      $$ = $1;
    }
  | stmt_decl {
      $$ = $1;
    }
  ;

stmt_list
  : /* empty */ {
      $$ = nullptr;
    }
  | stmt {
      $$ = $1;
    }
  | stmt_list stmt {
      $$ = $1->push($2);
    }
  ;

stmt_expr
  : SEMICOLON {
      MAKE($$, @$, stmt_expr_t, );
    }
  | expr SEMICOLON {
      MAKE($$, @$, stmt_expr_t, $1);
    }
  ;

stmt_label
  : ID COLON stmt {
      MAKE($$, @$, stmt_label_t, $1, $3);
    }
  | CASE expr_cond COLON stmt {
      MAKE($$, @$, stmt_label_t, $2, $4);
    }
  | DEFAULT COLON stmt {
      MAKE($$, @$, stmt_label_t, $3);
    }
  ;

stmt_compound
  : LBRA stmt_list RBRA {
      MAKE($$, @$, stmt_compound_t, $2);
    }
  ;

stmt_select
  : IF LPAR expr RPAR stmt ELSE stmt {
      MAKE($$, @$, stmt_select_t, $3, $5, $7);
    }
  | IF LPAR expr RPAR stmt {
      MAKE($$, @$, stmt_select_t, stmt_select_t::kind_t::IF, $3, $5);
    }
  | SWITCH LPAR expr RPAR stmt {
      MAKE($$, @$, stmt_select_t, stmt_select_t::kind_t::SWITCH, $3, $5);
    }
  ;

stmt_iter
	: WHILE LPAR expr RPAR stmt {
	    MAKE($$, @$, stmt_iter_t, $3, $5);
	  }
	| DO stmt WHILE LPAR expr RPAR SEMICOLON {
      MAKE($$, @$, stmt_iter_t, $2, $5);
    }
	| FOR LPAR stmt_expr stmt_expr RPAR stmt {
      $$ = new stmt_iter_t(
        $3, $4 ? $4->expr : nullptr, nullptr, $6
      );
    }
	| FOR LPAR stmt_expr stmt_expr expr RPAR stmt {
      $$ = new stmt_iter_t(
        $3, $4 ? $4->expr : nullptr, $5, $7
      );
    }
	| FOR LPAR stmt_expr stmt_expr stmt_compound RPAR stmt {
      $$ = new stmt_iter_t(
        $3, $4 ? $4->expr : nullptr, $5, $7
      );
    }
	| FOR LPAR stmt_decl stmt_expr RPAR stmt {
      $$ = new stmt_iter_t(
        $3, $4 ? $4->expr : nullptr, nullptr, $6
      );
    }
	| FOR LPAR stmt_decl stmt_expr expr RPAR stmt {
      $$ = new stmt_iter_t(
        $3, $4 ? $4->expr : nullptr, $5, $7
      );
    }
	| FOR LPAR stmt_decl stmt_expr stmt_compound RPAR stmt {
	    $$ = new stmt_iter_t(
	      $3, $4 ? $4->expr : nullptr, $5, $7
	    );
	  }
	;

stmt_jump
  : GOTO ID SEMICOLON {
      MAKE($$, @$, stmt_jump_t, $2);
    }
  | CONTINUE SEMICOLON {
      MAKE($$, @$, stmt_jump_t, stmt_jump_t::kind_t::CONTINUE);
    }
  | BREAK SEMICOLON {
      MAKE($$, @$, stmt_jump_t, stmt_jump_t::kind_t::BREAK);
    }
  | RETURN SEMICOLON {
      MAKE($$, @$, stmt_jump_t, stmt_jump_t::kind_t::RETURN);
    }
  | RETURN expr_list SEMICOLON {
      MAKE($$, @$, stmt_jump_t, $2);
    }
  ;

stmt_decl
  : VAR decl_comma_list SEMICOLON {
      MAKE($$, @$, stmt_decl_t, $2);
    }
  ;

expr
  : expr_assign {
      $$ = $1;
    }
  ;

expr_list
  : /* empty */ {
      $$ = nullptr;
    }
  | expr {
      $$ = $1;
    }
  | expr_list COMMA expr {
      $$ = $1->push($3);
    }
  ;

expr_assign
  : expr_cond {
      $$ = $1;
    }
  | expr_prefix ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::ASSIGN, $1, $3);
    }
  | expr_prefix MUL_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::MUL_ASSIGN, $1, $3);
    }
  | expr_prefix DIV_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::DIV_ASSIGN, $1, $3);
    }
  | expr_prefix MOD_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::MOD_ASSIGN, $1, $3);
    }
  | expr_prefix ADD_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::ADD_ASSIGN, $1, $3);
    }
  | expr_prefix SUB_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::SUB_ASSIGN, $1, $3);
    }
  | expr_prefix LEFT_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::LEFT_ASSIGN, $1, $3);
    }
  | expr_prefix RIGHT_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::RIGHT_ASSIGN, $1, $3);
    }
  | expr_prefix AND_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::AND_ASSIGN, $1, $3);
    }
  | expr_prefix XOR_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::XOR_ASSIGN, $1, $3);
    }
  | expr_prefix OR_ASSIGN expr_assign {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::OR_ASSIGN, $1, $3);
    }
  ;

expr_cond
  : expr_lor {
      $$ = $1;
    }
  | expr_lor COND expr COLON expr_cond {
      MAKE($$, @$, expr_ternary_t, $1, $3, $5);
    }
  ;

expr_lor
  : expr_land {
      $$ = $1;
    }
  | expr_lor LOR expr_land {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::LOR, $1, $3);
    }
  ;

expr_land
  : expr_or {
      $$ = $1;
    }
  | expr_land LAND expr_or {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::LAND, $1, $3);
    }
  ;

expr_or
  : expr_xor {
      $$ = $1;
    }
  | expr_or OR expr_xor {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::OR, $1, $3);
    }
  ;

expr_xor
  : expr_and {
      $$ = $1;
    }
  | expr_xor XOR expr_and {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::XOR, $1, $3);
    }
  ;

expr_and
  : expr_equal {
      $$ = $1;
    }
  | expr_and AND expr_equal {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::AND, $1, $3);
    }
  ;

expr_equal
  : expr_relational {
      $$ = $1;
    }
  | expr_equal EQ expr_relational {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::EQ, $1, $3);
    }
  | expr_equal NEQ expr_relational {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::NEQ, $1, $3);
    }
  ;

expr_relational
  : expr_shift {
      $$ = $1;
    }
  | expr_relational LT expr_shift {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::LT, $1, $3);
    }
  | expr_relational GT expr_shift {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::GT, $1, $3);
    }
  | expr_relational LE expr_shift {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::LTE, $1, $3);
    }
  | expr_relational GE expr_shift {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::GTE, $1, $3);
    }
  ;

expr_shift
  : expr_add {
      $$ = $1;
    }
  | expr_shift LS expr_add {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::LEFT, $1, $3);
    }
  | expr_shift RS expr_add {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::RIGHT, $1, $3);
    }
  ;

expr_add
  : expr_mul {
      $$ = $1;
    }
  | expr_add ADD expr_mul {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::ADD, $1, $3);
    }
  | expr_add SUB expr_mul {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::SUB, $1, $3);
    }
  ;

expr_mul
  : expr_cast {
      $$ = $1;
    }
  | expr_mul MUL expr_cast {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::MUL, $1, $3);
    }
  | expr_mul DIV expr_cast {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::DIV, $1, $3);
    }
  | expr_mul MOD expr_cast {
      MAKE($$, @$, expr_dop_t, expr_t::kind_t::MOD, $1, $3);
    }
  ;

expr_cast
  : expr_prefix {
      $$ = $1;
    }
  | LPAR type_specifier RPAR expr_cast {
      MAKE($$, @$, expr_cast_t, $4, $2);
    }
  | expr_cast AS type_specifier {
      MAKE($$, @$, expr_cast_t, $1, $3);
    }
  ;

expr_prefix
  : expr_postfix {
      $$ = $1;
    }
  | INC expr_prefix {
      MAKE($$, @$, expr_prefix_t, expr_t::kind_t::INC_PRE, $2);
    }
  | DEC expr_prefix {
      MAKE($$, @$, expr_prefix_t, expr_t::kind_t::DEC_PRE, $2);
    }
  | ADD expr_prefix {
      MAKE($$, @$, expr_prefix_t, expr_t::kind_t::ADD_PRE, $2);
    }
  | SUB expr_prefix {
      MAKE($$, @$, expr_prefix_t, expr_t::kind_t::SUB_PRE, $2);
    }
  | NOT expr_prefix {
      MAKE($$, @$, expr_prefix_t, expr_t::kind_t::NOT_PRE, $2);
    }
  | TID expr_prefix {
      MAKE($$, @$, expr_prefix_t, expr_t::kind_t::TID_PRE, $2);
    }
  ;

expr_postfix
  : expr_primary {
      $$ = $1;
    }
  | expr_postfix LSQU RSQU {
      MAKE($$, @$, expr_pos_t, $1, nullptr);
    }
  | expr_postfix LSQU expr RSQU {
      MAKE($$, @$, expr_pos_t, $1, $3);
    }
  | expr_postfix LPAR expr_list RPAR {
      MAKE($$, @$, expr_call_t, $1, $3);
    }
  | expr_postfix INC {
      MAKE($$, @$, expr_postfix_t, expr_t::kind_t::INC_POST, $1);
    }
  | expr_postfix DEC {
      MAKE($$, @$, expr_postfix_t, expr_t::kind_t::DEC_POST, $1);
    }
  ;

expr_primary
  : expr_const {
      MAKE($$, @$, expr_primary_t, expr_t::kind_t::CONST, $1);
    }
  | LPAR expr_list RPAR {
      MAKE($$, @$, expr_primary_t, expr_t::kind_t::ENCLOSE, $2);
    }
  ;

expr_kvp
  : expr_cond COLON expr {
      MAKE($$, @$, expr_kvp_t, $1, $3);
    }
  ;

expr_kvp_list
  : /* empty */ {
      $$ = nullptr;
    }
  | expr_kvp {
      $$ = $1;
    }
  | expr_kvp_list COMMA expr_kvp {
      $$ = $1->push($3);
    }
  ;

expr_const
  : id {
      $$ = $1;
    }
  | const_value {
      $$ = $1;
    }
  | const_lambda {
      $$ = $1;
    }
  | const_initializer {
      $$ = $1;
    }
  ;

const_value
  : FLOAT_CONST {
      MAKE($$, @$, const_value_t, const_value_t::kind_t::FLOAT, $1);
    }
  | STRING_CONST {
      MAKE($$, @$, const_value_t, const_value_t::kind_t::STRING, $1);
    }
  | INT_CONST {
      MAKE($$, @$, const_value_t, const_value_t::kind_t::INT, $1);
    }
  ;

const_lambda
  : LPAR id_list RPAR ARROW expr {
      MAKE($$, @$, const_lambda_t, $2, $5);
    }
  | LPAR id_list RPAR ARROW stmt_compound {
      MAKE($$, @$, const_lambda_t, $2, $5);
    }
  | id ARROW expr {
      MAKE($$, @$, const_lambda_t, $1, $3);
    }
  | id ARROW stmt_compound {
      MAKE($$, @$, const_lambda_t, $1, $3);
    }
  ;

const_initializer
  : LSQU expr_list RSQU {
      MAKE($$, @$, const_initializer_t, $2);
    }
  | LBRA expr_kvp_list RBRA {
      MAKE($$, @$, const_initializer_t, $2);
    }
  ;

%%

void dyc::parser::error(const dyc::parser::location_type& l, const std::string& m) {
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