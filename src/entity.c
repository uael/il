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

#include <adt/xmalloc.h>
#include <entity_t.h>

#include "entity.h"

#include "expr.h"
#include "stmt.h"
#include "type.h"

void jl_field_dtor(jl_field_t *self);
void jl_var_dtor(jl_var_t *self);
void jl_param_dtor(jl_param_t *self);
void jl_func_dtor(jl_func_t *self);
void jl_enum_dtor(jl_enum_t *self);
void jl_struct_dtor(jl_struct_t *self);
void jl_union_dtor(jl_union_t *self);
void jl_label_dtor(jl_label_t *self);

jl_entity_t jl_entity_undefined() {
  return (jl_entity_t) {JL_ENTITY_UNDEFINED};
}

void jl_entity_undef(jl_entity_t *self) {
  *self = jl_entity_undefined();
}

void jl_entity_dtor(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_UNDEFINED:
      return;
    case JL_ENTITY_FIELD:
      if (self->u._field && self->u._field->refs <= 0) {
        jl_field_dtor(self->u._field);
        free(self->u._field);
        self->u._field = NULL;
      }
      break;
    case JL_ENTITY_VAR:
      if (self->u._var && self->u._var->refs <= 0) {
        jl_var_dtor(self->u._var);
        free(self->u._var);
        self->u._var = NULL;
      }
      break;
    case JL_ENTITY_PARAM:
      if (self->u._param && self->u._param->refs <= 0) {
        jl_param_dtor(self->u._param);
        free(self->u._param);
        self->u._param = NULL;
      }
      break;
    case JL_ENTITY_FUNC:
      if (self->u._func && self->u._func->refs <= 0) {
        jl_func_dtor(self->u._func);
        free(self->u._func);
        self->u._func = NULL;
      }
      break;
    case JL_ENTITY_ENUM:
      if (self->u._enum && self->u._enum->refs <= 0) {
        jl_enum_dtor(self->u._enum);
        free(self->u._enum);
        self->u._enum = NULL;
      }
      break;
    case JL_ENTITY_STRUCT:
      if (self->u._struct && self->u._struct->refs <= 0) {
        jl_struct_dtor(self->u._struct);
        free(self->u._struct);
        self->u._struct = NULL;
      }
      break;
    case JL_ENTITY_UNION:
      if (self->u._union && self->u._union->refs <= 0) {
        jl_union_dtor(self->u._union);
        free(self->u._union);
        self->u._union = NULL;
      }
      break;
    case JL_ENTITY_LABEL:
      if (self->u._label && self->u._label->refs <= 0) {
        jl_label_dtor(self->u._label);
        free(self->u._label);
        self->u._label = NULL;
      }
      break;
    default:
      break;
  }
  *self = jl_entity_undefined();
}

void jl_entity_switch(jl_entity_t *self, enum jl_entity_n kind) {
  if (self->kind != kind || !jl_entity_is_defined(self)) {
    jl_entity_dtor(self);
    *self = (jl_entity_t) {
      .kind = kind
    };
    switch (kind) {
      case JL_ENTITY_UNDEFINED:
        break;
      case JL_ENTITY_FIELD:
        self->u._field = xmalloc(sizeof(jl_field_t));
        *self->u._field = (jl_field_t) {.refs = 0};
        break;
      case JL_ENTITY_VAR:
        self->u._var = xmalloc(sizeof(jl_var_t));
        *self->u._var = (jl_var_t) {.refs = 0};
        break;
      case JL_ENTITY_PARAM:
        self->u._param = xmalloc(sizeof(jl_param_t));
        *self->u._param = (jl_param_t) {.refs = 0};
        break;
      case JL_ENTITY_FUNC:
        self->u._func = xmalloc(sizeof(jl_func_t));
        *self->u._func = (jl_func_t) {.refs = 0};
        break;
      case JL_ENTITY_ENUM:
        self->u._enum = xmalloc(sizeof(jl_enum_t));
        *self->u._enum = (jl_enum_t) {.refs = 0};
        break;
      case JL_ENTITY_STRUCT:
        self->u._struct = xmalloc(sizeof(jl_struct_t));
        *self->u._struct = (jl_struct_t) {.refs = 0};
        break;
      case JL_ENTITY_UNION:
        self->u._union = xmalloc(sizeof(jl_union_t));
        *self->u._union = (jl_union_t) {.refs = 0};
        break;
      case JL_ENTITY_LABEL:
        self->u._label = xmalloc(sizeof(jl_label_t));
        *self->u._label = (jl_label_t) {.refs = 0};
        break;
    }
  }
}

