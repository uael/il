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

#include "type.h"

#include "entity.h"
#include "expr.h"
#include "stmt.h"

void jl_pointer_dtor(jl_pointer_t *self);
void jl_array_dtor(jl_array_t *self);
void jl_compound_dtor(jl_compound_t *self);

void jl_type_undef(jl_type_t *self) {
  *self = jl_type_undefined();
}

void jl_type_dtor(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_UNDEFINED:
      return;
    case JL_TYPE_POINTER:
      if (jl_pu(self, pointer) && jl_pu(self, pointer)->refs <= 0) {
        jl_pointer_dtor(jl_pu(self, pointer));
        free(jl_pu(self, pointer));
        jl_pu(self, pointer) = NULL;
      }
      break;
    case JL_TYPE_ARRAY:
      if (jl_pu(self, array) && jl_pu(self, array)->refs <= 0) {
        jl_array_dtor(jl_pu(self, array));
        free(jl_pu(self, array));
        jl_pu(self, array) = NULL;
      }
      break;
    case JL_TYPE_COMPOUND:
      if (jl_pu(self, compound) && jl_pu(self, compound)->refs <= 0) {
        jl_compound_dtor(jl_pu(self, compound));
        free(jl_pu(self, compound));
        jl_pu(self, compound) = NULL;
      }
      break;
    default:
      break;
  }
  *self = jl_type_undefined();
}

void jl_type_switch(jl_type_t *self, enum jl_type_n kind) {
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;

  if (self->kind != kind || !jl_ptype_is_defined(self)) {
    specifiers = self->specifiers;
    qualifiers = self->qualifiers;
    if (jl_ptype_is_literal(self)) {
      *self = (jl_type_t) {
        .kind = kind,
        .specifiers = specifiers,
        .qualifiers = qualifiers
      };
    } else {
      jl_type_dtor(self);
      self->kind = kind;
      self->specifiers = specifiers;
      self->qualifiers = qualifiers;
      switch (kind) {
        case JL_TYPE_POINTER:
          jl_pu(self, pointer) = xmalloc(sizeof(jl_pointer_t));
          *jl_pu(self, pointer) = (jl_pointer_t) {.refs = 0};
          break;
        case JL_TYPE_ARRAY:
          jl_pu(self, array) = xmalloc(sizeof(jl_array_t));
          *jl_pu(self, array) = (jl_array_t) {.refs = 0};
          break;
        case JL_TYPE_COMPOUND:
          jl_pu(self, compound) = xmalloc(sizeof(jl_compound_t));
          *jl_pu(self, compound) = (jl_compound_t) {.refs = 0};
          break;
        default:
          break;
      }
    }
    jl_type_update_size(self);
  }
}

void jl_type_acquire(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_UNDEFINED:
      puts("cannot acquire undefined type");
      exit(1);
    case JL_TYPE_POINTER:
      ++jl_pu(self, pointer)->refs;
      break;
    case JL_TYPE_ARRAY:
      ++jl_pu(self, array)->refs;
      break;
    case JL_TYPE_COMPOUND:
      ++jl_pu(self, compound)->refs;
      break;
    default:
      break;
  }
}

void jl_type_release(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_UNDEFINED:
      puts("cannot release undefined type");
      exit(1);
    case JL_TYPE_POINTER:
      --jl_pu(self, pointer)->refs;
      break;
    case JL_TYPE_ARRAY:
      --jl_pu(self, array)->refs;
      break;
    case JL_TYPE_COMPOUND:
      --jl_pu(self, compound)->refs;
      break;
    default:
      break;
  }
}

bool jl_type_is_defined(jl_type_t self) {
  switch (self.kind) {
    case JL_TYPE_POINTER:
      return jl_u(self, pointer) != NULL;
    case JL_TYPE_ARRAY:
      return jl_u(self, array) != NULL;
    case JL_TYPE_COMPOUND:
      return jl_u(self, compound) != NULL;
    case JL_TYPE_UNDEFINED:
      return false;
    default:
      return true;
  }
}

bool jl_ptype_is_defined(jl_type_t *self) {
  return jl_type_is_defined(*self);
}

bool jl_type_is_ref(jl_type_t type) {
  return jl_type_is_array(type) || jl_type_is_pointer(type);
}

bool jl_type_is_func(jl_type_t type) {
  return jl_type_is_compound(type) && jl_entity_is_func(jl_u(type, compound)->entity);
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
  if (a.kind != b.kind || a.size != b.size || jl_type_is_unsigned(a) != jl_type_is_unsigned(b)) {
    return false;
  }
  if (jl_type_is_compound(a)) {
    return jl_entity_equals(jl_type_compound(a)->entity, jl_type_compound(b)->entity);
  }
  return true;
}

jl_type_t jl_type_deref(jl_type_t a) {
  switch (a.kind) {
    case JL_TYPE_POINTER:
      return jl_u(a, pointer)->of;
    case JL_TYPE_ARRAY:
      return jl_u(a, array)->of;
    default:
      exit(1);
  }
}

