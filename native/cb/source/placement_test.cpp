#include "cb/source/placement.hpp"

#include <format>
#include <memory>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {
// (std::string::contains is C++23.)
bool has(std::string_view haystack, std::string_view needle) { return haystack.find(needle) != std::string_view::npos; }
}  // namespace


using namespace cb;
using namespace cb::source::placement;
namespace stdfs = std::filesystem;

namespace {

stdfs::path table_file() { return testing::data_path("source/testdata/placement.json"); }

// A source tree with a few module folders already in it.
struct Tree {
    fs::ScopedTempDir dir{"cbplace"};
    Tree() {
        for (const char* d : {"shell/zebra", "base/Alpha", "design/hidden", ".git/hidden", "tools/hidden", "inc/hidden", "build/hidden", "mmedia/zebra"}) {
            fs::make_dirs(dir.path() / d);
        }
        fs::write_text(dir.path() / "shell" / "afile", "not a folder");   // only folders count
    }
};

}  // namespace

CB_TEST("source.placement.how_names", [] {
    CB_CHECK_EQ(how_name(How::AlreadyThere), "already there");
    CB_CHECK_EQ(how_name(How::Known), "known");
    CB_CHECK_EQ(how_name(How::Kind), "kind");
    CB_CHECK_EQ(how_name(How::Installed), "installed");
    CB_CHECK_EQ(how_name(How::Suffix), "suffix");
    CB_CHECK_EQ(how_name(How::Prefix), "prefix");
    CB_CHECK_EQ(how_name(How::Unsorted), "unsorted");
});

CB_TEST("source.placement.the_win95_table_loads_in_the_files_order", [] {
    const ProductTable t = load_table(table_file(), "win95");
    CB_CHECK_EQ(t.known.size(), 69u);
    CB_CHECK_EQ(t.known.at("kernel32"), "core/kernel32");
    CB_CHECK_EQ(t.suffix.size(), 10u);
    CB_CHECK_EQ(t.kind.at("le"), "vmm32");
    CB_CHECK_EQ(t.installed.size(), 2u);
    CB_CHECK_EQ(t.installed.begin()->first, "/command/");   // the order the file has them in is the order they are tried
    CB_CHECK_EQ(t.prefix.size(), 41u);
    CB_CHECK_EQ(t.prefix.front().first, "mci");
    CB_CHECK_EQ(t.prefix.front().second, "mmedia");
    // a product the file does not know has the empty table, and so does one whose entry is not an object
    const ProductTable none = load_table(table_file(), "win2000");
    CB_CHECK(none.known.empty() && none.kind.empty() && none.installed.size() == 0 && none.suffix.empty() && none.prefix.empty());
});

CB_TEST("source.placement.each_way_of_deciding", [] {
    Tree tree;
    const Placement p(tree.dir.path(), load_table(table_file(), "win95"));
    const auto check = [&](std::string_view module, const stdfs::path& file, std::optional<base::Lane> lane, std::string_view folder, How how) {
        const Decision d = p.resolve(module, file, lane);
        CB_CHECK_EQ(d.folder, std::string(folder));
        CB_CHECK(d.how == how);
    };
    check("zebra.dll", {}, std::nullopt, "mmedia/zebra", How::AlreadyThere);          // sorted first of the two that hold it
    check("ALPHA.DLL", {}, std::nullopt, "base/Alpha", How::AlreadyThere);             // name case does not matter
    check("kernel32.dll", {}, std::nullopt, "core/kernel32", How::Known);              // the table's value is the folder itself
    check("Kernel32.DLL", {}, std::nullopt, "core/kernel32", How::Known);
    check("vmm.386", "C:/win/system/vmm.386", base::Lane::Le, "vmm32/vmm", How::Kind);
    check("himem.sys", {}, base::Lane::Dos, "msdos/himem", How::Kind);
    check("x.pdr", "C:\\WIN\\IOSUBSYS\\x.pdr", std::nullopt, "vmm32/x", How::Installed);   // backslashes and case are normalised
    check("more.exe", "D:/win95/command/more.exe", std::nullopt, "msdos/more", How::Installed);
    check("foo.acm", {}, std::nullopt, "mmedia/foo", How::Suffix);                     // suffix before prefix
    check("mcifoo.dll", {}, std::nullopt, "mmedia/mcifoo", How::Prefix);
    check("zzz.dll", {}, std::nullopt, "unsorted/zzz", How::Unsorted);
    check("a.b.dll", {}, std::nullopt, "unsorted/a.b", How::Unsorted);                 // only the last extension is dropped
    check("noext", {}, std::nullopt, "unsorted/noext", How::Unsorted);
    check("C:/anywhere/KERNEL32.DLL", {}, std::nullopt, "core/kernel32", How::Known);  // a path as the module: its name counts
});

