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
  dyc::ast::expr_const_t *_expr_const;
  dyc::ast::expr_primary_t *_expr_primary;
  dyc::ast::expr_postfix_t *_expr_postfix;
  dyc::ast::expr_prefix_t *_expr_prefix;
  dyc::ast::expr_cast_t *_expr_cast;
  dyc::ast::expr_mul_t *_expr_mul;
  dyc::ast::expr_add_t *_expr_add;
  dyc::ast::expr_shift_t *_expr_shift;
  dyc::ast::expr_relational_t *_expr_relational;
  dyc::ast::expr_equal_t *_expr_equal;
  dyc::ast::expr_and_t *_expr_and;
  dyc::ast::expr_xor_t *_expr_xor;
  dyc::ast::expr_or_t *_expr_or;
  dyc::ast::expr_land_t *_expr_land;
  dyc::ast::expr_lor_t *_expr_lor;
  dyc::ast::expr_cond_t *_expr_cond;
  dyc::ast::expr_assign_t *_expr_assign;

  dyc::ast::const_value_t *_const_value;
  dyc::ast::const_lambda_t *_const_lambda;
  dyc::ast::const_initializer_t *_const_initializer;

  dyc::ast::ds_map_t *_ds_map;
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
%type <_expr_assign> expr_assign
%type <_expr_cond> expr_cond
%type <_expr_lor> expr_lor
%type <_expr_land> expr_land
%type <_expr_or> expr_or
%type <_expr_xor> expr_xor
%type <_expr_and> expr_and
%type <_expr_equal> expr_equal
%type <_expr_relational> expr_relational
%type <_expr_shift> expr_shift
%type <_expr_add> expr_add
%type <_expr_mul> expr_mul
%type <_expr_cast> expr_cast
%type <_expr_prefix> expr_prefix
%type <_expr_postfix> expr_postfix
%type <_expr_primary> expr_primary
%type <_expr_const> expr_const

%type <_const_value> const_value
%type <_const_lambda> const_lambda
%type <_const_initializer> const_initializer

%type <_ds_map> ds_map

%{
#include "driver.h"
#include "scanner.h"

using namespace std;
using namespace dyc::ast;

#undef yylex
#define yylex driver.lexer->lex
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

%destructor { if ($$) delete $$; $$ = nullptr; } ds_map

%start program

%%

program
  : decl_list {
      driver.ast = dyc::ast_t($1);
    }
  ;

id
  : ID {
      $$ = new identifier_t($1);
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
      $$ = new generic_t($1, nullptr);
    }
  | GENERIC COLON type_specifier {
      $$ = new generic_t($1, $3);
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
      $$ = new decl_property_t($1, $3, nullptr, false);
    }
  | id_list ASSIGN expr_cond {
      $$ = new decl_property_t($1, nullptr, $3, true);
    }
  | id_list COLON type_specifier ASSIGN expr_cond {
      $$ = new decl_property_t($1, $3, $5, true);
    }
  | id_list COLON type_specifier ARROW expr_cond {
      $$ = new decl_property_t($1, $3, $5, false);
    }
  ;

decl_property_compound
  : id_list ARROW stmt_compound {
      $$ = new decl_property_t($1, nullptr, $3, false);
    }
  | id_list COLON type_specifier_list ARROW stmt_compound {
      $$ = new decl_property_t($1, $3, $5, false);
    }
  ;

decl_function_expr
  : id_list generics decl_args ARROW expr_cond {
      $$ = new decl_function_t($1, $2, $3, nullptr, $5);
    }
  | id_list generics decl_args COLON type_specifier_list ARROW expr_cond {
      $$ = new decl_function_t($1, $2, $3, $5, $7);
    }
  ;

decl_function_compound
  : id_list generics decl_args ARROW stmt_compound {
      $$ = new decl_function_t($1, $2, $3, nullptr, $5);
    }
  | id_list generics decl_args COLON type_specifier_list ARROW stmt_compound {
      $$ = new decl_function_t($1, $2, $3, $5, $7);
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
      $$ = new type_specifier_t($1);
    }
  | MUL type_specifier_unit {
      $2->ptr_lvl++;
      $$ = $2;
    }
  | type_specifier_unit LSQU RSQU {
      $1->array_lvl++;
      $$ = $1;
    }
  | type_specifier_unit LPAR type_specifier_list RPAR {
      $1->call_chain.push_back($3);
      $$ = $1;
    }
  | type_specifier_unit LPAR RPAR {
      $1->call_chain.push_back(nullptr);
      $$ = $1;
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
      $$ = new type_scalar_t(type_scalar_t::kind_t::VOID);
    }
  | BOOL {
      $$ = new type_scalar_t(type_scalar_t::kind_t::BOOL);
    }
  | CHAR {
      $$ = new type_scalar_t(type_scalar_t::kind_t::CHAR);
    }
  | INT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::INT);
    }
  | UINT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::UINT);
    }
  | SINT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::SINT);
    }
  | SHORT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::SHORT);
    }
  | USHORT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::USHORT);
    }
  | SSHORT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::SSHORT);
    }
  | FLOAT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::FLOAT);
    }
  | UFLOAT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::UFLOAT);
    }
  | SFLOAT {
      $$ = new type_scalar_t(type_scalar_t::kind_t::SFLOAT);
    }
  | DOUBLE {
      $$ = new type_scalar_t(type_scalar_t::kind_t::DOUBLE);
    }
  | UDOUBLE {
      $$ = new type_scalar_t(type_scalar_t::kind_t::UDOUBLE);
    }
  | SDOUBLE {
      $$ = new type_scalar_t(type_scalar_t::kind_t::SDOUBLE);
    }
  ;

