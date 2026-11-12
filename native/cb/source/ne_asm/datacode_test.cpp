// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/ne_asm/datacode: the two detectors on bytes and listings written out by hand (the expected regions were
// worked out from the rules, not recorded from a run), the merge of the two, the report, and the command on the real module
// LZEXPAND.DLL with a listing of its own (canned, as objdump would print it: objdump is not needed).
//
// DIVERGES from the Python module, by design: the command line is CodeBreaker's (`--seg`, `--ords`, no argparse abbreviations or
// wording); a segment that does not exist is a UsageError (the Python indexed a list); the regions carry a kind enum, and the
// preview of a region that ends past the segment stops at the segment's end.
#include "cb/source/ne_asm/datacode.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"

namespace dc = cb::source::ne_asm::datacode;
namespace sg = cb::source::ne_asm::segment;
using cb::Bytes;

namespace {

Bytes bytes_text(std::string_view s) { return Bytes(s.begin(), s.end()); }

std::vector<sg::Row> make_rows(std::initializer_list<std::pair<std::uint32_t, const char*>> list) {
    std::vector<sg::Row> rows;
    for (const auto& [offset, text] : list) rows.push_back({offset, text});
    return rows;
}

std::string kinds(const std::vector<dc::Region>& regions) {
    std::string out;
    for (const dc::Region& r : regions) out += std::format("{}-{}:{} ", r.start, r.end, dc::kind_name(r.kind));
    return out;
}

}  // namespace

CB_TEST("source.ne_asm.datacode.strings_are_runs_of_text", [] {
    // "primary\0" at 1; "ab" is too short; "HELLO" has no lowercase letter (a register push/pop run or a table, not text);
    // a tab is printable; a run at the very end has no NUL to eat
    Bytes code = {0x00};
    for (char c : std::string_view("primary")) code.push_back(static_cast<std::uint8_t>(c));
    for (const int b : std::initializer_list<int>{0x00, 'a', 'b', 0x00, 'H', 'E', 'L', 'L', 'O', 0x00, 'a', '\t', 'b', 'c', 'd', 0x01, 'e', 'n', 'd', 's', '!'}) {
        code.push_back(static_cast<std::uint8_t>(b));
    }
    const auto found = dc::find_strings(code);
    CB_CHECK_EQ(found.size(), 3U);
    CB_CHECK_EQ(found[0].start, 1U);
    CB_CHECK_EQ(found[0].end, 9U);   // the NUL is eaten
    CB_CHECK_EQ(found[0].text, "primary");
    CB_CHECK_EQ(found[1].text, "a\tbcd");
    CB_CHECK_EQ(found[1].end, found[1].start + 5);   // no NUL after it (0x01)
    CB_CHECK_EQ(found[2].text, "ends!");
    CB_CHECK_EQ(found[2].end, static_cast<std::uint32_t>(code.size()));
    CB_CHECK_EQ(dc::find_strings(code, 2).size(), 4U);   // "ab" is now long enough; "HELLO" is still not text
});

CB_TEST("source.ne_asm.datacode.no_strings_in_nothing", [] {
    CB_CHECK(dc::find_strings({}).empty());
    CB_CHECK(dc::find_strings(bytes_text("abcd")).empty());   // four bytes: below the minimum
    CB_CHECK_EQ(dc::find_strings(bytes_text("abcde")).size(), 1U);
    CB_CHECK(dc::find_strings(bytes_text("12345678")).empty());   // printable but no lowercase letter
});

CB_TEST("source.ne_asm.datacode.garbage_is_a_cluster_of_nonsense_instructions", [] {
    const auto rows = make_rows({
        {0, "push   bp"},
        {1, "outs   dx,BYTE PTR ds:[si]"},   // data mnemonic
        {2, "arpl   WORD PTR [bx+si],ax"},    // data mnemonic
        {3, "(bad)"},                         // undecodable
        {4, "mov    ax,bx"},                  // ends the run: [1,4) is three rows, ending where row 4 starts
        {5, "mov    eax,0x1"},                // 32-bit register in 16-bit code
        {6, "ds     lea ax,[bx]"},            // a segment prefix with an operand
        {7, "mov    esp,ebp"},
        {8, "ret"},                           // [5,8)
        {9, "mov    ax,[bx+eaxx]"},           // "eaxx" is not a register: a word, not a substring
        {10, "es"},                           // a prefix alone is not garbage
        {20, "hlt"},                          // a run of two
        {21, "sahf"},
        {22, "nop"},
        {30, "lds    si,[bx]"},               // a run at the very end: it ends at its last row
        {31, "les    di,[bx]"},
        {32, "xlat"},
    });
    CB_CHECK_EQ(kinds(dc::garbage_spans(rows)), "1-4:data 5-8:data 30-32:data ");
});

CB_TEST("source.ne_asm.datacode.garbage_ignores_the_order_and_repeats_of_rows", [] {
    const auto rows = make_rows({{3, "into"}, {1, "daa"}, {2, "das"}, {0, "nop"}, {1, "aaa"}, {4, "nop"}});
    CB_CHECK_EQ(kinds(dc::garbage_spans(rows)), "1-4:data ");   // offset 1 met twice: the later text; sorted by offset
    CB_CHECK(dc::garbage_spans({}).empty());
});

