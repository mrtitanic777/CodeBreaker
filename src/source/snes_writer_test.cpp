// The Super Nintendo writer: a cartridge comes back as source (the 65816's SLEIGH description) that builds the same file, the register widths followed through the code.
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
    CB_CHECK_EQ(r.code_bytes, 54u);   // (58 less the four bytes of the long LDA, written as bytes below)
    CB_CHECK_EQ(r.image_bytes, file.size());
    CB_CHECK(!r.config);
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has_line(source, ".equ INIDISP, 0x2100"));
    CB_CHECK(has_line(source, ".equ VMDATAL, 0x2118"));
    CB_CHECK(has_line(source, ".equ dp_10, 0x10"));
    CB_CHECK(has_line(source, "; BANK0"));
    CB_CHECK(has_line(source, ".org 0x008000"));
    CB_CHECK(has_line(source, "reset:"));
    CB_CHECK(has_line(source, "XCE"));
    CB_CHECK(has_line(source, "REP #0x30"));
    CB_CHECK(has_line(source, ".context m16=1 x16=1"));   // (the widths ride on `.context`, which the description's variables take)
    CB_CHECK(has_line(source, "LDA #0x1234"));   // the immediate is two bytes because REP #$30 made A 16 bits wide
    CB_CHECK(has_line(source, "LDX #0x1fff"));
    CB_CHECK(has_line(source, ".context m16=0 x16=1"));
    CB_CHECK(has_line(source, "LDA #0x8f"));     // and one again after SEP #$20
    CB_CHECK(has_line(source, "STA INIDISP"));
    CB_CHECK(has_line(source, "JSL sub_008040"));
    CB_CHECK(has_line(source, "JSR sub_008050"));
    CB_CHECK(has_line(source, "JML L_00801A"));
    CB_CHECK(has_line(source, "LDX #0x10"));
    // (a long LDA of a bank-0 address reads `LDA 0x9000,X`, the text of the 16-bit one: it stays as its bytes, with the reading in a comment)
    CB_CHECK(has_line(source, ".byte 0xbf,0x00,0x90,0x00  ; LDA 0x9000,X"));
    CB_CHECK(has_line(source, "STA dp_10"));
    CB_CHECK(has_line(source, "STA VMDATAL"));
    CB_CHECK(has_line(source, "RTL"));
    CB_CHECK(has_line(source, "LDA #0xabcd"));
    CB_CHECK(has_line(source, "STA 0x300"));
    CB_CHECK(has_line(source, "nmi:"));
    CB_CHECK(has_line(source, ".half cop,brk,nmi,nmi,0x0000,irq   ; native mode: COP, BRK, ABORT, NMI, -, IRQ") ||
             has_line(source, ".half cop,brk,nmi,nmi,0xffff,irq   ; native mode: COP, BRK, ABORT, NMI, -, IRQ"));
});

CB_TEST("source.snes_writer.a_copier_header_is_a_segment_of_its_own", [] {
    fs::ScopedTempDir dir("cb_snes_writer_header");
    const Bytes file = formats::snes_test_image::cartridge(true);
    fs::write_file(dir.path() / "demo.smc", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.smc", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, file.size() - 512);
    CB_CHECK(has_line(fs::read_text(r.source), "; HEADER"));
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
    CB_CHECK(has_line(source, "STA D_C02000_w"));   // a 16-bit data operand is read in the instruction's own bank, where HiROM has ROM at $2000: the label's low 16 bits
    CB_CHECK(has_line(source, ".equ D_C02000_w, 0x2000"));
    CB_CHECK(has_line(source, ".org 0xc00000"));
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
    CB_CHECK(has_line(source, "JSR sub_808050"));   // (the description writes a JSR's target with its bank, the program bank's)
    CB_CHECK(has_line(source, ".org 0x808000"));
});

CB_TEST("source.snes_writer.a_file_that_is_not_a_cartridge_is_refused", [] {
    fs::ScopedTempDir dir("cb_snes_writer_bad");
    fs::write_file(dir.path() / "bad.sfc", Bytes(100, 0x11));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "bad.sfc", dir.path() / "out"));
});

}  // namespace
}  // namespace cb::source::snes_writer
