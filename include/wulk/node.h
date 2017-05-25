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

#ifndef   WULK_NODE_H__
# define  WULK_NODE_H__

#include "ir.h"

wulk_node_t wulk_entity(wulk_entity_t entity);
wulk_node_t wulk_expr(wulk_expr_t expr);
wulk_node_t wulk_stmt(wulk_stmt_t stmt);
wulk_node_t wulk_type(wulk_type_t type);

#define wulk_node_undefined() ((wulk_node_t) {.kind = WULK_NODE_UNDEFINED})

#endif /* WULK_NODE_H__ */
