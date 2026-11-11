#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/content.hpp"

namespace {

using namespace cb;
using namespace cb::java::projects::minecraft::content;
namespace stdfs = std::filesystem;

stdfs::path java_tree() { return testing::data_path("java/projects/minecraft/testdata/content_java"); }

using Names = std::vector<std::string>;

bool has(const std::string& s, std::string_view part) { return s.find(part) != std::string::npos; }

CB_TEST("java.minecraft.content.category_of", [] {
    CB_CHECK_EQ(category_of("/lang/en_US.lang"), "Language files");
    CB_CHECK_EQ(category_of("/font/default.png"), "Font");
    CB_CHECK_EQ(category_of("/font.txt"), "Font");                      // "/font" is a prefix, not a folder
    CB_CHECK_EQ(category_of("/gui/gui.png"), "GUI textures");
    CB_CHECK_EQ(category_of("/title/mojang.png"), "Title / menu textures");
    CB_CHECK_EQ(category_of("/terrain.png"), "Terrain atlas");
    CB_CHECK_EQ(category_of("/terrain/sun.png"), "Terrain atlas");
    CB_CHECK_EQ(category_of("/item/arrows.png"), "Item textures");
    CB_CHECK_EQ(category_of("/items.png"), "Item textures");
    CB_CHECK_EQ(category_of("/mob/pig.png"), "Mob textures");
    CB_CHECK_EQ(category_of("/armor/chain_1.png"), "Armor textures");
    CB_CHECK_EQ(category_of("/environment/clouds.png"), "Environment (sky/weather)");
    CB_CHECK_EQ(category_of("/art/kz.png"), "Painting art");
    CB_CHECK_EQ(category_of("/achievement/bg.png"), "Achievement textures");
    CB_CHECK_EQ(category_of("/misc/shadow.png"), "Misc textures");
    CB_CHECK_EQ(category_of("/data/custom.properties"), "Other resources");
    CB_CHECK_EQ(category_of(""), "Other resources");
});

CB_TEST("java.minecraft.content.scan_assets", [] {
    const Assets a = scan_assets(java_tree());

    // resource paths: every quoted "/dir/name.ext" with a shipped extension, comments included (they are text too);
    // not a path with spaces, not an upper-case extension, not a file that is not .java
    CB_CHECK_EQ(a.resources.size(), std::size_t{12});
    CB_CHECK((a.resources.at("/font/default.png")) == (Names{"Minecraft"}));
    CB_CHECK((a.resources.at("/font.txt")) == (Names{"Minecraft"}));
    CB_CHECK((a.resources.at("/lang/en_US.lang")) == (Names{"Minecraft"}));
    CB_CHECK((a.resources.at("/lang/ignored.lang")) == (Names{"Minecraft"}));
    CB_CHECK((a.resources.at("/terrain.png")) == (Names{"Block"}));
    CB_CHECK((a.resources.at("/gui/gui.png")) == (Names{"RenderGlobal"}));
    CB_CHECK((a.resources.at("/data/custom.properties")) == (Names{"RenderGlobal"}));
    CB_CHECK((a.resources.at("/shaders/post.fsh")) == (Names{"RenderGlobal"}));
    CB_CHECK(!a.resources.contains("/terrain/sun.PNG"));
    CB_CHECK(!a.resources.contains("/misc/not an asset.png"));
    CB_CHECK(!a.resources.contains("/gui/not_java.png"));

    // sound events: dotted lower-case names at a play call; a class that uses one twice is listed once
    CB_CHECK_EQ(a.sounds.size(), std::size_t{5});
    CB_CHECK((a.sounds.at("menu.title")) == (Names{"Minecraft"}));
    CB_CHECK((a.sounds.at("random.click")) == ((Names{"Block", "RenderGlobal"})));   // sorted
    CB_CHECK((a.sounds.at("random.pop")) == ((Names{"Block", "RenderGlobal"})));
    CB_CHECK((a.sounds.at("step.stone")) == (Names{"Block"}));
    CB_CHECK((a.sounds.at("note.harp")) == (Names{"RenderGlobal"}));
    CB_CHECK(!a.sounds.contains("click"));                                          // an event has a dot

    // StepSound families keyed by the first argument: lower-case names only
    CB_CHECK_EQ(a.step_sounds.size(), std::size_t{3});
    CB_CHECK((a.step_sounds.at("stone")) == (Names{"Block"}));
    CB_CHECK((a.step_sounds.at("grass")) == (Names{"Block"}));
    CB_CHECK((a.step_sounds.at("wood")) == (Names{"Block"}));
    CB_CHECK(!a.step_sounds.contains("Stone"));
});

CB_TEST("java.minecraft.content.non_ascii_names", [] {
    // a class file named with a non-ASCII letter, and a resource path holding one: both pass through as UTF-8
    fs::ScopedTempDir tmp("cbcontent");
    fs::write_text(tmp.path() / stdfs::u8path("Caf\xC3\xA9.java"), "bind(\"/uni/caf\xC3\xA9.png\"); bind(\"/uni/x.png\");\n");
    const Assets a = scan_assets(tmp.path());
    CB_CHECK_EQ(a.resources.size(), std::size_t{2});
    CB_CHECK(a.resources.at("/uni/caf\xC3\xA9.png") == Names{"Caf\xC3\xA9"});
    CB_CHECK(a.resources.at("/uni/x.png") == Names{"Caf\xC3\xA9"});
});

CB_TEST("java.minecraft.content.scan_missing_or_empty", [] {
    CB_CHECK(scan_assets(java_tree() / "nowhere").resources.empty());
    fs::ScopedTempDir tmp("cbcontent");
    const Assets none = scan_assets(tmp.path());
    CB_CHECK(none.resources.empty() && none.sounds.empty() && none.step_sounds.empty());
});

CB_TEST("java.minecraft.content.render_small", [] {
    Assets a;
    a.resources = {{"/gui/gui.png", {"A", "B", "C", "D", "E"}}, {"/lang/en.lang", {"A"}}};
    a.sounds = {{"random.click", {"A", "B", "C", "D"}}, {"step.stone", {"A"}}};
    a.step_sounds = {{"stone", {"Block"}}};
    const std::string md = render_content_md(a, "Alpha 1.2");
    // the title and the counts line
    CB_CHECK(md.starts_with("# Content.md \xE2\x80\x94 asset manifest for Alpha 1.2\n\nEverything the converted game loads"));
    CB_CHECK(has(md, "Counts: 2 resource files, 2 sound events, 1 step-sound families."));
    // categories in name order; paths use backslashes under Content; more than four users are cut with an ellipsis
    CB_CHECK(has(md, "### GUI textures\n\n| Content path | Referenced by |\n|---|---|\n"
                         "| `Content\\gui\\gui.png` | A, B, C, D \xE2\x80\xA6 |\n"));
    CB_CHECK(has(md, "### Language files\n\n| Content path | Referenced by |\n|---|---|\n| `Content\\lang\\en.lang` | A |\n"));
    CB_CHECK(md.find("### GUI textures") < md.find("### Language files"));
    // sound events: folders from the dots, three users shown at most
    CB_CHECK(has(md, "| `random.click` | `Content\\sound\\random\\click.ogg` | A, B, C \xE2\x80\xA6 |\n"));
    CB_CHECK(has(md, "| `step.stone` | `Content\\sound\\step\\stone.ogg` | A |\n"));
    // step families
    CB_CHECK(has(md, "| Family | Events | Blocks |\n|---|---|---|\n| `stone` | `step.stone`, `dig.stone` | Block |\n"));
    CB_CHECK(md.ends_with("`Content\\music\\` (or an XACT streaming bank) and the title/menu loop.\n\n"));
});

CB_TEST("java.minecraft.content.render_without_step_sounds", [] {
    Assets a;
    a.resources = {{"/x/y.png", {"A"}}};
    const std::string md = render_content_md(a);
    CB_CHECK(has(md, "asset manifest for this build"));
    CB_CHECK(!has(md, "## Block step / dig sound families"));       // the section is left out when there is none
    CB_CHECK(has(md, "### Other resources"));
    CB_CHECK(has(md, "## Sound events"));                            // ...but the sound table is always there
});

CB_TEST("java.minecraft.content.write_manifest_golden", [] {
    fs::ScopedTempDir tmp("cbcontent");
    const stdfs::path out = write_content_md(java_tree(), tmp.path() / "deeper", "the fixture");
    CB_CHECK_EQ(fs::to_text(out), fs::to_text(tmp.path() / "deeper" / "Content.md"));
    const std::string got = fs::read_text(out);
    const std::string want = fs::read_text(testing::data_path("java/projects/minecraft/testdata/content.golden.md"));
    CB_CHECK_EQ(got, want);
    CB_CHECK_EQ(render_content_md(java_tree(), "the fixture"), want);
});

}  // namespace
