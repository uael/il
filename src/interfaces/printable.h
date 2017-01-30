#ifndef _JAYL_PRINTABLE_H
#define _JAYL_PRINTABLE_H

#include <ostream>

namespace jayl {
  namespace interfaces {

    class Printable {
      public:
      virtual void print(std::ostream &os) const = 0;
    };

    inline std::ostream &operator<<(std::ostream &os, const Printable &printable) {
      printable.print(os);
      return os;
    }

  }
}
#endif
