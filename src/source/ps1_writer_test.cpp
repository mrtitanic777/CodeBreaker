// Tests of the PlayStation writer over a small executable of its own. The program is assembled from GNU as source (with CodeBreaker's
// own assembler); the writer is held to what it is for -- the source it writes, built again, is the executable it came from -- and to
// the text it writes, a golden file that was read through for correctness (testdata/writers/ps1.golden.s).

#include "source/ps1_writer.hpp"

#include "build/mips_asm.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "source/isa.hpp"
#include "source/risc_source.hpp"

namespace cb::source::ps1_writer {
namespace {

// The program: `entry` calls `print`, polls a hardware register, loops; `helper` is never called (it is found by its prologue after
// print's return); the data is a string, a table of the three functions, a word that is no instruction, and a run of zeros.
constexpr std::string_view kProgram = R"(
entry:  addiu $sp, $sp, -24
        sw    $ra, 20($sp)
        lui   $a0, %hi(msg)
        jal   print
        addiu $a0, $a0, %lo(msg)
        lui   $t0, 0x1F80
        lw    $t1, 0x1814($t0)
        nop
        lui   $t2, 0xE100
        sw    $t2, 0x1810($t0)
        beq   $v0, $zero, done
        nop
loop:   addiu $v0, $v0, -1
        bne   $v0, $zero, loop
        nop
done:   lw    $ra, 20($sp)
        addiu $sp, $sp, 24
        jr    $ra
        nop
print:  jr    $ra
        move  $v0, $a0
helper: addiu $sp, $sp, -8
        sw    $a0, 0($sp)
        lui   $v0, %hi(counter)
        lw    $v1, %lo(counter)($v0)
        nop
        addiu $v1, $v1, 1
        sw    $v1, %lo(counter)($v0)
        jr    $ra
        addiu $sp, $sp, 8
msg:    .asciz "HELLO, PLAYSTATION"
        .align 2
table:  .word entry, print, helper
        .word 0xFFFFFFFF
        .space 32
        .word 0x12345678
.equ counter, 0x80100000
)";

// The executable: a 2048-byte header and the program at $80010000.
Bytes executable(std::string_view program = kProgram) {
    const std::string header = R"(
        .ascii "PS-X EXE"
        .space 8
        .word 0x80010000
        .word 0x80108000
        .word 0x80010000
        .word text_end - 0x80010000
        .word 0, 0, 0, 0
        .word 0x801FFF00, 0
        .space 20
        .asciz "Sony Computer Entertainment Inc. for North America area"
        .space 0x800 - 0x4C - 56
        .org 0x80010000
)";
    const std::string source = header + std::string(program) + "text_end:\n";
    // t_size is the program's own length, so the header is assembled with the program after it
    return build::mips_asm::Assembler(isa::mips_encoder()).build(source);
}

std::string golden(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

void check_golden(std::string_view name, const std::string& actual) {
    if (fs::get_env("CB_UPDATE_GOLDEN")) fs::write_file(testing::data_path("source/testdata/writers/" + std::string(name)), as_bytes(actual));
    CB_CHECK_EQ(actual, golden(name));
}

CB_TEST("source.ps1_writer.the_executable_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_ps1");
    const Bytes image = executable();
    const formats::psx::Exe exe = formats::psx::parse(image);
    CB_CHECK_EQ(exe.header.t_size, static_cast<std::uint32_t>(image.size() - 0x800));
    const image_writer::Written r = write(exe, "demo.exe", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK_EQ(r.functions.size(), std::size_t{3});
    CB_CHECK_EQ(r.functions.at(0x80010000), std::string("entry"));
    CB_CHECK_EQ(r.image_bytes, image.size());
    CB_CHECK(r.source == dir.path() / "demo.s");
    CB_CHECK(r.built && *r.built == dir.path() / "demo.built.exe" && fs::read_file(*r.built) == image);
    check_golden("ps1.golden.s", fs::read_text(r.source));
});

