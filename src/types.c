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

#include "types.h"
#include "entity.h"

void jl_literal_dtor(jl_type_t *self);
void jl_pointer_dtor(jl_type_t *self);
void jl_array_dtor(jl_type_t *self);
void jl_compound_dtor(jl_type_t *self);

static void jl_type_init(jl_type_t *self, jl_type_n kind, unsigned char qualifiers, void *ptr) {
  *self = (jl_type_t) {
    .kind = kind,
    .qualifiers = qualifiers
  };
  switch (kind) {
    case JL_TYPE_LITERAL:
      self->_literal = ptr ? ptr : xmalloc(sizeof(jl_literal_t));
      break;
    case JL_TYPE_POINTER:
      self->_pointer = ptr ? ptr : xmalloc(sizeof(jl_pointer_t));
      break;
    case JL_TYPE_ARRAY:
      self->_array = ptr ? ptr : xmalloc(sizeof(jl_array_t));
      break;
    case JL_TYPE_COMPOUND:
      self->_compound = ptr ? ptr : xmalloc(sizeof(jl_compound_t));
      break;
    default:
      break;
  }
}

void jl_type_dtor(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_LITERAL:
      if (self->_literal->refs <= 0) {
        jl_literal_dtor(self);
        free(self->_literal);
        self->_literal = NULL;
      }
      break;
    case JL_TYPE_POINTER:
      if (self->_pointer->refs <= 0) {
        jl_pointer_dtor(self);
        free(self->_pointer);
        self->_pointer = NULL;
      }
      break;
    case JL_TYPE_ARRAY:
      if (self->_array->refs <= 0) {
        jl_array_dtor(self);
        free(self->_array);
        self->_array = NULL;
      }
      break;
    case JL_TYPE_COMPOUND:
      if (self->_compound->refs <= 0) {
        jl_compound_dtor(self);
        free(self->_compound);
        self->_compound = NULL;
      }
      break;
    default:
      break;
  }
  self->kind = JL_TYPE_UNDEFINED;
}


void jl_literal_init(jl_type_t *self, jl_literal_n kind, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_LITERAL, qualifiers, NULL);
  *self->_literal = (jl_literal_t) {
    .kind = kind,
  };
}

void jl_literal_init_s(jl_type_t *self, char *value, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_LITERAL, qualifiers, NULL);
  *self->_literal = (jl_literal_t) {
    .kind = JL_LITERAL_STRING,
    .s = value
  };
}

void jl_literal_init_i(jl_type_t *self, int value, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_LITERAL, qualifiers, NULL);
  *self->_literal = (jl_literal_t) {
    .kind = JL_LITERAL_INT,
    .i = value
  };
}

void jl_literal_init_in(jl_type_t *self, jl_literal_n kind, int value, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_LITERAL, qualifiers, NULL);
  *self->_literal = (jl_literal_t) {
    .kind = kind,
    .i = value
  };
}

void jl_literal_init_f(jl_type_t *self, float value, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_LITERAL, qualifiers, NULL);
  *self->_literal = (jl_literal_t) {
    .kind = JL_LITERAL_FLOAT,
    .f = value
  };
}

void jl_literal_init_fn(jl_type_t *self, jl_literal_n kind, float value, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_LITERAL, qualifiers, NULL);
  *self->_literal = (jl_literal_t) {
    .kind = kind,
    .f = value
  };
}

void jl_literal_dtor(jl_type_t *self) {}


void jl_pointer_init(jl_type_t *self, jl_type_t of, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_POINTER, qualifiers, NULL);
  *self->_pointer = (jl_pointer_t) {
    .of = of
  };
}

void jl_pointer_dtor(jl_type_t *self) {}


void jl_array_init(jl_type_t *self, jl_type_t of, jl_expr_t size, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_ARRAY, qualifiers, NULL);
  *self->_array = (jl_array_t) {
    .of = of,
    .size = size
  };
}

void jl_array_dtor(jl_type_t *self) {}


void jl_compound_init(jl_type_t *self, jl_entity_t entity, unsigned char qualifiers) {
  jl_type_init(self, JL_TYPE_COMPOUND, qualifiers, NULL);
  *self->_compound = (jl_compound_t) {
    .entity = entity
  };
}

void jl_compound_dtor(jl_type_t *self) {}
