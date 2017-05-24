/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "type.h"

#include "entity.h"
#include "expr.h"
#include "stmt.h"

wulk_type_t wulk_void(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_VOID,
    .literal = {
      .size = 0,
      .align = 0
    }
  };
}

wulk_type_t wulk_bool(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_BOOL,
    .literal = {
      .size = 1,
      .align = 1
    }
  };
}

wulk_type_t wulk_char(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_CHAR,
    .literal = {
      .size = 1,
      .align = 1
    }
  };
}

wulk_type_t wulk_short(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_SHORT,
    .literal = {
      .size = 2,
      .align = 2
    }
  };
}

wulk_type_t wulk_int(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_INT,
    .literal = {
      .size = 4,
      .align = 4
    }
  };
}

wulk_type_t wulk_uint(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_INT,
    .literal = {
      .size = 4,
      .align = 4,
      .specifiers = WULK_TYPE_SPECIFIER_UNSIGNED
    }
  };
}

wulk_type_t wulk_long(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_LONG,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

wulk_type_t wulk_ulong(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_LONG,
    .literal = {
      .size = 8,
      .align = 8,
      .specifiers = WULK_TYPE_SPECIFIER_UNSIGNED
    }
  };
}

wulk_type_t wulk_double(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_DOUBLE,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

wulk_type_t wulk_float(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_FLOAT,
    .literal = {
      .size = 4,
      .align = 4
    }
  };
}

wulk_type_t wulk_long_long(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_LONG_LONG,
    .literal = {
      .size = 8,
      .align = 8
    }
  };
}

wulk_type_t wulk_long_double(void) {
  return (wulk_type_t) {
    .kind = WULK_TYPE_LONG_DOUBLE,
    .literal = {
      .size = 16,
      .align = 16
    }
  };
}

wulk_type_t wulk_pointer(wulk_type_t of) {
  wulk_type_t type = {
    .kind = WULK_TYPE_POINTER,
    .pointer = {
      .size = 8,
      .align = 8,
      .of = xmalloc(sizeof(wulk_type_t))
    }
  };

  *type.pointer.of = of;
  return type;
}

static void wulk_pointer_dtor(wulk_pointer_t *self) {
  wulk_type_dtor(self->of);
  xfree(self->of);
}

wulk_type_t wulk_array(wulk_type_t of, wulk_expr_t length) {
  wulk_type_t type = {
    .kind = WULK_TYPE_ARRAY,
    .array = {
      .size = of.size,
      .align = of.align,
      .of = xmalloc(sizeof(wulk_type_t)),
      .length = xmalloc(sizeof(wulk_expr_t))
    }
  };

  *type.array.of = of;
  *type.array.length = length;
  if (wulk_expr_is_constant(length)) {
    type.size *= wulk_eval_ulong(length);
  }
  return type;
}

static void wulk_array_dtor(wulk_array_t *self) {
  wulk_type_dtor(self->of);
  xfree(self->of);
  wulk_expr_dtor(self->length);
  xfree(self->length);
}

wulk_type_t wulk_compound(wulk_entity_t entity) {
  wulk_type_t type = {
    .kind = WULK_TYPE_COMPOUND,
    .compound = {
      .size = entity.size,
      .align = entity.align,
      .entity = xmalloc(sizeof(wulk_entity_t))
    }
  };

  *type.compound.entity = entity;
  return type;
}

static void wulk_compound_dtor(wulk_compound_t *self) {
  wulk_entity_dtor(self->entity);
  xfree(self->entity);
}


void wulk_type_dtor(wulk_type_t *self) {
  switch (self->kind) {
    case WULK_TYPE_UNDEFINED:
      return;
    case WULK_TYPE_POINTER:
      wulk_pointer_dtor(&self->pointer);
      break;
    case WULK_TYPE_ARRAY:
      wulk_array_dtor(&self->array);
      break;
    case WULK_TYPE_COMPOUND:
      wulk_compound_dtor(&self->compound);
      break;
    default:
      break;
  }
  wulk_type_undef(self);
}

bool wulk_type_is_ref(wulk_type_t type) {
  return wulk_is(type, WULK_TYPE_ARRAY) || wulk_is(type, WULK_TYPE_POINTER);
}

bool wulk_type_is_func(wulk_type_t type) {
  return wulk_is(type, WULK_TYPE_COMPOUND) && wulk_pis(type.compound.entity, WULK_ENTITY_FUNC);
}

bool wulk_type_equals(wulk_type_t a, wulk_type_t b) {
  if (!memcmp(&a, &b, sizeof(a))) {
    return true;
  }
  if (wulk_type_is_ref(a)) {
    return wulk_type_is_ref(b) && wulk_type_equals(wulk_type_deref(a), wulk_type_deref(b));
  }
  if (wulk_type_is_ref(b)) {
    return false;
  }
  if (a.kind != b.kind || a.size != b.size || wulk_type_specified(a, UNSIGNED) != wulk_type_specified(b, UNSIGNED)) {
    return false;
  }
  if (wulk_is(a, WULK_TYPE_COMPOUND)) {
    return wulk_is(b, WULK_TYPE_COMPOUND) && wulk_entity_equals(*a.compound.entity, *b.compound.entity);
  }
  return true;
}

wulk_type_t wulk_type_deref(wulk_type_t a) {
  switch (a.kind) {
    case WULK_TYPE_POINTER:
      return *a.pointer.of;
    case WULK_TYPE_ARRAY:
      return *a.array.of;
    default:
      return wulk_type_undefined();
  }
}

wulk_entity_r wulk_type_fields(wulk_type_t self) {
  return wulk_entity_fields(*self.compound.entity);
}

wulk_field_t *wulk_field_lookup(wulk_type_t self, const char *name) {
  return wulk_entity_field_lookup(*self.compound.entity, name);
}

unsigned wulk_type_merge(wulk_type_t *self, enum wulk_type_n with) {
  return WULK_TYPE_MERGE_ERROR_NONE;
}
