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

#include <stdexcept>
#include "context.h"

namespace ddc {

  context::~context() {
    clearExpressions();
  }

  void context::clearExpressions() {
    for (unsigned int i = 0; i < nodes.size(); ++i) {
      delete nodes[i];
    }
    nodes.clear();
    for (unsigned int i = 0; i < declarations.size(); ++i) {
      delete declarations[i];
    }
    declarations.clear();
  }

  bool context::existsVariable(const std::string &varname) const {
    return variables.find(varname) != variables.end();
  }

  double context::getVariable(const std::string &varname) const {
    std::map<std::string, double>::const_iterator vi = variables.find(varname);
    if (vi == variables.end())
      throw (std::runtime_error("Unknown variable."));
    else
      return vi->second;
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