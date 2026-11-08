// codebreaker/java/projects/minecraft/profiles/minecraft_indev.hpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_indev.
//
// Indev edition profile -- SOURCE-DERIVED from a RetroMCP Indev decompile (net.minecraft.game.*,
// com.mojang.nbt; 230 classes).
//
// Indev has the Block/Item/Material registries and mobs/items/inventory/crafting, but the world is a
// single FINITE ``World`` (flat ``byte[]`` block array indexed ``(y*length + z)*width + x`` -- fields
// width/length/height) with NO chunk streaming. Saves are one GZIP+NBT stream via ``LevelLoader``
// (``com.mojang.nbt``). No ``Chunk``/``ChunkProvider``, no ``BiomeGenBase``/``RegionFile`` -- those
// distinguish it from Alpha/Beta.
//
// The flat ``World.d[]`` walks into the (engine-level) OOB fixed-array crash class; bounds-guarding that
// access is a rules concern, not a profile field.
//
// codebreaker.java.profiles.Profile has no typed port yet; the Profile record of the minecraft_xbox360 port
// stands for it. `register(Profile(...))` -- adding the profile to the Python registry -- is not ported:
// the registry is Python state of the profiles package. The Profile the Python registers is PROFILE() here.
// Values built from other profile modules are read when first asked for (function-local statics /
// accessors), so no start-up order between files matters.
#pragma once

#include "py.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_indev {

using minecraft_xbox360::Profile;

// net.minecraft.game.* / net.minecraft.client.* / com.mojang.nbt / util -- from the real package tree.
const py::dict<py::str, py::str>& PACKAGE_ROLES();

// Material statics feed Block ctors; Block statics feed Item; CraftingManager reads both. Material's
// subclasses (MaterialTransparent/Liquid/Logic) load with Material. (Derived from static-init reads.)
extern const py::list<py::str> STATIC_INIT_ORDER;

// Reference registries with NULL slots for unregistered ids -> deref of an empty slot is a virtual call
// on NULL. Block.blocksList[256] (ids 0,61-255 NULL) and Item.itemsList[1024] (unregistered NULL).
extern const py::set<py::str> NULLABLE;

// Fields legitimately NULL at points in the lifecycle (set after ctor / cleared on world unload), with
// unguarded derefs on the console. Names verified against Minecraft.java / World.java / Entity.java.
extern const py::set<py::str> NULLABLE_FIELDS;

// Getters that can return NULL (sparse lookups / optional refs) whose pointer result is dereferenced.
extern const py::set<py::str> NULLABLE_GETTERS;

// Pointer return type of each nullable getter, so the C++03 null-guard hoist can declare a typed temp.
extern const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE;

// the Profile the module registers
const Profile& PROFILE();

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_indev
