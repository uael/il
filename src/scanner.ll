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
#include "scanner.h"

using namespace ddc::ast;

typedef ddc::parser::token token;

#define STEP yylloc->step()
#define YY_USER_ACTION STEP; yylloc->columns(yyleng);
#define T(t) token::t
#define RET(t) return T(t)
#define SAVE_ID yylval->_id = new string(yytext, yyleng)
#define SAVE_RET_MODIFIER(t) yylval->_modifier = modifier_t::t; RET(t)
#define SAVE_RET_SCALAR(t) yylval->_value = new val_t<scalar_t>(&scalar::t, new string(yytext, yyleng)); RET(t)
#define SAVE_RET_OP(t) yylval->_op = op_t::t; RET(t)
#define SAVE_RET_CMP(t) yylval->_cmp = cmp_t::t; RET(t)
#define yyterminate() return T(END)
#define YY_NO_UNISTD_H
%}

%option c++
%option prefix="ddc"
%option batch
%option debug
%option noyywrap
%option yywrap nounput
%option stack

%%

":"             RET(COLON);
";"             RET(SEMICOLON);
","             RET(COMMA);
"("             RET(LPAR);
")"             RET(RPAR);
"{"             RET(LBRA);
"}"             RET(RBRA);
"=>"            RET(INLINE);
"="             RET(ASSIGN);
"+"             SAVE_RET_OP(ADD);
"-"             SAVE_RET_OP(SUB);
"*"             SAVE_RET_OP(MUL);
"/"             SAVE_RET_OP(DIV);
"=="            SAVE_RET_CMP(EQ);
"!="            SAVE_RET_CMP(NEQ);
"<"             SAVE_RET_CMP(LT);
"<="            SAVE_RET_CMP(LE);
">"             SAVE_RET_CMP(GT);
">="            SAVE_RET_CMP(GE);
"class"         RET(KCLASS);
"public"        SAVE_RET_MODIFIER(PUBLIC);
"private"       SAVE_RET_MODIFIER(PRIVATE);
"protected"     SAVE_RET_MODIFIER(PROTECTED);
"int"           RET(KINT);
[A-Za-z0-9_]*   SAVE_ID; RET(ID);
[0-9]+          SAVE_RET_SCALAR(INT);
[ \t\r]+        STEP;
[\n]+           STEP;

%%

namespace ddc {
    scanner::scanner(std::istream* in, std::ostream* out)
        : yyFlexLexer(in, out) { }

    scanner::~scanner() { }

    void scanner::set_debug(bool b) {
        yy_flex_debug = b;
    }
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
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */