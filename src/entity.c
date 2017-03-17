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

#include <adt/xmalloc.h>

#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

#define jl_entity_var(e) ((void) assert(jl_entity_is_var(*e)), (e)->_var)
#define jl_entity_param(e) ((void) assert(jl_entity_is_param(*e)), (e)->_param)
#define jl_entity_func(e) ((void) assert(jl_entity_is_func(*e)), (e)->_func)
#define jl_entity_enum(e) ((void) assert(jl_entity_is_enum(*e)), (e)->_enum)
#define jl_entity_struct(e) ((void) assert(jl_entity_is_struct(*e)), (e)->_struct)
#define jl_entity_union(e) ((void) assert(jl_entity_is_union(*e)), (e)->_union)
#define jl_entity_label(e) ((void) assert(jl_entity_is_label(*e)), (e)->_label)

typedef struct jl_var_t jl_var_t;
typedef struct jl_param_t jl_param_t;
typedef struct jl_func_t jl_func_t;
typedef struct jl_enum_t jl_enum_t;
typedef struct jl_struct_t jl_struct_t;
typedef struct jl_union_t jl_union_t;
typedef struct jl_label_t jl_label_t;

void jl_var_dtor(jl_entity_t *self);
void jl_param_dtor(jl_entity_t *self);
void jl_func_dtor(jl_entity_t *self);
void jl_enum_dtor(jl_entity_t *self);
void jl_struct_dtor(jl_entity_t *self);
void jl_union_dtor(jl_entity_t *self);
void jl_label_dtor(jl_entity_t *self);

struct jl_var_t {
  unsigned refs;
  const char *name;
  jl_type_t type;
  jl_expr_t initializer;
};

struct jl_param_t {
  unsigned refs;
  const char *name;
  unsigned position;
  jl_type_t type;
  jl_expr_t initializer;
};

struct jl_func_t {
  unsigned refs;
  jl_func_specifier_n specifiers;
  const char *name;
  jl_type_t return_type;
  jl_entity_r params;
  jl_stmt_t body;
};

struct jl_enum_t {
  unsigned refs;
  const char *name;
  jl_entity_r fields;
};

struct jl_struct_t {
  unsigned refs;
  const char *name;
  jl_entity_r fields;
};

struct jl_union_t {
  unsigned refs;
  const char *name;
  jl_entity_r fields;
};

struct jl_label_t {
  unsigned refs;
  const char *name;
  jl_stmt_t next;
};


jl_entity_t jl_entity_undefined() {
  return (jl_entity_t) {JL_ENTITY_UNDEFINED};
}

void jl_entity_dtor(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_UNDEFINED:
      return;
    case JL_ENTITY_VAR:
      if (self->_var->refs <= 0) {
        jl_var_dtor(self);
        free(self->_var);
        self->_var = NULL;
      }
      break;
    case JL_ENTITY_PARAM:
      if (self->_param->refs <= 0) {
        jl_param_dtor(self);
        free(self->_param);
        self->_param = NULL;
      }
      break;
    case JL_ENTITY_FUNC:
      if (self->_func->refs <= 0) {
        jl_func_dtor(self);
        free(self->_func);
        self->_func = NULL;
      }
      break;
    case JL_ENTITY_ENUM:
      if (self->_enum->refs <= 0) {
        jl_enum_dtor(self);
        free(self->_enum);
        self->_enum = NULL;
      }
      break;
    case JL_ENTITY_STRUCT:
      if (self->_struct->refs <= 0) {
        jl_struct_dtor(self);
        free(self->_struct);
        self->_struct = NULL;
      }
      break;
    case JL_ENTITY_UNION:
      if (self->_union->refs <= 0) {
        jl_union_dtor(self);
        free(self->_union);
        self->_union = NULL;
      }
      break;
    case JL_ENTITY_LABEL:
      if (self->_label->refs <= 0) {
        jl_label_dtor(self);
        free(self->_label);
        self->_label = NULL;
      }
      break;
  }
  *self = jl_entity_undefined();
}

void jl_entity_switch(jl_entity_t *self, jl_entity_n kind) {
  if (self->kind != kind || !jl_entity_is_defined(self)) {
    jl_entity_dtor(self);
    *self = (jl_entity_t) {
      .kind = kind
    };
    switch (kind) {
      case JL_ENTITY_UNDEFINED:
        break;
      case JL_ENTITY_VAR:
        self->_var = xmalloc(sizeof(jl_var_t));
        break;
      case JL_ENTITY_PARAM:
        self->_param = xmalloc(sizeof(jl_param_t));
        break;
      case JL_ENTITY_FUNC:
        self->_func = xmalloc(sizeof(jl_func_t));
        break;
      case JL_ENTITY_ENUM:
        self->_enum = xmalloc(sizeof(jl_enum_t));
        break;
      case JL_ENTITY_STRUCT:
        self->_struct = xmalloc(sizeof(jl_struct_t));
        break;
      case JL_ENTITY_UNION:
        self->_union = xmalloc(sizeof(jl_union_t));
        break;
      case JL_ENTITY_LABEL:
        self->_label = xmalloc(sizeof(jl_label_t));
        break;
    }
  }
}

