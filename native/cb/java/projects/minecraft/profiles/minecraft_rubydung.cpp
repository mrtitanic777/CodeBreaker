#include "cb/java/projects/minecraft/profiles/minecraft_rubydung.hpp"

#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_rubydung {

const OrderedMap<std::string, std::string>& package_roles() {
    static const OrderedMap<std::string, std::string> roles = [] {
        OrderedMap<std::string, std::string> m = minecraft_base::shared_package_roles();
        m.insert_or_assign("com.mojang.rubydung", "Top-level pre-classic game shell, main loop and input.");
        m.insert_or_assign("com.mojang.rubydung.level", "The flat Level model, tiles, level renderer and frustum.");
        m.insert_or_assign("com.mojang.rubydung.character", "Player and mob (Zombie) entities.");
        m.insert_or_assign("com.mojang.rubydung.phys", "Collision primitives (AABB).");
        return m;
    }();
    return roles;
}

const std::vector<std::string>& static_init_order() {
    static const std::vector<std::string> v = {"Tile"};
    return v;
}

const typemap::NameSet& nullable() {
    static const typemap::NameSet s = {"tiles"};
    return s;
}

const typemap::NameSet& nullable_fields() {
    static const typemap::NameSet s = {"level", "player", "levelRenderer", "particleEngine"};
    return s;
}

java::profiles::Profile profile() {
    java::profiles::Profile p;
    p.name = "minecraft_rubydung";
    p.static_init_order = static_init_order();
    p.package_roles = package_roles();
    p.heavy_types = minecraft_base::shared_heavy_types();
    p.vcxproj_filters = minecraft_base::build_filters();
    p.dead_types = std::string(minecraft_base::shared_dead_types());
    p.type.nullable = nullable();
    p.type.nullable_fields = nullable_fields();
    return p;
}

}  // namespace cb::java::projects::minecraft::profiles::minecraft_rubydung
