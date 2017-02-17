#ifndef _JAYL_FIR_TYPE_H_
#define _JAYL_FIR_TYPE_H_

#include "fir_symbol.h"

TYPEDEF_ENUMS(
  fir_type_kind_t,
  fir_scalar_kind_t,
  fir_qualifiers_t
);

TYPEDEF_UNIONS(
  fir_type_t
);

TYPEDEF_STRUCTS(
  fir_type_scalar_t,
  fir_type_array_t,
  fir_type_pointer_t,
  fir_type_function_t,
  fir_type_struct_t,
  fir_type_union_t,
  fir_type_class_t,
  fir_type_frame_t,
  fir_type_enum_t
);

enum _fir_type_kind_t {
  FIR_TYPE_SCALAR,
  FIR_TYPE_ARRAY,
  FIR_TYPE_POINTER,
  FIR_TYPE_FUNCTION,
  FIR_TYPE_STRUCT,
  FIR_TYPE_UNION,
  FIR_TYPE_CLASS,
  FIR_TYPE_FRAME,
  FIR_TYPE_ENUM
};

enum _fir_scalar_kind_t {
  FIR_SCALAR_VOID,
  FIR_SCALAR_BOOL,
  FIR_SCALAR_BYTE,
  FIR_SCALAR_SHORT,
  FIR_SCALAR_INT,
  FIR_SCALAR_LONG,
  FIR_SCALAR_LONGLONG,
  FIR_SCALAR_FLOAT,
  FIR_SCALAR_DOUBLE,
  FIR_SCALAR_LONGDOUBLE
};

enum _fir_qualifiers_t {
  FIR_QUALIFIER_NONE,
  FIR_QUALIFIER_CONST,
  FIR_QUALIFIER_VOLATILE,
  FIR_QUALIFIER_UNSIGNED
};

#define FIR_TYPE_BASE \
  fir_type_kind_t kind; \
  fir_type_t *next; \
  fir_qualifiers_t qualifiers;

struct _fir_type_scalar_t {
  FIR_TYPE_BASE;
};

struct _fir_type_array_t {
  FIR_TYPE_BASE;
};

struct _fir_type_pointer_t {
  FIR_TYPE_BASE;
};

struct _fir_type_function_t {
  FIR_TYPE_BASE;
};

struct _fir_type_struct_t {
  FIR_TYPE_BASE;
};

struct _fir_type_union_t {
  FIR_TYPE_BASE;
};

struct _fir_type_class_t {
  FIR_TYPE_BASE;
};

struct _fir_type_frame_t {
  FIR_TYPE_BASE;
};

struct _fir_type_enum_t {
  FIR_TYPE_BASE;
};

#endif /* _JAYL_FIR_TYPE_H_ */
