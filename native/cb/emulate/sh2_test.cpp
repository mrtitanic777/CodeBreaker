// cb/emulate/sh2_test.cpp -- the SH-2 interpreter: expected values worked out by hand from the SH7604 manual (the carry and overflow of ADDC
// ADDV SUBC, the long and the 64-bit multiplies, MAC.W and its saturation, the 16-step unsigned division, the divide unit on the chip),
// delay slots (the slot runs, a slot that cannot is refused), addressing, faults, and the device log.
#include "cb/emulate/sh2.hpp"

#include <format>

#include "cb/core/testing.hpp"
#include "cb/source/sh2_test_programs.hpp"

namespace {

namespace sh2 = cb::emulate::sh2;
using cb::Bytes;

constexpr std::uint32_t kCode = 0x06000000;
constexpr std::uint32_t kData = 0x06002000;

// The program at $06000000 (its first line is the function) run with `options` over 64 KB of RAM there.
struct Machine {
    sh2::Bus bus;
    Machine() { bus.add_ram(kCode, 0x50000); }
    void load(std::string_view body) {
        const Bytes code = cb::source::sh2_test::assemble(std::string(".org 0x06000000\n") + std::string(body));
        bus.load(kCode, code);
    }
    sh2::RunResult run(const sh2::RunOptions& options = {}) { return sh2::run(bus, kCode, options); }
};

sh2::RunOptions args(std::uint32_t r4, std::uint32_t r5 = 0, std::uint32_t r6 = 0, std::uint32_t r7 = 0) {
    sh2::RunOptions o;
    o.a = {r4, r5, r6, r7};
    return o;
}

sh2::RunResult call(std::string_view body, const sh2::RunOptions& options = {}) {
    Machine m;
    m.load(body);
    return m.run(options);
}

}  // namespace

CB_TEST("emulate.sh2.carry_and_overflow", [] {
    // ADDC: 0xFFFFFFFF + 1 carries out
    sh2::RunResult r = call("clrt\naddc r5,r4\nmovt r0\nrts\nnop", args(0xFFFFFFFF, 1));
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(r.r[4], 0u);
    CB_CHECK_EQ(r.r[0], 1u);
    // ADDV: 0x7FFFFFFF + 1 overflows
    r = call("addv r5,r4\nmovt r0\nrts\nnop", args(0x7FFFFFFF, 1));
    CB_CHECK_EQ(r.r[4], 0x80000000u);
    CB_CHECK_EQ(r.r[0], 1u);
    // SUBC: 0 - 0 - T with T set borrows
    r = call("sett\nsubc r5,r4\nmovt r0\nrts\nnop", args(0, 0));
    CB_CHECK_EQ(r.r[4], 0xFFFFFFFFu);
    CB_CHECK_EQ(r.r[0], 1u);
    // SUBV: 0x80000000 - 1 overflows
    r = call("subv r5,r4\nmovt r0\nrts\nnop", args(0x80000000, 1));
    CB_CHECK_EQ(r.r[4], 0x7FFFFFFFu);
    CB_CHECK_EQ(r.r[0], 1u);
    // NEGC and NEG
    r = call("sett\nnegc r4,r5\nmovt r0\nneg r4,r6\nrts\nnop", args(5));
    CB_CHECK_EQ(r.r[5], 0xFFFFFFFAu);   // 0 - 5 - 1
    CB_CHECK_EQ(r.r[0], 1u);
    CB_CHECK_EQ(r.r[6], 0xFFFFFFFBu);
});

CB_TEST("emulate.sh2.compares_and_shifts", [] {
    // r4 = -1, r5 = 1: signed greater-than is false, unsigned higher is true
    sh2::RunResult r = call("cmp/gt r5,r4\nmovt r0\ncmp/hi r5,r4\nmovt r1\ncmp/ge r5,r5\nmovt r2\ncmp/pl r4\nmovt r3\nrts\nnop", args(0xFFFFFFFF, 1));
    CB_CHECK_EQ(r.r[0], 0u);
    CB_CHECK_EQ(r.r[1], 1u);
    CB_CHECK_EQ(r.r[2], 1u);
    CB_CHECK_EQ(r.r[3], 0u);
    // CMP/STR: one byte equal
    r = call("cmp/str r5,r4\nmovt r0\nrts\nnop", args(0x11223344, 0xAA22BBCC));
    CB_CHECK_EQ(r.r[0], 1u);
    // SHLL then ROTCR through T
    r = call("shll r4\nmovt r0\nrotcr r4\nmovt r1\nrts\nnop", args(0x80000001));
    CB_CHECK_EQ(r.r[0], 1u);                // the bit shifted out
    CB_CHECK_EQ(r.r[4], 0x80000001u);       // (2 >> 1) with T rotated into the top
    CB_CHECK_EQ(r.r[1], 0u);
    r = call("shlr16 r4\nshll8 r5\nshar r6\nrotl r7\nrts\nnop", args(0x12345678, 0x00112233, 0x80000000, 0x80000001));
    CB_CHECK_EQ(r.r[4], 0x1234u);
    CB_CHECK_EQ(r.r[5], 0x11223300u);
    CB_CHECK_EQ(r.r[6], 0xC0000000u);
    CB_CHECK_EQ(r.r[7], 3u);
    r = call("swap.b r4,r0\nswap.w r4,r1\nxtrct r5,r4\nextu.b r5,r2\nexts.b r5,r3\nrts\nnop", args(0x11223344, 0xAABBCCDD));
    CB_CHECK_EQ(r.r[0], 0x11224433u);
    CB_CHECK_EQ(r.r[1], 0x33441122u);
    CB_CHECK_EQ(r.r[4], 0xCCDD1122u);   // the middle of r5:r4
    CB_CHECK_EQ(r.r[2], 0xDDu);
    CB_CHECK_EQ(r.r[3], 0xFFFFFFDDu);
});

CB_TEST("emulate.sh2.multiplies", [] {
    sh2::RunResult r = call("dmulu.l r5,r4\nsts mach,r0\nsts macl,r1\nmuls.w r7,r6\nsts macl,r2\nmulu.w r7,r6\nsts macl,r3\nrts\nnop",
                            args(0x10000, 0x10000, 0xFFFF, 2));
    CB_CHECK_EQ(r.r[0], 1u);            // 0x10000 * 0x10000 = 0x1_0000_0000
    CB_CHECK_EQ(r.r[1], 0u);
    CB_CHECK_EQ(r.r[2], 0xFFFFFFFEu);   // -1 * 2
    CB_CHECK_EQ(r.r[3], 0x1FFFEu);      // 65535 * 2
    r = call("dmuls.l r5,r4\nsts mach,r0\nsts macl,r1\nmul.l r5,r4\nsts macl,r2\nrts\nnop", args(0xFFFFFFFE, 3));
    CB_CHECK_EQ(r.r[0], 0xFFFFFFFFu);   // -2 * 3 = -6: the 64-bit product's high word is all ones
    CB_CHECK_EQ(r.r[1], 0xFFFFFFFAu);
    CB_CHECK_EQ(r.r[2], 0xFFFFFFFAu);
});

