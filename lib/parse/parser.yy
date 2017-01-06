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
#include "ast/ast.h"

#include <cstdio>
#include <cstdlib>
%}

%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {Jay}
%define parser_class_name {Parser}

%parse-param {class Driver &driver}

%locations
%define api.pure
%pure-parser

%union {
  std::string *_string;
  Jay::Ast::Identifier *_id;
  Jay::Ast::Generic *_generic;
  Jay::Ast::Closure *_closure;
  Jay::Ast::Decl *_decl;
  Jay::Ast::Decl *_decl_list;
  Jay::Ast::TypeSpecifier *_type_specifier;
  Jay::Ast::Type *_type;
  Jay::Ast::TypeInternal *_type_internal;
  Jay::Ast::TypeUserdef *_type_userdef;
  Jay::Ast::Stmt *_stmt;
  Jay::Ast::StmtExpr *_stmt_expr;
  Jay::Ast::StmtLabel *_stmt_label;
  Jay::Ast::StmtCompound *_stmt_compound;
  Jay::Ast::StmtSelect *_stmt_select;
  Jay::Ast::StmtIter *_stmt_iter;
  Jay::Ast::StmtJump *_stmt_jump;
  Jay::Ast::StmtDecl *_stmt_decl;
  Jay::Ast::Expr *_expr;
  Jay::Ast::ConstValue *_const_value;
  Jay::Ast::ConstLambda *_const_lambda;
  Jay::Ast::ConstInitializer *_const_initializer;
}

%token END 0 "end of file"
%token EOL "end of line"
%token <_string> ID USERDEF INT_CONST FLOAT_CONST STRING_CONST
%token INCLUDE USE
%token NAMESPACE TUPLE FRAME ENUM STRUCT INTERFACE CLASS
%token VOID BOOL CHAR INT STRING UINT SINT SHORT USHORT SSHORT FLOAT UFLOAT SFLOAT DOUBLE UDOUBLE SDOUBLE
%token GT LT ADD SUB MUL DIV EQ NEQ LE GE
%token COLON DOUBLE_COLON SEMICOLON COMMA LPAR RPAR LBRA RBRA ARROW ASSIGN
%token COND MOD ACCESS AND INC DEC LS LSQU RS RSQU NOT DOT TID OR XOR LAND LOR
%token ELLIPSIS RIGHT_ASSIGN LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN
%token MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token AS CASE DEFAULT IF ELSE SWITCH WHILE FOR DO GOTO CONTINUE BREAK RETURN
%token PRIVATE PROTECTED CONST VOLATILE ABSTRACT STATIC VIRTUAL FINAL INLINE
%token VAR NEW SIZEOF TYPEOF ASSERT TRY CATCH SELF THIS

%type <_id> id id_list userdef userdef_list
%type <_generic> generic generic_list generics
%type <_closure> closure closure_or_empty
%type <_decl> decl_file_item decl_frame decl_frame_item decl_use decl_var decl_dtor decl_ctor
%type <_decl_list> decl_file_body decl_frame_body frame_body decl_var_list
%type <_type_specifier> type_specifier type_specifier_list type_specifier_unit
%type <_type> type
%type <_type_internal> type_internal
%type <_type_userdef> type_userdef type_userdef_unit
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
%type <_expr> expr_unary
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
using namespace Jay::Ast;

#undef yylex
#define yylex driver.lexer->lex
#define YYERR goto yyerrlab

#define MAKE(n, l, t, ...) do { n = new t(__VA_ARGS__); n->loc = &l; } while(0)
#define DESTRUCT(n) if (n) delete n; n = nullptr
%}

%destructor { DESTRUCT($$); }
  ID USERDEF INT_CONST FLOAT_CONST STRING_CONST
  id id_list
  userdef userdef_list
  generic generic_list generics
  closure closure_or_empty
  decl_file_item decl_frame decl_frame_item decl_use decl_var decl_dtor decl_ctor decl_file_body
  decl_frame_body frame_body decl_var_list
  type_specifier type_specifier_list type_specifier_unit type type_internal type_userdef type_userdef_unit
  stmt stmt_list stmt_expr stmt_label stmt_compound stmt_select stmt_iter stmt_jump stmt_decl
  expr expr_list expr_assign expr_cond expr_lor expr_land expr_or expr_xor expr_and expr_equal expr_relational
  expr_shift expr_add expr_mul expr_cast expr_unary expr_postfix expr_primary expr_kvp expr_kvp_list expr_const
  const_value const_lambda const_initializer

