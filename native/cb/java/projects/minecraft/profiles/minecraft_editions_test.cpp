// The six edition profiles (rubydung, indev, infdev, alpha, beta, release). The counts and first keys below were
// read off the Python registry (codebreaker.java.profiles.get) and checked by hand against the module sources.
#include <string>

#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_alpha.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_beta.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_indev.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_infdev.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_release.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_rubydung.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace {

using namespace cb;
namespace mcp = cb::java::projects::minecraft::profiles;
using cb::java::profiles::Profile;

struct Shape {
    std::size_t init, roles, filters, nullable, fields, getters, getter_types, heavy;
};

void check_shape(const Profile& p, const char* name, const Shape& s) {
    CB_CHECK_EQ(p.name, name);
    CB_CHECK_EQ(p.static_init_order.size(), s.init);
    CB_CHECK_EQ(p.package_roles.size(), s.roles);
    CB_CHECK_EQ(p.vcxproj_filters.size(), s.filters);
    CB_CHECK_EQ(p.type.nullable.size(), s.nullable);
    CB_CHECK_EQ(p.type.nullable_fields.size(), s.fields);
    CB_CHECK_EQ(p.type.nullable_getters.size(), s.getters);
    CB_CHECK_EQ(p.type.nullable_getter_return_type.size(), s.getter_types);
    CB_CHECK_EQ(p.heavy_types.size(), s.heavy);
    CB_CHECK(p.heavy_types_high.empty());
    CB_CHECK(p.dead_types && *p.dead_types == mcp::minecraft_base::shared_dead_types());
    CB_CHECK(!p.optional_types);
    CB_CHECK(p.type.unsigned_byte_scalar_fields.empty() && p.type.default_value_types.empty());
    // every getter that has a pointer return type is a nullable getter
    for (const auto& [getter, type] : p.type.nullable_getter_return_type) {
        CB_CHECK(p.type.nullable_getters.contains(getter));
        CB_CHECK(!type.empty());
    }
}

CB_TEST("java.minecraft.editions.shapes", [] {
    check_shape(mcp::minecraft_rubydung::profile(), "minecraft_rubydung", {1, 5, 55, 1, 4, 0, 0, 4});
    check_shape(mcp::minecraft_indev::profile(), "minecraft_indev", {4, 31, 55, 2, 13, 12, 12, 4});
    check_shape(mcp::minecraft_infdev::profile(), "minecraft_infdev", {3, 6, 55, 2, 5, 3, 3, 4});
    check_shape(mcp::minecraft_alpha::profile(), "minecraft_alpha", {10, 17, 55, 2, 18, 11, 10, 4});
    check_shape(mcp::minecraft_beta::profile(), "minecraft_beta", {10, 17, 55, 2, 18, 11, 10, 4});
    check_shape(mcp::minecraft_release::profile(), "minecraft_release", {10, 19, 55, 2, 18, 11, 10, 4});
});

CB_TEST("java.minecraft.editions.package_role_order", [] {
    // The shared 'util' role comes first and an edition that restates it changes the text, not the position.
    CB_CHECK_EQ(mcp::minecraft_rubydung::package_roles().begin()->first, "util");
    CB_CHECK_EQ(mcp::minecraft_rubydung::package_roles().at("util"), "Standalone utility helpers (math, noise, etc.).");
    CB_CHECK_EQ(mcp::minecraft_indev::package_roles().begin()->first, "util");
    CB_CHECK_EQ(mcp::minecraft_indev::package_roles().at("util"),
                "Misc helpers: MathHelper, IProgressUpdate (progress callback).");
    CB_CHECK_EQ(mcp::minecraft_infdev::package_roles().begin()->first, "util");
    std::string last;
    for (const auto& kv : mcp::minecraft_indev::package_roles()) last = kv.first;
    CB_CHECK_EQ(last, "net.minecraft.client.sound");
    // release = the xbox360 roles in their order, then the three release-era packages
    const auto& rel = mcp::minecraft_release::package_roles();
    CB_CHECK_EQ(rel.begin()->first, "net.minecraft.game");
    CB_CHECK_EQ(rel.at("net.minecraft.server"),
                "Integrated server: world ticking, save, and player management.");
    // 'net.minecraft.client' already exists in the xbox360 table: its text is replaced, its position is kept.
    CB_CHECK_EQ(rel.at("net.minecraft.client"), "Client game shell, input and main loop.");
    last.clear();
    for (const auto& kv : rel) last = kv.first;
    CB_CHECK_EQ(last, "net.minecraft.server");
});

CB_TEST("java.minecraft.editions.vocabulary", [] {
    CB_CHECK_EQ(mcp::minecraft_rubydung::static_init_order().front(), "Tile");
    CB_CHECK(mcp::minecraft_rubydung::nullable().contains("tiles"));
    CB_CHECK((mcp::minecraft_indev::static_init_order()) ==
             (std::vector<std::string>{"Material", "Block", "Item", "CraftingManager"}));
    CB_CHECK_EQ(mcp::minecraft_indev::nullable_getter_return_type().at("getIntermediateWithZValue"), "Vec3D");
    CB_CHECK_EQ(mcp::minecraft_infdev::nullable_getter_return_type().at("getChunk"), "Chunk");
    CB_CHECK(mcp::minecraft_infdev::nullable_fields().contains("textures"));
    // alpha and beta are the canonical profile under another name; release adds only package roles
    const Profile x = mcp::minecraft_xbox360::profile();
    for (const Profile& p : {mcp::minecraft_alpha::profile(), mcp::minecraft_beta::profile(),
                             mcp::minecraft_release::profile()}) {
        CB_CHECK(p.static_init_order == x.static_init_order);
        CB_CHECK(p.type.nullable_fields == x.type.nullable_fields);
        CB_CHECK(p.type.nullable_getter_return_type == x.type.nullable_getter_return_type);
        CB_CHECK(p.vcxproj_filters.size() == x.vcxproj_filters.size());
        for (std::size_t i = 0; i < x.vcxproj_filters.size(); ++i) {
            CB_CHECK_EQ(p.vcxproj_filters[i].pattern, x.vcxproj_filters[i].pattern);
            CB_CHECK_EQ(p.vcxproj_filters[i].folder, x.vcxproj_filters[i].folder);
        }
    }
});

}  // namespace
