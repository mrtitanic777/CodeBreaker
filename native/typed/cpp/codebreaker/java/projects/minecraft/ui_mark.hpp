// codebreaker/java/projects/minecraft/ui_mark.hpp -- SHIM over cb/java/projects/minecraft/ui_mark.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/projects/minecraft/ui_mark.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::ui_mark {

// One marked definition: (file name, line number, method name)
using Mark = std::tuple<py::str, int64_t, py::str>;

// The dict mark() returns ({'classes': set, 'marked': {class: [(file,line,method)]}, 'skipped': int}); run() adds
// 'index' (the path written, or None).
struct Result {
    py::set<py::str> classes;
    py::dict<py::str, py::list<Mark>> marked;
    int64_t skipped = 0;
    std::optional<py::str> index;
};

namespace shim_ {
namespace cbu = cb::java::projects::minecraft::ui_mark;
inline std::filesystem::path path(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }
inline std::vector<std::string> extra(const std::optional<py::list<py::str>>& extra_classes) {
    return extra_classes ? cb::compat::from_py(*extra_classes) : std::vector<std::string>();
}
inline Result to_old(const cbu::Result& r) {
    Result out;
    out.classes = cb::compat::to_py(r.classes);
    for (const auto& [cls, marks] : r.marked) {
        py::list<Mark> list;
        for (const cbu::Mark& m : marks) list.append(Mark{cb::compat::to_py(m.file), cb::compat::to_py(m.line), cb::compat::to_py(m.method)});
        out.marked.set(cb::compat::to_py(cls), list);
    }
    out.skipped = cb::compat::to_py(r.skipped);
    if (r.index) out.index = cb::compat::to_py(cb::fs::to_text(*r.index));
    return out;
}
inline cbu::Result from_old(const Result& r) {
    cbu::Result out;
    out.classes = cb::compat::from_py(r.classes);
    for (const auto& kv : r.marked.items()) {
        std::vector<cbu::Mark> marks;
        for (const Mark& m : std::get<1>(kv)) {
            marks.push_back({cb::compat::from_py(std::get<0>(m)), cb::compat::int_from_py<std::size_t>(std::get<1>(m)),
                             cb::compat::from_py(std::get<2>(m))});
        }
        out.marked[cb::compat::from_py(std::get<0>(kv))] = std::move(marks);
    }
    out.skipped = cb::compat::int_from_py<std::size_t>(r.skipped);
    return out;
}
}  // namespace shim_

// Name-based heuristic: Minecraft UI classes are Gui* or end in Screen/Menu.
inline bool is_ui_class_name(const py::str& name) {
    return cb::java::projects::minecraft::ui_mark::is_ui_class_name(cb::compat::from_py(name));
}

// The set of UI class names: name-pattern matches PLUS classes that extend a UI base (transitively).
inline py::set<py::str> discover_ui_classes(const py::str& src_dir, const std::optional<py::list<py::str>>& extra = std::nullopt) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(shim_::cbu::discover_ui_classes(shim_::path(src_dir), shim_::extra(extra)));
    });
}

// Insert `// J2X-UI[Class::method]` at each UI method definition.
inline Result mark(const py::str& src_dir, bool dry_run = false, const std::optional<py::list<py::str>>& extra_classes = std::nullopt) {
    return cb::compat::guard([&] {
        return shim_::to_old(shim_::cbu::mark(shim_::path(src_dir), dry_run, shim_::extra(extra_classes)));
    });
}

inline py::str render_index(const Result& result) {
    return cb::compat::guard([&] { return cb::compat::to_py(shim_::cbu::render_index(shim_::from_old(result))); });
}

// Mark the tree (unless dry_run) and write UI_INDEX.md. Returns the result dict + index path.
inline Result run(const py::str& src_dir, bool dry_run = false, const std::optional<py::list<py::str>>& extra_classes = std::nullopt,
                  const std::optional<py::str>& index_dir = std::nullopt) {
    return cb::compat::guard([&] {
        std::optional<std::filesystem::path> dir;
        if (index_dir && !index_dir->empty()) dir = shim_::path(*index_dir);   // (an empty folder name meant "the source tree")
        return shim_::to_old(shim_::cbu::run(shim_::path(src_dir), dry_run, shim_::extra(extra_classes), dir));
    });
}

}  // namespace codebreaker::java::projects::minecraft::ui_mark
