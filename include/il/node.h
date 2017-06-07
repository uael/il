/*
 * libil - Intermediate Language cross-platform c library
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

#ifndef   IL_NODE_H__
# define  IL_NODE_H__

#include "ir.h"

il_node_t il_entity(il_entity_t entity);
il_node_t il_expr(il_expr_t expr);
il_node_t il_stmt(il_stmt_t stmt);
il_node_t il_type(il_type_t type);

#define il_node_undefined() ((il_node_t) {.kind = IL_NODE_UNDEFINED})

#endif /* IL_NODE_H__ */
