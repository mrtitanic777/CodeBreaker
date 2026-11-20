// Tests of the SH-2 writers over programs made in the test (source/sh2_test_programs.hpp): the generic writer, the 32X cartridge writer and
// the Saturn disc writer. Each is held to what it is for -- the source it writes, built again, is the image it came from -- and to the text it
// writes: functions found by calls, by constants and by their prologues, PC-relative constants named, registers named, the header fields.
#include "source/sh2_writer.hpp"

#include <algorithm>
#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/saturn.hpp"
#include "source/saturn_writer.hpp"
#include "source/sega32x_writer.hpp"
#include "source/sh2_test_programs.hpp"

namespace cb::source {
namespace {

namespace t = sh2_test;

bool has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

// CB_SH2_KEEP=<folder>: the written source is kept there, to be looked at.
void keep(const image_writer::Written& r, std::string_view name) {
    const auto folder = fs::get_env("CB_SH2_KEEP");
    if (!folder || folder->empty()) return;
    fs::make_dirs(fs::from_text(*folder));
    fs::write_file(fs::from_text(*folder) / (std::string(name) + ".s"), fs::read_file(r.source));
}

CB_TEST("source.sh2_writer.a_program_comes_back_identical_and_readable", [] {
    fs::ScopedTempDir dir("sh2-writer");
    const std::map<std::uint32_t, std::string> hardware = {{0x4020, "COMM0"}};
    sh2_writer::Image image;
    image.bytes = t::assemble(t::kSdramProgram);
    image.parts = {{"program", 0, static_cast<std::uint32_t>(image.bytes.size()), 0x06000000, true, true}};
    image.entries = {0x06000040};
    image.names[0x06000040] = "entry";
    image.hardware = &hardware;
    image.head = {"! test"};
    const image_writer::Written r = sh2_writer::write(image, "demo", dir.path(), "demo.built.bin");
    keep(r, "demo");
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK(fs::read_file(*r.built) == image.bytes);
    // `entry`, `tally` (a BSR target) and `helper` (only its address is in the code: a constant a load reads)
    CB_CHECK_EQ(r.functions.size(), 3u);
    CB_CHECK_EQ(r.functions.at(0x06000040), "entry");
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has(source, "        .org 0x06000000"));
    CB_CHECK(has(source, "        .long entry, 0x0603FFFC, entry, 0x0603FFFC"));      // the vectors, the handlers by name
    CB_CHECK(has(source, "        .space 48"));
    CB_CHECK(has(source, "entry:"));
    CB_CHECK(has(source, "        mov.l   r14,@-r15"));
    CB_CHECK(has(source, "        bf      L_"));                                       // a jump target is a label
    CB_CHECK(has(source, "        bsr     sub_"));
    CB_CHECK(has(source, "        jsr     @r0"));
    CB_CHECK(has(source, "        .equ COMM0, 0x20004020"));                           // a register of the machine is a name
    CB_CHECK(has(source, "        .long COMM0"));
    CB_CHECK(has(source, "        mov.l   K_0600"));                                   // a constant is a label ...
    CB_CHECK(has(source, "        .long sub_"));                                       // ... and a pointer to a function is its name
    CB_CHECK(has(source, "        .long 0x0000000A"));                                // a number is a number
    CB_CHECK(has(source, "        mov.w   K_0600"));
    CB_CHECK(has(source, "        .word 0x00FF"));
    CB_CHECK(has(source, "        .asciz \"hello, 32x\""));
    CB_CHECK(has(source, "        .long entry, sub_"));                                // the table of the three functions
    CB_CHECK(has(source, "! 3 functions,"));
});

CB_TEST("source.sh2_writer.data_is_data_and_the_rebuild_is_still_the_image", [] {
    fs::ScopedTempDir dir("sh2-writer");
    sh2_writer::Image image;
    for (unsigned i = 0; i < 4096; ++i) image.bytes.push_back(static_cast<std::uint8_t>((i * 7 + (i >> 3) * 13) ^ (i >> 5)));
    for (unsigned i = 0; i < 64; ++i) image.bytes[1000 + i] = 0xAB;   // a run of one value
    image.parts = {{"all", 0, 4096, 0x06000000, true, true}};
    // (no entry points: none of it is code)
    const image_writer::Written r = sh2_writer::write(image, "noise", dir.path(), "noise.built.bin");
    keep(r, "noise");
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(has(fs::read_text(r.source), ".fill 64, 1, 0xAB"));
});

CB_TEST("source.sh2_writer.parts_at_their_own_addresses", [] {
    // two code parts and a data part between them, the data written through `special`
    fs::ScopedTempDir dir("sh2-writer");
    const Bytes a = t::assemble("        .org 0x06002000\nf: mov #1,r0\n   rts\n   nop\n");
    const Bytes b = t::assemble("        .org 0x06004000\ng: mov #2,r0\n   bsr h\n   nop\n   rts\n   nop\nh: rts\n   nop\n");
    sh2_writer::Image image;
    image.bytes = a;
    image.bytes.insert(image.bytes.end(), {0x11, 0x22, 0x33, 0x44});
    image.bytes.insert(image.bytes.end(), b.begin(), b.end());
    image.parts = {{"first", 0, static_cast<std::uint32_t>(a.size()), 0x06002000, true, true},
                   {"middle", static_cast<std::uint32_t>(a.size()), 4, 0x06003000, false, true},
                   {"second", static_cast<std::uint32_t>(a.size() + 4), static_cast<std::uint32_t>(b.size()), 0x06004000, true, true}};
    image.entries = {0x06002000, 0x06004000};
    image.names = {{0x06002000, "f"}, {0x06004000, "g"}};
    const std::uint32_t middle = static_cast<std::uint32_t>(a.size());
    image.special = [middle](std::uint32_t off, std::vector<std::string>& lines) -> std::uint32_t {
        if (off != middle) return 0;
        lines.push_back("        .long 0x11223344    ! a field");
        return 4;
    };
    const image_writer::Written r = sh2_writer::write(image, "parts", dir.path(), "parts.built.bin");
    keep(r, "parts");
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.functions.size(), 3u);   // f, g and the BSR target
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has(source, "        .org 0x06002000"));
    CB_CHECK(has(source, "        .org 0x06003000"));
    CB_CHECK(has(source, "        .org 0x06004000"));
    CB_CHECK(has(source, "        .long 0x11223344    ! a field"));
});

