// cb-unity: alone (file-local names that other files in a batch also use)
// The Nintendo 64's R4300i interpreter. Programs are assembled by CodeBreaker's own big-endian assembler and run; the expected values are worked
// out by hand from the MIPS III and R4300i manuals (sign extension of 32-bit results, the 128-bit products, the unaligned loads and stores'
// byte merging, branch likely, the FPU's formats and rounding modes), not taken from this code's output.
#include "emulate/r4300.hpp"

#include <bit>
#include <format>
#include <functional>

#include "source/dialect.hpp"
#include "source/risc_source.hpp"
#include "core/testing.hpp"
#include "decompile/mips.hpp"
#include "source/isa_r4300.hpp"

namespace r4 = cb::emulate::r4300;
using cb::Bytes;
using std::string;

namespace {

constexpr std::uint32_t kEntry = 0x80001000;

Bytes assemble(const string& source) {
    return cb::source::risc_source::assemble("r4300", cb::source::dialect::find("gas-mips").parse(".org 0x80001000\n" + source), {});
}

// RDRAM holding the program at $1000.
Bytes ram_with(const string& source) {
    Bytes ram(r4::kRdramSize, 0);
    const Bytes code = assemble(source);
    std::copy(code.begin(), code.end(), ram.begin() + 0x1000);
    return ram;
}

r4::RunResult exec(const string& source, const r4::RunOptions& options = {}, const std::function<void(r4::Bus&)>& setup = {}, cb::ByteView rom = {}) {
    r4::Bus bus(ram_with(source), rom);
    if (setup) setup(bus);
    return r4::run(bus, kEntry, options);
}

std::uint64_t big_word(const r4::Bus& bus, std::size_t at, unsigned size) {
    std::uint64_t v = 0;
    for (unsigned i = 0; i < size; ++i) v = v << 8 | bus.rdram()[at + i];
    return v;
}

constexpr std::uint64_t kAllOnes = ~std::uint64_t{0};

}  // namespace

CB_TEST("emulate.r4300.thirty_two_bit_results_are_sign_extended_to_64", [] {
    const r4::RunResult r = exec(R"(
        lui   $t0, 0x8000          # 0xFFFFFFFF80000000
        daddiu $t1, $zero, -1      # all ones
        dsrl32 $t2, $t1, 0         # 0x00000000FFFFFFFF
        dsll32 $t3, $t2, 0         # 0xFFFFFFFF00000000
        addiu $t4, $zero, -2
        ori   $t5, $zero, 0xFFFF
        sll   $t5, $t5, 16         # 0xFFFF0000 as a word: sign-extended
        addu  $t6, $t0, $t0        # 0x80000000 + 0x80000000 = 0 (a word)
        daddu $t7, $t0, $t0        # 64-bit: 0xFFFFFFFF00000000
        dsra  $t8, $t0, 4          # arithmetic: 0xFFFFFFFFF8000000
        dsrl  $t9, $t0, 4          # logical:    0x0FFFFFFFF8000000
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[8], 0xFFFFFFFF80000000ull);
    CB_CHECK_EQ(r.r[9], kAllOnes);
    CB_CHECK_EQ(r.r[10], 0xFFFFFFFFull);
    CB_CHECK_EQ(r.r[11], 0xFFFFFFFF00000000ull);
    CB_CHECK_EQ(r.r[12], 0xFFFFFFFFFFFFFFFEull);
    CB_CHECK_EQ(r.r[13], 0xFFFFFFFFFFFF0000ull);
    CB_CHECK_EQ(r.r[14], 0ull);
    CB_CHECK_EQ(r.r[15], 0xFFFFFFFF00000000ull);
    CB_CHECK_EQ(r.r[24], 0xFFFFFFFFF8000000ull);
    CB_CHECK_EQ(r.r[25], 0x0FFFFFFFF8000000ull);
    CB_CHECK_EQ(r.r[0], 0ull);
    CB_CHECK_EQ(r.steps, std::uint64_t{13});
});

