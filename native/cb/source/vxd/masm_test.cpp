// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/masm. The text of one instruction, the body of an object and the repair loop are checked on a recovery made by
// hand (instructions, labels and bytes written out below over the object table of the real driver splitter.vxd: the expected text was
// worked out from the emission rules, not recorded from a run); the assemblers are stand-ins that read the MASM text the module writes
// (a perfect one, and one whose encoding of a chosen instruction is a byte longer than the original's, as MASM's is where it re-encodes),
// and a COFF object written by hand carries the symbols. x86asm, and the real ML where installed, are used. The listing of a real driver
// (splitter.vxd) goes through the module end to end.
//
// Behaviour pinned here: the assemblers' files get the line ends the text has (a text-mode write would turn
// the CRLF of the text into CR CR LF); the DOS lane is the caller's (`Toolchain::period`) and not found here; a COFF object that is cut
// short is a FormatError.
#include "cb/source/vxd/masm.hpp"

#include <format>
#include <optional>
#include <ranges>
#include <type_traits>
#include <functional>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace vx = cb::source::vxd;
namespace mx = cb::source::vxd::masm;
namespace le = cb::formats::le;
using cb::Bytes;

namespace {

// ---- values as text, for the failure messages of CHECK_SAME (the in-tree show() cannot format a container) ----

std::string rendered(std::string_view s);
template <class T> requires(std::is_arithmetic_v<T>) std::string rendered(T v);
std::string rendered(const vx::Place& p);
template <class A, class B> std::string rendered(const std::pair<A, B>& p);
template <class T> std::string rendered(const std::optional<T>& v);
template <class R> requires(std::ranges::range<R> && !std::is_convertible_v<const R&, std::string_view>) std::string rendered(const R& range);

std::string rendered(std::string_view s) { return "\"" + std::string(s) + "\""; }
template <class T> requires(std::is_arithmetic_v<T>) std::string rendered(T v) { return std::to_string(v); }
std::string rendered(const vx::Place& p) { return std::format("({}, {:#x})", p.object, p.offset); }
template <class A, class B> std::string rendered(const std::pair<A, B>& p) { return "(" + rendered(p.first) + ", " + rendered(p.second) + ")"; }
template <class T> std::string rendered(const std::optional<T>& v) { return v ? rendered(*v) : std::string("none"); }
template <class R> requires(std::ranges::range<R> && !std::is_convertible_v<const R&, std::string_view>) std::string rendered(const R& range) {
    std::string out = "[";
    for (const auto& item : range) out += (out.size() > 1 ? ", " : "") + rendered(item);
    return out + "]";
}

#define CHECK_SAME(actual, expected) CB_CHECK_EQ(rendered(actual), rendered(expected))


le::Module splitter() { return le::Module::open(cb::testing::data_path("formats/testdata/splitter.vxd")); }

vx::Instruction insn(std::uint32_t size, std::string mnemonic, std::string operands, Bytes raw) {
    vx::Instruction i;
    i.size = size;
    i.mnemonic = std::move(mnemonic);
    i.operands = std::move(operands);
    i.raw = std::move(raw);
    return i;
}

// A recovery over object 1: push ebp / mov ebp,esp / jmp 0xc / seven data bytes / loc_1_000C: pop ebp / ret.
struct Hand {
    le::Module driver = splitter();
    vx::DdkNames names;
    mx::EmissionParts parts;
    Hand() {
        parts.code = {{0x55, 0x8B, 0xEC, 0xEB, 0x07, 1, 2, 3, 4, 5, 6, 7, 0x5D, 0xC3}};
        auto& i = parts.instructions;
        i[{1, 0}] = insn(1, "push", "ebp", {0x55});
        i[{1, 1}] = insn(2, "mov", "ebp, esp", {0x8B, 0xEC});
        i[{1, 3}] = insn(2, "jmp", "0xc", {0xEB, 0x07});
        i[{1, 12}] = insn(1, "pop", "ebp", {0x5D});
        i[{1, 13}] = insn(1, "ret", "", {0xC3});
        parts.labels[{1, 12}] = "loc_1_000C";
    }
    mx::Emission emission() { return mx::Emission(driver, names, parts); }
};

// A COFF object with one code section and the symbols given (section-relative, defined in section 1).
Bytes coff(const Bytes& text, const std::vector<std::pair<std::string, std::uint32_t>>& symbols) {
    cb::io::ByteWriter w;
    const std::uint32_t raw_pointer = 20 + 40;
    const std::uint32_t symbol_pointer = raw_pointer + static_cast<std::uint32_t>(text.size());
    w.u16(0x14C);
    w.u16(1);
    w.u32(0);
    w.u32(symbol_pointer);
    w.u32(static_cast<std::uint32_t>(symbols.size()));
    w.u16(0);
    w.u16(0);
    for (const char c : std::string(".text\0\0\0", 8)) w.u8(static_cast<std::uint8_t>(c));
    w.u32(0);
    w.u32(0);
    w.u32(static_cast<std::uint32_t>(text.size()));
    w.u32(raw_pointer);
    w.u32(0);
    w.u32(0);
    w.u16(0);
    w.u16(0);
    w.u32(0x60000020);   // code, execute, read
    w.bytes(text);
    Bytes strings(4, 0);
    for (const auto& [name, value] : symbols) {
        if (name.size() <= 8) {
            for (std::size_t k = 0; k < 8; ++k) w.u8(k < name.size() ? static_cast<std::uint8_t>(name[k]) : 0);
        } else {
            w.u32(0);
            w.u32(static_cast<std::uint32_t>(strings.size()));
            for (const char c : name) strings.push_back(static_cast<std::uint8_t>(c));
            strings.push_back(0);
        }
        w.u32(value);
        w.u16(1);
        w.u16(0);
        w.u8(2);
        w.u8(0);
    }
    const std::uint32_t total = static_cast<std::uint32_t>(strings.size());
    for (std::size_t k = 0; k < 4; ++k) strings[k] = static_cast<std::uint8_t>(total >> (8 * k));
    w.bytes(strings);
    return w.take();
}

}  // namespace