type_generic
  : GENERIC {
      $$ = new type_generic_t($1);
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
      $$ = new stmt_expr_t();
    }
  | expr SEMICOLON {
      $$ = new stmt_expr_t($1);
    }
  ;

stmt_label
  : ID COLON stmt {
      $$ = new stmt_label_t($1, $3);
    }
  | CASE expr_cond COLON stmt {
      $$ = new stmt_label_t($2, $4);
    }
  | DEFAULT COLON stmt {
      $$ = new stmt_label_t($3);
    }
  ;

stmt_compound
  : LBRA stmt_list RBRA {
      $$ = new stmt_compound_t($2);
    }
  ;

stmt_select
  : IF LPAR expr RPAR stmt ELSE stmt {
      $$ = new stmt_select_t($3, $5, $7);
    }
  | IF LPAR expr RPAR stmt {
      $$ = new stmt_select_t(stmt_select_t::kind_t::IF, $3, $5);
    }
  | SWITCH LPAR expr RPAR stmt {
      $$ = new stmt_select_t(stmt_select_t::kind_t::SWITCH, $3, $5);
    }
  ;

stmt_iter
	: WHILE LPAR expr RPAR stmt {
	    $$ = new stmt_iter_t($3, $5);
	  }
	| DO stmt WHILE LPAR expr RPAR SEMICOLON {
      $$ = new stmt_iter_t($2, $5);
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
      $$ = new stmt_jump_t($2);
    }
  | CONTINUE SEMICOLON {
      $$ = new stmt_jump_t(stmt_jump_t::kind_t::CONTINUE);
    }
  | BREAK SEMICOLON {
      $$ = new stmt_jump_t(stmt_jump_t::kind_t::BREAK);
    }
  | RETURN SEMICOLON {
      $$ = new stmt_jump_t(stmt_jump_t::kind_t::RETURN);
    }
  | RETURN expr_list SEMICOLON {
      $$ = new stmt_jump_t($2);
    }
  ;

stmt_decl
  : VAR decl_comma_list SEMICOLON {
      $$ = new stmt_decl_t($2);
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
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::NONE, $1));
      $$ = $3;
    }
  | expr_prefix MUL_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::MUL, $1));
      $$ = $3;
    }
  | expr_prefix DIV_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::DIV, $1));
      $$ = $3;
    }
  | expr_prefix MOD_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::MOD, $1));
      $$ = $3;
    }
  | expr_prefix ADD_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::ADD, $1));
      $$ = $3;
    }
  | expr_prefix SUB_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::SUB, $1));
      $$ = $3;
    }
  | expr_prefix LEFT_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::LEFT, $1));
      $$ = $3;
    }
  | expr_prefix RIGHT_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::RIGHT, $1));
      $$ = $3;
    }
  | expr_prefix AND_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::AND, $1));
      $$ = $3;
    }
  | expr_prefix XOR_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::XOR, $1));
      $$ = $3;
    }
  | expr_prefix OR_ASSIGN expr_assign {
      $3->assign_chain.push_back(tuple<expr_assign_t::kind_t, expr_prefix_t *>(expr_assign_t::kind_t::OR, $1));
      $$ = $3;
    }
  ;

expr_cond
  : expr_lor {
      $$ = $1;
    }
  | expr_lor COND expr COLON expr_cond {
      $5->ternary_chain.push_back(tuple<expr_lor_t *, expr_t *>($1, $3));
      $$ = $5;
    }
  ;

