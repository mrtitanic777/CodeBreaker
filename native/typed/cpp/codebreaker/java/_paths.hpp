// codebreaker/java/_paths.hpp -- SHIM over cb/java/paths.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/java/paths.hpp"
#include "py_os.hpp"

namespace codebreaker::java::_paths {

// The old code held the java folder as a module variable; it starts as the folder of this header (as the port
// did) and the host sets it with set_here().
inline py::str& here_() {
    static py::str here = py::os::path::dirname(py::os::path::abspath(py::str(__FILE__)));
    return here;
}
inline py::str _HERE() { return here_(); }
inline void set_here(const py::str& here) { here_() = here; }

// Absolute path to a data file or folder shipped with the package.
inline py::str data_path(const py::list<py::str>& parts) {
    py::str out = here_();
    for (int64_t i = 0; i < parts.size(); i++) out = py::os::path::join(out, parts[i]);
    return out;
}

}  // namespace codebreaker::java::_paths
