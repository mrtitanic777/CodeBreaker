// cb-unity: alone (file-local names that other files in a batch also use)
// The assembler's big-endian mode (the Nintendo 64's): data directives lay a value out most significant byte first, instructions come from the
// R4300 encoder big-endian, and the default (the PlayStation's little-endian) is as it was.
#include "cb/build/mips_asm.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/source/isa.hpp"
#include "cb/source/isa_r4300.hpp"

namespace {

using cb::Bytes;
namespace mips_asm = cb::build::mips_asm;

Bytes big(std::string_view source) { return mips_asm::Assembler(cb::source::isa::r4300_encoder(), mips_asm::ByteOrder::Big).build(source); }
Bytes little(std::string_view source) { return mips_asm::Assembler(cb::source::isa::mips_encoder()).build(source); }

std::string hex(const Bytes& b) {
    std::string out;
    for (const std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

}  // namespace

CB_TEST("build.mips_asm_n64.data_directives_are_big_endian", [] {
    CB_CHECK_EQ(hex(big(".word 0x80371240")), std::string("80371240"));
    CB_CHECK_EQ(hex(big(".half 0x1234, 0xABCD")), std::string("1234abcd"));
    CB_CHECK_EQ(hex(big(".byte 1, 2, 3")), std::string("010203"));
    CB_CHECK_EQ(hex(big(".dword 0x0123456789ABCDEF")), std::string("0123456789abcdef"));
    CB_CHECK_EQ(hex(big(".quad -2")), std::string("fffffffffffffffe"));
    CB_CHECK_EQ(hex(big(".fill 2, 4, 0x11223344")), std::string("1122334411223344"));
    CB_CHECK_EQ(hex(big(".fill 1, 2, 0x1234")), std::string("1234"));
    CB_CHECK_EQ(hex(big(".ascii \"AB\"\n.align 2\n.word 1")), std::string("41420000" "00000001"));
    // the same lines, little-endian (the default), as they were
    CB_CHECK_EQ(hex(little(".word 0x80371240")), std::string("40123780"));
    CB_CHECK_EQ(hex(little(".half 0x1234")), std::string("3412"));
    CB_CHECK_EQ(hex(little(".dword 0x0123456789ABCDEF")), std::string("efcdab8967452301"));
});

CB_TEST("build.mips_asm_n64.instructions_are_the_r4300_encoders_big_endian_bytes", [] {
    CB_CHECK_EQ(hex(big("daddiu $sp, $sp, -16\njr $ra\nnop")), std::string("67bdfff0" "03e00008" "00000000"));
    CB_CHECK_EQ(hex(big("add.s $f0, $f2, $f4\nc.lt.d $f2, $f4\neret")), std::string("46041000" "4624103c" "42000018"));
    CB_CHECK_THROWS_AS(cb::Error, big("rtps"));         // not an R4300 instruction
    CB_CHECK_THROWS_AS(cb::Error, little("daddiu $sp, $sp, -16"));   // not an R3000A one
    CB_CHECK_EQ(hex(little("addiu $sp, $sp, -32")), std::string("e0ffbd27"));
});

CB_TEST("build.mips_asm_n64.labels_branches_and_the_two_passes_work_as_before", [] {
    // a forward branch likely, a loop, a jump to a label, `.org` for the address and a word holding a label
    const Bytes image = big(R"(
        .org 0x80001000
start:  beql  $a0, $zero, later     # 0x80001000 -> 0x80001010: offset 3
        nop
        b     start
        nop
later:  jal   start
        nop
        .word later, start
)");
    CB_CHECK_EQ(hex(image), std::string("50800003" "00000000" "1000fffd" "00000000" "0c000400" "00000000" "80001010" "80001000"));
});

CB_TEST("build.mips_asm_n64.the_size_of_an_instruction_with_an_unknown_name_is_the_same_on_both_passes", [] {
    // `la` is two words and a forward label is not known on the first pass
    const Bytes image = big(R"(
        .org 0x80001000
        la    $a0, msg
        jr    $ra
        nop
msg:    .asciz "hi"
)");
    CB_CHECK_EQ(image.size(), std::size_t{8 + 8 + 3});
    CB_CHECK_EQ(hex(Bytes(image.begin(), image.begin() + 8)), std::string("3c048000" "24841010"));
});