CB_TEST("emulate.r4300.multiply_and_divide_in_32_and_64_bits", [] {
    const r4::RunResult r = exec(R"(
        addiu $t0, $zero, -3
        addiu $t1, $zero, 7
        mult  $t0, $t1             # -21: LO = -21, HI = -1 (both sign-extended)
        mflo  $s0
        mfhi  $s1
        multu $t0, $t1             # 0xFFFFFFFD * 7 = 0x6FFFFFFEB: LO = sx(0xFFFFFFEB), HI = 6
        mflo  $s2
        mfhi  $s3
        div   $t0, $t1             # -3 / 7 = 0 remainder -3
        mflo  $s4
        mfhi  $s5
        daddiu $t2, $zero, -1
        daddiu $t3, $zero, 2
        dmult $t2, $t3             # -2: HI = -1
        mflo  $s6
        mfhi  $s7
        dmultu $t2, $t3            # 0xFFFFFFFFFFFFFFFF * 2 = 0x1FFFFFFFFFFFFFFFE: HI = 1
        mflo  $t4
        mfhi  $t5
        daddiu $t6, $zero, 100
        daddiu $t7, $zero, 7
        ddiv  $t6, $t7             # 14 remainder 2
        mflo  $t8
        mfhi  $t9
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[16], 0xFFFFFFFFFFFFFFEBull);
    CB_CHECK_EQ(r.r[17], kAllOnes);
    CB_CHECK_EQ(r.r[18], 0xFFFFFFFFFFFFFFEBull);
    CB_CHECK_EQ(r.r[19], 6ull);
    CB_CHECK_EQ(r.r[20], 0ull);
    CB_CHECK_EQ(r.r[21], 0xFFFFFFFFFFFFFFFDull);
    CB_CHECK_EQ(r.r[22], 0xFFFFFFFFFFFFFFFEull);
    CB_CHECK_EQ(r.r[23], kAllOnes);
    CB_CHECK_EQ(r.r[12], 0xFFFFFFFFFFFFFFFEull);
    CB_CHECK_EQ(r.r[13], 1ull);
    CB_CHECK_EQ(r.r[24], 14ull);
    CB_CHECK_EQ(r.r[25], 2ull);
});

CB_TEST("emulate.r4300.division_by_zero_and_the_extreme_quotient_do_what_the_hardware_does", [] {
    const r4::RunResult r = exec(R"(
        addiu $t0, $zero, 5
        div   $t0, $zero           # LO = -1 for a positive dividend, HI = the dividend
        mflo  $s0
        mfhi  $s1
        divu  $t0, $zero           # LO = all ones (a word, sign-extended), HI = the dividend
        mflo  $s2
        lui   $t1, 0x8000
        addiu $t2, $zero, -1
        div   $t1, $t2             # 0x80000000 / -1: LO = 0x80000000 (sx), HI = 0
        mflo  $s3
        mfhi  $s4
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.r[16], kAllOnes);
    CB_CHECK_EQ(r.r[17], 5ull);
    CB_CHECK_EQ(r.r[18], kAllOnes);
    CB_CHECK_EQ(r.r[19], 0xFFFFFFFF80000000ull);
    CB_CHECK_EQ(r.r[20], 0ull);
});

CB_TEST("emulate.r4300.branch_likely_skips_its_delay_slot_only_when_not_taken", [] {
    const r4::RunResult r = exec(R"(
        bnel  $zero, $zero, never  # not taken: the delay slot is skipped
        addiu $t0, $t0, 1
        beql  $zero, $zero, taken  # taken: the delay slot runs
        addiu $t1, $t1, 1
        addiu $t2, $t2, 100        # branched over
taken:  addiu $t3, $t3, 7
        addiu $v0, $zero, 1
        blezl $v0, never           # 1 <= 0 is false: skipped
        addiu $t4, $t4, 1
        bgtzl $v0, ok              # taken
        addiu $t5, $t5, 1
        addiu $t6, $t6, 100
ok:     bltzl $v0, never           # 1 < 0 is false: skipped
        addiu $t7, $t7, 1
never:  jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[8], 0ull);
    CB_CHECK_EQ(r.r[9], 1ull);
    CB_CHECK_EQ(r.r[10], 0ull);
    CB_CHECK_EQ(r.r[11], 7ull);
    CB_CHECK_EQ(r.r[12], 0ull);
    CB_CHECK_EQ(r.r[13], 1ull);
    CB_CHECK_EQ(r.r[14], 0ull);
    CB_CHECK_EQ(r.r[15], 0ull);
});

CB_TEST("emulate.r4300.branch_and_link_writes_the_return_address_and_calls_return", [] {
    const r4::RunResult r = exec(R"(
        addiu $s7, $ra, 0
        jal   f
        nop
        bgezal $zero, g            # always taken, links
        nop
        jr    $s7
        nop
f:      jr    $ra
        addiu $s0, $s0, 1          # the delay slot of the return runs
g:      jr    $ra
        addiu $s1, $s1, 2
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[16], 1ull);
    CB_CHECK_EQ(r.r[17], 2ull);
    CB_CHECK_EQ(r.r[31], 0xFFFFFFFF80001014ull);   // the link BGEZAL made: the instruction after its delay slot
    CB_CHECK_EQ(r.r[23], 0xFFFFFFFFBFC0FF00ull);   // $ra as run() gave it, sign-extended
});

