#ifndef _JAYL_FIR_ENTITY_H_
#define _JAYL_FIR_ENTITY_H_

#include <p99.h>

union _fir_type_t;
union _fir_expr_t;
struct _fir_symbol_t;

#define TYPEDEF_STRUCT(s) typedef struct _ ## s s
#define TYPEDEF_STRUCTS(...) P99_SEP(TYPEDEF_STRUCT, __VA_ARGS__)

#define TYPEDEF_UNION(s) typedef union _ ## s s
#define TYPEDEF_UNIONS(...) P99_SEP(TYPEDEF_UNION, __VA_ARGS__)

#define TYPEDEF_ENUM(s) typedef enum _ ## s s
#define TYPEDEF_ENUMS(...) P99_SEP(TYPEDEF_ENUM, __VA_ARGS__)

TYPEDEF_ENUMS(
  fir_ent_kind_t
);

TYPEDEF_UNIONS(
  fir_ent_t
);

TYPEDEF_STRUCTS(
  fir_ent_global_t,
  fir_ent_func_t,
  fir_ent_struct_t,
  fir_ent_union_t,
  fir_ent_class_t,
  fir_ent_frame_t,
  fir_ent_enum_t,
  fir_ent_namespace_t,
  fir_ent_use_t
);

enum _fir_ent_kind_t {
  FIR_ENT_GLOBAL,
  FIR_ENT_FUNC,
  FIR_ENT_STRUCT,
  FIR_ENT_UNION,
  FIR_ENT_CLASS,
  FIR_ENT_FRAME,
  FIR_ENT_ENUM,
  FIR_ENT_NAMESPACE,
  FIR_ENT_USE
};

#define FIR_ENT_BASE \
  fir_ent_kind_t kind; \
  fir_ent_t *next; \
  struct _fir_symbol_t *symbol

struct _fir_ent_global_t {
  FIR_ENT_BASE;
  struct _fir_type_t *type;
  struct _fir_expr_t *value;
};

struct _fir_ent_func_t {
  FIR_ENT_BASE;
};

struct _fir_ent_struct_t {
  FIR_ENT_BASE;
};

struct _fir_ent_union_t {
  FIR_ENT_BASE;
};

struct _fir_ent_class_t {
  FIR_ENT_BASE;
};

struct _fir_ent_frame_t {
  FIR_ENT_BASE;
};

struct _fir_ent_enum_t {
  FIR_ENT_BASE;
};

struct _fir_ent_namespace_t {
  FIR_ENT_BASE;
};

struct _fir_ent_use_t {
  FIR_ENT_BASE;
};

union _fir_ent_t {
  fir_ent_kind_t kind;
  fir_ent_global_t e_global;
  fir_ent_func_t e_func;
  fir_ent_struct_t e_struct;
  fir_ent_union_t e_union;
  fir_ent_class_t e_class;
  fir_ent_frame_t e_frame;
  fir_ent_enum_t e_enum;
  fir_ent_namespace_t e_namespace;
  fir_ent_use_t e_use;
};

#endif /* _JAYL_FIR_ENTITY_H_ */
