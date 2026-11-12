// cb/java/projects/minecraft/profiles/minecraft_release.hpp -- the Release (1.x) edition target profile.
//
// Release builds on the Beta vocabulary and adds registries and subsystems (achievements/stats, more world
// providers, the client/server package split). This profile starts from the Beta set and layers the release-era
// package roles on top.
//
// A scaffold: the release line spans many versions with real structural change (block states, the flattening,
// data-driven registries). Verify and expand the static-init seed and nullable vocabulary against a decompile of the
// specific release version being targeted.
#pragma once

#include <string>

#include "cb/core/ordered_map.hpp"
#include "cb/java/profiles/package.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_release {

// The Beta package roles plus the three release-era packages (net.minecraft.src / client / server).
const OrderedMap<std::string, std::string>& package_roles();

// The profile named "minecraft_release".
java::profiles::Profile profile();

}  // namespace cb::java::projects::minecraft::profiles::minecraft_release
