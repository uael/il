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
#include "ast.h"
#include "scanner.h"

typedef ddc::parser::token token;

#define yyterminate() return token::END
#define YY_NO_UNISTD_H
%}

%option c++
%option prefix="ddc"
%option batch
%option debug
%option yywrap nounput
%option stack

%{
#define YY_USER_ACTION  yylloc->step(); yylloc->columns(yyleng);
#define TOKEN(t) token::TOKEN_##t
#define SAVE_TOKEN yylval->_string = new std::string(yytext, yyleng)
%}

%%

:               return TOKEN(COLON);
[A-Za-z0-9_]*   SAVE_TOKEN; return TOKEN(IDENTIFIER);

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