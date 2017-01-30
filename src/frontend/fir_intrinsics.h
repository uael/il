#ifndef _JAYL_FIR_INTRINSICS_H
#define _JAYL_FIR_INTRINSICS_H

#include <vector>
#include "fir.h"

namespace jayl {
  namespace fir {

    std::vector<fir::FuncDecl::Ptr> createIntrinsics();

  }
}
#endif
