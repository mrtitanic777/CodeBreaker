// codebreaker/emulate/package.cpp -- typed C++ port of codebreaker/emulate/__init__.py (see package.hpp).
#include "codebreaker/emulate/package.hpp"

#include "codebreaker/platforms/package.hpp"

namespace codebreaker::emulate {

py::dict<py::str, std::tuple<py::str, py::str>> _runners() { return platforms::runners(); }

const py::dict<py::str, std::tuple<py::str, py::str>>& RUNNERS() {
    static const py::dict<py::str, std::tuple<py::str, py::str>> r = _runners();   // each platform's runner (codebreaker/platforms)
    return r;
}

std::tuple<py::str, py::str> runner(const py::str& kind) {
    const auto& r = RUNNERS();
    if (!r.contains(kind)) throw py::KeyError(py::repr(kind).utf8());   // RUNNERS[kind]
    return r.at(kind);
}

}  // namespace codebreaker::emulate
