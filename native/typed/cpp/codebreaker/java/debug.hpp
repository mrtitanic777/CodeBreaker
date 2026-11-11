// codebreaker/java/debug.hpp -- SHIM over cb/java/debug.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer. (DEBUG_H/DEBUG_CPP/DEBUG_README and _DEBUG_MARKER had no consumer outside the module; the text is
// reached through the new debug_header()/debug_cpp()/debug_readme().)
#pragma once

#include <cstdint>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/debug.hpp"

namespace codebreaker::java::debug {

inline const py::str DEBUG_H = cb::compat::to_py(cb::java::debug::debug_header(true));
inline const py::str DEBUG_CPP = cb::compat::to_py(cb::java::debug::debug_cpp());
inline const py::str DEBUG_README = cb::compat::to_py(cb::java::debug::debug_readme());

// Emit Debug.h + Debug.cpp + DEBUG.md into out_dir. Returns the paths.
inline py::list<py::str> write_debug(const py::str& out_dir, bool enabled_default = true) {
    return cb::compat::guard([&] {
        std::vector<std::string> paths;
        for (const auto& p : cb::java::debug::write_debug(cb::fs::from_text(cb::compat::from_py(out_dir)), enabled_default)) {
            paths.push_back(cb::fs::to_text(p));
        }
        return cb::compat::to_py(paths);
    });
}

// {tag: [(file, line)]} for every J2X-DEBUG marker across the tree.
inline py::dict<py::str, py::list<std::tuple<py::str, int64_t>>> scan_debug_markers(const py::str& out_dir) {
    return cb::compat::guard([&] {
        py::dict<py::str, py::list<std::tuple<py::str, int64_t>>> out;
        for (const auto& [tag, sites] : cb::java::debug::scan_debug_markers(cb::fs::from_text(cb::compat::from_py(out_dir)))) {
            py::list<std::tuple<py::str, int64_t>> list;
            for (const auto& s : sites) list.append(std::make_tuple(cb::compat::to_py(s.file), static_cast<int64_t>(s.line)));
            out.set(cb::compat::to_py(tag), list);
        }
        return out;
    });
}

}  // namespace codebreaker::java::debug

