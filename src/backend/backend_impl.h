#ifndef _JAYL_BACKEND_IMPL_H
#define _JAYL_BACKEND_IMPL_H

#include <set>
#include "interfaces/uncopyable.h"

namespace jayl {
  namespace ir {
    class Var;

    class Func;

    class Storage;
  }
  namespace backend {
    class Function;

    class BackendImpl : jayl::interfaces::Uncopyable {
      public:
      virtual ~BackendImpl() {}

      /// Compile the closure consisting of the function and a context.
      virtual Function *compile(ir::Func func, const ir::Storage &storage) = 0;
    };

  }
}
#endif
