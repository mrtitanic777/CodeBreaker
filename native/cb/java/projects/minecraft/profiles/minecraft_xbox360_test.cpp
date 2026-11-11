#include <string>
#include <vector>

#include "cb/core/regex.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace {

using namespace cb;
namespace xbox = cb::java::projects::minecraft::profiles::minecraft_xbox360;
namespace base = cb::java::projects::minecraft::profiles::minecraft_base;
using cb::java::profiles::FilterRule;

// The folder of the first rule whose pattern matches the class basename ("" = falls through to 'Game (misc)').
std::string classify(const std::vector<FilterRule>& rules, std::string_view cls) {
    for (const FilterRule& r : rules) {
        const Regex re(r.pattern);   // (a test: built per call on purpose; production code compiles each rule once)
        if (re.search(cls)) return r.folder;
    }
    return "";
}

// Expected folders for sample class names, computed with Python's re over the original table (first match wins).
struct Case {
    const char* cls;
    const char* folder;
};
const Case kCases[] = {
#include "testdata/classify_cases.inc"
};

CB_TEST("java.minecraft.xbox360.table_sizes", [] {
    CB_CHECK_EQ(xbox::static_init_order().size(), std::size_t{10});
    CB_CHECK_EQ(xbox::static_init_order().front(), "MathHelper");
    CB_CHECK_EQ(xbox::static_init_order().back(), "Packet");
    CB_CHECK_EQ(xbox::package_roles().size(), std::size_t{17});
    CB_CHECK_EQ(xbox::package_roles().begin()->first, "net.minecraft.game");        // the order is the report's
    CB_CHECK_EQ(xbox::package_roles().at("util"), "Standalone utility helpers (math, noise, etc.).");
    CB_CHECK_EQ(xbox::heavy_types().size(), std::size_t{4});
    CB_CHECK_EQ(xbox::nullable().size(), std::size_t{2});
    CB_CHECK_EQ(xbox::nullable_fields().size(), std::size_t{18});
    CB_CHECK_EQ(xbox::nullable_getters().size(), std::size_t{11});
    CB_CHECK_EQ(xbox::nullable_getter_return_type().size(), std::size_t{10});
    CB_CHECK_EQ(xbox::vcxproj_filters().size(), std::size_t{55});
    CB_CHECK_EQ(xbox::dead_types().size(), std::size_t{747});
    // every getter with a pointer return type is one of the nullable getters; getSmeltingResultItem returns int
    for (const auto& [getter, type] : xbox::nullable_getter_return_type()) {
        CB_CHECK(xbox::nullable_getters().contains(getter));
        CB_CHECK(!type.empty());
    }
    CB_CHECK(xbox::nullable_getters().contains("getSmeltingResultItem"));
    CB_CHECK(!xbox::nullable_getter_return_type().contains("getSmeltingResultItem"));
});

CB_TEST("java.minecraft.xbox360.profile", [] {
    const auto p = xbox::profile();
    CB_CHECK_EQ(p.name, "minecraft_xbox360");
    CB_CHECK((p.static_init_order) == (xbox::static_init_order()));
    CB_CHECK((p.heavy_types) == (xbox::heavy_types()));
    CB_CHECK(p.heavy_types_high.empty());
    CB_CHECK_EQ(p.vcxproj_filters.size(), std::size_t{55});
    CB_CHECK(p.dead_types && *p.dead_types == xbox::dead_types());
    CB_CHECK(!p.optional_types);
    CB_CHECK(p.type.nullable.contains("blocksList") && p.type.nullable.contains("itemsList"));
    CB_CHECK(p.type.nullable_fields.contains("thePlayer"));
    CB_CHECK(p.type.unsigned_byte_scalar_fields.empty() && p.type.default_value_types.empty());
});

CB_TEST("java.minecraft.xbox360.filters_compile_and_are_anchored", [] {
    for (const FilterRule& r : xbox::vcxproj_filters()) {
        CB_CHECK(!r.folder.empty());
        CB_CHECK(r.pattern.starts_with("^"));
        CB_CHECK(Regex::compile(r.pattern).ok());
    }
    CB_CHECK(Regex::compile(std::string(xbox::dead_types())).ok());
});

CB_TEST("java.minecraft.xbox360.classification", [] {
    for (const Case& c : kCases) CB_CHECK_EQ(classify(xbox::vcxproj_filters(), c.cls), c.folder);
});

CB_TEST("java.minecraft.xbox360.dead_types_are_precise", [] {
    const Regex dead{std::string(xbox::dead_types())};
    for (const char* name : {"Frame", "Applet", "BufferedImage", "ImageIO", "IsomPreview", "IsomPreviewFoo", "Iso",
                             "KeyListener", "WindowEvent", "NodeList"}) {
        CB_CHECK(dead.search(name));
    }
    // never a wildcard that sweeps in a real game type
    for (const char* name : {"LevelListener", "FooEvent", "Frames", "MyFrame", "Block", "Packet", ""}) {
        CB_CHECK(!dead.search(name));
    }
});

CB_TEST("java.minecraft.base.shared_pieces", [] {
    CB_CHECK_EQ(base::shared_dead_types(), xbox::dead_types());
    CB_CHECK((base::shared_heavy_types()) == (xbox::heavy_types()));
    CB_CHECK_EQ(base::full_filters().size(), xbox::vcxproj_filters().size());
    const auto& roles = base::shared_package_roles();
    CB_CHECK_EQ(roles.size(), std::size_t{1});
    CB_CHECK_EQ(roles.at("util"), xbox::package_roles().at("util"));
});

CB_TEST("java.minecraft.base.build_filters_era_rules_win", [] {
    const auto plain = base::build_filters();
    CB_CHECK_EQ(plain.size(), xbox::vcxproj_filters().size());
    CB_CHECK_EQ(classify(plain, "Tile"), "Rendering\\Renderers");

    const std::vector<FilterRule> era = {{"^Tile$", "Era\\Tiles"}, {"^Zombie$", "Era\\Mobs"}};
    const auto rules = base::build_filters(era);
    CB_CHECK_EQ(rules.size(), xbox::vcxproj_filters().size() + 2);
    CB_CHECK_EQ(rules[0].folder, "Era\\Tiles");                    // era rules first
    CB_CHECK_EQ(classify(rules, "Tile"), "Era\\Tiles");            // so they override the shared taxonomy
    CB_CHECK_EQ(classify(rules, "Zombie"), "Era\\Mobs");
    CB_CHECK_EQ(classify(rules, "GuiMainMenu"), "GUI\\Screens");   // everything else falls through to it
    // the shared table itself is untouched by building an edition's
    CB_CHECK_EQ(base::full_filters().size(), xbox::vcxproj_filters().size());
});

}  // namespace
