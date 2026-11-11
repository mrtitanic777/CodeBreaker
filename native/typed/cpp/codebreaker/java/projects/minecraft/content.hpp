// codebreaker/java/projects/minecraft/content.hpp -- SHIM over cb/java/projects/minecraft/content.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/projects/minecraft/content.hpp"
#include "cb/core/fs.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::content {

// What scan_assets returns: {'resources': {path: sorted[classes]}, 'sounds': {event: sorted[classes]},
// 'step_sounds': {name: sorted[classes]}} across the corpus.
struct Assets {
    py::dict<py::str, py::list<py::str>> resources;
    py::dict<py::str, py::list<py::str>> sounds;
    py::dict<py::str, py::list<py::str>> step_sounds;
};
template <class F> void for_each_field(const Assets& r, F&& f) {
    f("resources", r.resources);
    f("sounds", r.sounds);
    f("step_sounds", r.step_sounds);
}

inline py::str _category_of(const py::str& path) {
    return cb::compat::to_py(cb::java::projects::minecraft::content::category_of(cb::compat::from_py(path)));
}

inline Assets scan_assets(const py::str& java_dir) {
    return cb::compat::guard([&] {
        const auto a = cb::java::projects::minecraft::content::scan_assets(cb::fs::from_text(cb::compat::from_py(java_dir)));
        return Assets{cb::compat::to_py(a.resources), cb::compat::to_py(a.sounds), cb::compat::to_py(a.step_sounds)};
    });
}

inline py::str render_content_md(const py::str& java_dir, const py::str& version_label = py::str("this build")) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::java::projects::minecraft::content::render_content_md(
            cb::fs::from_text(cb::compat::from_py(java_dir)), cb::compat::from_py(version_label)));
    });
}

inline py::str write_content_md(const py::str& java_dir, const py::str& out_dir, const py::str& version_label = py::str("this build")) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::fs::to_text(cb::java::projects::minecraft::content::write_content_md(
            cb::fs::from_text(cb::compat::from_py(java_dir)), cb::fs::from_text(cb::compat::from_py(out_dir)),
            cb::compat::from_py(version_label))));
    });
}

}  // namespace codebreaker::java::projects::minecraft::content