CB_TEST("source.vxd.masm.operands_are_written_in_masm_hex", [] {
    CHECK_SAME(mx::masm_operands("eax, 0x1f"), "eax, 1fh");
    CHECK_SAME(mx::masm_operands("eax, 0xf1"), "eax, 0f1h");   // a number that starts with a letter takes a zero
    CHECK_SAME(mx::masm_operands("0x10"), "10h");
    CHECK_SAME(mx::masm_operands("[ebx+0xa0], 0xFF"), "[ebx+0a0h], 0FFh");
    CHECK_SAME(mx::masm_operands("dword ptr [0x0]"), "dword ptr [0h]");
    CHECK_SAME(mx::masm_operands("ax, 12"), "ax, 12");
    CHECK_SAME(mx::masm_operands("0x"), "0x");        // no digits: left alone
    CHECK_SAME(mx::masm_operands("0xg"), "0xg");
    CHECK_SAME(mx::masm_operands("loc_0x1"), "loc_1h");   // anywhere in the operand, as the pattern was
    CHECK_SAME(mx::masm_operands(""), "");
});

CB_TEST("source.vxd.masm.branches_and_the_encodings_masm_cannot_keep", [] {
    for (const char* name : {"call", "jmp", "je", "jnz", "jcxz", "jecxz", "loop", "loopne"}) CB_CHECK(mx::is_branch(name));
    for (const char* name : {"jmpe", "mov", "ret", "j", ""}) CB_CHECK(!mx::is_branch(name));

    CB_CHECK(mx::has_absolute_memory(insn(5, "mov", "eax, dword ptr [0x1234]", {})));
    CB_CHECK(mx::has_absolute_memory(insn(5, "add", "[ 12 ], eax", {})));
    CB_CHECK(mx::has_absolute_memory(insn(5, "mov", "al, byte ptr [0]", {})));
    CB_CHECK(!mx::has_absolute_memory(insn(3, "mov", "eax, [ebx]", {})));
    CB_CHECK(!mx::has_absolute_memory(insn(3, "mov", "eax, [ebx+0x10]", {})));
    CB_CHECK(!mx::has_absolute_memory(insn(3, "mov", "eax, [0x1234+eax]", {})));
    CB_CHECK(!mx::has_absolute_memory(insn(3, "mov", "eax, [0x]", {})));
    CB_CHECK(!mx::has_absolute_memory(insn(3, "mov", "eax, 0x1234", {})));

    for (const int prefix : {0x2E, 0x26, 0x36, 0x3E, 0x64, 0x65}) CB_CHECK(mx::has_segment_prefix(insn(3, "mov", "", {static_cast<std::uint8_t>(prefix), 0x8B, 0x00})));
    CB_CHECK(!mx::has_segment_prefix(insn(3, "mov", "", {0x66, 0x8B, 0x00})));
    CB_CHECK(!mx::has_segment_prefix(insn(1, "ret", "", {})));
});

