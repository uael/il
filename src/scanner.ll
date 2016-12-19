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

%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%{
#include "dyc.h"
#include "y.tab.h"

using namespace std;
using namespace dyc::ast;

typedef dyc::parser::token token;

#define STEP yylloc->step()
#define YY_USER_ACTION STEP; yylloc->columns(yyleng);
#define T(t) token::t
#define RET(t) return T(t)
#define SAVE_STRING yylval->_string = new string(yytext, yyleng)
#define yyterminate() return T(END)
#define YY_NO_UNISTD_H
%}

%option c++
%option prefix="dyc"
%option batch
%option debug
%option noyywrap
%option yywrap nounput
%option stack

%%

"..."					                    RET(ELLIPSIS);

">>="					                    RET(RIGHT_ASSIGN);
"<<="					                    RET(LEFT_ASSIGN);
"+="					                    RET(ADD_ASSIGN);
"-="					                    RET(SUB_ASSIGN);
"*="					                    RET(MUL_ASSIGN);
"/="					                    RET(DIV_ASSIGN);
"%="					                    RET(MOD_ASSIGN);
"&="					                    RET(AND_ASSIGN);
"^="					                    RET(XOR_ASSIGN);
"|="					                    RET(OR_ASSIGN);

"."                               RET(DOT);
"->"                              RET(ACCESS);
"!"                               RET(NOT);
"?"                               RET(COND);
":"                               RET(COLON);
"::"                              RET(DOUBLE_COLON);
";"                               RET(SEMICOLON);
","                               RET(COMMA);
"("                               RET(LPAR);
")"                               RET(RPAR);
"["                               RET(LSQU);
"]"                               RET(RSQU);
"{"                               RET(LBRA);
"}"                               RET(RBRA);
"=>"                              RET(ARROW);

"="                               RET(ASSIGN);
"~"                               RET(TID);
"&"                               RET(AND);
"^"                               RET(XOR);
"|"                               RET(OR);
"+"                               RET(ADD);
"-"                               RET(SUB);
"*"                               RET(MUL);
"/"                               RET(DIV);
"%"                               RET(MOD);

"&&"                              RET(LAND);
"||"                              RET(LOR);
"=="                              RET(EQ);
"!="                              RET(NEQ);
"<"                               RET(LT);
"<="                              RET(LE);
">"                               RET(GT);
">="                              RET(GE);
"<<"                              RET(LS);
">>"                              RET(RS);

"++"                              RET(INC);
"--"                              RET(DEC);

"include"                         RET(INCLUDE);
"use"                             RET(USE);

"namespace"                       RET(NAMESPACE);
"frame"                           RET(FRAME);
"tuple"                           RET(TUPLE);
"enum"                            RET(ENUM);
"struct"                          RET(STRUCT);
"class"                           RET(CLASS);
"interface"                       RET(INTERFACE);

"private"                         RET(PRIVATE);
"protected"                       RET(PROTECTED);
"const"                           RET(CONST);
"volatile"                        RET(VOLATILE);
"abstract"                        RET(ABSTRACT);
"static"                          RET(STATIC);
"virtual"                         RET(VIRTUAL);
"final"                           RET(FINAL);
"inline"                          RET(INLINE);

"var"                             RET(VAR);
"new"                             RET(NEW);
"sizeof"                          RET(SIZEOF);
"typeof"                          RET(TYPEOF);
"assert"                          RET(ASSERT);
"try"                             RET(TRY);
"catch"                           RET(CATCH);
"self"                            RET(SELF);
"this"                            RET(THIS);

"as"                              RET(AS);
"case"                            RET(CASE);
"default"                         RET(DEFAULT);
"if"                              RET(IF);
"else"                            RET(ELSE);
"switch"                          RET(SWITCH);
"while"                           RET(WHILE);
"for"                             RET(FOR);
"do"                              RET(DO);
"goto"                            RET(GOTO);
"continue"                        RET(CONTINUE);
"break"                           RET(BREAK);
"return"                          RET(RETURN);

"void"                            RET(VOID);
"bool"                            RET(BOOL);
"char"                            RET(CHAR);
"int"                             RET(INT);
"string"                          RET(STRING);
"unsigned int"                    RET(UINT);
"signed int"                      RET(SINT);
"short"                           RET(SHORT);
"unsigned short"                  RET(USHORT);
"signed short"                    RET(SSHORT);
"float"                           RET(FLOAT);
"unsigned float"                  RET(UFLOAT);
"signed float"                    RET(SFLOAT);
"double"                          RET(DOUBLE);
"unsigned double"                 RET(UDOUBLE);
"signed double"                   RET(SDOUBLE);

[a-z_][a-z0-9_]*	                SAVE_STRING; RET(ID);
[A-Z][A-Za-z0-9]*                 SAVE_STRING; RET(USERDEF);

{HP}{H}+{IS}?				              SAVE_STRING; RET(INT_CONST);
{NZ}{D}*{IS}?				              SAVE_STRING; RET(INT_CONST);
"0"{O}*{IS}?				              SAVE_STRING; RET(INT_CONST);
{CP}?"'"([^'\\\n]|{ES})+"'"       SAVE_STRING; RET(INT_CONST);
{D}+{E}{FS}?				              SAVE_STRING; RET(FLOAT_CONST);
{D}*"."{D}+{E}?{FS}?			        SAVE_STRING; RET(FLOAT_CONST);
{D}+"."{E}?{FS}?			            SAVE_STRING; RET(FLOAT_CONST);
{HP}{H}+{P}{FS}?			            SAVE_STRING; RET(FLOAT_CONST);
{HP}{H}*"."{H}+{P}{FS}?			      SAVE_STRING; RET(FLOAT_CONST);
{HP}{H}+"."{P}{FS}?			          SAVE_STRING; RET(FLOAT_CONST);
({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+ SAVE_STRING; RET(STRING_CONST);

{WS}+                             STEP;

%%

namespace dyc {
  scanner::scanner(std::istream* in, std::ostream* out) : yyFlexLexer(in, out) { }
  scanner::~scanner() { }
  void scanner::set_debug(bool b) { yy_flex_debug = b; }
}

#ifdef yylex
#undef yylex
#endif

int yyFlexLexer::yylex() {
  std::cerr << "in yyFlexLexer::yylex() !" << std::endl;
  return 0;
}

int yyFlexLexer::yywrap() {
  return 1;
}

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */