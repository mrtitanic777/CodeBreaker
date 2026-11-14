// cb-unity: alone
#include "cb/verify/gate16/scoreboard16.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {
namespace sb = cb::verify::gate16::scoreboard16;
namespace m16 = cb::verify::authored::match16;
using cb::verify::ByteMatch;

sb::Masked masked(std::initializer_list<std::uint8_t> code, std::initializer_list<std::uint8_t> mask = {}) {
    sb::Masked m;
    m.code.assign(code);
    m.mask.assign(mask);
    if (m.mask.empty()) m.mask.assign(m.code.size(), 0);
    return m;
}

bool has(std::string_view hay, std::string_view needle) { return hay.find(needle) != std::string_view::npos; }

// the byte comparison the real toolchain makes, reduced to the bytes outside the masks
class ByteToolchain final : public m16::Toolchain {
public:
    std::vector<std::string> flag_sets(const std::string&) const override { return {}; }
    std::map<std::string, std::optional<m16::Masked>> compile(const m16::Compilation&) override { return {}; }
    m16::Verdict distance(const m16::Masked& built, const m16::Masked& original) const override {
        m16::Verdict v;
        v.total = std::max(built.code.size(), original.code.size());
        if (built.code.size() != original.code.size()) {
            v.match = ByteMatch::Different;
            v.differing = v.total;
            return v;
        }
        for (std::size_t i = 0; i < built.code.size(); ++i)
            v.differing += built.code[i] != original.code[i] && !(i < built.mask.size() && built.mask[i]) && !(i < original.mask.size() && original.mask[i]);
        v.match = v.differing ? ByteMatch::SameSize : ByteMatch::Identical;
        return v;
    }
};

sb::Row row_of(ByteMatch msc8, std::size_t off, sb::Agreement agree, std::optional<std::size_t> watcom, std::size_t original, bool library = false) {
    sb::Row r;
    r.original = original;
    r.msc8_vs_original = msc8;
    r.msc8_bytes_off = off;
    r.watcom_vs_msc8 = agree;
    r.watcom = watcom;
    r.fits = watcom && *watcom <= original;
    r.library = library;
    return r;
}
}  // namespace

CB_TEST("verify.scoreboard16.watcom_flags", [] {
    using V = std::vector<std::string>;
    const V base = {"-bt=dos", "-zq", "-w0", "-zl", "-ecc", "-zu", "-zmsc", "-j", "-zastd=c99"};
    auto with = [&](V extra) {
        V v = base;
        v.insert(v.end(), extra.begin(), extra.end());
        return v;
    };
    CB_CHECK((sb::watcom_flags("") == base));
    CB_CHECK((sb::watcom_flags("/AM /Gsw /Os") == with({"-mm", "-s", "-zW", "-os"})));
    CB_CHECK((sb::watcom_flags("-AL /G2 /Ox") == with({"-ml", "-2", "-obmilr"})));
    CB_CHECK((sb::watcom_flags("/Oxs") == with({"-obmilr", "-os"})));
    CB_CHECK((sb::watcom_flags("/Od") == with({"-od"})));
    CB_CHECK((sb::watcom_flags("/Ot") == with({"-ot"})));
    CB_CHECK((sb::watcom_flags("/AX /Zi /c") == with({"-mm"})));   // an unknown model is medium; other switches are not mapped
});

CB_TEST("verify.scoreboard16.patterns", [] {
    const std::string c =
        "x = CONCAT22(a, b); y = CONCAT22(c, d); p = MK_FP(s, o);\n"
        "v = codebreaker_ds(); w = a >> 0x10; z = xunknown2; q = unaff_SI + in_DX;\n"
        "u = ZEXT24(k); *(int *)0x1a2 = 1;\n";
    const auto found = sb::count_patterns(c);
    const std::vector<std::pair<std::string, std::size_t>> expect = {
        {"CONCAT", 2}, {"MK_FP", 1}, {"segment helper", 1}, {"split far (>> 0x10)", 1}, {"xunknown", 1},
        {"unaff_/in_/extraout_", 2}, {"SUB/ZEXT/SEXT", 1}, {"constant DS address", 1}};
    CB_CHECK((found == expect));
    CB_CHECK(sb::count_patterns("return a + b;").empty());
    CB_CHECK(sb::count_patterns("CONCAT2(a)").empty());   // CONCAT takes two sizes
    CB_CHECK(sb::count_patterns("xxMK_FP(a)").empty());    // whole words
});

