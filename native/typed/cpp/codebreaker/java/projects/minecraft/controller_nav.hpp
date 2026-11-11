// codebreaker/java/projects/minecraft/controller_nav.hpp -- SHIM over cb/java/projects/minecraft/controller_nav.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer. (The port's compiled patterns and templates
// _NAV_METHOD, _GRAB_SAFE ... had no consumer and are not carried.)
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/projects/minecraft/controller_nav.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::controller_nav {

// Every patch_* returns (new text, ok).
using Patched = std::tuple<py::str, bool>;

namespace shim_ {
namespace cbn = cb::java::projects::minecraft::controller_nav;
inline Patched to_old(const cbn::Patched& p) { return Patched{cb::compat::to_py(p.text), p.ok}; }
}  // namespace shim_

// Add `int navSelected;` + the method decl after the controlList field.
inline Patched patch_guiscreen_h(const py::str& text) {
    return shim_::to_old(shim_::cbn::patch_guiscreen_h(cb::compat::from_py(text)));
}

// Add the ctor init, the two includes, and the adapter body.
inline Patched patch_guiscreen_cpp(const py::str& text) {
    return shim_::to_old(shim_::cbn::patch_guiscreen_cpp(cb::compat::from_py(text)));
}

// Inject `<recv>->handleControllerMenuNav();` before the per-frame `<recv>->updateScreen();`.
inline Patched patch_minecraft_callsite(const py::str& text) {
    return shim_::to_old(shim_::cbn::patch_minecraft_callsite(cb::compat::from_py(text)));
}

// In displayGuiScreen (the ONE screen-transition choke point), make the controller's input mode a pure function of
// game state.
inline Patched patch_minecraft_displayguiscreen(const py::str& text, bool has_container) {
    return shim_::to_old(shim_::cbn::patch_minecraft_displayguiscreen(cb::compat::from_py(text), has_container));
}

// Draw the Pointer.png crosshair cursor right after the current screen renders, ONLY in container mode.
inline Patched patch_entityrenderer_pointer(const py::str& text) {
    return shim_::to_old(shim_::cbn::patch_entityrenderer_pointer(cb::compat::from_py(text)));
}

// Replace MouseHelper::grab/ungrabMouseCursor with console-safe bodies.
inline Patched patch_mousehelper_grab(const py::str& text) {
    return shim_::to_old(shim_::cbn::patch_mousehelper_grab(cb::compat::from_py(text)));
}

// The dict run() returns: which edits applied, and `missing` (fail-loud reasons).
struct Result {
    bool guiscreen_h = false;
    bool guiscreen_cpp = false;
    bool callsite = false;
    bool focusmode = false;
    bool mousehelper = false;
    bool pointer = false;
    py::list<py::str> missing;
};

// Inject the controller menu-nav glue across the converted tree. `missing` is fail-loud: a required anchor was not
// found (structural drift) so the UI would ship dead.
inline Result run(const py::str& out_dir) {
    return cb::compat::guard([&] {
        const auto r = shim_::cbn::run(cb::fs::from_text(cb::compat::from_py(out_dir)));
        return Result{r.guiscreen_h, r.guiscreen_cpp, r.callsite, r.focusmode, r.mousehelper, r.pointer, cb::compat::to_py(r.missing)};
    });
}

}  // namespace codebreaker::java::projects::minecraft::controller_nav
