// codebreaker/java/projects/minecraft/profiles/minecraft_release.hpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_release.
//
// Release edition profile (1.x).
//
// Release builds on the Beta vocabulary and add registries/subsystems (achievements/stats, more world
// providers, the client/server package split). This profile starts from the proven Beta set and layers
// release-era package roles on top.
//
// SCAFFOLD NOTE: the release line spans many versions with real structural change (block states, the
// flattening, data-driven registries). Verify and expand the static-init seed and nullable vocabulary
// against a decompile of the specific release version being targeted.
//
// The Profile is codebreaker.java.profiles.Profile (aliased by the minecraft_xbox360 port). `register(Profile(...))` is
// done by the project package (load_profiles). The Profile the Python registers is PROFILE() here.
// Values built from other profile modules are read when first asked for (function-local statics /
// accessors), so no start-up order between files matters.
#pragma once

#include "py.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_release {

using minecraft_xbox360::Profile;

const py::dict<py::str, py::str>& PACKAGE_ROLES();

// the Profile the module registers
const Profile& PROFILE();

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_release