CB_TEST("source.sega32x_writer.a_cartridge_comes_back_as_two_sources", [] {
    fs::ScopedTempDir dir("sega32x-writer");
    const Bytes image = t::cartridge_32x();
    fs::write_file(dir.path() / "game.32x", image);
    const image_writer::Written r = sega32x_writer::reconstruct(dir.path() / "game.32x", dir.path() / "out");
    keep(r, "sega32x");
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK(fs::read_file(*r.built) == image);
    CB_CHECK_EQ(r.source.filename().string(), "game.sh2.s");
    CB_CHECK(std::filesystem::exists(dir.path() / "out" / "game.asm"));
    // the SH-2's functions are at $06000000 and up, the 68000's below $400000
    CB_CHECK_EQ(r.functions.at(0x06000040), "master_entry");
    CB_CHECK(r.functions.contains(0x3F0));
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has(source, "        .equ MARS_SYS_COMM0, 0x20004020"));
    CB_CHECK(has(source, "        .long MARS_SYS_COMM0"));
    CB_CHECK(has(source, "master_entry:"));
    CB_CHECK(has(source, "\"TESTROM32X\""));
    // an edit to the SH-2 source changes the cartridge the sources build
    std::string edited = source;
    const std::size_t at = edited.find("        add     #3,r0");
    CB_CHECK(at != std::string::npos);
    edited.replace(at, std::string("        add     #3,r0").size(), "        add     #4,r0");
    const Bytes rebuilt = build::sh_asm::Assembler(isa::sh2_encoder()).build(edited);
    CB_CHECK(rebuilt.size() == t::assemble(t::kSdramProgram).size());
    CB_CHECK(rebuilt != t::assemble(t::kSdramProgram));
});

CB_TEST("source.saturn_writer.a_disc_comes_back_as_one_source", [] {
    fs::ScopedTempDir dir("saturn-writer");
    const Bytes disc = t::disc_saturn();
    fs::write_file(dir.path() / "game.iso", disc);
    const image_writer::Written r = saturn_writer::reconstruct(dir.path() / "game.iso", dir.path() / "out");
    keep(r, "saturn");
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.notes.empty());
    const Bytes program = t::assemble(t::kSaturnProgram);
    CB_CHECK(fs::read_file(dir.path() / "out" / "game.built.bin") == t::disc_image(disc, program.size()));
    // what the lane runs is a disc of its own that holds them: the reader finds the same system area and first program in it
    CB_CHECK_EQ(r.built->filename().string(), "game.built.iso");
    const formats::saturn::Disc rebuilt = formats::saturn::read(*r.built);
    const formats::saturn::Disc original = formats::saturn::parse(disc);
    CB_CHECK(rebuilt.system == original.system);
    CB_CHECK(rebuilt.first_read == original.first_read);
    CB_CHECK_EQ(rebuilt.first_read_name, "0.BIN");
    for (const char* name : {"ip_area_jp", "ip_area_tw", "ip_area_us", "ip_area_eu", "ip_init", "first_read"}) {
        const bool found = std::any_of(r.functions.begin(), r.functions.end(), [&](const auto& f) { return f.second == name; });
        if (!found) std::cout << "missing function " << name << "\n";
        CB_CHECK(found);
    }
    for (const auto& [a, n] : r.functions) std::cout << std::hex << a << " " << n << "\n";
    CB_CHECK_EQ(r.functions.at(0x06004000), "first_read");
    CB_CHECK_EQ(r.functions.at(0x06002E80), "ip_init");
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has(source, "        .ascii \"SEGA SEGASATURN \"    ! hardware ID"));
    CB_CHECK(has(source, "! game title"));
    CB_CHECK(has(source, "        .long 0x00001000    ! IP size"));
    CB_CHECK(has(source, "        .long 0x06004000    ! first program: load address"));
    CB_CHECK(has(source, "        .org 0x06002000"));
    CB_CHECK(has(source, "        .org 0x06004000"));
    CB_CHECK(has(source, "ip_area_jp:"));
    CB_CHECK(has(source, "        bra     ip_init"));
    CB_CHECK(has(source, "        .equ VDP2_TVSTAT, 0x25F80004"));
    CB_CHECK(has(source, "        .equ SCU_IST, 0x25FE00A4"));
    CB_CHECK(has(source, "        .equ var_06080000, 0x06080000"));
    CB_CHECK(has(source, "        .long VDP2_TVSTAT"));
    CB_CHECK(has(source, "        .asciz \"SATURN\""));
    CB_CHECK(has(source, "        .long first_read"));   // the initial program's constant is the first program's address
});

}  // namespace
}  // namespace cb::source