CB_TEST("emulate.r4300.loads_and_stores_of_every_size_and_the_unaligned_ones", [] {
    // memory at $80100000: 01 23 45 67 89 AB CD EF
    const auto setup = [](r4::Bus& bus) {
        const std::uint8_t bytes[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
        std::copy(std::begin(bytes), std::end(bytes), bus.rdram().begin() + 0x100000);
    };
    const r4::RunResult r = exec(R"(
        lui   $a0, 0x8010
        ld    $s0, 0($a0)          # 0x0123456789ABCDEF
        lw    $s1, 4($a0)          # 0x89ABCDEF: sign-extended
        lwu   $s2, 4($a0)          # zero-extended
        lb    $s3, 5($a0)          # 0xAB: sign-extended
        lbu   $s4, 5($a0)
        lh    $s5, 6($a0)          # 0xCDEF: sign-extended
        lhu   $s6, 6($a0)
        daddiu $t0, $zero, 0
        ldl   $t0, 3($a0)          # (D << 24) | 0
        daddiu $t1, $zero, -1
        ldr   $t1, 3($a0)          # (D >> 32) in the low half, the high half kept
        daddiu $t2, $zero, 0
        lwl   $t2, 1($a0)          # word 0x01234567 << 8
        daddiu $t3, $zero, -1
        lwr   $t3, 1($a0)          # word >> 16 in the low half, the other bits kept
        jr    $ra
        nop
    )", {}, setup);
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[16], 0x0123456789ABCDEFull);
    CB_CHECK_EQ(r.r[17], 0xFFFFFFFF89ABCDEFull);
    CB_CHECK_EQ(r.r[18], 0x89ABCDEFull);
    CB_CHECK_EQ(r.r[19], 0xFFFFFFFFFFFFFFABull);
    CB_CHECK_EQ(r.r[20], 0xABull);
    CB_CHECK_EQ(r.r[21], 0xFFFFFFFFFFFFCDEFull);
    CB_CHECK_EQ(r.r[22], 0xCDEFull);
    CB_CHECK_EQ(r.r[8], 0x6789ABCDEF000000ull);
    CB_CHECK_EQ(r.r[9], 0xFFFFFFFF01234567ull);
    CB_CHECK_EQ(r.r[10], 0x23456700ull);
    CB_CHECK_EQ(r.r[11], 0xFFFFFFFFFFFF0123ull);
});

