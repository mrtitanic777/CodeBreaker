// cb-unity: alone
// Golden tests of `reach`: what it answers on real files.
// The inputs and answers are kept into coverage/testdata and read through (the platforms they name, the numbers in the
// tables). Only the words of the report's first sentence differ: it names the command as `codebreaker reach`.
#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/coverage/reach.hpp"

namespace {

using namespace cb;
namespace r = cb::coverage::reach;
namespace stdfs = std::filesystem;

std::vector<std::vector<std::string>> golden_rows(const char* file) {
    std::vector<std::vector<std::string>> rows;
    const std::string whole = fs::read_text(testing::data_path(std::string("coverage/testdata/") + file));
    for (const std::string_view line : text::lines(whole)) {
        if (line.empty()) continue;
        std::vector<std::string> fields;
        for (const std::string_view f : text::split(line, '\t')) fields.emplace_back(f);
        rows.push_back(std::move(fields));
    }
    return rows;
}

Bytes from_hex_text(std::string_view hex) {
    Bytes b;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
        b.push_back(static_cast<std::uint8_t>(std::stoi(std::string(hex.substr(i, 2)), nullptr, 16)));
    }
    return b;
}

}  // namespace

CB_TEST("coverage.reach.gap_of_matches_the_original_on_the_sample_programs", [] {
    const auto golden = golden_rows("gap.golden");
    CB_CHECK_EQ(golden.size(), 31u);
    for (const auto& g : golden) {
        const stdfs::path file = testing::data_path("coverage/testdata/games/" + g[0]);
        const Bytes whole = fs::read_file(file);
        const auto gap = r::gap_of(file, ByteView(whole).first(std::min<std::size_t>(whole.size(), 0x200)));
        if (g[1] == "-") {
            CB_CHECK(!gap);
        } else {
            CB_CHECK(gap.has_value());
            if (gap) {
                CB_CHECK_EQ(gap->title, g[1]);
                CB_CHECK_EQ(gap->cpu, g[2]);
            }
        }
    }
});

CB_TEST("coverage.reach.gap_of_on_headers_and_extensions", [] {
    const auto golden = golden_rows("gaphead.golden");
    CB_CHECK_EQ(golden.size(), 7u);
    for (const auto& g : golden) {
        const Bytes head = g.size() > 1 ? from_hex_text(g[1]) : Bytes();
        const auto gap = r::gap_of("x" + g[0], head);
        if (g[2] == "-") {
            CB_CHECK(!gap);
        } else {
            CB_CHECK(gap.has_value());
            if (gap) {
                CB_CHECK_EQ(gap->title, g[2]);
                CB_CHECK_EQ(gap->cpu, g[3]);
            }
        }
    }
});

CB_TEST("coverage.reach.spread_matches_the_original", [] {
    const auto golden = golden_rows("spread.golden");
    CB_CHECK_EQ(golden.size(), 14u);
    for (const auto& g : golden) {
        std::vector<r::Row> rows;
        std::size_t i = 0;
        for (const std::string_view s : text::split(g[0], ',')) {
            if (s.empty()) continue;
            r::Row rw;
            rw.size = std::stoull(std::string(s));
            rw.sha = std::format("p{}_{}", s, i++);
            rows.push_back(rw);
        }
        std::vector<r::Row*> ptrs;
        for (r::Row& rw : rows) ptrs.push_back(&rw);
        const long n = std::stol(g[1]);   // a count below nothing asks for the first
        const auto picked = r::spread(ptrs, n < 0 ? 0 : static_cast<std::size_t>(n), std::stoull(g[2]));
        std::string got = "[";
        for (std::size_t k = 0; k < picked.size(); ++k) got += (k ? ", " : "") + ("\"" + picked[k]->sha + "\"");
        got += "]";
        CB_CHECK_EQ(got, g[3]);
    }
});

CB_TEST("coverage.reach.report_matches_the_original_on_its_tables", [] {
    std::size_t seen = 0;
    std::string failures;   // which report, and the first line that differs
    for (const auto& entry : stdfs::directory_iterator(testing::data_path("coverage/testdata/reports"))) {
        if (entry.path().extension() != ".json") continue;
        const stdfs::path md = stdfs::path(entry.path()).replace_extension(".md");
        const std::string want = text::replace_all(fs::read_text(md), "(`python -m codebreaker reach`)", "(`codebreaker reach`)");
        std::vector<r::Row> rows = r::read_results(entry.path());
        if (entry.path().stem() == "big") {   // the harness's `big` case: the rows of `mixed` twice over
            const std::vector<r::Row> once = r::read_results(entry.path());
            rows.insert(rows.end(), once.begin(), once.end());
        }
        const std::string got = r::report(rows);
        if (got != want) {
            const auto a = text::split(got, '\n'), b = text::split(want, '\n');
            std::string at = "(same lines, different length)";
            for (std::size_t k = 0; k < std::max(a.size(), b.size()); ++k) {
                const std::string_view x = k < a.size() ? a[k] : "<end>", y = k < b.size() ? b[k] : "<end>";
                if (x != y) {
                    at = std::format("line {}: new [{}] old [{}]", k + 1, x, y);
                    break;
                }
            }
            failures += fs::to_text(entry.path().filename()) + ": " + at + "\n";
        }
        ++seen;
    }
    CB_CHECK_EQ(failures, "");
    CB_CHECK_EQ(seen, 9u);
});
