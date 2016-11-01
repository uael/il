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

#include <iostream>
#include <fstream>
#include <typeinfo>

#include "driver.h"

int main(int argc, char *argv[]) {
  ddc::context calc;
  ddc::driver driver(calc);
  bool readfile = false;

  for (int ai = 1; ai < argc; ++ai) {
    if (argv[ai] == std::string("-p")) {
      driver.trace_parsing = true;
    } else if (argv[ai] == std::string("-s")) {
      driver.trace_scanning = true;
    } else {
      // read a file with nodes

      std::fstream infile(argv[ai]);
      if (!infile.good()) {
        std::cerr << "Could not open file: " << argv[ai] << std::endl;
        return 0;
      }

      calc.clearExpressions();
      bool result = driver.parse_stream(infile, argv[ai]);
      if (result) {
        std::cout << "Expressions:" << std::endl;
        for (unsigned int ei = 0; ei < calc.declarations.size(); ++ei) {
          std::cout << "[" << ei << "]:" << std::endl;
          std::cout << "tree:" << std::endl;
          std::cout << typeid(calc.declarations[ei]).name() << std::endl;
        }
      }

      readfile = true;
    }
  }

  if (readfile) return 0;

  std::cout << "Reading nodes from stdin" << std::endl;

  std::string line;
  while (std::cout << "input: " &&
         std::getline(std::cin, line) &&
         !line.empty()) {
    calc.clearExpressions();
    bool result = driver.parse_string(line, "input");

    if (result) {
      for (unsigned int ei = 0; ei < calc.declarations.size(); ++ei) {
        std::cout << "tree:" << std::endl;
        std::cout << typeid(calc.declarations[ei]).name() << std::endl;
      }
    }
  }
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */