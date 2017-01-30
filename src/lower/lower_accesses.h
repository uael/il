#ifndef _JAYL_LOWER_ACCESSES_H
#define _JAYL_LOWER_ACCESSES_H

#include "ir.h"

namespace jayl {
  namespace ir {

/// Lower tensor reads and writes to loads and stores. Loads are lowered based
/// on the storage scheme of the 
    Func lowerTensorAccesses(Func func);

    Func lowerFieldAccesses(Func func);

  }
}
#endif
