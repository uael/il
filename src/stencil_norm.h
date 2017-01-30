#ifndef _JAYL_STENCIL_NORM_H
#define _JAYL_STENCIL_NORM_H

#include "ir.h"

namespace jayl {
  namespace ir {

/// Rewrite grid relative indexing in assembly functions to normalize
/// the row index to zero for all output tensor writes. This matches the
/// pattern of a gather stencil, and allows easy lowering of the map.
    Func normalizeRowIndices(Func func);

  }
}

#endif
