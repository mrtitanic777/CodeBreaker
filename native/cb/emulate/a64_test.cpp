// cb-unity: alone
// The AArch64 interpreter: flags, the data-processing groups, bitfields, multiplies, every addressing form of loads and stores, exclusives, branches, conditional select,
// scalar floating point, system registers, kernel calls, and the stops (what is not run says why). Programs are written in the assembler's own syntax.
#include "cb/emulate/a64.hpp"

#include <bit>
#include <string>

#include "cb/build/a64_asm.hpp"
#include "cb/core/testing.hpp"

namespace cb::emulate::a64 {
namespace {

constexpr std::uint64_t kCode = 0x10000, kData = 0x100000, kRo = 0x200000;

// A machine with the program at 0x10000, 64 KB of data at 0x100000 (sp at its top) and 4 KB of read-only memory at 0x200000.
struct Rig {
    RunResult result;
    std::unique_ptr<Memory> memory;

    RunResult run(const std::string& program, RunOptions options = {}, bool add_return = true) {
        const build::a64_asm::Image image = build::a64_asm::build(".org 0x10000\n" + program + (add_return ? "\n        ret\n" : "\n"));
        auto regions = std::make_shared<std::vector<Region>>();
        regions->push_back({kCode, image.data, false});
        regions->push_back({kRo, Bytes(0x1000, 0x5A), false});
        memory = std::make_unique<Memory>(regions);
        memory->map_writable(kData, 0x10000);
        if (options.sp == 0) options.sp = kData + 0x8000;
        result = emulate::a64::run(*memory, kCode, options);
        return result;
    }
    std::uint64_t x(unsigned n) const { return result.regs.x[n]; }
    double d(unsigned n) const { return std::bit_cast<double>(result.regs.v[n].lo); }
    float s(unsigned n) const { return std::bit_cast<float>(static_cast<std::uint32_t>(result.regs.v[n].lo)); }
};

}  // namespace

CB_TEST("emulate.a64.flags_of_the_arithmetic", [] {
    Rig m;
    m.run(R"(
        movn x1, #0                      // -1
        adds x2, x1, #1                  // 0, carry out, zero
        mrs x10, nzcv
        movz x3, #0x7fff, lsl #48
        movk x3, #0xffff, lsl #32
        movk x3, #0xffff, lsl #16
        movk x3, #0xffff
        adds x4, x3, #1                  // signed overflow
        mrs x11, nzcv
        mov x5, #1
        cmp x5, #2                       // 1 - 2: negative, borrow (no carry)
        mrs x12, nzcv
        mov w6, #0x7fffffff
        add w7, w6, #1
        cmn w6, #1
        mrs x13, nzcv
        mov x8, #5
        subs x9, x8, #5
        mrs x14, nzcv
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(2), 0u);
    CB_CHECK_EQ(m.x(10), 0x60000000u);
    CB_CHECK_EQ(m.x(4), 0x8000000000000000ull);
    CB_CHECK_EQ(m.x(11), 0x90000000u);
    CB_CHECK_EQ(m.x(12), 0x80000000u);
    CB_CHECK_EQ(m.x(7), 0x80000000u);
    CB_CHECK_EQ(m.x(13), 0x90000000u);   // 0x7fffffff + 1: negative with overflow
    CB_CHECK_EQ(m.x(14), 0x60000000u);
});

CB_TEST("emulate.a64.logic_shifts_and_bitfields", [] {
    Rig m;
    m.run(R"(
        movz x1, #0x1234
        movk x1, #0x5678, lsl #16
        lsl x2, x1, #4
        lsr x3, x1, #4
        movz x4, #0xff00
        mvn x4, x4                       // 0xffff...00ff
        asr x5, x4, #8
        and x6, x1, #0xff00
        orr x7, x1, #0xf0000000
        eor x8, x1, x1
        bic x9, x1, x1, lsl #0
        ubfx x10, x1, #4, #8             // 0x23
        sbfx x11, x4, #0, #8             // 0xff sign extended
        mov x12, xzr
        bfi x12, x1, #8, #8              // 0x3400
        ubfiz x13, x1, #4, #4            // (0x4) << 4
        extr x14, x1, x4, #8
        ror x15, x1, #4
        rev w16, w1
        rbit w17, w1
        clz x18, x1
        rev16 x19, x1
        sxtb x20, w4
        uxth w21, w4
        mvn x22, x1
        tst x1, #1
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(2), 0x56781234ull << 4);
    CB_CHECK_EQ(m.x(3), 0x5678123ull);
    CB_CHECK_EQ(m.x(4), 0xFFFFFFFFFFFF00FFull);
    CB_CHECK_EQ(m.x(5), 0xFFFFFFFFFFFFFF00ull);
    CB_CHECK_EQ(m.x(6), 0x1200ull);
    CB_CHECK_EQ(m.x(7), 0xF6781234ull);
    CB_CHECK_EQ(m.x(8), 0u);
    CB_CHECK_EQ(m.x(9), 0u);
    CB_CHECK_EQ(m.x(10), 0x23u);
    CB_CHECK_EQ(m.x(11), 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK_EQ(m.x(12), 0x3400u);
    CB_CHECK_EQ(m.x(13), 0x40u);
    CB_CHECK_EQ(m.x(14), 0x34FFFFFFFFFFFF00ull);
    CB_CHECK_EQ(m.x(15), 0x4000000005678123ull);
    CB_CHECK_EQ(m.x(16), 0x34127856u);
    CB_CHECK_EQ(m.x(17), 0x2C481E6Au);
    CB_CHECK_EQ(m.x(18), 33u);
    CB_CHECK_EQ(m.x(19), 0x0000000078563412ull);
    CB_CHECK_EQ(m.x(20), 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK_EQ(m.x(21), 0xFFu);
    CB_CHECK_EQ(m.x(22), ~0x56781234ull);
});

CB_TEST("emulate.a64.multiplies_divides_and_remainders", [] {
    Rig m;
    m.run(R"(
        movz x1, #0x1234, lsl #32
        mov x2, #7
        mul x3, x1, x2
        umulh x4, x1, x1
        movn x5, #0                      // -1
        smulh x6, x5, x5                 // 0 (the high half of 1)
        umulh x7, x5, x5                 // 0xfffffffffffffffe
        udiv x8, x1, x2
        msub x9, x8, x2, x1              // the remainder
        mov x10, #100
        sdiv x11, x10, x5                // -100
        sdiv x12, x10, xzr               // by zero: 0
        smull x13, w5, w5                // 1
        umull x14, w5, w5                // 0xfffffffe00000001
        madd x15, x2, x2, x2             // 56
        mneg x16, x2, x2
        mov x17, #0x8000000000000000
        sdiv x18, x17, x5                // the one overflow: stays
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(3), 0x1234ull * 7 << 32);
    CB_CHECK_EQ(m.x(4), 0x1234ull * 0x1234ull);
    CB_CHECK_EQ(m.x(6), 0u);
    CB_CHECK_EQ(m.x(7), 0xFFFFFFFFFFFFFFFEull);
    CB_CHECK_EQ(m.x(8), (0x1234ull << 32) / 7);
    CB_CHECK_EQ(m.x(9), (0x1234ull << 32) % 7);
    CB_CHECK_EQ(m.x(11), static_cast<std::uint64_t>(-100));
    CB_CHECK_EQ(m.x(12), 0u);
    CB_CHECK_EQ(m.x(13), 1u);
    CB_CHECK_EQ(m.x(14), 0xFFFFFFFE00000001ull);
    CB_CHECK_EQ(m.x(15), 56u);
    CB_CHECK_EQ(m.x(16), static_cast<std::uint64_t>(-49));
    CB_CHECK_EQ(m.x(18), 0x8000000000000000ull);
});

CB_TEST("emulate.a64.loads_and_stores_in_every_addressing_form", [] {
    Rig m;
    m.run(R"(
        movz x20, #0x10, lsl #16         // 0x100000: the data
        movn x1, #0                      // -1
        str x1, [x20]
        mov x2, #0x80
        strb w2, [x20, #8]
        ldrb w3, [x20, #8]
        ldrsb x4, [x20, #8]              // sign extended
        mov w5, #0x8000
        strh w5, [x20, #10]
        ldrsh w6, [x20, #10]
        ldrh w7, [x20, #10]
        mov x8, #3
        str w8, [x20, x8, lsl #2]        // [x20 + 12]
        ldr w9, [x20, #12]
        add x10, x20, #0x40
        str x1, [x10, #-8]!              // pre-index
        mov x11, x10
        ldr x12, [x10], #16              // post-index
        sub x13, x10, x11
        stp x2, x8, [x20, #0x20]
        ldp x14, x15, [x20, #0x20]
        stp x2, x8, [sp, #-16]!
        ldp x16, x17, [sp], #16
        ldur x18, [x20, #1]
        ldrsw x19, [x20]
        mov w21, #-1
        add x25, x20, #0x100
        ldr w23, [x20, w8, uxtw #2]
        ldr x24, [x25, w21, sxtw #3]     // negative index: x25 - 8
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(3), 0x80u);
    CB_CHECK_EQ(m.x(4), 0xFFFFFFFFFFFFFF80ull);
    CB_CHECK_EQ(m.x(6), 0xFFFF8000u);
    CB_CHECK_EQ(m.x(7), 0x8000u);
    CB_CHECK_EQ(m.x(9), 3u);
    CB_CHECK_EQ(m.x(12), 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK_EQ(m.x(13), 0x10u);
    CB_CHECK_EQ(m.x(14), 0x80u);
    CB_CHECK_EQ(m.x(15), 3u);
    CB_CHECK_EQ(m.x(16), 0x80u);
    CB_CHECK_EQ(m.x(17), 3u);
    CB_CHECK_EQ(m.x(19), 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK_EQ(m.x(23), 3u);
    std::uint64_t v = 0;
    CB_CHECK(m.memory->read(kData, 8, v));
    CB_CHECK_EQ(v & 0xFFFFFFFFull, 0xFFFFFFFFull);
    CB_CHECK(m.memory->read(kData + 0x38, 8, v));
    CB_CHECK_EQ(v, 0xFFFFFFFFFFFFFFFFull);
});

CB_TEST("emulate.a64.branches_calls_and_loops", [] {
    Rig m;
    m.run(R"(
        mov x28, x30
        mov x1, #0
        mov x2, #10
loop:
        add x1, x1, x2
        subs x2, x2, #1
        b.ne loop                        // 55
        bl square
        cbz x0, skip
        mov x4, #1
skip:
        mov x5, #0x20
        tbz w5, #5, wrong
        mov x6, #6
        adr x7, target
        blr x7
        b done
wrong:
        mov x6, #99
target:
        mov x8, #8
        ret
done:
        mov x9, #9
        b finish
square:
        mul x0, x1, x1
        ret
finish:
        mov x30, x28
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(1), 55u);
    CB_CHECK_EQ(m.x(0), 3025u);
    CB_CHECK_EQ(m.x(4), 1u);
    CB_CHECK_EQ(m.x(6), 6u);
    CB_CHECK_EQ(m.x(8), 8u);
    CB_CHECK_EQ(m.x(9), 9u);
});

CB_TEST("emulate.a64.conditional_select_and_compare", [] {
    Rig m;
    m.run(R"(
        mov x1, #5
        mov x2, #9
        cmp x1, x2
        csel x3, x1, x2, lt              // 5
        csel x4, x1, x2, gt              // 9
        cset x5, lo
        csetm x6, hi
        csinc x7, x1, x2, ge             // 10
        csneg x8, x1, x2, ge             // -9
        cinc x9, x1, lt                  // 6
        ccmp x1, #5, #4, lt              // lt holds: compares 5 with 5 -> Z
        cset x10, eq
        ccmp x1, #5, #0, gt              // gt does not hold: flags 0
        cset x11, eq
        cmn x1, x2
        cset x12, mi
    )");
    CB_CHECK_EQ(m.x(3), 5u);
    CB_CHECK_EQ(m.x(4), 9u);
    CB_CHECK_EQ(m.x(5), 1u);
    CB_CHECK_EQ(m.x(6), 0u);
    CB_CHECK_EQ(m.x(7), 10u);
    CB_CHECK_EQ(m.x(8), static_cast<std::uint64_t>(-9));
    CB_CHECK_EQ(m.x(9), 6u);
    CB_CHECK_EQ(m.x(10), 1u);
    CB_CHECK_EQ(m.x(11), 0u);
    CB_CHECK_EQ(m.x(12), 0u);
});

CB_TEST("emulate.a64.exclusive_access", [] {
    Rig m;
    m.run(R"(
        movz x20, #0x10, lsl #16
        mov x1, #5
        str x1, [x20]
        ldxr x2, [x20]
        add x2, x2, #1
        stxr w3, x2, [x20]               // succeeds: 0
        ldr x4, [x20]
        ldxr x5, [x20]
        clrex
        stxr w6, x1, [x20]               // the monitor is gone: 1
        ldr x7, [x20]
        mov x8, #3
        stlr x8, [x20]
        ldar x9, [x20]
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(3), 0u);
    CB_CHECK_EQ(m.x(4), 6u);
    CB_CHECK_EQ(m.x(6), 1u);
    CB_CHECK_EQ(m.x(7), 6u);
    CB_CHECK_EQ(m.x(9), 3u);
});

CB_TEST("emulate.a64.scalar_floating_point", [] {
    Rig m;
    m.run(R"(
        fmov d0, #1.5
        fmov d1, #2.0
        fadd d2, d0, d1                  // 3.5
        fmul d3, d0, d1                  // 3
        fdiv d4, d0, d1                  // 0.75
        fsub d5, d0, d1
        fsqrt d6, d1
        fmadd d7, d0, d1, d0             // 1.5 * 2 + 1.5
        fneg d8, d0
        fabs d9, d8
        fcvtzs x1, d2                    // 3
        scvtf d10, x1
        fcmp d0, d1
        mrs x2, nzcv                     // less: N
        fcmp d1, d1
        mrs x3, nzcv                     // equal: ZC
        fcvt s11, d4
        fadd s12, s11, s11               // 1.5
        fcvt d13, s12
        frintm d14, d5                   // floor(-0.5) = -1
        fmov x4, d3
        fmov d15, x4
        fmov w5, s12
        movi d16, #0
        fcsel d17, d0, d1, lt
        fcvtzu w6, d0
        scvtf s18, w6
        fcmp d0, #0.0
        mrs x7, nzcv
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.d(2), 3.5);
    CB_CHECK_EQ(m.d(3), 3.0);
    CB_CHECK_EQ(m.d(4), 0.75);
    CB_CHECK_EQ(m.d(5), -0.5);
    CB_CHECK_EQ(m.d(6), std::sqrt(2.0));
    CB_CHECK_EQ(m.d(7), 4.5);
    CB_CHECK_EQ(m.d(9), 1.5);
    CB_CHECK_EQ(m.x(1), 3u);
    CB_CHECK_EQ(m.d(10), 3.0);
    CB_CHECK_EQ(m.x(2), 0x80000000u);
    CB_CHECK_EQ(m.x(3), 0x60000000u);
    CB_CHECK_EQ(m.s(12), 1.5f);
    CB_CHECK_EQ(m.d(13), 1.5);
    CB_CHECK_EQ(m.d(14), -1.0);
    CB_CHECK_EQ(m.d(15), 3.0);
    CB_CHECK_EQ(m.x(5), 0x3FC00000u);
    CB_CHECK_EQ(m.d(17), 2.0);   // (the last compare said equal: not lt)
    CB_CHECK_EQ(m.x(6), 1u);
    CB_CHECK_EQ(m.s(18), 1.0f);
    CB_CHECK_EQ(m.x(7), 0x20000000u);
});

CB_TEST("emulate.a64.floating_point_conversions_saturate", [] {
    Rig m;
    m.run(R"(
        fmov d0, #-2.5
        fcvtzs w1, d0                    // -2
        fcvtzu w2, d0                    // 0 (negative, unsigned)
        fcvtms x3, d0                    // -3
        fcvtps x4, d0                    // -2
        fcvtas x5, d0                    // -3 (ties away)
        fcvtns x6, d0                    // -2 (ties even)
        movz x7, #0x7ff0, lsl #48        // +infinity as a double
        fmov d1, x7
        fcvtzs x8, d1                    // saturates
        fcvtzs w9, d1
        add x10, x7, #1                  // a NaN
        fmov d2, x10
        fcvtzs x11, d2
        fmov d3, #4.0
        fcvtzs w12, d3, #3               // 4.0 * 8 = 32
        scvtf d4, w12, #4                // 32 / 16 = 2
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(1), 0xFFFFFFFEu);
    CB_CHECK_EQ(m.x(2), 0u);
    CB_CHECK_EQ(m.x(3), static_cast<std::uint64_t>(-3));
    CB_CHECK_EQ(m.x(4), static_cast<std::uint64_t>(-2));
    CB_CHECK_EQ(m.x(5), static_cast<std::uint64_t>(-3));
    CB_CHECK_EQ(m.x(6), static_cast<std::uint64_t>(-2));
    CB_CHECK_EQ(m.x(8), 0x7FFFFFFFFFFFFFFFull);
    CB_CHECK_EQ(m.x(9), 0x7FFFFFFFu);
    CB_CHECK_EQ(m.x(11), 0u);   // (NaN)
    CB_CHECK_EQ(m.x(12), 32u);
    CB_CHECK_EQ(m.d(4), 2.0);
});

CB_TEST("emulate.a64.system_registers", [] {
    Rig m;
    RunOptions o;
    o.tpidr_el0 = 0x1234;
    o.tpidrro_el0 = 0x5678;
    m.run(R"(
        mrs x1, tpidr_el0
        mrs x2, tpidrro_el0
        mov x3, #0xff
        msr tpidr_el0, x3
        mrs x4, tpidr_el0
        mrs x5, cntvct_el0
        mrs x6, cntvct_el0
        mrs x7, cntfrq_el0
        mov x8, #0x40000000
        lsl x8, x8, #1
        msr nzcv, x8
        mrs x9, nzcv
        mov x10, #3
        msr fpcr, x10
        mrs x11, fpcr
    )", o);
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(1), 0x1234u);
    CB_CHECK_EQ(m.x(2), 0x5678u);
    CB_CHECK_EQ(m.x(4), 0xFFu);
    CB_CHECK(m.x(6) > m.x(5));
    CB_CHECK_EQ(m.x(7), 19200000u);
    CB_CHECK_EQ(m.x(9), 0x80000000u);
    CB_CHECK_EQ(m.x(11), 3u);
});

CB_TEST("emulate.a64.kernel_calls_are_logged_and_the_exits_end_the_run", [] {
    Rig m;
    RunResult r = m.run(R"(
        mov x0, #0x21
        mov x1, #7
        svc #0x1E
        mov x9, x0
        mov x1, #0x1000000
        svc #1                           // SetHeapSize
        mov x10, x1
        mov x0, #0
        svc #7                           // ExitProcess
        mov x11, #1
    )");
    CB_CHECK_EQ(r.exit(), "stopped: svc ExitProcess");
    CB_CHECK_EQ(r.calls.size(), 3u);
    CB_CHECK_EQ(r.calls[0].number, 0x1Eu);
    CB_CHECK_EQ(svc_name(r.calls[0].number), "GetSystemTick");
    CB_CHECK_EQ(r.calls[1].args[1], 0x1000000u);
    CB_CHECK_EQ(m.x(10), kHeapBase);
    CB_CHECK(m.x(9) > 0);
    CB_CHECK_EQ(m.x(11), 0u);
    CB_CHECK_EQ(svc_name(0x21), "SendSyncRequest");
    CB_CHECK_EQ(svc_name(0x7F), "CallSecureMonitor");
    CB_CHECK_EQ(svc_name(0x3F), "");
    // the heap is usable after SetHeapSize
    Rig h;
    h.run(R"(
        mov x1, #0x200000
        svc #1
        mov x2, #77
        str x2, [x1]
        ldr x3, [x1]
    )");
    CB_CHECK_EQ(h.x(3), 77u);
});

CB_TEST("emulate.a64.an_ipc_request_is_decoded", [] {
    Rig m;
    RunOptions o;
    o.tpidrro_el0 = kData;
    RunResult r = m.run(R"(
        movz x20, #0x10, lsl #16
        movz x1, #4                      // header: type 4 (request), no descriptors
        str x1, [x20]
        movz x1, #6                      // 6 words of raw data
        str w1, [x20, #4]
        movz x1, #0x4653
        movk x1, #0x4943, lsl #16        // 'SFCI'
        str x1, [x20, #16]
        mov x1, #1
        str x1, [x20, #24]               // command 1: GetService
        movz x1, #0x7366                 // "fs"
        movk x1, #0x2d70, lsl #16        // "p-"
        str x1, [x20, #32]
        mov x0, #0x1234
        svc #0x21
    )", o);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.calls.size(), 1u);
    CB_CHECK(r.calls[0].note.find("cmd 1") != std::string::npos);
});

CB_TEST("emulate.a64.what_cannot_run_stops_with_its_reason", [] {
    Rig m;
    CB_CHECK_EQ(m.run("        movz x20, #0x20, lsl #16\n        ldr x1, [x20]\n        str x1, [x20]").exit().substr(0, 41), "stopped: data abort: write to read-only m");
    CB_CHECK(m.run("        mov x20, #0x1000\n        ldr x1, [x20]").exit().starts_with("stopped: data abort: read at 0x1000"));
    CB_CHECK(m.run("        brk #3").exit().starts_with("stopped: brk"));
    CB_CHECK(m.run("        .word 0xffffffff").exit().starts_with("stopped: undefined instruction ffffffff"));
    CB_CHECK(m.run("        mrs x0, midr_el1").exit().find("system register not modelled") != std::string::npos);
    CB_CHECK(m.run("        crc32b w0, w1, w2").exit().find("crc32 is not run") != std::string::npos);
    RunOptions few;
    few.max_steps = 50;
    CB_CHECK_EQ(m.run("spin:\n        b spin", few, false).exit(), "limit");
    CB_CHECK(m.run("        mov x30, #0x4000\n        ret", {}, false).exit().starts_with("stopped: instruction fetch from unmapped address 0x4000"));
    CB_CHECK(m.run("        mov x1, #1", {}).end == End::Return);
});

CB_TEST("emulate.a64.memory_keeps_the_shared_image_unchanged", [] {
    auto regions = std::make_shared<std::vector<Region>>();
    regions->push_back({0x1000, Bytes(16, 0x11), true});
    Memory a(regions), b(regions);
    CB_CHECK(a.write(0x1004, 4, 0xDEADBEEF));
    std::uint64_t v = 0;
    CB_CHECK(a.read(0x1004, 4, v));
    CB_CHECK_EQ(v, 0xDEADBEEFull);
    CB_CHECK(b.read(0x1004, 4, v));
    CB_CHECK_EQ(v, 0x11111111ull);
    CB_CHECK_EQ(a.changes().size(), 4u);
    CB_CHECK(!a.read(0x2000, 1, v));
    CB_CHECK(!a.write(0x2000, 1, 1));
    Memory ro;
    ro.map_zero(0x3000, 16);
    CB_CHECK(ro.read(0x3000, 8, v) && v == 0);
    CB_CHECK(!ro.write(0x3000, 1, 1));
});


namespace {

std::uint64_t lane(const V128& v, unsigned bytes, unsigned i) {
    std::uint64_t r = 0;
    for (unsigned k = 0; k < bytes; ++k) {
        const unsigned at = i * bytes + k;
        r |= static_cast<std::uint64_t>((at < 8 ? v.lo >> (8 * at) : v.hi >> (8 * (at - 8))) & 0xFF) << (8 * k);
    }
    return r;
}

}  // namespace

CB_TEST("emulate.a64.vector_instructions", [] {
    Rig m;
    m.run(R"(
        movz x20, #0x10, lsl #16
        movi v0.16b, #0x11
        movi v1.16b, #0x22
        add v2.16b, v0.16b, v1.16b       // 0x33 in every byte
        st1 {v0.16b, v1.16b}, [x20], #32
        sub x20, x20, #32
        ld1 {v4.16b, v5.16b}, [x20]
        umov x0, v5.d[0]
        eor v6.16b, v0.16b, v1.16b       // 0x33
        cnt v7.16b, v6.16b               // 4 bits set in each
        addv b8, v7.16b                  // 64
        umov w1, v8.b[0]
        dup v9.4s, w1
        fmov v11.4s, #2.0
        fmul v12.4s, v11.4s, v11.4s      // 4.0
        shl v14.4s, v9.4s, #2            // 256
        ushr v15.4s, v14.4s, #4          // 16
        xtn v16.4h, v14.4s
        ushll v17.4s, v16.4h, #1         // 512
        zip1 v18.4s, v9.4s, v11.4s
        movi v19.16b, #3
        tbl v20.16b, {v1.16b}, v19.16b   // byte 3 of v1: 0x22
        ins v21.s[2], w1
        mvni v22.4s, #0
        sub x21, x20, x20
        ld1r {v23.8h}, [x20]
        mov v24.16b, v0.16b
        bsl v24.16b, v1.16b, v6.16b
        ext v25.16b, v0.16b, v1.16b, #8
        uaddlv h26, v0.16b
        sshr v27.2d, v22.2d, #60
        fcvtzs v28.4s, v12.4s            // 4
        scvtf v29.4s, v28.4s             // 4.0
        cmeq v30.16b, v0.16b, v0.16b
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    const auto& v = m.result.regs.v;
    CB_CHECK_EQ(lane(v[2], 1, 15), 0x33u);
    CB_CHECK_EQ(m.x(0), 0x2222222222222222ull);
    CB_CHECK_EQ(lane(v[7], 1, 3), 4u);
    CB_CHECK_EQ(m.x(1), 64u);
    CB_CHECK_EQ(lane(v[9], 4, 3), 64u);
    CB_CHECK_EQ(lane(v[12], 4, 2), std::bit_cast<std::uint32_t>(4.0f));
    CB_CHECK_EQ(lane(v[14], 4, 1), 256u);
    CB_CHECK_EQ(lane(v[15], 4, 1), 16u);
    CB_CHECK_EQ(lane(v[16], 2, 3), 256u);
    CB_CHECK_EQ(v[16].hi, 0u);
    CB_CHECK_EQ(lane(v[17], 4, 3), 512u);
    CB_CHECK_EQ(lane(v[18], 4, 0), 64u);
    CB_CHECK_EQ(lane(v[18], 4, 1), std::bit_cast<std::uint32_t>(2.0f));
    CB_CHECK_EQ(lane(v[20], 1, 5), 0x22u);
    CB_CHECK_EQ(lane(v[21], 4, 2), 64u);
    CB_CHECK_EQ(lane(v[22], 4, 1), 0xFFFFFFFFu);
    CB_CHECK_EQ(lane(v[23], 2, 7), 0x1111u);
    CB_CHECK_EQ(lane(v[24], 1, 0), 0x22u);
    CB_CHECK_EQ(lane(v[25], 1, 0), 0x11u);
    CB_CHECK_EQ(lane(v[26], 2, 0), 16u * 0x11u);
    CB_CHECK_EQ(lane(v[27], 8, 0), 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK_EQ(lane(v[28], 4, 1), 4u);
    CB_CHECK_EQ(lane(v[29], 4, 1), std::bit_cast<std::uint32_t>(4.0f));
    CB_CHECK_EQ(lane(v[30], 1, 9), 0xFFu);
    std::uint64_t stored = 0;
    CB_CHECK(m.memory->read(kData + 16, 8, stored));
    CB_CHECK_EQ(stored, 0x2222222222222222ull);
});

CB_TEST("emulate.a64.a_vector_memcpy_loop", [] {
    Rig m;
    m.run(R"(
        movz x20, #0x10, lsl #16         // source
        add x21, x20, #0x1000            // destination
        mov x2, #0
fill:
        strb w2, [x20, x2]
        add x2, x2, #1
        cmp x2, #64
        b.ne fill
        mov x3, #2
copy:
        ld1 {v0.16b, v1.16b}, [x20], #32
        st1 {v0.16b, v1.16b}, [x21], #32
        subs x3, x3, #1
        b.ne copy
        sub x22, x21, #64
        ldr x4, [x22, #8]
        ldr x5, [x22, #56]
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.x(4), 0x0F0E0D0C0B0A0908ull);
    CB_CHECK_EQ(m.x(5), 0x3F3E3D3C3B3A3938ull);
});

}  // namespace cb::emulate::a64
