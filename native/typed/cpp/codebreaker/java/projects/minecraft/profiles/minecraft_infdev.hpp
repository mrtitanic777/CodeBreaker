// codebreaker/java/projects/minecraft/profiles/minecraft_infdev.hpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_infdev.
//
// Infdev edition profile.
//
// Infdev introduces INFINITE worlds: the flat finite Level is replaced by chunk streaming
// (``Chunk`` + a chunk provider), and world save/load moves to per-chunk storage. Block/Item/Material
// registries carry over from Indev.
//
// SCAFFOLD NOTE: verify the nullable vocabulary (esp. the chunk getters) and static-init seed against a
// real Infdev decompile.
//
// codebreaker.java.profiles.Profile has no typed port yet; the Profile record of the minecraft_xbox360 port
// stands for it. `register(Profile(...))` -- adding the profile to the Python registry -- is not ported:
// the registry is Python state of the profiles package. The Profile the Python registers is PROFILE() here.
// Values built from other profile modules are read when first asked for (function-local statics /
// accessors), so no start-up order between files matters.
#pragma once

#include "py.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_infdev {

using minecraft_xbox360::Profile;

const py::dict<py::str, py::str>& PACKAGE_ROLES();

extern const py::list<py::str> STATIC_INIT_ORDER;

// Registry arrays with NULL slots.
extern const py::set<py::str> NULLABLE;

extern const py::set<py::str> NULLABLE_FIELDS;

// Chunk lookups can miss (unloaded/ungenerated chunk) -> NULL.
extern const py::set<py::str> NULLABLE_GETTERS;

extern const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE;

// the Profile the module registers
const Profile& PROFILE();

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_infdev
