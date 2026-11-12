// Tests of the flat Z80 writer over small programs of its own. The programs are machine code written out byte by byte; the writer is
// held to what it is for -- the source it writes, built again, is the block it came from -- and to the text it writes, a golden file
// that was read through for correctness (testdata/writers/z80.golden.asm; the Python writer wrote the same bytes for this block).

#include "cb/source/z80_writer.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::source::z80_writer {
namespace {

// $8000 start: CALL $8010; LD HL,$8020; OUT ($FE),A; JP start
// $8010 sub:   LD ($5C08),A; RET
// $8020: a table of text
Bytes block() {
    Bytes image(0x40, 0);
    const Bytes code = {0xCD, 0x10, 0x80, 0x21, 0x20, 0x80, 0xD3, 0xFE, 0xC3, 0x00, 0x80};
    std::copy(code.begin(), code.end(), image.begin());
    const Bytes sub = {0x32, 0x08, 0x5C, 0xC9};
    std::copy(sub.begin(), sub.end(), image.begin() + 0x10);
    const std::string_view table = "HELLO";
    std::copy(table.begin(), table.end(), image.begin() + 0x20);
    return image;
}

const Machine kSpectrum{.ports = {{0xFE, "ULA"}}, .names = {{0x5C08, "LAST_K"}}};

std::string golden(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

// The text the writer wrote against the golden file. (CB_UPDATE_GOLDEN=1 writes the file instead, to be read through before it is checked in.)
void check_golden(std::string_view name, const std::string& actual) {
    if (fs::get_env("CB_UPDATE_GOLDEN")) fs::write_file(testing::data_path("source/testdata/writers/" + std::string(name)), as_bytes(actual));
    CB_CHECK_EQ(actual, golden(name));
}

CB_TEST("source.z80_writer.the_block_comes_back_as_source_that_builds_the_same_bytes", [] {
    fs::ScopedTempDir dir("cb_z80");
    const Bytes image = block();
    const Block b{.image = image, .origin = 0x8000, .entries = {{0x8000, "start"}}, .header = {"ZX Spectrum demo", "64 bytes at $8000"}};
    const image_writer::Written r = write(b, kSpectrum, dir.path(), "demo");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x8000, "start"}, {0x8010, "sub_8010"}}));   // the entry keeps its name; a call makes a function
    CB_CHECK_EQ(r.code_bytes, 15u);
    CB_CHECK_EQ(r.image_bytes, 0x40u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    CB_CHECK(r.built_image && *r.built_image == image);
    check_golden("z80.golden.asm", fs::read_text(r.source));
});

CB_TEST("source.z80_writer.the_golden_names_what_the_code_touches", [] {
    // Read for what it says: the machine's port and system variable are defined and used by name, the entry keeps its name, a pointer
    // loaded into HL is a data label, the bytes after code that the flow does not reach are data.
    const std::string text = golden("z80.golden.asm");
    for (const char* line : {"; ZX Spectrum demo", "; 64 bytes at $8000", "ULA              EQU $FE", "LAST_K           EQU $5C08", "        ORG $8000",
                             "start:", "        CALL sub_8010", "        LD HL,D_8020", "        OUT (ULA),A", "        JP start", "sub_8010:",
                             "        LD (LAST_K),A", "D_8020:", "        DEFB \"HELLO\"", "        DEFS 27,$00"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
});

CB_TEST("source.z80_writer.a_machine_names_only_what_the_code_uses", [] {
    fs::ScopedTempDir dir("cb_z80");
    const Bytes image = block();
    const Block b{.image = image, .origin = 0x8000, .entries = {{0x8000, "start"}}};
    const Machine rich{.ports = {{0xFE, "ULA"}, {0x1F, "KEMPSTON"}}, .names = {{0x5C08, "LAST_K"}, {0x5C3B, "FLAGS"}, {0x0D6B, "CLS"}}};
    const image_writer::Written r = write(b, rich, dir.path(), "demo");
    CB_CHECK(r.same);
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find("KEMPSTON") == std::string::npos && source.find("FLAGS") == std::string::npos && source.find("CLS") == std::string::npos);
    const image_writer::Written bare = write(b, Machine{}, dir.path() / "bare", "demo");   // no names at all: numbers, and it still builds back
    CB_CHECK(bare.same);
    CB_CHECK(fs::read_text(bare.source).find("        LD ($5C08),A") != std::string::npos);
    CB_CHECK(fs::read_text(bare.source).find("        OUT ($FE),A") != std::string::npos);
});

CB_TEST("source.z80_writer.a_call_makes_a_function_and_a_restart_does_not", [] {
    // $0000: RST $08; CALL NZ,$0010; RET      $0008: RET      $0010: RET
    Bytes image(0x20, 0xFF);
    const Bytes code = {0xCF, 0xC4, 0x10, 0x00, 0xC9};
    std::copy(code.begin(), code.end(), image.begin());
    image[0x08] = 0xC9;
    image[0x10] = 0xC9;
    const Block b{.image = image, .origin = 0, .entries = {{0, "start"}}};
    const image_writer::Walked<decompile::z80::Insn> w = walk(b);
    CB_CHECK((w.starts == std::vector<std::uint32_t>{0x0000, 0x0010}));   // the conditional call's target, not the restart's
    CB_CHECK((w.targets == std::set<std::uint32_t>{0x0008, 0x0010}));
    CB_CHECK_EQ(w.code.size(), 5u);   // RST, CALL NZ, RET; and the RET at each target
    fs::ScopedTempDir dir("cb_z80");
    const image_writer::Written r = write(b, Machine{}, dir.path(), "rst");
    CB_CHECK(r.same);
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find("L_0008:") != std::string::npos);   // the restart's target is a label, not a function
    CB_CHECK(source.find("sub_0010:") != std::string::npos);
    CB_CHECK(source.find("sub_0008") == std::string::npos);
});

CB_TEST("source.z80_writer.entries_outside_the_block_are_no_code", [] {
    const Bytes image = block();
    const Block b{.image = image, .origin = 0x8000, .entries = {{0x9000, "outside"}, {0x7FFF, "before"}}};
    const image_writer::Walked<decompile::z80::Insn> w = walk(b);
    CB_CHECK(w.code.empty());
    CB_CHECK(w.starts.empty());
});

CB_TEST("source.z80_writer.a_block_at_the_top_of_memory_is_cut_by_the_address_space", [] {
    const Bytes image(0x20, 0xC9);   // RET after RET, running off the end of memory at $10000
    const Block b{.image = image, .origin = 0xFFF0, .entries = {{0xFFF0, "start"}}};
    const image_writer::Walked<decompile::z80::Insn> w = walk(b);
    CB_CHECK_EQ(w.code.size(), 1u);   // the flow stops at the first RET; nothing past $FFFF is decoded
    CB_CHECK(w.code.contains(0xFFF0));
});

CB_TEST("source.z80_writer.a_prefix_that_does_nothing_is_written_as_bytes", [] {
    // $8000: DD 00 C9 -- a DD prefix before an opcode it does not change is a byte of its own, which an assembler cannot write as an instruction
    const Bytes image = {0xDD, 0x00, 0xC9};
    const Block b{.image = image, .origin = 0x8000, .entries = {{0x8000, "start"}}};
    fs::ScopedTempDir dir("cb_z80");
    const image_writer::Written r = write(b, Machine{}, dir.path(), "prefix");
    CB_CHECK(r.same);
    CB_CHECK(fs::read_text(r.source).find("        DEFB $DD") != std::string::npos);
});

}  // namespace
}  // namespace cb::source::z80_writer
