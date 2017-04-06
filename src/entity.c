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
#include <limits.h>
#include <assert.h>

#include <adt/xmalloc.h>

#include "entity.h"

#include "expr.h"
#include "stmt.h"
#include "type.h"
#include "compiler.h"

void jl_field_dtor(jl_field_t *self);
void jl_var_dtor(jl_var_t *self);
void jl_param_dtor(jl_param_t *self);
void jl_func_dtor(jl_func_t *self);
void jl_enum_dtor(jl_enum_t *self);
void jl_struct_dtor(jl_struct_t *self);
void jl_union_dtor(jl_union_t *self);
void jl_label_dtor(jl_label_t *self);

void jl_entity_dtor(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_UNDEFINED:
      return;
    case JL_ENTITY_FIELD:
      jl_field_dtor(&self->field);
      break;
    case JL_ENTITY_VAR:
      jl_var_dtor(&self->variable);
      break;
    case JL_ENTITY_PARAM:
      jl_param_dtor(&self->parameter);
      break;
    case JL_ENTITY_FUNC:
      jl_func_dtor(&self->function);
      break;
    case JL_ENTITY_ENUM:
      jl_enum_dtor(&self->enumerable);
      break;
    case JL_ENTITY_STRUCT:
      jl_struct_dtor(&self->structure);
      break;
    case JL_ENTITY_UNION:
      jl_union_dtor(&self->u_structure);
      break;
    case JL_ENTITY_LABEL:
      jl_label_dtor(&self->label);
      break;
    default:
      break;
  }
  jl_entity_undef(self);
}

void jl_entity_update_size(jl_entity_t *self) {
  jl_entity_r fields = jl_entity_fields(*self);
  jl_entity_t entity;

  self->size = 0;
  adt_vector_foreach(fields, entity) {
    self->size += entity.type.size;
  }
}

bool jl_entity_equals(jl_entity_t a, jl_entity_t b) {
  unsigned i;
  jl_entity_r a_fields, b_fields;

  if (a.kind != b.kind) {
    return false;
  }
  if (!jl_type_specified(a.type, UNSIGNED)) {
    return jl_type_equals(a.type, b.type);
  }
  a_fields = jl_entity_fields(a);
  b_fields = jl_entity_fields(b);
  if (adt_vector_size(a_fields) != adt_vector_size(b_fields)) {
    return false;
  }
  for (i = 0; i < adt_vector_size(a_fields); ++i) {
    if (!jl_entity_equals(adt_vector_at(a_fields, i), adt_vector_at(b_fields, i))) {
      return false;
    }
  }
  return true;
}

jl_entity_r jl_entity_fields(jl_entity_t self) {
  switch (self.kind) {
    case JL_ENTITY_FUNC:
      return self.function.params;
    case JL_ENTITY_ENUM:
      return self.enumerable.vars;
    case JL_ENTITY_STRUCT:
      return self.structure.fields;
    case JL_ENTITY_UNION:
      return self.u_structure.fields;
    default:
      return (jl_entity_r) {0};
  }
}

