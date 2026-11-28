// cb-unity: alone
// The Channel F writer over a small cartridge: the source it writes, built again by CodeBreaker's F8 assembler, is the cartridge.

#include "formats/chf.hpp"
#include "source/console_shapes.hpp"

#include "source/dialect.hpp"
#include "source/processors.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/chf_test_image.hpp"

namespace cb::source::console_writer {
namespace {

// The bytes written to a file in `dir` and back as source through the console writer (config/platforms/chf.json).
[[maybe_unused]] image_writer::Written write_file_of(const fs::ScopedTempDir& dir, const Bytes& bytes, const char* name) {
    fs::write_file(dir.path() / name, bytes);
    return reconstruct("chf", dir.path() / name, dir.path());
}
}  // namespace

namespace ti = formats::chf::test_image;

CB_TEST("source.console_writer.chf.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_chf_writer");
    const image_writer::Written r = write_file_of(dir, ti::cartridge(), "demo.chf");
    CB_CHECK_EQ(r.notes.empty() ? std::string() : r.notes.front(), std::string());
    CB_CHECK(r.linked && r.same);
    CB_CHECK((r.functions == image_writer::Functions{{0x800, "start"}, {0x820, "sub_0820"}}));
    CB_CHECK_EQ(r.image_bytes, 0x400u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"ORG $0800", "start:", "sub_0820:", "L_0811:", "PI sub_0820", "BT 4,L_0811", "LR K,P", "LR R5,A", "LI $05", "FILL "}) {
        if (s.find(want) == std::string::npos) CB_CHECK_EQ(std::string("missing: ") + want, std::string());
    }
});

CB_TEST("source.console_writer.chf.a_call_into_the_bios_is_by_name", [] {
    Bytes rom = ti::cartridge();
    const Bytes call = {0x28, 0x06, 0x79};   // PI $0679
    std::copy(call.begin(), call.end(), rom.begin() + 0x10);
    fs::ScopedTempDir dir("cb_chf_bios");
    const image_writer::Written r = write_file_of(dir, rom, "demo.chf");
    CB_CHECK(r.linked && r.same);
    const std::string s = fs::read_text(r.source);
    CB_CHECK(s.find("BIOS_DRAWCHAR") != std::string::npos);
    CB_CHECK(s.find("EQU $0679") != std::string::npos);
});

CB_TEST("source.console_writer.chf.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_chf_edit");
    const image_writer::Written r = write_file_of(dir, ti::cartridge(), "demo.chf");
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("AI $03");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 6, "AI $04");
    Bytes expected = ti::cartridge();
    expected[0x22] = 0x04;
    CB_CHECK(processors::assemble_described(processors::find("f8"), dialect::find("asmf8").parse(s), ".").data == expected);
});

}  // namespace cb::source::console_writer
