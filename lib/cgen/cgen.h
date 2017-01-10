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

#define APPEND(str) \
  (cursor += str)

#define APPENDLN(str) \
  (cursor += str + "\n")

#define CGEN(type, node) \
  ({ if (gen_cursor) delete gen_cursor; \
    gen_cursor = nullptr; \
    if (node) { \
      gen_cursor = new type(this->program, node); \
      gen_cursor->generate(file); \
    } \
    gen_cursor ? gen_cursor->cursor : ""; \
  })

#define CGEN_ALL(type, node, ...) \
  ({ \
    std::string __ret; \
    if (node) { \
      foreach(__node, node) { \
        __ret += CGEN(type, __node); \
        if (__node->next && strlen(__VA_ARGS__"") > 0) { \
          __ret += __VA_ARGS__""; \
        } \
      } \
    }; \
    __ret; \
  })

#define TRY_CGEN(type) \
  do {Ast::type *__n; if ((__n = as(this->node, Ast::type))) { cursor = CGEN(type, __n); return; }} while (0)

#define CGEN_APPEND(type, node) \
  APPEND(CGEN(type, node))

#define CGEN_APPENDLN(type, node) \
  APPENDLN(CGEN(type, node))

#define CGEN_APPEND_ALL(type, node) \
  do { if (node) { foreach(__node, node) { CGEN_APPEND(type, __node); }}} while (0)

#define CGEN_APPENDLN_ALL(type, node) \
  do { if (node) { foreach(__node, node) { CGEN_APPENDLN(type, __node); }}} while (0)

namespace Jay {
  namespace Gen {
    struct File;
  }

  struct Generator {
    std::string cursor;

    virtual ~Generator() {}
    virtual void generate(Gen::File *file) {}
  };

  template <typename T = Ast::Node>
  struct CGen : Generator {
    Ast::Program *program;
    Generator *gen_cursor = nullptr;
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