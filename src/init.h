#ifndef INIT_H
#define INIT_H

#include <algorithm>
#include <string>

#include "error.h"
#include "ir.h"
#include "program.h"

namespace jayl {

  extern const std::vector<std::string> VALID_BACKENDS;
  extern std::string kBackend;
  extern bool kIndexlessStencils;

// Settings struct with default values
  struct Settings {
    std::string backend = "target-c";
    int floatSize = 8;
    bool stencils = false;
  };

  inline void init(const Settings &settings) {
    // backend
    uassert(std::find(VALID_BACKENDS.begin(), VALID_BACKENDS.end(),
      settings.backend) != VALID_BACKENDS.end())
      << "Invalid backend: " << settings.backend;
    kBackend = settings.backend;

    // floatSize
    uassert(settings.floatSize == 4 ||
      settings.floatSize == 8)
      << "Invalid float bytes: " << settings.floatSize;
    ir::ScalarType::floatBytes = settings.floatSize;

    // stencils
    kIndexlessStencils = settings.stencils;
  }

  inline void init(std::string backend = "cpu", int floatSize = 8) {
    Settings settings;
    settings.backend = backend;
    settings.floatSize = floatSize;
    init(settings);
  }

}  // namespace jayl

#endif
