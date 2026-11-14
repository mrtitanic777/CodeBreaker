// Console lane checks: the project layout (the template's well-formedness, an idempotent re-run, an already structured folder, a
// flat dump's root) and the runnable entry (the entry point constructs and runs with default arguments: the entry's start-up calls).
#include <algorithm>
#include <vector>

#include "cb/java/console/platform_refs.hpp"
#include "cb/java/console/project_layout.hpp"
#include "cb/java/console/test_support.hpp"
#include "cb/java/parse.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

const vcxproj::FilterRules& cpt_rules() {
    static const vcxproj::FilterRules r({{"^(stdafx|Common)$", "Runtime"}});
    return r;
}

stdfs::path cpt_template() { return testing::data_path("../../data/java/console/templates/xbox360.vcxproj.in"); }

void cpt_flat_dump(const stdfs::path& root) {
    for (const char* n : {"Foo.cpp", "Bar.cpp", "stdafx.cpp", "Foo.h", "Bar.h", "stdafx.h"}) fs::write_text(root / n, std::string("// ") + n);
    fs::write_text(root / "Content" / "terrain.png", "png");
}

std::vector<std::string> cpt_listing(const stdfs::path& dir) {
    std::vector<std::string> names;
    for (const auto& e : stdfs::directory_iterator(dir)) names.push_back(fs::to_text(e.path().filename()));
    std::ranges::sort(names);
    return names;
}

bool cpt_has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

// A template with an XML comment holding "--" is not loadable by Visual Studio; a cheap structural check of the filled template: every comment is
// closed and none holds "--", and every opening tag is closed.
void cpt_well_formed(const std::string& xml) {
    std::size_t at = 0;
    while ((at = xml.find("<!--", at)) != std::string::npos) {
        const std::size_t end = xml.find("-->", at + 4);
        CB_CHECK(end != std::string::npos);
        if (end == std::string::npos) return;
        CB_CHECK(xml.substr(at + 4, end - at - 4).find("--") == std::string::npos);
        at = end + 3;
    }
    std::vector<std::string> open;
    std::size_t i = 0;
    while ((i = xml.find('<', i)) != std::string::npos) {
        if (xml.compare(i, 4, "<!--") == 0) { i = xml.find("-->", i) + 3; continue; }
        if (xml.compare(i, 2, "<?") == 0) { i = xml.find("?>", i) + 2; continue; }
        const std::size_t close = xml.find('>', i);
        CB_CHECK(close != std::string::npos);
        if (close == std::string::npos) return;
        const std::string tag = xml.substr(i + 1, close - i - 1);
        if (!tag.empty() && tag[0] == '/') {
            CB_CHECK(!open.empty() && open.back() == tag.substr(1));
            if (!open.empty()) open.pop_back();
        } else if (!tag.empty() && tag.back() != '/') {
            open.push_back(tag.substr(0, tag.find_first_of(" \t\r\n")));
        }
        i = close + 1;
    }
    CB_CHECK(open.empty());
}

}  // namespace

CB_TEST("java.console.ports.the_template_filled_in_is_well_formed_xml", [] {
    const std::string filled = text::replace_all(fs::read_text(cpt_template()), "{{GUID}}", "{AAAABBBB-CCCC-DDDD-EEEE-FFFF00001111}");
    CB_CHECK(!cpt_has(filled, "{{GUID}}"));
    cpt_well_formed(filled);
});

