#include "indexvar.h"

#include "ir.h"

namespace jayl {
  namespace ir {

// class IndexVarContent
    IndexVarContent::~IndexVarContent() {
      delete fixedExpr;
    }

// class IndexVar
    std::ostream &operator<<(std::ostream &os, const IndexVar &var) {
      iassert(var.defined()) << "Undefined IndexVar";
      if (var.isReductionVar()) {
        os << var.getOperator();
      }
      return os << var.getName();
    }

  }
} // namespace jayl::ir
