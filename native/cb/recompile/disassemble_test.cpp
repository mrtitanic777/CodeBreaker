#include "cb/recompile/disassemble.hpp"

#include <map>
#include <sstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::recompile::disassemble;

namespace {

bool has(std::string_view haystack, std::string_view needle) { return haystack.find(needle) != std::string_view::npos; }

struct GoldenRow {
    std::uint32_t address = 0, word = 0;
    bool vector = false;
    std::string mnemonic, operands;
};

Bytes slice() { return fs::read_file(testing::data_path("recompile/testdata/ppc_slice.bin")); }

// ppc_slice.listing.txt: what the reference gives for the 16 KB of Minecraft code (see testdata/gen_ppc_slice.py).
std::vector<GoldenRow> golden() {
    std::vector<GoldenRow> rows;
    const std::string body = fs::read_text(testing::data_path("recompile/testdata/ppc_slice.listing.txt"));
    for (std::string_view line : text::lines(body)) {
        if (line.empty()) continue;
        const auto parts = text::split_n(line, ' ', 3);
        GoldenRow r;
        r.address = *text::parse_int<std::uint32_t>(parts[0], 16);
        r.word = *text::parse_int<std::uint32_t>(parts[1], 16);
        r.vector = parts[2] == "V";
        const std::string_view rest = parts[3];
        const std::size_t bar = rest.find('|');
        r.mnemonic = std::string(rest.substr(0, bar));
        r.operands = std::string(rest.substr(bar + 1));
        rows.push_back(std::move(r));
    }
    return rows;
}

std::vector<Span> parse_runs(std::string_view line) {
    std::vector<Span> out;
    for (std::string_view run : text::split_ws(line)) {
        const std::size_t dash = run.find('-');
        out.push_back({*text::parse_int<std::size_t>(run.substr(0, dash)), *text::parse_int<std::size_t>(run.substr(dash + 1))});
    }
    return out;
}

VectorDecoder vector_from(const std::vector<GoldenRow>& rows) {
    std::map<std::uint32_t, std::pair<std::string, std::string>> table;   // lookup only
    for (const GoldenRow& r : rows) {
        if (r.vector) table[r.word] = {r.mnemonic, r.operands};
    }
    return [table](std::uint32_t word) -> std::optional<std::pair<std::string, std::string>> {
        const auto it = table.find(word);
        if (it == table.end()) return std::nullopt;
        return it->second;
    };
}

}  // namespace

CB_TEST("recompile.disassemble.vmx128_primary_opcodes", [] {
    CB_CHECK(is_vmx128(0x10000000));
    CB_CHECK(is_vmx128(0x14000000));
    CB_CHECK(is_vmx128(0x1BFFFFFF));
    CB_CHECK(!is_vmx128(0x0FFFFFFF));
    CB_CHECK(!is_vmx128(0x1C000000));
    CB_CHECK(!is_vmx128(0x38000007));
});

CB_TEST("recompile.disassemble.listing_matches_the_reference_on_real_code", [] {
    const Bytes code = slice();
    const std::vector<GoldenRow> rows = golden();
    CB_CHECK_EQ(rows.size(), 4096u);
    Disassembler dis(vector_from(rows));
    const std::vector<Row> got = dis.listing(code, 0x822B6E00);
    CB_CHECK_EQ(got.size(), rows.size());
    std::size_t differing = 0;
    for (std::size_t i = 0; i < got.size() && i < rows.size(); ++i) {
        const bool same = got[i].address == rows[i].address && got[i].word == rows[i].word && got[i].mnemonic == rows[i].mnemonic &&
                          got[i].operands == rows[i].operands;
        if (!same && ++differing <= 3) CB_CHECK(same);   // show the first few
    }
    CB_CHECK_EQ(differing, 0u);
    // without the vector decoder the words it does not decode are Capstone's, and the rest are the same
    Disassembler plain;
    const std::vector<Row> raw = plain.listing(code, 0x822B6E00);
    std::size_t scalar_differing = 0;
    for (std::size_t i = 0; i < raw.size() && i < rows.size(); ++i) {
        if (!rows[i].vector && (raw[i].mnemonic != rows[i].mnemonic || raw[i].operands != rows[i].operands)) ++scalar_differing;
    }
    CB_CHECK_EQ(scalar_differing, 0u);
});