%start file

%%

file
  :
    decl_file_body {
      if (driver.program.files) {
        driver.program.files->push(new File(driver.streamname, $1));
      } else {
        driver.program.files = new File(driver.streamname, $1);
      }
    }
  ;

id
  :
    ID {
      MAKE($$, @$, Identifier, $1);
    }
  ;

id_list
  :
    id {
      $$ = $1;
    }
  |
    id_list COMMA id {
      $$ = $1->push($3);
    }
  ;

userdef
  :
    USERDEF {
      MAKE($$, @$, Identifier, $1);
    }
  ;

userdef_list
  :
    userdef {
      $$ = $1;
    }
  |
    userdef_list DOT userdef {
      $$ = $1->push($3);
    }
  ;
  
generic
  :
    userdef {
      MAKE($$, @$, Generic, $1, nullptr);
    }
  |
    userdef COLON type_specifier {
      MAKE($$, @$, Generic, $1, $3);
    }
  ;
  
generic_list
  :
    generic {
      $$ = $1;
    }
  |
    generic_list COMMA generic {
      $$ = $1->push($3);
    }
  ;

generics
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    LT generic_list GT {
      $$ = $2;
    }
  ;

closure
  :
    ARROW expr {
      $$ = $2;
    }
  |
    stmt_compound {
      $$ = $1;
      $$->macro = false;
    }
  |
    ARROW stmt_compound {
      $$ = $2;
    }
  ;

closure_or_empty
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    closure {
      $$ = $1;
    }
  ;

eod
  :
    SEMICOLON
  |
    /* empty */ {
      if (
        driver.lexer->prev != token::RBRA &&
        driver.lexer->prev != token::SEMICOLON &&
        driver.lexer->current != token::RBRA &&
        driver.lexer->current != token::END) {
        YYERR;
      }
    }
  ;

decl_file_item
  :
    INCLUDE id_list {
      MAKE($$, @$, DeclInclude, $2);
    }
  |
    NAMESPACE userdef_list LBRA decl_file_body RBRA {
      MAKE($$, @$, DeclNested, $2, $4);
    }
  |
    decl_use {
      $$ = $1;
    }
  |
    decl_var {
      $$ = $1;
    }
  |
    decl_frame {
      $$ = $1;
    }
  ;

decl_file_body
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    decl_file_item eod {
      $$ = $1;
    }
  |
    decl_file_body eod decl_file_item eod {
      $$ = $1->push($3);
    }
  ;

decl_use
  :
    USE userdef_list {
      MAKE($$, @$, DeclUse, $2);
    }
  ;

decl_var
  :
    id_list ASSIGN expr_cond {
      MAKE($$, @$, DeclProperty, $1, nullptr, $3, true);
    }
  |
    id_list COLON type_specifier ASSIGN expr_cond {
      MAKE($$, @$, DeclProperty, $1, $3, $5, true);
    }
  |
    id_list COLON type_specifier closure_or_empty {
      MAKE($$, @$, DeclProperty, $1, $3, $4, false);
    }
  |
    id_list generics LPAR decl_var_list RPAR closure_or_empty {
      MAKE($$, @$, DeclFunction, $1, $2, $4, nullptr, $6);
    }
  |
    id_list generics LPAR decl_var_list RPAR COLON type_specifier_list closure_or_empty {
      MAKE($$, @$, DeclFunction, $1, $2, $4, $7, $8);
    }
  ;

decl_var_list
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    decl_var {
      $$ = $1;
    }
  |
    decl_var_list COMMA decl_var {
      $$ = $1->push($3);
    }
  ;

decl_frame
  :
    FRAME userdef generics COLON type_specifier_list decl_frame_body {
      MAKE($$, @$, DeclFrame, $2, $3, $5, $6);
    }
  ;

decl_frame_item
  :
    decl_use {
      $$ = $1;
    }
  |
    decl_var {
      $$ = $1;
    }
  |
    decl_ctor {
      $$ = $1;
    }
  |
    decl_dtor {
      $$ = $1;
    }
  |
    decl_frame {
      $$ = $1;
    }
  ;

decl_frame_body
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    LBRA frame_body RBRA {
      $$ = $2;
    }
  ;

frame_body
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    decl_frame_item eod {
      $$ = $1;
    }
  |
    frame_body eod decl_frame_item eod {
      $$ = $1->push($3);
    }
  ;

