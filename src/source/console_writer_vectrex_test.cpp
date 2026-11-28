// cb-unity: alone
// The Vectrex writer over a small cartridge written byte by byte: the source it writes, built again by CodeBreaker's 6809 assembler, is the cartridge.

#include "formats/vectrex.hpp"
#include "source/console_shapes.hpp"

#include "source/dialect.hpp"
#include "source/processors.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/vectrex_test_image.hpp"

namespace cb::source::console_writer {
namespace {

// The bytes written to a file in `dir` and back as source through the console writer (config/platforms/vectrex.json).
[[maybe_unused]] image_writer::Written write_file_of(const fs::ScopedTempDir& dir, const Bytes& bytes, const char* name) {
    fs::write_file(dir.path() / name, bytes);
    return reconstruct("vectrex", dir.path() / name, dir.path());
}
}  // namespace

namespace ti = formats::vectrex::test_image;

CB_TEST("source.console_writer.vectrex.the_code_scan_finds_the_entry_and_the_call", [] {
    const formats::vectrex::Rom rom = formats::vectrex::parse(ti::cartridge());
    CB_CHECK((rom.functions(m6809_code::decoder()) == std::vector<std::uint32_t>{0x17, 0x25}));
});

CB_TEST("source.console_writer.vectrex.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_vec_writer");
    const image_writer::Written r = write_file_of(dir, ti::cartridge(), "demo.vec");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x17, "start"}, {0x25, "sub_0025"}}));
    CB_CHECK_EQ(r.image_bytes, 0x40u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"FCC \"g GCE 1982\"", "FDB $FD0D", "FCC \"TEST\"", "Wait_Recal    ", "EQU $F192", "VIA_int_enable", "JSR Wait_Recal", "STA VIA_int_enable", "BSR sub_0025", "BRA start", "start", "sub_0025"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.console_writer.vectrex.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_vec_edit");
    const image_writer::Written r = write_file_of(dir, ti::cartridge(), "demo.vec");
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("LDA #$7F");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 8, "LDA #$7E");
    Bytes expected = ti::cartridge();
    expected[0x1C - 1] = 0x7E;
    CB_CHECK(processors::assemble_described(processors::find("m6809"), dialect::find("asm6809").parse(s), ".").data == expected);
});

}  // namespace cb::source::console_writer