CB_TEST("recompile.disassemble.listing_start_count_and_unreadable_words", [] {
    const Bytes code = slice();
    const std::vector<GoldenRow> rows = golden();
    Disassembler dis(vector_from(rows));
    const auto part = dis.listing(code, 0x822B6E00, 10, 3);   // start rounds down to a word
    CB_CHECK_EQ(part.size(), 3u);
    CB_CHECK_EQ(part[0].address, 0x822B6E08u);
    CB_CHECK_EQ(part[0].mnemonic, rows[2].mnemonic);
    CB_CHECK(dis.listing(code, 0, 0, 0).empty());
    CB_CHECK(dis.listing({}, 0).empty());
    const Bytes three = {0x38, 0x00, 0x00};   // short of a word: nothing
    CB_CHECK(dis.listing(three, 0).empty());
    // a word Capstone does not read is a .long (the golden has 39 of them: all-zero words)
    const Bytes zero(4, 0);
    const auto z = dis.listing(zero, 0x1000);
    CB_CHECK_EQ(z.size(), 1u);
    CB_CHECK_EQ(z[0].mnemonic, ".long");
    CB_CHECK_EQ(z[0].operands, "0x00000000");
    const std::array<std::uint8_t, 4> raw = part[0].raw();
    CB_CHECK_EQ(static_cast<int>(raw[0]), static_cast<int>(code[8]));
});

CB_TEST("recompile.disassemble.flow_reaches_code_and_leaves_the_rest_as_data", [] {
    const Bytes code = slice();
    std::istringstream in(fs::read_text(testing::data_path("recompile/testdata/ppc_slice.reach.txt")));
    std::string key, line;
    std::vector<Span> covered, data;
    while (in >> key) {
        std::getline(in, line);
        if (key == "covered") covered = parse_runs(line);
        if (key == "data") data = parse_runs(line);
    }
    const std::uint32_t entries[] = {0x822B6EA8, 0x822B6E00};
    Disassembler dis;
    const Coverage reached = dis.classify(code, 0x822B6E00, entries);
    CB_CHECK_EQ(reached.size(), code.size());
    std::vector<Span> runs;
    for (std::size_t i = 0; i < reached.size(); ++i) {
        if (!reached[i]) continue;
        if (!runs.empty() && runs.back().end == i) ++runs.back().end;
        else runs.push_back({i, i + 1});
    }
    CB_CHECK_EQ(runs.size(), covered.size());
    for (std::size_t i = 0; i < runs.size() && i < covered.size(); ++i) {
        CB_CHECK_EQ(runs[i].begin, covered[i].begin);
        CB_CHECK_EQ(runs[i].end, covered[i].end);
    }
    const std::vector<Span> spans = dis.data_spans(code, 0x822B6E00, entries);
    CB_CHECK_EQ(spans.size(), data.size());
    for (std::size_t i = 0; i < spans.size() && i < data.size(); ++i) {
        CB_CHECK_EQ(spans[i].begin, data[i].begin);
        CB_CHECK_EQ(spans[i].end, data[i].end);
    }
    // entries outside the code reach nothing
    const std::uint32_t outside[] = {0x10000000};
    const Coverage none = dis.classify(code, 0x822B6E00, outside);
    CB_CHECK(std::none_of(none.begin(), none.end(), [](bool b) { return b; }));
    // a longer minimum run drops the short ones
    const std::vector<Span> big = dis.data_spans(code, 0x822B6E00, entries, 1000);
    CB_CHECK_EQ(big.size(), 3u);   // the runs of 7492, 1008 and 6000 bytes
});

