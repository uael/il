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

#include <malloc.h>

#include "types.h"
#include "entity.h"

void jl_type_init(jl_type_t *self, jl_type_n kind, unsigned char qualifiers) {
  *self = (jl_type_t) {
    .kind = kind,
    .qualifiers = qualifiers
  };
  switch (kind) {
    case JL_TYPE_LITERAL:
      self->_literal = malloc(sizeof(jl_literal_t));
      break;
    case JL_TYPE_POINTER:
      self->_pointer = malloc(sizeof(jl_pointer_t));
      break;
    case JL_TYPE_ARRAY:
      self->_array = malloc(sizeof(jl_array_t));
      break;
    case JL_TYPE_COMPOUND:
      self->_compound = malloc(sizeof(jl_compound_t));
      break;
    default:
      break;
  }
}

void jl_type_init_literal(jl_type_t *self, jl_literal_t *literal, unsigned char qualifiers) {
  *self = (jl_type_t) {
    .kind = JL_TYPE_LITERAL,
    ._literal = literal,
    .qualifiers = qualifiers
  };
}

void jl_type_init_pointer(jl_type_t *self, jl_pointer_t *pointer, unsigned char qualifiers) {
  *self = (jl_type_t) {
    .kind = JL_TYPE_POINTER,
    ._pointer = pointer,
    .qualifiers = qualifiers
  };
}

void jl_type_init_array(jl_type_t *self, jl_array_t *array, unsigned char qualifiers) {
  *self = (jl_type_t) {
    .kind = JL_TYPE_ARRAY,
    ._array = array,
    .qualifiers = qualifiers
  };
}

void jl_type_init_compound(jl_type_t *self, jl_compound_t *compound, unsigned char qualifiers) {
  *self = (jl_type_t) {
    .kind = JL_TYPE_COMPOUND,
    ._compound = compound,
    .qualifiers = qualifiers
  };
}

void jl_type_dtor(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_LITERAL:
      if (self->_literal->refs <= 0) {
        jl_literal_dtor(self->_literal);
        free(self->_literal);
        self->_literal = NULL;
      }
      break;
    case JL_TYPE_POINTER:
      if (self->_pointer->refs <= 0) {
        jl_pointer_dtor(self->_pointer);
        free(self->_pointer);
        self->_pointer = NULL;
      }
      break;
    case JL_TYPE_ARRAY:
      if (self->_array->refs <= 0) {
        jl_array_dtor(self->_array);
        free(self->_array);
        self->_array = NULL;
      }
      break;
    case JL_TYPE_COMPOUND:
      if (self->_compound->refs <= 0) {
        jl_compound_dtor(self->_compound);
        free(self->_compound);
        self->_compound = NULL;
      }
      break;
    default:
      break;
  }
}


void jl_literal_init_s(jl_literal_t *self, char *value) {
  *self = (jl_literal_t) {
    .kind = JL_LITERAL_STRING,
    .s = value
  };
}

void jl_literal_init_i(jl_literal_t *self, int value) {
  *self = (jl_literal_t) {
    .kind = JL_LITERAL_INT,
    .i = value
  };
}

void jl_literal_init_in(jl_literal_t *self, jl_literal_n kind, int value) {
  *self = (jl_literal_t) {
    .kind = kind,
    .i = value
  };
}

void jl_literal_init_f(jl_literal_t *self, float value) {
  *self = (jl_literal_t) {
    .kind = JL_LITERAL_FLOAT,
    .f = value
  };
}

void jl_literal_init_fn(jl_literal_t *self, jl_literal_n kind, float value) {
  *self = (jl_literal_t) {
    .kind = kind,
    .f = value
  };
}

void jl_literal_dtor(jl_literal_t *self) {}


void jl_pointer_init(jl_pointer_t *self, jl_type_t of) {
  *self = (jl_pointer_t) {
    .of = of
  };
}

void jl_pointer_dtor(jl_pointer_t *self) {}


void jl_array_init(jl_array_t *self, jl_type_t of, jl_expr_t size) {
  *self = (jl_array_t) {
    .of = of,
    .size = size
  };
}

void jl_array_dtor(jl_array_t *self) {}


void jl_compound_init(jl_compound_t *self, jl_entity_t entity) {
  *self = (jl_compound_t) {
    .entity = entity
  };
}

void jl_compound_dtor(jl_compound_t *self) {}