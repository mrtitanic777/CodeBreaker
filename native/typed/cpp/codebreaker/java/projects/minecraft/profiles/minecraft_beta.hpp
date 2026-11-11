// codebreaker/java/projects/minecraft/profiles/minecraft_beta.hpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_beta.
//
// Beta edition profile.
//
// The canonical :mod:`minecraft_xbox360` profile is predominantly a Beta port (World/Chunk/Region-NBT,
// biomes, the full block/entity/GUI taxonomy). This profile reuses that proven vocabulary verbatim; it
// is the reference edition and the natural target of the (optional, later) `minecraft_xbox360` alias.
//
// The Profile is codebreaker.java.profiles.Profile (aliased by the minecraft_xbox360 port). `register(Profile(...))` is
// done by the project package (load_profiles). The Profile the Python registers is PROFILE() here.
// Values built from other profile modules are read when first asked for (function-local statics /
// accessors), so no start-up order between files matters.
#pragma once

#include "py.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_beta {

using minecraft_xbox360::Profile;

// the Profile the module registers
const Profile& PROFILE();

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_beta
