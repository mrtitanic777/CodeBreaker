#include "cb/java/projects/minecraft/profiles/minecraft_beta.hpp"

#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_beta {

java::profiles::Profile profile() {
    java::profiles::Profile p = minecraft_xbox360::profile();
    p.name = "minecraft_beta";
    p.vcxproj_filters = minecraft_base::build_filters();
    return p;
}

}  // namespace cb::java::projects::minecraft::profiles::minecraft_beta
