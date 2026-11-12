#include "cb/java/report.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using cb::java::ledger::Category;
using cb::java::ledger::EventDetail;
using cb::java::ledger::Ledger;
using cb::java::ledger::Severity;
namespace rpt = cb::java::report;

std::string lf_only(std::string_view s) { return cb::text::replace_all(s, "\r\n", "\n"); }

std::string report_golden(std::string_view name) {
    return lf_only(cb::fs::read_text(cb::testing::data_path(std::string("java/testdata/report/") + std::string(name))));
}

// A small ledger of the shapes a conversion leaves: two files with findings, one with only notes.
Ledger sample_ledger() {
    Ledger l;
    l.add("Block.cpp", Category::Risk, "new bool[] without ()", {.line = 12, .severity = Severity::High, .source = "rule",
                                                                  .rule = "uninit-array", .snippet = "bool* a = new bool[16];"});
    l.add("Block.cpp", Category::Guessed, "value type assumed", {.line = 3, .severity = Severity::Med, .source = "marker",
                                                                  .rule = "PORT", .snippet = "// PORT: assumed by value"});
    l.add("Block.cpp", Category::Annotated, "a note", {.source = "marker", .rule = "PORT NOTE"});
    l.add("World.cpp", Category::Stubbed, "D3D9 needed", {.line = 40, .severity = Severity::High, .source = "marker", .rule = "PORT stub"});
    l.add("sub/Item.h", Category::Error, "C2065 undeclared identifier", {.line = 7, .source = "build", .rule = "C2065"});
    l.add("Quiet.cpp", Category::Ported, "literal", {});
    return l;
}

CB_TEST("java.report.bar", [] {
    CB_CHECK_EQ(rpt::bar(3, 18, 0), "");
    CB_CHECK_EQ(rpt::bar(0, 4, 8), "\xC2\xB7\xC2\xB7\xC2\xB7\xC2\xB7");
    CB_CHECK_EQ(rpt::bar(8, 4, 8), "\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88");
    CB_CHECK_EQ(rpt::bar(1, 4, 8), "\xC2\xB7\xC2\xB7\xC2\xB7\xC2\xB7");   // half a cell rounds to even: 0, as the Python's round
    CB_CHECK_EQ(rpt::bar(3, 4, 8), "\xE2\x96\x88\xE2\x96\x88\xC2\xB7\xC2\xB7");   // 1.5 rounds to 2
});

CB_TEST("java.report.safe_name", [] {
    CB_CHECK_EQ(rpt::safe_name("sub/Item.h"), "sub_Item.h");
    CB_CHECK_EQ(rpt::safe_name("a\\b/c.cpp"), "a_b_c.cpp");
    CB_CHECK_EQ(rpt::safe_name("Plain.cpp"), "Plain.cpp");
});

CB_TEST("java.report.index_matches_the_reviewed_golden", [] {
    const Ledger l = sample_ledger();
    rpt::Meta meta;
    meta.profile = "test profile";
    meta.output_dir = "out/dir";
    CB_CHECK_EQ(rpt::render_index(l, meta), report_golden("index.golden.md"));
});

CB_TEST("java.report.index_of_an_empty_ledger", [] {
    const std::string idx = rpt::render_index(Ledger{});
    CB_CHECK(idx.starts_with("# Conversion Report\n\n## Totals\n"));
    CB_CHECK(idx.find("| **Total events** | **0** |") != std::string::npos);
    CB_CHECK(idx.find("0 files have findings; 0 carry HIGH-severity risk.") != std::string::npos);
    CB_CHECK(idx.find("`ERROR`") == std::string::npos);   // a category with no events is not listed
});

CB_TEST("java.report.file_report_matches_the_reviewed_golden", [] {
    const Ledger l = sample_ledger();
    const auto by_file = l.by_file();
    CB_CHECK_EQ(rpt::render_file_report("Block.cpp", by_file.at("Block.cpp")), report_golden("Block.cpp.golden.md"));
});

CB_TEST("java.report.only_the_risky_files_are_in_the_table", [] {
    const std::string idx = rpt::render_index(sample_ledger());
    // Quiet.cpp holds a PORTED note (risk 0): a file of its own, but not a row of the risk table
    CB_CHECK(idx.find("Quiet.cpp") == std::string::npos);
    CB_CHECK(idx.find("[Block.cpp](files/Block.cpp.md)") != std::string::npos);
    CB_CHECK(idx.find("[sub/Item.h](files/sub_Item.h.md)") != std::string::npos);
});

CB_TEST("java.report.write_reports_writes_the_bundle", [] {
    cb::fs::ScopedTempDir tmp("cbreport");
    const Ledger l = sample_ledger();
    const auto dir = rpt::write_reports(l, tmp.path());
    CB_CHECK_EQ(cb::fs::to_text(dir), cb::fs::to_text(tmp.path() / "report"));
    CB_CHECK(std::filesystem::is_regular_file(dir / "index.md"));
    CB_CHECK(std::filesystem::is_regular_file(dir / "report.json"));
    for (const char* f : {"Block.cpp.md", "World.cpp.md", "sub_Item.h.md", "Quiet.cpp.md"}) {
        CB_CHECK(std::filesystem::is_regular_file(dir / "files" / f));
    }
    CB_CHECK_EQ(cb::fs::read_text(dir / "report.json"), l.to_json());
    CB_CHECK_EQ(lf_only(cb::fs::read_text(dir / "files" / "World.cpp.md")),
                rpt::render_file_report("World.cpp", l.by_file().at("World.cpp")));
});

CB_TEST("java.report.write_reports_to_another_folder", [] {
    cb::fs::ScopedTempDir tmp("cbreport");
    const auto custom = tmp.path() / "elsewhere" / "r";
    const auto dir = rpt::write_reports(sample_ledger(), tmp.path() / "unused", custom, {.profile = "p"});
    CB_CHECK_EQ(cb::fs::to_text(dir), cb::fs::to_text(custom));
    CB_CHECK(std::filesystem::is_regular_file(custom / "index.md"));
    CB_CHECK(!std::filesystem::exists(tmp.path() / "unused"));
    CB_CHECK(cb::fs::read_text(custom / "index.md").find("- **Profile:** p") != std::string::npos);
});

}  // namespace
