// cb-unity: alone (file-local names that other files in a batch also use)
// Tests of the Nintendo 64 writer over a small ROM of its own (source/n64_test_rom.hpp). The writer is held to what it is for -- the source it writes,
// built again, is the ROM it came from, byte for byte, in whichever byte order the file had -- and to what the source says: the header field by
// field, the boot code as data, functions found by flow and through a table, hardware registers by name, branch likely, doubleword and FPU
// instructions, ERET ending a function.
#include "source/n64_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "source/n64_test_rom.hpp"

namespace nw = cb::source::n64_writer;
namespace nt = cb::source::n64_test;
namespace n = cb::formats::n64;
using std::string;

namespace {

bool contains(const string& text, std::string_view part) { return text.find(part) != string::npos; }

string source_of(const cb::source::image_writer::Written& r) { return cb::fs::read_text(r.source); }

}  // namespace

CB_TEST("source.n64_writer.the_source_builds_back_into_the_rom", [] {
    cb::fs::ScopedTempDir dir("cb_n64_writer");
    cb::fs::write_file(dir.path() / "game.z64", nt::test_rom());
    const cb::source::image_writer::Written r = nw::reconstruct(dir.path() / "game.z64", dir.path() / "out");
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.built.has_value());
    // entry, print (called), helper and handler (only in the table: found by their prologue and by ERET ending one)
    CB_CHECK_EQ(r.functions.size(), std::size_t{4});
    CB_CHECK_EQ(r.functions.begin()->second, string("entry"));
    CB_CHECK_EQ(r.functions.begin()->first, 0x80001000u);
    CB_CHECK(r.code_bytes > 100);
    CB_CHECK_EQ(r.image_bytes, nt::test_rom().size());
    CB_CHECK(cb::fs::read_file(*r.built) == nt::test_rom());
});

CB_TEST("source.n64_writer.the_source_says_what_the_rom_holds", [] {
    cb::fs::ScopedTempDir dir("cb_n64_writer");
    cb::fs::write_file(dir.path() / "game.z64", nt::test_rom());
    const string text = source_of(nw::reconstruct(dir.path() / "game.z64", dir.path() / "out"));
    // the header, field by field
    CB_CHECK(contains(text, ".word 0x80371240    ; PI bus configuration"));
    CB_CHECK(contains(text, ".word entry    ; entry point"));
    CB_CHECK(contains(text, "; CRC1"));
    CB_CHECK(contains(text, "CRCs not checked"));   // the test ROM is shorter than the megabyte the CRCs cover: they are kept as they are
    CB_CHECK(contains(text, ".ascii \"N64 LANE TEST       \"    ; name"));
    CB_CHECK(contains(text, ".ascii \"NT\"    ; game code"));
    // the boot code is data, with its text found in it
    CB_CHECK(contains(text, "IPL3 STAND-IN FOR THE TESTS"));
    // the program: addresses the code builds are labels or hardware names, the instructions are the R4300's
    CB_CHECK(contains(text, ".org 0x80001000"));
    CB_CHECK(contains(text, "entry:"));
    CB_CHECK(contains(text, "daddiu sp, sp, -0x20"));
    CB_CHECK(contains(text, "sd ra, 0x18(sp)"));
    CB_CHECK(contains(text, "lui a0, %hi(D_"));
    CB_CHECK(contains(text, "lui t0, %hi(MI_INTR_MASK)"));
    CB_CHECK(contains(text, "lw t1, %lo(MI_INTR_MASK)(t0)"));
    CB_CHECK(contains(text, ".equ MI_INTR_MASK, 0xA430000C"));
    CB_CHECK(contains(text, "VI_ORIGIN"));
    CB_CHECK(contains(text, "PI_STATUS"));
    CB_CHECK(contains(text, "bnel t4, zero, L_"));
    CB_CHECK(contains(text, "lwc1 f0, %lo(D_"));
    CB_CHECK(contains(text, "add.S f2, f0, f0"));
    CB_CHECK(contains(text, "bc1fl L_"));
    CB_CHECK(contains(text, "eret"));
    CB_CHECK(contains(text, "dsll32 v0, a0, 0x4"));
    // the data: the string, the table of the four functions as labels, the word that is no instruction, the run of zeros
    CB_CHECK(contains(text, ".asciz \"HELLO, NINTENDO 64\""));
    CB_CHECK(contains(text, ".word 0x3F800000, entry, sub_"));
    CB_CHECK(contains(text, "0xFFFFFFFF"));
    CB_CHECK(contains(text, ".space 32"));
    // each function is a fragment of the L2 module, and the head says what the ROM is
    CB_CHECK(contains(text, "Nintendo 64 ROM \"N64 LANE TEST\" (NTE)"));
    CB_CHECK(contains(text, "boot code unknown boot code"));
});

