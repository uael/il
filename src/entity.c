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
#include <limits.h>
#include <assert.h>

#include "adt/xmalloc.h"

#include "entity.h"

#include "expr.h"
#include "stmt.h"
#include "type.h"
#include "compiler.h"

wulk_entity_t wulk_field(const char *name, wulk_type_t type) {
  return (wulk_entity_t) {
    .kind = WULK_ENTITY_FIELD,
    .field = {
      .offset = type.align,
      .type = type,
      .size = type.size,
      .align = type.align
    }
  };
}

static void wulk_field_dtor(wulk_field_t *self) {
  wulk_type_dtor(&self->type);
}

wulk_entity_t wulk_var_int(const char *name, int d) {
  return wulk_var(name, wulk_type_undefined(), wulk_const_int(wulk_no_lloc(), d));
}

wulk_entity_t wulk_var_float(const char *name, float f) {
  return wulk_var(name, wulk_type_undefined(), wulk_const_float(wulk_no_lloc(), f));
}

wulk_entity_t wulk_var_string(const char *name, const char *s) {
  return wulk_var(name, wulk_type_undefined(), wulk_const_string(wulk_no_lloc(), s));
}

wulk_entity_t wulk_var(const char *name, wulk_type_t type, wulk_expr_t initializer) {
  if (!wulk_defined(type)) {
    type = initializer.type;
  } else if (!wulk_type_equals(type, initializer.type)) {
    initializer = wulk_cast(wulk_no_lloc(), type, initializer);
  }
  return (wulk_entity_t) {
    .kind = WULK_ENTITY_VAR,
    .variable = {
      .initializer = initializer,
      .name = name,
      .type = type,
      .size = type.size,
      .align = type.align
    }
  };
}

static void wulk_var_dtor(wulk_var_t *self) {
  wulk_type_dtor(&self->type);
  wulk_expr_dtor(&self->initializer);
}

wulk_entity_t wulk_param_int(unsigned position, const char *name, int d) {
  return wulk_param(position, name, wulk_type_undefined(), wulk_const_int(wulk_no_lloc(), d));
}

wulk_entity_t wulk_param_float(unsigned position, const char *name, float f) {
  return wulk_param(position, name, wulk_type_undefined(), wulk_const_float(wulk_no_lloc(), f));
}

wulk_entity_t wulk_param_string(unsigned position, const char *name, const char *s) {
  return wulk_param(position, name, wulk_type_undefined(), wulk_const_string(wulk_no_lloc(), s));
}

wulk_entity_t wulk_param(unsigned position, const char *name, wulk_type_t type, wulk_expr_t initializer) {
  if (!wulk_defined(type)) {
    type = initializer.type;
  } else if (!wulk_type_equals(type, initializer.type)) {
    initializer = wulk_cast(wulk_no_lloc(), type, initializer);
  }
  return (wulk_entity_t) {
    .kind = WULK_ENTITY_PARAM,
    .parameter = {
      .position = position,
      .initializer = initializer,
      .name = name,
      .type = type,
      .size = type.size,
      .align = type.align
    }
  };
}

static void wulk_param_dtor(wulk_param_t *self) {
  wulk_type_dtor(&self->type);
  wulk_expr_dtor(&self->initializer);
}

wulk_entity_t wulk_func_decl(wulk_type_t return_type, const char *name, wulk_param_t *params) {
  return wulk_func(return_type, name, params, wulk_stmt_undefined());
}

wulk_entity_t wulk_proc_decl(const char *name, wulk_param_t *params) {
  return wulk_func(wulk_void(), name, params, wulk_stmt_undefined());
}

wulk_entity_t wulk_func_def(wulk_func_t prototype, wulk_stmt_t body) {
  wulk_entity_t param,
    entity = wulk_func(prototype.return_type, prototype.name, NULL, body);
  
  adt_vector_foreach(prototype.params, param) {
    adt_vector_push(entity.function.params, param);
  }
  return entity;
}