void jl_entity_acquire(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_UNDEFINED:
      puts("cannot acquire undefined type");
      exit(1);
    case JL_ENTITY_VAR:
      ++self->_var->refs;
      break;
    case JL_ENTITY_PARAM:
      ++self->_param->refs;
      break;
    case JL_ENTITY_FUNC:
      ++self->_func->refs;
      break;
    case JL_ENTITY_ENUM:
      ++self->_enum->refs;
      break;
    case JL_ENTITY_STRUCT:
      ++self->_struct->refs;
      break;
    case JL_ENTITY_UNION:
      ++self->_union->refs;
      break;
    case JL_ENTITY_LABEL:
      ++self->_label->refs;
      break;
  }
}

void jl_entity_release(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_UNDEFINED:
      puts("cannot release undefined type");
      exit(1);
    case JL_ENTITY_VAR:
      --self->_var->refs;
      break;
    case JL_ENTITY_PARAM:
      --self->_param->refs;
      break;
    case JL_ENTITY_FUNC:
      --self->_func->refs;
      break;
    case JL_ENTITY_ENUM:
      --self->_enum->refs;
      break;
    case JL_ENTITY_STRUCT:
      --self->_struct->refs;
      break;
    case JL_ENTITY_UNION:
      --self->_union->refs;
      break;
    case JL_ENTITY_LABEL:
      --self->_label->refs;
      break;
  }
}

bool jl_entity_is_defined(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_VAR:
      return self->_var != NULL;
    case JL_ENTITY_PARAM:
      return self->_param != NULL;
    case JL_ENTITY_FUNC:
      return self->_func != NULL;
    case JL_ENTITY_ENUM:
      return self->_enum != NULL;
    case JL_ENTITY_STRUCT:
      return self->_struct != NULL;
    case JL_ENTITY_UNION:
      return self->_union != NULL;
    case JL_ENTITY_LABEL:
      return self->_label != NULL;
    case JL_ENTITY_UNDEFINED:
    default:
      return false;
  }
}


jl_entity_t jl_var_undefined() {
  return (jl_entity_t) {JL_ENTITY_VAR};
}

jl_entity_t jl_var_int(const char *name, int d) {
  jl_entity_t entity;

  jl_var_init(&entity, name, jl_literal_int(), jl_const_int(d));
  return entity;
}

jl_entity_t jl_var_float(const char *name, float f) {
  jl_entity_t entity;

  jl_var_init(&entity, name, jl_literal_float(), jl_const_float(f));
  return entity;
}

jl_entity_t jl_var_string(const char *name, const char *s) {
  jl_entity_t entity;

  jl_var_init(&entity, name, jl_literal_string(), jl_const_string(s));
  return entity;
}

jl_entity_t jl_var(const char *name, jl_type_t type, jl_expr_t initializer) {
  jl_entity_t entity;

  jl_var_init(&entity, name, type, initializer);
  return entity;
}

void jl_var_init(jl_entity_t *self, const char *name, jl_type_t type, jl_expr_t initializer) {
  jl_entity_switch(self, JL_ENTITY_VAR);
  jl_var_set_name(self, name);
  jl_var_set_type(self, type);
  jl_var_set_initializer(self, initializer);
}

void jl_var_dtor(jl_entity_t *self) {}

const char *jl_var_get_name(jl_entity_t *self) {
  return jl_entity_var(self)->name;
}

void jl_var_set_name(jl_entity_t *self, const char *name) {
  jl_entity_var(self)->name = name;
}

jl_type_t jl_var_get_type(jl_entity_t *self) {
  return jl_entity_var(self)->type;
}

void jl_var_set_type(jl_entity_t *self, jl_type_t type) {
  jl_entity_var(self)->type = type;
}

jl_expr_t jl_var_get_initializer(jl_entity_t *self) {
  return jl_entity_var(self)->initializer;
}

void jl_var_set_initializer(jl_entity_t *self, jl_expr_t initializer) {
  jl_entity_var(self)->initializer = initializer;
}


jl_entity_t jl_param_undefined() {
  return (jl_entity_t) {JL_ENTITY_PARAM};
}

jl_entity_t jl_param_int(unsigned position, const char *name, int d) {
  jl_entity_t entity;

  jl_param_init(&entity, position, name, jl_literal_int(), jl_const_int(d));
  return entity;
}