void jl_type_update_size(jl_type_t *self) {
  self->size = 0;
  switch (self->kind) {
    case JL_TYPE_BOOL:
    case JL_TYPE_CHAR:
      self->size = 1;
      break;
    case JL_TYPE_SHORT:
      self->size = 2;
      break;
    case JL_TYPE_INT:
    case JL_TYPE_FLOAT:
      self->size = 4;
      break;
    case JL_TYPE_LONG:
    case JL_TYPE_DOUBLE:
    case JL_TYPE_LONG_LONG:
      self->size = 8;
      break;
    case JL_TYPE_LONG_DOUBLE:
      self->size = 16;
      break;
    case JL_TYPE_POINTER:
      self->size = 8;
      break;
    case JL_TYPE_ARRAY:
      self->size = jl_pu(jl_expr_const(jl_pu(self, array)->size), ul) * jl_pu(self, array)->of.size;
      break;
    case JL_TYPE_COMPOUND:
      self->size = jl_pu(self, compound)->entity.size;
      break;
    default:
      break;
  }
}

size_t jl_sizeof(jl_type_t type) {
  return type.size;
}

size_t jl_type_alignment(jl_type_t type) {
  size_t m = 0, d;
  jl_entity_t entity;
  jl_entity_r entities;

  assert(!jl_type_is_func(type));
  if (jl_type_is_array(type)) {
    return jl_type_alignment(jl_type_array(type)->of);
  }
  if (jl_type_is_compound(type)) {
    entities = jl_type_fields(type);
    adt_vector_foreach(entities, entity) {
      d = jl_type_alignment(jl_entity_type(entity));
      if (d > m) m = d;
    }
    assert(m);
    return m;
  }
  return jl_sizeof(type);
}

jl_entity_r jl_type_fields(jl_type_t self) {
  return jl_entity_fields(jl_type_compound(self)->entity);
}

jl_field_t *jl_field_lookup(jl_type_t self, const char *name) {
  return jl_entity_field_lookup(jl_type_compound(self)->entity, name);
}


jl_type_t jl_void() {
  static jl_type_t type = {JL_TYPE_VOID};
  return type;
}

jl_type_t jl_bool() {
  static jl_type_t type = {JL_TYPE_DOUBLE};
  return type;
}

jl_type_t jl_char() {
  static jl_type_t type = {JL_TYPE_CHAR};
  return type;
}

jl_type_t jl_short() {
  static jl_type_t type = {JL_TYPE_SHORT};
  return type;
}

jl_type_t jl_int() {
  static jl_type_t type = {JL_TYPE_INT};
  return type;
}

jl_type_t jl_uint() {
  static jl_type_t type = {JL_TYPE_INT, .specifiers = JL_TYPE_SPECIFIER_UNSIGNED};
  return type;
}

jl_type_t jl_long() {
  static jl_type_t type = {JL_TYPE_LONG};
  return type;
}

jl_type_t jl_ulong() {
  static jl_type_t type = {JL_TYPE_LONG, .specifiers = JL_TYPE_SPECIFIER_UNSIGNED};
  return type;
}

jl_type_t jl_double() {
  static jl_type_t type = {JL_TYPE_DOUBLE};
  return type;
}

jl_type_t jl_float() {
  static jl_type_t type = {JL_TYPE_FLOAT};
  return type;
}

jl_type_t jl_long_long() {
  static jl_type_t type = {JL_TYPE_LONG_LONG};
  return type;
}

jl_type_t jl_long_double() {
  static jl_type_t type = {JL_TYPE_LONG_DOUBLE};
  return type;
}

jl_type_t jl_literal(enum jl_type_n kind) {
  jl_type_t type = {kind};

  assert(kind >= JL_TYPE_BOOL && kind <= JL_TYPE_LONG_DOUBLE);
  jl_type_update_size(&type);
  return type;
}


jl_type_t jl_pointer(jl_type_t of) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_pointer_init(&type, of);
  return type;
}

void jl_pointer_init(jl_type_t *self, jl_type_t of) {
  jl_type_switch(self, JL_TYPE_POINTER);
  jl_ptype_pointer(self)->of = of;
  jl_type_update_size(self);
}

void jl_pointer_dtor(jl_pointer_t *self) {
  jl_type_dtor(&self->of);
}


jl_type_t jl_array(jl_type_t of) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_array_init(&type, of, jl_expr_undefined());
  return type;
}

jl_type_t jl_narray(jl_type_t of, jl_expr_t size) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_array_init(&type, of, size);
  return type;
}

void jl_array_init(jl_type_t *self, jl_type_t of, jl_expr_t size) {
  jl_type_switch(self, JL_TYPE_ARRAY);
  jl_ptype_array(self)->of = of;
  jl_ptype_array(self)->size = size;
  jl_type_update_size(self);
}

void jl_array_dtor(jl_array_t *self) {
  jl_type_dtor(&self->of);
  jl_expr_dtor(&self->size);
}


jl_type_t jl_compound(jl_entity_t entity) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_compound_init(&type, entity);
  return type;
}

void jl_compound_init(jl_type_t *self, jl_entity_t entity) {
  jl_type_switch(self, JL_TYPE_COMPOUND);
  jl_ptype_compound(self)->entity = entity;
  jl_type_update_size(self);
}

void jl_compound_dtor(jl_compound_t *self) {
  jl_entity_dtor(&self->entity);
}
