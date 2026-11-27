// MIPS source of GNU as's language (the PlayStation's R3000A little-endian, the Nintendo 64's R4300 big-endian) built through the one assembler:
// gas-mips's dialect over the directive layer over the MIPS descriptions (what build/mips_asm_n64_test's cases said, byte for byte).
#include <format>

#include "core/error.hpp"
#include "core/testing.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace {

using cb::Bytes;

Bytes build_as(const char* cpu, std::string_view source) { return cb::source::risc_source::assemble(cpu, cb::source::dialect::find("gas-mips").parse(source), {}); }
Bytes big(std::string_view source) { return build_as("r4300", source); }
Bytes little(std::string_view source) { return build_as("mips", source); }

std::string hex(const Bytes& b) {
    std::string out;
    for (const std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

}  // namespace

CB_TEST("source.mips_source.data_directives_are_big_endian", [] {
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

CB_TEST("source.mips_source.instructions_are_the_r4300_encoders_big_endian_bytes", [] {
    CB_CHECK_EQ(hex(big("daddiu $sp, $sp, -16\njr $ra\nnop")), std::string("67bdfff0" "03e00008" "00000000"));
    CB_CHECK_EQ(hex(big("add.s $f0, $f2, $f4\nc.lt.d $f2, $f4\neret")), std::string("46041000" "4624103c" "42000018"));
    CB_CHECK_THROWS_AS(cb::Error, big("rtps"));         // not an R4300 instruction

    CB_CHECK_EQ(hex(little("addiu $sp, $sp, -32")), std::string("e0ffbd27"));
});

CB_TEST("source.mips_source.labels_branches_and_the_two_passes_work_as_before", [] {
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

CB_TEST("source.mips_source.the_size_of_an_instruction_with_an_unknown_name_is_the_same_on_both_passes", [] {
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

CB_TEST("source.mips_source.jal_to_a_label_ahead_is_the_same_word_as_to_one_known", [] {
    // (source.isa.the_r3000a_encoder: `jal foo` at $80010000 with foo at $80010100 is 0x0C004040, little-endian)
    const Bytes image = little(R"(.org 0x80010000
        jal foo
        .org 0x80010100
foo:    nop
)");
    CB_CHECK_EQ(hex(Bytes(image.begin(), image.begin() + 4)), std::string("4040000c"));
    CB_CHECK_THROWS_AS(cb::Error, (void)little("NOT AN INSTRUCTION"));
    CB_CHECK_THROWS_AS(cb::Error, (void)little("jal bar"));
});