CB_TEST("java.console.ports.a_flat_dump_is_structured_with_a_bound_solution", [] {
    fs::ScopedTempDir tmp("cbcport");
    const stdfs::path root = tmp.path() / "MyGame";
    cpt_flat_dump(root);
    const auto r = project_layout::layout(root, cpt_template(), cpt_rules());
    CB_CHECK_EQ(r.name, "MyGame");
    CB_CHECK(r.moved >= std::size_t{7});
    CB_CHECK((cpt_listing(root) == std::vector<std::string>{"MyGame", "MyGame.sln"}));   // the root holds only the project folder and the .sln
    CB_CHECK(stdfs::is_regular_file(root / "MyGame" / "Foo.cpp"));
    CB_CHECK(stdfs::is_directory(root / "MyGame" / "Content"));
    CB_CHECK(stdfs::is_regular_file(root / "MyGame" / "MyGame.vcxproj"));
    const std::string sln = fs::read_text(r.sln);
    CB_CHECK(cpt_has(sln, "MyGame\\MyGame.vcxproj"));
    CB_CHECK(cpt_has(sln, r.guid));
    const std::string vcx = fs::read_text(r.project_file);
    CB_CHECK(cpt_has(vcx, r.guid));
    CB_CHECK(cpt_has(vcx, "Foo.cpp"));                           // the file list is refreshed
    CB_CHECK(cpt_has(vcx, "Create</PrecompiledHeader>"));        // the stdafx precompiled-header block is kept
    cpt_well_formed(vcx);                                        // the project and its filters parse
    const stdfs::path filters = r.project_file.parent_path() / "MyGame.vcxproj.filters";
    CB_CHECK(stdfs::is_regular_file(filters));
    cpt_well_formed(fs::read_text(filters));
});

CB_TEST("java.console.ports.a_second_layout_moves_nothing_and_never_nests_again", [] {
    fs::ScopedTempDir tmp("cbcport");
    const stdfs::path root = tmp.path() / "MyGame";
    cpt_flat_dump(root);
    const auto r1 = project_layout::layout(root, cpt_template(), cpt_rules());
    const auto r2 = project_layout::layout(root, cpt_template(), cpt_rules());
    CB_CHECK_EQ(r2.moved, std::size_t{0});
    CB_CHECK_EQ(r1.guid, r2.guid);
    CB_CHECK(!stdfs::exists(r1.proj_dir / "MyGame"));            // never <root>/<Name>/<Name>/
    CB_CHECK((cpt_listing(root) == std::vector<std::string>{"MyGame", "MyGame.sln"}));
});

CB_TEST("java.console.ports.a_project_that_has_its_own_guid_keeps_it", [] {
    fs::ScopedTempDir tmp("cbcport");
    const stdfs::path root = tmp.path() / "MyGame";
    fs::write_text(root / "MyGame" / "Foo.cpp", "x");
    const std::string pre = "{AAAABBBB-CCCC-DDDD-EEEE-FFFF00001111}";
    fs::write_text(root / "MyGame" / "MyGame.vcxproj", text::replace_all(fs::read_text(cpt_template()), "{{GUID}}", pre));
    const auto r = project_layout::layout(root, cpt_template(), cpt_rules());
    CB_CHECK_EQ(r.guid, pre);
    CB_CHECK(cpt_has(fs::read_text(r.sln), pre));
});

CB_TEST("java.console.ports.sources_already_in_the_project_folder_stay", [] {
    fs::ScopedTempDir tmp("cbcport");
    const stdfs::path root = tmp.path() / "MyGame";
    for (const char* n : {"Foo.cpp", "stdafx.cpp"}) fs::write_text(root / "MyGame" / n, "x");
    const auto r = project_layout::layout(root, cpt_template(), cpt_rules());
    CB_CHECK_EQ(r.moved, std::size_t{0});
    CB_CHECK(stdfs::is_regular_file(r.sln));
});

CB_TEST("java.console.ports.the_runnable_entry_starts_the_game_up_and_runs_it", [] {
    const auto game = java::parse::parse_source("public final class Game implements Runnable {\n"
                                                " public Game(Object canvas, int w, int h, boolean f) {}\n public void run() {}\n}");
    const std::string entry = platform_refs::generate_runnable_entry_point(game);
    CB_CHECK(cpt_has(entry, "new Game(NULL, SCREEN_WIDTH, SCREEN_HEIGHT, false)"));   // a reference becomes NULL, ints the screen size, a bool false
    CB_CHECK(cpt_has(entry, "_game->run();"));
    CB_CHECK(!cpt_has(entry, "MinecraftImpl"));
    CB_CHECK(cpt_has(entry, "j2x_initAll();") && cpt_has(entry, "Display::create();"));
});
