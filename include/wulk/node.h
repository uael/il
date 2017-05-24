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

#ifndef   WULK_NODE_H__
# define  WULK_NODE_H__

#include "ir.h"

wulk_node_t wulk_entity(wulk_entity_t entity);
wulk_node_t wulk_expr(wulk_expr_t expr);
wulk_node_t wulk_stmt(wulk_stmt_t stmt);
wulk_node_t wulk_type(wulk_type_t type);

#define wulk_node_undefined() ((wulk_node_t) {.kind = WULK_NODE_UNDEFINED})

#endif /* WULK_NODE_H__ */