CB_TEST("source.placement.folders_the_tree_does_not_count", [] {
    Tree tree;
    const Placement p(tree.dir.path(), ProductTable{});
    const auto& existing = p.existing();
    CB_CHECK_EQ(existing.size(), 2u);
    CB_CHECK_EQ(existing.at("zebra"), "mmedia/zebra");   // 'mmedia' sorts before 'shell'
    CB_CHECK_EQ(existing.at("alpha"), "base/Alpha");
    CB_CHECK(!existing.contains("hidden"));              // design, build, tools, inc and dot folders hold none
    // a tree that is not there has nothing in it
    const Placement empty(tree.dir.path() / "nowhere", ProductTable{});
    CB_CHECK(empty.existing().empty());
    CB_CHECK_EQ(empty.resolve("any.dll").folder, "unsorted/any");
});

CB_TEST("source.placement.folder_is_created_on_demand", [] {
    Tree tree;
    const Placement p(tree.dir.path(), load_table(table_file(), "win95"));
    const stdfs::path wanted = tree.dir.path() / "core" / "kernel32";
    CB_CHECK(!stdfs::exists(wanted));
    CB_CHECK(p.folder("kernel32.dll", {}, std::nullopt, false) == wanted);
    CB_CHECK(!stdfs::exists(wanted));
    CB_CHECK(p.folder("kernel32.dll") == wanted);
    CB_CHECK(stdfs::is_directory(wanted));
});

CB_TEST("source.placement.other_products_have_their_own_tables", [] {
    Tree tree;
    const Placement p98(tree.dir.path(), load_table(table_file(), "win98"));
    CB_CHECK_EQ(p98.resolve("sound.drv").folder, "drivers/sound");
    CB_CHECK_EQ(p98.resolve("kernel32.dll").folder, "unsorted/kernel32");   // win95's names mean nothing here
    CB_CHECK_EQ(p98.resolve("himem.sys", {}, base::Lane::Dos).folder, "msdos/himem");
});

CB_TEST("source.placement.a_damaged_table_is_refused", [] {
    fs::ScopedTempDir dir{"cbplace"};
    const auto write = [&](std::string_view body) {
        const stdfs::path f = dir.path() / "t.json";
        fs::write_text(f, std::string(body));
        return f;
    };
    CB_CHECK_THROWS_AS(NotFoundError, (void)load_table(dir.path() / "missing.json", "win95"));
    CB_CHECK_THROWS_AS(ParseError, (void)load_table(write("{ not json"), "win95"));
    CB_CHECK_THROWS_AS(FormatError, (void)load_table(write("[1, 2]"), "win95"));
    CB_CHECK_THROWS_AS(FormatError, (void)load_table(write(R"({"win95": {"known": ["a"]}})"), "win95"));
    CB_CHECK_THROWS_AS(FormatError, (void)load_table(write(R"({"win95": {"known": {"a": 3}}})"), "win95"));
    CB_CHECK_THROWS_AS(FormatError, (void)load_table(write(R"({"win95": {"prefix": [["only-one"]]}})"), "win95"));
    CB_CHECK_THROWS_AS(FormatError, (void)load_table(write(R"({"win95": {"prefix": {"mci": "mmedia"}}})"), "win95"));
    // a product with an entry that is not an object has the empty table
    CB_CHECK(load_table(write(R"({"win95": 3})"), "win95").known.empty());
});

