#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/ledger.hpp"

namespace {

using namespace cb::java::ledger;

Ledger sample() {
    Ledger l;
    l.add("b/Zed.cpp", Category::Error, "C2065 undeclared", {12, std::nullopt, "build", "C2065", "x = y;"});
    l.add("a/Alpha.cpp", Category::Guessed, "guessed pointer", {7, std::nullopt, "emit"});
    l.add("a/Alpha.cpp", Category::Rule, "rule fired", {7, std::nullopt, "", "trig-double"});
    l.add("a/Alpha.cpp", Category::Stubbed, "stub here", {3, Severity::Med});
    l.add("c/Beta.cpp", Category::Ported, "clean");
    l.add("b/Zed.cpp", Category::Warning, "tab\there \"quoted\" back\\slash", {2});
    return l;
}

CB_TEST("java.ledger.names_and_defaults", [] {
    for (Category c : all_categories()) CB_CHECK(parse_category(name(c)) == c);
    CB_CHECK(!parse_category("error"));   // exact
    CB_CHECK(parse_severity("MED") == Severity::Med);
    CB_CHECK(!parse_severity("HI"));
    CB_CHECK_EQ(weight(Severity::High), 5);
    CB_CHECK_EQ(weight(Severity::Med), 2);
    CB_CHECK_EQ(weight(Severity::Low), 0);
    CB_CHECK(default_severity(Category::Error) == Severity::High);
    CB_CHECK(default_severity(Category::Guessed) == Severity::High);
    CB_CHECK(default_severity(Category::Stubbed) == Severity::High);
    CB_CHECK(default_severity(Category::Blocked) == Severity::High);
    CB_CHECK(default_severity(Category::Risk) == Severity::Med);
    CB_CHECK(default_severity(Category::Warning) == Severity::Med);
    CB_CHECK(default_severity(Category::Overridden) == Severity::Med);
    CB_CHECK(default_severity(Category::Annotated) == Severity::Low);
    CB_CHECK(default_severity(Category::Ported) == Severity::Low);
    CB_CHECK(default_severity(Category::Rule) == Severity::Low);
});

CB_TEST("java.ledger.add_and_queries", [] {
    Ledger l = sample();
    CB_CHECK_EQ(l.size(), std::size_t{6});
    CB_CHECK(l.events()[0].severity == Severity::High);      // the category's default
    CB_CHECK(l.events()[3].severity == Severity::Med);       // overridden by the producer
    CB_CHECK_EQ(l.events()[0].snippet, "x = y;");
    CB_CHECK_EQ(l.events()[4].line, std::uint32_t{0});

    const std::vector<std::string> files = {"a/Alpha.cpp", "b/Zed.cpp", "c/Beta.cpp"};
    CB_CHECK(l.files() == files);

    auto by_file = l.by_file();
    const auto& alpha = by_file.at("a/Alpha.cpp");
    CB_CHECK_EQ(alpha.size(), std::size_t{3});
    CB_CHECK_EQ(alpha[0]->line, std::uint32_t{3});               // by line...
    CB_CHECK(alpha[1]->category == Category::Guessed);           // ...then severity, HIGH before LOW at line 7
    CB_CHECK(alpha[2]->category == Category::Rule);

    CategoryCounts cats = l.totals_by_category();
    CB_CHECK_EQ(cats[static_cast<std::size_t>(Category::Error)], std::size_t{1});
    CB_CHECK_EQ(cats[static_cast<std::size_t>(Category::Blocked)], std::size_t{0});
    SeverityCounts sevs = l.totals_by_severity();
    CB_CHECK_EQ(sevs[static_cast<std::size_t>(Severity::High)], std::size_t{2});
    CB_CHECK_EQ(sevs[static_cast<std::size_t>(Severity::Med)], std::size_t{2});
    CB_CHECK_EQ(sevs[static_cast<std::size_t>(Severity::Low)], std::size_t{2});
});

CB_TEST("java.ledger.risk_table", [] {
    // Python's file_risk_table for these events: Alpha 7 (stubbed MED + guessed HIGH), Zed 7 (error HIGH + warning
    // MED), Beta 0 -- the tie broken by file name.
    Ledger l = sample();
    auto rows = l.file_risk_table();
    CB_CHECK_EQ(rows.size(), std::size_t{3});
    CB_CHECK_EQ(rows[0].file, "a/Alpha.cpp");
    CB_CHECK_EQ(rows[0].risk, 7);
    CB_CHECK_EQ(rows[1].file, "b/Zed.cpp");
    CB_CHECK_EQ(rows[1].risk, 7);
    CB_CHECK_EQ(rows[2].file, "c/Beta.cpp");
    CB_CHECK_EQ(rows[2].risk, 0);
    CB_CHECK_EQ(rows[0].categories[static_cast<std::size_t>(Category::Stubbed)], std::size_t{1});
    CB_CHECK_EQ(rows[0].severities[static_cast<std::size_t>(Severity::Low)], std::size_t{1});
    CB_CHECK_EQ(Ledger::risk_score({}), 0);

    // a file with more HIGH events outranks one with fewer
    l.add("c/Beta.cpp", Category::Blocked, "cannot port");
    l.add("c/Beta.cpp", Category::Blocked, "cannot port 2");
    CB_CHECK_EQ(l.file_risk_table()[0].file, "c/Beta.cpp");
    CB_CHECK_EQ(l.file_risk_table()[0].risk, 10);
});

CB_TEST("java.ledger.extend_and_empty", [] {
    Ledger a = sample();
    Ledger b;
    CB_CHECK_EQ(b.size(), std::size_t{0});
    CB_CHECK(b.files().empty() && b.file_risk_table().empty());
    b.extend(a.events());
    b.extend(a.events());
    CB_CHECK_EQ(b.size(), std::size_t{12});
    CB_CHECK_EQ(b.totals_by_category()[static_cast<std::size_t>(Category::Error)], std::size_t{2});
});

// testdata/ledger.golden.json: Python's Ledger.to_json() for the same six events (reviewed: key order, the totals
// listing every category and severity, escapes of tab/quote/backslash).
CB_TEST("java.ledger.to_json_matches_python", [] {
    CB_CHECK_EQ(sample().to_json() + "\n", cb::fs::read_text(cb::testing::data_path("java/testdata/ledger.golden.json")) + "\n");
});

}  // namespace
