/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