CB_TEST("source.vxd.masm.a_db_line_is_hex_with_a_leading_zero", [] {
    CHECK_SAME(mx::db_line(Bytes{0xAA, 0x01, 0x00}), "        db  0AAh, 001h, 000h");
    CHECK_SAME(mx::db_line(Bytes{0x7F}), "        db  07Fh");
});

CB_TEST("source.vxd.masm.one_instruction_as_text", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    const auto line = [&](std::uint32_t at) { return mx::masm_line(code, 1, code.instructions().at({1, at})); };
    CHECK_SAME(*line(0), "        push   ebp");
    CHECK_SAME(*line(1), "        mov    ebp, esp");
    CHECK_SAME(*line(3), "        jmp    SHORT loc_1_000C");   // a 2-byte jmp to a label is pinned short
    // a near jump (5 bytes) is pinned near
    hand.parts.instructions[{1, 3}] = insn(5, "jmp", "0xc", {0xE9, 0, 0, 0, 0});
    const mx::Emission near = hand.emission();
    CHECK_SAME(*mx::masm_line(near, 1, near.instructions().at({1, 3})), "        jmp    NEAR PTR loc_1_000C");
    // a call is never sized; a direct branch to an unlabelled address cannot be written (MASM rejects `jmp <number>`)
    hand.parts.instructions[{1, 3}] = insn(5, "call", "0xc", {0xE8, 0, 0, 0, 0});
    const mx::Emission call = hand.emission();
    CHECK_SAME(*mx::masm_line(call, 1, call.instructions().at({1, 3})), "        call   loc_1_000C");
    hand.parts.instructions[{1, 3}] = insn(5, "call", "0x99", {0xE8, 0, 0, 0, 0});
    const mx::Emission lost = hand.emission();
    CB_CHECK(!mx::masm_line(lost, 1, lost.instructions().at({1, 3})).has_value());
    // capstone's far branch, and the string operations (`lodsb al, [esi]` is `lodsb`; a pushal is a pushad)
    CB_CHECK(!mx::masm_line(code, 1, insn(7, "ljmp", "0x8:0x10", {})).has_value());
    CHECK_SAME(*mx::masm_line(code, 1, insn(2, "lodsb", "al, byte ptr [esi]", {0xAC})), "        lodsb");
    CHECK_SAME(*mx::masm_line(code, 1, insn(2, "rep movsd", "dword ptr es:[edi], dword ptr [esi]", {0xF3, 0xA5})), "        rep movsd");
    CHECK_SAME(*mx::masm_line(code, 1, insn(1, "pushal", "", {0x60})), "        pushad");
});

CB_TEST("source.vxd.masm.the_body_of_an_object", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    const mx::Body body = mx::object_body(code, 1);
    const std::vector<std::string> expected = {
        "        push   ebp",
        "        mov    ebp, esp",
        "        jmp    SHORT loc_1_000C",
        "        db  001h, 002h, 003h, 004h, 005h, 006h, 007h",   // the seven bytes between the jmp and its target are data
        "loc_1_000C:",
        "        pop    ebp",
        "        ret",
    };
    CHECK_SAME(body.lines, expected);
    // the symbolic lines, 1-based: push at 1, mov at 2, jmp at 3, pop at 6, ret at 7
    const std::map<std::size_t, std::uint32_t> lines = {{1, 0}, {2, 1}, {3, 3}, {6, 12}, {7, 13}};
    CHECK_SAME(body.line_map, lines);

    // a forced instruction is raw; a base line offsets the map; probe labels come before every instruction
    mx::BodyOptions options;
    options.force_raw = {1};
    options.base_line = 3;
    options.probe = true;
    const mx::Body probed = mx::object_body(code, 1, options);
    CHECK_SAME(probed.lines.at(0), "public Q0");
    CHECK_SAME(probed.lines.at(1), "Q0:");
    CHECK_SAME(probed.lines.at(2), "        push   ebp");
    CHECK_SAME(probed.lines.at(5), "        db  08Bh, 0ECh");   // mov ebp,esp, forced
    CHECK_SAME(probed.line_map.at(3 + 3), 0U);   // push is the third line of the body
    CB_CHECK(!probed.line_map.contains(3 + 6));    // the forced one is not mapped
    // a range emits only its part
    mx::BodyOptions part;
    part.range = std::make_pair(std::uint32_t{12}, std::uint32_t{14});
    const std::vector<std::string> tail = {"loc_1_000C:", "        pop    ebp", "        ret"};
    CHECK_SAME(mx::object_body(code, 1, part).lines, tail);
});

CB_TEST("source.vxd.masm.an_extra_label_inside_an_instruction_splits_it", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    mx::BodyOptions options;
    options.extra_labels = {{2, "VXD_DDB"}};   // the second byte of `mov ebp,esp`
    const std::vector<std::string> expected = {"        push   ebp", "        db  08Bh", "public VXD_DDB", "VXD_DDB label byte", "        db  0ECh",
                                               "        jmp    SHORT loc_1_000C", "        db  001h, 002h, 003h, 004h, 005h, 006h, 007h",
                                               "loc_1_000C:", "        pop    ebp", "        ret"};
    CHECK_SAME(mx::object_body(code, 1, options).lines, expected);
    // a label at an instruction start is defined before it, and data stops at an extra label
    options.extra_labels = {{0, "TOP"}, {8, "MID"}};
    const mx::Body body = mx::object_body(code, 1, options);
    CHECK_SAME(body.lines.at(0), "public TOP");
    CHECK_SAME(body.lines.at(1), "TOP label byte");
    CB_CHECK(std::find(body.lines.begin(), body.lines.end(), "        db  001h, 002h, 003h") != body.lines.end());   // up to byte 8
    CB_CHECK(std::find(body.lines.begin(), body.lines.end(), "MID label byte") != body.lines.end());
});

CB_TEST("source.vxd.masm.an_object_as_a_segment", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    const mx::ObjectSource source = mx::emit_object(code, 1);
    CB_CHECK(source.text.starts_with(".386p\r\n_LTEXT segment dword public use32 'CODE'\r\n\r\n        push   ebp\r\n"));
    CB_CHECK(source.text.ends_with("        ret\r\n_LTEXT ends\r\n        end\r\n"));
    CHECK_SAME(source.line_map.at(4), 0U);   // three header lines, push is source line 4
    CHECK_SAME(mx::emit_object(code, 1, "SEG2").text.substr(7, 29), "SEG2 segment dword public use");
});

CB_TEST("source.vxd.masm.ranges_cut_at_function_starts", [] {
    Hand hand;
    for (std::uint32_t k = 0; k < 10; ++k) hand.parts.instructions[{1, 100 + 4 * k}] = insn(4, "nop", "", {0x90, 0x90, 0x90, 0x90});
    hand.parts.labels[{1, 120}] = "sub_1_0078";
    hand.parts.labels[{1, 132}] = "loc_1_0084";
    hand.parts.code[0].resize(150);
    const mx::Emission code = hand.emission();
    using Ranges = std::vector<std::pair<std::uint32_t, std::uint32_t>>;
    CHECK_SAME(mx::split_ranges(code, 1, 6000), (Ranges{{0, 150}}));
    // 5 instructions of the hand recovery and 10 nops; after 7 instructions a cut falls at the next `sub_` label (offset 120, the
    // nop that is the ninth instruction of the object): the first range holds 0..120, the second the rest
    CHECK_SAME(mx::split_ranges(code, 1, 7), (Ranges{{0, 120}, {120, 150}}));
    // the label at 132 is not a function start; a label before the count is reached cuts nothing
    CHECK_SAME(mx::split_ranges(code, 1, 100), (Ranges{{0, 150}}));
});

CB_TEST("source.vxd.masm.a_branch_that_leaves_the_range_is_forced", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    CB_CHECK(mx::cross_range_force(code, 1, 0, 14).empty());   // the jmp's target (12) is in
    CHECK_SAME(mx::cross_range_force(code, 1, 0, 12), (std::set<std::uint32_t>{3}));   // out of [0,12)
    CHECK_SAME(mx::cross_range_force(code, 1, 12, 14), (std::set<std::uint32_t>{}));   // the jmp is not in the range at all
});

