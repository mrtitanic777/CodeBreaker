// Tests of the Minecraft project: the profiles it registers, the edition it picks, and the hooks on both targets.

#include <memory>
#include <string>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/parse.hpp"
#include "cb/java/projects/minecraft/package.hpp"

namespace {

namespace mc = cb::java::projects::minecraft;
namespace pr = cb::java::projects;
namespace ir = cb::java::ir;

struct Lines {
    std::vector<std::string> lines;
    pr::Progress sink() {
        return [this](std::string_view s) { lines.emplace_back(s); };
    }
    bool has(std::string_view needle) const {
        for (const std::string& l : lines)
            if (l.find(needle) != std::string::npos) return true;
        return false;
    }
};

std::vector<ir::JClass> parsed(std::initializer_list<const char*> sources) {
    std::vector<ir::JClass> out;
    for (const char* s : sources) out.push_back(cb::java::parse::parse_source(s));
    return out;
}

CB_TEST("java.minecraft.package.registers_the_seven_era_profiles", [] {
    cb::java::profiles::ProfileSet set;
    mc::register_profiles(set);
    const std::vector<std::string> expected = {"generic",           "minecraft_alpha",    "minecraft_beta",    "minecraft_indev",
                                               "minecraft_infdev",  "minecraft_release",  "minecraft_rubydung", "minecraft_xbox360"};
    CB_CHECK(set.names() == expected);
    for (const std::string& name : set.names()) {
        const auto& p = set.get(name);
        if (name == "generic") {
            CB_CHECK(p.heavy_types_high.empty());
        } else {
            // the Tessellator's by-value copy is HIGH on every era
            CB_CHECK((p.heavy_types_high == std::vector<std::string>{"Tesselator", "Tessellator"}));
            CB_CHECK_EQ(p.name, name);
        }
    }
    CB_CHECK(!set.get("minecraft_xbox360").static_init_order.empty());
});

CB_TEST("java.minecraft.package.the_project_carries_its_data", [] {
    const mc::Minecraft m;
    CB_CHECK_EQ(m.name, "minecraft");
    CB_CHECK_EQ(m.char_map.at(0x2302), std::uint32_t{127});
    CB_CHECK_EQ(m.registrars.size(), std::size_t{4});
    CB_CHECK_EQ(m.registrars[2], R"(register\w*)");
});

CB_TEST("java.minecraft.package.profile_for_follows_edition_then_version_then_source", [] {
    const mc::Minecraft m;
    const std::vector<ir::JClass> none;
    CB_CHECK_EQ(m.profile_for("alpha", "", none).value(), "minecraft_alpha");
    CB_CHECK_EQ(m.profile_for("", "b1.6.6", none).value(), "minecraft_beta");
    CB_CHECK_EQ(m.profile_for("", "", parsed({"package com.mojang.rubydung;\npublic class RubyDung { }"})).value(), "minecraft_rubydung");
    // nothing decides: the default profile
    CB_CHECK_EQ(m.profile_for("", "", none).value(), std::string(mc::kDefaultProfile));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)m.profile_for("pocket", "", none));
});

CB_TEST("java.minecraft.package.the_console_fixes_run_only_on_the_console", [] {
    mc::Minecraft m;
    cb::Arena arena;
    const std::vector<cb::translate::ir::Node*> units;
    const pr::Patched other = m.ir_patches(arena, units, "pc");
    CB_CHECK(!other.sound && !other.applet && !other.render);
    const pr::Patched none = m.ir_patches(arena, units, std::nullopt);
    CB_CHECK(!none.sound && !none.applet && !none.render);
    const pr::Patched console = m.ir_patches(arena, units, "xbox360");
    CB_CHECK(console.sound && console.applet && console.render);
    CB_CHECK_EQ(console.sound->sites, std::size_t{0});

    const std::string gl = "void f() { int n = GL11::glRenderMode(GL11::GL_SELECT); }\n";
    CB_CHECK_EQ(m.finalize_text(gl, "pc"), gl);
    CB_CHECK_EQ(m.finalize_text(gl, std::nullopt), gl);
});

