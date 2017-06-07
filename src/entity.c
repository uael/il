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
#include <limits.h>
#include <assert.h>

#include "adt/xmalloc.h"

#include "entity.h"

#include "expr.h"
#include "stmt.h"
#include "type.h"
#include "compiler.h"

il_entity_t il_field(UNUSED const char *name, il_type_t type) {
  return (il_entity_t) {
    .kind = IL_ENTITY_FIELD,
    .field = {
      .offset = type.align,
      .type = type,
      .size = type.size,
      .align = type.align
    }
  };
}

static void il_field_dtor(il_field_t *self) {
  il_type_dtor(&self->type);
}

il_entity_t il_var_int(const char *name, int d) {
  return il_var(name, il_type_undefined(), il_const_int(il_no_lloc(), d));
}

il_entity_t il_var_float(const char *name, float f) {
  return il_var(name, il_type_undefined(), il_const_float(il_no_lloc(), f));
}

il_entity_t il_var_string(const char *name, const char *s) {
  return il_var(name, il_type_undefined(), il_const_string(il_no_lloc(), s));
}

il_entity_t il_var(const char *name, il_type_t type, il_expr_t initializer) {
  if (!il_defined(type)) {
    type = initializer.type;
  } else if (!il_type_equals(type, initializer.type)) {
    initializer = il_cast(il_no_lloc(), type, initializer);
  }
  return (il_entity_t) {
    .kind = IL_ENTITY_VAR,
    .variable = {
      .initializer = initializer,
      .name = name,
      .type = type,
      .size = type.size,
      .align = type.align
    }
  };
}

static void il_var_dtor(il_var_t *self) {
  il_type_dtor(&self->type);
  il_expr_dtor(&self->initializer);
}

il_entity_t il_param_int(unsigned position, const char *name, int d) {
  return il_param(position, name, il_type_undefined(), il_const_int(il_no_lloc(), d));
}

il_entity_t il_param_float(unsigned position, const char *name, float f) {
  return il_param(position, name, il_type_undefined(), il_const_float(il_no_lloc(), f));
}

il_entity_t il_param_string(unsigned position, const char *name, const char *s) {
  return il_param(position, name, il_type_undefined(), il_const_string(il_no_lloc(), s));
}

