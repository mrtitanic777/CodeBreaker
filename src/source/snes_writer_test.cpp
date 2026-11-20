// The Super Nintendo writer: a cartridge comes back as ca65 source that builds the same file, the register widths followed through the code.
#include "source/snes_writer.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/snes_test_image.hpp"

namespace cb::source::snes_writer {
namespace {

bool has_line(const std::string& source, std::string_view line) {
    for (const std::string_view l : text::lines(source)) {
        if (text::trim(l) == line) return true;
    }
    return false;
}

constexpr std::uint32_t kNative8 = kStartsNative;
constexpr std::uint32_t kNativeWideIndex = kStartsNative | kStartsWith16BitIndex;

CB_TEST("source.snes_writer.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_snes_writer");
    const Bytes file = formats::snes_test_image::cartridge();
    fs::write_file(dir.path() / "demo.sfc", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.sfc", dir.path() / "out");
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.linked && r.same);
    const image_writer::Functions expected = {{0x008000, "reset"},
                                              {0x008040 | kNativeWideIndex, "sub_008040"},
                                              {0x008050 | kNativeWideIndex, "sub_008050"},
                                              {0x008060 | kNative8, "nmi"},
                                              {0x008061 | kNative8, "irq"},
                                              {0x008062 | kNative8, "cop"},
                                              {0x008063 | kNative8, "brk"}};
    CB_CHECK(r.functions == expected);
    CB_CHECK_EQ(r.code_bytes, 58u);
    CB_CHECK_EQ(r.image_bytes, file.size());
    CB_CHECK(r.config.has_value());
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has_line(source, ".p816"));
    CB_CHECK(has_line(source, "INIDISP        = $2100"));
    CB_CHECK(has_line(source, "VMDATAL        = $2118"));
    CB_CHECK(has_line(source, "dp_10          = $10"));
    CB_CHECK(has_line(source, ".segment \"BANK0\""));
    CB_CHECK(has_line(source, "reset:"));
    CB_CHECK(has_line(source, "XCE"));
    CB_CHECK(has_line(source, "REP #$30"));
    CB_CHECK(has_line(source, ".a16"));
    CB_CHECK(has_line(source, "LDA #$1234"));   // the immediate is two bytes because REP #$30 made A 16 bits wide
    CB_CHECK(has_line(source, ".i16"));
    CB_CHECK(has_line(source, "LDX #$1FFF"));
    CB_CHECK(has_line(source, ".a8"));
    CB_CHECK(has_line(source, "LDA #$8F"));     // and one again after SEP #$20
    CB_CHECK(has_line(source, "STA INIDISP"));
    CB_CHECK(has_line(source, "JSL sub_008040"));
    CB_CHECK(has_line(source, "JSR sub_008050"));
    CB_CHECK(has_line(source, "JML L_00801A"));
    CB_CHECK(has_line(source, "LDX #$0010"));
    CB_CHECK(has_line(source, "LDA f:D_009000,x"));
    CB_CHECK(has_line(source, "STA dp_10"));
    CB_CHECK(has_line(source, "STA VMDATAL"));
    CB_CHECK(has_line(source, "RTL"));
    CB_CHECK(has_line(source, "LDA #$ABCD"));
    CB_CHECK(has_line(source, "STA $0300"));
    CB_CHECK(has_line(source, "nmi:"));
    CB_CHECK(has_line(source, ".word cop, brk, nmi, nmi, $0000, irq   ; native mode: COP, BRK, ABORT, NMI, -, IRQ") ||
             has_line(source, ".word cop, brk, nmi, nmi, $FFFF, irq   ; native mode: COP, BRK, ABORT, NMI, -, IRQ"));
    CB_CHECK(fs::read_text(*r.config).find("BANK0: start = $008000, size = $8000, fill = yes") != std::string::npos);
});

CB_TEST("source.snes_writer.a_copier_header_is_a_segment_of_its_own", [] {
    fs::ScopedTempDir dir("cb_snes_writer_header");
    const Bytes file = formats::snes_test_image::cartridge(true);
    fs::write_file(dir.path() / "demo.smc", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.smc", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, file.size() - 512);
    CB_CHECK(has_line(fs::read_text(r.source), ".segment \"HEADER\""));
    CB_CHECK(fs::read_text(*r.config).find("HEADER: start = $000000, size = $0200") != std::string::npos);
});

CB_TEST("source.snes_writer.a_hirom_cartridge_is_written_at_c0", [] {
    fs::ScopedTempDir dir("cb_snes_writer_hirom");
    const Bytes file = formats::snes_test_image::hirom();
    fs::write_file(dir.path() / "demo.sfc", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.sfc", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    const std::uint32_t both_wide = kStartsNative | kStartsWith16BitAccumulator | kStartsWith16BitIndex;
    CB_CHECK((r.functions == image_writer::Functions{{0xC08000, "reset"}, {0xC08020 | both_wide, "sub_C08020"}, {0xC08060 | kNative8, "nmi"}, {0xC08061 | kNative8, "irq"},
                                                     {0xC08062 | kNative8, "cop"}, {0xC08063 | kNative8, "brk"}}));
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has_line(source, "JSL sub_C08020"));
    CB_CHECK(has_line(source, "STA .loword(D_C02000)"));   // a 16-bit data operand is read in the instruction's own bank, where HiROM has ROM at $2000
    CB_CHECK(fs::read_text(*r.config).find("BANK0: start = $C00000, size = $10000, fill = yes") != std::string::npos);
});

CB_TEST("source.snes_writer.the_mirror_the_long_addresses_use_is_the_one_written", [] {
    fs::ScopedTempDir dir("cb_snes_writer_mirror");
    Bytes file = formats::snes_test_image::cartridge();
    file[0x13 + 3] = file[0x1A + 3] = file[0x43 + 3] = 0x80;   // JSL $808040, JML $80801A and LDA $809000,X: the long addresses are in the $80 mirror
    fs::write_file(dir.path() / "demo.sfc", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.sfc", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.functions.contains(0x808040 | kNativeWideIndex));
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has_line(source, "JSL sub_808040"));
    CB_CHECK(has_line(source, "JSR .loword(sub_808050)"));   // a 16-bit operand names a label with a bank as its low word
    CB_CHECK(fs::read_text(*r.config).find("BANK0: start = $808000") != std::string::npos);
});

CB_TEST("source.snes_writer.a_file_that_is_not_a_cartridge_is_refused", [] {
    fs::ScopedTempDir dir("cb_snes_writer_bad");
    fs::write_file(dir.path() / "bad.sfc", Bytes(100, 0x11));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "bad.sfc", dir.path() / "out"));
});

}  // namespace
}  // namespace cb::source::snes_writer
