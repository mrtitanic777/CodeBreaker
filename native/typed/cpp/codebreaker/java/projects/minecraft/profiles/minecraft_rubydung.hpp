// codebreaker/java/projects/minecraft/profiles/minecraft_rubydung.hpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_rubydung.
//
// RubyDung (pre-classic) edition profile.
//
// Package: ``com.mojang.rubydung`` (+ ``.level`` / ``.character`` / ``.phys``). The world is a single
// flat ``Level`` (no chunks, no Block/Item registries yet); blocks are the pre-classic ``Tile`` class
// whose static instances (``Tile.rock``/``grass``/...) populate ``Tile.tiles[]``. Rendering is the
// one-'l' ``Tesselator`` + immediate-mode GL.
//
// SCAFFOLD NOTE: the nullable vocabulary below is intentionally minimal and should be verified/expanded
// against a real RubyDung decompile (see the rubydung-reconvert-setup memory for the source path). The
// crash-class RULES (uninit arrays, Rule-of-Three, ctor delegation, static-init) are engine-level and
// apply regardless of this vocabulary.
//
// The Profile is codebreaker.java.profiles.Profile (aliased by the minecraft_xbox360 port). `register(Profile(...))` is
// done by the project package (load_profiles). The Profile the Python registers is PROFILE() here.
// Values built from other profile modules are read when first asked for (function-local statics /
// accessors), so no start-up order between files matters.
#pragma once

#include "py.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_rubydung {

using minecraft_xbox360::Profile;

const py::dict<py::str, py::str>& PACKAGE_ROLES();

// Tile holds the static tile instances (Tile.rock/grass/...) and the Tile.tiles[] registry, so its
// statics must init first (they are read when the Level/renderer builds).
extern const py::list<py::str> STATIC_INIT_ORDER;

// Tile.tiles[] has NULL slots for unregistered tile ids -- deref of an unregistered slot is a virtual
// call on NULL (IP 0x0 on the console).
extern const py::set<py::str> NULLABLE;

// Fields legitimately NULL at points in the lifecycle (set after ctor / cleared on shutdown).
extern const py::set<py::str> NULLABLE_FIELDS;

extern const py::set<py::str> NULLABLE_GETTERS;

extern const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE;

// the Profile the module registers
const Profile& PROFILE();

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_rubydung
