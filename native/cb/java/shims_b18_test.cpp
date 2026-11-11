// The shims at the old header paths for java/emit (naming, std_transforms), java/console/platform_equivalents,
// java/profiles and java/projects/minecraft (achievements, content, controller_nav, keybind_mark, raycast, ui_mark,
// profiles/minecraft_base, profiles/minecraft_xbox360): they must compile with the old layer and forward to the new
// code. Built at /W3 (the old layer is not warning-clean).
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "codebreaker/java/console/platform_equivalents.hpp"
#include "codebreaker/java/emit/naming.hpp"
#include "codebreaker/java/emit/std_transforms.hpp"
#include "codebreaker/java/profiles/package.hpp"
#include "codebreaker/java/projects/minecraft/achievements.hpp"
#include "codebreaker/java/projects/minecraft/content.hpp"
#include "codebreaker/java/projects/minecraft/controller_nav.hpp"
#include "codebreaker/java/projects/minecraft/keybind_mark.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_base.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
#include "codebreaker/java/projects/minecraft/raycast.hpp"
#include "codebreaker/java/projects/minecraft/ui_mark.hpp"

namespace {

namespace jv = codebreaker::java;
namespace mc = codebreaker::java::projects::minecraft;
using cb::compat::from_py;
using cb::compat::to_py;
namespace stdfs = std::filesystem;

CB_TEST("java.shims_b18.naming", [] {
    namespace n = jv::emit::naming;
    CB_CHECK_EQ(from_py(n::static_field_name(py::str("__count"))), "s_count");
    CB_CHECK_EQ(from_py(n::instance_field_name(py::str("x"))), "x");
    CB_CHECK_EQ(from_py(n::field_name(n::Field{py::str("_x"), true})), "s_x");
    CB_CHECK_EQ(from_py(n::field_name(n::Field{py::str("_x"), false})), "_x");
    CB_CHECK_EQ(from_py(n::guard_macro(py::str("Vec3D"))), "VEC3D_H");
    CB_CHECK_EQ(from_py(n::enum_member(py::str("NULL"))), "NULL_");
    CB_CHECK_EQ(from_py(n::enum_member(py::str("STONE"))), "STONE");
});

CB_TEST("java.shims_b18.std_transforms", [] {
    namespace st = jv::emit::std_transforms;
    CB_CHECK_EQ(from_py(st::apply_std_transforms(py::str("a = NULL;\n"), st::CppStandard{2011})),
                "a = nullptr;  // [C++11] nullptr replaces NULL\n");
    CB_CHECK_EQ(from_py(st::apply_std_transforms(py::str("a = NULL;\n"), st::CppStandard{2003})), "a = NULL;\n");
    CB_CHECK_EQ(from_py(st::apply_std_transforms(py::str("a = NULL;\n"), std::nullopt)), "a = NULL;\n");
    CB_CHECK_EQ(from_py(st::sanitize_reserved_members(py::str("E::NULL x"))), "E::NULL_ x");
    CB_CHECK_EQ(from_py(st::_annotate_first(py::str("a X\nb"), py::str("X"), py::str("// n"))), "a X  // n\nb");
    // (_sub_outside_literals takes an old py::re pattern, whose engine cb_tests does not link: it is compiled, not run)
});

CB_TEST("java.shims_b18.platform_equivalents", [] {
    namespace pe = jv::console::platform_equivalents;
    CB_CHECK_EQ(pe::EQUIV.size(), 10);
    auto gl = pe::lookup(py::str("GL11"));
    CB_CHECK(gl.has_value() && gl->lib == "d3d9.lib" && gl->status == "real" && gl->note.has_value() && !gl->methods.has_value());
    auto glu = pe::lookup(py::str("GLU"));
    CB_CHECK(glu.has_value() && glu->methods.has_value() && glu->methods->contains(py::str("gluLookAt")));
    CB_CHECK(!pe::lookup(py::str("Nope")).has_value());
    CB_CHECK_EQ(from_py(pe::kind(*gl)), "xdk");
    CB_CHECK_EQ(from_py(pe::kind(*pe::lookup(py::str("DataInputStream")))), "handwritten");
    CB_CHECK_EQ(from_py(pe::kind(*pe::lookup(py::str("CodecWav")))), "dead");
    CB_CHECK_EQ(from_py(pe::annotation(py::str("Socket"))),
                "// [360] equivalent: XNetStartup(BYPASS_SECURITY) + socket/connect/send/recv (Winsock)  (xnet.lib)");
    CB_CHECK_EQ(from_py(pe::annotation(py::str("Nope"))), "");
    CB_CHECK_EQ(pe::all_entries().size(), 45);
    CB_CHECK_EQ(from_py(std::get<0>(pe::all_entries()[0])), "video");
    CB_CHECK_EQ(from_py(std::get<1>(pe::all_entries()[0])), "GL11");
});

CB_TEST("java.shims_b18.minecraft_profiles", [] {
    namespace xbox = mc::profiles::minecraft_xbox360;
    namespace base = mc::profiles::minecraft_base;
    CB_CHECK_EQ(xbox::STATIC_INIT_ORDER.size(), 10);
    CB_CHECK_EQ(xbox::VCXPROJ_FILTERS.size(), 55);
    CB_CHECK_EQ(xbox::PACKAGE_ROLES.size(), 17);
    CB_CHECK(xbox::NULLABLE_FIELDS.contains(py::str("thePlayer")));
    CB_CHECK_EQ(from_py(xbox::NULLABLE_GETTER_RETURN_TYPE.at(py::str("getItem"))), "Item");
    CB_CHECK_EQ(xbox::DEAD_TYPES.size(), 747);
    CB_CHECK_EQ(from_py(xbox::PROFILE.name), "minecraft_xbox360");
    CB_CHECK(xbox::PROFILE.dead_types.has_value() && !xbox::PROFILE.optional_types.has_value());
    CB_CHECK(xbox::PROFILE.heavy_types_high.empty());

    CB_CHECK(base::SHARED_DEAD_TYPES == xbox::DEAD_TYPES);
    CB_CHECK_EQ(base::SHARED_HEAVY_TYPES.size(), 4);
    CB_CHECK_EQ(base::SHARED_PACKAGE_ROLES.size(), 1);
    CB_CHECK_EQ(base::FULL_FILTERS.size(), 55);
    CB_CHECK_EQ(base::build_filters().size(), 55);
    py::list<std::tuple<py::str, py::str>> era;
    era.append(std::make_tuple(py::str("^Tile$"), py::str("Era")));
    const auto rules = base::build_filters(era);
    CB_CHECK_EQ(rules.size(), 56);
    CB_CHECK_EQ(from_py(std::get<1>(rules[0])), "Era");                       // era rules come first
    CB_CHECK_EQ(from_py(std::get<0>(rules[1])), from_py(std::get<0>(xbox::VCXPROJ_FILTERS[0])));
});

CB_TEST("java.shims_b18.profile_registry", [] {
    // (get() and use() ask the old project layer to load a project, which cb_tests does not link: they are compiled,
    // not run, and the registry they sit on is what is checked here)
    namespace jp = jv::profiles;
    namespace xbox = mc::profiles::minecraft_xbox360;
    CB_CHECK_EQ(from_py(jp::GENERIC()->name), "generic");
    CB_CHECK(jp::active() == jp::GENERIC());
    const jp::Profile mine = jp::from_cb(cb::java::projects::minecraft::profiles::minecraft_xbox360::profile());
    CB_CHECK_EQ(from_py(mine.name), "minecraft_xbox360");
    CB_CHECK(mine.static_init_order.size() == xbox::STATIC_INIT_ORDER.size() && mine.vcxproj_filters.size() == 55);
    CB_CHECK(mine.nullable_fields.contains(py::str("worldObj")) && mine.dead_types.has_value());
    CB_CHECK(mine.heavy_types_high.empty() && mine.unsigned_byte_scalar_fields.empty());
    auto reg = jp::register_(mine);                                                 // the registry holds this very object
    reg->heavy_types_high = py::list<py::str>{py::str("Tessellator")};              // ...which stays shared and mutable
    bool listed = false;
    for (const py::str& n : jp::names()) listed |= n == "minecraft_xbox360";
    CB_CHECK(listed);
    CB_CHECK_EQ(from_py(jp::names()[0]), "generic");                                          // sorted
    int seen = 0;
    CB_CHECK_EQ(jp::on_change([&](const jp::Profile&) { ++seen; }) != nullptr, true);
    CB_CHECK_EQ(seen, 0);
});

CB_TEST("java.shims_b18.text_passes", [] {
    // raycast: the templates and the rewrite
    namespace rc = mc::raycast;
    CB_CHECK_EQ(rc::RAYCAST_PICK_BODY_.size(), 2326);
    CB_CHECK_EQ(rc::EMPTY_PICK_HELPER_.size(), 193);
    CB_CHECK_EQ(from_py(rc::convert_gl_pick_to_raycast(py::str("void A::f() { glPushName(1); }"))),
                "void A::f() {" + from_py(rc::EMPTY_PICK_HELPER_) + "}");

    // controller_nav: (text, ok)
    namespace cn = mc::controller_nav;
    auto [text, ok] = cn::patch_minecraft_callsite(py::str("\n\tx->updateScreen();"));
    CB_CHECK(ok);
    CB_CHECK_EQ(from_py(text), "\n\tx->handleControllerMenuNav();\n\tx->updateScreen();");
    CB_CHECK(!std::get<1>(cn::patch_guiscreen_h(py::str("class A {};"))));
    CB_CHECK(std::get<1>(cn::patch_mousehelper_grab(py::str("CONSOLE grab (CodeBreaker)"))));

    // content: the category helper
    CB_CHECK_EQ(from_py(mc::content::_category_of(py::str("/gui/gui.png"))), "GUI textures");
});

CB_TEST("java.shims_b18.tree_passes", [] {
    cb::fs::ScopedTempDir tmp("cbshimb18");
    const std::string src = cb::fs::to_text(tmp.path());
    cb::fs::write_text(tmp.path() / "GuiScreen.h", "class GuiScreen : public Gui {};\nclass Hud : public GuiScreen {};\n");
    cb::fs::write_text(tmp.path() / "GuiScreen.cpp", "void GuiScreen::draw()\n{\n}\nvoid Hud::draw() { }\n");
    cb::fs::write_text(tmp.path() / "Minecraft.cpp", "if (Keyboard::isKeyDown(Keyboard::KEY_F1)) { }\n");

    namespace ui = mc::ui_mark;
    CB_CHECK(ui::is_ui_class_name(py::str("GuiButton")));
    CB_CHECK_EQ(ui::discover_ui_classes(to_py(src)).size(), 2);
    auto dry = ui::run(to_py(src), true);
    CB_CHECK(!dry.index.has_value() && dry.marked.size() == 2);
    auto marked = ui::run(to_py(src));
    CB_CHECK(marked.index.has_value());
    CB_CHECK_EQ(std::get<1>(marked.marked.at(py::str("GuiScreen"))[0]), 1);
    CB_CHECK_EQ(from_py(std::get<2>(marked.marked.at(py::str("Hud"))[0])), "draw");
    CB_CHECK_EQ(from_py(ui::render_index(marked)).find("## GuiScreen (1)") != std::string::npos, true);
    CB_CHECK(cb::fs::read_text(tmp.path() / "GuiScreen.cpp").find("// J2X-UI[GuiScreen::draw]") != std::string::npos);

    namespace kb = mc::keybind_mark;
    CB_CHECK_EQ(from_py(kb::_action(py::str("F1"))), "hide GUI / HUD");
    CB_CHECK_EQ(kb::_keys_on_line(py::str("Keyboard::isKeyDown(Keyboard::KEY_F1)")).size(), 1);
    auto r = kb::run(to_py(src));
    CB_CHECK_EQ(r.sites.size(), 1);
    CB_CHECK_EQ(std::get<1>(r.sites[0]), 1);
    CB_CHECK_EQ(from_py(std::get<0>(r.sites[0])), "Minecraft.cpp");
    CB_CHECK(r.index.has_value());
    CB_CHECK(from_py(kb::render_index(r)).find("`KEY_F1` | hide GUI / HUD | `Minecraft.cpp:1`") != std::string::npos);
    auto again = kb::mark(to_py(src));
    CB_CHECK_EQ(again.skipped, 1);

    // content: manifest of a Java tree
    cb::fs::write_text(tmp.path() / "java" / "A.java", "x(\"/gui/gui.png\"); y.playSound(\"random.click\", 1);\n");
    auto assets = mc::content::scan_assets(to_py(cb::fs::to_text(tmp.path() / "java")));
    CB_CHECK_EQ(assets.resources.size(), 1);
    CB_CHECK_EQ(from_py(assets.sounds.at(py::str("random.click"))[0]), "A");
    const std::string out = from_py(mc::content::write_content_md(to_py(cb::fs::to_text(tmp.path() / "java")), to_py(cb::fs::to_text(tmp.path() / "o")), py::str("T")));
    CB_CHECK(stdfs::exists(stdfs::path(out)));
    CB_CHECK(from_py(mc::content::render_content_md(to_py(cb::fs::to_text(tmp.path() / "java")))).find("asset manifest for this build") != std::string::npos);
});

CB_TEST("java.shims_b18.achievements", [] {
    namespace ac = mc::achievements;
    CB_CHECK_EQ(from_py(*ac::normalize_title_id(py::str("0xfffe0001"))), "FFFE0001");
    CB_CHECK(!ac::normalize_title_id(py::str("zz")).has_value());
    CB_CHECK(!ac::normalize_title_id(std::nullopt).has_value());
    CB_CHECK_EQ(from_py(ac::suggest_title_id(py::str("Minecraft"))), "FFFE5FA6");
    CB_CHECK_EQ(from_py(ac::suggest_title_id(std::nullopt)), "FFFE786B");

    cb::fs::ScopedTempDir tmp("cbshimb18");
    cb::fs::write_text(tmp.path() / "code" / "A.cpp",
                       "x = (new Achievement(3, \"mineWood\", 2, 1, Block::s_wood, openInventory))->setSpecial();\n");
    auto found = ac::scan_achievements(to_py(cb::fs::to_text(tmp.path() / "code")));
    CB_CHECK_EQ(found.size(), 1);
    CB_CHECK_EQ(found[0].id, 3);
    CB_CHECK(found[0].hidden && found[0].parent.has_value() && found[0].icon.has_value());
    auto res = ac::run(to_py(cb::fs::to_text(tmp.path() / "code")), to_py(cb::fs::to_text(tmp.path() / "out")));
    CB_CHECK_EQ(res.count, 1);
    CB_CHECK_EQ(from_py(res.title_id), "00000000");
    CB_CHECK_EQ(from_py(res.mapping.profile), "retail");
    CB_CHECK_EQ(res.mapping.total_score, 1000);
    CB_CHECK_EQ(res.mapping.entries.size(), 1);
    CB_CHECK_EQ(res.icons.unresolved.size(), 1);                                // no Content folder: say why
    CB_CHECK_EQ(res.code.size(), 2);
    CB_CHECK(stdfs::exists(stdfs::path(from_py(res.report))));
    CB_CHECK_THROWS_AS(py::KeyError, (void)ac::run(to_py(cb::fs::to_text(tmp.path() / "code")), to_py(cb::fs::to_text(tmp.path() / "o2")),
                                                     std::nullopt, py::str("gold")));
});

}  // namespace
