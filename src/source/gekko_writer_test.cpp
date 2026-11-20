// cb-unity: alone (file-local names that other files in a batch also use)
// Tests of the GameCube, Wii and Wii U writer over small programs of its own (source/gekko_test_images.hpp). The writer is held to what it is
// for -- the source it writes, built again, is the program it came from, byte for byte -- and to what the source says: the header field by
// field, functions found by calls, by their prologue and through a table, hardware registers by name, addresses built with @ha / @l.
#include "source/gekko_writer.hpp"

#include "build/mips_asm.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/gekko_test_images.hpp"
#include "source/isa_ppc.hpp"

namespace gw = cb::source::gekko_writer;
namespace gt = cb::source::gekko_test;
using std::string;

namespace {

bool contains(const string& text, std::string_view part) { return text.find(part) != string::npos; }

string source_of(const cb::source::image_writer::Written& r) { return cb::fs::read_text(r.source); }

cb::source::image_writer::Written write_to(cb::fs::ScopedTempDir& dir, const char* name, const cb::Bytes& bytes) {
    cb::fs::write_file(dir.path() / name, bytes);
    return gw::reconstruct(dir.path() / name, dir.path() / "out");
}

}  // namespace

CB_TEST("source.gekko_writer.a_dol_builds_back_into_itself", [] {
    cb::fs::ScopedTempDir dir("cb_gekko_writer");
    const cb::source::image_writer::Written r = write_to(dir, "game.dol", gt::dol());
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.built.has_value());
    CB_CHECK(cb::fs::read_file(*r.built) == gt::dol());
    // entry, helper (called), callback (named only by the table in the data)
    CB_CHECK_EQ(r.functions.size(), std::size_t{3});
    CB_CHECK_EQ(r.functions.at(gt::kText), string("entry"));
    CB_CHECK_EQ(r.functions.at(gt::kHelper), string("sub_80003150"));
    CB_CHECK_EQ(r.functions.at(gt::kCallback), string("sub_80003190"));
    CB_CHECK(r.code_bytes > 60);
    CB_CHECK_EQ(r.image_bytes, gt::dol().size());
});

CB_TEST("source.gekko_writer.the_source_says_what_the_dol_holds", [] {
    cb::fs::ScopedTempDir dir("cb_gekko_writer");
    const string text = source_of(write_to(dir, "game.dol", gt::dol()));
    // the header, a line to each table: offsets in the file, addresses as the labels of the sections, sizes as the difference of two labels
    CB_CHECK(contains(text, ".org 0x00000000    # dol header (file offset)"));
    CB_CHECK(contains(text, "# file offsets of text0-6"));
    CB_CHECK(contains(text, "text0, 0, 0, 0, 0, 0, 0    # load addresses of text0-6"));
    CB_CHECK(contains(text, "text0_end - text0, 0, 0, 0, 0, 0, 0    # sizes of text0-6"));
    CB_CHECK(contains(text, "data0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    # load addresses of data0-10"));
    CB_CHECK(contains(text, ".long 0x80005000, 0x00000200    # bss address and size"));
    CB_CHECK(contains(text, ".long entry    # entry point"));
    // the sections at their addresses
    CB_CHECK(contains(text, ".org 0x80003100    # text0"));
    CB_CHECK(contains(text, ".org 0x80004000    # data0"));
    CB_CHECK(contains(text, "text0:"));
    CB_CHECK(contains(text, "text0_end:"));
    // the code: functions are labels, an address the code builds is a name with @ha and @l, a call and a branch name their targets
    CB_CHECK(contains(text, "entry:"));
    CB_CHECK(contains(text, "lis r3,D_80004000@ha"));
    CB_CHECK(contains(text, "addi r3,r3,D_80004000@l"));
    CB_CHECK(contains(text, "bl sub_80003150"));
    CB_CHECK(contains(text, "bne entry"));
    CB_CHECK(contains(text, "stwu r1,-16(r1)"));
    CB_CHECK(contains(text, "sub_80003150:"));
    CB_CHECK(contains(text, "sub_80003190:"));
    // the hardware register is named, and the .equ defines it
    CB_CHECK(contains(text, "lis r5,VI_VTR@ha"));
    CB_CHECK(contains(text, "lwz r6,VI_VTR@l(r5)"));
    CB_CHECK(contains(text, ".equ VI_VTR, 0xCC002000"));
    // the data: the string as text, the table's words as the labels of the functions
    CB_CHECK(contains(text, "D_80004000:"));
    CB_CHECK(contains(text, ".asciz \"Hello, Cube!\""));
    CB_CHECK(contains(text, ".long sub_80003190, sub_80003150"));
});

