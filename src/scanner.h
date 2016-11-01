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

#ifndef YY_DECL
#define  YY_DECL            \
    ddc::parser::token_type        \
    ddc::scanner::lex(        \
  ddc::parser::semantic_type* yylval,    \
  ddc::parser::location_type* yylloc    \
    )
#endif

#ifndef __FLEX_LEXER_H
#  define yyFlexLexer ddcFlexLexer
#  include "FlexLexer.h"
#  undef yyFlexLexer
#endif

#include "parser.h"

namespace ddc {

  class scanner : public ddcFlexLexer {
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
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */