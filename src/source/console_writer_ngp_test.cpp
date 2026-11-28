// cb-unity: alone
// The Neo Geo Pocket writer over a small cartridge: the source it writes, built again by CodeBreaker's TLCS-900/H assembler, is the cartridge.

#include "formats/ngp.hpp"
#include "source/console_shapes.hpp"

#include "source/dialect.hpp"
#include "source/processors.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/ngp_test_image.hpp"

namespace cb::source::console_writer {
namespace {

// The bytes written to a file in `dir` and back as source through the console writer (config/platforms/ngp.json).
[[maybe_unused]] image_writer::Written write_file_of(const fs::ScopedTempDir& dir, const Bytes& bytes, const char* name) {
    fs::write_file(dir.path() / name, bytes);
    return reconstruct("ngp", dir.path() / name, dir.path());
}
}  // namespace

namespace ti = formats::ngp::test_image;

CB_TEST("source.console_writer.ngp.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_ngp_writer");
    const image_writer::Written r = write_file_of(dir, ti::cartridge(), "demo.ngp");
    CB_CHECK_EQ(r.notes.empty() ? std::string() : r.notes.front(), std::string());
    CB_CHECK(r.linked && r.same);
    // (start; the VBlank handler found through the store into the BIOS's vector table; the called subroutine)
    CB_CHECK((r.functions == image_writer::Functions{{0x200040, "start"}, {0x200080, "sub_200080"}, {0x2000A0, "isr_vbl"}}));
    CB_CHECK_EQ(r.image_bytes, 0x1000u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"HW_WATCHDOG EQU 0x006F", "K2GE_RAS_V EQU 0x8009", "ORG 0x200000", "DB \" LICENSED BY SNK CORPORATION\"", "DB <start,>start,0x20,0x00", "start:", "sub_200080:",
                             "isr_vbl:", "D_2000C0:", "ld (HW_WATCHDOG),0x4E", "ld XWA,isr_vbl", "ld (BIOS_INT_VBL),XWA", "ld A,(K2GE_RAS_V)", "call sub_200080", "ld XHL,D_2000C0", "jr Z,start",
                             "swi 1", "reti", "FILL "}) {
        if (s.find(want) == std::string::npos) CB_CHECK_EQ(std::string("missing: ") + want, std::string());
    }
});

CB_TEST("source.console_writer.ngp.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_ngp_edit");
    const image_writer::Written r = write_file_of(dir, ti::cartridge(), "demo.ngp");
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("add WA,BC");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 9, "sub WA,BC");
    const Bytes original = ti::cartridge(), built = processors::assemble_described(processors::find("tlcs900"), dialect::find("asmtlcs900").parse(s), ".").data;
    CB_CHECK_EQ(built.size(), original.size());
    std::size_t different = 0;
    for (std::size_t k = 0; k < built.size(); ++k) different += built[k] != original[k];
    CB_CHECK_EQ(different, 1u);   // (add WA,r and sub WA,r differ in one byte of the operation)
});

CB_TEST("source.console_writer.ngp.an_instruction_with_a_second_encoding_is_written_as_bytes", [] {
    // ld A,(XWA) as $C3 $E0 $21 (the extended mode) instead of $80 $21: the text assembles to the shorter one, so the bytes are kept as data
    Bytes rom = ti::cartridge();
    const Bytes extended = {0xC3, 0xE0, 0x21, 0x0E};   // ld A,(XWA) ; ret
    std::copy(extended.begin(), extended.end(), rom.begin() + 0x100);
    fs::ScopedTempDir dir("cb_ngp_alias");
    // `sub` (at $200080) now begins with a jump to $200100
    rom[0x80] = 0x1B;
    rom[0x81] = 0x00;
    rom[0x82] = 0x01;
    rom[0x83] = 0x20;
    const image_writer::Written r = write_file_of(dir, rom, "alias.ngp");
    CB_CHECK(r.linked && r.same);
    const std::string s = fs::read_text(r.source);
    CB_CHECK(s.find("DB 0xC3,0xE0,0x21  ; ld A,(XWA)") != std::string::npos);
});

}  // namespace cb::source::console_writer
