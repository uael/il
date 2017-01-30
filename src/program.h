#ifndef _JAYL_PROGRAM_H
#define _JAYL_PROGRAM_H

#include <string>
#include <ostream>
#include <vector>
#include <memory>

#include "function.h"
#include "init.h"
#include "interfaces/uncopyable.h"

namespace jayl {

  extern const std::vector<std::string> VALID_BACKENDS;
  extern std::string kBackend;

  class Diagnostics;

/// A Jayl program. You can load Jayl source code using the \ref loadString
/// and \ref loadFile and compile the program using the \ref compile method.
  class Program : private interfaces::Uncopyable {
    public:
    /// Create a new Jayl program.
    Program();

    ~Program();

    /// Clear program of data (makes it undefined).
    void clear();

    /// Add the Jayl code in the given string to the program.
    /// \return 0 on success, and 1 if the Jayl code has errors. If the code
    ///         has errors these can be retrieved through the \ref getErrors
    ///         and \ref getErrorString methods.
    int loadString(const std::string &programString);

    /// Add the Jayl code in the given file to the program.
    /// \return 0 on success, 1 if the Jayl code has errors, and 2 if the file
    ///         could not be read. If the code has errors these can be retrieved
    ///         through the \ref getErrors and \ref getErrorString methods.
    int loadFile(const std::string &filename);

    /// Returns the names of all the functions in the program.
    std::vector<std::string> getFunctionNames() const;

    /// Compile and return a runnable function, or an undefined function if an
    /// error occured.
    Function compile(const std::string &function);

    Function compileWithTimers(const std::string &function);

    /// Verify the program by executing in-code comment tests.
    int verify();

    bool hasErrors() const;

    const Diagnostics &getDiagnostics() const;

    /// Writes a human-readable string represeting the program to the stream.
    friend std::ostream &operator<<(std::ostream &, const Program &);

    private:
    struct ProgramContent;
    ProgramContent *content;
  };

}
#endif