CB_TEST("source.vxd.masm.a_coff_object_gives_its_code_and_symbols", [] {
    const Bytes text = {1, 2, 3, 4, 5, 6};
    const Bytes object = coff(text, {{"Q0", 0}, {"Q3", 3}, {"a_symbol_with_a_long_name", 5}});
    const mx::CoffText found = mx::coff_text(object);
    CHECK_SAME(found.text, text);
    CHECK_SAME(found.symbols.size(), 3U);
    CHECK_SAME(found.symbols.at("Q3"), 3U);
    CHECK_SAME(found.symbols.at("a_symbol_with_a_long_name"), 5U);   // from the string table
    // cut short: a FormatError, at every structure
    for (const std::size_t keep : {std::size_t{0}, std::size_t{3}, std::size_t{12}, std::size_t{30}, object.size() - 40}) {
        CB_CHECK_THROWS_AS(cb::FormatError, (void)mx::coff_text(cb::ByteView(object).first(keep)));
    }
    CB_CHECK_THROWS_AS(cb::FormatError, (void)mx::coff_text(Bytes(20, 0)));   // no sections
});

namespace {

// A stand-in for the assembler that reads the MASM text. Labels `Q<hex>:` are the offsets; a `db` line is its bytes; any other line is
// the instruction at the last label, written as the original's bytes -- except `drifting`, which comes out in its mirror encoding
// (8B/89: the choice MASM makes for `mov r,r`).
struct FakeMasm {
    const std::map<std::uint32_t, cb::Bytes>* originals;   // instruction offset -> its bytes
    std::optional<std::uint32_t> drifting;
    std::size_t* runs = nullptr;
    mx::Assembled operator()(std::string_view source) const {
        if (runs) ++*runs;
        Bytes text;
        std::vector<std::pair<std::string, std::uint32_t>> symbols;
        std::optional<std::uint32_t> at;
        bool in_segment = false;
        for (const std::string_view line : cb::text::lines(source)) {
            if (line.find("segment") != std::string_view::npos) {
                in_segment = true;
            } else if (line.starts_with("Q") && line.ends_with(":")) {
                at = static_cast<std::uint32_t>(*cb::text::parse_int<std::uint64_t>(line.substr(1, line.size() - 2), 16));
                symbols.emplace_back(std::string(line.substr(0, line.size() - 1)), static_cast<std::uint32_t>(text.size()));
            } else if (line.starts_with("        db  ")) {
                for (const std::string_view item : cb::text::split(line.substr(12), ',')) {
                    const std::string_view hex = cb::text::trim(item);
                    text.push_back(static_cast<std::uint8_t>(*cb::text::parse_int<unsigned>(hex.substr(1, hex.size() - 2), 16)));
                }
            } else if (in_segment && line.starts_with("        ") && line != "        end" && at) {
                Bytes bytes = originals->at(*at);
                if (drifting == at) bytes[0] ^= 0x02;
                for (const std::uint8_t b : bytes) text.push_back(b);
            }
        }
        return {coff(text, symbols), ""};
    }
};

// The fake as a Toolchain: the period assembler.
mx::Toolchain fake_toolchain(const FakeMasm& fake) {
    mx::Toolchain tools;
    tools.period = [fake](std::span<const std::string> texts) {
        std::vector<mx::Assembled> out;
        for (const std::string& t : texts) out.push_back(fake(t));
        return out;
    };
    return tools;
}

std::map<std::uint32_t, Bytes> originals_of(const mx::Emission& code) {
    std::map<std::uint32_t, Bytes> out;
    for (const auto& [at, i] : code.instructions()) out[static_cast<std::uint32_t>(at.offset)] = i.raw;
    return out;
}

}  // namespace

CB_TEST("source.vxd.masm.a_perfect_assembler_leaves_every_instruction_symbolic", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    const auto originals = originals_of(code);
    std::size_t runs = 0;
    const mx::Toolchain tools = fake_toolchain({&originals, std::nullopt, &runs});
    const mx::ObjectReport report = mx::compare_object(code, 1, tools);
    CB_CHECK(report.match);
    CHECK_SAME(report.symbolic, 5U);   // push, mov, jmp, pop, ret
    CHECK_SAME(report.forced_raw, 0U);
    CB_CHECK(!report.all_raw);
    CHECK_SAME(report.original_length, 14U);
    CHECK_SAME(report.assembled, report.original);
    CHECK_SAME(report.files, 1U);
    CHECK_SAME(runs, 1U);   // one assembly was enough
});

