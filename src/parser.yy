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
  id_t *_id;
  ids_t *_ids;

  modifier_t _modifier;
  op_t _op;
  cmp_t _cmp;

  node_t *_node;
  nodes_t *_nodes;

  stmt_t *_stmt;
  stmts_t *_stmts;

  expr_t *_expr;
  exprs_t *_exprs;

  decl_t *_decl;
  decls_t *_decls;

  sign_t *_sign;
  signs_t *_signs;

  ival_t *_val;
  vals_t *_vals;

  scalar_t *_scalar;
  scalars_t *_scalars;
  scalar_decl_t *_scalar_decl;
  scalar_decls_t *_scalar_decls;

  class_t *_class;
  classs_t *_classs;
  class_decl_t *_class_decl;
  class_decls_t *_class_decls;
  
  iface_t *_iface;
  ifaces_t *_ifaces;
  iface_decl_t *_iface_decl;
  iface_decls_t *_iface_decls;
  
  ivar_t *_var;
  vars_t *_vars;
  ivar_decl_t *_var_decl;
  var_decls_t *_var_decls;
  
  iconst_t *_const;
  consts_t *_consts;
  iconst_decl_t *_const_decl;
  const_decls_t *_const_decls;
  
  iarg_t *_arg;
  args_t *_args;
  iarg_decl_t *_arg_decl;
  arg_decls_t *_arg_decls;
  
  ifunc_t *_func;
  funcs_t *_funcs;
  ifunc_decl_t *_func_decl;
  func_decls_t *_func_decls;
  
  imeth_t *_meth;
  meths_t *_meths;
  imeth_decl_t *_meth_decl;
  meth_decls_t *_meth_decls;
  
  iprop_t *_prop;
  props_t *_props;
  iprop_decl_t *_prop_decl;
  prop_decls_t *_prop_decls;

  program_t *_program;
}

%token END 0 "end of file"
%token EOL "end of line"
%token <_id> ID KCLASS KINT
%token <_modifier> PUBLIC PRIVATE PROTECTED
%token <_op> ADD SUB MUL DIV
%token <_cmp> EQ NEQ LT LE GT GE
%token <_val> INT
%token COLON SEMICOLON COMMA LPAR RPAR LBRA RBRA INLINE ASSIGN

%type <_id> id
%type <_ids> ids
%type <_modifier> modifier
%type <_op> op
%type <_cmp> cmp
%type <_node> node
%type <_nodes> nodes
%type <_stmt> stmt
%type <_stmts> stmts
%type <_expr> expr
%type <_exprs> exprs
%type <_decl> decl
%type <_decls> decls
%type <_sign> sign
%type <_signs> signs
%type <_val> val
%type <_vals> vals
%type <_scalar> scalar
%type <_scalars> scalars
%type <_scalar_decl> scalar_decl
%type <_scalar_decls> scalar_decls
%type <_class> class
%type <_classs> classs
%type <_class_decl> class_decl
%type <_class_decls> class_decls
%type <_iface> iface
%type <_ifaces> ifaces
%type <_iface_decl> iface_decl
%type <_iface_decls> iface_decls
%type <_var> var
%type <_vars> vars
%type <_var_decl> var_decl
%type <_var_decls> var_decls
%type <_const> const
%type <_consts> consts
%type <_const_decl> const_decl
%type <_const_decls> const_decls
%type <_arg> arg
%type <_args> args
%type <_arg_decl> arg_decl
%type <_arg_decls> arg_decls
%type <_func> func
%type <_funcs> funcs
%type <_func_decl> func_decl
%type <_func_decls> func_decls
%type <_meth> meth
%type <_meths> meths
%type <_meth_decl> meth_decl
%type <_meth_decls> meth_decls
%type <_prop> prop
%type <_props> props
%type <_prop_decl> prop_decl
%type <_prop_decls> prop_decls
%type <_program> program

%destructor { delete $$; } ID KCLASS KINT INT

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
    : func_decls class_decls
        { MAKE($$, program, *$1, *$2); }
    ;

class_decls
    : /* empty */
        { MAKE($$, class_decls); }
    | class
        { MAKE($$, class_decls); PUSH($$, *$1); }
    | class_decls class
        { PUSH($1, *$2); $$ = $1; }
    ;

func_decls
    : /* empty */
        { MAKE($$, func_decls); }
    | func
        { MAKE($$, func_decls); PUSH($$, *$1); }
    | funcs func
        { PUSH($1, *$2); $$ = $1; }
    ;

class_decl
    : KCLASS ID LBRA consts props funcs RBRA
        { MAKE($$, class, *$2, *$3); }
    ;

func_decl
    : func_pt closure
        { MAKE($$, func, $1->id, $2->sign, *$3); }
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