jl_entity_t jl_param_float(unsigned position, const char *name, float f) {
  jl_entity_t entity;

  jl_param_init(&entity, position, name, jl_literal_float(), jl_const_float(f));
  return entity;
}

jl_entity_t jl_param_string(unsigned position, const char *name, const char *s) {
  jl_entity_t entity;

  jl_param_init(&entity, position, name, jl_literal_string(), jl_const_string(s));
  return entity;
}

void jl_param_init(jl_entity_t *self, unsigned position, const char *name, jl_type_t type, jl_expr_t initializer) {
  jl_entity_switch(self, JL_ENTITY_PARAM);
  jl_param_set_position(self, position);
  jl_param_set_name(self, name);
  jl_param_set_type(self, type);
  jl_param_set_initializer(self, initializer);
}

void jl_param_dtor(jl_entity_t *self) {}

unsigned jl_param_get_position(jl_entity_t *self) {
  return jl_entity_param(self)->position;
}

void jl_param_set_position(jl_entity_t *self, unsigned position) {
  jl_entity_param(self)->position = position;
}

const char *jl_param_get_name(jl_entity_t *self) {
  return jl_entity_param(self)->name;
}

void jl_param_set_name(jl_entity_t *self, const char *name) {
  jl_entity_param(self)->name = name;
}

jl_type_t jl_param_get_type(jl_entity_t *self) {
  return jl_entity_param(self)->type;
}

void jl_param_set_type(jl_entity_t *self, jl_type_t type) {
  jl_entity_param(self)->type = type;
}

jl_expr_t jl_param_get_initializer(jl_entity_t *self) {
  return jl_entity_param(self)->initializer;
}

void jl_param_set_initializer(jl_entity_t *self, jl_expr_t initializer) {
  jl_entity_param(self)->initializer = initializer;
}


jl_entity_t jl_func_undefined() {
  return (jl_entity_t) {JL_ENTITY_FUNC};
}

jl_entity_t jl_func_decl(jl_func_specifier_n specifiers, jl_type_t return_type, const char *name, jl_entity_r params) {
  jl_entity_t entity;

  jl_func_init(&entity, specifiers, return_type, name, params, jl_stmt_undefined());
  return entity;
}

jl_entity_t jl_proc_decl(jl_func_specifier_n specifiers, const char *name, jl_entity_r params) {
  jl_entity_t entity;

  jl_func_init(&entity, specifiers, jl_literal_void(), name, params, jl_stmt_undefined());
  return entity;
}

jl_entity_t jl_func_def(jl_entity_t prototype, jl_stmt_t body) {
  jl_entity_t entity;

  jl_func_init(
    &entity,
    jl_func_get_specifiers(&prototype),
    jl_func_get_return_type(&prototype),
    jl_func_get_name(&prototype),
    jl_func_get_params(&prototype),
    body
  );
  return entity;
}

void jl_func_init(jl_entity_t *self, jl_func_specifier_n s, jl_type_t r, const char * n, jl_entity_r p, jl_stmt_t b) {
  jl_entity_switch(self, JL_ENTITY_FUNC);
  jl_func_set_specifiers(self, s);
  jl_func_set_return_type(self, r);
  jl_func_set_name(self, n);
  jl_func_set_params(self, p);
  jl_func_set_body(self, b);
}

void jl_func_dtor(jl_entity_t *self) {
  jl_vector_dtor(jl_entity_func(self)->params);
}

bool jl_func_is_inline(jl_entity_t *self) {
  return jl_func_get_specifiers(self) & JL_FUNC_SPECIFIER_INLINE;
}

bool jl_func_is_noreturn(jl_entity_t *self) {
  return jl_func_get_specifiers(self) & JL_FUNC_SPECIFIER_NORETURN;
}

jl_func_specifier_n jl_func_get_specifiers(jl_entity_t *self) {
  return jl_entity_func(self)->specifiers;
}

void jl_func_set_specifiers(jl_entity_t *self, jl_func_specifier_n specifiers) {
  jl_entity_func(self)->specifiers = specifiers;
}

void jl_func_add_specifier(jl_entity_t *self, jl_func_specifier_n specifier) {
  jl_entity_func(self)->specifiers |= specifier;
}

void jl_func_rem_specifier(jl_entity_t *self, jl_func_specifier_n specifier) {
  jl_entity_func(self)->specifiers &= ~specifier;
}

const char *jl_func_get_name(jl_entity_t *self) {
  return jl_entity_func(self)->name;
}

void jl_func_set_name(jl_entity_t *self, const char *name) {
  jl_entity_func(self)->name = name;
}

jl_type_t jl_func_get_return_type(jl_entity_t *self) {
  return jl_entity_func(self)->return_type;
}

