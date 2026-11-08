// codebreaker/java/_paths.cpp -- C++ port of codebreaker.java._paths.
//
// _paths.py -- where the data the Java transpiler ships with is found: the platform layer
// (platform/canonical, reference, companions, assets) and the API maps (api_maps/), all inside this
// package. Machine locations (the XDK, Java, the decompiler jar, overlays) are settings, not here.
//
//     from codebreaker.java._paths import data_path
//     data_path('platform', 'canonical')

#include "codebreaker/java/_paths.hpp"

namespace codebreaker::java::_paths {

namespace {
py::str& here_() {
    static py::str here = py::os::path::dirname(py::os::path::abspath(py::str(__FILE__)));
    return here;
}
}  // namespace

py::str _HERE() { return here_(); }

void set_here(const py::str& here) { here_() = here; }

py::str data_path(const py::list<py::str>& parts) {
    py::str out = py::os::path::join(here_());
    for (int64_t i = 0; i < parts.size(); i++) out = py::os::path::join(out, parts[i]);
    return out;
}

}  // namespace codebreaker::java::_paths
