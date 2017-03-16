/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
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

#include "entities.h"

static void jl_entity_init(jl_entity_t *self, jl_entity_n kind, void *ptr) {
  *self = (jl_entity_t) {
    .kind = kind
  };

  switch (kind) {
    case JL_ENTITY_VAR:
      self->_var = ptr ? ptr : xmalloc(sizeof(struct jl_var_t));
      break;
    case JL_ENTITY_PARAM:
      self->_param = ptr ? ptr : xmalloc(sizeof(struct jl_param_t));
      break;
    case JL_ENTITY_FUNC:
      self->_func = ptr ? ptr : xmalloc(sizeof(struct jl_func_t));
      break;
    case JL_ENTITY_ENUM:
      self->_enum = ptr ? ptr : xmalloc(sizeof(struct jl_enum_t));
      break;
    case JL_ENTITY_STRUCT:
      self->_struct = ptr ? ptr : xmalloc(sizeof(struct jl_struct_t));
      break;
    case JL_ENTITY_UNION:
      self->_union = ptr ? ptr : xmalloc(sizeof(struct jl_union_t));
      break;
    case JL_ENTITY_LABEL:
      self->_label = ptr ? ptr : xmalloc(sizeof(struct jl_label_t));
      break;
    default:
      break;
  }
}

void jl_var_init(jl_entity_t *self, const char *name, jl_type_t type, jl_expr_t initializer, unsigned flags) {
  jl_entity_init(self, JL_ENTITY_VAR, NULL);
  *self->_var = (jl_var_t) {
    .name = name,
    .type = type,
    .initializer = initializer,
    .flags = flags
  };
}

void jl_var_dtor(jl_entity_t *self) {}


void jl_param_init(jl_entity_t *self, unsigned position, const char *name, jl_type_t type, jl_expr_t initializer,
  unsigned flags) {
  jl_entity_init(self, JL_ENTITY_PARAM, NULL);
  *self->_param = (jl_param_t) {
    .position = position,
    .name = name,
    .type = type,
    .initializer = initializer,
    .flags = flags
  };
}

void jl_param_dtor(jl_entity_t *self) {}


void jl_func_init(jl_entity_t *self, const char *name, jl_type_t return_type, jl_param_r params, jl_stmt_t body,
  unsigned flags) {
  jl_entity_init(self, JL_ENTITY_FUNC, NULL);
  *self->_func = (jl_func_t) {
    .name = name,
    .return_type = return_type,
    .params = params,
    .body = body,
    .flags = flags
  };
}

void jl_func_dtor(jl_entity_t *self) {
  jl_vector_dtor(self->_func->params);
}


void jl_enum_init(jl_entity_t *self, const char *name, jl_type_t item_type, jl_var_r vars, unsigned flags) {
  jl_entity_init(self, JL_ENTITY_ENUM, NULL);
  *self->_enum = (jl_enum_t) {
    .name = name,
    .item_type = item_type,
    .vars = vars,
    .flags = flags
  };
}

void jl_enum_dtor(jl_entity_t *self) {
  jl_vector_dtor(self->_enum->vars);
}


void jl_struct_init(jl_entity_t *self, const char *name, jl_var_r vars, unsigned flags) {
  jl_entity_init(self, JL_ENTITY_STRUCT, NULL);
  *self->_struct = (jl_struct_t) {
    .name = name,
    .vars = vars,
    .flags = flags
  };
}

void jl_struct_dtor(jl_entity_t *self) {
  jl_vector_dtor(self->_struct->vars);
}


void jl_union_init(jl_entity_t *self, const char *name, jl_var_r vars, unsigned flags) {
  jl_entity_init(self, JL_ENTITY_UNION, NULL);
  *self->_union = (jl_union_t) {
    .name = name,
    .vars = vars,
    .flags = flags
  };
}

void jl_union_dtor(jl_entity_t *self) {
  jl_vector_dtor(self->_union->vars);
}

void jl_label_init(jl_entity_t *self, const char *name, jl_stmt_t next, unsigned flags) {
  jl_entity_init(self, JL_ENTITY_LABEL, NULL);
  *self->_label = (jl_label_t) {
    .name = name,
    .next = next,
    .flags = flags
  };
}

void jl_label_dtor(jl_entity_t *label) {}
