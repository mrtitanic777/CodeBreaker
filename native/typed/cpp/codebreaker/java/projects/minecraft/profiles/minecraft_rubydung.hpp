// codebreaker/java/projects/minecraft/profiles/minecraft_rubydung.hpp -- SHIM over
// cb/java/projects/minecraft/profiles/minecraft_rubydung.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer. The old record of the profile the module registers is PROFILE().
#pragma once

#include "cb/java/projects/minecraft/profiles/minecraft_rubydung.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
#include "py.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_rubydung {

using minecraft_xbox360::Profile;

// the Profile the module registers
inline const Profile& PROFILE() {
    static const Profile p = minecraft_xbox360::from_cb(cb::java::projects::minecraft::profiles::minecraft_rubydung::profile());
    return p;
}

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_rubydung