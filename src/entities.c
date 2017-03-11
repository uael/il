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

void jl_var_init(jl_var_t *self, const char *name, jl_type_t type, struct jl_expr_t *initializer, unsigned flags) {
  *self = (jl_var_t) {
    .name = name,
    .type = type,
    .initializer = malloc(sizeof(struct jl_expr_t)),
    .flags = flags
  };
  if (initializer) {
    *self->initializer = *initializer;
  }
}

void jl_var_dtor(jl_var_t *self) {
  jl_expr_dtor(self->initializer);
  free(self->initializer);
  self->initializer = NULL;
}


void jl_param_init(jl_param_t *self, unsigned position, const char *name, jl_type_t type, struct jl_expr_t *initializer,
  unsigned flags) {
  *self = (jl_param_t) {
    .position = position,
    .name = name,
    .type = type,
    .initializer = malloc(sizeof(struct jl_expr_t)),
    .flags = flags
  };
  if (initializer) {
    *self->initializer = *initializer;
  }
}

void jl_param_dtor(jl_param_t *self) {
  jl_expr_dtor(self->initializer);
  free(self->initializer);
  self->initializer = NULL;
}


void jl_func_init(jl_func_t *self, const char *name, jl_type_t return_type, jl_param_r params, struct jl_stmt_t *body,
  unsigned flags) {
  *self = (jl_func_t) {
    .name = name,
    .return_type = return_type,
    .params = params,
    .body = malloc(sizeof(struct jl_stmt_t)),
    .flags = flags
  };
  if (body) {
    *self->body = *body;
  }
}

void jl_func_dtor(jl_func_t *self) {
  jl_vector_dtor(self->params);
  jl_stmt_dtor(self->body);
  free(self->body);
  self->body = NULL;
}


void jl_enum_init(jl_enum_t *self, const char *name, jl_type_t item_type, jl_var_r vars, unsigned flags) {
  *self = (jl_enum_t) {
    .name = name,
    .item_type = item_type,
    .vars = vars,
    .flags = flags
  };
}

void jl_enum_dtor(jl_enum_t *self) {
  jl_vector_dtor(self->vars);
}


void jl_struct_init(jl_struct_t *self, const char *name, jl_var_r vars, unsigned flags) {
  *self = (jl_struct_t) {
    .name = name,
    .vars = vars,
    .flags = flags
  };
}

void jl_struct_dtor(jl_struct_t *self) {
  jl_vector_dtor(self->vars);
}


void jl_union_init(jl_union_t *self, const char *name, jl_var_r vars, unsigned flags) {
  *self = (jl_union_t) {
    .name = name,
    .vars = vars,
    .flags = flags
  };
}

void jl_union_dtor(jl_union_t *self) {
  jl_vector_dtor(self->vars);
}

void jl_label_init(jl_label_t *self, const char *name, jl_stmt_t next, unsigned flags) {
  *self = (jl_label_t) {
    .name = name,
    .next = next,
    .flags = flags
  };
}

void jl_label_dtor(jl_label_t *label) {}