il_entity_t il_param(unsigned position, const char *name, il_type_t type, il_expr_t initializer) {
  if (!il_defined(type)) {
    type = initializer.type;
  } else if (!il_type_equals(type, initializer.type)) {
    initializer = il_cast(il_no_lloc(), type, initializer);
  }
  return (il_entity_t) {
    .kind = IL_ENTITY_PARAM,
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

static void il_param_dtor(il_param_t *self) {
  il_type_dtor(&self->type);
  il_expr_dtor(&self->initializer);
}

il_entity_t il_func_decl(il_type_t return_type, const char *name, il_param_t *params) {
  return il_func(return_type, name, params, il_stmt_undefined());
}

il_entity_t il_proc_decl(const char *name, il_param_t *params) {
  return il_func(il_void(), name, params, il_stmt_undefined());
}

il_entity_t il_func_def(il_func_t prototype, il_stmt_t body) {
  il_entity_t param,
    entity = il_func(prototype.return_type, prototype.name, NULL, body);
  
  adt_vector_foreach(prototype.params, param) {
    adt_vector_push(entity.function.params, param);
  }
  return entity;
}

il_entity_t il_func(il_type_t return_type, const char *name, il_param_t *params, il_stmt_t body) {
  unsigned count = 0;
  il_param_t param;
  il_entity_t entity = (il_entity_t) {
    .kind = IL_ENTITY_FUNC,
    .function = {
      .body = body,
      .name = name,
      .return_type = return_type
    }
  };
  if (params) while (params->name) {
    param = *params++;
    adt_vector_push(entity.function.params, il_param(count++, param.name, param.type, param.initializer));
  }
  return entity;
}

static void il_func_dtor(il_func_t *self) {
  il_entity_t param;

  adt_vector_foreach(self->params, param) {
    il_entity_dtor(&param);
  }
  adt_vector_dtor(self->params);
  il_type_dtor(&self->return_type);
  il_stmt_dtor(&self->body);
}

il_entity_t il_enum_anonymous(il_field_t *fields) {
  return il_enum(NULL, fields);
}

il_entity_t il_enum(const char *name, il_field_t *fields) {
  il_field_t field;
  il_entity_t entity = (il_entity_t) {
    .kind = IL_ENTITY_ENUM,
    .enumerable = {
      .name = name,
      .type = il_int()
    }
  };
  entity.size = entity.type.size;
  entity.align = entity.type.align;
  if (fields) while (fields->name) {
    field = *fields++;
    adt_vector_push(entity.enumerable.vars, il_field(field.name, field.type));
  }
  return entity;
}

static void il_enum_dtor(il_enum_t *self) {
  il_entity_t field;

  adt_vector_foreach(self->vars, field) {
    il_entity_dtor(&field);
  }
  adt_vector_dtor(self->vars);
}

static il_entity_t *type_add_field(il_entity_t *self, const char *name, il_type_t type, unsigned short width);

static UNUSED void reset_field_alignment(il_entity_t *self) {
  int d;
  size_t n;
  const il_entity_t *m;

  assert(il_pis(self, IL_ENTITY_STRUCT));
  n = adt_vector_length(self->structure.fields);
  if (n) {
    m = &adt_vector_at(self->structure.fields, n - 1);
    if (m->field.field_width) {
      d = m->field.field_offset + m->field.field_width;
      if (d < 32) {
        type_add_field(self, NULL, il_int(), (short) (32 - d));
      }
    } else if (self->size % 4 != 0) {
      self->size += self->size % 4;
    }
  }
}

static size_t adjust_member_alignment(il_entity_t *self, il_type_t type) {
  size_t align = 0;

  if (il_pis(self, IL_ENTITY_STRUCT)) {
    align = type.align;
    if (self->size % align) {
      self->size += align - (self->size % align);
      assert(self->size % align == 0);
    }

    align = self->size;
  }
  return align;
}

static il_entity_t *add_member(il_entity_t *self, il_entity_t m) {
  il_entity_r entities;
  il_entity_t *member = NULL;

  if (m.name && strcmp(m.name, "...") == 0) {
    // TODO: va_args
    exit(1);
  } else {
    if (m.name && il_entity_field_lookup(*self, m.name)) {
      il_fatal_err(NULL, "Member '%s' already exists.", m.name);
    }
    switch (self->kind) {
      case IL_ENTITY_STRUCT:
        entities = self->structure.fields;
        break;
      case IL_ENTITY_UNION:
        entities = self->u_structure.fields;
        break;
      default:
        return NULL;
    }
    adt_vector_push(entities, m);
    member = &adt_vector_back(entities);
    if (m.type.size == 0) {
      il_fatal_err(NULL, "Member '%s' has incomplete type.", m.name);
    }
    if (LONG_MAX - m.field.offset < m.type.size) {
      il_fatal_err(NULL, "Object is too large.");
    }
    if (self->size < m.field.offset + m.type.size) {
      self->size = m.field.offset + m.type.size;
    }
  }
  return member;
}

static int pack_field(const il_entity_t *prev, il_entity_t *m) {
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

static size_t remove_anonymous_fields(il_entity_t *self) {
  int i;
  size_t align, maxalign;
  il_field_t *m;
  il_entity_r entities;

  switch (self->kind) {
    case IL_ENTITY_STRUCT:
      entities = self->structure.fields;
      break;
    case IL_ENTITY_UNION:
      entities = self->u_structure.fields;
      break;
    default:
      return 0;
  }

  maxalign = 0;
  for (i = (int) (adt_vector_length(entities) - 1); i >= 0; --i) {
    m = &adt_vector_at(entities, i).field;
    if (!m->name) {
      il_entity_dtor(&adt_vector_at(entities, i));
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

static il_entity_t *type_add_field(il_entity_t *self, const char *name, il_type_t type, unsigned short width) {
  il_entity_t m;
  const il_entity_t *prev;

  m = il_field(name, type);
  m.field.field_width = width;
  if (il_pis(self, IL_ENTITY_STRUCT)) {
    prev = &adt_vector_back(self->structure.fields);
    if (!prev || !pack_field(prev, &m)) {
      m.field.field_offset = 0;
      m.field.offset = adjust_member_alignment(self, type);
    }
  }
  return add_member(self, m);
}

static il_entity_t *type_add_anonymous_member(il_entity_t *self, il_type_t type) {
  unsigned i;
  size_t offset;
  il_entity_t m;
  il_entity_t *member;
  il_entity_r entities;

  if (!il_is(type, IL_TYPE_COMPOUND)) {
    // TODO: handle error
    exit(1);
  }
  member = type.compound.entity;
  switch (member->kind) {
    case IL_ENTITY_STRUCT:
      entities = member->structure.fields;
      break;
    case IL_ENTITY_UNION:
      entities = member->u_structure.fields;
      break;
    default:
      return NULL;
  }
  if (il_pis(self, IL_ENTITY_STRUCT) && il_pis(member, IL_ENTITY_UNION)) {
    offset = adjust_member_alignment(self, type);
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      m.field.offset += offset;
      add_member(self, m);
    }
  } else if (il_pis(self, IL_ENTITY_UNION) && il_pis(member, IL_ENTITY_STRUCT)) {
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      add_member(self, m);
    }
  } else {
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      il_entity_add_field(self, m.name, m.type, 0);
    }
  }
  return NULL;
}

void type_seal(il_entity_t *self) {
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

il_entity_t il_struct(const char *name, il_field_t *fields) {
  il_entity_t entity = (il_entity_t) {
    .kind = IL_ENTITY_STRUCT,
    .structure = {
      .name = name,
      .size = 0
    }
  };

  if (fields) {
    while (fields->name) {
      il_entity_add_field(&entity, fields->name, fields->type, fields->field_width);
      ++fields;
    }
    type_seal(&entity);
  }
  return entity;
}

il_entity_t il_struct_anonymous(il_field_t *fields) {
  return il_struct(NULL, fields);
}

void il_struct_dtor(il_struct_t *self) {
  il_entity_t field;

  adt_vector_foreach(self->fields, field) {
    il_entity_dtor(&field);
  }
  adt_vector_dtor(self->fields);
}

il_entity_t il_union(const char *name, il_field_t *fields) {
  il_entity_t entity = (il_entity_t) {
    .kind = IL_ENTITY_UNION,
    .u_structure = {
      .name = name,
      .size = 0
    }
  };

  if (fields) {
    while (fields->name) {
      il_entity_add_field(&entity, fields->name, fields->type, fields->field_width);
      ++fields;
    }
    type_seal(&entity);
  }
  return entity;
}

il_entity_t il_union_anonymous(il_field_t *fields) {
  return il_union(NULL, fields);
}

static void il_union_dtor(il_union_t *self) {
  il_entity_t field;

  adt_vector_foreach(self->fields, field) {
    il_entity_dtor(&field);
  }
  adt_vector_dtor(self->fields);
}

void il_label_dtor(UNUSED il_label_t *self) {}

void il_entity_dtor(il_entity_t *self) {
  switch (self->kind) {
    case IL_ENTITY_UNDEFINED:
      return;
    case IL_ENTITY_FIELD:
      il_field_dtor(&self->field);
      break;
    case IL_ENTITY_VAR:
      il_var_dtor(&self->variable);
      break;
    case IL_ENTITY_PARAM:
      il_param_dtor(&self->parameter);
      break;
    case IL_ENTITY_FUNC:
      il_func_dtor(&self->function);
      break;
    case IL_ENTITY_ENUM:
      il_enum_dtor(&self->enumerable);
      break;
    case IL_ENTITY_STRUCT:
      il_struct_dtor(&self->structure);
      break;
    case IL_ENTITY_UNION:
      il_union_dtor(&self->u_structure);
      break;
    case IL_ENTITY_LABEL:
      il_label_dtor(&self->label);
      break;
    default:
      break;
  }
  il_entity_undef(self);
}

bool il_entity_equals(il_entity_t a, il_entity_t b) {
  unsigned i;
  il_entity_r a_fields, b_fields;

  if (a.kind != b.kind) {
    return false;
  }
  if (!il_type_specified(a.type, UNSIGNED)) {
    return il_type_equals(a.type, b.type);
  }
  a_fields = il_entity_fields(a);
  b_fields = il_entity_fields(b);
  if (adt_vector_size(a_fields) != adt_vector_size(b_fields)) {
    return false;
  }
  for (i = 0; i < adt_vector_size(a_fields); ++i) {
    if (!il_entity_equals(adt_vector_at(a_fields, i), adt_vector_at(b_fields, i))) {
      return false;
    }
  }
  return true;
}

il_entity_r il_entity_fields(il_entity_t self) {
  switch (self.kind) {
    case IL_ENTITY_FUNC:
      return self.function.params;
    case IL_ENTITY_ENUM:
      return self.enumerable.vars;
    case IL_ENTITY_STRUCT:
      return self.structure.fields;
    case IL_ENTITY_UNION:
      return self.u_structure.fields;
    default:
      return (il_entity_r) {0};
  }
}

il_field_t *il_entity_field_lookup(il_entity_t self, const char *name) {
  il_entity_r fields;
  il_entity_t entity;

  switch (self.kind) {
    case IL_ENTITY_STRUCT:
      fields = self.structure.fields;
      adt_vector_foreach(fields, entity) {
          if (strcmp(name, entity.name) == 0) {
            return &adt_vector_at(fields, __i).field;
          }
        }
      break;
    case IL_ENTITY_UNION:
      fields = self.u_structure.fields;
      adt_vector_foreach(fields, entity) {
          if (strcmp(name, entity.name) == 0) {
            return &adt_vector_at(fields, __i).field;
          }
        }
      break;
    default:
      break;
  }
  return NULL;
}

il_entity_t *il_entity_add_field(il_entity_t *self, const char *name, il_type_t type, short width) {
  il_entity_t m;

  if (width) {
    return type_add_field(self, name, type, width);
  }
  if (!name) {
    return type_add_anonymous_member(self, type);
  }
  m = il_field(name, type);
  m.field.offset = adjust_member_alignment(self, type);
  m.name = name;
  m.type = type;
  return add_member(self, m);
}