CB_TEST("emulate.r4300.unaligned_stores_merge_into_memory", [] {
    r4::Bus bus(ram_with(R"(
        lui   $a0, 0x8010
        lui   $t4, 0x1122
        ori   $t4, $t4, 0x3344
        dsll  $t4, $t4, 16
        ori   $t4, $t4, 0x5566
        dsll  $t4, $t4, 16
        ori   $t4, $t4, 0x7788     # 0x1122334455667788
        sdl   $t4, 3($a0)          # bytes 3-7 get the high five bytes: 11 22 33 44 55
        sdr   $t4, 11($a0)         # at +8: bytes 0-3 get the low four: 55 66 77 88
        swl   $t4, 17($a0)         # at +16: the word's bytes 1-3 get the high three of the low word: 55 66 77
        swr   $t4, 23($a0)         # at +20: all four low bytes: 55 66 77 88
        jr    $ra
        nop
    )"));
    const r4::RunResult r = r4::run(bus, kEntry);
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(big_word(bus, 0x100000, 8), 0x0000001122334455ull);
    CB_CHECK_EQ(big_word(bus, 0x100008, 8), 0x5566778800000000ull);
    CB_CHECK_EQ(big_word(bus, 0x100010, 4), 0x00556677ull);
    CB_CHECK_EQ(big_word(bus, 0x100014, 4), 0x55667788ull);
});

CB_TEST("emulate.r4300.a_misaligned_access_and_an_unmapped_one_end_the_run", [] {
    const r4::RunResult a = exec("lui $a0, 0x8010\n lw $v0, 2($a0)\n jr $ra\n nop\n");
    CB_CHECK_EQ(a.exit(), string("stopped: address error at 80100002"));
    const r4::RunResult b = exec("lui $a0, 0xA800\n lw $v0, 0($a0)\n jr $ra\n nop\n");   // physical $08000000 is SRAM: a block; $A8000000 -> $08000000
    CB_CHECK_EQ(b.exit(), string("return"));
    const r4::RunResult c = exec("lui $a0, 0xA7F0\n lw $v0, 0($a0)\n jr $ra\n nop\n");   // physical $07F00000: DOM2 ... mapped as registers
    CB_CHECK_EQ(c.exit(), string("return"));
    const r4::RunResult d = exec("lui $a0, 0x8100\n lw $v0, 0($a0)\n jr $ra\n nop\n");   // physical $01000000: nothing there
    CB_CHECK_EQ(d.exit(), string("stopped: bus error at 81000000"));
    const r4::RunResult e = exec("lui $a0, 0x0040\n lw $v0, 0($a0)\n jr $ra\n nop\n");   // $00400000 is KUSEG: the TLB is empty
    CB_CHECK_EQ(e.exit(), string("stopped: tlb miss at 00400000"));
});

CB_TEST("emulate.r4300.overflow_traps_and_syscalls_stop_the_run_with_their_reason", [] {
    const r4::RunResult add = exec("lui $t0, 0x7FFF\n ori $t0, $t0, 0xFFFF\n addu $v0, $t0, $t0\n add $v1, $t0, $t0\n jr $ra\n nop\n");
    CB_CHECK_EQ(add.exit(), string("stopped: arithmetic overflow"));
    CB_CHECK_EQ(add.r[2], 0xFFFFFFFFFFFFFFFEull);   // ADDU did not trap
    CB_CHECK_EQ(add.r[3], 0ull);                      // ADD did not write
    CB_CHECK_EQ(add.fault->pc, 0x8000100Cu);
    const r4::RunResult dadd = exec("daddiu $t1, $zero, -1\n dsrl $t0, $t1, 1\n daddu $v0, $t0, $t0\n dadd $v1, $t0, $t0\n jr $ra\n nop\n");
    CB_CHECK_EQ(dadd.exit(), string("stopped: arithmetic overflow"));
    CB_CHECK_EQ(dadd.r[2], 0xFFFFFFFFFFFFFFFEull);
    CB_CHECK_EQ(exec("syscall\n jr $ra\n nop\n").exit(), string("stopped: syscall"));
    CB_CHECK_EQ(exec("break 7\n jr $ra\n nop\n").exit(), string("stopped: break 7"));
    CB_CHECK_EQ(exec("addiu $t0, $zero, 3\n teq $t0, $t0\n jr $ra\n nop\n").exit(), string("stopped: trap"));
    CB_CHECK_EQ(exec("addiu $t0, $zero, 3\n tne $t0, $t0\n jr $ra\n nop\n").exit(), string("return"));
    CB_CHECK_EQ(exec("addiu $t0, $zero, 3\n tgei $t0, 2\n jr $ra\n nop\n").exit(), string("stopped: trap"));
    CB_CHECK_EQ(exec("addiu $t0, $zero, 3\n tlti $t0, 2\n jr $ra\n nop\n").exit(), string("return"));
});

CB_TEST("emulate.r4300.kseg0_and_kseg1_are_one_memory_and_registers_are_logged", [] {
    const r4::RunResult r = exec(R"(
        lui   $t0, 0xA010
        addiu $t1, $zero, 0x1234
        sw    $t1, 0($t0)          # KSEG1 $A0100000 -> physical $00100000
        lui   $t2, 0x8010
        lw    $v0, 0($t2)          # the same word through KSEG0
        lui   $t3, 0xA430
        addiu $t4, $zero, 0x555
        sw    $t4, 0x0C($t3)       # MI_INTR_MASK
        lw    $v1, 0x0C($t3)       # reads back what was written
        lui   $t5, 0xA404
        lw    $s0, 0x10($t5)       # SP_STATUS: halted
        lui   $t6, 0xA440
        lw    $s1, 0x10($t6)       # VI_CURRENT: a scanline counter
        lw    $s2, 0x10($t6)
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[2], 0x1234ull);
    CB_CHECK_EQ(r.r[3], 0x555ull);
    CB_CHECK_EQ(r.r[16], 1ull);
    CB_CHECK_EQ(r.r[17], 0ull);
    CB_CHECK_EQ(r.r[18], 1ull);
    // RDRAM is not a device: only the registers are logged, in order, with physical addresses
    CB_CHECK_EQ(r.io.size(), std::size_t{5});
    CB_CHECK_EQ(r.io[0].text(), string("w4"));
    CB_CHECK_EQ(r.io[0].address, 0x0430000Cu);
    CB_CHECK_EQ(r.io[0].value, 0x555ull);
    CB_CHECK_EQ(r.io[1].text(), string("r4"));
    CB_CHECK_EQ(r.io[2].address, 0x04040010u);
    CB_CHECK_EQ(r.io[3].address, 0x04400010u);
    // the run record: big-endian, registers as results, the I/O in order
    const cb::emulate::Run record = r4::record(r);
    CB_CHECK(record.endian == cb::emulate::Endian::Big);
    CB_CHECK_EQ(record.io.size(), std::size_t{5});
    CB_CHECK(record.exit == string("return"));
});

CB_TEST("emulate.r4300.the_pi_dma_copies_the_cartridge_into_rdram", [] {
    Bytes rom(0x2000);
    for (std::size_t i = 0; i < rom.size(); ++i) rom[i] = static_cast<std::uint8_t>(i * 7 + 3);
    r4::Bus bus(ram_with(R"(
        lui   $t0, 0xA460
        lui   $t1, 0x0010
        sw    $t1, 0($t0)          # PI_DRAM_ADDR = $100000
        lui   $t2, 0x1000
        ori   $t2, $t2, 0x1000
        sw    $t2, 4($t0)          # PI_CART_ADDR = $10001000
        addiu $t3, $zero, 15
        sw    $t3, 0x0C($t0)       # PI_WR_LEN: 16 bytes, cartridge to RDRAM
        lui   $a0, 0x9000          # the cartridge through the CPU: $10000000 is KSEG0 $90000000
        lw    $v0, 0x1000($a0)
        jr    $ra
        nop
    )"), rom);
    const r4::RunResult r = r4::run(bus, kEntry);
    CB_CHECK_EQ(r.exit(), string("return"));
    for (std::size_t i = 0; i < 16; ++i) CB_CHECK_EQ(static_cast<int>(bus.rdram()[0x100000 + i]), static_cast<int>(rom[0x1000 + i]));
    CB_CHECK_EQ(static_cast<int>(bus.rdram()[0x100010]), 0);
    CB_CHECK_EQ(r.r[2], (static_cast<std::uint64_t>(rom[0x1000]) << 24 | static_cast<std::uint64_t>(rom[0x1001]) << 16 | static_cast<std::uint64_t>(rom[0x1002]) << 8 | rom[0x1003]) |
                         ((rom[0x1000] & 0x80) != 0 ? 0xFFFFFFFF00000000ull : 0));
});

CB_TEST("emulate.r4300.fpu_single_double_compare_and_branch", [] {
    const r4::RunResult r = exec(R"(
        lui   $t0, 0x3FC0          # 1.5f
        mtc1  $t0, $f0
        lui   $t0, 0x4010          # 2.25f
        mtc1  $t0, $f2
        add.s $f4, $f0, $f2        # 3.75f = 0x40700000
        mfc1  $v0, $f4
        cvt.d.s $f6, $f4           # 3.75 = 0x400E000000000000
        dmfc1 $v1, $f6
        trunc.w.d $f8, $f6         # 3
        mfc1  $s0, $f8
        mul.s $f10, $f0, $f2       # 1.5 * 2.25 = 3.375 = 0x40580000
        mfc1  $s5, $f10
        sub.s $f12, $f0, $f2       # -0.75 = 0xBF400000
        mfc1  $s6, $f12
        div.s $f14, $f2, $f0       # 2.25 / 1.5 = 1.5
        mfc1  $s7, $f14
        c.lt.s $f0, $f2            # 1.5 < 2.25
        bc1t  yes
        nop
        addiu $s1, $zero, 1        # skipped
yes:    addiu $s2, $zero, 2
        c.eq.s $f0, $f2
        bc1f  no
        nop
        addiu $s3, $zero, 1        # skipped
no:     addiu $s4, $zero, 4
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[2], 0x40700000ull);
    CB_CHECK_EQ(r.r[3], 0x400E000000000000ull);
    CB_CHECK_EQ(r.r[16], 3ull);
    CB_CHECK_EQ(r.r[21], 0x40580000ull);
    CB_CHECK_EQ(r.r[22], 0xFFFFFFFFBF400000ull);
    CB_CHECK_EQ(r.r[23], 0x3FC00000ull);
    CB_CHECK_EQ(r.r[17], 0ull);
    CB_CHECK_EQ(r.r[18], 2ull);
    CB_CHECK_EQ(r.r[19], 0ull);
    CB_CHECK_EQ(r.r[20], 4ull);
    CB_CHECK_EQ(r.fpr[6], 0x400E000000000000ull);
});

CB_TEST("emulate.r4300.fpu_integer_conversions_follow_the_rounding_mode", [] {
    const r4::RunResult r = exec(R"(
        lui   $t0, 0x4020          # 2.5f
        mtc1  $t0, $f0
        cvt.w.s $f2, $f0           # round to nearest even: 2
        mfc1  $s0, $f2
        addiu $t1, $zero, 2
        ctc1  $t1, $31             # round toward +infinity
        cvt.w.s $f2, $f0           # 3
        mfc1  $s1, $f2
        addiu $t1, $zero, 3
        ctc1  $t1, $31             # round toward -infinity
        cvt.w.s $f2, $f0           # 2
        mfc1  $s2, $f2
        addiu $t1, $zero, 1
        ctc1  $t1, $31             # round toward zero
        lui   $t0, 0xC020          # -2.5f
        mtc1  $t0, $f4
        cvt.w.s $f6, $f4           # -2
        mfc1  $s3, $f6
        round.w.s $f6, $f4         # (the rounding instructions say their own mode) -2: nearest even
        mfc1  $s4, $f6
        floor.w.s $f6, $f4         # -3
        mfc1  $s5, $f6
        ceil.w.s $f6, $f4          # -2
        mfc1  $s6, $f6
        cfc1  $s7, $31             # the control register: the mode we set
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[16], 2ull);
    CB_CHECK_EQ(r.r[17], 3ull);
    CB_CHECK_EQ(r.r[18], 2ull);
    CB_CHECK_EQ(r.r[19], 0xFFFFFFFFFFFFFFFEull);
    CB_CHECK_EQ(r.r[20], 0xFFFFFFFFFFFFFFFEull);
    CB_CHECK_EQ(r.r[21], 0xFFFFFFFFFFFFFFFDull);
    CB_CHECK_EQ(r.r[22], 0xFFFFFFFFFFFFFFFEull);
    CB_CHECK_EQ(r.r[23], 1ull);
});

CB_TEST("emulate.r4300.fpu_double_arithmetic_and_long_conversions", [] {
    const r4::RunResult r = exec(R"(
        lui   $t0, 0x4019          # 6.25 = 0x4019000000000000
        dsll32 $t0, $t0, 0
        dmtc1 $t0, $f0
        sqrt.d $f2, $f0            # 2.5 = 0x4004000000000000
        dmfc1 $s0, $f2
        neg.d $f4, $f2             # -2.5
        dmfc1 $s1, $f4
        abs.d $f6, $f4
        dmfc1 $s2, $f6
        trunc.l.d $f8, $f0         # 6
        dmfc1 $s3, $f8
        cvt.d.l $f10, $f8          # back to 6.0 = 0x4018000000000000
        dmfc1 $s4, $f10
        cvt.s.d $f12, $f0          # 6.25f = 0x40C80000
        mfc1  $s5, $f12
        c.ole.d $f2, $f0           # 2.5 <= 6.25
        bc1fl bad                  # not taken (the condition is true): the delay slot is skipped
        addiu $s6, $s6, 1
        addiu $s7, $zero, 9
bad:    jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[16], 0x4004000000000000ull);
    CB_CHECK_EQ(r.r[17], 0xC004000000000000ull);
    CB_CHECK_EQ(r.r[18], 0x4004000000000000ull);
    CB_CHECK_EQ(r.r[19], 6ull);
    CB_CHECK_EQ(r.r[20], 0x4018000000000000ull);
    CB_CHECK_EQ(r.r[21], 0x40C80000ull);
    CB_CHECK_EQ(r.r[22], 0ull);
    CB_CHECK_EQ(r.r[23], 9ull);
});

CB_TEST("emulate.r4300.fpu_invalid_operations_give_the_mips_quiet_nan", [] {
    // MIPS's NaNs: quiet when the top bit of the fraction is CLEAR. 0 / 0 is invalid: the default quiet NaN. A quiet NaN operand is passed on
    // unchanged; a signaling one (top bit set: the host's own quiet NaN, 0x7FC00000) is an invalid operation.
    const r4::RunResult r = exec(R"(
        mtc1  $zero, $f0
        mtc1  $zero, $f2
        div.s $f4, $f0, $f2        # 0 / 0: the default quiet NaN, 0x7FBFFFFF
        mfc1  $s0, $f4
        lui   $t0, 0x7F8A
        ori   $t0, $t0, 0xBCDE     # a quiet NaN with a payload
        mtc1  $t0, $f6
        add.s $f8, $f6, $f0
        mfc1  $s1, $f8
        lui   $t1, 0x7FC0          # a signaling NaN
        mtc1  $t1, $f10
        mul.s $f12, $f0, $f10
        mfc1  $s2, $f12
        sqrt.s $f14, $f10
        mfc1  $s3, $f14
        lui   $t2, 0xBF80          # sqrt(-1) is invalid too
        mtc1  $t2, $f16
        sqrt.s $f18, $f16
        mfc1  $s4, $f18
        cvt.w.s $f20, $f6          # converting a NaN is "unimplemented": the run ends
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.r[16], 0x7FBFFFFFull);
    CB_CHECK_EQ(r.r[17], 0x7F8ABCDEull);
    CB_CHECK_EQ(r.r[18], 0x7FBFFFFFull);
    CB_CHECK_EQ(r.r[19], 0x7FBFFFFFull);
    CB_CHECK_EQ(r.r[20], 0x7FBFFFFFull);
    CB_CHECK_EQ(r.exit(), string("stopped: not run: cvt.w.s"));
});

CB_TEST("emulate.r4300.the_double_register_pairs_depend_on_the_status_fr_bit", [] {
    const auto run_with = [](std::uint64_t status) {
        Bytes ram = ram_with("mtc1 $t0, $f0\n mtc1 $t1, $f1\n dmfc1 $t2, $f0\n dmfc1 $t3, $f1\n");
        r4::Bus bus(ram);
        r4::Cpu cpu(bus);
        cpu.cop0[12] = status;
        cpu.r[8] = 0x11111111;
        cpu.r[9] = 0x22222222;
        cpu.jump_to(kEntry);
        for (int i = 0; i < 4; ++i) CB_CHECK(!cpu.step());
        return cpu;
    };
    // FR = 0: $f1 is the high half of the pair $f0/$f1
    const r4::Cpu pair = run_with(0x30000000);
    CB_CHECK_EQ(pair.r[10], 0x2222222211111111ull);
    CB_CHECK_EQ(pair.r[11], 0x2222222211111111ull);   // an odd register's 64-bit read is the pair
    // FR = 1: 32 registers of their own
    const r4::Cpu wide = run_with(0x34000000);
    CB_CHECK_EQ(wide.fpr[0] & 0xFFFFFFFF, 0x11111111ull);
    CB_CHECK_EQ(wide.fpr[1] & 0xFFFFFFFF, 0x22222222ull);
    CB_CHECK_EQ(wide.r[10], 0x11111111ull);
    CB_CHECK_EQ(wide.r[11], 0x22222222ull);
});

CB_TEST("emulate.r4300.the_tlb_maps_page_pairs_and_misses_fault", [] {
    const auto program = R"(
        mtc0  $zero, $0            # Index 0
        lui   $t0, 0x0040
        mtc0  $t0, $10             # EntryHi: VPN2 of $00400000
        addiu $t0, $zero, 0x4007
        mtc0  $t0, $2              # EntryLo0: PFN $100, global, valid, dirty
        addiu $t0, $zero, 0x4047
        mtc0  $t0, $3              # EntryLo1: PFN $101
        mtc0  $zero, $5            # PageMask: 4 KB pages
        tlbwi
        lui   $t1, 0x0040
        lw    $v0, 0($t1)          # the even page -> physical $00100000
        lw    $v1, 0x1000($t1)     # the odd page  -> physical $00101000
        sw    $v0, 0x10($t1)       # a store through a dirty page
        lw    $a0, 0x2000($t1)     # past the pair: a miss
        jr    $ra
        nop
    )";
    r4::Bus bus(ram_with(program));
    bus.rdram()[0x100000] = 0xA1;
    bus.rdram()[0x100003] = 0xA2;
    bus.rdram()[0x101000] = 0xB1;
    bus.rdram()[0x101003] = 0xB2;
    const r4::RunResult r = r4::run(bus, kEntry);
    CB_CHECK_EQ(r.exit(), string("stopped: tlb miss at 00402000"));
    CB_CHECK_EQ(r.r[2], 0xFFFFFFFFA10000A2ull);
    CB_CHECK_EQ(r.r[3], 0xFFFFFFFFB10000B2ull);
    CB_CHECK_EQ(big_word(bus, 0x100010, 4), 0xA10000A2ull);
    // TLBP finds the entry, TLBR reads it back
    const r4::RunResult probe = exec(R"(
        addiu $t0, $zero, 5
        mtc0  $t0, $0              # Index 5
        lui   $t0, 0x0040
        mtc0  $t0, $10
        addiu $t0, $zero, 0x4007
        mtc0  $t0, $2
        mtc0  $t0, $3
        tlbwi
        mtc0  $zero, $0
        tlbp                       # the entry for $00400000 is Index 5
        mfc0  $v0, $0
        mtc0  $zero, $2
        addiu $t0, $zero, 5
        mtc0  $t0, $0
        tlbr
        mfc0  $v1, $2              # EntryLo0 as written (the global bit is the AND of both)
        lui   $t0, 0x0123
        mtc0  $t0, $10
        tlbp                       # no entry: bit 31 of Index
        mfc0  $a0, $0
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(probe.exit(), string("return"));
    CB_CHECK_EQ(probe.r[2], 5ull);
    CB_CHECK_EQ(probe.r[3], 0x4007ull);
    CB_CHECK_EQ(probe.r[4] & 0x80000000ull, 0x80000000ull);
});

CB_TEST("emulate.r4300.cop0_registers_and_eret", [] {
    const r4::RunResult r = exec(R"(
        mfc0  $s0, $12             # Status as the boot code leaves it
        mfc0  $s1, $15             # PRId
        mtc0  $zero, $15           # read only: ignored
        mfc0  $s2, $15
        addiu $t0, $zero, 0x300
        mtc0  $t0, $13             # Cause: only the two software interrupt bits are writable
        addiu $t0, $zero, -1
        mtc0  $t0, $13
        mfc0  $s3, $13
        la    $t1, back
        mtc0  $t1, $14             # EPC
        addiu $t2, $zero, 2
        mtc0  $t2, $12             # Status.EXL set (and the rest cleared)
        eret                       # to EPC, clearing EXL
        addiu $s4, $zero, 99       # not run
back:   mfc0  $s5, $12
        mfc0  $s6, $9              # Count: it counts instructions
        mfc0  $s7, $9
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.exit(), string("return"));
    CB_CHECK_EQ(r.r[16], 0x34000000ull);
    CB_CHECK_EQ(r.r[17], 0x0B22ull);
    CB_CHECK_EQ(r.r[18], 0x0B22ull);
    CB_CHECK_EQ(r.r[19], 0x300ull);
    CB_CHECK_EQ(r.r[20], 0ull);
    CB_CHECK_EQ(r.r[21], 0ull);   // Status after ERET: EXL cleared
    CB_CHECK_EQ(r.r[23] - r.r[22], 1ull);
});

CB_TEST("emulate.r4300.ll_and_sc_and_the_step_limit", [] {
    const r4::RunResult r = exec(R"(
        lui   $a0, 0x8010
        ll    $t0, 0($a0)
        addiu $t0, $t0, 5
        sc    $t0, 0($a0)          # the link is intact: stores, $t0 = 1
        move  $s0, $t0
        sc    $t0, 0($a0)          # no link now: $t0 = 0, nothing stored
        move  $s1, $t0
        lw    $s2, 0($a0)
        jr    $ra
        nop
    )");
    CB_CHECK_EQ(r.r[16], 1ull);
    CB_CHECK_EQ(r.r[17], 0ull);
    CB_CHECK_EQ(r.r[18], 5ull);
    r4::RunOptions few;
    few.max_steps = 50;
    const r4::RunResult spin = exec("loop: b loop\n nop\n", few);
    CB_CHECK_EQ(spin.exit(), string("limit"));
    CB_CHECK(r4::record(spin).truncated);
});

CB_TEST("emulate.r4300.arguments_and_saved_registers_are_sign_extended_into_64_bits", [] {
    r4::RunOptions options;
    options.a = {0xFFFFFFFF, 2, 0x80000000, 4};
    options.s[0] = 0x80000001;
    const r4::RunResult r = exec("daddu $v0, $a0, $a1\n addu $v1, $a2, $zero\n move $s1, $s0\n jr $ra\n nop\n", options);
    CB_CHECK_EQ(r.r[2], 1ull);                       // -1 + 2
    CB_CHECK_EQ(r.r[3], 0xFFFFFFFF80000000ull);
    CB_CHECK_EQ(r.r[17], 0xFFFFFFFF80000001ull);
    CB_CHECK_EQ(r.r[29], 0xFFFFFFFF803FFF00ull);
});

CB_TEST("emulate.r4300.every_word_the_decoder_reads_is_one_the_interpreter_runs", [] {
    // the interpreter decodes with the table of decompile/mips (so a word that is not an instruction there is no instruction here), and what the
    // table reads must never be "no instruction" to the interpreter: a hole in one of the two shows up as a fault of that kind
    std::uint64_t state = 0x2545F4914F6CDD1Dull;
    const auto next = [&] {
        state = state * 6364136223846793005ull + 1442695040888963407ull;
        return static_cast<std::uint32_t>(state >> 32);
    };
    std::size_t read = 0, skipped = 0;
    string first_bad;
    Bytes ram(r4::kRdramSize, 0);
    r4::Bus bus(ram);
    for (std::uint32_t op = 0; op < 64; ++op) {
        for (int i = 0; i < 1500; ++i) {
            std::uint32_t word = op << 26 | (next() & 0x03FFFFFF);
            if (op == 0x11 && i % 2 == 0) word = 0x44000000u | (next() & 0x001FFFFF) | ((i % 8 < 4 ? 16u : 17u) << 21);
            const auto insn = cb::decompile::mips::decode_word(word, kEntry, cb::decompile::mips::Variant::R4300);
            for (int k = 0; k < 4; ++k) bus.rdram()[0x1000 + k] = static_cast<std::uint8_t>(word >> (24 - 8 * k));
            r4::Cpu cpu(bus);
            cpu.cop0[12] = 0x34000000;
            cpu.jump_to(kEntry);
            const std::optional<r4::Fault> fault = cpu.step();
            const bool none = fault && fault->kind == r4::Fault::Kind::NoInstruction;
            if (insn) ++read;
            else ++skipped;
            if (insn && none && first_bad.empty()) first_bad = std::format("{:08X} {}", word, cb::decompile::mips::text(*insn));
            if (!insn && !none && first_bad.empty()) first_bad = std::format("{:08X} undecoded, ran", word);
        }
    }
    CB_CHECK(read > 20000);
    CB_CHECK(skipped > 10000);
    CB_CHECK_EQ(first_bad, string());
});
