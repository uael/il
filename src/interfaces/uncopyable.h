#ifndef _JAYL_UNCOPYABLE_H
#define _JAYL_UNCOPYABLE_H

namespace jayl {
  namespace interfaces {

    class Uncopyable {
      protected:
      Uncopyable() = default;

      ~Uncopyable() = default;

      private:
      Uncopyable(const Uncopyable &) = delete;

      Uncopyable &operator=(const Uncopyable &) = delete;
    };

  }
}
#endif
