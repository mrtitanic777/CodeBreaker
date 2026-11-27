// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/sh4_test.cpp -- the SH-4 variant of the interpreter: little-endian memory, the FPU (single and double operations, pair moves, FIPR,
// FTRV, FMAC, FSCA, FLOAT/FTRC), register banks, SR's RB, RTE through SPC and SSR, shifts, the store queue, the on-chip registers and the cache
// operations as logged state. Programs are assembled from GNU as source with CodeBreaker's own SH-4 assembler.
#include <bit>
#include <cmath>
#include <string>

#include "core/testing.hpp"
#include "emulate/sh2.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace sh = cb::emulate::sh2;
using cb::Bytes;

namespace {

constexpr std::uint32_t kCode = 0x0C010000;

sh::Bus make_bus() {
    sh::Bus bus(sh::Variant::Sh4);
    bus.add_ram(0x0C000000, 0x100000);
    return bus;
}

sh::RunResult go(sh::Bus& bus, const std::string& source, sh::RunOptions options = {}) {
    const Bytes code = cb::source::risc_source::assemble("sh4", cb::source::dialect::find("gas-sh4").parse(".org 0x0C010000\n" + source), {});
    bus.load(kCode, code);
    options.sp = 0x8C0F0000;
    return sh::run(bus, kCode, options);
}

float as_float(std::uint32_t b) { return std::bit_cast<float>(b); }

}  // namespace

CB_TEST("emulate.sh4.memory_is_little_endian_and_words_are_fetched_that_way", [] {
    sh::Bus bus = make_bus();
    const sh::RunResult r = go(bus, "mov.l r4,@r5\n rts\n nop\n", {.a = {0x11223344, 0x0C000100}});
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(bus.peek(0x0C000100, 1), 0x44u);
    CB_CHECK_EQ(bus.peek(0x0C000103, 1), 0x11u);
    CB_CHECK_EQ(bus.peek(0x0C000100, 4), 0x11223344u);
    CB_CHECK_EQ(bus.peek(0x0C000100, 2), 0x3344u);
});

CB_TEST("emulate.sh4.single_precision_arithmetic", [] {
    sh::Bus bus = make_bus();
    const sh::RunResult r = go(bus,
                               "fldi1 fr0\n fldi1 fr1\n fadd fr1,fr0\n fmul fr0,fr0\n fsqrt fr0\n"   // 1+1=2, 4, 2
                               "fmov fr0,fr2\n fneg fr2\n fabs fr2\n fcmp/gt fr1,fr2\n movt r1\n"   // 2 > 1: T=1
                               "ftrc fr0,fpul\n sts fpul,r0\n rts\n nop\n");
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(as_float(r.fr[0]), 2.0f);
    CB_CHECK_EQ(r.r[0], 2u);
    CB_CHECK_EQ(r.r[1], 1u);
    CB_CHECK_EQ(as_float(r.fr[2]), 2.0f);
});

CB_TEST("emulate.sh4.double_precision_follows_fpscr_pr", [] {
    sh::Bus bus = make_bus();
    // PR=1: fadd dr2,dr2 adds doubles; float fpul,dr2 converts the integer 21
    sh::RunOptions options;
    options.fpscr = sh::fpscr::PR;
    options.a = {21};
    const sh::RunResult r = go(bus, "lds r4,fpul\n float fpul,dr2\n fadd dr2,dr2\n ftrc dr2,fpul\n sts fpul,r0\n rts\n nop\n", options);
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(r.r[0], 42u);
    const std::uint64_t bits = static_cast<std::uint64_t>(r.fr[2]) << 32 | r.fr[3];
    CB_CHECK_EQ(std::bit_cast<double>(bits), 42.0);
    CB_CHECK_EQ(r.fpscr & sh::fpscr::PR, sh::fpscr::PR);
});

CB_TEST("emulate.sh4.fcnv_converts_between_the_precisions", [] {
    sh::Bus bus = make_bus();
    sh::RunOptions options;
    options.fpscr = sh::fpscr::PR;
    options.a = {std::bit_cast<std::uint32_t>(1.5f)};
    const sh::RunResult r = go(bus, "lds r4,fpul\n fcnvsd fpul,dr4\n fcnvds dr4,fpul\n sts fpul,r0\n rts\n nop\n", options);
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(std::bit_cast<double>(static_cast<std::uint64_t>(r.fr[4]) << 32 | r.fr[5]), 1.5);
    CB_CHECK_EQ(r.r[0], std::bit_cast<std::uint32_t>(1.5f));
});

