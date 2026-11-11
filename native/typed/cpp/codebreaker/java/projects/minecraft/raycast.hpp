// codebreaker/java/projects/minecraft/raycast.hpp -- SHIM over cb/java/projects/minecraft/raycast.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/java/projects/minecraft/raycast.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::raycast {

// _RAYCAST_PICK_BODY and _EMPTY_PICK_HELPER in the Python (spelled with a trailing underscore: an underscore followed
// by a capital is reserved in C++)
inline const py::str RAYCAST_PICK_BODY_ = cb::compat::to_py(cb::java::projects::minecraft::raycast::raycast_pick_body());
inline const py::str EMPTY_PICK_HELPER_ = cb::compat::to_py(cb::java::projects::minecraft::raycast::empty_pick_helper());

inline py::str convert_gl_pick_to_raycast(const py::str& text) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::java::projects::minecraft::raycast::convert_gl_pick_to_raycast(cb::compat::from_py(text)));
    });
}

}  // namespace codebreaker::java::projects::minecraft::raycast