CB_TEST("emulate.sh2.mac", [] {
    Machine m;
    m.load("clrmac\nmac.w @r4+,@r5+\nmac.w @r4+,@r5+\nsts macl,r0\nsts mach,r1\nrts\nnop");
    m.bus.load(kData, Bytes{0, 3, 0xFF, 0xFE});              // 3, -2
    m.bus.load(kData + 0x10, Bytes{0, 4, 0, 5});              // 4, 5
    sh2::RunResult r = m.run(args(kData, kData + 0x10));
    CB_CHECK_EQ(r.r[0], 2u);                                   // 3*4 + (-2)*5
    CB_CHECK_EQ(r.r[1], 0u);
    CB_CHECK_EQ(r.r[4], kData + 4);
    CB_CHECK_EQ(r.r[5], kData + 0x14);
    // with S set the sum saturates at 32 bits and MACH records it
    m.load("lds r7,macl\nldc r6,sr\nmac.w @r4+,@r5+\nsts macl,r0\nsts mach,r1\nrts\nnop");
    m.bus.load(kData, Bytes{0, 1});
    m.bus.load(kData + 0x10, Bytes{0, 1});
    r = m.run(args(kData, kData + 0x10, 0xF2, 0x7FFFFFFF));
    CB_CHECK_EQ(r.r[0], 0x7FFFFFFFu);
    CB_CHECK_EQ(r.r[1], 1u);
    // MAC.L: 64-bit product added to MACH:MACL
    Machine l;
    l.load("clrmac\nmac.l @r4+,@r5+\nsts macl,r0\nsts mach,r1\nrts\nnop");
    l.bus.load(kData, Bytes{0x00, 0x01, 0x00, 0x00});         // 0x10000
    l.bus.load(kData + 0x10, Bytes{0x00, 0x01, 0x00, 0x00});
    r = l.run(args(kData, kData + 0x10));
    CB_CHECK_EQ(r.r[0], 0u);
    CB_CHECK_EQ(r.r[1], 1u);
});

CB_TEST("emulate.sh2.unsigned_division_by_div1", [] {
    // the manual's 32 / 16 unsigned division: the divisor shifted into the high half, DIV0U, sixteen DIV1, ROTCL, EXTU.W
    std::string program = "shll16 r5\ndiv0u\n";
    for (int i = 0; i < 16; ++i) program += "div1 r5,r4\n";
    program += "rotcl r4\nextu.w r4,r0\nrts\nnop";
    CB_CHECK_EQ(call(program, args(1000, 7)).r[0], 142u);
    CB_CHECK_EQ(call(program, args(65535, 1)).r[0], 65535u);
    CB_CHECK_EQ(call(program, args(100, 100)).r[0], 1u);
    CB_CHECK_EQ(call(program, args(99, 100)).r[0], 0u);
    // DIV0S: the sign bits
    sh2::RunResult r = call("div0s r5,r4\nmovt r0\nrts\nnop", args(0x80000000, 1));
    CB_CHECK_EQ(r.r[0], 1u);
});

CB_TEST("emulate.sh2.divide_unit", [] {
    Machine m;
    m.load(R"(
        mov.l   K,r1
        mov     #7,r2
        mov.l   r2,@r1
        mov.w   K_n,r3
        mov.l   r3,@(4,r1)
        mov.l   @(4,r1),r0
        mov.l   @(16,r1),r4
        mov.l   @(20,r1),r5
        mov.l   @(0x30,r1),r6
        rts
        nop
        .align 2
K:      .long 0xFFFFFF00
K_n:    .word 1000
)");
    const sh2::RunResult r = m.run();
    CB_CHECK_EQ(r.r[0], 142u);   // 1000 / 7
    CB_CHECK_EQ(r.r[4], 6u);     // the remainder, in DVDNTH
    CB_CHECK_EQ(r.r[5], 142u);   // DVDNTL
    CB_CHECK_EQ(r.r[6], 6u);     // the registers are seen again at $FFFFFF20: DVDNTH at +$30
    // the accesses to the chip are logged in order
    CB_CHECK(r.io.size() >= 3);
    CB_CHECK_EQ(r.io[0].text(), std::string("w4"));
    CB_CHECK_EQ(r.io[0].address, 0xFFFFFF00u);
    CB_CHECK_EQ(r.io[0].value, 7u);
    // 64 / 32: DVDNTH:DVDNTL
    Machine w;
    w.load(R"(
        mov.l   K,r1
        mov     #10,r2
        mov.l   r2,@r1
        mov     #1,r3
        mov.l   r3,@(16,r1)
        mov     #0,r3
        mov.l   r3,@(20,r1)
        mov.l   @(20,r1),r0
        mov.l   @(16,r1),r4
        mov.l   @(8,r1),r5
        rts
        nop
        .align 2
K:      .long 0xFFFFFF00
)");
    const sh2::RunResult q = w.run();
    CB_CHECK_EQ(q.r[0], 0x19999999u);   // 2^32 / 10 = 429496729 remainder 6
    CB_CHECK_EQ(q.r[4], 6u);
    CB_CHECK_EQ(q.r[5], 0u);            // no overflow
    // a division by zero sets OVF in DVCR
    Machine z;
    z.load(R"(
        mov.l   K,r1
        mov     #0,r2
        mov.l   r2,@r1
        mov     #5,r3
        mov.l   r3,@(4,r1)
        mov.l   @(8,r1),r5
        mov.l   @(4,r1),r0
        rts
        nop
        .align 2
K:      .long 0xFFFFFF00
)");
    const sh2::RunResult o = z.run();
    CB_CHECK_EQ(o.r[5] & 1u, 1u);
    CB_CHECK_EQ(o.r[0], 0x7FFFFFFFu);
});

