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

#ifndef _DRIVER_H
#define _DRIVER_H

#include <string>
#include <vector>
#include "scanner.h"
#include "context.h"

namespace dyc {

  class driver {
  public:
    bool trace_scanning;
    bool trace_parsing;
    std::string streamname;

    dyc::context_t context;

    class scanner *lexer;

    driver();

    bool parse_stream(std::istream &in, const std::string &sname = "stream input");
    bool parse_string(const std::string &input, const std::string &sname = "string stream");
    bool parse_file(const std::string &filename);
    void error(const class location &l, const std::string &m);
    void error(const std::string &m);
  };
}

#endif /* _DRIVER_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */