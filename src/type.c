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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "type.h"

#include "entity.h"
#include "expr.h"
#include "stmt.h"

jl_type_t jl_void(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_VOID,
    .literal = {
      .size = 0,
      .align = 0
    }
  };
}

jl_type_t jl_bool(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_BOOL,
    .literal = {
      .size = 1,
      .align = 1
    }
  };
}

jl_type_t jl_char(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_CHAR,
    .literal = {
      .size = 1,
      .align = 1
    }
  };
}

jl_type_t jl_short(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_SHORT,
    .literal = {
      .size = 2,
      .align = 2
    }
  };
}

jl_type_t jl_int(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_INT,
    .literal = {
      .size = 4,
      .align = 4
    }
  };
}

jl_type_t jl_uint(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_INT,
    .literal = {
      .size = 4,
      .align = 4,
      .specifiers = JL_TYPE_SPECIFIER_UNSIGNED
    }
  };
}

jl_type_t jl_long(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_LONG,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

jl_type_t jl_ulong(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_LONG,
    .literal = {
      .size = 8,
      .align = 8,
      .specifiers = JL_TYPE_SPECIFIER_UNSIGNED
    }
  };
}

jl_type_t jl_double(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_DOUBLE,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

jl_type_t jl_float(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_FLOAT,
    .literal = {
      .size = 4,
      .align = 4
    }
  };
}

jl_type_t jl_long_long(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_LONG_LONG,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

jl_type_t jl_long_double(void) {
  return (jl_type_t) {
    .kind = JL_TYPE_LONG_DOUBLE,
    .literal = {
      .size = 16,
      .align = 16
    }
  };
}

jl_type_t jl_pointer(jl_type_t of) {
  jl_type_t type = {
    .kind = JL_TYPE_POINTER,
    .pointer = {
      .size = 8,
      .align = 8,
      .of = xmalloc(sizeof(jl_type_t))
    }
  };

  *type.pointer.of = of;
  return type;
}

static void jl_pointer_dtor(jl_pointer_t *self) {
  jl_type_dtor(self->of);
  xfree(self->of);
}

jl_type_t jl_array(jl_type_t of, jl_expr_t length) {
  jl_type_t type = {
    .kind = JL_TYPE_ARRAY,
    .array = {
      .size = of.size,
      .align = of.align,
      .of = xmalloc(sizeof(jl_type_t)),
      .length = xmalloc(sizeof(jl_expr_t))
    }
  };

  *type.array.of = of;
  *type.array.length = length;
  if (jl_expr_is_constant(length)) {
    type.size *= jl_eval_ulong(length);
  }
  return type;
}

static void jl_array_dtor(jl_array_t *self) {
  jl_type_dtor(self->of);
  xfree(self->of);
  jl_expr_dtor(self->length);
  xfree(self->length);
}

jl_type_t jl_compound(jl_entity_t entity) {
  jl_type_t type = {
    .kind = JL_TYPE_COMPOUND,
    .compound = {
      .size = entity.size,
      .align = entity.align,
      .entity = xmalloc(sizeof(jl_entity_t))
    }
  };

  *type.compound.entity = entity;
  return type;
}

static void jl_compound_dtor(jl_compound_t *self) {
  jl_entity_dtor(self->entity);
  xfree(self->entity);
}


void jl_type_dtor(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_UNDEFINED:
      return;
    case JL_TYPE_POINTER:
      jl_pointer_dtor(&self->pointer);
      break;
    case JL_TYPE_ARRAY:
      jl_array_dtor(&self->array);
      break;
    case JL_TYPE_COMPOUND:
      jl_compound_dtor(&self->compound);
      break;
    default:
      break;
  }
  jl_type_undef(self);
}

bool jl_type_is_ref(jl_type_t type) {
  return jl_is(type, JL_TYPE_ARRAY) || jl_is(type, JL_TYPE_POINTER);
}

bool jl_type_is_func(jl_type_t type) {
  return jl_is(type, JL_TYPE_COMPOUND) && jl_pis(type.compound.entity, JL_ENTITY_FUNC);
}

bool jl_type_equals(jl_type_t a, jl_type_t b) {
  if (!memcmp(&a, &b, sizeof(a))) {
    return true;
  }
  if (jl_type_is_ref(a)) {
    return jl_type_is_ref(b) && jl_type_equals(jl_type_deref(a), jl_type_deref(b));
  }
  if (jl_type_is_ref(b)) {
    return false;
  }
  if (a.kind != b.kind || a.size != b.size || jl_type_specified(a, UNSIGNED) != jl_type_specified(b, UNSIGNED)) {
    return false;
  }
  if (jl_is(a, JL_TYPE_COMPOUND)) {
    return jl_is(b, JL_TYPE_COMPOUND) && jl_entity_equals(*a.compound.entity, *b.compound.entity);
  }
  return true;
}

jl_type_t jl_type_deref(jl_type_t a) {
  switch (a.kind) {
    case JL_TYPE_POINTER:
      return *a.pointer.of;
    case JL_TYPE_ARRAY:
      return *a.array.of;
    default:
      return jl_type_undefined();
  }
}

jl_entity_r jl_type_fields(jl_type_t self) {
  return jl_entity_fields(*self.compound.entity);
}

jl_field_t *jl_field_lookup(jl_type_t self, const char *name) {
  return jl_entity_field_lookup(*self.compound.entity, name);
}

unsigned jl_type_merge(jl_type_t *self, enum jl_type_n with) {
  return JL_TYPE_MERGE_ERROR_NONE;
}