CB_TEST("source.vxd.masm.the_repair_loop_byte_preserves_only_what_drifts", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    const auto originals = originals_of(code);
    std::size_t runs = 0;
    // MASM encodes `mov ebp,esp` (offset 1) in the mirror form (89 instead of 8B): the first pass finds it through the label of
    // the instruction, whose bytes differ at the place the label says; the instruction is byte-preserved and the second pass is exact
    const mx::Toolchain tools = fake_toolchain({&originals, 1U, &runs});
    const mx::ObjectReport report = mx::compare_object(code, 1, tools);
    CB_CHECK(report.match);
    CHECK_SAME(report.force_raw, (std::set<std::uint32_t>{1}));
    CHECK_SAME(report.symbolic, 4U);
    CHECK_SAME(report.forced_raw, 1U);
    CHECK_SAME(runs, 2U);
    CHECK_SAME(report.assembled, report.original);
});

CB_TEST("source.vxd.masm.a_range_that_never_converges_falls_back_to_raw", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    const auto originals = originals_of(code);
    // an assembler that re-encodes the first instruction: with max_repair = 1 the loop gives up after the first pass and the whole range
    // is byte-preserved
    const mx::Toolchain tools = fake_toolchain({&originals, 0U, nullptr});
    const mx::RangeResult r = mx::converge_range(code, 1, tools, 0, 14, code.code(1), 1);
    CB_CHECK(r.match);   // the all-raw assembly is the original
    CHECK_SAME(r.symbolic, 0U);
    CHECK_SAME(r.raw, 5U);
    CHECK_SAME(r.text, Bytes(code.code(1).begin(), code.code(1).end()));
});

CB_TEST("source.vxd.masm.an_assembler_error_names_the_instruction_to_preserve", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    const auto originals = originals_of(code);
    // the first assembly fails with ML's message for source line 5 (the line of `mov` in the probe source); the instruction is forced and
    // the next assembly goes through
    std::size_t runs = 0;
    mx::Toolchain tools;
    tools.period = [&](std::span<const std::string> texts) {
        std::vector<mx::Assembled> out;
        for (const std::string& t : texts) {
            ++runs;
            if (t.find("        mov    ebp, esp") != std::string::npos) {
                std::size_t line = 1;
                for (const std::string_view l : cb::text::lines(t)) {
                    if (l == "        mov    ebp, esp") break;
                    ++line;
                }
                out.push_back({std::nullopt, std::format("m.asm({}): error A2009: syntax error in expression\n", line)});
            } else {
                out.push_back(FakeMasm{&originals, std::nullopt, nullptr}(t));
            }
        }
        return out;
    };
    const mx::ObjectReport report = mx::compare_object(code, 1, tools);
    CB_CHECK(report.match);
    CHECK_SAME(report.force_raw, (std::set<std::uint32_t>{1}));
    CHECK_SAME(runs, 2U);
});

CB_TEST("source.vxd.masm.a_module_is_compared_object_by_object", [] {
    Hand hand;
    const mx::Emission code = hand.emission();
    const auto originals = originals_of(code);
    const mx::Toolchain tools = fake_toolchain({&originals, std::nullopt, nullptr});
    // splitter.vxd's own objects: those that are executable are compared against the (hand) code of object 1 only, so the other
    // executable objects, which have no code in the hand recovery, are empty ranges and trivially exact
    const mx::ModuleReport report = mx::compare_module(code, tools);
    CB_CHECK(!report.objects.empty());
    CB_CHECK(report.byte_exact);
    CHECK_SAME(report.symbolic, 5U);
});

CB_TEST("source.vxd.masm.no_assembler_is_a_log_and_no_object", [] {
    const mx::Toolchain none;
    const mx::Assembled made = mx::assemble_coff(none, ".386p\r\n");
    CB_CHECK(!made.object.has_value());
    CB_CHECK(made.log.find("no assembler") != std::string::npos);
    const std::vector<std::string> texts = {"a", "b"};
    const auto batch = mx::assemble_coff_batch(none, texts);
    CHECK_SAME(batch.size(), 2U);
    CB_CHECK(!batch[0].object);
    const auto macro = mx::assemble_macro_batch(none, texts);
    CHECK_SAME(macro.size(), 2U);
    CB_CHECK(!macro[1].object);
});

CB_TEST("source.vxd.masm.the_real_assembler_agrees_where_it_is_installed", [] {
    const mx::Toolchain tools = mx::Toolchain::standard({}, cb::testing::data_path(""));
    CB_CHECK(tools.x86asm || tools.native_ml);   // x86asm is always there
    Hand hand;
    const mx::Emission code = hand.emission();
    mx::ObjectReport report;
    CB_CHECK_NOTHROW(report = mx::compare_object(code, 1, tools));
    CHECK_SAME(report.original_length, 14U);   // what the real assembler makes of it is its own; the report is made either way
});
