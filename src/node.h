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

#ifndef   JL_NODE_H__
# define  JL_NODE_H__

#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

typedef struct jl_node jl_node_t;

enum jl_node_n {
  JL_NODE_UNDEFINED = 0,
  JL_NODE_ENTITY,
  JL_NODE_EXPR,
  JL_NODE_STMT,
  JL_NODE_TYPE
};

struct jl_node {
  union {
    jl_lloc_t lloc;
    jl_entity_t entity;
    jl_expr_t expr;
    jl_stmt_t stmt;
    jl_type_t type;
  };
  enum jl_node_n kind;
};

jl_node_t jl_entity(jl_entity_t entity);
jl_node_t jl_expr(jl_expr_t expr);
jl_node_t jl_stmt(jl_stmt_t stmt);
jl_node_t jl_type(jl_type_t type);

#define jl_node_undefined() (jl_node_t) {JL_NODE_UNDEFINED}

#define jl_node_is_entity(n) ((n).kind == JL_NODE_ENTITY)
#define jl_node_is_expr(n) ((n).kind == JL_NODE_EXPR)
#define jl_node_is_stmt(n) ((n).kind == JL_NODE_STMT)
#define jl_node_is_type(n) ((n).kind == JL_NODE_TYPE)
#define jl_pnode_is_entity(n) ((n)->kind == JL_NODE_ENTITY)
#define jl_pnode_is_expr(n) ((n)->kind == JL_NODE_EXPR)
#define jl_pnode_is_stmt(n) ((n)->kind == JL_NODE_STMT)
#define jl_pnode_is_type(n) ((n)->kind == JL_NODE_TYPE)

#endif /* JL_NODE_H__ */
