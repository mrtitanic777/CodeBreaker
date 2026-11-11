// DIVERGES (checked against the Python on its 85 recorded cases: 0 unexplained differences):
//   * names are walked ordered by bytes, files before folders (the Python saw the Windows listing order); the catalogue is
//     sorted, so nothing visible changes.
//   * `run` on a source folder that does not exist is a NotFoundError (the Python's FileNotFoundError came from writing the
//     index); an explicit `index_dir` that does not exist is created.
//   * files are written back with LF endings (the Python's text mode wrote CRLF on Windows).

#include <filesystem>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/ui_mark.hpp"

namespace {

namespace fs = cb::fs;
namespace testing = cb::testing;
using namespace cb::java::projects::minecraft::ui_mark;
namespace stdfs = std::filesystem;
using Names = std::set<std::string>;

// A flat converted-tree fixture: headers that declare the classes, sources that define their methods.
const char* kGuiScreenCpp =
    "#include \"GuiScreen.h\"\n"                                   // 1
    "void GuiScreen::drawScreen(int x, int y, float a)\n"           // 2  definition
    "{\n"                                                           // 3
    "\tthis->drawBackground();\n"                                   // 4
    "\tGuiScreen::drawRect(0, 0, 1, 1);\n"                          // 5  a call: ends with ';'
    "}\n"                                                           // 6
    "GuiScreen::~GuiScreen()\n"                                     // 7  destructor definition
    "{\n"                                                           // 8
    "}\n"                                                           // 9
    "int GuiScreen::proto(int a);\n"                                // 10 prototype
    "void Plain::update() { }\n"                                    // 11 not a UI class
    "void GuiScreen::keyTyped(char c, int k) {   // J2X-UI[GuiScreen::keyTyped]\n"   // 12 already marked
    "\tfoo(GuiScreen::bar(1),\n"                                    // 13 a call continued on the next line
    "\t\t2);\n"                                                     // 14
    "}\n";                                                          // 15

const char* kGuiMainMenuCpp =
    "void GuiMainMenu::initGui(\n"
    "\tint a)\n"
    "{\n"
    "\tGuiMainMenu::helper();\n"
    "}\n";

const char* kWidgetCpp =
    "void Widget::a() { GuiScreen::b(); }\n"
    "void Widget::c()\n"
    "{\n"
    "}\n";

stdfs::path make_tree(const fs::ScopedTempDir& tmp) {
    const stdfs::path src = tmp.path() / "src";
    fs::write_text(src / "GuiScreen.h", "class GuiScreen : public Gui {\npublic:\n\tvirtual void drawScreen(int x, int y, float a);\n};\n");
    fs::write_text(src / "GuiMainMenu.h", "class GuiMainMenu : public GuiScreen\n{\n};\n");
    fs::write_text(src / "MyOptions.h", "class MyOptions: public GuiMainMenu {};\n");
    fs::write_text(src / "Widget.h", "class Widget : public GuiButton, private Helper {};\n");
    fs::write_text(src / "Overlay.h", "class Overlay : Base<Gui> {};\n");
    fs::write_text(src / "Plain.h", "class Fwd;\nclass Entity {};\nclass Plain : public Entity {};\n");
    fs::write_text(src / "LoadingScreen.h", "class LoadingScreen {};\n");
    fs::write_text(src / "GuiInline.h", "class GuiInline { int x; };\n");
    fs::write_text(src / "GuiScreen.cpp", kGuiScreenCpp);
    fs::write_text(src / "GuiMainMenu.cpp", kGuiMainMenuCpp);
    fs::write_text(src / "Widget.cpp", kWidgetCpp);
    return src;
}

CB_TEST("java.minecraft.ui_mark.class_names", [] {
    CB_CHECK(is_ui_class_name("GuiScreen"));
    CB_CHECK(is_ui_class_name("GuiButton"));
    CB_CHECK(is_ui_class_name("LoadingScreen"));
    CB_CHECK(is_ui_class_name("PauseMenu"));
    CB_CHECK(is_ui_class_name("Gui"));
    CB_CHECK(!is_ui_class_name("Entity"));
    CB_CHECK(!is_ui_class_name("MyGui"));         // Gui must start the name
    CB_CHECK(!is_ui_class_name("Screens"));
    CB_CHECK(!is_ui_class_name(""));
});

CB_TEST("java.minecraft.ui_mark.discover", [] {
    fs::ScopedTempDir tmp("cbuimark");
    const stdfs::path src = make_tree(tmp);
    // by name (GuiScreen, GuiInline, LoadingScreen), by a UI base (Widget: GuiButton), through a template argument
    // (Overlay: Base<Gui>), transitively (GuiMainMenu: GuiScreen, MyOptions: GuiMainMenu)
    CB_CHECK((discover_ui_classes(src)) == ((Names{"GuiInline", "GuiMainMenu", "GuiScreen", "LoadingScreen", "MyOptions", "Overlay", "Widget"})));
    // classes the caller knows are UI even when no header declares them
    CB_CHECK(discover_ui_classes(src, {"CustomPanel"}).contains("CustomPanel"));
    CB_CHECK(discover_ui_classes(tmp.path() / "nowhere").empty());
    // a class named with a letter beyond ASCII is one name
    fs::write_text(tmp.path() / "u.h", "class Gui\xC3\xA9tude : public Gui {};\nclass Sub\xC3\xA9 : public Gui\xC3\xA9tude {};\n");
    const auto with_u = discover_ui_classes(tmp.path());
    CB_CHECK(with_u.contains("Gui\xC3\xA9tude") && with_u.contains("Sub\xC3\xA9"));
});

CB_TEST("java.minecraft.ui_mark.discover_cycles_and_long_chains", [] {
    fs::ScopedTempDir tmp("cbuimark");
    fs::write_text(tmp.path() / "a.h",
                   "class A : public B {};\nclass B : public C {};\nclass C : public D {};\nclass D : public GuiSlot {};\n"
                   "class X : public Y {};\nclass Y : public X {};\n");          // a cycle that never reaches a UI base
    CB_CHECK((discover_ui_classes(tmp.path())) == ((Names{"A", "B", "C", "D"})));
});

CB_TEST("java.minecraft.ui_mark.dry_run", [] {
    fs::ScopedTempDir tmp("cbuimark");
    const stdfs::path src = make_tree(tmp);
    const Result r = run(src, true);
    CB_CHECK(!r.index);
    CB_CHECK(!stdfs::exists(src / "UI_INDEX.md"));
    CB_CHECK_EQ(fs::read_text(src / "GuiScreen.cpp"), kGuiScreenCpp);
    CB_CHECK_EQ(fs::read_text(src / "GuiMainMenu.cpp"), kGuiMainMenuCpp);
    CB_CHECK_EQ(fs::read_text(src / "Widget.cpp"), kWidgetCpp);
    // it still reports what it would mark
    CB_CHECK_EQ(r.marked.at("GuiScreen").size(), std::size_t{4});
    CB_CHECK_EQ(r.skipped, std::size_t{1});
});

CB_TEST("java.minecraft.ui_mark.mark_and_index", [] {
    fs::ScopedTempDir tmp("cbuimark");
    const stdfs::path src = make_tree(tmp);
    const Result r = run(src);

    CB_CHECK_EQ(r.classes.size(), std::size_t{7});
    CB_CHECK_EQ(r.skipped, std::size_t{1});
    CB_CHECK_EQ(r.marked.size(), std::size_t{3});
    const std::vector<Mark> screen = {{"GuiScreen.cpp", 2, "drawScreen"}, {"GuiScreen.cpp", 7, "~GuiScreen"},
                                      {"GuiScreen.cpp", 12, "keyTyped"}, {"Widget.cpp", 1, "b"}};
    CB_CHECK(r.marked.at("GuiScreen") == screen);   // (a line naming two UI classes goes to the first by name: Widget.cpp:1)
    CB_CHECK(r.marked.at("GuiMainMenu") == std::vector<Mark>{{"GuiMainMenu.cpp", 1, "initGui"}});
    CB_CHECK(r.marked.at("Widget") == std::vector<Mark>{{"Widget.cpp", 2, "c"}});

    CB_CHECK_EQ(fs::read_text(src / "GuiScreen.cpp"),
                "#include \"GuiScreen.h\"\n"
                "void GuiScreen::drawScreen(int x, int y, float a)  // J2X-UI[GuiScreen::drawScreen]\n"
                "{\n"
                "\tthis->drawBackground();\n"
                "\tGuiScreen::drawRect(0, 0, 1, 1);\n"
                "}\n"
                "GuiScreen::~GuiScreen()  // J2X-UI[GuiScreen::~GuiScreen]\n"
                "{\n"
                "}\n"
                "int GuiScreen::proto(int a);\n"
                "void Plain::update() { }\n"
                "void GuiScreen::keyTyped(char c, int k) {   // J2X-UI[GuiScreen::keyTyped]\n"
                "\tfoo(GuiScreen::bar(1),\n"
                "\t\t2);\n"
                "}\n");
    CB_CHECK_EQ(fs::read_text(src / "GuiMainMenu.cpp"),
                "void GuiMainMenu::initGui(  // J2X-UI[GuiMainMenu::initGui]\n"
                "\tint a)\n"
                "{\n"
                "\tGuiMainMenu::helper();\n"
                "}\n");
    CB_CHECK_EQ(fs::read_text(src / "Widget.cpp"),
                "void Widget::a() { GuiScreen::b(); }  // J2X-UI[GuiScreen::b]\n"
                "void Widget::c()  // J2X-UI[Widget::c]\n"
                "{\n"
                "}\n");

    CB_CHECK(r.index && fs::same_path(*r.index, src / "UI_INDEX.md"));
    CB_CHECK_EQ(fs::read_text(src / "UI_INDEX.md"),
                "# UI_INDEX.md -- catalogue of every UI function\n"
                "\n"
                "Every UI method carries a greppable `// J2X-UI[Class::method]` marker; `grep -rn \"J2X-UI\" .` lists them all. "
                "Regenerate after each UI merge.\n"
                "\n"
                "- UI classes: **7**   UI methods marked: **6**\n"
                "\n"
                "## GuiMainMenu (1)\n"
                "\n"
                "| Method | Location |\n"
                "|---|---|\n"
                "| `GuiMainMenu::initGui` | `GuiMainMenu.cpp:1` |\n"
                "\n"
                "## GuiScreen (4)\n"
                "\n"
                "| Method | Location |\n"
                "|---|---|\n"
                "| `GuiScreen::drawScreen` | `GuiScreen.cpp:2` |\n"
                "| `GuiScreen::~GuiScreen` | `GuiScreen.cpp:7` |\n"
                "| `GuiScreen::keyTyped` | `GuiScreen.cpp:12` |\n"
                "| `GuiScreen::b` | `Widget.cpp:1` |\n"
                "\n"
                "## Widget (1)\n"
                "\n"
                "| Method | Location |\n"
                "|---|---|\n"
                "| `Widget::c` | `Widget.cpp:2` |\n"
                "\n"
                "## UI classes with no marked methods (header-only or all inline)\n"
                "\n"
                "`GuiInline`, `LoadingScreen`, `MyOptions`, `Overlay`\n"
                "\n");
});

CB_TEST("java.minecraft.ui_mark.idempotent", [] {
    fs::ScopedTempDir tmp("cbuimark");
    const stdfs::path src = make_tree(tmp);
    (void)run(src);
    const std::string a = fs::read_text(src / "GuiScreen.cpp");
    const std::string b = fs::read_text(src / "GuiMainMenu.cpp");
    const std::string c = fs::read_text(src / "Widget.cpp");
    const Result again = run(src, false, {}, tmp.path());          // the catalogue goes to another folder
    CB_CHECK_EQ(again.skipped, std::size_t{6});                    // every definition already carries its marker
    CB_CHECK_EQ(fs::read_text(src / "GuiScreen.cpp"), a);
    CB_CHECK_EQ(fs::read_text(src / "GuiMainMenu.cpp"), b);
    CB_CHECK_EQ(fs::read_text(src / "Widget.cpp"), c);
    CB_CHECK(stdfs::exists(tmp.path() / "UI_INDEX.md"));
});

CB_TEST("java.minecraft.ui_mark.extra_classes_and_empty_index", [] {
    fs::ScopedTempDir tmp("cbuimark");
    fs::write_text(tmp.path() / "Panel.cpp", "void CustomPanel::paint() {\n}\nvoid Other::paint() {\n}\n");
    const Result r = run(tmp.path(), false, {"CustomPanel"});
    CB_CHECK_EQ(r.marked.size(), std::size_t{1});
    CB_CHECK_EQ(fs::read_text(tmp.path() / "Panel.cpp"),
                "void CustomPanel::paint() {  // J2X-UI[CustomPanel::paint]\n}\nvoid Other::paint() {\n}\n");

    // the index needs a place: a source folder that does not exist has none, an explicit folder is made
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)run(tmp.path() / "nowhere"));
    const Result made = run(tmp.path() / "nowhere", false, {}, tmp.path() / "made" / "here");
    CB_CHECK(made.index && stdfs::exists(tmp.path() / "made" / "here" / "UI_INDEX.md"));

    // nothing at all: an index with zero counts and no sections
    fs::ScopedTempDir empty("cbuimark");
    const Result none = mark(empty.path());
    CB_CHECK(none.classes.empty() && none.marked.empty());
    CB_CHECK_EQ(render_index(none),
                "# UI_INDEX.md -- catalogue of every UI function\n\n"
                "Every UI method carries a greppable `// J2X-UI[Class::method]` marker; `grep -rn \"J2X-UI\" .` lists them all. "
                "Regenerate after each UI merge.\n\n"
                "- UI classes: **0**   UI methods marked: **0**\n\n");
});

}  // namespace