CB_TEST("source.ps1_writer.what_the_code_points_at_is_written_by_name", [] {
    fs::ScopedTempDir dir("cb_ps1");
    const image_writer::Written r = write(formats::psx::parse(executable()), "demo.exe", dir.path());
    const std::string source = fs::read_text(r.source);
    // functions found by a call, by the entry point, and by a prologue after a return (helper)
    CB_CHECK(r.functions.size() == 3);
    // the string and its address pair
    CB_CHECK(source.find(".asciz \"HELLO, PLAYSTATION\"") != std::string::npos);
    CB_CHECK(source.find("lui a0, %hi(D_") != std::string::npos);
    CB_CHECK(source.find("addiu a0, a0, %lo(D_") != std::string::npos);
    // the hardware register, by name
    CB_CHECK(source.find(".equ GPU_GP1, 0x1F801814") != std::string::npos);
    CB_CHECK(source.find("lw t1, %lo(GPU_GP1)(t0)") != std::string::npos);
    CB_CHECK(source.find(".equ GPU_GP0, 0x1F801810") != std::string::npos);
    // a variable past the program
    CB_CHECK(source.find(".equ var_80100000, 0x80100000") != std::string::npos);
    CB_CHECK(source.find("lw v1, %lo(var_80100000)(v0)") != std::string::npos);
    // the table of functions, and what is no instruction
    CB_CHECK(source.find(".word entry, sub_") != std::string::npos);
    CB_CHECK(source.find(", 0xFFFFFFFF") != std::string::npos);
    CB_CHECK(source.find(".space 32") != std::string::npos);
    // the header's fields
    CB_CHECK(source.find(".word entry    ; pc0: where it starts") != std::string::npos);
    CB_CHECK(source.find(".asciz \"Sony Computer Entertainment Inc. for North America area\"\n        .space 1916") != std::string::npos);
});

CB_TEST("source.ps1_writer.an_edited_source_is_not_the_original", [] {
    // the source built with one instruction changed is another program: the writer's own check says so
    fs::ScopedTempDir dir("cb_ps1");
    const Bytes image = executable();
    const image_writer::Written r = write(formats::psx::parse(image), "demo.exe", dir.path());
    std::string source = fs::read_text(r.source);
    source = text::replace_first(source, "addiu sp, sp, -0x18", "addiu sp, sp, -0x20");
    const Bytes other = risc_source::assemble("mips", source, dir.path());
    CB_CHECK_EQ(other.size(), image.size());
    CB_CHECK(other != image);
});


CB_TEST("source.ps1_writer.functions_nothing_calls_are_found_by_where_they_lie_and_by_pointers", [] {
    // `orphan` begins right after a function's end and reads as a function (no prologue); `handler` is only in a table; the words after them
    // are data and stay data
    fs::ScopedTempDir dir("cb_ps1");
    const Bytes image = executable(R"(
entry:  jal   a
        nop
        jr    $ra
        nop
a:      jr    $ra
        addiu $v0, $zero, 1
orphan: addiu $v0, $zero, 5
        beq   $v0, $zero, orphan
        nop
        jr    $ra
        nop
table:  .word handler, 0x12345678
handler: addiu $v0, $zero, 7
        jr    $ra
        nop
        .word 0x12345678, 0xFFFFFFFF
        .space 16
)");
    const image_writer::Written r = write(formats::psx::parse(image), "orphans.exe", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.functions.size(), std::size_t{4});
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find(".word sub_") != std::string::npos);   // the table names its function
    CB_CHECK(source.find(".word 0x12345678, 0xFFFFFFFF") != std::string::npos);
    CB_CHECK(source.find(".space 16") != std::string::npos);
});

CB_TEST("source.ps1_writer.a_header_with_other_bytes_is_written_as_bytes", [] {
    fs::ScopedTempDir dir("cb_ps1");
    Bytes image = executable();
    image[0x90] = 0x7B;   // something in the zeros after the marker
    image[0x3A] = 0x01;   // and in the registers the BIOS saves
    const image_writer::Written r = write(formats::psx::parse(image), "demo.exe", dir.path());
    CB_CHECK(r.linked && r.same);
});

CB_TEST("source.ps1_writer.a_program_with_no_code_is_all_data", [] {
    fs::ScopedTempDir dir("cb_ps1");
    const Bytes image = executable(".word 0xFFFFFFFF, 0x12345678\n.asciz \"abcd\"\n");
    const image_writer::Written r = write(formats::psx::parse(image), "plain.exe", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.functions.empty());
    CB_CHECK_EQ(r.code_bytes, std::size_t{0});
});

CB_TEST("source.ps1_writer.reconstruct_reads_a_file", [] {
    fs::ScopedTempDir dir("cb_ps1");
    const Bytes image = executable();
    fs::write_file(dir.path() / "SLUS_000.00", image);
    const image_writer::Written r = reconstruct(dir.path() / "SLUS_000.00", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.source == dir.path() / "out" / "SLUS_000.s");
    fs::write_file(dir.path() / "notps.exe", Bytes(0x900, 0x4D));
    CB_CHECK_THROWS_AS(cb::FormatError, reconstruct(dir.path() / "notps.exe", dir.path() / "out2"));
});

CB_TEST("source.ps1_writer.hardware_names_are_the_documentations", [] {
    CB_CHECK_EQ(hardware().at(0x1F801070), std::string("I_STAT"));
    CB_CHECK_EQ(hardware().at(0x1F8010A8), std::string("DMA2_CHCR"));
    CB_CHECK_EQ(hardware().at(0x1F801810), std::string("GPU_GP0"));
    CB_CHECK_EQ(hardware().at(0x1F801DAA), std::string("SPU_CTRL"));
});

}  // namespace
}  // namespace cb::source::ps1_writer