CB_TEST("verify.scoreboard16.same_code_and_trim", [] {
    // a fix-up's bytes, either side's, do not count
    CB_CHECK(sb::same_code(masked({1, 2, 3, 4}, {0, 0, 1, 0}), masked({1, 2, 9, 4}, {0, 0, 0, 0})));
    CB_CHECK(!sb::same_code(masked({1, 2, 3, 4}), masked({1, 2, 9, 4})));
    CB_CHECK(!sb::same_code(masked({1, 2}), masked({1, 2, 3})));
    // the padding after the last ret goes: int 3, nop and zeros, but not a byte a fix-up names, and never the only byte
    CB_CHECK_EQ(sb::trim_padding(masked({0x55, 0xC3, 0xCC, 0xCC, 0x90, 0x00})).code.size(), std::size_t{2});
    CB_CHECK_EQ(sb::trim_padding(masked({0x55, 0xC3, 0xCC}, {0, 0, 1})).code.size(), std::size_t{3});
    CB_CHECK_EQ(sb::trim_padding(masked({0x55, 0x8B, 0x90})).code.size(), std::size_t{3});   // a nop that is the function's own
    CB_CHECK_EQ(sb::trim_padding(masked({0x00})).code.size(), std::size_t{1});
    CB_CHECK_EQ(sb::trim_padding(masked({0x55, 0xC3, 0xCC})).mask.size(), std::size_t{2});
});

CB_TEST("verify.scoreboard16.report", [] {
    using A = sb::Agreement;
    sb::Rows rows;
    sb::Row a = row_of(ByteMatch::Identical, 0, A::Same, 10, 10);
    sb::Row b = row_of(ByteMatch::SameSize, 7, A::Differs, 14, 12);
    b.patterns = {{"CONCAT", 2}, {"MK_FP", 1}};
    sb::Row c = row_of(ByteMatch::Different, 30, A::NotApplicable, std::nullopt, 40);
    c.patterns = {{"MK_FP", 3}};
    sb::Row lib = row_of(ByteMatch::Identified, 0, A::Same, 50, 50, true);
    rows.insert_or_assign("a", a);
    rows.insert_or_assign("b", b);
    rows.insert_or_assign("c", c);
    rows.insert_or_assign("lib", lib);
    const auto lines = sb::report_lines(rows);
    // the library function is left out: 3 functions
    CB_CHECK_EQ(lines[0], std::string("3 functions (runtime library aside)"));
    CB_CHECK(has(lines[1], "   1   (decompiler's work: 2)"));
    CB_CHECK(has(lines[2], "   1   (compiler's work: 2)"));
    CB_CHECK(has(lines[3], "   1   (would move: 2)"));   // b is 2 bytes over; c has no OpenWatcom code
    CB_CHECK_EQ(lines[4], std::string("  bytes over the original's length, all together: 2"));
    // the patterns of the functions Microsoft C 8 does not match, the largest (by bytes off) first: MK_FP 37 (b 7 + c 30), CONCAT 7
    CB_CHECK_EQ(lines.size(), std::size_t{8});
    //
    CB_CHECK_EQ(lines[6], "    MK_FP" + std::string(19, ' ') + " " + "   2" + "  " + "    37");
    CB_CHECK_EQ(lines[7], "    CONCAT" + std::string(18, ' ') + " " + "   1" + "  " + "     7");
});