CB_TEST("source.n64_writer.a_v64_and_an_n64_file_rebuild_in_their_own_byte_order", [] {
    cb::fs::ScopedTempDir dir("cb_n64_writer");
    cb::Bytes z64_source;
    for (const n::ByteOrder order : {n::ByteOrder::V64, n::ByteOrder::N64}) {
        const string name = order == n::ByteOrder::V64 ? "game.v64" : "game.n64";
        cb::fs::write_file(dir.path() / name, nt::test_rom(order));
        const cb::source::image_writer::Written r = nw::reconstruct(dir.path() / name, dir.path() / ("out_" + name));
        CB_CHECK(r.linked && r.same);
        CB_CHECK(r.built->extension() == (order == n::ByteOrder::V64 ? ".v64" : ".n64"));
        CB_CHECK(cb::fs::read_file(*r.built) == nt::test_rom(order));
        const string text = source_of(r);
        CB_CHECK(contains(text, "the rebuilt file is made in the file's own order"));
        CB_CHECK(contains(text, ".word 0x80371240    ; PI bus configuration"));   // the source is the console's order whatever the file's
    }
});

CB_TEST("source.n64_writer.what_is_past_the_first_megabyte_is_data_at_its_cartridge_address", [] {
    cb::fs::ScopedTempDir dir("cb_n64_writer");
    cb::fs::write_file(dir.path() / "big.z64", nt::test_rom(n::ByteOrder::Z64, 0x4000));
    const cb::source::image_writer::Written r = nw::reconstruct(dir.path() / "big.z64", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    const string text = source_of(r);
    CB_CHECK(contains(text, ".org 0xB0101000"));
    CB_CHECK(contains(text, ".space 16384"));
    CB_CHECK(contains(text, ".word 0xCAFEBABE"));
    CB_CHECK_EQ(r.image_bytes, std::size_t{0x101000 + 0x4000 + 4});
});

CB_TEST("source.n64_writer.the_hardware_table_names_the_consoles_registers", [] {
    const auto& table = nw::hardware();
    CB_CHECK_EQ(table.at(0x0430000C), string("MI_INTR_MASK"));
    CB_CHECK_EQ(table.at(0x04400004), string("VI_ORIGIN"));
    CB_CHECK_EQ(table.at(0x04600010), string("PI_STATUS"));
    CB_CHECK_EQ(table.at(0x04040010), string("SP_STATUS"));
    CB_CHECK_EQ(table.at(0x04100000), string("DPC_START"));
    CB_CHECK_EQ(table.at(0x04800018), string("SI_STATUS"));
    CB_CHECK_EQ(table.at(0x04500004), string("AI_LEN"));
    CB_CHECK_EQ(table.at(0x04700000), string("RI_MODE"));
});

CB_TEST("source.n64_writer.an_edited_program_is_not_the_original", [] {
    // one instruction changed in the ROM's own source: the writer still gives source that builds back into THAT rom, and it is not the original
    cb::fs::ScopedTempDir dir("cb_n64_writer");
    string changed = nt::program();
    changed.replace(changed.find("dsll32 $v0, $a0, 4"), 18, "dsll32 $v0, $a0, 5");
    cb::fs::write_file(dir.path() / "a.z64", nt::test_rom());
    cb::fs::write_file(dir.path() / "b.z64", nt::test_rom(n::ByteOrder::Z64, 0, changed));
    const cb::source::image_writer::Written a = nw::reconstruct(dir.path() / "a.z64", dir.path() / "out_a");
    const cb::source::image_writer::Written b = nw::reconstruct(dir.path() / "b.z64", dir.path() / "out_b");
    CB_CHECK(a.same && b.same);
    CB_CHECK(cb::fs::read_file(*a.built) != cb::fs::read_file(*b.built));
    CB_CHECK(contains(source_of(b), "dsll32 v0, a0, 0x5"));
});
