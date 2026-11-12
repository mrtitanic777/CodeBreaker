// cb/java/projects/minecraft/profiles/minecraft_infdev.hpp -- the Infdev edition target profile.
//
// Infdev introduces INFINITE worlds: the flat finite Level is replaced by chunk streaming (Chunk plus a chunk
// provider) and world save/load moves to per-chunk storage. The Block/Item/Material registries carry over from Indev.
//
// A scaffold: verify the nullable vocabulary (especially the chunk getters) and the static-init seed against a real
// Infdev decompile.
#pragma once

#include <string>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/java/profiles/package.hpp"
#include "cb/java/typemap.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_infdev {

const OrderedMap<std::string, std::string>& package_roles();
const std::vector<std::string>& static_init_order();

// Registry arrays with NULL slots.
const typemap::NameSet& nullable();
const typemap::NameSet& nullable_fields();

// Chunk lookups can miss (an unloaded or ungenerated chunk) and give NULL.
const typemap::NameSet& nullable_getters();
const typemap::NameMap& nullable_getter_return_type();

// The profile named "minecraft_infdev".
java::profiles::Profile profile();

}  // namespace cb::java::projects::minecraft::profiles::minecraft_infdev
