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

#ifndef _SCANNER_H
#define _SCANNER_H

#include "dyc.h"
using namespace dyc::ast;

#ifndef YY_DECL
#define  YY_DECL            \
    dyc::parser::token_type        \
    dyc::scanner::lex(        \
  dyc::parser::semantic_type* yylval,    \
  dyc::parser::location_type* yylloc    \
    )
#endif

#ifndef __FLEX_LEXER_H
#  define yyFlexLexer dycFlexLexer
#  include "FlexLexer.h"
#  undef yyFlexLexer
#endif

#if __cplusplus > 199711L
#  define register
#endif

#ifdef BISON_USE_PARSER_H_EXTENSION
#  include "parser.h"
#else
#  include "parser.hh"
#endif

namespace dyc {

  class scanner : public dycFlexLexer {
  public:
    scanner(std::istream *arg_yyin = 0, std::ostream *arg_yyout = 0);

    virtual ~scanner();
    virtual parser::token_type lex(parser::semantic_type *yylval, parser::location_type *yylloc);
    void set_debug(bool b);
  };

}

#endif /* _SCANNER_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */