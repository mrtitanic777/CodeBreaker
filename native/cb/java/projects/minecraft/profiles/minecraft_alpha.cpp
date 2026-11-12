#include "cb/java/projects/minecraft/profiles/minecraft_alpha.hpp"

#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_alpha {

java::profiles::Profile profile() {
    java::profiles::Profile p = minecraft_xbox360::profile();
    p.name = "minecraft_alpha";
    p.vcxproj_filters = minecraft_base::build_filters();
    return p;
}

}  // namespace cb::java::projects::minecraft::profiles::minecraft_alpha
