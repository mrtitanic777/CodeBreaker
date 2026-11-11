// DIVERGES (checked against the Python on its 81 recorded cases -- CR, CRLF, bad UTF-8, non-ASCII keys, files named like
// folders, a source folder that is a file: 0 unexplained differences):
//   * the tree is walked with names ordered by bytes (upper case first), files before folders; the Windows listing the Python
//     saw ordered them by letters. Only the order of `sites` across folders differs; the index sorts by file and line.
//   * `run` on a source folder that does not exist is a NotFoundError (the Python's FileNotFoundError came from writing the
//     index); an explicit `index_dir` that does not exist is created instead of failing after the files were already marked.
//   * files are written back with LF endings (the Python's text mode wrote CRLF on Windows).

#include <filesystem>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/keybind_mark.hpp"

namespace {

namespace fs = cb::fs;
namespace testing = cb::testing;
using namespace cb::java::projects::minecraft::keybind_mark;
namespace stdfs = std::filesystem;

const char* kMinecraft =
    "void Minecraft::runTick() {\n"
    "\tif (Keyboard::getEventKey() == Keyboard::KEY_F1) { toggleHud(); }   \n"
    "\tif (Keyboard::isKeyDown(Keyboard::KEY_F3) && Keyboard::isKeyDown(Keyboard::KEY_S)) { reload(); }\n"
    "\tif (Keyboard::getEventKey() == Keyboard::KEY_F12) { }\n"
    "\tif (Keyboard::getEventKey()==Keyboard::KEY_1) { slot(0); } // J2X-KEYBIND[KEY_1=hotbar slot 1 | already]\n"
    "\tif (Keyboard::getEventKey() == KEY_F2) { }\n"       // not a Keyboard:: site
    "}\n";

const char* kGui =
    "void GuiScreen::keyTyped(char c, int k) {\n"
    "\tif (k == Keyboard::KEY_ESCAPE) { }\n"                                     // a bare comparison: not a site
    "\tif (Keyboard::isKeyDown( Keyboard::KEY_F5 )) { thirdPerson(); }\n"
    "}\n";

constexpr const char* kTail = " | keyboard is DEAD on console -- ROUTE TO: controller button OR menu toggle]";

stdfs::path make_tree(const fs::ScopedTempDir& tmp) {
    const stdfs::path src = tmp.path() / "src";
    fs::write_text(src / "Minecraft.cpp", kMinecraft);
    fs::write_text(src / "gui" / "GuiScreen.cpp", kGui);
    fs::write_text(src / "Minecraft.h", "if (Keyboard::isKeyDown(Keyboard::KEY_F8)) { }\n");   // headers are not scanned
    return src;
}

CB_TEST("java.minecraft.keybind_mark.actions", [] {
    CB_CHECK_EQ(action_of("F1"), "hide GUI / HUD");
    CB_CHECK_EQ(action_of("F11"), "toggle fullscreen");
    CB_CHECK_EQ(action_of("S"), "reload chunks (with F3 held)");
    CB_CHECK_EQ(action_of("RCONTROL"), "sprint");
    CB_CHECK_EQ(action_of("9"), "hotbar slot 9");
    CB_CHECK_EQ(action_of("F12"), "(unknown action -- inspect the branch)");
    CB_CHECK_EQ(action_of(""), "(unknown action -- inspect the branch)");
    CB_CHECK_EQ(action_of("f1"), "(unknown action -- inspect the branch)");       // keys are upper-case as Keyboard names them
});

CB_TEST("java.minecraft.keybind_mark.keys_on_line", [] {
    using V = std::vector<std::string>;
    CB_CHECK((keys_on_line("if (Keyboard::getEventKey() == Keyboard::KEY_F1) {}")) == (V{"F1"}));
    CB_CHECK((keys_on_line("Keyboard::isKeyDown(Keyboard::KEY_F3) && Keyboard::isKeyDown(Keyboard::KEY_S)")) == ((V{"F3", "S"})));
    CB_CHECK((keys_on_line("Keyboard::getEventKey()==Keyboard::KEY_LSHIFT")) == (V{"LSHIFT"}));
    CB_CHECK((keys_on_line("Keyboard::isKeyDown ( Keyboard::KEY_1 )")) == (V{"1"}));
    CB_CHECK(keys_on_line("Keyboard::getEventKey() != Keyboard::KEY_F1").empty());        // only `==` and isKeyDown
    CB_CHECK(keys_on_line("x == Keyboard::KEY_F1").empty());
    CB_CHECK(keys_on_line("").empty());
    // a key named with a letter beyond ASCII is one name (as the Python's \w read it)
    CB_CHECK(keys_on_line("Keyboard::isKeyDown(Keyboard::KEY_\xC3\xA9t\xC3\xA9)") == V{"\xC3\xA9t\xC3\xA9"});
    CB_CHECK(keys_on_line("Keyboard::isKeyDown(Keyboard::KEY_)").empty());
});

CB_TEST("java.minecraft.keybind_mark.dry_run_reports_without_touching", [] {
    fs::ScopedTempDir tmp("cbkeybind");
    const stdfs::path src = make_tree(tmp);
    const Result r = mark(src, true);
    CB_CHECK_EQ(r.sites.size(), std::size_t{5});
    CB_CHECK_EQ(r.skipped, std::size_t{1});
    CB_CHECK_EQ(fs::read_text(src / "Minecraft.cpp"), kMinecraft);
    CB_CHECK_EQ(fs::read_text(src / "gui" / "GuiScreen.cpp"), kGui);
    const Result d = run(src, true);
    CB_CHECK(!d.index);
    CB_CHECK(!stdfs::exists(src / "KEYBIND_INDEX.md"));
});

CB_TEST("java.minecraft.keybind_mark.mark_and_index", [] {
    fs::ScopedTempDir tmp("cbkeybind");
    const stdfs::path src = make_tree(tmp);
    const Result r = run(src);

    // sites: this folder's files first (by name), then the sub-folder's
    CB_CHECK_EQ(r.sites.size(), std::size_t{5});
    CB_CHECK_EQ(r.skipped, std::size_t{1});
    CB_CHECK_EQ(r.sites[0].file, "Minecraft.cpp");
    CB_CHECK_EQ(r.sites[0].line, std::size_t{2});
    CB_CHECK_EQ(r.sites[0].label, "KEY_F1=hide GUI / HUD");
    CB_CHECK_EQ(r.sites[1].line, std::size_t{3});
    CB_CHECK_EQ(r.sites[1].label, "KEY_F3=toggle debug overlay (F3 panel) KEY_S=reload chunks (with F3 held)");
    CB_CHECK_EQ(r.sites[2].label, "KEY_F12=(unknown action -- inspect the branch)");
    CB_CHECK_EQ(r.sites[4].file, "GuiScreen.cpp");
    CB_CHECK_EQ(r.sites[4].line, std::size_t{3});

    // the marked source: the trailing blanks go, the marker is appended; the already-marked line, the non-site
    // lines and the header are as they were
    const std::string want_mc =
        "void Minecraft::runTick() {\n"
        "\tif (Keyboard::getEventKey() == Keyboard::KEY_F1) { toggleHud(); }   // J2X-KEYBIND[KEY_F1=hide GUI / HUD" + std::string(kTail) + "\n"
        "\tif (Keyboard::isKeyDown(Keyboard::KEY_F3) && Keyboard::isKeyDown(Keyboard::KEY_S)) { reload(); }   // J2X-KEYBIND["
        "KEY_F3=toggle debug overlay (F3 panel) KEY_S=reload chunks (with F3 held)" + std::string(kTail) + "\n"
        "\tif (Keyboard::getEventKey() == Keyboard::KEY_F12) { }   // J2X-KEYBIND[KEY_F12=(unknown action -- inspect the branch)" + std::string(kTail) + "\n"
        "\tif (Keyboard::getEventKey()==Keyboard::KEY_1) { slot(0); } // J2X-KEYBIND[KEY_1=hotbar slot 1 | already]\n"
        "\tif (Keyboard::getEventKey() == KEY_F2) { }\n"
        "}\n";
    CB_CHECK_EQ(fs::read_text(src / "Minecraft.cpp"), want_mc);
    CB_CHECK_EQ(fs::read_text(src / "gui" / "GuiScreen.cpp"),
                std::string("void GuiScreen::keyTyped(char c, int k) {\n"
                            "\tif (k == Keyboard::KEY_ESCAPE) { }\n"
                            "\tif (Keyboard::isKeyDown( Keyboard::KEY_F5 )) { thirdPerson(); }   // J2X-KEYBIND[KEY_F5=toggle third-person view") +
                    kTail + "\n}\n");
    CB_CHECK_EQ(fs::read_text(src / "Minecraft.h"), "if (Keyboard::isKeyDown(Keyboard::KEY_F8)) { }\n");

    // the index: rows by file name then line; five sites over six distinct keys
    CB_CHECK(r.index && fs::same_path(*r.index, src / "KEYBIND_INDEX.md"));
    CB_CHECK_EQ(fs::read_text(src / "KEYBIND_INDEX.md"),
                "# KEYBIND_INDEX.md -- every hardcoded keyboard action (route to controller OR menu)\n"
                "\n"
                "Minecraft binds these actions to physical keys. The console has NO keyboard, so each site is DEAD until re-routed. "
                "Every site carries a greppable `// J2X-KEYBIND[...]` marker (`grep -rn \"J2X-KEYBIND\" .`). Decide, per key, whether "
                "it becomes a **controller button** or a **menu toggle** (a GameSettings option, like Show FPS -> the debug panel), "
                "then wire it and clear the marker.\n"
                "\n"
                "- key-press sites: **5**   distinct keys: **6**\n"
                "\n"
                "| Key | Action | Site | Route decision |\n"
                "|---|---|---|---|\n"
                "| `KEY_F5` | toggle third-person view | `GuiScreen.cpp:3` | _controller button / menu toggle_ |\n"
                "| `KEY_F1` | hide GUI / HUD | `Minecraft.cpp:2` | _controller button / menu toggle_ |\n"
                "| `KEY_F3, KEY_S` | toggle debug overlay (F3 panel) + reload chunks (with F3 held) | `Minecraft.cpp:3` | _controller button / menu toggle_ |\n"
                "| `KEY_F12` | (unknown action -- inspect the branch) | `Minecraft.cpp:4` | _controller button / menu toggle_ |\n"
                "| `KEY_1` | hotbar slot 1 | `Minecraft.cpp:5` | _controller button / menu toggle_ |\n"
                "\n");
});

CB_TEST("java.minecraft.keybind_mark.idempotent", [] {
    fs::ScopedTempDir tmp("cbkeybind");
    const stdfs::path src = make_tree(tmp);
    (void)run(src);
    const std::string once_mc = fs::read_text(src / "Minecraft.cpp");
    const std::string once_gui = fs::read_text(src / "gui" / "GuiScreen.cpp");
    const Result again = run(src, false, tmp.path());          // the index goes to another folder this time
    CB_CHECK_EQ(again.sites.size(), std::size_t{5});
    CB_CHECK_EQ(again.skipped, std::size_t{5});                // every site carries its marker: nothing is doubled
    CB_CHECK_EQ(fs::read_text(src / "Minecraft.cpp"), once_mc);
    CB_CHECK_EQ(fs::read_text(src / "gui" / "GuiScreen.cpp"), once_gui);
    CB_CHECK(stdfs::exists(tmp.path() / "KEYBIND_INDEX.md"));
});

CB_TEST("java.minecraft.keybind_mark.empty_or_missing_tree", [] {
    fs::ScopedTempDir tmp("cbkeybind");
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)run(tmp.path() / "nowhere"));          // nowhere to write the index
    const Result elsewhere = run(tmp.path() / "nowhere", false, tmp.path() / "made" / "here");   // an explicit folder is made
    CB_CHECK(elsewhere.index && stdfs::exists(tmp.path() / "made" / "here" / "KEYBIND_INDEX.md"));
    const Result none = mark(tmp.path() / "nowhere");
    CB_CHECK(none.sites.empty() && none.skipped == 0);
    const std::string index = render_index(none);
    CB_CHECK(index.find("- key-press sites: **0**   distinct keys: **0**") != std::string::npos);
    CB_CHECK(index.ends_with("|---|---|---|---|\n\n"));
});

}  // namespace
