// codebreaker/java/projects/minecraft/keybind_mark.hpp -- SHIM over cb/java/projects/minecraft/keybind_mark.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/projects/minecraft/keybind_mark.hpp"
#include "py.hpp"

namespace codebreaker::java::projects::minecraft::keybind_mark {

// One key-press site: (file name, line number, the keys on the line, the label written into the marker)
using Site = std::tuple<py::str, int64_t, py::list<py::str>, py::str>;

// The dict mark() returns ({'sites': [...], 'skipped': int}); run() adds 'index' (the path written, or None).
struct Result {
    py::list<Site> sites;
    int64_t skipped = 0;
    std::optional<py::str> index;
};

namespace shim_ {
namespace cbk = cb::java::projects::minecraft::keybind_mark;
inline Result to_old(const cbk::Result& r) {
    Result out;
    for (const cbk::Site& s : r.sites) {
        out.sites.append(Site{cb::compat::to_py(s.file), cb::compat::to_py(s.line), cb::compat::to_py(s.keys), cb::compat::to_py(s.label)});
    }
    out.skipped = cb::compat::to_py(r.skipped);
    if (r.index) out.index = cb::compat::to_py(cb::fs::to_text(*r.index));
    return out;
}
inline cbk::Result from_old(const Result& r) {
    cbk::Result out;
    for (const Site& s : r.sites) {
        out.sites.push_back({cb::compat::from_py(std::get<0>(s)), cb::compat::int_from_py<std::size_t>(std::get<1>(s)),
                             cb::compat::from_py(std::get<2>(s)), cb::compat::from_py(std::get<3>(s))});
    }
    out.skipped = cb::compat::int_from_py<std::size_t>(r.skipped);
    return out;
}
inline std::filesystem::path path(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }
}  // namespace shim_

// Every KEY_x referenced on a line (a line may test two, e.g. F3+S).
inline py::list<py::str> _keys_on_line(const py::str& line) {
    return cb::compat::to_py(cb::java::projects::minecraft::keybind_mark::keys_on_line(cb::compat::from_py(line)));
}

inline py::str _action(const py::str& key) {
    return cb::compat::to_py(cb::java::projects::minecraft::keybind_mark::action_of(cb::compat::from_py(key)));
}

// Append `// J2X-KEYBIND[KEY_x -> action | ROUTE: controller button OR menu toggle]` to each keyboard key-press line.
inline Result mark(const py::str& src_dir, bool dry_run = false) {
    return cb::compat::guard([&] { return shim_::to_old(shim_::cbk::mark(shim_::path(src_dir), dry_run)); });
}

inline py::str render_index(const Result& result) {
    return cb::compat::guard([&] { return cb::compat::to_py(shim_::cbk::render_index(shim_::from_old(result))); });
}

// Mark the tree (unless dry_run) and write KEYBIND_INDEX.md. Returns the result dict + index path.
inline Result run(const py::str& src_dir, bool dry_run = false, const std::optional<py::str>& index_dir = std::nullopt) {
    return cb::compat::guard([&] {
        std::optional<std::filesystem::path> dir;
        if (index_dir && !index_dir->empty()) dir = shim_::path(*index_dir);   // (an empty folder name meant "the source tree")
        return shim_::to_old(shim_::cbk::run(shim_::path(src_dir), dry_run, dir));
    });
}

}  // namespace codebreaker::java::projects::minecraft::keybind_mark
