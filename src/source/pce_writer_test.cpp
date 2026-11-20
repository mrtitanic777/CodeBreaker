// The PC Engine writer: a HuCard comes back as source (the HuC6280's SLEIGH description) that builds the same file.
#include "source/pce_writer.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/pce_test_image.hpp"

namespace cb::source::pce_writer {
namespace {

bool has_line(const std::string& source, std::string_view line) {
    for (const std::string_view l : text::lines(source)) {
        if (text::trim(l) == line) return true;
    }
    return false;
}

CB_TEST("source.pce_writer.a_card_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_pce_writer");
    const Bytes file = formats::pce_test_image::cartridge();
    fs::write_file(dir.path() / "demo.pce", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.pce", dir.path() / "out");
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.linked && r.same);
    const image_writer::Functions expected = {{0x00E000, "reset"},     {0x00E020, "sub_00E020"}, {0x00E040, "nmi"},
                                              {0x00E041, "irq1"},      {0x00E042, "irq2"},       {0x014000, "sub_014000"}};
    CB_CHECK(r.functions == expected);
    CB_CHECK_EQ(r.code_bytes, 56u);   // (62 less the two STA absolute of a zero-page-sized address: three bytes each, below)
    CB_CHECK_EQ(r.image_bytes, file.size());
    CB_CHECK(!r.config);
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has_line(source, ".equ VDC_SELECT, 0x0000"));
    CB_CHECK(has_line(source, ".equ VDC_DATA_L, 0x0002"));
    CB_CHECK(has_line(source, ".equ zp_20, 0x20"));
    CB_CHECK(has_line(source, "; BANK0"));
    CB_CHECK(has_line(source, "; BANK1"));
    CB_CHECK(has_line(source, ".org 0xe000"));
    CB_CHECK(has_line(source, ".org 0x4000"));
    CB_CHECK(has_line(source, "reset:"));
    CB_CHECK(has_line(source, "CSH"));
    CB_CHECK(has_line(source, "TAM #0x4"));
    CB_CHECK(has_line(source, "JSR sub_014000"));   // bank 1, found mapped at $4000 by the LDA #1 and TAM #$04 before it
    CB_CHECK(has_line(source, "BSR sub_00E020"));
    CB_CHECK(has_line(source, "ST0 #0x5  ; VDC CR"));
    CB_CHECK(has_line(source, "LDA zp_20"));
    CB_CHECK(has_line(source, "TII D_00E100, 0x2000, 0x4"));
    CB_CHECK(has_line(source, "BBS0 zp_21, L_00E030"));
    // (an absolute STA of a zero-page-sized address, which the description's text cannot say, stays as its bytes)
    CB_CHECK(has_line(source, ".byte 0x8d,0x00,0x00  ; STA 0x0"));
    CB_CHECK(has_line(source, ".byte 0x8d,0x02,0x00  ; STA 0x2"));
    CB_CHECK(has_line(source, "TST #0x1, zp_20"));
    CB_CHECK(has_line(source, ".half irq2,irq1,irq2,nmi,reset"));
});

CB_TEST("source.pce_writer.a_copier_header_is_a_segment_of_its_own", [] {
    fs::ScopedTempDir dir("cb_pce_writer_header");
    const Bytes file = formats::pce_test_image::cartridge(true);
    fs::write_file(dir.path() / "demo.pce", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.pce", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, file.size() - 512);
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has_line(source, "; HEADER"));
});

CB_TEST("source.pce_writer.what_is_not_reached_stays_data_and_an_edit_still_rebuilds", [] {
    fs::ScopedTempDir dir("cb_pce_writer_edit");
    Bytes file = formats::pce_test_image::cartridge();
    file[0x0B + 1] = 0x00;   // LDA #$00 instead of #$01: $4000 now shows bank 0, so bank 1 is reached by nothing and is data
    fs::write_file(dir.path() / "demo.pce", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.pce", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(!r.functions.contains(0x014000));
    CB_CHECK_EQ(r.code_bytes, 50u);   // the nine bytes of bank 1's function are data now, and the absolute STA of $0000 and $0002 (three bytes each) are written as bytes
    CB_CHECK(has_line(fs::read_text(r.source), "JSR 0x4000"));   // bank 0 is shown at $E000: the operand cannot name its label, so it is the number
});

CB_TEST("source.pce_writer.a_file_that_is_not_a_card_is_refused", [] {
    fs::ScopedTempDir dir("cb_pce_writer_bad");
    fs::write_file(dir.path() / "bad.pce", Bytes(100, 0x11));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "bad.pce", dir.path() / "out"));
});

}  // namespace
}  // namespace cb::source::pce_writer
