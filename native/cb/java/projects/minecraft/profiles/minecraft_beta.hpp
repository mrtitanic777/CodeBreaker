// cb/java/projects/minecraft/profiles/minecraft_beta.hpp -- the Beta edition target profile.
//
// The canonical minecraft_xbox360 profile is predominantly a Beta port (World/Chunk/Region-NBT, biomes, the full
// block/entity/GUI taxonomy), so this profile reuses that vocabulary as it is. It is the reference edition.
#pragma once

#include "cb/java/profiles/package.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_beta {

// The profile named "minecraft_beta".
java::profiles::Profile profile();

}  // namespace cb::java::projects::minecraft::profiles::minecraft_beta
