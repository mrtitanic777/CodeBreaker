#include "cb/java/console/project_layout.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

const vcxproj::FilterRules& rules() {
    static const vcxproj::FilterRules r({{"^(Debug|HeapTrack)$", "Debug"}, {"^(stdafx|Common)$", "Runtime"}, {"^Block$", "Blocks\\Base"},
                                         {"^LiquidBlock$", "Blocks\\Liquid"}, {"^Gui\\w*", "Gui"}, {"^A&B$", "Amp & <Co>"},
                                         {"^Zed$", "Zed"}});
    return r;
}

stdfs::path template_file() { return testing::data_path("../../codebreaker/java/console/templates/xbox360.vcxproj.in"); }

// A folder's files as the golden lists them: every folder in the order of its path (written with backslashes), its
// files by name; the solution and project files with their text, the others with their size.
std::string snapshot(const std::string& label, const stdfs::path& root, const project_layout::Layout& made) {
    std::vector<std::pair<std::string, stdfs::path>> dirs;   // (sort key, folder)
    dirs.emplace_back(fs::to_text(root), root);
    for (const auto& e : stdfs::recursive_directory_iterator(root))
        if (e.is_directory()) dirs.emplace_back(fs::to_text(e.path()), e.path());
    for (auto& [key, dir] : dirs) std::replace(key.begin(), key.end(), '/', '\\');
    std::sort(dirs.begin(), dirs.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::string out = std::format("layout {}: name={} moved={} guid={}", label, made.name, made.moved, made.guid);
    for (const auto& [key, dir] : dirs) {
        std::vector<std::string> files;
        for (const auto& e : stdfs::directory_iterator(dir))
            if (e.is_regular_file()) files.push_back(fs::to_text(e.path().filename()));
        std::sort(files.begin(), files.end());
        for (const std::string& f : files) {
            const std::string rel = fs::to_text((dir / f).lexically_relative(root));
            const std::string ext = fs::to_text(stdfs::path(f).extension());
            if (ext == ".sln" || ext == ".vcxproj" || ext == ".filters")
                out += std::format("\n  file {}:\n{}", rel, test::lf(fs::read_text(dir / f)));
            else
                out += std::format("\n  file {} ({} bytes)", rel, fs::file_size(dir / f));
        }
    }
    return out;
}

CB_TEST("java.console.project_layout.matches_the_original", [] {
    fs::ScopedTempDir tmp("cblay");
    struct Case {
        const char* label;
        const char* src;
        const char* sub;
        const char* name;
        bool regen;
    };
    std::vector<std::string> elements;
    for (const Case& c : {Case{"flat", "in_flat", "Game", "", true}, Case{"guid", "in_guid", "Proj", "", true},
                          Case{"named", "in_named", "Root", "Alt", true}, Case{"noregen", "in_flat", "Game", "", false},
                          Case{"empty", "in_empty", "Empty", "", true}}) {
        const stdfs::path work = tmp.path() / "work";
        stdfs::remove_all(work);
        fs::copy_tree(test::data(std::string("layout/") + c.src), work);
        const stdfs::path root = work / c.sub;
        project_layout::Options options;
        options.name = c.name;
        options.regenerate = c.regen;
        project_layout::Layout made = project_layout::layout(root, template_file(), rules(), options);
        elements.push_back(snapshot(c.label, root, made));
        elements.push_back("  stdout: " + text::replace_all(made.refreshed ? made.refreshed->summary() + "\n" : std::string(), "\n", "\\n"));
        if (std::string(c.label) == "flat") {   // running again changes nothing
            project_layout::Layout again = project_layout::layout(root, template_file(), rules(), options);
            elements.push_back(snapshot("flat-again", root, again));
        }
        stdfs::remove_all(work);
    }
    CB_CHECK_EQ(text::join(elements, "\n") + "\n", test::golden("layout.golden.txt"));
});

CB_TEST("java.console.project_layout.pieces", [] {
    // the GUID of a name never changes (a re-run binds the same .sln to the same .vcxproj)
    CB_CHECK_EQ(project_layout::project_guid("Game"), "{3AC54909-BE8D-9388-5187-E1192E8D3ED6}");
    CB_CHECK_EQ(project_layout::project_guid("Game"), project_layout::project_guid("Game"));
    CB_CHECK_NE(project_layout::project_guid("Game"), project_layout::project_guid("game"));

    fs::ScopedTempDir tmp("cblay");
    CB_CHECK(!project_layout::existing_guid(tmp.path() / "missing.vcxproj"));
    fs::write_text(tmp.path() / "a.vcxproj", "<Project/>");
    CB_CHECK(!project_layout::existing_guid(tmp.path() / "a.vcxproj"));
    fs::write_text(tmp.path() / "b.vcxproj", "\xEF\xBB\xBF<Project><ProjectGuid>{1-2}</ProjectGuid></Project>");   // with a byte-order mark
    CB_CHECK_EQ(*project_layout::existing_guid(tmp.path() / "b.vcxproj"), "{1-2}");
    // a tag without a brace in it is passed over for the next one that has a GUID
    fs::write_text(tmp.path() / "c.vcxproj", "<ProjectGuid>nobrace</ProjectGuid><ProjectGuid>{ok}</ProjectGuid>");
    CB_CHECK_EQ(*project_layout::existing_guid(tmp.path() / "c.vcxproj"), "{ok}");

    const std::string sln = project_layout::solution_text("Game", "{G}");
    CB_CHECK(sln.starts_with("\nMicrosoft Visual Studio Solution File, Format Version 11.00\n# Visual Studio 2010\n"));
    CB_CHECK(sln.find("= \"Game\", \"Game\\Game.vcxproj\", \"{G}\"") != std::string::npos);
    CB_CHECK(sln.find("\t\t{G}.Release_LTCG|Xbox 360.Build.0 = Release_LTCG|Xbox 360\n") != std::string::npos);
    CB_CHECK_EQ(std::count(sln.begin(), sln.end(), '\r'), std::ptrdiff_t{0});   // CRLF is applied when it is written
    const stdfs::path written = project_layout::write_solution(tmp.path(), "Game", "{G}");
    CB_CHECK_EQ(fs::to_text(written.filename()), "Game.sln");
    CB_CHECK(fs::read_text(written).find("\r\nEndProject\r\n") != std::string::npos);

    CB_CHECK(!project_layout::has_flat_sources(tmp.path() / "nowhere"));
    CB_CHECK(!project_layout::has_flat_sources(tmp.path()));
    fs::write_text(tmp.path() / "x.cpp", "");
    CB_CHECK(project_layout::has_flat_sources(tmp.path()));
});

CB_TEST("java.console.project_layout.move_keeps_what_belongs_at_the_root", [] {
    fs::ScopedTempDir tmp("cblay");
    const stdfs::path root = tmp.path() / "Game";
    for (const char* f : {"a.cpp", "b.h", "Game.sln", "Game.SUO", "x.sdf", "Content/c.png"}) fs::write_text(root / f, "x");
    fs::write_text(root / "Game" / "a.cpp", "OLD");   // already there: replaced
    fs::write_text(root / "Game" / "stale.cpp", "stays");
    const std::size_t moved = project_layout::move_flat_into(root, root / "Game", "Game");
    CB_CHECK_EQ(moved, std::size_t{3});   // a.cpp, b.h, Content
    CB_CHECK(stdfs::exists(root / "Game.sln") && stdfs::exists(root / "Game.SUO") && stdfs::exists(root / "x.sdf"));
    CB_CHECK_EQ(fs::read_text(root / "Game" / "a.cpp"), "x");
    CB_CHECK(stdfs::exists(root / "Game" / "stale.cpp") && stdfs::exists(root / "Game" / "Content" / "c.png"));
    CB_CHECK(!stdfs::exists(root / "a.cpp") && !stdfs::exists(root / "Content"));
});

CB_TEST("java.console.project_layout.template_is_needed_only_for_a_new_project", [] {
    fs::ScopedTempDir tmp("cblay");
    const stdfs::path root = tmp.path() / "Game";
    fs::write_text(root / "a.cpp", "x");
    const stdfs::path missing = tmp.path() / "no-template.in";
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)project_layout::layout(root, missing, rules()));
    // the folder is already laid out (it moved the sources before it noticed the template was missing): with a
    // project file present the template is never read
    fs::write_text(root / "Game" / "Game.vcxproj", "<Project>\n</Project>\n");
    CB_CHECK_EQ(project_layout::layout(root, missing, rules()).moved, std::size_t{0});
    // a template with the GUID marker gets this project's GUID
    fs::write_text(tmp.path() / "t.in", "<ProjectGuid>{{GUID}}</ProjectGuid>\n");
    const stdfs::path other = tmp.path() / "Other";
    fs::write_text(other / "b.cpp", "x");
    const auto made = project_layout::layout(other, tmp.path() / "t.in", rules());
    CB_CHECK(fs::read_text(made.project_file).find("<ProjectGuid>" + made.guid + "</ProjectGuid>") != std::string::npos);
    CB_CHECK_EQ(made.guid, project_layout::project_guid("Other"));
});

}  // namespace
