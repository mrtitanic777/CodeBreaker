// cb-unity: alone (file-local names that other files in a batch also use)
// source/dreamcast_writer_test.cpp -- a Dreamcast disc written as GNU as source for the SH-4 and built again: IP.BIN's header field by field, the
// physical addresses and the cache bits of the program's pointers, the machine's registers by name, the FPU's instructions, little-endian data, a scrambled
// program written descrambled and put back scrambled in the disc the lane runs.
#include "source/dreamcast_writer.hpp"

#include <algorithm>
#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/dreamcast_test_programs.hpp"

namespace dw = cb::source::dreamcast_writer;
namespace dc = cb::formats::dreamcast;
namespace t = cb::source::dreamcast_test;
using cb::Bytes;

namespace {

bool has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

void keep(const cb::source::image_writer::Written& r, std::string_view name) {
    const auto folder = cb::fs::get_env("CB_SH2_KEEP");
    if (!folder || folder->empty()) return;
    cb::fs::make_dirs(cb::fs::from_text(*folder));
    cb::fs::write_file(cb::fs::from_text(*folder) / (std::string(name) + ".s"), cb::fs::read_file(r.source));
}

}  // namespace

CB_TEST("source.dreamcast_writer.a_disc_comes_back_identical_and_readable", [] {
    cb::fs::ScopedTempDir dir("cb_dc_writer");
    cb::fs::write_file(dir.path() / "game.iso", t::iso(false));
    const cb::source::image_writer::Written r = dw::reconstruct(dir.path() / "game.iso", dir.path() / "out");
    keep(r, "dreamcast");
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.functions.size() >= 4);   // the bootstrap, the program's start, `leaf` (a BSR target) and `syscall` (after a function's end)
    CB_CHECK(r.functions.contains(0x0C008300));
    CB_CHECK(r.functions.contains(0x0C010000));
    CB_CHECK_EQ(r.functions.at(0x0C010000), "first_read");
    // what the lane runs beside the original: a disc the reader takes, holding the same program
    CB_CHECK(r.built && r.built->extension() == ".iso");
    CB_CHECK(dc::read(*r.built).program == dc::read(dir.path() / "game.iso").program);

    const std::string source = cb::fs::read_text(r.source);
    CB_CHECK(has(source, "        .org 0x0C008000"));
    CB_CHECK(has(source, "        .org 0x0C010000"));
    CB_CHECK(has(source, "        .ascii \"SEGA SEGAKATANA \"    ! hardware ID"));   // the header, field by field
    CB_CHECK(has(source, "! maker ID"));
    CB_CHECK(has(source, "! boot file"));
    CB_CHECK(has(source, "        .ascii \"1ST_READ.BIN    \""));
    CB_CHECK(has(source, "ip_bootstrap:"));
    CB_CHECK(has(source, "        .long first_read + 0xA0000000"));                 // IP.BIN's pointer to the program, uncached
    CB_CHECK(has(source, "        .equ PVR_SPG_STATUS, 0xA05F810C"));               // a register of the machine is a name
    CB_CHECK(has(source, "        .long PVR_SPG_STATUS"));
    CB_CHECK(has(source, "        .equ var_8C800000, 0x8C800000"));                 // a variable in RAM
    CB_CHECK(has(source, "        .equ var_8C0000B0, 0x8C0000B0"));                 // (the BIOS's system-call vector)
    CB_CHECK(has(source, "        fldi1   fr0"));                                   // the FPU's instructions are decoded
    CB_CHECK(has(source, "        fadd    fr1,fr0"));
    CB_CHECK(has(source, "        fmov.s  fr0,@r4"));
    CB_CHECK(has(source, "        ftrc    fr0,fpul"));
    CB_CHECK(has(source, "        sts     fpul,r0"));
    CB_CHECK(has(source, "        mov.l   K_"));
    CB_CHECK(has(source, "        .asciz \"DREAMCAST\""));
    CB_CHECK(has(source, "        .long 0x12345678"));                              // data is little-endian: the longword reads as written
});

CB_TEST("source.dreamcast_writer.a_scrambled_program_is_written_descrambled_and_put_back", [] {
    cb::fs::ScopedTempDir dir("cb_dc_writer");
    const Bytes program = t::assemble(t::big_source());
    cb::fs::write_file(dir.path() / "game.iso", t::iso(program, true));
    const cb::source::image_writer::Written r = dw::reconstruct(dir.path() / "game.iso", dir.path() / "out");
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    std::cout << "big program: " << r.functions.size() << " functions\n";
    CB_CHECK(r.functions.size() >= 5);   // (functions only called through their neighbours are found a few rounds deep: see sh2_writer kRounds)
    const std::string source = cb::fs::read_text(r.source);
    CB_CHECK(has(source, "scrambled on the disc and written here descrambled"));
    // the rebuilt disc holds the program scrambled, as the original does
    const dc::Disc original = dc::read(dir.path() / "game.iso");
    const dc::Disc rebuilt = dc::read(*r.built);
    CB_CHECK(rebuilt.scrambled);
    CB_CHECK(rebuilt.first_read == original.first_read);
    CB_CHECK(rebuilt.program == program);
});

CB_TEST("source.dreamcast_writer.hardware_names_the_machines_registers", [] {
    const auto& h = dw::hardware();
    CB_CHECK_EQ(h.at(0x005F8000), "PVR_ID");
    CB_CHECK_EQ(h.at(0x005F810C), "PVR_SPG_STATUS");
    CB_CHECK_EQ(h.at(0x005F6900), "SB_ISTNRM");
    CB_CHECK_EQ(h.at(0x005F6C18), "SB_MDST");
    CB_CHECK_EQ(h.at(0x005F709C), "GD_STATUS");
    CB_CHECK_EQ(h.at(0x005F7800), "SB_ADSTAG");
    CB_CHECK_EQ(h.at(0x005F7840), "SB_E2STAG");
    CB_CHECK_EQ(h.at(0x00702C00 - 0x400), "AICA_MASTER_VOL");
    CB_CHECK_EQ(h.at(0x00700080), "AICA_CH1_00");
    CB_CHECK_EQ(h.at(0xFF00001C), "CCR");
    CB_CHECK_EQ(h.at(0xFF000010), "MMUCR");
    CB_CHECK_EQ(h.at(0xFFD80004), "TMU_TSTR");
});