void jl_entity_acquire(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_UNDEFINED:
      puts("cannot acquire undefined type");
      exit(1);
    case JL_ENTITY_FIELD:
      ++self->u._field->refs;
      break;
    case JL_ENTITY_VAR:
      ++self->u._var->refs;
      break;
    case JL_ENTITY_PARAM:
      ++self->u._param->refs;
      break;
    case JL_ENTITY_FUNC:
      ++self->u._func->refs;
      break;
    case JL_ENTITY_ENUM:
      ++self->u._enum->refs;
      break;
    case JL_ENTITY_STRUCT:
      ++self->u._struct->refs;
      break;
    case JL_ENTITY_UNION:
      ++self->u._union->refs;
      break;
    case JL_ENTITY_LABEL:
      ++self->u._label->refs;
      break;
    default:
      break;
  }
}

void jl_entity_release(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_UNDEFINED:
      puts("cannot release undefined type");
      exit(1);
    case JL_ENTITY_FIELD:
      --self->u._field->refs;
      break;
    case JL_ENTITY_VAR:
      --self->u._var->refs;
      break;
    case JL_ENTITY_PARAM:
      --self->u._param->refs;
      break;
    case JL_ENTITY_FUNC:
      --self->u._func->refs;
      break;
    case JL_ENTITY_ENUM:
      --self->u._enum->refs;
      break;
    case JL_ENTITY_STRUCT:
      --self->u._struct->refs;
      break;
    case JL_ENTITY_UNION:
      --self->u._union->refs;
      break;
    case JL_ENTITY_LABEL:
      --self->u._label->refs;
      break;
    default:
      break;
  }
}

void jl_entity_update_size(jl_entity_t *self) {
  jl_entity_r fields = jl_entity_fields(*self);
  jl_entity_t entity;

  self->size = 0;
  adt_vector_foreach(fields, entity) {
    self->size += jl_entity_type(entity).size;
  }
}

bool jl_entity_is_defined(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_FIELD:
      return self->u._field != NULL;
    case JL_ENTITY_VAR:
      return self->u._var != NULL;
    case JL_ENTITY_PARAM:
      return self->u._param != NULL;
    case JL_ENTITY_FUNC:
      return self->u._func != NULL;
    case JL_ENTITY_ENUM:
      return self->u._enum != NULL;
    case JL_ENTITY_STRUCT:
      return self->u._struct != NULL;
    case JL_ENTITY_UNION:
      return self->u._union != NULL;
    case JL_ENTITY_LABEL:
      return self->u._label != NULL;
    case JL_ENTITY_UNDEFINED:
    default:
      return false;
  }
}

bool jl_entity_equals(jl_entity_t a, jl_entity_t b) {
  unsigned i;
  jl_entity_r a_fields, b_fields;
  jl_type_t a_type;

  if (a.kind != b.kind) {
    return false;
  }
  a_type = jl_entity_type(a);
  if (!jl_type_is_unsigned(a_type)) {
    return jl_type_equals(a_type, jl_entity_type(b));
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
      return self.u._func->params;
    case JL_ENTITY_ENUM:
      return self.u._enum->vars;
    case JL_ENTITY_STRUCT:
      return self.u._struct->fields;
    case JL_ENTITY_UNION:
      return self.u._union->fields;
    default:
      return (jl_entity_r) {0};
  }
}

jl_field_t *jl_entity_field_lookup(jl_entity_t self, const char *name) {
  jl_entity_r fields;
  jl_entity_t field;

  switch (self.kind) {
    case JL_ENTITY_STRUCT:
      fields = jl_entity_struct(self)->fields;
      adt_vector_foreach(fields, field) {
        if (strcmp(name, jl_entity_field(field)->name) == 0) {
          return jl_entity_field(field);
        }
      }
      break;
    case JL_ENTITY_UNION:
      fields = jl_entity_union(self)->fields;
      adt_vector_foreach(fields, field) {
        if (strcmp(name, jl_entity_field(field)->name) == 0) {
          return jl_entity_field(field);
        }
      }
      break;
    default:
      break;
  }
  return NULL;
}