CB_TEST("recompile.disassemble.a_blr_ends_flow_and_a_branch_is_followed", [] {
    // 0: b +8   4: li r3,1 (never reached)   8: li r3,2   C: blr   10: li r3,3 (after the end)
    const Bytes code = {0x48, 0x00, 0x00, 0x08, 0x38, 0x60, 0x00, 0x01, 0x38, 0x60, 0x00, 0x02, 0x4E, 0x80, 0x00, 0x20, 0x38, 0x60, 0x00, 0x03};
    Disassembler dis;
    const std::uint32_t entries[] = {0x1000};
    const Coverage c = dis.classify(code, 0x1000, entries);
    const std::vector<bool> expected = {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0};
    CB_CHECK(c == expected);
    const auto spans = dis.data_spans(code, 0x1000, entries);
    CB_CHECK_EQ(spans.size(), 2u);
    CB_CHECK_EQ(spans[0].begin, 4u);
    CB_CHECK_EQ(spans[1].end, 20u);
}
);

CB_TEST("recompile.disassemble.an_image_is_flattened_as_loaded", [] {
    ImageFacts img;
    img.data = {1, 2, 3, 4, 5, 6, 7, 8};
    img.base = 0x1000;
    img.entry = 0;
    img.code_begin = 0x1000;
    img.code_end = 0x1004;
    img.chunks = {{0x1000, 0, 4}, {0x1008, 4, 4}};
    img.sections = {{".text", 0x1000, 4, 0}, {".data", 0x1008, 4, 0}, {".rdata", 0x1010, 4, 0x20000000}};
    const FlatImage flat = flatten(img);
    CB_CHECK((flat.bytes == Bytes{1, 2, 3, 4, 0, 0, 0, 0, 5, 6, 7, 8}));   // the gap is zero
    CB_CHECK_EQ(flat.entry, 0x1000u);                                      // no entry: the start of the code
    CB_CHECK(flat.sections[0].executable && !flat.sections[1].executable && flat.sections[2].executable);
    const CodeSection cs = code_section(img);
    CB_CHECK((cs.code == Bytes{1, 2, 3, 4}));
    CB_CHECK_EQ(cs.address, 0x1000u);
    img.entry = 0x1002;
    CB_CHECK_EQ(code_section(img).entry, 0x1002u);
    ImageFacts empty;
    CB_CHECK_THROWS_AS(FormatError, (void)flatten(empty));
    img.chunks.push_back({0x1000, 6, 100});   // a chunk the file cannot hold
    CB_CHECK_THROWS_AS(FormatError, (void)flatten(img));
});

CB_TEST("recompile.disassemble.the_command_lists_words_and_marks_data", [] {
    const Bytes code = slice();
    const ImageReader reader = [&](const std::filesystem::path&) {
        ImageFacts img;
        img.data = code;
        img.base = 0x822B6E00;
        img.entry = 0x822B6EA8;
        img.code_begin = 0x822B6E00;
        img.code_end = 0x822B6E00 + static_cast<std::uint32_t>(code.size());
        img.chunks = {{0x822B6E00, 0, static_cast<std::uint32_t>(code.size())}};
        return img;
    };
    testing::CapturedStd captured;
    CB_CHECK_EQ(main(reader, {}, std::vector<std::string>{"dir/default.xex", "--limit", "3"}), 0);
    const std::string out = captured.out();
    CB_CHECK(out.starts_with("default.xex  code at 0x822B6E00, entry 0x822B6EA8, 16384 bytes\n"));
    CB_CHECK(has(out, "    822B6E00  91410098  stw        r10, 0x98(r1)\n"));
    CB_CHECK(has(out, "  --- 16384 bytes, 14840 read as data (90.6%)"));   // the reference: 1544 bytes covered
    captured.out();
    CB_CHECK_EQ(main(reader, {}, std::vector<std::string>{}), kExitUsage);   // no image: usage
});