wulk_entity_t wulk_func(wulk_type_t return_type, const char *name, wulk_param_t *params, wulk_stmt_t body) {
  unsigned count = 0;
  wulk_param_t param;
  wulk_entity_t entity = (wulk_entity_t) {
    .kind = WULK_ENTITY_FUNC,
    .function = {
      .body = body,
      .name = name,
      .return_type = return_type
    }
  };
  if (params) while (params->name) {
    param = *params++;
    adt_vector_push(entity.function.params, wulk_param(count++, param.name, param.type, param.initializer));
  }
  return entity;
}

static void wulk_func_dtor(wulk_func_t *self) {
  wulk_entity_t param;

  adt_vector_foreach(self->params, param) {
    wulk_entity_dtor(&param);
  }
  adt_vector_dtor(self->params);
  wulk_type_dtor(&self->return_type);
  wulk_stmt_dtor(&self->body);
}

wulk_entity_t wulk_enum_anonymous(wulk_field_t *fields) {
  return wulk_enum(NULL, fields);
}

wulk_entity_t wulk_enum(const char *name, wulk_field_t *fields) {
  wulk_field_t field;
  wulk_entity_t entity = (wulk_entity_t) {
    .kind = WULK_ENTITY_ENUM,
    .enumerable = {
      .name = name,
      .type = wulk_int()
    }
  };
  entity.size = entity.type.size;
  entity.align = entity.type.align;
  if (fields) while (fields->name) {
    field = *fields++;
    adt_vector_push(entity.enumerable.vars, wulk_field(field.name, field.type));
  }
  return entity;
}

static void wulk_enum_dtor(wulk_enum_t *self) {
  wulk_entity_t field;

  adt_vector_foreach(self->vars, field) {
    wulk_entity_dtor(&field);
  }
  adt_vector_dtor(self->vars);
}

static wulk_entity_t *type_add_field(wulk_entity_t *self, const char *name, wulk_type_t type, unsigned short width);

static void reset_field_alignment(wulk_entity_t *self) {
  int d;
  size_t n;
  const wulk_entity_t *m;

  assert(wulk_pis(self, WULK_ENTITY_STRUCT));
  n = adt_vector_length(self->structure.fields);
  if (n) {
    m = &adt_vector_at(self->structure.fields, n - 1);
    if (m->field.field_width) {
      d = m->field.field_offset + m->field.field_width;
      if (d < 32) {
        type_add_field(self, NULL, wulk_int(), (short) (32 - d));
      }
    } else if (self->size % 4 != 0) {
      self->size += self->size % 4;
    }
  }
}

static size_t adjust_member_alignment(wulk_entity_t *self, wulk_type_t type) {
  size_t align = 0;

  if (wulk_pis(self, WULK_ENTITY_STRUCT)) {
    align = type.align;
    if (self->size % align) {
      self->size += align - (self->size % align);
      assert(self->size % align == 0);
    }

    align = self->size;
  }
  return align;
}

static wulk_entity_t *add_member(wulk_entity_t *self, wulk_entity_t m) {
  wulk_entity_r entities;
  wulk_entity_t *member = NULL;

  if (m.name && strcmp(m.name, "...") == 0) {
    // TODO: va_args
    exit(1);
  } else {
    if (m.name && wulk_entity_field_lookup(*self, m.name)) {
      wulk_fatal_err(NULL, "Member '%s' already exists.", m.name);
    }
    switch (self->kind) {
      case WULK_ENTITY_STRUCT:
        entities = self->structure.fields;
        break;
      case WULK_ENTITY_UNION:
        entities = self->u_structure.fields;
        break;
      default:
        return NULL;
    }
    adt_vector_push(entities, m);
    member = &adt_vector_back(entities);
    if (m.type.size == 0) {
      wulk_fatal_err(NULL, "Member '%s' has incomplete type.", m.name);
    }
    if (LONG_MAX - m.field.offset < m.type.size) {
      wulk_fatal_err(NULL, "Object is too large.");
    }
    if (self->size < m.field.offset + m.type.size) {
      self->size = m.field.offset + m.type.size;
    }
  }
  return member;
}

static int pack_field(const wulk_entity_t *prev, wulk_entity_t *m) {
  unsigned short bits;

  assert(prev);
  bits = prev->field.field_offset + prev->field.field_width;
  if (bits + m->field.field_width <= (unsigned short) (sizeof(int) * 8)) {
    m->field.offset = prev->field.offset;
    m->field.field_offset = bits;
    return 1;
  }
  return 0;
}

