#include "java/projects/package.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "java/parse.hpp"

namespace {

namespace pr = cb::java::projects;
namespace ir = cb::java::ir;

// The real manifest of the Minecraft project, copied from the source tree.
std::filesystem::path real_projects() { return cb::testing::data_path("java/testdata/projects"); }

std::vector<ir::JClass> classes_of(std::initializer_list<const char*> sources) {
    std::vector<ir::JClass> out;
    for (const char* s : sources) out.push_back(cb::java::parse::parse_source(s));
    return out;
}

CB_TEST("java.projects.reads_the_real_manifest", [] {
    const pr::ProjectSet set(real_projects());
    CB_CHECK_EQ(set.manifests().size(), std::size_t{1});
    const pr::Manifest& m = set.manifests()[0];
    CB_CHECK_EQ(m.name, "minecraft");
    CB_CHECK((m.packages == std::vector<std::string>{"net.minecraft", "com.mojang"}));
    CB_CHECK((m.classes == std::vector<std::string>{"Minecraft", "MinecraftApplet", "MinecraftServer", "RubyDung"}));
    CB_CHECK((m.options == std::vector<std::string>{"edition", "mc_version"}));
    CB_CHECK((m.profile_prefixes == std::vector<std::string>{"minecraft_"}));
    CB_CHECK(m.description.starts_with("Minecraft, RubyDung to release"));
});

CB_TEST("java.projects.manifests_come_in_folder_order_and_default_their_name", [] {
    cb::fs::ScopedTempDir tmp("cbprojects");
    cb::fs::write_text(tmp.path() / "zeta" / "manifest.json", R"({"packages": ["z."]})");
    cb::fs::write_text(tmp.path() / "alpha" / "manifest.json", R"({"name": "Alpha Game", "classes": ["Main", 7], "options": "edition"})");
    cb::fs::write_text(tmp.path() / "empty" / "readme.txt", "no manifest here");
    cb::fs::write_text(tmp.path() / "loose.json", "{}");
    const pr::ProjectSet set(tmp.path());
    CB_CHECK_EQ(set.manifests().size(), std::size_t{2});
    CB_CHECK_EQ(set.manifests()[0].name, "Alpha Game");   // folder alpha: the manifest's own name wins
    CB_CHECK((set.manifests()[0].classes == std::vector<std::string>{"Main"}));   // a non-string entry is no class name
    CB_CHECK(set.manifests()[0].options.empty());                                 // "options" that is not a list is nothing
    CB_CHECK_EQ(set.manifests()[1].name, "zeta");   // no "name": the folder's
    CB_CHECK(pr::ProjectSet(tmp.path() / "absent").manifests().empty());
});

CB_TEST("java.projects.a_broken_manifest_is_a_format_error", [] {
    cb::fs::ScopedTempDir tmp("cbprojects");
    cb::fs::write_text(tmp.path() / "bad" / "manifest.json", "{ not json");
    CB_CHECK_THROWS_AS(cb::FormatError, (void)pr::ProjectSet(tmp.path()));
    cb::fs::write_text(tmp.path() / "bad" / "manifest.json", "[1, 2]");
    CB_CHECK_THROWS_AS(cb::FormatError, (void)pr::ProjectSet(tmp.path()));
});

CB_TEST("java.projects.matches", [] {
    const pr::ProjectSet set(real_projects());
    const pr::Manifest& m = set.manifests()[0];
    CB_CHECK(pr::matches(m, classes_of({"package net.minecraft.src;\npublic class Block { }"})));
    CB_CHECK(pr::matches(m, classes_of({"package a;\npublic class A { }", "package com.mojang.nbt;\npublic class B { }"})));
    CB_CHECK(pr::matches(m, classes_of({"public class RubyDung { }"})));   // by name, in no package
    CB_CHECK(!pr::matches(m, classes_of({"package org.apache;\npublic class Block { }"})));
    CB_CHECK(!pr::matches(m, {}));
});

CB_TEST("java.projects.choose", [] {
    const pr::ProjectSet set(real_projects());
    const auto mc = classes_of({"package net.minecraft.src;\npublic class Block { }"});
    const auto other = classes_of({"package org.apache;\npublic class Thing { }"});
    CB_CHECK_EQ(set.choose(other), "generic");
    CB_CHECK_EQ(set.choose(mc), "minecraft");                                    // by the program's marks
    CB_CHECK_EQ(set.choose(other, "", {.edition = "alpha"}), "minecraft");       // by a claimed option
    CB_CHECK_EQ(set.choose(other, "", {.mc_version = "1.2"}), "minecraft");
    CB_CHECK_EQ(set.choose(mc, "generic"), "generic");                           // asked for beats everything
    CB_CHECK_EQ(set.choose(other, "minecraft"), "minecraft");
    CB_CHECK_THROWS_AS(cb::UsageError, (void)set.choose(other, "tetris"));
    CB_CHECK((pr::ChooseOptions{.edition = "x"}).given() == std::set<std::string, std::less<>>{"edition"});
    CB_CHECK(pr::ChooseOptions{}.given().empty());
});

CB_TEST("java.projects.for_profile", [] {
    const pr::ProjectSet set(real_projects());
    CB_CHECK_EQ(set.for_profile("minecraft_alpha").value_or("-"), "minecraft");
    CB_CHECK(!set.for_profile("xbox360_default"));
    CB_CHECK(!set.for_profile("minecraft"));   // the prefix is `minecraft_`
});

CB_TEST("java.projects.project_lookup", [] {
    pr::ProjectSet set(real_projects());
    CB_CHECK_EQ(set.project("generic").name, "generic");
    CB_CHECK(&set.project("") == &set.project("generic"));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)set.project("tetris"));
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)set.project("minecraft"));   // a manifest, but no code in this set
    auto mc = std::make_unique<pr::Project>();
    const pr::Project* raw = mc.get();
    set.add("minecraft", std::move(mc));
    CB_CHECK(&set.project("minecraft") == raw);
    CB_CHECK_EQ(set.project("minecraft").name, "minecraft");
});

CB_TEST("java.projects.the_generic_project_does_nothing", [] {
    pr::Project generic;
    CB_CHECK(!generic.profile_for("", "", {}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)generic.profile_for("alpha", "", {}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)generic.profile_for("", "1.0", {}));
    CB_CHECK_EQ(generic.finalize_text("int x;", "xbox360"), "int x;");
    cb::Arena arena;
    const pr::Patched patched = generic.ir_patches(arena, {}, std::nullopt);
    CB_CHECK(!patched.sound && !patched.applet && !patched.render);
    int said = 0;
    generic.after_emit("out", std::nullopt, patched, [&](std::string_view) { ++said; });
    generic.after_construction("out", std::nullopt, [&](std::string_view) { ++said; });
    generic.reports("java", "out", {}, [&](std::string_view) { ++said; });
    CB_CHECK_EQ(said, 0);
    CB_CHECK_EQ(generic.registrars[0], R"(register\w*)");
});

}  // namespace