namespace {

// A corpus whose files' third byte says what they are (the formats' header probe stands in for it).
struct CorpusTree {
    fs::ScopedTempDir dir{"cbplacecorpus"};
    fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    std::optional<fs::ScopedEnv> root_env;   // the default product's folder is found through CB_CORPUS_ROOT
    fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};
    stdfs::path win95;

    CorpusTree() {
        win95 = dir.path() / "corpus95";
        root_env.emplace("CB_CORPUS_ROOT", fs::to_text(win95));
        fs::make_dirs(win95 / "WINDOWS" / "SYSTEM");
        fs::write_text(win95 / "WINDOWS" / "SYSTEM" / "kernel32.dll", "MZP");
        fs::write_text(win95 / "WINDOWS" / "SYSTEM" / "vmm.386", "MZL");
        fs::write_text(win95 / "WINDOWS" / "SYSTEM" / "mcizzz.dll", "MZP");
    }
    std::unique_ptr<base::Corpus> corpus() {
        base::Products products;
        products.target = "win95";
        products.target_folder = win95;
        products.checkout_root = dir.path() / "CodeBreaker";
        products.state = dir.path() / "state";
        base::FormatProbes probes;
        probes.lane = [](const stdfs::path& p) -> std::optional<base::Lane> {
            const std::string head = fs::read_text(p).substr(0, 3);
            if (head.size() < 3 || head[0] != 'M') return std::nullopt;
            return head[2] == 'L' ? base::Lane::Le : base::Lane::Pe;
        };
        return std::make_unique<base::Corpus>(products, probes);
    }
};

}  // namespace

CB_TEST("source.placement.the_place_command_reports_each_module", [] {
    CorpusTree world;
    const auto corpus = world.corpus();
    Tree tree;
    const Placement p(tree.dir.path(), load_table(table_file(), "win95"));
    testing::CapturedStd captured;
    const std::vector<std::string> args = {"kernel32", "VMM.386", "mcizzz.dll", "nothing.dll"};
    CB_CHECK_EQ(place_command(p, *corpus, args), 0);
    const std::string out = captured.out();
    const auto row = [](std::string_view module, std::string_view folder, std::string_view how) {
        return std::format("{:<16} {:<32} {}\n", module, folder, how);   // name, folder and how, in columns
    };
    CB_CHECK(has(out, row("kernel32.dll", "core/kernel32", "known")));
    CB_CHECK(has(out, row("vmm.386", "vmm32/vmm", "kind")));    // the file's header says LE
    CB_CHECK(has(out, row("mcizzz.dll", "mmedia/mcizzz", "prefix")));
    CB_CHECK(has(out, row("nothing.dll", "unsorted/nothing", "unsorted")));
    CB_CHECK(has(out, "kernel32.dll     core/kernel32 "));   // a 16-column name, a space, a 32-column folder
    CB_CHECK(has(out, "4 modules: known 1, kind 1, prefix 1, unsorted 1\n"));
});

CB_TEST("source.placement.the_place_command_with_nothing_to_place_prints_its_help", [] {
    CorpusTree world;
    const auto corpus = world.corpus();
    Tree tree;
    const Placement p(tree.dir.path(), ProductTable{});
    testing::CapturedStd captured;
    CB_CHECK((place_command(p, *corpus, std::vector<std::string>{})) == (kExitUsage));
    CB_CHECK(has(captured.out(), "codebreaker place <module> ... | --all [--list]"));
});

CB_TEST("source.placement.the_place_command_all_counts_the_corpus", [] {
    CorpusTree world;
    const auto corpus = world.corpus();
    Tree tree;
    const Placement p(tree.dir.path(), load_table(table_file(), "win95"));
    testing::CapturedStd captured;
    CB_CHECK((place_command(p, *corpus, std::vector<std::string>{"--all"})) == (0));
    // without --list only the summary is printed: two PE modules, and the one .386 as a driver
    const std::string out = captured.out();
    CB_CHECK(!has(out, "kernel32.dll"));
    CB_CHECK(has(out, "3 modules: "));
});
