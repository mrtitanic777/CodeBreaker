// cb-unity: alone
// The CP1610 assembler's directives, segments and labels (the instructions are decompile/cp1610's).
#include "cb/build/cp1610_asm.hpp"

#include "cb/build/asm_text.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/intv.hpp"

namespace cb::build::cp1610_asm {

CB_TEST("build.cp1610_asm.segments_directives_labels_and_the_crc", [] {
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

CB_TEST("build.cp1610_asm.raw_format_and_errors", [] {
    const Bytes raw = build("        FORMAT RAW\n        SEGMENT $50,$50\n        ORG $5002\n        DECLE 1,2\n");
    CB_CHECK_EQ(raw.size(), 512u);
    CB_CHECK_EQ(raw[5], 1);
    CB_CHECK_EQ(raw[7], 2);
    CB_CHECK_THROWS_AS(AsmError, (void)build("        DECLE 1\n"));                                        // outside a segment
    CB_CHECK_THROWS_AS(AsmError, (void)build("        SEGMENT $50,$50\n        BEQ nowhere\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        SEGMENT $50,$50\n        ORG $5100\n        DECLE 1\n"));   // past the segment
    CB_CHECK_THROWS_AS(AsmError, (void)build("        SEGMENT $50,$50\n        FROB\n"));
});

}  // namespace cb::build::cp1610_asm
