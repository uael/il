/// This file converts between types in the jayl namespace and types in the
/// jayl::ir namespace.

#ifndef _JAYL_CONVERT_TYPES_H
#define _JAYL_CONVERT_TYPES_H

#include "tensor_type.h"
#include "types.h"

namespace jayl {
  class TensorType;

  namespace ir {
    class Type;

    ir::Type convert(const jayl::TensorType &tensorType);

    jayl::TensorType convert(const ir::Type &tensorType);

  }
}
#endif