decl_ctor
  :
    SELF LPAR decl_var_list RPAR closure_or_empty {
      MAKE($$, @$, DeclCtor, $3, $5, false);
    }
  |
    STATIC LPAR decl_var_list RPAR closure_or_empty {
      MAKE($$, @$, DeclCtor, $3, $5);
    }
  |
    SELF LPAR id_list RPAR closure_or_empty {
      MAKE($$, @$, DeclCtor, $3, $5, false);
    }
  |
    STATIC LPAR id_list RPAR closure_or_empty {
      MAKE($$, @$, DeclCtor, $3, $5);
    }
  ;

decl_dtor
  :
    TID SELF LPAR decl_var_list RPAR closure_or_empty {
      MAKE($$, @$, DeclCtor, $4, $6, false);
    }
  |
    TID STATIC LPAR decl_var_list RPAR closure_or_empty {
      MAKE($$, @$, DeclCtor, $4, $6);
    }
  |
    TID SELF LPAR id_list RPAR closure_or_empty {
      MAKE($$, @$, DeclCtor, $4, $6, false);
    }
  |
    TID STATIC LPAR id_list RPAR closure_or_empty {
      MAKE($$, @$, DeclCtor, $4, $6);
    }
  ;

type_specifier
  :
    type_specifier_unit {
      $$ = $1;
    }
  |
    TUPLE LT type_specifier_list GT {
      $$ = $3;
    }
  ;

type_specifier_list
  :
    type_specifier {
      $$ = $1;
    }
  |
    type_specifier_list COMMA type_specifier {
      $$ = $1->push($3);
    }
  ;

type_specifier_unit
  :
    type {
      $$ = $1;
    }
  |
    MUL type_specifier_unit {
      MAKE($$, @$, TypePtr, $2);
    }
  |
    type_specifier_unit LSQU RSQU {
      MAKE($$, @$, TypeArray, $1);
    }
  |
    type_specifier_unit LSQU expr_cond RSQU {
      MAKE($$, @$, TypeArray, $1, $3);
    }
  |
    type_specifier_unit LPAR RPAR {
      MAKE($$, @$, type_callable_t, $1);
    }
  |
    type_specifier_unit LPAR type_specifier_list RPAR {
      MAKE($$, @$, type_callable_t, $1, $3);
    }
  ;

type
  :
    type_internal {
      $$ = $1;
    }
  |
    type_userdef {
      $$ = $1;
    }
  ;

type_internal
  :
    SELF {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::SELF);
    }
  |
    STATIC {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::STATIC);
    }
  |
    VOID {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::VOID);
    }
  |
    BOOL {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::BOOL);
    }
  |
    CHAR {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::CHAR);
    }
  |
    INT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::INT);
    }
  |
    STRING {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::STRING);
    }
  |
    UINT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::UINT);
    }
  |
    SINT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::SINT);
    }
  |
    SHORT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::SHORT);
    }
  |
    USHORT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::USHORT);
    }
  |
    SSHORT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::SSHORT);
    }
  |
    FLOAT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::FLOAT);
    }
  |
    UFLOAT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::UFLOAT);
    }
  |
    SFLOAT {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::SFLOAT);
    }
  |
    DOUBLE {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::DOUBLE);
    }
  |
    UDOUBLE {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::UDOUBLE);
    }
  |
    SDOUBLE {
      MAKE($$, @$, TypeInternal, TypeInternal::Kind::SDOUBLE);
    }
  ;

type_userdef_unit
  :
    userdef {
      MAKE($$, @$, TypeUserdef, $1);
    }
  |
    userdef LT type_specifier_list GT {
      MAKE($$, @$, TypeGeneric, $1, $3);
    }
  ;

type_userdef
  :
    type_userdef_unit {
      $$ = $1;
    }
  |
    type_userdef DOT type_userdef_unit {
      $$ = $1->push($3);
    }
  ;

stmt
  :
    stmt_expr {
      $$ = $1;
    }
  |
    stmt_label {
      $$ = $1;
    }
  |
    stmt_compound {
      $$ = $1;
    }
  |
    stmt_select {
      $$ = $1;
    }
  |
    stmt_iter {
      $$ = $1;
    }
  |
    stmt_jump {
      $$ = $1;
    }
  |
    stmt_decl {
      $$ = $1;
    }
  ;

stmt_list
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    stmt {
      $$ = $1;
    }
  |
    stmt_list stmt {
      $$ = $1->push($2);
    }
  ;

stmt_expr
  :
    SEMICOLON {
      MAKE($$, @$, StmtExpr, );
    }
  |
    expr SEMICOLON {
      MAKE($$, @$, StmtExpr, $1);
    }
  ;

