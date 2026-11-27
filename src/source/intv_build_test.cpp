// cb-unity: alone
// The Intellivision cartridge layer's directives, segments and labels, and the instruction syntax by example (cp1610_asm's and decompile/cp1610's
// cases), through the one assembler.
#include "source/intv_build.hpp"

#include <format>

#include "core/error.hpp"
#include "core/testing.hpp"
#include "formats/intv.hpp"

namespace cb::source::intv_build {
namespace {

using Words = std::vector<std::uint16_t>;

// The decles of one instruction line at `pc`, as the layer builds them: after `labels` (name -> value), after an SDBD when `sdbd`.
Words words_of(std::string_view line, std::uint32_t pc = 0x5000, const std::vector<std::pair<std::string, std::uint32_t>>& labels = {}, bool sdbd = false) {
    std::string source = "FORMAT RAW\n        SEGMENT $50,$50\n";
    for (const auto& [name, value] : labels) source += std::format("{} EQU ${:X}\n", name, value);
    source += std::format("        ORG ${:X}\n", pc);
    if (sdbd) source += "        SDBD\n";
    source += std::string("        ") + std::string(line) + "\n        DECLE $3FF\n";
    const Bytes raw = build(source);
    Words all;
    for (std::size_t i = static_cast<std::size_t>(pc - 0x5000 + (sdbd ? 1 : 0)) * 2; i + 1 < raw.size(); i += 2) all.push_back(static_cast<std::uint16_t>((raw[i] << 8) | raw[i + 1]));
    while (!all.empty() && all.back() == 0) all.pop_back();
    if (!all.empty()) all.pop_back();   // (the marker)
    return all;
}

}  // namespace

CB_TEST("source.intv_build.segments_directives_labels_and_the_crc", [] {
    const char* source =
        "; a program\n"
        "STIC_DISPLAY_ENABLE EQU $0020\n"
        "        SEGMENT $50,$50\n"
        "        ORG $5000\n"
        "start:\n"
        "        MVII #5,R0\n"
        "        MVO R0,STIC_DISPLAY_ENABLE\n"
        "        BNEQ done\n"
        "        JSR R4,sub\n"
        "done:   MOVR R5,R7\n"
        "sub:    SDBD\n"
        "        MVII #table,R1\n"
        "        MOVR R4,R7\n"
        "table:  DECLE $12, 3, done\n"
        "        FILL 3,$FF\n"
        "        ROMTAIL $01,$02\n";
    const Bytes file = build(source);
    const formats::intv::Rom rom = formats::intv::parse(file);
    CB_CHECK_EQ(rom.segments.size(), 1u);
    CB_CHECK_EQ(rom.segments[0].start, 0x5000u);
    const auto& w = rom.segments[0].words;
    CB_CHECK((std::vector<std::uint16_t>(w.begin(), w.begin() + 8) == std::vector<std::uint16_t>{0x2B8, 5, 0x240, 0x20, 0x20C, 3, 4, 0x50}));
    CB_CHECK_EQ(w[8], 0x0Au);    // the JSR's address: sub is at $500A
    CB_CHECK_EQ(w[9], 0xAFu);    // done: MOVR R5,R7
    CB_CHECK_EQ(w[10], 1u);      // sub: SDBD
    CB_CHECK((std::vector<std::uint16_t>(w.begin() + 11, w.begin() + 15) == std::vector<std::uint16_t>{0x2B9, 0x0F, 0x50, 0xA7}));   // MVII #table,R1 as two decles; MOVR R4,R7
    CB_CHECK((std::vector<std::uint16_t>(w.begin() + 15, w.begin() + 19) == std::vector<std::uint16_t>{0x12, 3, 0x5009, 0xFF}));
    CB_CHECK_EQ(rom.tail.size(), 2u);
    CB_CHECK(rom.serialize(true) == file);   // the stored CRC is the computed one
});

CB_TEST("source.intv_build.raw_format_and_errors", [] {
    const Bytes raw = build("        FORMAT RAW\n        SEGMENT $50,$50\n        ORG $5002\n        DECLE 1,2\n");
    CB_CHECK_EQ(raw.size(), 512u);
    CB_CHECK_EQ(raw[5], 1);
    CB_CHECK_EQ(raw[7], 2);
    CB_CHECK_THROWS_AS(ParseError, (void)build("        DECLE 1\n"));                                        // outside a segment
    CB_CHECK_THROWS_AS(ParseError, (void)build("        SEGMENT $50,$50\n        BEQ nowhere\n"));
    CB_CHECK_THROWS_AS(ParseError, (void)build("        SEGMENT $50,$50\n        ORG $5100\n        DECLE 1\n"));   // past the segment
    CB_CHECK_THROWS_AS(ParseError, (void)build("        SEGMENT $50,$50\n        FROB\n"));
});

CB_TEST("source.intv_build.the_syntax_by_example", [] {
    CB_CHECK((words_of("MVII #$12,R0") == Words{0x2B8, 0x12}));
    CB_CHECK((words_of("MVII #$1234,R1", 0x5000, {}, true) == Words{0x2B9, 0x34, 0x12}));
    CB_CHECK((words_of("MOVR R5,R7") == Words{0xAF}));
    CB_CHECK((words_of("SDBD") == Words{1}));
    CB_CHECK((words_of("MVO R0,$0100") == Words{0x240, 0x100}));
    CB_CHECK((words_of("MVI $0100,R3") == Words{0x283, 0x100}));
    CB_CHECK((words_of("MVI@ R4,R1") == Words{0x2A1}));
    CB_CHECK((words_of("MVO@ R1,R4") == Words{0x261}));
    CB_CHECK((words_of("ADDI #5,R2") == Words{0x2FA, 5}));
    CB_CHECK((words_of("CMP@ R3,R0") == Words{0x358}));
    CB_CHECK((words_of("JSR R5,$5100") == Words{4, 0x150, 0x100}));
    CB_CHECK((words_of("J $5000") == Words{4, 0x350, 0}));
    CB_CHECK((words_of("JSRE R6,label", 0x5000, {{"label", 0x5100}}) == Words{4, 0x251, 0x100}));
    CB_CHECK((words_of("BNEQ there", 0x5000, {{"there", 0x5010}}) == Words{0x20C, 0xE}));
    CB_CHECK((words_of("B there", 0x5000, {{"there", 0x5000}}) == Words{0x220, 1}));
    CB_CHECK((words_of("BEXT 3,there", 0x5000, {{"there", 0x5002}}) == Words{0x213, 0}));
    CB_CHECK((words_of("SLL R1,2") == Words{0x4D}));
    CB_CHECK((words_of("SARC R3") == Words{0x7B}));
    CB_CHECK((words_of("INCR R3") == Words{0x0B}));
    CB_CHECK((words_of("GSWD R2") == Words{0x32}));
    CB_CHECK((words_of("RSWD R5") == Words{0x3D}));
    CB_CHECK((words_of("NOP") == Words{0x34}));
    CB_CHECK((words_of("SIN 1") == Words{0x37}));
    CB_CHECK((words_of("MVII #table,R4", 0x5000, {{"table", 0x2F5}}) == Words{0x2BC, 0x2F5}));
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("MVII #$400,R0"));
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("MVO R0,$4000"));
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("MVI@ R0,R1"));
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("BNEQ there", 0x5000, {{"there", 0x6000}}));
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("GSWD R5"));
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("FROB"));
});

}  // namespace cb::source::intv_build
