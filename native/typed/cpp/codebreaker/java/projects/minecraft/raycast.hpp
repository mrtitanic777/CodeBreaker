// codebreaker/java/projects/minecraft/raycast.hpp -- C++ port of codebreaker.java.projects.minecraft.raycast.
// Minecraft's GL-selection picking as a screen-centre raycast on the 360 (moved from transpile.py with
// the project layer). Operates on emitted C++ text: the template names the game's own fields (player,
// level, hitResult).
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::raycast {

// _RAYCAST_PICK_BODY and _EMPTY_PICK_HELPER in the Python (spelled with a trailing underscore: an
// underscore followed by a capital is reserved in C++)
extern const py::str RAYCAST_PICK_BODY_;
extern const py::str EMPTY_PICK_HELPER_;

py::str convert_gl_pick_to_raycast(const py::str& text);

}  // namespace codebreaker::java::projects::minecraft::raycast