stmt_label
  :
    ID COLON stmt {
      MAKE($$, @$, StmtLabel, $1, $3);
    }
  |
    CASE expr_cond COLON stmt {
      MAKE($$, @$, StmtLabel, $2, $4);
    }
  |
    DEFAULT COLON stmt {
      MAKE($$, @$, StmtLabel, $3);
    }
  ;

stmt_compound
  :
    LBRA stmt_list RBRA {
      MAKE($$, @$, StmtCompound, $2);
    }
  ;

stmt_select
  :
    IF LPAR expr RPAR stmt ELSE stmt {
      MAKE($$, @$, StmtSelect, $3, $5, $7);
    }
  |
    IF LPAR expr RPAR stmt {
      MAKE($$, @$, StmtSelect, StmtSelect::Kind::IF, $3, $5);
    }
  |
    SWITCH LPAR expr RPAR stmt {
      MAKE($$, @$, StmtSelect, StmtSelect::Kind::SWITCH, $3, $5);
    }
  ;

stmt_iter
	:
    WHILE LPAR expr RPAR stmt {
	    MAKE($$, @$, StmtIter, $3, $5);
	  }
	|
    DO stmt WHILE LPAR expr RPAR SEMICOLON {
      MAKE($$, @$, StmtIter, $2, $5);
    }
	|
    FOR LPAR stmt_expr stmt_expr RPAR stmt {
      $$ = new StmtIter(
        $3, $4 ? $4->expr : nullptr, nullptr, $6
      );
    }
	|
    FOR LPAR stmt_expr stmt_expr expr RPAR stmt {
      $$ = new StmtIter(
        $3, $4 ? $4->expr : nullptr, $5, $7
      );
    }
	|
    FOR LPAR stmt_expr stmt_expr stmt_compound RPAR stmt {
      $$ = new StmtIter(
        $3, $4 ? $4->expr : nullptr, $5, $7
      );
    }
	|
    FOR LPAR stmt_decl stmt_expr RPAR stmt {
      $$ = new StmtIter(
        $3, $4 ? $4->expr : nullptr, nullptr, $6
      );
    }
	|
    FOR LPAR stmt_decl stmt_expr expr RPAR stmt {
      $$ = new StmtIter(
        $3, $4 ? $4->expr : nullptr, $5, $7
      );
    }
	|
    FOR LPAR stmt_decl stmt_expr stmt_compound RPAR stmt {
	    $$ = new StmtIter(
	      $3, $4 ? $4->expr : nullptr, $5, $7
	    );
	  }
	;

stmt_jump
  :
    GOTO ID SEMICOLON {
      MAKE($$, @$, StmtJump, $2);
    }
  |
    CONTINUE SEMICOLON {
      MAKE($$, @$, StmtJump, StmtJump::Kind::CONTINUE);
    }
  |
    BREAK SEMICOLON {
      MAKE($$, @$, StmtJump, StmtJump::Kind::BREAK);
    }
  |
    RETURN SEMICOLON {
      MAKE($$, @$, StmtJump, StmtJump::Kind::RETURN);
    }
  |
    RETURN expr_list SEMICOLON {
      MAKE($$, @$, StmtJump, $2);
    }
  ;

stmt_decl
  :
    VAR decl_var_list SEMICOLON {
      MAKE($$, @$, StmtDecl, $2);
    }
  |
    decl_use eod {
      MAKE($$, @$, StmtDecl, $1);
    }
  |
    decl_frame eod {
      MAKE($$, @$, StmtDecl, $1);
    }
  ;

expr
  :
    expr_assign {
      $$ = $1;
    }
  ;

expr_list
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    expr {
      $$ = $1;
    }
  |
    expr_list COMMA expr {
      $$ = $1->push($3);
    }
  ;

expr_assign
  :
    expr_cond {
      $$ = $1;
    }
  |
    expr_unary ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::ASSIGN, $1, $3);
    }
  |
    expr_unary MUL_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::MUL_ASSIGN, $1, $3);
    }
  |
    expr_unary DIV_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::DIV_ASSIGN, $1, $3);
    }
  |
    expr_unary MOD_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::MOD_ASSIGN, $1, $3);
    }
  |
    expr_unary ADD_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::ADD_ASSIGN, $1, $3);
    }
  |
    expr_unary SUB_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::SUB_ASSIGN, $1, $3);
    }
  |
    expr_unary LEFT_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::LEFT_ASSIGN, $1, $3);
    }
  |
    expr_unary RIGHT_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::RIGHT_ASSIGN, $1, $3);
    }
  |
    expr_unary AND_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::AND_ASSIGN, $1, $3);
    }
  |
    expr_unary XOR_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::XOR_ASSIGN, $1, $3);
    }
  |
    expr_unary OR_ASSIGN expr_assign {
      MAKE($$, @$, ExprDop, Expr::Kind::OR_ASSIGN, $1, $3);
    }
  ;

