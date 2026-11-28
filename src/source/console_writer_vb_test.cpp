// cb-unity: alone
// The Virtual Boy writer over a small cartridge: the source it writes, built again by the one assembler (the V810 SLEIGH description), is the cartridge.

#include "formats/vb.hpp"
#include "knowledge/package.hpp"
#include "source/console_writer.hpp"

#include "source/dialect.hpp"
#include "source/processors.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/vb_test_image.hpp"

namespace cb::source::console_writer::vb_test {
namespace {

// The cartridge written to `folder` as `name` and its source written beside it.
image_writer::Written write(const Bytes& file, std::string_view name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    fs::write_file(folder / std::string(name), file);
    return console_writer::reconstruct("vb", folder / std::string(name), folder);
}
const image_writer::Names& hardware() {   // (config/knowledge/vb/registers.json)
    static const image_writer::Names table = knowledge::standard().hardware_names("vb", describe("vb").vectored.registers);
    return table;
}

}  // namespace

namespace ti = formats::vb::test_image;

CB_TEST("source.console_writer.vb.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_vb_cw");
    const image_writer::Written r = write(ti::cartridge(), "demo.vb", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x07000030, "sub_07000030"}, {0x07003E40, "vector_vip"}, {0x07003FF0, "reset"}}));
    CB_CHECK_EQ(r.image_bytes, 0x4000u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {".ascii \"TEST CART           \"", ".equ VIP_INTPND, 0x0005F800", "movhi hi(VIP_INTPND),r0,r8", "ld.h lo(VIP_INTPND)[r8],r9", "jal sub_07000030", "jr L_07000000", "reti", "reset:",
                             "sub_07000030:", ".org 0x07000000", ".fill "}) {
        if (s.find(want) == std::string::npos) CB_CHECK_EQ(std::string("missing: ") + want, std::string());
    }
});

CB_TEST("source.console_writer.vb.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_vb_edit");
    const image_writer::Written r = write(ti::cartridge(), "demo.vb", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("shl 1,r10");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 9, "shl 2,r10");
    Bytes expected = ti::cartridge();
    expected[0x32] = 0x42;
    CB_CHECK(processors::assemble_described(processors::find("v810"), dialect::find("v810").parse(s), ".").data == expected);
});

CB_TEST("source.console_writer.vb.the_hardware_registers_have_names", [] {
    CB_CHECK_EQ(hardware().at(0x5F822), "VIP_DPCTRL");
    CB_CHECK_EQ(hardware().at(0x01000400), "VSU_S1INT");
    CB_CHECK_EQ(hardware().at(0x0100051C), "VSU_S5SWP");
    CB_CHECK_EQ(hardware().at(0x02000028), "HW_SCR");
    CB_CHECK(!hardware().contains(0x01000598));
});

}  // namespace cb::source::console_writer::vb_test
