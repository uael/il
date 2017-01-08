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

#ifndef _CGEN_H
#define _CGEN_H

#include <map>
#include "ast/ast.h"

#define CGEN(type, node, ...) \
  ({ if (gen_cursor) { delete gen_cursor; } gen_cursor = new type(this->program, node); gen_cursor->generate(file); gen_cursor; })

#define TRY_CGEN(type) \
  do {Ast::type *__n; if ((__n = as(this->node, Ast::type))) { cursor = CGEN(type, __n)->cursor; return; }} while (0)

namespace Jay {
  namespace Gen {
    struct File;
  }

  struct _CGen {
    std::string cursor;
    virtual void generate(Gen::File *file) {};
  };

  template <typename T = Ast::Node>
  struct CGen : _CGen {
    Ast::Program *program;
    _CGen *gen_cursor = nullptr;
    T *node;

    CGen(Ast::Program *program, T *node) : program(program), node(node) {}
    CGen(Ast::Program *program) : CGen(program, program) {}

    virtual ~CGen() {
      delete gen_cursor;
    };
  };
}

#include "program.h"
#include "file.h"

#endif /* _CGEN_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */