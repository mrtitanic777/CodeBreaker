// codebreaker/java/console/_shim.hpp -- what the shims of this folder (xdk_scan, sdk, project_scan, worklist,
// project_layout, vcxproj, assets, xact) share: the old layer's text for the new tree's paths, and back.
// Deleted with the last of those shims (docs/CPP_DESIGN.md 6.3).
#pragma once

#include <filesystem>
#include <optional>
#include <string>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::shim {

inline std::filesystem::path path_of(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }

// A path in the text the old layer wrote them in: native (backslash) separators.
inline py::str native(const std::filesystem::path& p) {
    std::string t = cb::fs::to_text(p);
    for (char& c : t)
        if (c == '/') c = '\\';
    return cb::compat::to_py(t);
}

inline std::optional<py::str> native(const std::optional<std::filesystem::path>& p) {
    if (!p) return std::nullopt;
    return native(*p);
}

// An optional old-layer path that is None or empty means "not given".
inline std::optional<std::filesystem::path> given(const std::optional<py::str>& s) {
    if (!s || s->size() == 0) return std::nullopt;
    return path_of(*s);
}

}  // namespace codebreaker::java::console::shim
