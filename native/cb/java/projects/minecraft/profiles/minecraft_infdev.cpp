#include "cb/java/projects/minecraft/profiles/minecraft_infdev.hpp"

#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_infdev {

const OrderedMap<std::string, std::string>& package_roles() {
    static const OrderedMap<std::string, std::string> roles = [] {
        OrderedMap<std::string, std::string> m = minecraft_base::shared_package_roles();
        m.insert_or_assign("com.mojang.minecraft", "Top-level game shell, main loop and input.");
        m.insert_or_assign("com.mojang.minecraft.level",
                           "The infinite world model, chunk streaming, generation, save/load.");
        m.insert_or_assign("com.mojang.minecraft.level.tile", "Block/Tile definitions and per-block behaviour.");
        m.insert_or_assign("com.mojang.minecraft.character", "Player and mob entities.");
        m.insert_or_assign("com.mojang.minecraft.phys", "Collision primitives (AABB).");
        return m;
    }();
    return roles;
}

const std::vector<std::string>& static_init_order() {
    static const std::vector<std::string> v = {"Material", "Block", "Item"};
    return v;
}

const typemap::NameSet& nullable() {
    static const typemap::NameSet s = {"blocksList", "itemsList"};
    return s;
}

const typemap::NameSet& nullable_fields() {
    static const typemap::NameSet s = {"level", "player", "levelRenderer", "particleEngine", "textures"};
    return s;
}

const typemap::NameSet& nullable_getters() {
    static const typemap::NameSet s = {"getChunk", "getChunkFromChunkCoords", "getChunkFromBlockCoords"};
    return s;
}

const typemap::NameMap& nullable_getter_return_type() {
    static const typemap::NameMap m = {
        {"getChunk", "Chunk"},
        {"getChunkFromChunkCoords", "Chunk"},
        {"getChunkFromBlockCoords", "Chunk"},
    };
    return m;
}

java::profiles::Profile profile() {
    java::profiles::Profile p;
    p.name = "minecraft_infdev";
    p.static_init_order = static_init_order();
    p.package_roles = package_roles();
    p.heavy_types = minecraft_base::shared_heavy_types();
    p.vcxproj_filters = minecraft_base::build_filters();
    p.dead_types = std::string(minecraft_base::shared_dead_types());
    p.type.nullable = nullable();
    p.type.nullable_fields = nullable_fields();
    p.type.nullable_getters = nullable_getters();
    p.type.nullable_getter_return_type = nullable_getter_return_type();
    return p;
}

}  // namespace cb::java::projects::minecraft::profiles::minecraft_infdev
