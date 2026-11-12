// cb/java/projects/minecraft/profiles/minecraft_rubydung.hpp -- the RubyDung (pre-classic) edition target profile.
//
// Package com.mojang.rubydung (+ .level / .character / .phys). The world is a single flat Level (no chunks, no
// Block/Item registries yet); blocks are the pre-classic Tile class whose static instances (Tile.rock/grass/...)
// populate Tile.tiles[]. Rendering is the one-'l' Tesselator plus immediate-mode GL.
//
// A scaffold: the nullable vocabulary is deliberately minimal and should be verified and expanded against a real
// RubyDung decompile. The crash-class rules (uninitialised arrays, Rule-of-Three, ctor delegation, static-init) are
// engine-level and apply regardless of this vocabulary.
#pragma once

#include <string>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/java/profiles/package.hpp"
#include "cb/java/typemap.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_rubydung {

const OrderedMap<std::string, std::string>& package_roles();

// Tile holds the static tile instances and the Tile.tiles[] registry, so its statics initialise first.
const std::vector<std::string>& static_init_order();

// Tile.tiles[] has NULL slots for unregistered tile ids (a deref is a virtual call on NULL).
const typemap::NameSet& nullable();

// Fields legitimately NULL at points in the lifecycle (set after ctor / cleared on shutdown).
const typemap::NameSet& nullable_fields();

// The profile named "minecraft_rubydung" (it has no nullable getters).
java::profiles::Profile profile();

}  // namespace cb::java::projects::minecraft::profiles::minecraft_rubydung
