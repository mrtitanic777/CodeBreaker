// cb/java/projects/minecraft/raycast.hpp -- Minecraft's GL-selection picking as a screen-centre raycast on the 360.
//
// GL selection-mode picking (glRenderMode(GL_SELECT) + gluPickMatrix + a glPushName name stack) has NO Direct3D
// equivalent. Routing the *calls* to the d3d9 layer left them software-emulated -- fragile, viewport/cursor
// dependent, and effectively non-functional on the console. The correct port replaces the whole idiom with a
// raycast (an ALGORITHM, not a call mapping), leaving the game with ZERO GL selection semantics. This finalizer
// detects the idiom in emitted game code and rewrites it:
//   * the DRIVER   -- a `Class::pick(` body containing GL_SELECT   -> screen-centre voxel raycast
//   * the RENDERER -- a body containing glPushName (the name stack) -> emptied (dead after the raycast)
//   * the PICK CAM -- a body containing gluPickMatrix               -> emptied (dead after the raycast)
//
// It operates on emitted C++ text (moved here from the transpiler with the project layer): the template names the
// game's own members -- this->player{x,y,z,xo,yo,zo,xRot,yRot}, this->level->isSolidTile, this->hitResult,
// HitResult(x,y,z,o,face) -- and is installed only when those members are present in the file, so a
// non-Minecraft port is left untouched.
#pragma once

#include <string>
#include <string_view>

namespace cb::java::projects::minecraft::raycast {

// The body that replaces the GL-selection driver (between its braces, starting with a newline).
std::string_view raycast_pick_body();

// The body that replaces a dead GL-selection helper (name stack / pick matrix).
std::string_view empty_pick_helper();

// Rewrite the GL-selection picking idiom in emitted C++ to the raycast. Only real method DEFINITIONS
// (`Class::method(...) {`) are considered, never control-flow blocks; each body is brace-matched while string and
// character literals are skipped. Text without the idiom is returned unchanged.
std::string convert_gl_pick_to_raycast(std::string_view text);

}  // namespace cb::java::projects::minecraft::raycast