CB_TEST("emulate.sh2.delay_slots", [] {
    // the slot runs before the branch is taken; what it writes is seen by the target
    sh2::RunResult r = call("mov #1,r0\nbra target\nadd #1,r0\nmov #99,r0\ntarget: rts\nnop");
    CB_CHECK_EQ(r.r[0], 2u);
    // a delayed conditional branch runs its slot whether or not it is taken
    r = call("clrt\nmov #0,r0\nbt/s out\nadd #5,r0\nadd #1,r0\nout: rts\nnop");
    CB_CHECK_EQ(r.r[0], 6u);
    r = call("sett\nmov #0,r0\nbt/s out\nadd #5,r0\nadd #1,r0\nout: rts\nnop");
    CB_CHECK_EQ(r.r[0], 5u);
    // BT without a slot: the next instruction runs when it is not taken
    r = call("clrt\nmov #0,r0\nbt out\nadd #5,r0\nout: rts\nnop");
    CB_CHECK_EQ(r.r[0], 5u);
    // JSR links PR to the instruction after the slot; the slot runs before the call, and RTS returns through PR
    r = call(R"(
        sts.l   pr,@-r15
        mov.l   K,r1
        mov     #1,r4
        jsr     @r1
        add     #10,r4
        add     #100,r0
        lds.l   @r15+,pr
        rts
        nop
        .align 2
K:      .long   helper
helper: mov     r4,r0
        rts
        add     #1,r0
)");
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(r.r[0], 112u);   // helper saw r4 = 11 (the slot ran), returned 12; the caller added 100
    // BSR and BRAF
    r = call("sts.l pr,@-r15\nbsr f\nnop\nadd #1,r0\nlds.l @r15+,pr\nrts\nnop\nf: mov #20,r0\nrts\nnop");
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CB_CHECK_EQ(r.r[0], 21u);
    r = call("mov #2,r1\nbraf r1\nnop\nmov #1,r0\nrts\nmov #3,r0");
    CB_CHECK_EQ(r.r[0], 3u);   // the target is pc + 4 + r1: the `rts`
    // a branch in a delay slot cannot be
    r = call("bra x\nbra y\nx: nop\ny: rts\nnop");
    CB_CHECK_EQ(r.exit(), std::string("stopped: illegal slot at 06000002"));
    // a PC-relative load cannot be in one either
    r = call("bra x\nmov.l K,r1\nx: rts\nnop\n.align 2\nK: .long 1");
    CB_CHECK_EQ(r.exit(), std::string("stopped: illegal slot at 06000002"));
});