const char *jl_entity_name(jl_entity_t self) {
  switch (self.kind) {
    case JL_ENTITY_FIELD:
      return self.u._field->name;
    case JL_ENTITY_VAR:
      return self.u._var->name;
    case JL_ENTITY_PARAM:
      return self.u._param->name;
    case JL_ENTITY_FUNC:
      return self.u._func->name;
    case JL_ENTITY_ENUM:
      return self.u._enum->name;
    case JL_ENTITY_STRUCT:
      return self.u._struct->name;
    case JL_ENTITY_UNION:
      return self.u._union->name;
    case JL_ENTITY_LABEL:
      return self.u._label->name;
    default:
      break;
  }
  return NULL;
}

jl_type_t jl_entity_type(jl_entity_t self) {
  switch (self.kind) {
    case JL_ENTITY_FIELD:
      return self.u._field->type;
    case JL_ENTITY_VAR:
      return self.u._var->type;
    case JL_ENTITY_PARAM:
      return self.u._param->type;
    case JL_ENTITY_FUNC:
      return jl_pointer(jl_compound(self));
    default:
      return jl_type_undefined();
  }
}


void jl_entity_add_field(jl_entity_t *self, const char *name, jl_type_t type) {
  jl_entity_t field;

  switch (self->kind) {
    case JL_ENTITY_FUNC:
      if (jl_type_is_array(type)) {
        type = jl_pointer(jl_type_array(type)->of);
      }
      jl_param_init(&field, (unsigned) adt_vector_length(self->u._func->params), name, type, jl_expr_undefined());
      adt_vector_push(self->u._func->params, field);
      break;
    case JL_ENTITY_ENUM:
      jl_var_init(&field, name, type, jl_expr_undefined());
      adt_vector_push(self->u._enum->vars, field);
      break;
    case JL_ENTITY_STRUCT:
      jl_entity_switch(&field, JL_ENTITY_FIELD);
      field.u._field->name = name;
      field.u._field->type = type;
      field.u._field->offset = jl_type_alignment(type);
      if (self->size % field.u._field->offset) {
        self->size += field.u._field->offset - (self->size % field.u._field->offset);
        assert(self->size % field.u._field->offset == 0);
      }
      field.u._field->offset = self->size;
      if (LONG_MAX - field.u._field->offset < type.size) {
        puts("Object is too large.");
        exit(1);
      }
      if (self->size < field.u._field->offset + type.size) {
        self->size = field.u._field->offset + type.size;
      }
      adt_vector_push(self->u._struct->fields, field);
      break;
    case JL_ENTITY_UNION:
      jl_entity_switch(&field, JL_ENTITY_FIELD);
      field.u._field->name = name;
      field.u._field->type = type;
      adt_vector_push(self->u._union->fields, field);
      break;
    default:
      break;
  }
}

void jl_field_dtor(jl_field_t *self) {
  jl_type_dtor(&self->type);
}


jl_entity_t jl_var_undefined() {
  return (jl_entity_t) {JL_ENTITY_VAR};
}

jl_entity_t jl_var_int(const char *name, int d) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_var_init(&entity, name, jl_int(), jl_const_int(d));
  return entity;
}

jl_entity_t jl_var_float(const char *name, float f) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_var_init(&entity, name, jl_float(), jl_const_float(f));
  return entity;
}

jl_entity_t jl_var_string(const char *name, const char *s) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_var_init(&entity, name, jl_pointer(jl_char()), jl_const_string(s));
  return entity;
}

jl_entity_t jl_var(const char *name, jl_type_t type, jl_expr_t initializer) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_var_init(&entity, name, type, initializer);
  return entity;
}

void jl_var_init(jl_entity_t *self, const char *name, jl_type_t type, jl_expr_t initializer) {
  jl_entity_switch(self, JL_ENTITY_VAR);
  jl_pentity_var(self)->name = name;
  jl_pentity_var(self)->type = type;
  jl_pentity_var(self)->initializer = initializer;
}

void jl_var_dtor(jl_var_t *self) {
  jl_type_dtor(&self->type);
  jl_expr_dtor(&self->initializer);
}


jl_entity_t jl_param_undefined() {
  return (jl_entity_t) {JL_ENTITY_PARAM};
}

jl_entity_t jl_param_int(unsigned position, const char *name, int d) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_param_init(&entity, position, name, jl_int(), jl_const_int(d));
  return entity;
}

jl_entity_t jl_param_float(unsigned position, const char *name, float f) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_param_init(&entity, position, name, jl_float(), jl_const_float(f));
  return entity;
}

jl_entity_t jl_param_string(unsigned position, const char *name, const char *s) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_param_init(&entity, position, name, jl_pointer(jl_char()), jl_const_string(s));
  return entity;
}