void jl_func_set_return_type(jl_entity_t *self, jl_type_t return_type) {
  jl_entity_func(self)->return_type = return_type;
}

jl_entity_r jl_func_get_params(jl_entity_t *self) {
  return jl_entity_func(self)->params;
}

void jl_func_set_params(jl_entity_t *self, jl_entity_r params) {
  jl_entity_func(self)->params = params;
}

jl_stmt_t jl_func_get_body(jl_entity_t *self) {
  return jl_entity_func(self)->body;
}

void jl_func_set_body(jl_entity_t *self, jl_stmt_t body) {
  jl_entity_func(self)->body = body;
}


jl_entity_t jl_enum_undefined() {
  return (jl_entity_t) {JL_ENTITY_ENUM};
}

jl_entity_t jl_enum(const char *name, jl_entity_r fields) {
  jl_entity_t entity;

  jl_enum_init(&entity, name, fields);
  return entity;
}

jl_entity_t jl_enum_anonymous(jl_entity_r fields) {
  jl_entity_t entity;

  jl_enum_init(&entity, NULL, fields);
  return entity;
}

void jl_enum_init(jl_entity_t *self, const char *name, jl_entity_r fields) {
  jl_entity_switch(self, JL_ENTITY_ENUM);
  jl_enum_set_name(self, name);
  jl_enum_set_fields(self, fields);
}

void jl_enum_dtor(jl_entity_t *self) {
  jl_vector_dtor(jl_entity_enum(self)->fields);
}

const char *jl_enum_get_name(jl_entity_t *self) {
  return jl_entity_enum(self)->name;
}

void jl_enum_set_name(jl_entity_t *self, const char *name) {
  jl_entity_enum(self)->name = name;
}

jl_entity_r jl_enum_get_fields(jl_entity_t *self) {
  return jl_entity_enum(self)->fields;
}

void jl_enum_set_fields(jl_entity_t *self, jl_entity_r fields) {
  jl_entity_enum(self)->fields = fields;
}


jl_entity_t jl_struct_undefined() {
  return (jl_entity_t) {JL_ENTITY_STRUCT};
}

jl_entity_t jl_struct(const char *name, jl_entity_r fields) {
  jl_entity_t entity;

  jl_struct_init(&entity, name, fields);
  return entity;
}

jl_entity_t jl_struct_anonymous(jl_entity_r fields) {
  jl_entity_t entity;

  jl_struct_init(&entity, NULL, fields);
  return entity;
}

void jl_struct_init(jl_entity_t *self, const char *name, jl_entity_r fields) {
  jl_entity_switch(self, JL_ENTITY_STRUCT);
  jl_struct_set_name(self, name);
  jl_struct_set_fields(self, fields);
}

void jl_struct_dtor(jl_entity_t *self) {
  jl_vector_dtor(jl_entity_struct(self)->fields);
}

const char *jl_struct_get_name(jl_entity_t *self) {
  return jl_entity_struct(self)->name;
}

void jl_struct_set_name(jl_entity_t *self, const char *name) {
  jl_entity_struct(self)->name = name;
}

jl_entity_r jl_struct_get_fields(jl_entity_t *self) {
  return jl_entity_struct(self)->fields;
}

void jl_struct_set_fields(jl_entity_t *self, jl_entity_r fields) {
  jl_entity_struct(self)->fields = fields;
}


jl_entity_t jl_union_undefined() {
  return (jl_entity_t) {JL_ENTITY_UNION};
}

jl_entity_t jl_union(const char *name, jl_entity_r fields) {
  jl_entity_t entity;

  jl_union_init(&entity, name, fields);
  return entity;
}

jl_entity_t jl_union_anonymous(jl_entity_r fields) {
  jl_entity_t entity;

  jl_union_init(&entity, NULL, fields);
  return entity;
}

void jl_union_init(jl_entity_t *self, const char *name, jl_entity_r fields) {
  jl_entity_switch(self, JL_ENTITY_UNION);
  jl_union_set_name(self, name);
  jl_union_set_fields(self, fields);
}

void jl_union_dtor(jl_entity_t *self) {
  jl_vector_dtor(jl_entity_union(self)->fields);
}

const char *jl_union_get_name(jl_entity_t *self) {
  return jl_entity_union(self)->name;
}

void jl_union_set_name(jl_entity_t *self, const char *name) {
  jl_entity_union(self)->name = name;
}

jl_entity_r jl_union_get_fields(jl_entity_t *self) {
  return jl_entity_union(self)->fields;
}

void jl_union_set_fields(jl_entity_t *self, jl_entity_r fields) {
  jl_entity_union(self)->fields = fields;
}


void jl_label_dtor(jl_entity_t *self) {}
