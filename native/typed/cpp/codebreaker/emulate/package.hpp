// codebreaker/emulate/package.hpp -- SHIM over cb/emulate/package.hpp (docs/CPP_DESIGN.md 6.3): codebreaker/emulate/
// __init__.py's registry, answered from a cb::emulate::RunnerTable filled from the old platform registry. Deleted
// with its last py:: consumer.
//
//     auto [module, function] = emulate::runner("pe");     // -> ("codebreaker.emulate.emulator", "recorded")
//
// runner(kind) of a kind that is not a platform's name raises KeyError, as RUNNERS[kind] does.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/package.hpp"
#include "codebreaker/platforms/package.hpp"

namespace codebreaker::emulate {

// {platform name: (module, function)} of every platform whose code CodeBreaker can run
inline py::dict<py::str, std::tuple<py::str, py::str>> _runners() { return platforms::runners(); }

// each platform's runner (codebreaker/platforms)
inline const py::dict<py::str, std::tuple<py::str, py::str>>& RUNNERS() {
    static const py::dict<py::str, std::tuple<py::str, py::str>> r = _runners();
    return r;
}

// The function that runs code of this kind (the kinds are verify/reconstruct.py's): its (module, name).
inline std::tuple<py::str, py::str> runner(const py::str& kind) {
    cb::emulate::RunnerTable table;
    for (const auto& kv : RUNNERS().items()) {
        table.add(std::get<0>(kv).utf8(), {std::get<0>(std::get<1>(kv)).utf8(), std::get<1>(std::get<1>(kv)).utf8()});
    }
    try {
        const cb::emulate::RunnerName& r = table.at(kind.utf8());
        return {py::str(r.module), py::str(r.function)};
    } catch (const cb::UsageError&) {
        throw py::KeyError(py::repr(kind).utf8());   // RUNNERS[kind]
    }
}

}  // namespace codebreaker::emulate
