// codebreaker/java/projects/minecraft/profiles/minecraft_alpha.hpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_alpha.
//
// Alpha edition profile.
//
// Alpha is the era the canonical :mod:`minecraft_xbox360` profile was hand-tuned against (its nullable
// getters are documented as "Verified against the Alpha source"). This profile reuses that proven
// vocabulary verbatim; refine only if an Alpha-specific decompile shows a divergence from the Beta set.
//
// codebreaker.java.profiles.Profile has no typed port yet; the Profile record of the minecraft_xbox360 port
// stands for it. `register(Profile(...))` -- adding the profile to the Python registry -- is not ported:
// the registry is Python state of the profiles package. The Profile the Python registers is PROFILE() here.
// Values built from other profile modules are read when first asked for (function-local statics /
// accessors), so no start-up order between files matters.
#pragma once

#include "py.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_alpha {

using minecraft_xbox360::Profile;

// the Profile the module registers
const Profile& PROFILE();

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_alpha
