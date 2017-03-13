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

#include <adt/xmalloc.h>

#include "entity.h"
#include "entities.h"

void jl_entity_init(jl_entity_t *self, jl_entity_n kind, void *ptr) {
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
  }
}

void jl_entity_dtor(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_VAR:
      if (self->_var->refs <= 0) {
        jl_var_dtor(self->_var);
        free(self->_var);
        self->_var = NULL;
      }
      break;
    case JL_ENTITY_PARAM:
      if (self->_param->refs <= 0) {
        jl_param_dtor(self->_param);
        free(self->_param);
        self->_param = NULL;
      }
      break;
    case JL_ENTITY_FUNC:
      if (self->_func->refs <= 0) {
        jl_func_dtor(self->_func);
        free(self->_func);
        self->_func = NULL;
      }
      break;
    case JL_ENTITY_ENUM:
      if (self->_enum->refs <= 0) {
        jl_enum_dtor(self->_enum);
        free(self->_enum);
        self->_enum = NULL;
      }
      break;
    case JL_ENTITY_STRUCT:
      if (self->_struct->refs <= 0) {
        jl_struct_dtor(self->_struct);
        free(self->_struct);
        self->_struct = NULL;
      }
      break;
    case JL_ENTITY_UNION:
      if (self->_union->refs <= 0) {
        jl_union_dtor(self->_union);
        free(self->_union);
        self->_union = NULL;
      }
      break;
    case JL_ENTITY_LABEL:
      if (self->_label->refs <= 0) {
        jl_label_dtor(self->_label);
        free(self->_label);
        self->_label = NULL;
      }
      break;
  }
}