CB_TEST("emulate.sh2.addressing", [] {
    sh2::RunResult r = call(R"(
        mov.b   r4,@r6
        mov.b   @r6,r2
        mov.l   r4,@-r15
        mov.l   @r15+,r1
        mov     r4,r0
        mov.w   r0,@(8,r6)
        mov.w   @(8,r6),r0
        mov     r0,r3
        mov     #4,r0
        mov.l   r4,@(r0,r6)
        mov.l   @(r0,r6),r5
        rts
        nop
)", args(0x80, 0, kData));
    CB_CHECK_EQ(r.r[2], 0xFFFFFF80u);   // a byte load sign-extends
    CB_CHECK_EQ(r.r[1], 0x80u);
    CB_CHECK_EQ(r.r[15], 0x06040000u);  // the push and the pop balance
    CB_CHECK_EQ(r.r[3], 0x80u);
    CB_CHECK_EQ(r.r[5], 0x80u);
    // the GBR forms (the memory is read through R0)
    r = call(R"(
        ldc     r6,gbr
        mov     r4,r0
        mov.l   r0,@(8,gbr)
        mov     #0,r0
        mov.l   @(8,gbr),r0
        mov     r0,r1
        mov.b   r0,@(3,gbr)
        mov     #0,r0
        mov.b   @(3,gbr),r0
        mov     r0,r2
        or      #0x80,r0
        tst     #0x80,r0
        movt    r3
        rts
        nop
)", args(0x1234567F, 0, kData));
    CB_CHECK_EQ(r.r[1], 0x1234567Fu);
    CB_CHECK_EQ(r.r[2], 0x7Fu);
    CB_CHECK_EQ(r.r[3], 0u);   // (0x7F | 0x80) & 0x80 is not zero
    // constants: a longword, a word that is sign-extended, MOVA
    r = call("mov.l K,r1\nmov.w W,r2\nmova D,r0\nrts\nnop\n.align 2\nK: .long 0xDEADBEEF\nW: .word 0xFF80\n.align 2\nD: .long 0");
    CB_CHECK_EQ(r.r[1], 0xDEADBEEFu);
    CB_CHECK_EQ(r.r[2], 0xFFFFFF80u);
    CB_CHECK_EQ(r.r[0], 0x06000000u + 0x14);
    // the same RAM through the cached and uncached views
    r = call("mov.l K,r1\nmov.l r4,@r1\nmov.l K2,r2\nmov.l @r2,r0\nrts\nnop\n.align 2\nK: .long 0x26002000\nK2: .long 0x06002000", args(0xA5A55A5A));
    CB_CHECK_EQ(r.r[0], 0xA5A55A5Au);
});

CB_TEST("emulate.sh2.test_and_set_and_bytes_on_gbr", [] {
    Machine m;
    m.load("tas.b @r4\nmovt r0\ntas.b @r4\nmovt r1\nmov.b @r4,r2\nrts\nnop");
    m.bus.load(kData, Bytes{0});
    const sh2::RunResult r = m.run(args(kData));
    CB_CHECK_EQ(r.r[0], 1u);   // the byte was zero
    CB_CHECK_EQ(r.r[1], 0u);   // and has bit 7 set now
    CB_CHECK_EQ(r.r[2], 0xFFFFFF80u);
});