CB_TEST("emulate.sh4.pair_moves_with_sz_put_the_even_register_high", [] {
    sh::Bus bus = make_bus();
    // fschg: SZ=1; fmov.d dr0,@r4 stores a 64-bit value: little-endian, so FR1 (the low word) lands at the lower address
    const sh::RunResult r = go(bus, "fldi1 fr0\n fldi0 fr1\n fschg\n fmov.d dr0,@r4\n fmov.d @r4,dr2\n rts\n nop\n", {.a = {0x0C000200}});
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(bus.peek(0x0C000200, 4), 0u);
    CB_CHECK_EQ(bus.peek(0x0C000204, 4), 0x3F800000u);
    CB_CHECK_EQ(r.fr[2], 0x3F800000u);   // the pair read back: FR2 high
    CB_CHECK_EQ(r.fr[3], 0u);
    CB_CHECK((r.fpscr & sh::fpscr::SZ) != 0);
    // with SZ=0 the same word moves one float
    sh::Bus again = make_bus();
    const sh::RunResult s = go(again, "fldi1 fr0\n fmov.s fr0,@-r4\n rts\n nop\n", {.a = {0x0C000300}});
    CB_CHECK_EQ(s.r[4], 0x0C0002FCu);
    CB_CHECK_EQ(again.peek(0x0C0002FC, 4), 0x3F800000u);
});

CB_TEST("emulate.sh4.fipr_and_ftrv", [] {
    sh::Bus bus = make_bus();
    // fv0 = (1,1,0,0), fv4 = (1,1,0,0): the dot product 2 lands in fr7
    sh::RunResult r = go(bus, "fldi1 fr0\n fldi1 fr1\n fldi1 fr4\n fldi1 fr5\n fipr fv0,fv4\n rts\n nop\n");
    CB_CHECK_EQ(as_float(r.fr[7]), 2.0f);
    // XMTRX = 2 on its diagonal (the other bank), fv0 = (1,1,1,1): ftrv scales fr0 by two only for the matrix element in row 0 column 0
    sh::Bus other = make_bus();
    r = go(other,
           "frchg\n fldi1 fr0\n fadd fr0,fr0\n fldi1 fr5\n fldi1 fr10\n fldi1 fr15\n frchg\n"   // xf0 = 2, xf5 = xf10 = xf15 = 1
           "fldi1 fr0\n fldi1 fr1\n fldi1 fr2\n fldi1 fr3\n ftrv xmtrx,fv0\n rts\n nop\n");
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(as_float(r.fr[0]), 2.0f);
    CB_CHECK_EQ(as_float(r.fr[1]), 1.0f);
    CB_CHECK_EQ(as_float(r.fr[2]), 1.0f);
    CB_CHECK_EQ(as_float(r.fr[3]), 1.0f);
    CB_CHECK_EQ(r.fpscr & sh::fpscr::FR, 0u);
});

CB_TEST("emulate.sh4.fmac_fsca_float_and_ftrc", [] {
    sh::Bus bus = make_bus();
    sh::RunOptions options;
    options.a = {0x4000};   // a quarter turn
    const sh::RunResult r = go(bus,
                               "fldi1 fr0\n fldi1 fr1\n fadd fr1,fr1\n fldi1 fr2\n fmac fr0,fr1,fr2\n"   // fr2 = 1*2 + 1 = 3
                               "lds r4,fpul\n fsca fpul,dr4\n rts\n nop\n",
                               options);
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(as_float(r.fr[2]), 3.0f);
    CB_CHECK(std::fabs(as_float(r.fr[4]) - 1.0f) < 1e-5f);   // sin of a quarter turn
    CB_CHECK(std::fabs(as_float(r.fr[5])) < 1e-5f);          // cos
});

CB_TEST("emulate.sh4.register_banks_swap_with_sr_rb", [] {
    sh::Bus bus = make_bus();
    // r3_bank takes r4; toggling RB makes it r3: the bank not selected is reached with ldc/stc, the selected one is r0-r7
    const sh::RunResult r = go(bus,
                               "ldc r4,r3_bank\n stc sr,r1\n mov.l K_rb,r2\n xor r2,r1\n ldc r1,sr\n mov r3,r0\n rts\n nop\n .align 2\nK_rb: .long 0x20000000\n",
                               {.a = {0x1234, 0, 0, 0}});
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(r.r[0], 0x1234u);
    CB_CHECK_EQ(r.sr & sh::sr::RB, 0u);   // (a reset leaves RB set; the program cleared it)
});

CB_TEST("emulate.sh4.rte_restores_pc_and_sr_from_spc_and_ssr", [] {
    sh::Bus bus = make_bus();
    const sh::RunResult r = go(bus, "ldc r4,spc\n ldc r5,ssr\n rte\n nop\n", {.a = {sh::kReturnAddress, 0x700000F1}});
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(r.sr & sh::sr::T, 1u);
});

CB_TEST("emulate.sh4.shad_and_shld_shift_by_a_signed_count", [] {
    sh::Bus bus = make_bus();
    sh::RunResult r = go(bus, "mov #-4,r1\n shad r1,r4\n shld r1,r5\n shad r2,r6\n rts\n nop\n", {.a = {0x80000000, 0x80000000, 4, 0}, .s = {}});
    CB_CHECK_EQ(r.r[4], 0xF8000000u);   // arithmetic right by 4
    CB_CHECK_EQ(r.r[5], 0x08000000u);   // logical right by 4
    CB_CHECK_EQ(r.r[6], 4u << 0);       // r2 is 0: no shift
    sh::Bus left = make_bus();
    r = go(left, "mov #4,r1\n shld r1,r4\n rts\n nop\n", {.a = {1, 0, 0, 0}});
    CB_CHECK_EQ(r.r[4], 16u);
});

CB_TEST("emulate.sh4.the_store_queue_flushes_on_pref", [] {
    sh::Bus bus = make_bus();
    // QACR0 = $0C selects area 3 (main RAM); eight longwords written at $E0000100 go to $0C000100 when PREF names the queue
    const sh::RunResult r = go(bus,
                               "mov.l K_q,r1\n mov #0x0C,r2\n mov.l r2,@r1\n mov.l K_sq,r3\n mov.l r4,@r3\n mov #0x1C,r0\n mov.l r5,@(r0,r3)\n pref @r3\n rts\n nop\n"
                               " .align 2\nK_q: .long 0xFF000038\nK_sq: .long 0xE0000100\n",
                               {.a = {0xAAAA5555, 0x12345678}});
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(bus.peek(0x0C000100, 4), 0xAAAA5555u);
    CB_CHECK_EQ(bus.peek(0x0C00011C, 4), 0x12345678u);
    // the chip register write and the cache operation are in the log
    bool qacr = false, pref = false;
    for (const sh::BusAccess& a : r.io) {
        qacr = qacr || (a.kind == sh::BusAccess::Kind::Write && a.address == 0xFF000038 && a.value == 0x0C);
        pref = pref || (a.kind == sh::BusAccess::Kind::Cache && a.address == 0xE0000100 && a.value == 0);
    }
    CB_CHECK(qacr);
    CB_CHECK(pref);
});

CB_TEST("emulate.sh4.chip_registers_hold_what_is_written_and_cache_ops_are_logged", [] {
    sh::Bus bus = make_bus();
    const sh::RunResult r = go(bus,
                               "mov.l K_ccr,r1\n mov.l K_v,r2\n mov.l r2,@r1\n mov.l @r1,r0\n ocbi @r4\n ocbp @r4\n ocbwb @r4\n mov.l r0,@r4\n rts\n nop\n"
                               " .align 2\nK_ccr: .long 0xFF00001C\nK_v: .long 0x0000090D\n",
                               {.a = {0x0C000400}});
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(r.r[0], 0x90Du);
    unsigned cache_ops = 0;
    for (const sh::BusAccess& a : r.io)
        if (a.kind == sh::BusAccess::Kind::Cache) ++cache_ops;
    CB_CHECK_EQ(cache_ops, 3u);
    CB_CHECK_EQ(r.io.front().text(), std::string("w4"));
});

CB_TEST("emulate.sh4.fpu_is_disabled_by_sr_fd", [] {
    sh::Bus bus = make_bus();
    const sh::RunResult r = go(bus, "ldc r4,sr\n fldi0 fr0\n rts\n nop\n", {.a = {0x700080F0}});
    CB_CHECK(r.end == sh::End::Stopped);
    CB_CHECK(r.fault && r.fault->kind == sh::Fault::Kind::FpuDisabled);
});

CB_TEST("emulate.sh4.bios_stubs_are_logged_where_they_are_entered", [] {
    sh::Bus bus = make_bus();
    bus.add_ram(0, 0x1000);
    const Bytes stub = {0x0B, 0x00, 0x00, 0xE0};
    bus.load(0x100, stub);
    bus.on_enter(0x100, 0x103, [&bus](std::uint32_t pc, const std::array<std::uint32_t, 16>& regs) { bus.note(sh::BusAccess::Kind::Call, pc & sh::kPhysicalMask, regs[7]); });
    const sh::RunResult r = go(bus, "sts.l pr,@-r15\n mov.l K_f,r1\n mov #9,r7\n jsr @r1\n nop\n lds.l @r15+,pr\n rts\n nop\n .align 2\nK_f: .long 0x80000100\n");
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(r.r[0], 0u);   // the stub's `mov #0,r0`
    CB_CHECK_EQ(r.io.size(), std::size_t{1});
    CB_CHECK(r.io[0].kind == sh::BusAccess::Kind::Call && r.io[0].address == 0x100 && r.io[0].value == 9);
});