void jl_param_init(jl_entity_t *self, unsigned position, const char *name, jl_type_t type, jl_expr_t initializer) {
  jl_entity_switch(self, JL_ENTITY_PARAM);
  jl_pentity_param(self)->position = position;
  jl_pentity_param(self)->name = name;
  jl_pentity_param(self)->type = type;
  jl_pentity_param(self)->initializer = initializer;
}

void jl_param_dtor(jl_param_t *self) {
  jl_type_dtor(&self->type);
  jl_expr_dtor(&self->initializer);
}


jl_entity_t jl_func_undefined() {
  return (jl_entity_t) {JL_ENTITY_FUNC};
}

jl_entity_t jl_func_decl(jl_type_t return_type, const char *name, jl_entity_r params) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_func_init(&entity, return_type, name, params, jl_stmt_undefined());
  return entity;
}

jl_entity_t jl_proc_decl(const char *name, jl_entity_r params) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_func_init(&entity, jl_void(), name, params, jl_stmt_undefined());
  return entity;
}

jl_entity_t jl_func_def(jl_entity_t prototype, jl_stmt_t body) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_func_init(
    &entity,
    jl_entity_func(prototype)->return_type,
    jl_entity_func(prototype)->name,
    jl_entity_func(prototype)->params,
    body
  );
  return entity;
}

void jl_func_init(jl_entity_t *self, jl_type_t r, const char * n, jl_entity_r p, jl_stmt_t b) {
  jl_entity_switch(self, JL_ENTITY_FUNC);
  jl_pentity_func(self)->return_type = r;
  jl_pentity_func(self)->name = n;
  jl_pentity_func(self)->params = p;
  jl_pentity_func(self)->body = b;
  jl_entity_update_size(self);
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


jl_entity_t jl_enum_undefined() {
  return (jl_entity_t) {JL_ENTITY_ENUM};
}

jl_entity_t jl_enum(const char *name, jl_entity_r fields) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_enum_init(&entity, name, fields);
  return entity;
}

jl_entity_t jl_enum_anonymous(jl_entity_r fields) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_enum_init(&entity, NULL, fields);
  return entity;
}

void jl_enum_init(jl_entity_t *self, const char *name, jl_entity_r fields) {
  jl_entity_switch(self, JL_ENTITY_ENUM);
  jl_pentity_enum(self)->name = name;
  jl_pentity_enum(self)->vars = fields;
  jl_entity_update_size(self);
}

void jl_enum_dtor(jl_enum_t *self) {
  jl_entity_t var;

  adt_vector_foreach(self->vars, var) {
    jl_entity_dtor(&var);
  }
  adt_vector_dtor(self->vars);
}


jl_entity_t jl_struct_undefined() {
  return (jl_entity_t) {JL_ENTITY_STRUCT};
}

jl_entity_t jl_struct(const char *name, jl_field_t *fields, size_t n_fields) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_struct_init(&entity, name, fields, n_fields);
  return entity;
}

jl_entity_t jl_struct_anonymous(jl_field_t *fields, size_t n_fields) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_struct_init(&entity, NULL, fields, n_fields);
  return entity;
}

void jl_struct_init(jl_entity_t *self, const char *name, jl_field_t *fields, size_t n_fields) {
  jl_entity_t field;
  unsigned i;

  jl_entity_switch(self, JL_ENTITY_STRUCT);
  jl_pentity_struct(self)->name = name;
  jl_pentity_struct(self)->fields = (jl_entity_r) {0};
  if (fields) for (i = 0; i < n_fields; ++i) {
    jl_entity_add_field(self, fields[i].name, fields[i].type);
    field = adt_vector_back(self->u._struct->fields);
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

jl_entity_t jl_union(const char *name, jl_field_t *fields, size_t n_fields) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_union_init(&entity, name, fields, n_fields);
  return entity;
}

jl_entity_t jl_union_anonymous(jl_field_t *fields, size_t n_fields) {
  jl_entity_t entity = {JL_ENTITY_UNDEFINED};

  jl_union_init(&entity, NULL, fields, n_fields);
  return entity;
}

void jl_union_init(jl_entity_t *self, const char *name, jl_field_t *fields, size_t n_fields) {
  jl_entity_t field;
  unsigned i;

  jl_entity_switch(self, JL_ENTITY_UNION);
  jl_pentity_union(self)->name = name;
  jl_pentity_union(self)->fields = (jl_entity_r) {0};
  if (fields) for (i = 0; i < n_fields; ++i) {
      jl_entity_add_field(self, fields[i].name, fields[i].type);
      field = adt_vector_back(self->u._struct->fields);
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
