// cb-unity: alone
// The Virtual Boy writer over a small cartridge: the source it writes, built again by the one assembler (the V810 SLEIGH description), is the cartridge.

#include "source/vb_writer.hpp"

#include "source/processors.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/vb_test_image.hpp"

namespace cb::source::vb_writer {

namespace ti = formats::vb::test_image;

CB_TEST("source.vb_writer.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_vb_writer");
    const image_writer::Written r = write(formats::vb::parse(ti::cartridge()), "demo.vb", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x07000030, "sub_07000030"}, {0x07003E40, "vector_vip"}, {0x07003FF0, "reset"}}));
    CB_CHECK_EQ(r.image_bytes, 0x4000u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {".ascii \"TEST CART           \"", "movhi 0x6,r0,r8", "ld.h -0x800[r8],r9", "; jal 0x7000030", "; jr 0x7000000", "reti", "reset:",
                             "sub_07000030:", ".org 0x07000000", ".space "}) {
        if (s.find(want) == std::string::npos) CB_CHECK_EQ(std::string("missing: ") + want, std::string());
    }
});

CB_TEST("source.vb_writer.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_vb_edit");
    const image_writer::Written r = write(formats::vb::parse(ti::cartridge()), "demo.vb", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("shl 0x1,r10");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 11, "shl 0x2,r10");
    Bytes expected = ti::cartridge();
    expected[0x32] = 0x42;
    CB_CHECK(processors::assemble_described(processors::find("v810"), s, ".").data == expected);
});

CB_TEST("source.vb_writer.the_hardware_registers_have_names", [] {
    CB_CHECK_EQ(hardware().at(0x5F822), "VIP_DPCTRL");
    CB_CHECK_EQ(hardware().at(0x01000400), "VSU_S1INT");
    CB_CHECK_EQ(hardware().at(0x0100051C), "VSU_S5SWP");
    CB_CHECK_EQ(hardware().at(0x02000028), "HW_SCR");
    CB_CHECK(!hardware().contains(0x01000598));
});

}  // namespace cb::source::vb_writer