CB_TEST("java.minecraft.package.after_construction_tags_key_presses", [] {
    cb::fs::ScopedTempDir tmp("cbmcpkg");
    cb::fs::write_text(tmp.path() / "Minecraft.cpp",
                       "void Minecraft::runTick() {\n\tif (Keyboard::getEventKey() == Keyboard::KEY_F1) { toggleHud(); }\n}\n");
    mc::Minecraft m;
    Lines out;
    m.after_construction(tmp.path(), "pc", out.sink());
    CB_CHECK(out.lines.empty());   // not the console: untouched
    m.after_construction(tmp.path(), "xbox360", out.sink());
    CB_CHECK(out.has("[keybind] tagged 1 keyboard key-press site(s)"));
    CB_CHECK(cb::fs::read_text(tmp.path() / "Minecraft.cpp").find("J2X-KEYBIND[KEY_F1=") != std::string::npos);
    CB_CHECK(std::filesystem::exists(tmp.path() / "KEYBIND_INDEX.md"));
});

CB_TEST("java.minecraft.package.after_emit_on_a_clean_tree_says_nothing_but_the_menu_anchors", [] {
    cb::fs::ScopedTempDir tmp("cbmcpkg");
    mc::Minecraft m;
    Lines out;
    m.after_emit(tmp.path(), "pc", {}, out.sink());
    CB_CHECK(out.lines.empty());
    // the console, with nothing patched and a tree that has none of the files: only the controller anchors are missing
    m.after_emit(tmp.path(), "xbox360", {}, out.sink());
    CB_CHECK(out.has("[controller] WARNING: menu-nav anchor(s) not found"));
    CB_CHECK(!out.has("[sound]") && !out.has("[applet]") && !out.has("[render]"));
});

CB_TEST("java.minecraft.package.after_emit_reports_the_fixes_that_ran", [] {
    cb::fs::ScopedTempDir tmp("cbmcpkg");
    mc::Minecraft m;
    pr::Patched patched;
    patched.sound = cb::java::projects::minecraft::sound_fix::Report{};
    patched.sound->sites = 3;
    patched.sound->files = {{"SoundManager.cpp", 2, 1}};
    patched.render = cb::java::projects::minecraft::render_fix::Report{};
    patched.render->update = 1;
    patched.render->hurt = 2;
    Lines out;
    m.after_emit(tmp.path(), "xbox360", patched, out.sink());
    CB_CHECK(out.has("[sound] SFX+music bypass: 3 site(s) routed to the XACT bridge across 1 file(s)"));
    CB_CHECK(out.has("[render] chunk-rebuild budget applied to updateRenderers (1 site(s))"));
    CB_CHECK(out.has("[render] hurt-camera 0/0 NaN guard applied (2 site(s))"));
    CB_CHECK(!out.has("[applet]"));
});

CB_TEST("java.minecraft.package.reports_write_the_manifest_and_the_coverage", [] {
    cb::fs::ScopedTempDir tmp("cbmcpkg");
    const auto java_dir = cb::testing::data_path("java/projects/minecraft/testdata/content_java");
    mc::Minecraft m;
    Lines out;
    const std::vector<std::string> runtime = {"Thread.cpp"};
    m.reports(java_dir, tmp.path(), runtime, out.sink());
    CB_CHECK(out.has("[content] asset manifest written to"));
    CB_CHECK(out.has("[platform] coverage report written to"));
    CB_CHECK(!out.has("[achievements]"));   // the tree defines none
    CB_CHECK(std::filesystem::exists(tmp.path() / "Content.md"));
    CB_CHECK(std::filesystem::exists(tmp.path() / "PLATFORM_COVERAGE.md"));
});

}  // namespace
