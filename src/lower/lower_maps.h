#ifndef _JAYL_LOWER_MAPS_H
#define _JAYL_LOWER_MAPS_H

#include "ir.h"

namespace jayl {
  namespace ir {

/// Lower map statements to loops. Map assemblies are lowered to loops that
/// store the resulting tensors as specified by Func's Storage descriptor.
    Func lowerMaps(Func func);

  }
}
#endif
