// DIVERGES from the Python registry (java/profiles/__init__.py):
//   * there is no process-wide registry: a ProfileSet is an object, and the loader that registers a project's profiles on demand
//     is a function the owner sets (the Python called back into the project layer from get()).
//   * `active()` is the profile registered under the active NAME: replacing that profile with add() is seen at once (the Python
//     kept the old object active until use() was called again).
//   * an unknown profile name is a UsageError that lists the known names (the Python raised KeyError).
//   * no on_change observers: whoever switches the active profile passes it to the stages that need it.

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/profiles/package.hpp"

namespace {

using namespace cb;
using namespace cb::java::profiles;

Profile make(std::string name) {
    Profile p;
    p.name = std::move(name);
    return p;
}

CB_TEST("java.profiles.generic_baseline", [] {
    ProfileSet set;
    // the generic profile is registered, active, and empty: the core stays game-agnostic until a target is chosen
    CB_CHECK((set.names()) == (std::vector<std::string>{"generic"}));
    CB_CHECK_EQ(set.active_name(), "generic");
    const Profile& g = set.generic();
    CB_CHECK_EQ(g.name, "generic");
    CB_CHECK(&g == &set.active());
    CB_CHECK(g.static_init_order.empty() && g.package_roles.empty() && g.heavy_types.empty());
    CB_CHECK(g.heavy_types_high.empty() && g.vcxproj_filters.empty());
    CB_CHECK(!g.dead_types && !g.optional_types);
    CB_CHECK(g.type.nullable.empty() && g.type.nullable_fields.empty() && g.type.nullable_getters.empty());
    CB_CHECK(g.type.nullable_getter_return_type.empty() && g.type.unsigned_byte_scalar_fields.empty());
    CB_CHECK(g.type.default_value_types.empty());
});

CB_TEST("java.profiles.add_find_get", [] {
    ProfileSet set;
    Profile a = make("alpha");
    a.static_init_order = {"Material", "Block"};
    a.package_roles.insert_or_assign("net.minecraft.game", "Top-level game wiring.");
    a.package_roles.insert_or_assign("util", "Helpers.");
    a.vcxproj_filters = {{"^Gui\\w*", "GUI"}, {"^Block$", "Blocks"}};
    a.dead_types = "^(Frame|Panel)$";
    a.type.nullable_fields = {"theWorld", "mc"};
    a.type.unsigned_byte_scalar_fields = {{"Packet", "dimension"}};
    Profile& stored = set.add(a);
    CB_CHECK_EQ(stored.name, "alpha");

    const Profile* found = set.find("alpha");
    CB_CHECK(found == &stored);
    CB_CHECK((found->static_init_order) == ((std::vector<std::string>{"Material", "Block"})));
    CB_CHECK_EQ(found->vcxproj_filters.size(), std::size_t{2});
    CB_CHECK_EQ(found->vcxproj_filters[1].folder, "Blocks");
    CB_CHECK_EQ(*found->dead_types, "^(Frame|Panel)$");
    CB_CHECK(found->type.nullable_fields.contains("theWorld"));
    CB_CHECK(found->type.unsigned_byte_scalar_fields.contains({"Packet", "dimension"}));
    // package roles keep the order they were given in
    CB_CHECK_EQ(found->package_roles.begin()->first, "net.minecraft.game");

    CB_CHECK(set.find("nothing") == nullptr);
    CB_CHECK_EQ(set.get("alpha").name, "alpha");
    CB_CHECK_THROWS_AS(UsageError, (void)set.get("nothing"));
    try {
        (void)set.get("nothing");
    } catch (const UsageError& e) {
        CB_CHECK(std::string(e.what()).find("alpha") != std::string::npos);     // the message lists what exists
    }
});

CB_TEST("java.profiles.add_replaces_in_place", [] {
    ProfileSet set;
    Profile& first = set.add(make("beta"));
    Profile second = make("beta");
    second.heavy_types = {"Tessellator"};
    Profile& stored = set.add(second);
    CB_CHECK(&first == &stored);                       // one entry per name; its address survives
    CB_CHECK_EQ(set.names().size(), std::size_t{2});
    CB_CHECK((set.get("beta").heavy_types) == (std::vector<std::string>{"Tessellator"}));
});

CB_TEST("java.profiles.names_are_sorted", [] {
    ProfileSet set;
    set.add(make("minecraft_release"));
    set.add(make("minecraft_alpha"));
    set.add(make("zeta"));
    CB_CHECK((set.names()) == ((std::vector<std::string>{"generic", "minecraft_alpha", "minecraft_release", "zeta"})));
});

CB_TEST("java.profiles.use_switches_active", [] {
    ProfileSet set;
    Profile p = make("minecraft_beta");
    p.heavy_types_high = {"Tesselator"};
    set.add(p);
    const Profile& used = set.use("minecraft_beta");
    CB_CHECK_EQ(used.name, "minecraft_beta");
    CB_CHECK(&used == &set.active());
    CB_CHECK_EQ(set.active_name(), "minecraft_beta");
    CB_CHECK_EQ(set.generic().name, "generic");        // the baseline is still there

    CB_CHECK_THROWS_AS(UsageError, (void)set.use("nope"));
    CB_CHECK_EQ(set.active_name(), "minecraft_beta");  // a failed switch changes nothing

    // active is the profile registered under the active name: replacing it is seen at once
    Profile replacement = make("minecraft_beta");
    replacement.heavy_types_high = {"Tessellator", "NBTTagCompound"};
    set.add(replacement);
    CB_CHECK_EQ(set.active().heavy_types_high.size(), std::size_t{2});
    CB_CHECK_EQ(set.use("generic").name, "generic");
});

CB_TEST("java.profiles.loader_registers_on_demand", [] {
    ProfileSet set;
    int calls = 0;
    set.set_loader([&](ProfileSet& s, std::string_view name) {
        ++calls;
        if (name.starts_with("minecraft_")) {     // the project owning the name registers all of its profiles
            s.add(make("minecraft_alpha"));
            s.add(make("minecraft_beta"));
        }
    });
    CB_CHECK_EQ(calls, 0);
    CB_CHECK(set.find("generic") != nullptr);       // a known name never asks the loader
    CB_CHECK_EQ(calls, 0);

    CB_CHECK_EQ(set.get("minecraft_beta").name, "minecraft_beta");
    CB_CHECK_EQ(calls, 1);
    CB_CHECK(set.find("minecraft_alpha") != nullptr);   // registered by the same load
    CB_CHECK_EQ(calls, 1);

    CB_CHECK(set.find("minecraft_gamma") == nullptr);   // owned by the project but still unknown
    CB_CHECK_EQ(calls, 2);
    CB_CHECK(set.find("unrelated") == nullptr);
    CB_CHECK_EQ(calls, 3);
    CB_CHECK_THROWS_AS(UsageError, (void)set.use("unrelated"));

    CB_CHECK_EQ(set.use("minecraft_alpha").name, "minecraft_alpha");
});

}  // namespace
