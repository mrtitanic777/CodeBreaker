#include "cb/java/console/worklist.hpp"

#include "cb/core/error.hpp"
#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

// The stand-in compiler of testdata/worklist: its stderr is the text of the "source" file it is given.
worklist::Options fake_compiler() {
    worklist::Options options;
    options.compiler = fs::to_text(test::data("worklist/fakecc.cmd"));
    return options;
}

CB_TEST("java.console.worklist.matches_the_original", [] {
    const stdfs::path out = test::data("worklist/out");
    const worklist::Worklist w = worklist::check(out, fake_compiler());
    CB_CHECK_EQ(w.total, std::size_t{7});     // the .cpp files in the folder; header.h is not compiled
    CB_CHECK_EQ(w.clean, std::size_t{1});
    // the markdown the original wrote for the same folder, but for the command that regenerates it
    std::string golden = test::golden("worklist.WORKLIST.golden.md");
    golden = text::replace_all(golden, "`python -m codebreaker java worklist out`", "`codebreaker java worklist out`");
    CB_CHECK_EQ(test::lf(worklist::render("out", w)), golden);

    const worklist::Worklist none = worklist::check(test::data("worklist/emptyout"), fake_compiler());
    std::string golden_empty = test::golden("worklist.WORKLIST_EMPTY.golden.md");
    golden_empty = text::replace_all(golden_empty, "`python -m codebreaker java worklist emptyout`", "`codebreaker java worklist emptyout`");
    CB_CHECK_EQ(test::lf(worklist::render("emptyout", none)), golden_empty);
});

CB_TEST("java.console.worklist.order_and_jobs", [] {
    const stdfs::path out = test::data("worklist/out");
    worklist::Options options = fake_compiler();
    options.jobs = 4;   // the same answer however many compiles run at once
    const worklist::Worklist w = worklist::check(out, options);
    std::string order;
    for (const auto& f : w.failing) order += f.file + ":" + std::to_string(f.errors) + " ";
    // fewest errors first, equal counts by file name (capitals before lower case)
    CB_CHECK_EQ(order, "B.cpp:1 UPPER.CPP:1 long.cpp:1 one.cpp:1 tie.cpp:2 two.cpp:3 ");
});

CB_TEST("java.console.worklist.categories", [] {
    using worklist::dominant_category;
    CB_CHECK_EQ(dominant_category(""), "(unknown)");
    CB_CHECK_EQ(dominant_category("ok.cpp: warning: nothing\n"), "(unknown)");
    // quoted names and numbers are made alike, the most common wins
    CB_CHECK_EQ(dominant_category("a.cpp:1: error: unknown type 'Foo'\nb.cpp:2: error: unknown type 'Bar'\nc.cpp:3: error: other\n"),
                "unknown type X");
    CB_CHECK_EQ(dominant_category("x: error: line 12 of 34\ny: error: line 5 of 6\n"), "line N of N");
    CB_CHECK_EQ(dominant_category("x: error: first\ny: error: second\n"), "first");                // a tie: who came first
    CB_CHECK_EQ(dominant_category("x: error: a\ny: error: b\nz: error: b\n"), "b");
    CB_CHECK_EQ(dominant_category("\xE2\x80\x98" "x: error: 'a' and 'b' are 'c'\n"), "X and X are X");
    CB_CHECK_EQ(dominant_category("x: error: 'unclosed quote stays\n"), "'unclosed quote stays");
    CB_CHECK_EQ(dominant_category("x: error: " + std::string(100, 'z') + "\n"), std::string(60, 'z'));       // cut at 60 columns
    // 60 columns, not 60 bytes: a cut never splits a character
    const std::string accents(70, '\x7f');
    CB_CHECK_EQ(dominant_category("x: error: " + std::string(59, 'a') + "\xC3\xA9\xC3\xA9\n"), std::string(59, 'a') + "\xC3\xA9");
    CB_CHECK_EQ(worklist::count_errors("a: error: x\nb: note: y\nc: error: z\n: error:\nerror: no colon before\n"), std::size_t{3});
    CB_CHECK_EQ(worklist::count_errors(""), std::size_t{0});
});

CB_TEST("java.console.worklist.run_writes_the_file", [] {
    fs::ScopedTempDir tmp("cbwl");
    fs::write_text(tmp.path() / "out" / "bad.cpp", "bad.cpp:1: error: boom\n");
    fs::write_text(tmp.path() / "out" / "good.cpp", "fine\n");
    const stdfs::path file = tmp.path() / "report" / "WORKLIST.md";
    const std::string line = worklist::run(tmp.path() / "out", file, fake_compiler());
    CB_CHECK(line.starts_with("clean=1/2  failing=1  -> "));
    CB_CHECK(line.ends_with("WORKLIST.md"));
    const std::string md = test::lf(fs::read_text(file));
    CB_CHECK(md.find("1 / 2 clean; 1 failing.") != std::string::npos);
    CB_CHECK(md.find("| 1 | bad.cpp | boom |") != std::string::npos);
    // a compiler that cannot be started is an error, not "everything failed"
    worklist::Options broken;
    broken.compiler = "definitely-not-a-compiler-cbtest";
    CB_CHECK_THROWS_AS(cb::ToolError, (void)worklist::check(tmp.path() / "out", broken));
    // a folder that does not exist has nothing to compile
    const worklist::Worklist missing = worklist::check(tmp.path() / "nowhere", fake_compiler());
    CB_CHECK_EQ(missing.total, std::size_t{0});
});

}  // namespace
