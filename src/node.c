/*
 * Wulk - Wu uniform language kit
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#include "node.h"

il_node_t il_entity(il_entity_t entity) {
  return (il_node_t) {
    .kind = IL_NODE_ENTITY,
    .entity = entity
  };
}

il_node_t il_expr(il_expr_t expr) {
  return (il_node_t) {
    .kind = IL_NODE_EXPR,
    .expr = expr
  };
}

il_node_t il_stmt(il_stmt_t stmt) {
  return (il_node_t) {
    .kind = IL_NODE_STMT,
    .stmt = stmt
  };
}

il_node_t il_type(il_type_t type) {
  return (il_node_t) {
    .kind = IL_NODE_TYPE,
    .type = type
  };
}
