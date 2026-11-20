// cb-unity: alone
// The Vectrex writer over a small cartridge written byte by byte: the source it writes, built again by CodeBreaker's 6809 assembler, is the cartridge.

#include "source/vectrex_writer.hpp"

#include "build/m6809_asm.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/vectrex_test_image.hpp"

namespace cb::source::vectrex_writer {

namespace ti = formats::vectrex::test_image;

CB_TEST("source.vectrex_writer.the_code_scan_finds_the_entry_and_the_call", [] {
    const formats::vectrex::Rom rom = formats::vectrex::parse(ti::cartridge());
    CB_CHECK((rom.functions(decoder()) == std::vector<std::uint32_t>{0x17, 0x25}));
});

CB_TEST("source.vectrex_writer.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_vec_writer");
    const image_writer::Written r = write(formats::vectrex::parse(ti::cartridge()), "demo.vec", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x17, "start"}, {0x25, "sub_0025"}}));
    CB_CHECK_EQ(r.image_bytes, 0x40u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"FCC \"g GCE 1982\"", "FDB $FD0D", "FCC \"TEST\"", "Wait_Recal    ", "EQU $F192", "VIA_int_enable", "JSR Wait_Recal", "STA VIA_int_enable", "BSR sub_0025", "BRA start", "start", "sub_0025"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.vectrex_writer.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_vec_edit");
    const image_writer::Written r = write(formats::vectrex::parse(ti::cartridge()), "demo.vec", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("LDA #$7F");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 8, "LDA #$7E");
    Bytes expected = ti::cartridge();
    expected[0x1C - 1] = 0x7E;
    CB_CHECK(build::m6809_asm::build(s) == expected);
});

}  // namespace cb::source::vectrex_writer
