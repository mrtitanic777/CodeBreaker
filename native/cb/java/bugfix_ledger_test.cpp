#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/bugfix_ledger.hpp"

namespace {

using namespace cb::java::bugfix_ledger;
namespace stdfs = std::filesystem;

stdfs::path fixture() { return cb::testing::data_path("java/testdata/bugfix_tree"); }

// testdata/bugfix_tree is a small tree exercising every rule of the scan (layers, the pruned data folders, caches,
// the scanner's own file, meta references, a description borrowed from the next line, pipes, long text), and
// testdata/bugfix_ledger.golden.md is what the Python module renders for it.
// DIVERGES: the italic line under the title names the module `bugfix_ledger` and the `codebreaker java
// bugfix-ledger` command; the Python named its .py file and `python -m codebreaker`, which no longer exist.
CB_TEST("java.bugfix_ledger.render_matches_python", [] {
    auto entries = collect(fixture());
    CB_CHECK_EQ(entries.size(), std::size_t{12});
    CB_CHECK_EQ(render(entries), cb::fs::read_text(cb::testing::data_path("java/testdata/bugfix_ledger.golden.md")));
});

CB_TEST("java.bugfix_ledger.collect_details", [] {
    auto entries = collect(fixture());
    auto find = [&](std::string_view slug) -> const MarkerEntry* {
        for (const auto& e : entries) {
            if (e.slug == slug) return &e;
        }
        return nullptr;
    };
    // a description on the marker line, one borrowed from the next comment line, none at all
    CB_CHECK_EQ(find("inflate-window")->description, "the window must be 32K, not 16K");
    CB_CHECK_EQ(find("display-flip")->description, "The flip must happen before Present() | else tearing");
    CB_CHECK_EQ(find("deep-one")->description, "");
    CB_CHECK_EQ(find("display-flip")->line, std::uint32_t{2});
    CB_CHECK(find("display-flip")->layer == Layer::Reference);
    CB_CHECK(find("net-order")->layer == Layer::Companion);
    CB_CHECK(find("inflate-window")->layer == Layer::Canonical);
    CB_CHECK(find("md-slug")->layer == Layer::Generator);
    CB_CHECK_EQ(find("deep-one")->file, "sub/deeper/Deep.cc");
    // only the first marker on a line
    CB_CHECK(find("second-one") == nullptr);
    // meta references, other layers' folders, caches, the scanner itself and other extensions are skipped
    for (const char* skipped : {"marker", "markers", "not-in-any-layer", "api-map-pruned", "scanner-itself-skipped",
                                "wrong-extension", "pycache-skipped", "nested-pycache-skipped"}) {
        CB_CHECK(find(skipped) == nullptr);
    }
});

CB_TEST("java.bugfix_ledger.run_writes_file", [] {
    cb::fs::ScopedTempDir tmp("cbbugfix");
    auto entries = run(fixture(), tmp.path() / "state" / "BUGFIX_LEDGER.md");
    CB_CHECK_EQ(entries.size(), std::size_t{12});
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "state" / "BUGFIX_LEDGER.md"), render(entries));
});

CB_TEST("java.bugfix_ledger.empty_and_missing", [] {
    cb::fs::ScopedTempDir tmp("cbbugfix");
    CB_CHECK(collect(tmp.path()).empty());                 // no layer folders
    CB_CHECK(collect(tmp.path() / "missing").empty());
    const std::string md = render({});
    CB_CHECK(md.find("**0 markers across 0 distinct fixes.**") != std::string::npos);
    CB_CHECK(md.find("## ") == std::string::npos);        // no layer sections when there is nothing to list
});

CB_TEST("java.bugfix_ledger.sorted_within_layer", [] {
    std::vector<MarkerEntry> entries = {
        {Layer::Generator, "z.cpp", "late", 1, "d"},
        {Layer::Generator, "a.cpp", "second", 9, "d"},
        {Layer::Generator, "a.cpp", "first", 2, "d"},
    };
    const std::string md = render(entries);
    CB_CHECK(md.find("`first`") < md.find("`second`"));
    CB_CHECK(md.find("`second`") < md.find("`late`"));
    CB_CHECK(md.find("**3 markers across 3 distinct fixes.**") != std::string::npos);
    entries.push_back({Layer::Generator, "b.cpp", "first", 4, "again"});   // the same slug in two places counts once
    CB_CHECK(render(entries).find("**4 markers across 3 distinct fixes.**") != std::string::npos);
});

}  // namespace
