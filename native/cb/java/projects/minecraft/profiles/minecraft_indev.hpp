// cb/java/projects/minecraft/profiles/minecraft_indev.hpp -- the Indev edition target profile, derived from a
// RetroMCP Indev decompile (net.minecraft.game.*, com.mojang.nbt; 230 classes).
//
// Indev has the Block/Item/Material registries and mobs/items/inventory/crafting, but the world is a single FINITE
// World (a flat byte[] block array indexed (y*length + z)*width + x -- fields width/length/height) with NO chunk
// streaming. Saves are one GZIP+NBT stream via LevelLoader (com.mojang.nbt). There is no Chunk/ChunkProvider and no
// BiomeGenBase/RegionFile -- those distinguish it from Alpha/Beta.
//
// The flat World.d[] walks into the (engine-level) out-of-bounds fixed-array crash class; bounds-guarding that
// access is a rules concern, not a profile field.
#pragma once

#include <string>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/java/profiles/package.hpp"
#include "cb/java/typemap.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_indev {

// net.minecraft.game.* / net.minecraft.client.* / com.mojang.nbt / util -- from the real package tree.
const OrderedMap<std::string, std::string>& package_roles();

// Material statics feed Block ctors; Block statics feed Item; CraftingManager reads both. Material's subclasses
// (MaterialTransparent/Liquid/Logic) load with Material. (Derived from static-init reads.)
const std::vector<std::string>& static_init_order();

// Reference registries with NULL slots for unregistered ids: Block.blocksList[256] (ids 0, 61-255 NULL) and
// Item.itemsList[1024]. A deref of an empty slot is a virtual call on NULL.
const typemap::NameSet& nullable();

// Fields legitimately NULL at points in the lifecycle (set after ctor / cleared on world unload), with unguarded
// derefs on the console. Names verified against Minecraft.java / World.java / Entity.java.
const typemap::NameSet& nullable_fields();

// Getters that can return NULL (sparse lookups / optional references) whose pointer result is dereferenced.
const typemap::NameSet& nullable_getters();

// Pointer return type of each nullable getter, so the null-guard hoist can declare a typed temporary.
const typemap::NameMap& nullable_getter_return_type();

// The profile named "minecraft_indev".
java::profiles::Profile profile();

}  // namespace cb::java::projects::minecraft::profiles::minecraft_indev