expr_lor
  : expr_land {
      $$ = $1;
    }
  | expr_lor LOR expr_land {
      $1->lor_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_land
  : expr_or {
      $$ = $1;
    }
  | expr_land LAND expr_or {
      $1->land_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_or
  : expr_xor {
      $$ = $1;
    }
  | expr_or OR expr_xor {
      $1->or_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_xor
  : expr_and {
      $$ = $1;
    }
  | expr_xor XOR expr_and {
      $1->xor_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_and
  : expr_equal {
      $$ = $1;
    }
  | expr_and AND expr_equal {
      $1->and_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_equal
  : expr_relational {
      $$ = $1;
    }
  | expr_equal EQ expr_relational {
      $1->eq_chain.push_back($3);
      $$ = $1;
    }
  | expr_equal NEQ expr_relational {
      $1->neq_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_relational
  : expr_shift {
      $$ = $1;
    }
  | expr_relational LT expr_shift {
      $1->lt_chain.push_back($3);
      $$ = $1;
    }
  | expr_relational GT expr_shift {
      $1->gt_chain.push_back($3);
      $$ = $1;
    }
  | expr_relational LE expr_shift {
      $1->le_chain.push_back($3);
      $$ = $1;
    }
  | expr_relational GE expr_shift {
      $1->ge_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_shift
  : expr_add {
      $$ = $1;
    }
  | expr_shift LS expr_add {
      $1->ls_chain.push_back($3);
      $$ = $1;
    }
  | expr_shift RS expr_add {
      $1->rs_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_add
  : expr_mul {
      $$ = $1;
    }
  | expr_add ADD expr_mul {
      $1->add_chain.push_back($3);
      $$ = $1;
    }
  | expr_add SUB expr_mul {
      $1->sub_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_mul
  : expr_cast {
      $$ = $1;
    }
  | expr_mul MUL expr_cast {
      $1->mul_chain.push_back($3);
      $$ = $1;
    }
  | expr_mul DIV expr_cast {
      $1->div_chain.push_back($3);
      $$ = $1;
    }
  | expr_mul MOD expr_cast {
      $1->mod_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_cast
  : expr_prefix {
      $$ = $1;
    }
  | LPAR type_specifier RPAR expr_cast {
      $4->cast_chain.push_back($2);
      $$ = $4;
    }
  | expr_cast AS type_specifier {
      $1->cast_chain.push_back($3);
      $$ = $1;
    }
  ;

expr_prefix
  : expr_postfix {
      $$ = $1;
    }
  | INC expr_prefix {
      $2->inc_lvl++;
      $$ = $2;
    }
  | DEC expr_prefix {
      $2->dec_lvl++;
      $$ = $2;
    }
  | AND expr_prefix {
      $2->and_lvl++;
      $$ = $2;
    }
  | ADD expr_prefix {
      $2->add_lvl++;
      $$ = $2;
    }
  | SUB expr_prefix {
      $2->sub_lvl++;
      $$ = $2;
    }
  | MUL expr_prefix {
      $2->mul_lvl++;
      $$ = $2;
    }
  | NOT expr_prefix {
      $2->not_lvl++;
      $$ = $2;
    }
  | TID expr_prefix {
      $2->tid_lvl++;
      $$ = $2;
    }
  ;

expr_postfix
  : expr_primary {
      $$ = $1;
    }
  | expr_postfix LSQU RSQU {
      $1->position_chain.push_back(nullptr);
      $$ = $1;
    }
  | expr_postfix LSQU expr RSQU {
      $1->position_chain.push_back($3);
      $$ = $1;
    }
  | expr_postfix LPAR expr_list RPAR {
      $1->call_chain.push_back($3);
      $$ = $1;
    }
  | expr_postfix INC {
      $1->post_inc_lvl++;
      $$ = $1;
    }
  | expr_postfix DEC {
      $1->post_dec_lvl++;
      $$ = $1;
    }
  ;

expr_primary
  : expr_const {
      $$ = new expr_primary_t($1);
    }
  | LPAR expr_list RPAR {
      $$ = new expr_primary_t($2);
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
      $$ = new const_value_t(const_value_t::kind_t::FLOAT, $1);
    }
  | STRING_CONST {
      $$ = new const_value_t(const_value_t::kind_t::STRING, $1);
    }
  | INT_CONST {
      $$ = new const_value_t(const_value_t::kind_t::INT, $1);
    }
  ;

const_lambda
  : LPAR id_list RPAR ARROW expr {
      $$ = new const_lambda_t($2, $5);
    }
  | LPAR id_list RPAR ARROW stmt_compound {
      $$ = new const_lambda_t($2, $5);
    }
  | id ARROW expr {
      $$ = new const_lambda_t($1, $3);
    }
  | id ARROW stmt_compound {
      $$ = new const_lambda_t($1, $3);
    }
  ;

const_initializer
  : LSQU expr_list RSQU {
      $$ = new const_initializer_t($2);
    }
  | LBRA ds_map RBRA {
      $$ = new const_initializer_t($2);
    }
  ;

ds_map
  : /* empty */ {
      $$ = nullptr;
    }
  | expr_cond COLON expr {
      $$ = new ds_map_t();
      (*$$)[$1] = $3;
    }
  | ds_map COMMA expr_cond COLON expr {
      (*$1)[$3] = $5;
      $$ = $1;
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