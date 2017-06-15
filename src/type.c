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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "il/type.h"

#include "il/entity.h"
#include "il/expr.h"
#include "il/stmt.h"

il_type_t il_void(void) {
  return (il_type_t) {
    .kind = IL_TYPE_VOID,
    .literal = {
      .size = 0,
      .align = 0
    }
  };
}

il_type_t il_bool(void) {
  return (il_type_t) {
    .kind = IL_TYPE_BOOL,
    .literal = {
      .size = 1,
      .align = 1
    }
  };
}

il_type_t il_char(void) {
  return (il_type_t) {
    .kind = IL_TYPE_CHAR,
    .literal = {
      .size = 1,
      .align = 1
    }
  };
}

il_type_t il_short(void) {
  return (il_type_t) {
    .kind = IL_TYPE_SHORT,
    .literal = {
      .size = 2,
      .align = 2
    }
  };
}

il_type_t il_int(void) {
  return (il_type_t) {
    .kind = IL_TYPE_INT,
    .literal = {
      .size = 4,
      .align = 4
    }
  };
}

il_type_t il_uint(void) {
  return (il_type_t) {
    .kind = IL_TYPE_INT,
    .literal = {
      .size = 4,
      .align = 4,
      .specifiers = IL_TYPE_SPECIFIER_UNSIGNED
    }
  };
}

il_type_t il_long(void) {
  return (il_type_t) {
    .kind = IL_TYPE_LONG,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

il_type_t il_ulong(void) {
  return (il_type_t) {
    .kind = IL_TYPE_LONG,
    .literal = {
      .size = 8,
      .align = 8,
      .specifiers = IL_TYPE_SPECIFIER_UNSIGNED
    }
  };
}

il_type_t il_double(void) {
  return (il_type_t) {
    .kind = IL_TYPE_DOUBLE,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

il_type_t il_float(void) {
  return (il_type_t) {
    .kind = IL_TYPE_FLOAT,
    .literal = {
      .size = 4,
      .align = 4
    }
  };
}

il_type_t il_long_long(void) {
  return (il_type_t) {
    .kind = IL_TYPE_LONG_LONG,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

il_type_t il_long_double(void) {
  return (il_type_t) {
    .kind = IL_TYPE_LONG_DOUBLE,
    .literal = {
      .size = 16,
      .align = 16
    }
  };
}

il_type_t il_pointer(il_type_t of) {
  il_type_t type = {
    .kind = IL_TYPE_POINTER,
    .pointer = {
      .size = 8,
      .align = 8,
      .of = xmalloc(sizeof(il_type_t))
    }
  };

  *type.pointer.of = of;
  return type;
}

static void il_pointer_dtor(il_pointer_t *self) {
  il_type_dtor(self->of);
  xfree(self->of);
}

il_type_t il_array(il_type_t of, il_expr_t length) {
  il_type_t type = {
    .kind = IL_TYPE_ARRAY,
    .array = {
      .size = of.size,
      .align = of.align,
      .of = xmalloc(sizeof(il_type_t)),
      .length = xmalloc(sizeof(il_expr_t))
    }
  };

  *type.array.of = of;
  *type.array.length = length;
  if (il_expr_is_constant(length)) {
    type.size *= il_eval_ulong(length);
  }
  return type;
}

static void il_array_dtor(il_array_t *self) {
  il_type_dtor(self->of);
  xfree(self->of);
  il_expr_dtor(self->length);
  xfree(self->length);
}

il_type_t il_compound(il_entity_t entity) {
  il_type_t type = {
    .kind = IL_TYPE_COMPOUND,
    .compound = {
      .size = entity.size,
      .align = entity.align,
      .entity = xmalloc(sizeof(il_entity_t))
    }
  };

  *type.compound.entity = entity;
  return type;
}

static void il_compound_dtor(il_compound_t *self) {
  il_entity_dtor(self->entity);
  xfree(self->entity);
}


void il_type_dtor(il_type_t *self) {
  switch (self->kind) {
    case IL_TYPE_UNDEFINED:
      return;
    case IL_TYPE_POINTER:
      il_pointer_dtor(&self->pointer);
      break;
    case IL_TYPE_ARRAY:
      il_array_dtor(&self->array);
      break;
    case IL_TYPE_COMPOUND:
      il_compound_dtor(&self->compound);
      break;
    default:
      break;
  }
  il_type_undef(self);
}

bool il_type_is_ref(il_type_t type) {
  return il_is(type, IL_TYPE_ARRAY) || il_is(type, IL_TYPE_POINTER);
}

bool il_type_is_func(il_type_t type) {
  return il_is(type, IL_TYPE_COMPOUND) && il_pis(type.compound.entity, IL_ENTITY_FUNC);
}

bool il_type_equals(il_type_t a, il_type_t b) {
  if (!memcmp(&a, &b, sizeof(a))) {
    return true;
  }
  if (il_type_is_ref(a)) {
    return il_type_is_ref(b) && il_type_equals(il_type_deref(a), il_type_deref(b));
  }
  if (il_type_is_ref(b)) {
    return false;
  }
  if (a.kind != b.kind || a.size != b.size || il_type_specified(a, UNSIGNED) != il_type_specified(b, UNSIGNED)) {
    return false;
  }
  if (il_is(a, IL_TYPE_COMPOUND)) {
    return il_is(b, IL_TYPE_COMPOUND) && il_entity_equals(*a.compound.entity, *b.compound.entity);
  }
  return true;
}

il_type_t il_type_deref(il_type_t a) {
  switch (a.kind) {
    case IL_TYPE_POINTER:
      return *a.pointer.of;
    case IL_TYPE_ARRAY:
      return *a.array.of;
    default:
      return il_type_undefined();
  }
}

il_entity_r il_type_fields(il_type_t self) {
  return il_entity_fields(*self.compound.entity);
}

il_field_t *il_field_lookup(il_type_t self, const char *name) {
  return il_entity_field_lookup(*self.compound.entity, name);
}

unsigned il_type_merge(UNUSED il_type_t *self, UNUSED enum il_type_n with) {
  return IL_TYPE_MERGE_ERROR_NONE;
}