jl_field_t *jl_entity_field_lookup(jl_entity_t self, const char *name) {
  jl_entity_r fields;
  jl_entity_t entity;

  switch (self.kind) {
    case JL_ENTITY_STRUCT:
      fields = self.structure.fields;
      adt_vector_foreach(fields, entity) {
        if (strcmp(name, entity.name) == 0) {
          return &adt_vector_at(fields, __i).field;
        }
      }
      break;
    case JL_ENTITY_UNION:
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

void jl_entity_add_field(jl_entity_t *self, const char *name, jl_type_t type) {
  jl_entity_t entity;

  switch (self->kind) {
    case JL_ENTITY_FUNC:
      adt_vector_push(
        self->function.params,
        jl_param((unsigned) adt_vector_length(self->function.params), name, type, jl_expr_undefined())
      );
      break;
    case JL_ENTITY_ENUM:
      adt_vector_push(
        self->enumerable.vars,
        jl_var(name, type, jl_expr_undefined())
      );
      break;
    case JL_ENTITY_STRUCT:
      entity = jl_field(name, type);
      if (self->size % entity.field.offset) {
        self->size += entity.field.offset - (self->size % entity.field.offset);
        assert(self->size % entity.field.offset == 0);
      }
      entity.field.offset = self->size;
      if (self->size < entity.field.offset + type.size) {
        self->size = entity.field.offset + type.size;
      }
      adt_vector_push(self->structure.fields, entity);
      break;
    case JL_ENTITY_UNION:
      adt_vector_push(self->enumerable.vars, jl_field(name, type));
      break;
    default:
      break;
  }
}

jl_entity_t jl_field(const char *name, jl_type_t type) {
  return (jl_entity_t) {
    .kind = JL_ENTITY_FIELD,
    .type = type,
    .size = type.size,
    .align = type.align,
    .field.offset = type.align
  };
}

static void jl_field_dtor(jl_field_t *self) {
  jl_type_dtor(&self->type);
}

jl_entity_t jl_var_int(const char *name, int d) {
  return jl_var(name, jl_type_undefined(), jl_const_int(d));
}

jl_entity_t jl_var_float(const char *name, float f) {
  return jl_var(name, jl_type_undefined(), jl_const_float(f));
}

jl_entity_t jl_var_string(const char *name, const char *s) {
  return jl_var(name, jl_type_undefined(), jl_const_string(s));
}

jl_entity_t jl_var(const char *name, jl_type_t type, jl_expr_t initializer) {
  if (!jl_defined(type)) {
    type = initializer.type;
  } else if (!jl_type_equals(type, initializer.type)) {
    initializer = jl_cast(type, initializer);
  }
  return (jl_entity_t) {
    .kind = JL_ENTITY_VAR,
    .name = name,
    .type = type,
    .size = type.size,
    .align = type.align,
    .variable.initializer = initializer
  };
}

void jl_var_dtor(jl_var_t *self) {
  jl_type_dtor(&self->type);
  jl_expr_dtor(&self->initializer);
}

jl_entity_t jl_param_int(unsigned position, const char *name, int d) {
  return jl_param(position, name, jl_type_undefined(), jl_const_int(d));
}

jl_entity_t jl_param_float(unsigned position, const char *name, float f) {
  return jl_param(position, name, jl_type_undefined(), jl_const_float(f));
}

jl_entity_t jl_param_string(unsigned position, const char *name, const char *s) {
  return jl_param(position, name, jl_type_undefined(), jl_const_string(s));
}

jl_entity_t jl_param(unsigned position, const char *name, jl_type_t type, jl_expr_t initializer) {
  if (!jl_defined(type)) {
    type = initializer.type;
  } else if (!jl_type_equals(type, initializer.type)) {
    initializer = jl_cast(type, initializer);
  }
  return (jl_entity_t) {
    .kind = JL_ENTITY_PARAM,
    .name = name,
    .type = type,
    .size = type.size,
    .align = type.align,
    .parameter.position = position,
    .parameter.initializer = initializer
  };
}

void jl_param_dtor(jl_param_t *self) {
  jl_type_dtor(&self->type);
  jl_expr_dtor(&self->initializer);
}

jl_entity_t jl_func_decl(jl_type_t return_type, const char *name, jl_param_t *params) {
  return jl_func(return_type, name, params, jl_stmt_undefined());
}

jl_entity_t jl_proc_decl(const char *name, jl_param_t *params) {
  return jl_func(jl_void(), name, params, jl_stmt_undefined());
}

jl_entity_t jl_func_def(jl_func_t prototype, jl_stmt_t body) {
  jl_entity_t param, 
    entity = jl_func(prototype.return_type, prototype.name, NULL, body);
  
  adt_vector_foreach(prototype.params, param) {
    adt_vector_push(entity.function.params, param);
  }
  return entity;
}

jl_entity_t jl_func(jl_type_t return_type, const char *name, jl_param_t *params, jl_stmt_t body) {
  unsigned count = 0;
  jl_param_t param;
  jl_entity_t entity = (jl_entity_t) {
    .kind = JL_ENTITY_FUNC,
    .name = name,
    .type = return_type,
    .function.body = body
  };
  if (params) while (params->name) {
    param = *params++;
    adt_vector_push(entity.function.params, jl_param(count++, param.name, param.type, param.initializer));
  }
  return entity;
}

void jl_func_dtor(jl_func_t *self) {
  jl_entity_t param;

  adt_vector_foreach(self->params, param) {
    jl_entity_dtor(&param);
  }
  adt_vector_dtor(self->params);
  jl_type_dtor(&self->return_type);
  jl_stmt_dtor(&self->body);
}

jl_entity_t jl_enum_anonymous(jl_field_t *fields) {
  return jl_enum(NULL, fields);
}

jl_entity_t jl_enum(const char *name, jl_field_t *fields) {
  jl_field_t field;
  jl_entity_t entity = (jl_entity_t) {
    .kind = JL_ENTITY_ENUM,
    .name = name,
    .type = jl_int()
  };
  entity.size = entity.type.size;
  entity.align = entity.type.align;
  if (fields) while (fields->name) {
    field = *fields++;
    adt_vector_push(entity.enumerable.vars, jl_field(field.name, field.type));
  }
  return entity;
}

void jl_enum_dtor(jl_enum_t *self) {
  jl_entity_t field;

  adt_vector_foreach(self->vars, field) {
    jl_entity_dtor(&field);
  }
  adt_vector_dtor(self->vars);
}

jl_entity_t jl_struct_anonymous(jl_field_t *fields) {
  return jl_struct(NULL, fields);
}

static void type_add_field(jl_entity_t *self, const char *name, jl_type_t type, short width);

static void reset_field_alignment(jl_entity_t *self) {
  int d;
  size_t n;
  const jl_entity_t *m;

  assert(jl_pis(self, JL_ENTITY_STRUCT));
  n = adt_vector_length(self->structure.fields);
  if (n) {
    m = &adt_vector_at(self->structure.fields, n - 1);
    if (m->field.field_width) {
      d = m->field.field_offset + m->field.field_width;
      if (d < 32) {
        type_add_field(self, NULL, jl_int(), (short) (32 - d));
      }
    } else if (self->size % 4 != 0) {
      self->size += self->size % 4;
    }
  }
}

static size_t adjust_member_alignment(jl_entity_t *self, jl_type_t type) {
  size_t align = 0;

  if (jl_pis(self, JL_ENTITY_STRUCT)) {
    align = type.align;
    if (self->size % align) {
      self->size += align - (self->size % align);
      assert(self->size % align == 0);
    }

    align = self->size;
  }
  return align;
}

static void add_member(jl_entity_t *self, jl_entity_t m) {
  jl_entity_r entities;

  if (m.name && strcmp(m.name, "...") == 0) {
    // TODO: va_args
    exit(1);
  } else {
    if (m.name && jl_entity_field_lookup(*self, m.name)) {
      jl_fatal_err(NULL, "Member '%s' already exists.", m.name);
    }
    switch (self->kind) {
      case JL_ENTITY_STRUCT:
        entities = self->structure.fields;
        break;
      case JL_ENTITY_UNION:
        entities = self->u_structure.fields;
        break;
      default:
        return;
    }
    adt_vector_push(entities, m);
    if (m.type.size == 0) {
      jl_fatal_err(NULL, "Member '%s' has incomplete type.", m.name);
    }
    if (LONG_MAX - m.field.offset < m.type.size) {
      jl_fatal_err(NULL, "Object is too large.");
    }
    if (self->size < m.field.offset + m.type.size) {
      self->size = m.field.offset + m.type.size;
    }
  }
}

static void type_add_member(jl_entity_t *self, const char *name, jl_type_t type) {
  jl_entity_t m = jl_field(name, type);

  m.field.offset = adjust_member_alignment(self, type);
  m.name = name;
  m.type = type;
  add_member(self, m);
}

static int pack_field(const jl_entity_t *prev, jl_entity_t *m) {
  short bits;

  assert(prev);
  bits = prev->field.field_offset + prev->field.field_width;
  if (bits + m->field.field_width <= sizeof(int) * 8) {
    m->field.offset = prev->field.offset;
    m->field.field_offset = bits;
    return 1;
  }
  return 0;
}

static size_t remove_anonymous_fields(jl_entity_t *self) {
  int i;
  size_t align, maxalign;
  jl_field_t *m;
  jl_entity_r entities;

  switch (self->kind) {
    case JL_ENTITY_STRUCT:
      entities = self->structure.fields;
      break;
    case JL_ENTITY_UNION:
      entities = self->u_structure.fields;
      break;
    default:
      return 0;
  }

  maxalign = 0;
  for (i = (int) (adt_vector_length(entities) - 1); i >= 0; --i) {
    m = &adt_vector_at(entities, i).field;
    if (!m->name) {
      jl_entity_dtor(&adt_vector_at(entities, i));
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

static void type_add_field(jl_entity_t *self, const char *name, jl_type_t type, short width) {
  jl_entity_t m;
  const jl_entity_t *prev;

  if (name && !width) {
    jl_fatal_err(NULL, "Zero length field %s.", name);
  }
  if (jl_pis(self, JL_ENTITY_UNION) && !name) {
    return;
  }
  m = jl_field(name, type);
  m.field.field_width = width;
  if (jl_pis(self, JL_ENTITY_STRUCT)) {
    prev = &adt_vector_back(self->structure.fields);
    if (!prev || !pack_field(prev, &m)) {
      m.field.field_offset = 0;
      m.field.offset = adjust_member_alignment(self, type);
    }
  }
  if (!width) {
    reset_field_alignment(self);
  } else {
    add_member(self, m);
  }
}

void type_add_anonymous_member(jl_entity_t *self, jl_type_t type) {
  int i;
  size_t offset;
  jl_entity_t m;
  jl_entity_t *member;
  jl_entity_r entities;

  if (!jl_is(type, JL_TYPE_COMPOUND)) {
    // TODO: handle error
    exit(1);
  }
  member = type.compound.entity;
  switch (member->kind) {
    case JL_ENTITY_STRUCT:
      entities = member->structure.fields;
      break;
    case JL_ENTITY_UNION:
      entities = member->u_structure.fields;
      break;
    default:
      return;
  }
  if (jl_pis(self, JL_ENTITY_STRUCT) && jl_pis(member, JL_ENTITY_UNION)) {
    offset = adjust_member_alignment(self, type);
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      m.field.offset += offset;
      add_member(self, m);
    }
  } else if (jl_pis(self, JL_ENTITY_UNION) && jl_pis(member, JL_ENTITY_STRUCT)) {
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      add_member(self, m);
    }
  } else {
    for (i = 0; i < adt_vector_size(entities); ++i) {
      m = adt_vector_at(entities, i);
      type_add_member(self, m.name, m.type);
    }
  }
}

void type_seal(jl_entity_t *self) {
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

jl_entity_t jl_struct(const char *name, jl_field_t *fields) {
  size_t m = 0;
  jl_entity_t field, entity = (jl_entity_t) {
    .kind = JL_ENTITY_STRUCT,
    .name = name
  };
  if (fields) {
    while (fields->name) {
      field = jl_field(fields->name, fields->type);
      if (entity.size % field.field.offset) {
        entity.size += field.field.offset - (entity.size % field.field.offset);
        assert(entity.size % field.field.offset == 0);
      }
      entity.field.offset = entity.size;
      if (entity.size < field.field.offset + field.type.size) {
        entity.size = field.field.offset + field.type.size;
      }
      if (field.align > m) m = field.align;
      adt_vector_push(entity.structure.fields, field);
      ++fields;
    }
    assert(m);
    entity.align = m;
  }
  return entity;
}

void jl_struct_init(jl_entity_t *self, const char *name, jl_field_t *fields) {
  jl_entity_t field;
  unsigned i;

  jl_entity_switch(self, JL_ENTITY_STRUCT);
  jl_pentity_struct(self)->name = name;
  jl_pentity_struct(self)->fields = (jl_entity_r) {0};
  if (fields) for (i = 0; i < n_fields; ++i) {
    jl_entity_add_field(self, fields[i].name, fields[i].type);
    field = adt_vector_back(jl_pu(self, struct)->fields);
    jl_entity_field(field)->field_offset = fields[i].field_offset;
    jl_entity_field(field)->field_width = fields[i].field_width;
  }
  jl_entity_update_size(self);
}

void jl_struct_dtor(jl_struct_t *self) {
  jl_entity_t field;

  adt_vector_foreach(self->fields, field) {
    jl_entity_dtor(&field);
  }
  adt_vector_dtor(self->fields);
}


jl_entity_t jl_union_undefined() {
  return (jl_entity_t) {JL_ENTITY_UNION};
}

jl_entity_t jl_union(const char *name, jl_field_t *fields) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_union_init(&entity, name, fields, n_fields);
  return entity;
}

jl_entity_t jl_union_anonymous(jl_field_t *fields) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_union_init(&entity, NULL, fields, n_fields);
  return entity;
}

void jl_union_init(jl_entity_t *self, const char *name, jl_field_t *fields) {
  jl_entity_t field;
  unsigned i;

  jl_entity_switch(self, JL_ENTITY_UNION);
  jl_pentity_union(self)->name = name;
  jl_pentity_union(self)->fields = (jl_entity_r) {0};
  if (fields) for (i = 0; i < n_fields; ++i) {
      jl_entity_add_field(self, fields[i].name, fields[i].type);
      field = adt_vector_back(jl_pu(self, struct)->fields);
      jl_entity_field(field)->field_offset = fields[i].field_offset;
      jl_entity_field(field)->field_width = fields[i].field_width;
    }
  jl_entity_update_size(self);
}

void jl_union_dtor(jl_union_t *self) {
  jl_entity_t field;

  adt_vector_foreach(self->fields, field) {
    jl_entity_dtor(&field);
  }
  adt_vector_dtor(self->fields);
}


void jl_label_dtor(jl_label_t *self) {}