static size_t remove_anonymous_fields(wulk_entity_t *self) {
  int i;
  size_t align, maxalign;
  wulk_field_t *m;
  wulk_entity_r entities;

  switch (self->kind) {
    case WULK_ENTITY_STRUCT:
      entities = self->structure.fields;
      break;
    case WULK_ENTITY_UNION:
      entities = self->u_structure.fields;
      break;
    default:
      return 0;
  }

  maxalign = 0;
  for (i = (int) (adt_vector_length(entities) - 1); i >= 0; --i) {
    m = &adt_vector_at(entities, i).field;
    if (!m->name) {
      wulk_entity_dtor(&adt_vector_at(entities, i));
      adt_vector_erase(entities, i);
    } else {
      align = m->align;
      if (align > maxalign) {
        maxalign = align;
      }
    }
  }

  return maxalign;
}

static wulk_entity_t *type_add_field(wulk_entity_t *self, const char *name, wulk_type_t type, unsigned short width) {
  wulk_entity_t m;
  const wulk_entity_t *prev;

  m = wulk_field(name, type);
  m.field.field_width = width;
  if (wulk_pis(self, WULK_ENTITY_STRUCT)) {
    prev = &adt_vector_back(self->structure.fields);
    if (!prev || !pack_field(prev, &m)) {
      m.field.field_offset = 0;
      m.field.offset = adjust_member_alignment(self, type);
    }
  }
  return add_member(self, m);
}

static wulk_entity_t *type_add_anonymous_member(wulk_entity_t *self, wulk_type_t type) {
  unsigned i;
  size_t offset;
  wulk_entity_t m;
  wulk_entity_t *member;
  wulk_entity_r entities;

  if (!wulk_is(type, WULK_TYPE_COMPOUND)) {
    // TODO: handle error
    exit(1);
  }
  member = type.compound.entity;
  switch (member->kind) {
    case WULK_ENTITY_STRUCT:
      entities = member->structure.fields;
      break;
    case WULK_ENTITY_UNION:
      entities = member->u_structure.fields;
      break;
    default:
      return NULL;
  }
  if (wulk_pis(self, WULK_ENTITY_STRUCT) && wulk_pis(member, WULK_ENTITY_UNION)) {
    offset = adjust_member_alignment(self, type);
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      m.field.offset += offset;
      add_member(self, m);
    }
  } else if (wulk_pis(self, WULK_ENTITY_UNION) && wulk_pis(member, WULK_ENTITY_STRUCT)) {
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      add_member(self, m);
    }
  } else {
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      wulk_entity_add_field(self, m.name, m.type, 0);
    }
  }
  return NULL;
}

void type_seal(wulk_entity_t *self) {
  size_t align;

  align = remove_anonymous_fields(self);
  if (align == 0) {
    // TODO: handle error
    exit(1);
  }
  if (self->size % align) {
    self->size += align - (self->size % align);
  }
}

wulk_entity_t wulk_struct(const char *name, wulk_field_t *fields) {
  wulk_entity_t entity = (wulk_entity_t) {
    .kind = WULK_ENTITY_STRUCT,
    .structure = {
      .name = name,
      .size = 0
    }
  };

  if (fields) {
    while (fields->name) {
      wulk_entity_add_field(&entity, fields->name, fields->type, fields->field_width);
      ++fields;
    }
    type_seal(&entity);
  }
  return entity;
}

wulk_entity_t wulk_struct_anonymous(wulk_field_t *fields) {
  return wulk_struct(NULL, fields);
}

void wulk_struct_dtor(wulk_struct_t *self) {
  wulk_entity_t field;

  adt_vector_foreach(self->fields, field) {
    wulk_entity_dtor(&field);
  }
  adt_vector_dtor(self->fields);
}

wulk_entity_t wulk_union(const char *name, wulk_field_t *fields) {
  wulk_entity_t entity = (wulk_entity_t) {
    .kind = WULK_ENTITY_UNION,
    .u_structure = {
      .name = name,
      .size = 0
    }
  };

  if (fields) {
    while (fields->name) {
      wulk_entity_add_field(&entity, fields->name, fields->type, fields->field_width);
      ++fields;
    }
    type_seal(&entity);
  }
  return entity;
}

