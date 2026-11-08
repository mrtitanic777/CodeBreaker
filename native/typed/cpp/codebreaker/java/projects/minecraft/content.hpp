// codebreaker/java/projects/minecraft/content.hpp -- C++ port of codebreaker.java.projects.minecraft.content:
// scan the Java corpus for the ASSETS the game loads at runtime and emit `Content.md`.
#pragma once

#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::content {

// (The module's private constants _RES_PATH, _SOUND_CALL, _STEP_SOUND and _CATEGORY are in content.cpp:
// the patterns are built on first use, not at program start.)

// What scan_assets returns: the dict {'resources': {path: sorted[classes]}, 'sounds': {event: sorted[classes]},
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

py::str _category_of(const py::str& path);
Assets scan_assets(const py::str& java_dir);
py::str render_content_md(const py::str& java_dir, const py::str& version_label = py::str("this build"));
py::str write_content_md(const py::str& java_dir, const py::str& out_dir, const py::str& version_label = py::str("this build"));

}  // namespace codebreaker::java::projects::minecraft::content
