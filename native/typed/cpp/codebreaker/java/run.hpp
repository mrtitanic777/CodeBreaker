// codebreaker/java/run.hpp -- SHIM over cb/java/run.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
//
// The tool table, the parse and the tools whose modules are in native/cb are cb::java::run's; the tools whose modules are not
// refactored yet are the old-layer bodies of run_tools.cpp, handed over as cb::java::run::Tools (legacy_tools()). A tool is
// provided from native/cb and its body leaves run_tools.cpp when its module is refactored.
//
// A usage error or -h is cb::cli's: the usage and help text are the new tool's (the exit codes are as before; main returns 2
// for a command-line mistake). The old argparse emulation, its texts and its SystemExit are gone.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/run.hpp"

namespace codebreaker::java::run {

// Defined in run_tools.cpp (a translation unit of the shim: it calls old ports).
cb::java::run::Tools legacy_tools();

// The command line without the program name (sys.argv[1:]). -> the exit code
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] { return static_cast<int64_t>(cb::java::run::main(cb::compat::from_py(argv), legacy_tools())); });
}

}  // namespace codebreaker::java::run