wulk_entity_t wulk_union_anonymous(wulk_field_t *fields) {
  return wulk_union(NULL, fields);
}

static void wulk_union_dtor(wulk_union_t *self) {
  wulk_entity_t field;

  adt_vector_foreach(self->fields, field) {
    wulk_entity_dtor(&field);
  }
  adt_vector_dtor(self->fields);
}

void wulk_label_dtor(wulk_label_t *self) {}

void wulk_entity_dtor(wulk_entity_t *self) {
  switch (self->kind) {
    case WULK_ENTITY_UNDEFINED:
      return;
    case WULK_ENTITY_FIELD:
      wulk_field_dtor(&self->field);
      break;
    case WULK_ENTITY_VAR:
      wulk_var_dtor(&self->variable);
      break;
    case WULK_ENTITY_PARAM:
      wulk_param_dtor(&self->parameter);
      break;
    case WULK_ENTITY_FUNC:
      wulk_func_dtor(&self->function);
      break;
    case WULK_ENTITY_ENUM:
      wulk_enum_dtor(&self->enumerable);
      break;
    case WULK_ENTITY_STRUCT:
      wulk_struct_dtor(&self->structure);
      break;
    case WULK_ENTITY_UNION:
      wulk_union_dtor(&self->u_structure);
      break;
    case WULK_ENTITY_LABEL:
      wulk_label_dtor(&self->label);
      break;
    default:
      break;
  }
  wulk_entity_undef(self);
}

bool wulk_entity_equals(wulk_entity_t a, wulk_entity_t b) {
  unsigned i;
  wulk_entity_r a_fields, b_fields;

  if (a.kind != b.kind) {
    return false;
  }
  if (!wulk_type_specified(a.type, UNSIGNED)) {
    return wulk_type_equals(a.type, b.type);
  }
  a_fields = wulk_entity_fields(a);
  b_fields = wulk_entity_fields(b);
  if (adt_vector_size(a_fields) != adt_vector_size(b_fields)) {
    return false;
  }
  for (i = 0; i < adt_vector_size(a_fields); ++i) {
    if (!wulk_entity_equals(adt_vector_at(a_fields, i), adt_vector_at(b_fields, i))) {
      return false;
    }
  }
  return true;
}

wulk_entity_r wulk_entity_fields(wulk_entity_t self) {
  switch (self.kind) {
    case WULK_ENTITY_FUNC:
      return self.function.params;
    case WULK_ENTITY_ENUM:
      return self.enumerable.vars;
    case WULK_ENTITY_STRUCT:
      return self.structure.fields;
    case WULK_ENTITY_UNION:
      return self.u_structure.fields;
    default:
      return (wulk_entity_r) {0};
  }
}

wulk_field_t *wulk_entity_field_lookup(wulk_entity_t self, const char *name) {
  wulk_entity_r fields;
  wulk_entity_t entity;

  switch (self.kind) {
    case WULK_ENTITY_STRUCT:
      fields = self.structure.fields;
      adt_vector_foreach(fields, entity) {
          if (strcmp(name, entity.name) == 0) {
            return &adt_vector_at(fields, __i).field;
          }
        }
      break;
    case WULK_ENTITY_UNION:
      fields = self.u_structure.fields;
      adt_vector_foreach(fields, entity) {
          if (strcmp(name, entity.name) == 0) {
            return &adt_vector_at(fields, __i).field;
          }
        }
      break;
    default:
      break;
      return NULL;
  }
  return NULL;
}

wulk_entity_t *wulk_entity_add_field(wulk_entity_t *self, const char *name, wulk_type_t type, short width) {
  wulk_entity_t m;

  if (width) {
    return type_add_field(self, name, type, width);
  }
  if (!name) {
    return type_add_anonymous_member(self, type);
  }
  m = wulk_field(name, type);
  m.field.offset = adjust_member_alignment(self, type);
  m.name = name;
  m.type = type;
  return add_member(self, m);
}