CB_TEST("verify.scoreboard16.json_round_trip", [] {
    using A = sb::Agreement;
    sb::Rows rows;
    sb::Row b = row_of(ByteMatch::SameSize, 7, A::Differs, 14, 12);
    b.segment = 3;
    b.msc8 = 12;
    b.patterns = {{"CONCAT", 2}, {"MK_FP", 1}};
    b.c = "int f(void)\n{\n    return 1;\n}";
    b.hex = {"55c3", "55", ""};
    rows.insert_or_assign("b", b);
    rows.insert_or_assign("a", row_of(ByteMatch::NoCompile, 0, A::NotApplicable, std::nullopt, 4));
    const std::string json = sb::rows_to_json(rows);
    // the keys are the board's names, in the order of the rows; a missing compile is null
    CB_CHECK(has(json, "\"msc8_vs_original\": \"same size\""));
    CB_CHECK(has(json, "\"watcom_vs_msc8\": \"differs\""));
    CB_CHECK(json.find("\"b\"") < json.find("\"a\""));
    const sb::Rows back = sb::rows_from_json(json);
    CB_CHECK_EQ(back.size(), std::size_t{2});
    const sb::Row& r = back.at("b");
    CB_CHECK(r.segment == std::optional<std::uint32_t>(3));
    CB_CHECK(r.msc8 == std::optional<std::size_t>(12) && r.watcom == std::optional<std::size_t>(14));
    CB_CHECK(r.msc8_vs_original == ByteMatch::SameSize && r.watcom_vs_msc8 == A::Differs && r.fits == false);
    CB_CHECK((r.patterns == b.patterns));
    CB_CHECK_EQ(r.c, b.c);
    CB_CHECK_EQ(r.hex.original, std::string("55c3"));
    CB_CHECK(!back.at("a").watcom && back.at("a").msc8_vs_original == ByteMatch::NoCompile);
    CB_CHECK_THROWS_AS(cb::FormatError, sb::rows_from_json("[1, 2]"));
    CB_CHECK_THROWS_AS(cb::FormatError, sb::rows_from_json("{\"f\": {\"original\": 1}}"));
});

CB_TEST("verify.scoreboard16.show_lines", [] {
    sb::Row r;
    r.c = "int f(void) { return 1; }";
    r.hex = {"55c3", "55", "c3"};   // push bp; ret   (the 16-bit decoder)
    const auto lines = sb::show_lines(r);
    CB_CHECK_EQ(lines.size(), std::size_t{4});
    CB_CHECK_EQ(lines[0], r.c);
    CB_CHECK(lines[1].starts_with("  original") && has(lines[1], "| Microsoft C 8") && has(lines[1], "| OpenWatcom -zmsc"));
    CB_CHECK_EQ(lines[2], std::string("  push bp                         | push bp                         | ret "));
    CB_CHECK(lines[3].starts_with("  ret ") && lines[3].find('|') == 2 + 32);
});

CB_TEST("verify.scoreboard16.arguments", [] {
    const auto a = sb::parse_arguments({"setup.dll", "--segment", "2", "--json", "board.json"});
    CB_CHECK(a && a->module == "setup.dll" && a->segment == std::optional<std::uint32_t>(2) && a->json == "board.json" && a->show.empty());
    const auto s = sb::parse_arguments({"--show", "f,g", "--json", "board.json"});
    CB_CHECK(s && (s->show == std::vector<std::string>{"f", "g"}) && s->json == "board.json");
    CB_CHECK(!sb::parse_arguments({"--show", "f"}));            // a saved board is needed
    CB_CHECK(!sb::parse_arguments({}));
    CB_CHECK(!sb::parse_arguments({"x.dll", "--segment", "n"}));
    CB_CHECK(!sb::parse_arguments({"x.dll", "--json"}));
});