CB_TEST("source.gekko_writer.an_elf_builds_back_into_itself", [] {
    cb::fs::ScopedTempDir dir("cb_gekko_writer");
    const cb::source::image_writer::Written r = write_to(dir, "game.elf", gt::elf());
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.same);
    CB_CHECK(cb::fs::read_file(*r.built) == gt::elf());
    CB_CHECK_EQ(r.functions.size(), std::size_t{3});
    const string text = cb::fs::read_text(r.source);
    CB_CHECK(contains(text, "# e_ident: ELF, 32-bit, big-endian, version, OS/ABI, ABI version"));
    CB_CHECK(contains(text, ".short 0x0002, 0x0014    # type, machine"));
    CB_CHECK(contains(text, ".long entry    # entry point"));
    CB_CHECK(contains(text, "# program header 0: type, offset, vaddr, paddr, file size, memory size, flags, alignment"));
    CB_CHECK(contains(text, "# section 1 .text: name, type, flags"));
    CB_CHECK(contains(text, ".org 0x80003100    # .text"));
    CB_CHECK(contains(text, ".org 0x80004000    # .data"));
    CB_CHECK(contains(text, "bl sub_80003150"));
});

CB_TEST("source.gekko_writer.an_rpx_is_written_decompressed", [] {
    cb::fs::ScopedTempDir dir("cb_gekko_writer");
    const cb::Bytes rpx = gt::elf(true);
    const cb::source::image_writer::Written r = write_to(dir, "game.rpx", rpx);
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.same);
    // what was rebuilt is the decompressed file: the .text bytes, not the stored zlib block, and a flag word that no longer says deflated
    const cb::Bytes rebuilt = cb::fs::read_file(*r.built);
    CB_CHECK(rebuilt != rpx);
    CB_CHECK_EQ(r.functions.size(), std::size_t{3});
    const string text = cb::fs::read_text(r.source);
    CB_CHECK(contains(text, "Wii U RPX (decompressed)"));
    CB_CHECK(contains(text, "bl sub_80003150"));
    CB_CHECK(contains(text, ".short 0xFE01, 0x0014    # type, machine"));
});

CB_TEST("source.gekko_writer.refuses_what_is_not_a_program", [] {
    cb::fs::ScopedTempDir dir("cb_gekko_writer");
    cb::fs::write_file(dir.path() / "junk.dol", cb::Bytes(300, 0x41));
    CB_CHECK_THROWS_AS(cb::FormatError, gw::reconstruct(dir.path() / "junk.dol", dir.path() / "out"));
});

// What is edited in the source comes out edited in the program: a changed instruction is one byte of the rebuilt file.
CB_TEST("source.gekko_writer.the_source_can_be_edited_and_built", [] {
    cb::fs::ScopedTempDir dir("cb_gekko_writer");
    const cb::source::image_writer::Written r = write_to(dir, "game.dol", gt::dol());
    string text = source_of(r);
    const std::size_t at = text.find("li r3,7");
    CB_CHECK(at != string::npos);
    text.replace(at, 7, "li r3,8");
    const cb::build::mips_asm::Assembler assembler(cb::source::isa::ppc_encoder(), cb::build::mips_asm::ByteOrder::Big);
    const cb::Bytes edited = assembler.build(text);
    const cb::Bytes original = gt::dol();
    CB_CHECK_EQ(edited.size(), original.size());
    std::vector<std::size_t> differing;
    for (std::size_t i = 0; i < original.size(); ++i)
        if (original[i] != edited[i]) differing.push_back(i);
    CB_CHECK_EQ(differing.size(), std::size_t{1});
    CB_CHECK_EQ(differing.front(), std::size_t{0x100 + (gt::kCallback - gt::kText) + 3});
    CB_CHECK_EQ(int{edited[differing.front()]}, 8);
});
