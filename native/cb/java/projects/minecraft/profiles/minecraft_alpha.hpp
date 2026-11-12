// cb/java/projects/minecraft/profiles/minecraft_alpha.hpp -- the Alpha edition target profile.
//
// Alpha is the era the canonical minecraft_xbox360 profile was hand-tuned against (its nullable getters are
// documented as "Verified against the Alpha source"), so this profile reuses that vocabulary as it is; refine it
// only if an Alpha-specific decompile shows a divergence from the Beta set.
#pragma once

#include "cb/java/profiles/package.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_alpha {

// The profile named "minecraft_alpha".
java::profiles::Profile profile();

}  // namespace cb::java::projects::minecraft::profiles::minecraft_alpha
