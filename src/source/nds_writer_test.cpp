// Tests of the Nintendo DS writer over a small ROM of its own (nds_test_image.hpp: an ARMv5TE program and an ARM7TDMI program, the name table, the
// allocation table, a banner and a file). The writer is held to what it is for: the source it writes, built again, is the ROM it came from, with
// both programs read in their own instruction set.
#include "source/nds_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/nds_test_image.hpp"

namespace cb::source::nds_writer {
namespace {

bool has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

CB_TEST("source.nds_writer.the_rom_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_nds");
    const Bytes image = nds_test::test_rom();
    const image_writer::Written r = write(formats::nds::parse(image), "demo.nds", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK_EQ(r.image_bytes, image.size());
    CB_CHECK(r.built && fs::read_file(*r.built) == image);
    // ARM9: the entry, `count` (a bl), the Thumb function through the literal pool and blx, the one a blx names; ARM7: the entry and its helper
    CB_CHECK_EQ(r.functions.at(0x02000000), std::string("arm9_entry"));
    CB_CHECK_EQ(r.functions.at(0x037F8000), std::string("arm7_entry"));
    std::size_t thumb = 0, nine = 0, seven = 0;
    for (const auto& [address, name] : r.functions) {
        thumb += (address & 1) ? 1 : 0;
        nine += name.starts_with("arm9_") ? 1 : 0;
        seven += name.starts_with("arm7_") ? 1 : 0;
    }
    CB_CHECK_EQ(thumb, std::size_t{2});
    CB_CHECK_EQ(nine, std::size_t{4});
    CB_CHECK_EQ(seven, std::size_t{2});
});

CB_TEST("source.nds_writer.both_programs_and_the_file_system_are_written_as_what_they_are", [] {
    fs::ScopedTempDir dir("cb_nds");
    const image_writer::Written r = write(formats::nds::parse(nds_test::test_rom()), "demo.nds", dir.path());
    const std::string source = fs::read_text(r.source);
    // each program has its own instruction set and its own names
    CB_CHECK(has(source, "        .cpu arm946e-s"));
    CB_CHECK(has(source, "        .cpu arm7tdmi"));
    CB_CHECK(has(source, "clz r0, r0"));
    CB_CHECK(has(source, "blx r1"));
    CB_CHECK(has(source, "0xFA000007  @ blx 0x2000044"));   // (the description's `blx` to a Thumb address does not assemble back: the bits, with the instruction)
    CB_CHECK(has(source, "ldrd r2, r3, [r4, #8]"));
    CB_CHECK(has(source, "mcr p15, 0, r0, c7, c10, 4"));
    CB_CHECK(has(source, ".word arm9_thumb_") && has(source, " + 1"));
    CB_CHECK(has(source, "bl arm7_arm_"));
    CB_CHECK(has(source, ".asciz \"ARM9 DATA\""));
    // the register of the console, by its libnds name
    CB_CHECK(has(source, ".equ REG_IME, 0x04000208"));
    CB_CHECK(has(source, ".word REG_IME"));
    // the header says where the programs are with labels
    CB_CHECK(has(source, "arm9_rom"));
    CB_CHECK(has(source, ".word arm9_end - arm9"));
    CB_CHECK(has(source, ".word arm7_end - arm7"));
    CB_CHECK(has(source, ".word arm9_entry"));
    CB_CHECK(has(source, "@ the Nintendo logo"));
    CB_CHECK(has(source, ".ascii \"NDSTEST\""));
    CB_CHECK(has(source, ".ascii \"ATSE\""));
    // the file system
    CB_CHECK(has(source, "fnt:"));
    CB_CHECK(has(source, "hello.txt"));
    CB_CHECK(has(source, "fat:"));
    CB_CHECK(has(source, ".word file_0, file_0_end   @ file 0 hello.txt"));
    CB_CHECK(has(source, "file_0:   @ hello.txt"));
    CB_CHECK(has(source, "Hello from the DS file system."));
    CB_CHECK(has(source, "@ the title in English"));
    CB_CHECK(has(source, "@ Test\\nCod"));
    CB_CHECK(has(source, ".word banner"));
});

CB_TEST("source.nds_writer.whatever_a_rom_holds_it_comes_back_as_itself", [] {
    fs::ScopedTempDir dir("cb_nds");
    Bytes edited = nds_test::test_rom();
    edited[0x4000] ^= 1;   // the first byte of the ARM9 program
    const image_writer::Written r = write(formats::nds::parse(edited), "edited.nds", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.built && fs::read_file(*r.built) == edited);
});

CB_TEST("source.nds_writer.the_hardware_names_differ_between_the_processors", [] {
    CB_CHECK_EQ(hardware(formats::nds::Cpu::Arm9).at(0x04000000), std::string("REG_DISPCNT"));
    CB_CHECK_EQ(hardware(formats::nds::Cpu::Arm9).at(0x04001000), std::string("REG_DISPCNT_SUB"));
    CB_CHECK_EQ(hardware(formats::nds::Cpu::Arm9).at(0x04000280), std::string("REG_DIVCNT"));
    CB_CHECK(!hardware(formats::nds::Cpu::Arm7).contains(0x04000280));
    CB_CHECK_EQ(hardware(formats::nds::Cpu::Arm7).at(0x040001C2), std::string("REG_SPIDATA"));
    CB_CHECK_EQ(hardware(formats::nds::Cpu::Arm7).at(0x04000410), std::string("REG_SCHANNEL1_CR"));
    CB_CHECK_EQ(hardware(formats::nds::Cpu::Arm9).at(0x04000208), std::string("REG_IME"));
    CB_CHECK_EQ(hardware(formats::nds::Cpu::Arm7).at(0x04000208), std::string("REG_IME"));
});

}  // namespace
}  // namespace cb::source::nds_writer