// With CB_OPENWATCOM set the tool is run: two functions compiled together, a file that does not compile and is then taken
// a function at a time, and the whole board of a module with a fake Microsoft C beside it.
CB_TEST("verify.scoreboard16.openwatcom_for_real", [] {
    const auto root = cb::fs::get_env("CB_OPENWATCOM");
    if (!root) return;   // (not set up: nothing to run)
    const cb::build::watcom::OpenWatcom ow{cb::fs::from_text(*root)};
    if (ow.cc(cb::build::watcom::Target::Win16).empty()) return;
    const std::map<std::string, std::string> texts = {
        {"add1", "int add1(int a) { return a + 1; }"},
        {"twice", "int twice(int a) { return a + a; }"},
        {"broken", "int broken(int a) { return a +; }"}};
    std::vector<std::string> lines;
    const std::function<std::string(const std::string&)> switches = [](const std::string&) { return std::string("/AM /Gs /Os"); };
    const sb::WatcomCode got = sb::compile_watcom(ow, {{"add1", "twice"}, {"broken"}, {"missing"}}, texts, "", switches,
                                                  [&](std::string_view l) { lines.emplace_back(l); });
    CB_CHECK(got.code.contains("add1") && got.code.at("add1").has_value());
    CB_CHECK(got.code.contains("twice") && got.code.at("twice").has_value());
    CB_CHECK(!got.code.at("add1")->code.empty());
    // the one that does not compile is refused, with OpenWatcom's own error line, and says so in the log
    CB_CHECK(got.code.contains("broken") && !got.code.at("broken").has_value());
    CB_CHECK(got.refused.contains("broken") && has(got.refused.at("broken"), "Error!"));
    CB_CHECK(!got.code.contains("missing"));   // no text: not compiled
    CB_CHECK(lines.size() == 1 && has(lines[0], "OpenWatcom refused 1 functions, e.g. broken:"));
    // a file with one bad function is taken a function at a time: the others are not sunk with it
    const sb::WatcomCode apart = sb::compile_watcom(ow, {{"add1", "broken", "twice"}}, texts, "", switches, {});
    CB_CHECK(apart.code.at("add1").has_value() && apart.code.at("twice").has_value() && !apart.code.at("broken").has_value());
    CB_CHECK_EQ(apart.refused.size(), std::size_t{1});

    // the board: add1 has Microsoft C 8 give what OpenWatcom gave; twice did not compile with Microsoft C 8
    const sb::Masked add1 = *got.code.at("add1");
    const std::vector<m16::Function> fns = {{"add1", 1, 0, add1}, {"twice", 1, 0x20, masked({0x90})}};
    m16::Result result;
    result.code["add1"] = add1;
    result.code["twice"] = std::nullopt;
    result.verdicts["add1"] = {ByteMatch::Identical, 0, add1.code.size()};
    result.verdicts["twice"] = {ByteMatch::NoCompile, 0, 0};
    result.files = {{"add1", "twice"}};
    result.setting["add1"] = {"msvc", "/AM /Gs /Os"};
    result.setting["twice"] = {"msvc", "/AM /Gs /Os"};
    const std::map<std::string, std::string> board_texts = {{"add1", texts.at("add1")}, {"twice", texts.at("twice")}};
    ByteToolchain chain;
    sb::Inputs in;
    in.functions = fns;
    in.result = &result;
    in.texts = &board_texts;
    in.watcom = &ow;
    in.toolchain = &chain;
    const sb::Rows rows = sb::run(in);
    CB_CHECK_EQ(rows.size(), std::size_t{2});
    CB_CHECK_EQ(rows.begin()->first, std::string("add1"));
    const sb::Row& a = rows.at("add1");
    CB_CHECK(a.msc8_vs_original == ByteMatch::Identical && a.watcom_vs_msc8 == sb::Agreement::Same && a.fits && !a.library);
    CB_CHECK(a.segment == std::optional<std::uint32_t>(1) && a.msc8 == a.watcom && a.original == *a.msc8);
    CB_CHECK_EQ(a.c, texts.at("add1"));
    const sb::Row& t = rows.at("twice");
    CB_CHECK(!t.msc8 && t.watcom && t.msc8_vs_original == ByteMatch::NoCompile && t.watcom_vs_msc8 == sb::Agreement::NotApplicable && !t.fits);
    CB_CHECK(t.hex.msc8.empty() && !t.hex.watcom.empty() && t.hex.original == "90");
    // only a segment's functions
    in.segment = 2;
    CB_CHECK(sb::run(in).empty());
});