CB_TEST("emulate.sh2.faults_and_limits", [] {
    // an unaligned longword, an address nothing answers, zero words, TRAPA, SLEEP
    CB_CHECK_EQ(call("mov.l @r4,r0\nrts\nnop", args(kData + 1)).exit(), std::string("stopped: address error at 06002001"));
    CB_CHECK_EQ(call("mov.w @r4,r0\nrts\nnop", args(kData + 1)).exit(), std::string("stopped: address error at 06002001"));
    CB_CHECK_EQ(call("mov.l @r4,r0\nrts\nnop", args(0x12345678)).exit(), std::string("stopped: bus error at 12345678"));
    CB_CHECK_EQ(call("nop\n.word 0\nnop").exit(), std::string("stopped: no instruction at 06000002"));
    CB_CHECK_EQ(call("trapa #6\nrts\nnop").exit(), std::string("stopped: trapa 6"));
    CB_CHECK_EQ(call("sleep\nrts\nnop").exit(), std::string("stopped: sleep"));
    sh2::RunOptions few;
    few.max_steps = 50;
    const sh2::RunResult spin = call("here: bra here\nnop", few);
    CB_CHECK_EQ(spin.exit(), std::string("limit"));
    CB_CHECK(spin.steps >= 50);
    // a function that is entered where there is no RAM
    Machine m;
    m.load("rts\nnop");
    CB_CHECK_EQ(sh2::run(m.bus, 0x07000000).exit(), std::string("stopped: bus error at 07000000"));
    // the run starts as a call does: PR is the return address, SP and the arguments are set
    sh2::RunOptions o = args(11, 22, 33, 44);
    o.s[0] = 55;
    o.sp = 0x06008000;
    const sh2::RunResult r = call("sts pr,r0\nmov r15,r1\nrts\nnop", o);
    CB_CHECK_EQ(r.r[0], sh2::kReturnAddress);
    CB_CHECK_EQ(r.r[1], 0x06008000u);
    CB_CHECK_EQ(r.r[8], 55u);
    CB_CHECK_EQ(r.r[7], 44u);
});

CB_TEST("emulate.sh2.devices_are_logged", [] {
    Machine m;
    m.bus.map(0x05FE0000, 0x05FEFFFF,
              [](std::uint32_t a, unsigned size) -> std::uint32_t { return a == 0x05FE00A4 && size == 4 ? 0x12345678u : 0u; },
              [](std::uint32_t, unsigned, std::uint32_t) {});
    m.load("mov.l K,r1\nmov.l @r1,r0\nmov.l r0,@(4,r1)\nmov.w r0,@(8,r1)\nmov.b @r1,r2\nrts\nnop\n.align 2\nK: .long 0x25FE00A4");
    const sh2::RunResult r = m.run();
    CB_CHECK_EQ(r.r[0], 0x12345678u);
    CB_CHECK_EQ(r.io.size(), 4u);
    CB_CHECK_EQ(r.io[0].text(), std::string("r4"));
    CB_CHECK_EQ(r.io[0].address, 0x05FE00A4u);   // (physical: the cache view is not part of the address)
    CB_CHECK_EQ(r.io[0].value, 0x12345678u);
    CB_CHECK_EQ(r.io[1].text(), std::string("w4"));
    CB_CHECK_EQ(r.io[1].address, 0x05FE00A8u);
    CB_CHECK_EQ(r.io[2].text(), std::string("w2"));
    CB_CHECK_EQ(r.io[2].value, 0x5678u);
    CB_CHECK_EQ(r.io[3].text(), std::string("r1"));
});

CB_TEST("emulate.sh2.bus", [] {
    sh2::Bus bus;
    bus.add_ram(0x06000000, 0x100);
    CB_CHECK_THROWS_AS(cb::UsageError, bus.add_ram(0x060000F0, 0x100));
    CB_CHECK_THROWS_AS(cb::UsageError, bus.load(0x060000F8, Bytes(16, 1)));
    bus.write(0x06000010, 4, 0x01020304);
    CB_CHECK_EQ(bus.read(0x06000010, 1), 1u);
    CB_CHECK_EQ(bus.read(0x06000012, 2), 0x0304u);
    CB_CHECK_EQ(bus.read(0x26000010, 4), 0x01020304u);
    CB_CHECK(bus.mapped(0x06000000));
    CB_CHECK(!bus.mapped(0x06000100));
    CB_CHECK(bus.mapped(0xFFFFFE00));
    CB_CHECK(bus.log().empty());   // RAM is not logged
    CB_CHECK_EQ(bus.read(0x07000000, 4), 0u);
    CB_CHECK_EQ(bus.region(0x06000000).size(), 0x100u);
    CB_CHECK_EQ(bus.region_base(0x06000050), 0x06000000u);
});
