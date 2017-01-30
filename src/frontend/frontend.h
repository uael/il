#ifndef _JAYL_FRONTEND_H
#define _JAYL_FRONTEND_H

#include <vector>
#include <string>
#include <fstream>

namespace jayl {
  class ParseError;
  namespace internal {
    class ProgramContext;

/// Provides methods to convert Jayl-formated strings and files to Jayl IR.
///
/// Strings and files can be parsed using the \ref parseString and
/// \ref parseFile methods and the resulting IR can be retrieved using the
/// \ref getIR method. If the parse methods return an error value information
/// about the errors can be retrieved using the getErrors method.
    class Frontend {
      public:
      /// Parses, typechecks and turns a given Jayl-formated stream into Jayl IR.
      int parseStream(std::istream &programStream, ProgramContext *ctx,
        std::vector<ParseError> *errors);

      /// Parses, typechecks and turns a given Jayl-formated string into Jayl IR.
      int parseString(const std::string &programString, ProgramContext *ctx,
        std::vector<ParseError> *errors);

      /// Parses, typechecks and turns a given Jayl-formated file into Jayl IR.
      int parseFile(const std::string &filename, ProgramContext *ctx,
        std::vector<ParseError> *errors);
    };

  }
} // namespace jayl::internal

#endif