expr_cond
  :
    expr_lor {
      $$ = $1;
    }
  |
    expr_lor COND expr COLON expr_cond {
      MAKE($$, @$, ExprTernary, $1, $3, $5);
    }
  ;

expr_lor
  :
    expr_land {
      $$ = $1;
    }
  |
    expr_lor LOR expr_land {
      MAKE($$, @$, ExprDop, Expr::Kind::LOR, $1, $3);
    }
  ;

expr_land
  :
    expr_or {
      $$ = $1;
    }
  |
    expr_land LAND expr_or {
      MAKE($$, @$, ExprDop, Expr::Kind::LAND, $1, $3);
    }
  ;

expr_or
  :
    expr_xor {
      $$ = $1;
    }
  |
    expr_or OR expr_xor {
      MAKE($$, @$, ExprDop, Expr::Kind::OR, $1, $3);
    }
  ;

expr_xor
  :
    expr_and {
      $$ = $1;
    }
  |
    expr_xor XOR expr_and {
      MAKE($$, @$, ExprDop, Expr::Kind::XOR, $1, $3);
    }
  ;

expr_and
  :
    expr_equal {
      $$ = $1;
    }
  |
    expr_and AND expr_equal {
      MAKE($$, @$, ExprDop, Expr::Kind::AND, $1, $3);
    }
  ;

expr_equal
  :
    expr_relational {
      $$ = $1;
    }
  |
    expr_equal EQ expr_relational {
      MAKE($$, @$, ExprDop, Expr::Kind::EQ, $1, $3);
    }
  |
    expr_equal NEQ expr_relational {
      MAKE($$, @$, ExprDop, Expr::Kind::NEQ, $1, $3);
    }
  ;

expr_relational
  :
    expr_shift {
      $$ = $1;
    }
  |
    expr_relational LT expr_shift {
      MAKE($$, @$, ExprDop, Expr::Kind::LT, $1, $3);
    }
  |
    expr_relational GT expr_shift {
      MAKE($$, @$, ExprDop, Expr::Kind::GT, $1, $3);
    }
  |
    expr_relational LE expr_shift {
      MAKE($$, @$, ExprDop, Expr::Kind::LTE, $1, $3);
    }
  |
    expr_relational GE expr_shift {
      MAKE($$, @$, ExprDop, Expr::Kind::GTE, $1, $3);
    }
  ;

expr_shift
  :
    expr_add {
      $$ = $1;
    }
  |
    expr_shift LS expr_add {
      MAKE($$, @$, ExprDop, Expr::Kind::LEFT, $1, $3);
    }
  |
    expr_shift RS expr_add {
      MAKE($$, @$, ExprDop, Expr::Kind::RIGHT, $1, $3);
    }
  ;

expr_add
  :
    expr_mul {
      $$ = $1;
    }
  |
    expr_add ADD expr_mul {
      MAKE($$, @$, ExprDop, Expr::Kind::ADD, $1, $3);
    }
  |
    expr_add SUB expr_mul {
      MAKE($$, @$, ExprDop, Expr::Kind::SUB, $1, $3);
    }
  ;

expr_mul
  :
    expr_cast {
      $$ = $1;
    }
  |
    expr_mul MUL expr_cast {
      MAKE($$, @$, ExprDop, Expr::Kind::MUL, $1, $3);
    }
  |
    expr_mul DIV expr_cast {
      MAKE($$, @$, ExprDop, Expr::Kind::DIV, $1, $3);
    }
  |
    expr_mul MOD expr_cast {
      MAKE($$, @$, ExprDop, Expr::Kind::MOD, $1, $3);
    }
  ;

expr_cast
  :
    expr_unary {
      $$ = $1;
    }
  |
    LPAR type_specifier RPAR expr_cast {
      MAKE($$, @$, ExprCast, $4, $2);
    }
  |
    expr_cast AS type_specifier {
      MAKE($$, @$, ExprCast, $1, $3);
    }
  ;

