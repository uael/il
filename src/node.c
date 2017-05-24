/*
 * MIT License
 *
 * Copyright (c) 2016-2017 uael <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "node.h"

wulk_node_t wulk_entity(wulk_entity_t entity) {
  return (wulk_node_t) {
    .kind = WULK_NODE_ENTITY,
    .entity = entity
  };
}

wulk_node_t wulk_expr(wulk_expr_t expr) {
  return (wulk_node_t) {
    .kind = WULK_NODE_EXPR,
    .expr = expr
  };
}

wulk_node_t wulk_stmt(wulk_stmt_t stmt) {
  return (wulk_node_t) {
    .kind = WULK_NODE_STMT,
    .stmt = stmt
  };
}

wulk_node_t wulk_type(wulk_type_t type) {
  return (wulk_node_t) {
    .kind = WULK_NODE_TYPE,
    .type = type
  };
}
