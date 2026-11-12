#include "cb/java/projects/minecraft/profiles/minecraft_release.hpp"

#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_release {

const OrderedMap<std::string, std::string>& package_roles() {
    static const OrderedMap<std::string, std::string> roles = [] {
        OrderedMap<std::string, std::string> m = minecraft_xbox360::package_roles();
        m.insert_or_assign("net.minecraft.src", "Client-side game classes (release single-package layout).");
        m.insert_or_assign("net.minecraft.client", "Client game shell, input and main loop.");
        m.insert_or_assign("net.minecraft.server", "Integrated server: world ticking, save, and player management.");
        return m;
    }();
    return roles;
}

java::profiles::Profile profile() {
    java::profiles::Profile p = minecraft_xbox360::profile();
    p.name = "minecraft_release";
    p.package_roles = package_roles();
    p.vcxproj_filters = minecraft_base::build_filters();
    return p;
}

}  // namespace cb::java::projects::minecraft::profiles::minecraft_release
