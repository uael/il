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
#include <algorithm>
#include "file.h"
#include "decl.h"

namespace Jay {
  namespace Gen {
    File::File(Ast::Program *ast, Ast::File *node) : CGen(ast, node) {}

    struct MatchPathSeparator {
      bool operator()(char ch) const {
        return ch == '/';
      }
    };

    std::string basename(std::string const &pathname) {
      return std::string(
        std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator()).base(),
        pathname.end());
    }

    std::string no_ext(std::string const &filename) {
      std::string::const_reverse_iterator pivot = std::find(filename.rbegin(), filename.rend(), '.');
      return pivot == filename.rend() ? filename : std::string(filename.begin(), pivot.base() - 1);
    }

    void File::generate(File *file) {
      foreach(decl, node->decls) {
        Decl gen(this->program, decl);
        gen.generate(this);
      }
      this->write(no_ext(basename(node->filename)));
    }

    void File::write(std::string filename_no_ext) {
      std::ofstream ofstream;
      ofstream.open(filename_no_ext + ".h");
      ofstream << includes;
      ofstream << macros;
      ofstream << definitions;
      ofstream.close();
      ofstream.open(filename_no_ext + ".c");
      source_includes += "#include \"" + filename_no_ext + ".h\"\n";
      ofstream << source_includes;
      ofstream << source_macros;
      ofstream << declarations;
      ofstream.close();
    }
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
