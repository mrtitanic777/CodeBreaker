// cb-unity: alone
// The Intellivision writer over a small cartridge: the source it writes, built again by CodeBreaker's CP1610 assembler, is the cartridge file.

#include "source/intv_writer.hpp"

#include "build/cp1610_asm.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/intv_test_image.hpp"

namespace cb::source::intv_writer {

namespace ti = formats::intv::test_image;

CB_TEST("source.intv_writer.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_intv_writer");
    const image_writer::Written r = write(formats::intv::parse(ti::file()), "demo.rom", dir.path());
    CB_CHECK_EQ(r.notes.empty() ? std::string() : r.notes.front(), std::string());
    CB_CHECK(r.linked && r.same);
    CB_CHECK((r.functions == image_writer::Functions{{0x5020, "start"}, {0x5030, "sub_5030"}}));
    CB_CHECK_EQ(r.image_bytes, 256u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"SEGMENT $50,$50", "ORG $5000", "EQU $0020", "MVO R0,STIC_DISPLAY_ENABLE", "JSR R4,sub_5030", "SDBD", "MVII #$1234,R1", "start:", "sub_5030:",
                             "ROMTAIL $00,$11,$22,$33", "FILL ", "MOVR R4,R7"}) {
        if (s.find(want) == std::string::npos) CB_CHECK_EQ(std::string("missing: ") + want, std::string());
    }
});

CB_TEST("source.intv_writer.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_intv_edit");
    const image_writer::Written r = write(ti::rom(), "demo.rom", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("INCR R0");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 7, "DECR R0");
    formats::intv::Rom expected = ti::rom();
    expected.segments[0].words[0x30] = 0x10;
    CB_CHECK(build::cp1610_asm::build(s) == expected.serialize(true));
});

CB_TEST("source.intv_writer.the_hardware_registers_have_names", [] {
    CB_CHECK_EQ(hardware().at(0x20), "STIC_DISPLAY_ENABLE");
    CB_CHECK_EQ(hardware().at(0x2C), "STIC_BORDER_COLOR");
    CB_CHECK_EQ(hardware().at(0x01FA), "PSG_MIXER");
    CB_CHECK_EQ(hardware().at(0x01FE), "PSG_IO_PORT_0");
    CB_CHECK(!hardware().contains(0x0100));
});

}  // namespace cb::source::intv_writer