expr_unary
  :
    expr_postfix {
      $$ = $1;
    }
  |
    SIZEOF expr_unary {
      MAKE($$, @$, ExprSizeof, $2);
    }
  |
    SIZEOF LPAR type_specifier_unit RPAR {
      MAKE($$, @$, ExprSizeof, $3);
    }
  |
    INC expr_unary {
      MAKE($$, @$, ExprUnary, Expr::Kind::INC_PRE, $2);
    }
  |
    DEC expr_unary {
      MAKE($$, @$, ExprUnary, Expr::Kind::DEC_PRE, $2);
    }
  |
    MUL expr_cast {
      MAKE($$, @$, ExprUnary, Expr::Kind::MUL_PRE, $2);
    }
  |
    AND expr_cast {
      MAKE($$, @$, ExprUnary, Expr::Kind::AND_PRE, $2);
    }
  |
    ADD expr_cast {
      MAKE($$, @$, ExprUnary, Expr::Kind::ADD_PRE, $2);
    }
  |
    SUB expr_cast {
      MAKE($$, @$, ExprUnary, Expr::Kind::SUB_PRE, $2);
    }
  |
    NOT expr_cast {
      MAKE($$, @$, ExprUnary, Expr::Kind::NOT_PRE, $2);
    }
  |
    TID expr_cast {
      MAKE($$, @$, ExprUnary, Expr::Kind::TID_PRE, $2);
    }
  ;

expr_postfix
  :
    expr_primary {
      $$ = $1;
    }
  |
    expr_postfix DOT id {
      MAKE($$, @$, ExprNested, $1, $3);
    }
  |
    expr_postfix LSQU RSQU {
      MAKE($$, @$, ExprPos, $1, nullptr);
    }
  |
    expr_postfix LSQU expr RSQU {
      MAKE($$, @$, ExprPos, $1, $3);
    }
  |
    expr_postfix LPAR expr_list RPAR {
      MAKE($$, @$, ExprCall, $1, $3);
    }
  |
    expr_postfix INC {
      MAKE($$, @$, ExprPostfix, Expr::Kind::INC_POST, $1);
    }
  |
    expr_postfix DEC {
      MAKE($$, @$, ExprPostfix, Expr::Kind::DEC_POST, $1);
    }
  ;

expr_primary
  :
    expr_const {
      MAKE($$, @$, ExprPrimary, Expr::Kind::CONST, $1);
    }
  |
    LPAR expr_list RPAR {
      MAKE($$, @$, ExprPrimary, Expr::Kind::ENCLOSE, $2);
    }
  ;

expr_kvp
  :
    expr_cond COLON expr {
      MAKE($$, @$, ExprKvp, $1, $3);
    }
  ;

expr_kvp_list
  :
    /* empty */ {
      $$ = nullptr;
    }
  |
    expr_kvp {
      $$ = $1;
    }
  |
    expr_kvp_list COMMA expr_kvp {
      $$ = $1->push($3);
    }
  ;

expr_const
  :
    id {
      $$ = $1;
    }
  |
    userdef_list {
      $$ = $1;
    }
  |
    THIS {
      MAKE($$, @$, ConstThis);
    }
  |
    const_value {
      $$ = $1;
    }
  |
    const_lambda {
      $$ = $1;
    }
  |
    const_initializer {
      $$ = $1;
    }
  ;

const_value
  :
    FLOAT_CONST {
      MAKE($$, @$, ConstValue, ConstValue::Kind::FLOAT, $1);
    }
  |
    STRING_CONST {
      MAKE($$, @$, ConstValue, ConstValue::Kind::STRING, $1);
    }
  |
    INT_CONST {
      MAKE($$, @$, ConstValue, ConstValue::Kind::INT, $1);
    }
  ;

const_lambda
  :
    LPAR id_list RPAR closure {
      MAKE($$, @$, ConstLambda, $2, $4);
    }
  |
    id closure {
      MAKE($$, @$, ConstLambda, $1, $2);
    }
  ;

const_initializer
  :
    NEW type_userdef {
      MAKE($$, @$, ConstNew, $2);
    }
  |
    NEW type_userdef LPAR expr_list RPAR {
      MAKE($$, @$, ConstNew, $2, $4);
    }
  |
    LSQU expr_list RSQU {
      MAKE($$, @$, ConstInitializer, $2);
    }
  |
    LBRA expr_kvp_list RBRA {
      MAKE($$, @$, ConstInitializer, $2);
    }
  ;

%%

void Jay::Parser::error(const Jay::Parser::location_type& l, const std::string& m) {
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