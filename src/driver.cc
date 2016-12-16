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

#include <fstream>
#include <sstream>
#include <iostream>
#include "driver.h"

namespace dyc {

  driver::driver()
    : trace_scanning(false), trace_parsing(false) { }

  bool driver::parse_stream(std::istream &in, const std::string &sname) {
    streamname = sname;

    scanner scanner(&in);
    scanner.set_debug(trace_scanning);
    this->lexer = &scanner;

    parser parser(*this);
    parser.set_debug_level(trace_parsing);
    if (parser.parse() == 0) {
      ast.accept(nullptr);
      return true;
    }
    return false;
  }

  bool driver::parse_file(const std::string &filename) {
    std::ifstream in(filename.c_str());
    if (!in.good()) return false;
    return parse_stream(in, filename);
  }

  bool driver::parse_string(const std::string &input, const std::string &sname) {
    std::istringstream iss(input);
    return parse_stream(iss, sname);
  }

  void driver::error(const class location &l, const std::string &m) {
    std::cerr << l << ": " << m << std::endl;
  }

  void driver::error(const std::string &m) {
    std::cerr << m << std::endl;
  }
}

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */