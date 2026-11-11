// DIVERGES (checked against the Python on its 143 recorded cases -- 22 text patches each of the six kinds, run() over twelve
// fake converted trees, twice each; 2 differences, both here):
//   * patch_mousehelper_grab with only ONE of the two functions present returned the half-patched text with ok = false; it now
//     returns the text unchanged (nothing uses the text of a failed patch: run() drops it).
//   * files are written back with LF endings (the Python's text mode wrote CRLF on Windows).

#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/controller_nav.hpp"

namespace {

namespace fs = cb::fs;
namespace testing = cb::testing;
using namespace cb::java::projects::minecraft::controller_nav;
namespace stdfs = std::filesystem;
using Names = std::vector<std::string>;

// Fixtures: a converted tree in the converter's style (`in`, plus `in_container` for an edition with GuiContainer),
// and what the Python module produced for them (`expected*`: read and checked by hand -- the anchors it found, the
// order of the includes, the injected lines).
stdfs::path data(const char* name) { return testing::data_path(std::string("java/projects/minecraft/testdata/controller_nav/") + name); }

void copy_into(const stdfs::path& from, const stdfs::path& to) { fs::copy_tree(from, to); }

void expect_same_files(const stdfs::path& got, const stdfs::path& want) {
    std::size_t n = 0;
    for (const auto& entry : stdfs::directory_iterator(want)) {
        ++n;
        const std::string name = entry.path().filename().string();
        CB_CHECK_EQ(fs::read_text(got / name), fs::read_text(entry.path()));
    }
    std::size_t have = 0;
    for (const auto& entry : stdfs::directory_iterator(got)) {
        (void)entry;
        ++have;
    }
    CB_CHECK_EQ(have, n);   // nothing extra was written
}

CB_TEST("java.minecraft.controller_nav.run_button_menus", [] {
    fs::ScopedTempDir tmp("cbnav");
    copy_into(data("in"), tmp.path());
    const Result r = run(tmp.path());
    CB_CHECK(r.guiscreen_h && r.guiscreen_cpp && r.callsite && r.focusmode && r.mousehelper);
    CB_CHECK(!r.pointer);                       // no GuiContainer.h: no free cursor, no Pointer.png draw
    CB_CHECK(r.missing.empty());
    expect_same_files(tmp.path(), data("expected"));
});

CB_TEST("java.minecraft.controller_nav.run_container_edition", [] {
    fs::ScopedTempDir tmp("cbnav");
    copy_into(data("in"), tmp.path());
    copy_into(data("in_container"), tmp.path());
    const Result r = run(tmp.path());
    CB_CHECK(r.guiscreen_h && r.guiscreen_cpp && r.callsite && r.focusmode && r.mousehelper && r.pointer);
    CB_CHECK(r.missing.empty());
    expect_same_files(tmp.path(), data("expected_container"));
});

CB_TEST("java.minecraft.controller_nav.run_is_idempotent", [] {
    fs::ScopedTempDir tmp("cbnav");
    copy_into(data("in"), tmp.path());
    copy_into(data("in_container"), tmp.path());
    (void)run(tmp.path());
    const Result again = run(tmp.path());           // each edit finds its own marker and counts as applied
    CB_CHECK(again.guiscreen_h && again.guiscreen_cpp && again.callsite && again.focusmode && again.mousehelper &&
             again.pointer);
    CB_CHECK(again.missing.empty());
    expect_same_files(tmp.path(), data("expected_container"));
});

CB_TEST("java.minecraft.controller_nav.run_reports_missing_files", [] {
    fs::ScopedTempDir tmp("cbnav");
    const Result r = run(tmp.path());
    CB_CHECK(!r.guiscreen_h && !r.guiscreen_cpp && !r.callsite && !r.focusmode && !r.mousehelper && !r.pointer);
    CB_CHECK((r.missing) == ((Names{"GuiScreen.h (field+decl: file absent)", "GuiScreen.cpp (ctor+body: file absent)",
                       "Minecraft.cpp (per-frame call: file absent)",
                       "menu-nav glue NOT applied to any file (all-or-nothing: GuiScreen.h/GuiScreen.cpp/Minecraft.cpp must all match)",
                       "Minecraft.cpp (grab/cursor mode: file absent)",
                       "MouseHelper.cpp (console-safe grab/ungrab: file absent)"})));
});

CB_TEST("java.minecraft.controller_nav.glue_is_all_or_nothing", [] {
    // GuiScreen.cpp's constructor has no member-init list: the adapter cannot be wired, so NONE of the three
    // files of the glue is touched (a declared-but-undefined method would not link)
    fs::ScopedTempDir tmp("cbnav");
    copy_into(data("in"), tmp.path());
    fs::write_text(tmp.path() / "GuiScreen.cpp", "#include \"stdafx.h\"\nGuiScreen::GuiScreen()\n{\n}\n");
    const Result r = run(tmp.path());
    CB_CHECK(!r.guiscreen_h && !r.guiscreen_cpp && !r.callsite);
    CB_CHECK(r.focusmode && r.mousehelper);        // the input-mode edits are independent of the glue
    CB_CHECK((r.missing) == ((Names{"GuiScreen.cpp (ctor+body: anchor not found)",
                       "menu-nav glue NOT applied to any file (all-or-nothing: GuiScreen.h/GuiScreen.cpp/Minecraft.cpp must all match)"})));
    CB_CHECK_EQ(fs::read_text(tmp.path() / "GuiScreen.h"), fs::read_text(data("in") / "GuiScreen.h"));
    CB_CHECK_EQ(fs::read_text(tmp.path() / "GuiScreen.cpp"), "#include \"stdafx.h\"\nGuiScreen::GuiScreen()\n{\n}\n");
    CB_CHECK(fs::read_text(tmp.path() / "Minecraft.cpp").find("handleControllerMenuNav") == std::string::npos);
    CB_CHECK(fs::read_text(tmp.path() / "Minecraft.cpp").find("CONSOLE: pad grabbed") != std::string::npos);
});

CB_TEST("java.minecraft.controller_nav.patch_guiscreen_h", [] {
    const std::string in = "class GuiScreen {\n    JList<GuiButton*> controlList;\n    int x;\n};\n";
    const Patched p = patch_guiscreen_h(in);
    CB_CHECK(p.ok);
    CB_CHECK_EQ(p.text,
                "class GuiScreen {\n    JList<GuiButton*> controlList;\n"
                "    // CONSOLE (controller menu nav): D-pad-selected button index (-1 = none yet). CodeBreaker-injected.\n"
                "    int navSelected;\n    void handleControllerMenuNav();\n    int x;\n};\n");
    const Patched again = patch_guiscreen_h(p.text);
    CB_CHECK(again.ok && again.text == p.text);
    const Patched none = patch_guiscreen_h("class GuiScreen { int x; };\n");   // no controlList: nothing to anchor on
    CB_CHECK(!none.ok);
    CB_CHECK_EQ(none.text, "class GuiScreen { int x; };\n");
});

CB_TEST("java.minecraft.controller_nav.patch_guiscreen_cpp", [] {
    // includes already present are not repeated; the body is appended after the last line
    const std::string in = "#include \"a.h\"\n#include \"Input.h\"\nGuiScreen::GuiScreen() : x(0), y(1)\n{\n}\n\n\n";
    const Patched p = patch_guiscreen_cpp(in);
    CB_CHECK(p.ok);
    CB_CHECK(p.text.starts_with("#include \"a.h\"\n#include \"GuiButton.h\"\n#include \"Input.h\"\n"
                                "GuiScreen::GuiScreen() : x(0), y(1)\n\t, navSelected(-1)\n{\n}\n\n// CONSOLE (controller menu nav)"));
    CB_CHECK(p.text.ends_with("this->mc->displayWidth, this->mc->displayHeight);\n\t}\n}\n"));
    CB_CHECK(!patch_guiscreen_cpp("GuiScreen::GuiScreen() {}\n").ok);
    CB_CHECK(!patch_guiscreen_cpp("").ok);
});

CB_TEST("java.minecraft.controller_nav.patch_minecraft_callsite", [] {
    const std::string in = "void f() {\n\tif (c) {\n\t\tthis->mc->currentScreen->updateScreen();\n\t}\n}\n";
    const Patched p = patch_minecraft_callsite(in);
    CB_CHECK(p.ok);
    CB_CHECK_EQ(p.text,
                "void f() {\n\tif (c) {\n\t\tthis->mc->currentScreen->handleControllerMenuNav();\n"
                "\t\tthis->mc->currentScreen->updateScreen();\n\t}\n}\n");
    CB_CHECK(patch_minecraft_callsite(p.text).ok);
    CB_CHECK(!patch_minecraft_callsite("void f() { other->update(); }\n").ok);
    // only the FIRST tick call is wrapped
    const Patched two = patch_minecraft_callsite("\na->updateScreen();\n\nb->updateScreen();\n");
    CB_CHECK_EQ(two.text, "\na->handleControllerMenuNav();\na->updateScreen();\n\nb->updateScreen();\n");
});

CB_TEST("java.minecraft.controller_nav.patch_displayguiscreen", [] {
    const std::string in = "void M::d(GuiScreen* v) {\n\tthis->currentScreen = (GuiScreen *)v;\n\tother();\n}\n";
    const Patched menus = patch_minecraft_displayguiscreen(in, false);
    CB_CHECK(menus.ok);
    CB_CHECK(menus.text.starts_with("void M::d(GuiScreen* v) {\n\tthis->currentScreen = (GuiScreen *)v;\n\tMouse::setGrabbed(v == NULL && this->theWorld != NULL);   // CONSOLE: pad grabbed"));
    CB_CHECK(menus.text.find("g_guiCursorMode") == std::string::npos);
    const Patched cont = patch_minecraft_displayguiscreen(in, true);
    CB_CHECK(cont.ok);
    CB_CHECK(cont.text.find("\n\t{ extern bool g_guiCursorMode; g_guiCursorMode = (v != NULL && dynamic_cast<GuiContainer*>(v) != NULL); }"
                            "   // CONSOLE: analog cursor in container GUIs (CodeBreaker-injected)\n\tMouse::setGrabbed(v == NULL") !=
             std::string::npos);
    CB_CHECK(patch_minecraft_displayguiscreen(cont.text, true).ok);                 // its own marker
    CB_CHECK_EQ(patch_minecraft_displayguiscreen(cont.text, false).text, cont.text);
    CB_CHECK(!patch_minecraft_displayguiscreen("void f() {}\n", true).ok);
});

CB_TEST("java.minecraft.controller_nav.patch_pointer_namespace", [] {
    const std::string tail = "\n\tthis->mc->currentScreen->drawScreen(mx, my, a);\n\td3d9::glPopMatrix();\n";
    const Patched d3d = patch_entityrenderer_pointer("void f() {" + tail + "}\n");
    CB_CHECK(d3d.ok);
    CB_CHECK(d3d.text.find("\td3d9::glBindTexture(d3d9::GL_TEXTURE_2D,") != std::string::npos);
    CB_CHECK(d3d.text.find("GL11::") == std::string::npos && d3d.text.find("@NS@") == std::string::npos);
    // no d3d9 reference anywhere: the GL11 holder is used
    const Patched gl = patch_entityrenderer_pointer("void f() {\n\tthis->mc->currentScreen->drawScreen(mx, my, a);\n}\n");
    CB_CHECK(gl.ok);
    CB_CHECK(gl.text.find("\tGL11::glBindTexture(GL11::GL_TEXTURE_2D,") != std::string::npos);
    CB_CHECK(patch_entityrenderer_pointer(gl.text).ok);
    CB_CHECK(!patch_entityrenderer_pointer("void f() { draw(); }\n").ok);
});

CB_TEST("java.minecraft.controller_nav.patch_mousehelper", [] {
    const std::string in = "void MouseHelper::grabMouseCursor()\n{\n\tif (x) {\n\t\ty();\n\t}\n}\n\n"
                           "void MouseHelper::ungrabMouseCursor() {\n\tz();\n}\n";
    const Patched p = patch_mousehelper_grab(in);
    CB_CHECK(p.ok);
    CB_CHECK_EQ(p.text,
                "void MouseHelper::grabMouseCursor()\n{\n\tMouse::setGrabbed(true);   // CONSOLE grab (CodeBreaker): set the pad mode flag only -- NO AWT\n"
                "\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}\n\n"
                "void MouseHelper::ungrabMouseCursor()\n{\n\tMouse::setGrabbed(false);   // CONSOLE ungrab (CodeBreaker): set the pad mode flag only -- NO AWT\n"
                "\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}\n");
    CB_CHECK(patch_mousehelper_grab(p.text).ok);
    // trees converted under the tool's old name are recognised too
    CB_CHECK(patch_mousehelper_grab("// CONSOLE grab (j2ctool)\n").ok);
    // both functions or neither: one alone is a structural drift, and the text is returned as it was
    const std::string only_grab = "void MouseHelper::grabMouseCursor()\n{\n}\n";
    const Patched half = patch_mousehelper_grab(only_grab);
    CB_CHECK(!half.ok);
    CB_CHECK_EQ(half.text, only_grab);
    CB_CHECK(!patch_mousehelper_grab("").ok);
});

}  // namespace
