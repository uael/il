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

#ifndef _AST_NODE_H
#define _AST_NODE_H

#include <map>
#include <writer.h>

#define foreach(value, values) \
  for (__typeof__(values) value = values; value; value = (__typeof__(values)) value->next)

#define rforeach(value, values) \
  for (__typeof__(values) value = values; value; value = (__typeof__(values)) value->prev)

#define ACCEPT(node) if ((node)) (node)->accept(this)

namespace dyc {
  namespace ast {
    struct identifier_t;
    struct generic_t;
    struct closure_t;

    struct decl_t;
    struct decl_property_t;
    struct decl_function_t;

    struct type_specifier_t;
    struct type_t;
    struct type_scalar_t;
    struct type_generic_t;

    struct stmt_t;
    struct stmt_expr_t;
    struct stmt_label_t;
    struct stmt_compound_t;
    struct stmt_select_t;
    struct stmt_iter_t;
    struct stmt_jump_t;
    struct stmt_decl_t;

    struct expr_t;
    struct expr_op_t;
    struct expr_dop_t;
    struct expr_ternary_t;
    struct expr_cast_t;
    struct expr_const_t;

    struct const_value_t;
    struct const_lambda_t;
    struct const_initializer_t;

    typedef std::map<expr_t *, expr_t *> ds_map_t;

    struct node_t {
      node_t *scope = nullptr;
      node_t *prev = nullptr;
      node_t *next = nullptr;

      virtual ~node_t();
      virtual void write(writer_t *writer);
      virtual void accept(node_t *scope);

      template<typename T>
      T *push(T *node) {
        foreach(next, this) {
          if (!next->next) {
            next->next = node;
            node->prev = next;
            break;
          }
        }
        return dynamic_cast<T *>(this);
      }
    };
  }
}

#endif /* _AST_NODE_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */
