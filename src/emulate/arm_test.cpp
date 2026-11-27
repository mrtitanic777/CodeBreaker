// emulate/arm_test.cpp -- the ARM7TDMI interpreter: each behaviour worked out by hand from the ARM Architecture Reference Manual (flags
// of the arithmetic, the shifter's carry, the multiplies, the loads' rotations, the block transfers, the Thumb formats, the state switch, the
// modes' banked registers) and from GBATEK (the memory map, the mirrors, the byte writes to video memory, the open bus, the BIOS calls).
// Programs are assembled with CodeBreaker's own assembler at $02000000 (work RAM) and run as a function.
#include "emulate/arm.hpp"

#include <format>
#include <memory>
#include <string>

#include "source/risc_source.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/isa.hpp"

namespace cb::emulate::arm {
namespace {

constexpr std::uint32_t kCode = 0x02000000;

// A bus with `source` assembled to $02000000 (the assembler's `.org` is in the text it is given).
struct Rig {
    Bus bus;
    void load(const std::string& source) {
        const Bytes image = source::risc_source::gnu_arm(".org 0x02000000\n" + source);
        for (std::size_t i = 0; i < image.size(); ++i) bus.ewram()[i] = image[i];
    }
    RunResult go(std::uint32_t entry = kCode, RunOptions options = {}) { return run(bus, entry, options); }
};

constexpr std::uint32_t N = 0x80000000, Z = 0x40000000, C = 0x20000000, V = 0x10000000;

CB_TEST("emulate.arm.flags_of_the_arithmetic", [] {
    Rig add;
    add.load(" mvn r0, #0x80000000\n adds r0, r0, #1\n mrs r1, cpsr\n bx lr\n");   // 0x7FFFFFFF + 1: N and V
    const RunResult a = add.go();
    CB_CHECK(a.end == End::Return);
    CB_CHECK_EQ(a.r[0], 0x80000000u);
    CB_CHECK_EQ(a.r[1], N | V | 0x1F);

    Rig sub;
    sub.load(" subs r1, r2, r3\n mrs r0, cpsr\n bx lr\n");   // 5 - 7: a borrow (no carry), negative
    RunOptions five_seven;
    five_seven.a = {0, 0, 5, 7};
    const RunResult b = sub.go(kCode, five_seven);
    CB_CHECK_EQ(b.r[1], 0xFFFFFFFEu);
    CB_CHECK_EQ(b.r[0], N | 0x1F);

    Rig equal;
    equal.load(" cmp r0, r1\n moveq r2, #1\n movne r2, #2\n mrs r3, cpsr\n bx lr\n");   // equal: Z and C (no borrow)
    RunOptions same;
    same.a = {5, 5, 0, 0};
    const RunResult c = equal.go(kCode, same);
    CB_CHECK_EQ(c.r[2], 1u);
    CB_CHECK_EQ(c.r[3], Z | C | 0x1F);

    Rig carry;
    carry.load(" adds r2, r0, r1\n adc r3, r2, #0\n mrs r4, cpsr\n bx lr\n");   // 0xFFFFFFFF + 1 = 0 with carry; adc adds it
    RunOptions wrap;
    wrap.a = {0xFFFFFFFF, 1, 0, 0};
    const RunResult d = carry.go(kCode, wrap);
    CB_CHECK_EQ(d.r[2], 0u);
    CB_CHECK_EQ(d.r[3], 1u);
    CB_CHECK_EQ(d.r[4], Z | C | 0x1F);   // (the flags of the ADDS: the ADC did not set any)
});

CB_TEST("emulate.arm.the_shifters_carry", [] {
    Rig lsr;
    lsr.load(" movs r0, r0, lsr #1\n mrs r1, cpsr\n bx lr\n");   // 1 >> 1 = 0, the bit shifted out is the carry
    RunOptions one;
    one.a = {1, 0, 0, 0};
    const RunResult a = lsr.go(kCode, one);
    CB_CHECK_EQ(a.r[0], 0u);
    CB_CHECK_EQ(a.r[1], Z | C | 0x1F);

    Rig asr;
    asr.load(" movs r0, r0, asr #32\n mrs r1, cpsr\n bx lr\n");   // a shift by 32 fills with the sign, which is the carry
    RunOptions top;
    top.a = {0x80000000, 0, 0, 0};
    const RunResult b = asr.go(kCode, top);
    CB_CHECK_EQ(b.r[0], 0xFFFFFFFFu);
    CB_CHECK_EQ(b.r[1], N | C | 0x1F);

    Rig reg;
    reg.load(" mov r2, r0, lsl r1\n mov r3, r0, lsr r1\n movs r4, r0, ror r1\n mrs r5, cpsr\n bx lr\n");
    RunOptions by_register;
    by_register.a = {0x80000001, 33, 0, 0};   // by a register: 33 clears a LSL and a LSR; a ROR by 33 is by 1
    const RunResult c = reg.go(kCode, by_register);
    CB_CHECK_EQ(c.r[2], 0u);
    CB_CHECK_EQ(c.r[3], 0u);
    CB_CHECK_EQ(c.r[4], 0xC0000000u);   // 0x80000001 rotated right by one
    CB_CHECK_EQ(c.r[5], N | C | 0x1F);

    Rig rrx;
    rrx.load(" movs r1, r0, rrx\n mov r2, #0\n adc r2, r2, #0\n bx lr\n");   // the carry from RRX's bit 0 is added next
    RunOptions odd;
    odd.a = {3, 0, 0, 0};
    const RunResult d = rrx.go(kCode, odd);
    CB_CHECK_EQ(d.r[1], 1u);   // (the carry was clear: nothing rotated into bit 31)
    CB_CHECK_EQ(d.r[2], 1u);   // (and the bit shifted out is the new carry)

    Rig immediate;
    immediate.load(" movs r0, #0xFF000000\n mrs r1, cpsr\n bx lr\n");   // a rotated immediate's top bit is the carry of a logical operation
    const RunResult e = immediate.go();
    CB_CHECK_EQ(e.r[1], N | C | 0x1F);
});

CB_TEST("emulate.arm.the_multiplies", [] {
    Rig mul;
    mul.load(" mul r2, r0, r1\n mla r3, r0, r1, r1\n umull r4, r5, r0, r1\n smull r6, r7, r0, r1\n bx lr\n");
    RunOptions options;
    options.a = {0xFFFFFFFF, 2, 0, 0};
    const RunResult r = mul.go(kCode, options);
    CB_CHECK_EQ(r.r[2], 0xFFFFFFFEu);   // low 32 bits of 0xFFFFFFFF * 2
    CB_CHECK_EQ(r.r[3], 0u);            // + 2 wraps to 0
    CB_CHECK_EQ(r.r[4], 0xFFFFFFFEu);   // umull: 0x1_FFFFFFFE
    CB_CHECK_EQ(r.r[5], 1u);
    CB_CHECK_EQ(r.r[6], 0xFFFFFFFEu);   // smull: -1 * 2 = -2
    CB_CHECK_EQ(r.r[7], 0xFFFFFFFFu);

    Rig flags;
    flags.load(" muls r2, r0, r1\n mrs r3, cpsr\n bx lr\n");
    RunOptions zero;
    zero.a = {0x10000, 0x10000, 0, 0};   // 2^32: the low word is zero
    const RunResult z = flags.go(kCode, zero);
    CB_CHECK_EQ(z.r[2], 0u);
    CB_CHECK_EQ(z.r[3] & (N | Z), Z);
});

CB_TEST("emulate.arm.loads_and_stores_of_every_size_and_their_quirks", [] {
    Rig rig;
    rig.load(
        " ldr r4, [r0]\n"
        " ldr r5, [r0, #1]\n"      // a misaligned word load rotates the aligned word right by 8 bits per byte of misalignment
        " ldrb r6, [r0, #1]\n"
        " ldrh r7, [r0, #1]\n"     // a misaligned halfword load rotates by 8
        " ldrsh r8, [r0, #1]\n"    // ... and a misaligned signed halfword is a signed byte
        " ldrsb r9, [r0, #3]\n"
        " ldrsb r10, [r0, #4]\n"
        " ldrsh r11, [r0, #4]\n"
        " str r1, [r0, #9]\n"      // stores align down
        " strh r1, [r0, #0xD]\n"
        " strb r1, [r0, #0x10]\n"
        " bx lr\n");
    rig.bus.write(0x02000100, 4, 0x44332211);
    rig.bus.write(0x02000104, 4, 0x000080FF);
    RunOptions options;
    options.a = {0x02000100, 0xAABBCCDD, 0, 0};
    const RunResult r = rig.go(kCode, options);
    CB_CHECK(r.end == End::Return);
    CB_CHECK_EQ(r.r[4], 0x44332211u);
    CB_CHECK_EQ(r.r[5], 0x11443322u);
    CB_CHECK_EQ(r.r[6], 0x22u);
    CB_CHECK_EQ(r.r[7], 0x11000022u);   // 0x00002211 rotated right by 8
    CB_CHECK_EQ(r.r[8], 0x22u);
    CB_CHECK_EQ(r.r[9], 0x44u);
    CB_CHECK_EQ(r.r[10], 0xFFFFFFFFu);  // byte 0xFF
    CB_CHECK_EQ(r.r[11], 0xFFFF80FFu);  // halfword 0x80FF
    CB_CHECK_EQ(rig.bus.read(0x02000108, 4), 0xAABBCCDDu);
    CB_CHECK_EQ(rig.bus.read(0x0200010C, 2), 0xCCDDu);
    CB_CHECK_EQ(rig.bus.read(0x02000110, 1), 0xDDu);

    Rig indexing;
    indexing.load(" ldr r2, [r0, #4]!\n ldr r3, [r0], #4\n ldr r4, [r0, #-8]\n str r2, [r0, #-4]!\n bx lr\n");
    indexing.bus.write(0x02000100, 4, 0x11111111);
    indexing.bus.write(0x02000104, 4, 0x22222222);
    indexing.bus.write(0x02000108, 4, 0x33333333);
    RunOptions base;
    base.a = {0x02000100, 0, 0, 0};
    const RunResult i = indexing.go(kCode, base);
    CB_CHECK_EQ(i.r[2], 0x22222222u);
    CB_CHECK_EQ(i.r[3], 0x22222222u);   // post-indexed: loaded from the written-back base ($104), then $108
    CB_CHECK_EQ(i.r[4], 0x11111111u);   // $108 - 8
    CB_CHECK_EQ(i.r[0], 0x02000104u);   // str with a pre-decrement: $108 - 4
    CB_CHECK_EQ(indexing.bus.read(0x02000104, 4), 0x22222222u);

    Rig swap;
    swap.load(" swp r2, r1, [r0]\n bx lr\n");
    swap.bus.write(0x02000100, 4, 0x11111111);
    RunOptions values;
    values.a = {0x02000100, 0x22222222, 0, 0};
    const RunResult s = swap.go(kCode, values);
    CB_CHECK_EQ(s.r[2], 0x11111111u);
    CB_CHECK_EQ(swap.bus.read(0x02000100, 4), 0x22222222u);
});

CB_TEST("emulate.arm.block_transfers_and_the_stack", [] {
    Rig arm;
    arm.load(" push {r4, r5, lr}\n mov r4, #0\n mov r5, #0\n pop {r4, r5, pc}\n");
    RunOptions options;
    options.s[0] = 0x11;
    options.s[1] = 0x22;
    const RunResult a = arm.go(kCode, options);
    CB_CHECK(a.end == End::Return);
    CB_CHECK_EQ(a.r[4], 0x11u);
    CB_CHECK_EQ(a.r[5], 0x22u);
    CB_CHECK_EQ(a.r[13], 0x03007F00u);
    // the lowest register went to the lowest address: r4 at sp - 12, r5 at sp - 8, lr at sp - 4
    CB_CHECK_EQ(arm.bus.read(0x03007F00 - 12, 4), 0x11u);
    CB_CHECK_EQ(arm.bus.read(0x03007F00 - 8, 4), 0x22u);
    CB_CHECK_EQ(arm.bus.read(0x03007F00 - 4, 4), kReturnAddress);

    Rig thumb;
    thumb.load(" .thumb\n push {r4, lr}\n movs r4, #9\n pop {r4, pc}\n");
    const RunResult t = thumb.go(kCode | 1, options);
    CB_CHECK(t.end == End::Return);
    CB_CHECK_EQ(t.r[4], 0x11u);
    CB_CHECK_EQ(t.r[13], 0x03007F00u);

    Rig modes;   // ldmia/stmia with writeback; ldmib and stmdb address the other side of the base
    modes.load(" stmia r0!, {r1, r2}\n stmdb r0, {r1, r2}\n ldmib r3, {r4, r5}\n bx lr\n");
    modes.bus.write(0x02000204, 4, 0xA);
    modes.bus.write(0x02000208, 4, 0xB);
    RunOptions regs;
    regs.a = {0x02000100, 1, 2, 0x02000200};
    const RunResult m = modes.go(kCode, regs);
    CB_CHECK_EQ(m.r[0], 0x02000108u);
    CB_CHECK_EQ(modes.bus.read(0x02000100, 4), 1u);
    CB_CHECK_EQ(modes.bus.read(0x02000104, 4), 2u);
    CB_CHECK_EQ(m.r[4], 0xAu);
    CB_CHECK_EQ(m.r[5], 0xBu);
});

CB_TEST("emulate.arm.branches_loops_and_conditions", [] {
    Rig loop;
    loop.load(
        " mov r1, #0\n"
        "again: add r1, r1, r0\n"
        " subs r0, r0, #1\n"
        " bne again\n"
        " mov r0, r1\n"
        " bx lr\n");
    RunOptions five;
    five.a = {5, 0, 0, 0};
    const RunResult r = loop.go(kCode, five);
    CB_CHECK_EQ(r.r[0], 15u);   // 5 + 4 + 3 + 2 + 1
    CB_CHECK_EQ(r.steps, std::uint64_t{1 + 5 * 3 + 2});

    Rig call;
    call.load(
        " push {lr}\n"
        " bl double\n"
        " bl double\n"
        " pop {pc}\n"
        "double: add r0, r0, r0\n"
        " bx lr\n");
    RunOptions three;
    three.a = {3, 0, 0, 0};
    CB_CHECK_EQ(call.go(kCode, three).r[0], 12u);

    Rig forever;
    forever.load("spin: b spin\n");
    RunOptions few;
    few.max_steps = 100;
    const RunResult l = forever.go(kCode, few);
    CB_CHECK(l.end == End::Limit);
    CB_CHECK_EQ(l.steps, std::uint64_t{100});
    CB_CHECK_EQ(l.exit(), "limit");
});

CB_TEST("emulate.arm.thumb_state_and_the_switch_between_states", [] {
    Rig thumb;
    thumb.load(" .thumb\n movs r0, #5\n adds r0, r0, r0\n lsls r0, r0, #2\n bx lr\n");
    const RunResult t = thumb.go(kCode | 1);
    CB_CHECK(t.end == End::Return);
    CB_CHECK_EQ(t.r[0], 40u);
    CB_CHECK_EQ(t.steps, std::uint64_t{4});

    // ARM calls a Thumb function through `bx`: `add r1, pc, #1` makes the Thumb address (pc is two instructions ahead: $02000008, +1)
    Rig mix;
    mix.load(" add r1, pc, #1\n bx r1\n .thumb\n movs r0, #7\n bx lr\n");
    const RunResult m = mix.go();
    CB_CHECK(m.end == End::Return);
    CB_CHECK_EQ(m.r[0], 7u);


    Rig conditional;   // a conditional Thumb branch
    conditional.load(" .thumb\n cmp r0, #0\n beq zero\n movs r0, #1\n bx lr\nzero: movs r0, #2\n bx lr\n");
    RunOptions nonzero;
    nonzero.a = {9, 0, 0, 0};
    CB_CHECK_EQ(conditional.go(kCode | 1, nonzero).r[0], 1u);
    CB_CHECK_EQ(conditional.go(kCode | 1).r[0], 2u);

    Rig literal;   // a PC-relative load in Thumb state: the base is the word-aligned pc + 4
    literal.load(" .thumb\n nop\n ldr r0, pool\n bx lr\n nop\npool: .word 0x12345678\n");
    CB_CHECK_EQ(literal.go(kCode | 1).r[0], 0x12345678u);
});

CB_TEST("emulate.arm.the_status_registers_and_the_banked_registers_of_each_mode", [] {
    Rig rig;
    rig.load(
        " msr cpsr_c, #0x13\n"       // supervisor mode: its own r13 and r14
        " mrs r0, cpsr\n"
        " mov sp, #0x1000\n"
        " msr cpsr_c, #0x1F\n"       // back to system mode: its r13 is as it was
        " bx lr\n");
    const RunResult r = rig.go();
    CB_CHECK(r.end == End::Return);
    CB_CHECK_EQ(r.r[0], 0x13u);
    CB_CHECK_EQ(r.r[13], 0x03007F00u);
    CB_CHECK_EQ(r.cpsr & 0x1F, 0x1Fu);

    Rig fiq;   // FIQ has its own r8-r12
    fiq.load(" mov r8, #0x11\n msr cpsr_c, #0x11\n mov r8, #0x22\n msr cpsr_c, #0x1F\n mov r0, r8\n bx lr\n");
    CB_CHECK_EQ(fiq.go().r[0], 0x11u);

    Rig flags;   // flags written whole by MSR; the mode bits are untouched by the flags field
    flags.load(" msr cpsr_f, #0xA0000000\n mrs r0, cpsr\n bx lr\n");
    CB_CHECK_EQ(flags.go().r[0], N | C | 0x1F);

    Rig saved;   // SPSR: written in a mode that has one, read back
    saved.load(" msr cpsr_c, #0x12\n msr spsr_f, #0x50000000\n mrs r0, spsr\n msr cpsr_c, #0x1F\n bx lr\n");
    CB_CHECK_EQ(saved.go().r[0], 0x50000000u);
});

CB_TEST("emulate.arm.the_bios_is_logged_and_the_arithmetic_calls_run", [] {
    Rig div;
    div.load(" mov r0, #17\n mov r1, #5\n swi 0x60000\n bx lr\n");   // Div: r0 = 3, r1 = 2, r3 = |3|
    const RunResult d = div.go();
    CB_CHECK_EQ(d.r[0], 3u);
    CB_CHECK_EQ(d.r[1], 2u);
    CB_CHECK_EQ(d.r[3], 3u);
    CB_CHECK_EQ(d.bios.size(), std::size_t{1});
    CB_CHECK_EQ(d.bios[0].function, 6u);
    CB_CHECK_EQ(d.bios[0].text(), "SWI:06(11,5,0,0)");

    Rig negative;
    negative.load(" mvn r0, #6\n mov r1, #4\n swi 0x60000\n bx lr\n");   // -7 / 4 = -1 remainder -3, |q| = 1
    const RunResult n = negative.go();
    CB_CHECK_EQ(n.r[0], 0xFFFFFFFFu);
    CB_CHECK_EQ(n.r[1], 0xFFFFFFFDu);
    CB_CHECK_EQ(n.r[3], 1u);

    Rig thumb;   // Thumb: the comment's first byte is the number: Sqrt
    thumb.load(" .thumb\n movs r0, #100\n swi 0x08\n bx lr\n");
    CB_CHECK_EQ(thumb.go(kCode | 1).r[0], 10u);

    Rig copy;   // CpuSet: copy 3 words (control: 3 units, word size bit 26)
    copy.load(" ldr r2, control\n swi 0x0B0000\n bx lr\ncontrol: .word 0x04000003\n");
    copy.bus.write(0x02000200, 4, 1);
    copy.bus.write(0x02000204, 4, 2);
    copy.bus.write(0x02000208, 4, 3);
    RunOptions regs;
    regs.a = {0x02000200, 0x02000300, 0, 0};
    const RunResult c = copy.go(kCode, regs);
    CB_CHECK_EQ(copy.bus.read(0x02000300, 4), 1u);
    CB_CHECK_EQ(copy.bus.read(0x02000308, 4), 3u);
    CB_CHECK_EQ(c.bios[0].function, 0x0Bu);

    Rig unknown;   // any other call: logged, r0 = 0
    unknown.load(" mov r0, #9\n swi 0x050000\n bx lr\n");
    const RunResult u = unknown.go();
    CB_CHECK_EQ(u.r[0], 0u);
    CB_CHECK_EQ(u.bios[0].function, 5u);
});

CB_TEST("emulate.arm.registers_are_kept_and_every_access_is_logged", [] {
    Rig rig;
    rig.load(" mov r0, #0x04000000\n mov r1, #0x1F\n strh r1, [r0]\n ldrh r2, [r0]\n ldr r3, [r0, #4]\n str r1, [r0, #0x200]\n bx lr\n");
    const RunResult r = rig.go();
    CB_CHECK_EQ(r.r[2], 0x1Fu);   // an I/O register holds what was written
    CB_CHECK_EQ(r.io.size(), std::size_t{4});
    CB_CHECK_EQ(r.io[0].text(), "w2");
    CB_CHECK_EQ(r.io[0].address, 0x04000000u);
    CB_CHECK_EQ(r.io[0].value, 0x1Fu);
    CB_CHECK_EQ(r.io[1].text(), "r2");
    CB_CHECK_EQ(r.io[2].text(), "r4");
    CB_CHECK_EQ(r.io[3].address, 0x04000200u);

    Rig mapped;   // a range taken over by a callback answers the program (the VCOUNT register)
    int reads = 0;
    mapped.bus.map(0x04000006, 0x04000007, [&](std::uint32_t, unsigned) -> std::uint32_t { return 150 + static_cast<std::uint32_t>(reads++); });
    mapped.load(" mov r0, #0x04000000\n ldrh r1, [r0, #6]\n ldrh r2, [r0, #6]\n bx lr\n");
    const RunResult m = mapped.go();
    CB_CHECK_EQ(m.r[1], 150u);
    CB_CHECK_EQ(m.r[2], 151u);
    CB_CHECK_EQ(m.io.size(), std::size_t{2});
});

CB_TEST("emulate.arm.faults_end_the_run_with_the_reason", [] {
    Rig bus_error;
    bus_error.load(" ldr r0, [r1]\n bx lr\n");
    RunOptions nowhere;
    nowhere.a = {0, 0x01000000, 0, 0};
    const RunResult b = bus_error.go(kCode, nowhere);
    CB_CHECK(b.end == End::Stopped);
    CB_CHECK_EQ(b.exit(), "stopped: bus error at 01000000");

    Rig undefined;
    undefined.bus.write(kCode, 4, 0xEE000000);   // a coprocessor instruction: the GBA has none
    const RunResult u = undefined.go();
    CB_CHECK_EQ(u.exit(), "stopped: no instruction at 02000000");

    Rig store_in_rom;   // a store to the ROM is dropped, as the hardware does
    store_in_rom.load(" str r1, [r0]\n bx lr\n");
    RunOptions rom;
    rom.a = {0x08000000, 7, 0, 0};
    CB_CHECK(store_in_rom.go(kCode, rom).end == End::Return);

    Rig fetch;   // a jump to an address past the map is a bus error on the fetch
    fetch.load(" mov r0, #0x01000000\n bx r0\n");
    CB_CHECK_EQ(fetch.go().exit(), "stopped: bus error at 01000000");

    Rig hat;   // ldm with ^ and no pc moves the user bank: not run
    hat.load(" ldmia r0, {r1, r2}^\n bx lr\n");
    RunOptions base;
    base.a = {0x02000100, 0, 0, 0};
    CB_CHECK_EQ(hat.go(kCode, base).exit(), "stopped: not run: ldm^");
});

CB_TEST("emulate.arm.the_memory_map", [] {
    Bus bus;
    // work RAM and internal RAM repeat every time their size does
    bus.write(0x02000000, 4, 0x01020304);
    CB_CHECK_EQ(bus.read(0x02040000, 4), 0x01020304u);
    CB_CHECK_EQ(bus.read(0x02FC0000, 4), 0x01020304u);
    bus.write(0x03000004, 4, 0xCAFEF00D);
    CB_CHECK_EQ(bus.read(0x03008004, 4), 0xCAFEF00Du);
    // the last 32 KB of the video RAM's 128 KB window repeat the object tiles
    bus.write(0x06010000, 2, 0x1234);
    CB_CHECK_EQ(bus.read(0x06018000, 2), 0x1234u);
    CB_CHECK_EQ(bus.read(0x06010000, 2), 0x1234u);
    // a byte written to the palette or the background part of video RAM writes the halfword; object memory ignores it
    bus.write(0x05000001, 1, 0xAB);
    CB_CHECK_EQ(bus.read(0x05000000, 2), 0xABABu);
    bus.write(0x06000003, 1, 0x7E);
    CB_CHECK_EQ(bus.read(0x06000002, 2), 0x7E7Eu);
    bus.write(0x06010001, 1, 0x55);
    CB_CHECK_EQ(bus.read(0x06010000, 2), 0x1234u);
    bus.write(0x07000000, 1, 0x66);
    CB_CHECK_EQ(bus.read(0x07000000, 2), 0u);
    // the ROM reads what the image holds and, past it, the address bus's value (the halfword's own address halved)
    const Bytes rom = {0x78, 0x56, 0x34, 0x12};
    bus.set_rom(rom);
    CB_CHECK_EQ(bus.read(0x08000000, 4), 0x12345678u);
    CB_CHECK_EQ(bus.read(0x0A000000, 2), 0x5678u);   // the second view of the same ROM
    CB_CHECK_EQ(bus.read(0x08000004, 4), 0x00030002u);   // (halfwords: (0x08000004 >> 1) & 0xFFFF = 2 low, (0x08000006 >> 1) & 0xFFFF = 3 high)
    // what answers an address
    CB_CHECK(bus.mapped(0x04000000));
    CB_CHECK(bus.mapped(0x040003FE));
    CB_CHECK(!bus.mapped(0x04000400));
    CB_CHECK(!bus.mapped(0x01000000));
    CB_CHECK(!bus.mapped(0x10000000));
    CB_CHECK(bus.mapped(0x0E000000));
    CB_CHECK_EQ(bus.read(0x0E000000, 1), 0xFFu);   // (the save memory is erased: all ones)
    // a write to the ROM is dropped
    bus.write(0x08000000, 4, 0);
    CB_CHECK_EQ(bus.read(0x08000000, 4), 0x12345678u);
});

CB_TEST("emulate.arm.the_record_is_the_judges_run", [] {
    Rig rig;
    rig.load(" mov r0, #0x04000000\n str r0, [r0]\n mov r4, #4\n mov r0, #1\n bx lr\n");
    Bus& bus = rig.bus;
    const cb::emulate::Run run = recorded(bus, kCode);
    CB_CHECK_EQ(run.exit.value_or(""), "return");
    CB_CHECK_EQ(std::get<std::uint64_t>(run.results.at("R0")), std::uint64_t{1});
    CB_CHECK_EQ(std::get<std::uint64_t>(run.results.at("R4")), std::uint64_t{4});
    CB_CHECK_EQ(std::get<std::uint64_t>(run.results.at("SP")), std::uint64_t{0x03007F00});
    CB_CHECK_EQ(run.io.size(), std::size_t{1});
    CB_CHECK_EQ(run.io[0].kind, "w4");
    CB_CHECK(!run.truncated);
});

}  // namespace
}  // namespace cb::emulate::arm

// ---- the Nintendo DS: the ARM946E-S and the DS's memory map --------------------------------------------------------------------------
namespace cb::emulate::arm {
namespace {

// A DS bus with `source` (ARMv5TE, or the ARM7's when `ds7`) assembled to main RAM at $02000000.
struct DsRig {
    Bus bus;
    explicit DsRig(System system = System::Nds9) : bus(system) {}
    void load(const std::string& source) {
        const std::string cpu = bus.system() == System::Nds9 ? ".cpu arm946e-s\n" : ".cpu arm7tdmi\n";
        const Bytes image = source::risc_source::gnu_arm(".org 0x02000000\n" + cpu + source);
        bus.load(kCode, image);
    }
    RunResult go(std::uint32_t entry = kCode, RunOptions options = {}) {
        options.sp = 0x027C3F00;
        return run(bus, entry, options);
    }
};

CB_TEST("emulate.arm.armv5te_clz_and_the_saturating_arithmetic", [] {
    DsRig clz;
    clz.load(" clz r1, r0\n clz r2, r3\n bx lr\n");
    RunOptions options;
    options.a = {0x100, 0, 0, 0};
    const RunResult a = clz.go(kCode, options);
    CB_CHECK(a.end == End::Return);
    CB_CHECK_EQ(a.r[1], 23u);   // 0x100 has 23 leading zeros
    CB_CHECK_EQ(a.r[2], 32u);   // zero has 32

    DsRig sat;
    sat.load(" qadd r2, r0, r1\n qsub r3, r1, r0\n qdadd r4, r0, r0\n mrs r5, cpsr\n bx lr\n");
    RunOptions big;
    big.a = {0x7FFFFFFF, 1, 0, 0};
    const RunResult b = sat.go(kCode, big);
    CB_CHECK_EQ(b.r[2], 0x7FFFFFFFu);        // 0x7FFFFFFF + 1 saturates
    CB_CHECK_EQ(b.r[3], 0x80000002u);        // 1 - 0x7FFFFFFF = -0x7FFFFFFE: in range
    CB_CHECK_EQ(b.r[4], 0x7FFFFFFFu);        // 0x7FFFFFFF + 2 * 0x7FFFFFFF saturates twice
    CB_CHECK((b.r[5] & 0x08000000u) != 0);   // the sticky Q flag
    DsRig none;
    none.load(" qadd r2, r0, r1\n mrs r5, cpsr\n bx lr\n");
    RunOptions small;
    small.a = {5, 6, 0, 0};
    const RunResult c = none.go(kCode, small);
    CB_CHECK_EQ(c.r[2], 11u);
    CB_CHECK((c.r[5] & 0x08000000u) == 0);
});

CB_TEST("emulate.arm.armv5te_the_16_bit_multiplies", [] {
    DsRig rig;
    // r1 = {top 3, bottom 2}, r2 = {top 5, bottom 4}, r0 = 100
    rig.load(" smulbb r3, r1, r2\n smultt r4, r1, r2\n smlabb r5, r1, r2, r0\n smlatb r6, r1, r2, r0\n smlabt r7, r1, r2, r0\n bx lr\n");
    RunOptions options;
    options.a = {100, 0x00030002, 0x00050004, 0};
    const RunResult r = rig.go(kCode, options);
    CB_CHECK_EQ(r.r[3], 8u);        // 2 * 4
    CB_CHECK_EQ(r.r[4], 15u);       // 3 * 5
    CB_CHECK_EQ(r.r[5], 108u);      // 2 * 4 + 100
    CB_CHECK_EQ(r.r[6], 112u);      // 3 * 4 + 100
    CB_CHECK_EQ(r.r[7], 110u);      // 2 * 5 + 100
    DsRig neg;
    neg.load(" smulbb r3, r1, r2\n smlawb r4, r1, r2, r0\n smulwt r5, r1, r2\n smlalbb r6, r7, r1, r2\n bx lr\n");
    RunOptions signed_options;
    signed_options.a = {7, 0x0010FFFE, 0x0003FFFD, 0};   // r1: top 0x10, bottom -2; r2: top 3, bottom -3
    signed_options.s[2] = 5;                              // r6 (low) and r7 (high) start as 5 and 0
    const RunResult n = neg.go(kCode, signed_options);
    CB_CHECK_EQ(n.r[3], 6u);                  // (-2) * (-3)
    CB_CHECK_EQ(n.r[4], static_cast<std::uint32_t>(((0x0010FFFELL * -3) >> 16) + 7));   // SMLAWB: (rm * bottom of rs) >> 16, plus r0
    CB_CHECK_EQ(n.r[5], static_cast<std::uint32_t>((0x0010FFFELL * 3) >> 16));          // SMULWT: the top of rs
    CB_CHECK_EQ(n.r[6], 11u);                 // 5 + 6 as a 64-bit value: the low word
    CB_CHECK_EQ(n.r[7], 0u);
});

CB_TEST("emulate.arm.armv5te_ldrd_and_strd", [] {
    DsRig rig;
    rig.load(" strd r0, r1, [r4, #8]\n ldrd r2, r3, [r4, #8]\n ldrd r6, r7, [r4, #8]!\n bx lr\n");
    RunOptions options;
    options.a = {0x11111111, 0x22222222, 0, 0};
    options.s[0] = 0x02100000;   // r4
    const RunResult r = rig.go(kCode, options);
    CB_CHECK(r.end == End::Return);
    CB_CHECK_EQ(r.r[2], 0x11111111u);
    CB_CHECK_EQ(r.r[3], 0x22222222u);
    CB_CHECK_EQ(r.r[4], 0x02100008u);   // written back
    CB_CHECK_EQ(rig.bus.read(0x02100008, 4), 0x11111111u);
    CB_CHECK_EQ(rig.bus.read(0x0210000C, 4), 0x22222222u);
});

CB_TEST("emulate.arm.armv5te_blx_changes_state_and_loads_into_the_pc_do_too", [] {
    // an ARM function calls a Thumb one through a register, and another through a label; a Thumb function pops its way back (ARMv5: `pop {pc}` interworks)
    DsRig arm;
    arm.load(" push {lr}\n ldr r1, ptr\n blx r1\n mov r2, r0\n blx thumb_two\n add r0, r0, r2\n pop {pc}\nptr: .word thumb_one + 1\n"
             " .thumb\nthumb_one: movs r0, #7\n bx lr\n .align 2\nthumb_two: push {lr}\n movs r0, #30\n pop {pc}\n");
    const RunResult r = arm.go();
    CB_CHECK(r.end == End::Return);
    CB_CHECK_EQ(r.r[0], 37u);   // 7 + 30
    // a Thumb function calls an ARM one with BLX label
    DsRig thumb;
    thumb.load(" .thumb\n push {lr}\n blx arm_fn\n pop {pc}\n .align 2\n .arm\narm_fn: mov r0, #9\n bx lr\n");
    const RunResult t = thumb.go(kCode | 1);
    CB_CHECK(t.end == End::Return);
    CB_CHECK_EQ(t.r[0], 9u);
    // an LDR into the pc takes the state from bit 0
    DsRig load;
    load.load(" push {lr}\n ldr r1, ptr\n adr_lr: mov lr, pc\n ldr pc, ptr\n pop {pc}\nptr: .word thumb_fn + 1\n .thumb\nthumb_fn: movs r0, #3\n bx lr\n");
    const RunResult l = load.go();
    CB_CHECK(l.end == End::Return);
    CB_CHECK_EQ(l.r[0], 3u);
    // on the ARM7TDMI the same `pop {pc}` does not switch state, and the bits of a BLX are no instruction
    DsRig seven(System::Nds7);
    seven.bus.load(kCode, Bytes{0x33, 0xFF, 0x2F, 0xE1});   // blx r3
    const RunResult s = seven.go();
    CB_CHECK(s.end == End::Stopped);
    CB_CHECK_EQ(s.exit(), std::string("stopped: no instruction at 02000000"));
});

CB_TEST("emulate.arm.coprocessor_15_the_tcms_and_the_logged_accesses", [] {
    DsRig rig;
    rig.load(" mrc p15, 0, r0, c0, c0, 0\n mcr p15, 0, r1, c9, c1, 0\n mcr p15, 0, r2, c7, c10, 4\n str r3, [r4]\n ldr r5, [r4]\n bx lr\n");
    RunOptions options;
    options.a = {0, 0x0B00000A, 0, 0x5A5A5A5A};   // the data TCM moved to $0B000000, 16 KB
    options.s[0] = 0x0B000010;                       // r4: an address that only the moved TCM answers
    CB_CHECK(!rig.bus.mapped(0x0B000010));
    const RunResult r = rig.go(kCode, options);
    CB_CHECK(r.end == End::Return);
    CB_CHECK_EQ(r.r[0], 0x41059461u);   // the ARM946E-S's ID code
    CB_CHECK_EQ(r.r[5], 0x5A5A5A5Au);
    CB_CHECK(rig.bus.mapped(0x0B000010));
    // the coprocessor accesses are in the log, at $FFF00000 + opcode1 << 12 | CRn << 8 | CRm << 4 | opcode2
    CB_CHECK_EQ(r.io.size(), std::size_t{3});
    CB_CHECK_EQ(r.io[0].text(), std::string("r4"));
    CB_CHECK_EQ(r.io[0].address, 0xFFF00000u);
    CB_CHECK_EQ(r.io[1].text(), std::string("w4"));
    CB_CHECK_EQ(r.io[1].address, 0xFFF00910u);
    CB_CHECK_EQ(r.io[1].value, 0x0B00000Au);
    CB_CHECK_EQ(r.io[2].address, 0xFFF007A4u);
});

CB_TEST("emulate.arm.the_ds_memory_map", [] {
    Bus nine(System::Nds9);
    // main RAM is 4 MB and repeats
    nine.write(0x02000100, 4, 0xA1B2C3D4);
    CB_CHECK_EQ(nine.read(0x02400100, 4), 0xA1B2C3D4u);
    CB_CHECK_EQ(nine.read(0x02C00100, 4), 0xA1B2C3D4u);
    // the data TCM is over the main RAM it lies in
    nine.write(0x027C0004, 4, 0x01020304);
    CB_CHECK_EQ(nine.read(0x027C0004, 4), 0x01020304u);
    CB_CHECK_EQ(nine.read(0x027C4004, 4), 0u);   // (past its 16 KB: the main RAM, which was not written)
    CB_CHECK_EQ(nine.dtcm()[4], std::uint8_t{4});
    // the instruction TCM repeats every 32 KB from address 0
    nine.write(0x00000010, 4, 0xCAFEF00D);
    CB_CHECK_EQ(nine.read(0x00008010, 4), 0xCAFEF00Du);
    CB_CHECK_EQ(nine.itcm()[0x10], std::uint8_t{0x0D});
    // the shared WRAM, 32 KB, repeats
    nine.write(0x03000008, 4, 0x55AA55AA);
    CB_CHECK_EQ(nine.read(0x03FF8008, 4), 0x55AA55AAu);
    // video memory: a byte written to the palette, background or object memory is dropped on the ARM9, a halfword is kept; the LCDC window takes bytes
    nine.write(0x05000001, 1, 0xAB);
    CB_CHECK_EQ(nine.read(0x05000000, 2), 0u);
    nine.write(0x05000002, 2, 0x1234);
    CB_CHECK_EQ(nine.read(0x05000002, 2), 0x1234u);
    CB_CHECK_EQ(nine.read(0x05000802, 2), 0x1234u);   // (2 KB, repeated)
    nine.write(0x06000003, 1, 0x7E);
    CB_CHECK_EQ(nine.read(0x06000002, 2), 0u);
    nine.write(0x06800003, 1, 0x7E);
    CB_CHECK_EQ(nine.read(0x06800003, 1), 0x7Eu);
    nine.write(0x06200000, 4, 0x99999999);              // engine B's background memory is a block of its own
    CB_CHECK_EQ(nine.read(0x06200000, 4), 0x99999999u);
    CB_CHECK_EQ(nine.read(0x06000000, 4), 0u);
    // the registers (the second engine's too) are kept and logged
    nine.write(0x04001000, 4, 0x10000);
    CB_CHECK_EQ(nine.read(0x04001000, 4), 0x10000u);
    CB_CHECK_EQ(nine.log().size(), std::size_t{2});
    CB_CHECK(nine.mapped(0x04001FFC));
    CB_CHECK(!nine.mapped(0x04002000));
    CB_CHECK(!nine.mapped(0x08000000));       // (the cartridge is not mapped)
    CB_CHECK(!nine.mapped(0xFFFF0000));       // (nor is the BIOS)
    CB_CHECK(!nine.mapped(0x10000000));
    // the ARM7: its own 64 KB of WRAM at $03800000 (repeating), the shared 32 KB below it, no video memory
    Bus seven(System::Nds7);
    seven.write(0x03800000, 4, 0x13572468);
    CB_CHECK_EQ(seven.read(0x03810000, 4), 0x13572468u);
    CB_CHECK_EQ(seven.wram7()[0], std::uint8_t{0x68});
    seven.write(0x037F8000, 4, 0x11223344);
    CB_CHECK_EQ(seven.read(0x03000000, 4), 0x11223344u);
    CB_CHECK_EQ(seven.iwram()[0], std::uint8_t{0x44});
    CB_CHECK(!seven.mapped(0x06000000));
    CB_CHECK(!seven.mapped(0x00000000));      // (the BIOS)
    CB_CHECK(seven.mapped(0x02000000));
    // load() places bytes without a log entry
    nine.load(0x02000200, Bytes{1, 2, 3, 4});
    CB_CHECK_EQ(nine.read(0x02000200, 4), 0x04030201u);
    CB_CHECK_EQ(nine.log().size(), std::size_t{2});
});

CB_TEST("emulate.arm.the_ds_bios_numbers_its_calls_its_own_way", [] {
    DsRig rig;
    rig.load(" .thumb\n swi 9\n bx lr\n");   // Div on the DS
    RunOptions options;
    options.a = {100, 7, 0, 0};
    const RunResult r = rig.go(kCode | 1, options);
    CB_CHECK_EQ(r.r[0], 14u);
    CB_CHECK_EQ(r.r[1], 2u);
    CB_CHECK_EQ(r.r[3], 14u);
    CB_CHECK_EQ(r.bios.size(), std::size_t{1});
    CB_CHECK_EQ(r.bios[0].function, 9u);
    DsRig root;
    root.load(" .thumb\n swi 13\n bx lr\n");   // Sqrt
    RunOptions square;
    square.a = {1000000, 0, 0, 0};
    CB_CHECK_EQ(root.go(kCode | 1, square).r[0], 1000u);
    Rig gba;   // on the Game Boy Advance call 9 is not a call it answers
    gba.load(" .thumb\n swi 9\n bx lr\n");
    RunOptions same;
    same.a = {100, 7, 0, 0};
    CB_CHECK_EQ(gba.go(kCode | 1, same).r[0], 0u);
});

}  // namespace
}  // namespace cb::emulate::arm