CB_TEST("source.ne_asm.datacode.regions_merge_and_keep_the_first_kind", [] {
    // a string at [0,10) ("hello world" up to the 0x01), a garbage run at [8,14) overlapping it, a string at [14,20) touching it,
    // a lone garbage run at [30,33)
    Bytes code = bytes_text("hello wor\0");
    code.resize(14, 0x01);
    for (char c : std::string_view("lower!")) code.push_back(static_cast<std::uint8_t>(c));
    code.resize(40, 0x01);
    const auto rows = make_rows({{7, "nop"}, {8, "hlt"}, {10, "sahf"}, {12, "lahf"}, {14, "nop"}, {30, "hlt"}, {31, "hlt"}, {32, "hlt"}, {33, "nop"}});
    // strings: [0,10) and [14,20); garbage: [8,14) and [30,33)
    CB_CHECK_EQ(kinds(dc::data_regions(code, rows)), "0-20:str 30-33:data ");
});

CB_TEST("source.ne_asm.datacode.on_a_tie_the_data_region_comes_first", [] {
    // a string and a garbage run over the same bytes: the merged region keeps the kind of the one sorted first, Data
    const Bytes code = bytes_text("abcdef");
    const auto rows = make_rows({{0, "hlt"}, {1, "hlt"}, {2, "hlt"}, {6, "nop"}});
    CB_CHECK_EQ(kinds(dc::data_regions(code, rows)), "0-6:data ");
});

CB_TEST("source.ne_asm.datacode.the_report_lists_regions_with_a_preview", [] {
    Bytes code = bytes_text("\x01primary\x01");
    code[8] = 0;
    code.resize(30, 0x90);
    const std::vector<dc::Region> regions = dc::data_regions(code, {});
    CB_CHECK_EQ(regions.size(), 1U);
    const std::string text = dc::report("MOUSE", 1, code, regions);
    CB_CHECK_EQ(text, "MOUSE seg1: 1 data region(s), 8 bytes (26.7% of 30)\n  0001-0009  str     8B  |primary.|\n");
    // nothing found: the summary alone; an empty segment does not divide by zero
    CB_CHECK_EQ(dc::report("X", 2, {}, {}), "X seg2: 0 data region(s), 0 bytes (0.0% of 0)\n");
});

CB_TEST("source.ne_asm.datacode.the_report_stops_at_forty_regions_and_shows_24_bytes", [] {
    std::vector<dc::Region> regions;
    for (std::uint32_t k = 0; k < 45; ++k) regions.push_back({k * 100, k * 100 + 50, dc::Kind::String});
    Bytes code(5000, 'x');
    const std::string text = dc::report("M", 3, code, regions);
    std::size_t lines = 0;
    for (char c : text) lines += c == '\n';
    CB_CHECK_EQ(lines, 41U);   // the summary and 40 regions
    CB_CHECK(text.find("  0000-0032  str    50B  |xxxxxxxxxxxxxxxxxxxxxxxx|\n") != std::string::npos);
    // a region that reaches past the end of the segment shows what there is
    const std::string clipped = dc::report("M", 1, bytes_text("abc"), std::vector<dc::Region>{{1, 99, dc::Kind::Data}});
    CB_CHECK(clipped.find("|bc|") != std::string::npos);
});

namespace {

// A listing of a segment of `size` bytes in which the rows at 0x10..0x12 are garbage.
std::string canned_listing(cb::ByteView code, std::uint32_t) {
    std::string out = "\nDisassembly of section .data:\n\n";
    for (std::uint32_t at = 0; at < code.size(); ++at) {
        const char* text = (at >= 0x10 && at <= 0x12) ? "hlt" : "nop";
        out += std::format("{:4x}:\t{:02x}      \t{}\n", at, code[at], text);
    }
    return out;
}

// A corpus that holds nothing: the module is named by path.
struct World {
    cb::fs::ScopedTempDir dir{"cbdatacode"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    std::optional<cb::fs::ScopedEnv> root_env;
    std::unique_ptr<cb::base::Corpus> corpus;
    World() {
        cb::fs::make_dirs(dir.path() / "corpus");
        root_env.emplace("CB_CORPUS_ROOT", cb::fs::to_text(dir.path() / "corpus"));
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = dir.path() / "corpus";
        products.checkout_root = dir.path() / "CodeBreaker";
        products.state = dir.path() / "state";
        corpus = std::make_unique<cb::base::Corpus>(products);
    }
};

}  // namespace

CB_TEST("source.ne_asm.datacode.the_command_reports_a_real_segment", [] {
    World world;
    const std::string lz = cb::fs::to_text(cb::testing::data_path("formats/testdata/lzexpand.dll"));
    const std::string ords = cb::fs::to_text(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md"));
    cb::testing::CapturedStd captured;
    CB_CHECK_EQ(dc::main(*world.corpus, std::vector<std::string>{lz, "--seg", "1", "--ords", ords}, canned_listing), cb::kExitOk);
    const std::string out = captured.out();
    // the garbage rows 0x10..0x12 are the one cluster the canned listing has; strings come from the real bytes
    CB_CHECK(out.find(" data region(s), ") != std::string::npos);
    CB_CHECK(out.find("  0010-0013  data") != std::string::npos);
    CB_CHECK(out.starts_with(lz + " seg1: "));
});

CB_TEST("source.ne_asm.datacode.the_command_rejects_what_it_cannot_run", [] {
    World world;
    const std::string lz = cb::fs::to_text(cb::testing::data_path("formats/testdata/lzexpand.dll"));
    const std::string ords = cb::fs::to_text(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md"));
    CB_CHECK_THROWS_AS(cb::UsageError, dc::main(*world.corpus, std::vector<std::string>{lz, "--seg", "99", "--ords", ords}, canned_listing));
    CB_CHECK_THROWS_AS(cb::UsageError, dc::main(*world.corpus, std::vector<std::string>{lz, "--seg", "x", "--ords", ords}, canned_listing));
    CB_CHECK_EQ(dc::main(*world.corpus, std::vector<std::string>{}, canned_listing), cb::kExitUsage);
});
