// cb/emulate/m68k_test.cpp -- the 68000 interpreter: the bus, the flags of the arithmetic, logic and shift groups, multiply
// and divide, BCD, the addressing modes, the stack instructions, branches and conditions, the exceptions that end a run,
// running a function, devices, and the run record.
//
// Every expected value is worked out by hand from the M68000 Programmer's Reference Manual (not recorded from this code),
// by writing the program in Motorola syntax and assembling it with cb::decompile::m68k. The old harness
// (state/work/typed-port/emulate.m68k: 128,383 cases made by the Python module) agrees on every one of its single-step
// and run cases that the CPU defines.
//
// DIVERGES from the Python module:
//  - A device read's value is cut to the access's size (the Python passed a negative or wider number on, and the flags
//    then depended on bits the access never had).
//  - A run's `io` is what that run logged (the Python gave the bus's whole log, carried over a reused bus).
//  - Registers are 32 bits and SR 16 (Python's ints could hold more); the entry address is 32 bits.
//  - An instruction the interpreter has no operand semantics for (an operand kind the decoder never gives that
//    instruction) is an InternalError, not a TypeError; hand-made instructions are not a thing a Cpu is asked to run.
//  - A Bus over memory takes any size; an image that does not fit is a UsageError.
#include "cb/emulate/m68k.hpp"

#include <array>
#include <format>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/decompile/m68k.hpp"

namespace m = cb::emulate::m68k;
namespace d = cb::decompile::m68k;
using cb::Bytes;

#define CHECK_U(actual, expected) CB_CHECK_EQ(static_cast<std::uint64_t>(actual), static_cast<std::uint64_t>(expected))

namespace {

constexpr std::uint32_t kCode = 0x1000;
constexpr std::uint32_t kStack = 0x8000;
constexpr std::uint16_t kX = 0x10, kN = 0x08, kZ = 0x04, kV = 0x02, kC = 0x01;

// A program written line by line, assembled where it will run.
struct Asm {
    Bytes bytes;
    std::uint32_t base;
    explicit Asm(std::uint32_t at = kCode) : base(at) {}
    std::uint32_t here() const { return base + static_cast<std::uint32_t>(bytes.size()); }
    Asm& line(const std::string& text) {
        const Bytes b = d::assemble(text, here());
        bytes.insert(bytes.end(), b.begin(), b.end());
        return *this;
    }
    Asm& words(std::initializer_list<std::uint16_t> ws) {
        for (std::uint16_t w : ws) {
            bytes.push_back(static_cast<std::uint8_t>(w >> 8));
            bytes.push_back(static_cast<std::uint8_t>(w));
        }
        return *this;
    }
    void load(m::Bus& bus) const { std::copy(bytes.begin(), bytes.end(), bus.memory().begin() + base); }
};

// A small machine (64 KB; the stack at 0x8000) running `lines` then RTS.
struct Machine {
    m::Bus bus{0x10000};
    m::RunOptions options;
    Machine() { options.sp = kStack; }
    m::RunResult run(const std::vector<std::string>& lines) {
        Asm a;
        for (const std::string& l : lines) a.line(l);
        a.line("RTS");
        a.load(bus);
        return m::run(bus, kCode, options);
    }
};

std::uint16_t ccr(const m::RunResult& r) { return static_cast<std::uint16_t>(r.sr & 0x1F); }

}  // namespace

// ---- the bus ----------------------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.bus_is_big_endian_and_24_bit", [] {
    m::Bus bus(0x10000);
    bus.write(0x100, 4, 0x11223344);
    CHECK_U(bus.memory()[0x100], 0x11);
    CHECK_U(bus.memory()[0x103], 0x44);
    CHECK_U(bus.read(0x100, 4), 0x11223344);
    CHECK_U(bus.read(0x101, 2), 0x2233);
    CHECK_U(bus.read(0x102, 1), 0x33);
    bus.write(0x200, 2, 0xAABBCCDD);   // only the low two bytes of the value are stored
    CHECK_U(bus.read(0x200, 2), 0xCCDD);
    CHECK_U(bus.read(0x202, 2), 0);
    // addresses are 24 bits wide: the high byte is ignored
    CHECK_U(bus.read(0xFF000100, 4), 0x11223344);
});

CB_TEST("emulate.m68k.bus_beyond_its_memory_reads_zero_and_drops_writes", [] {
    m::Bus bus(0x1000);
    bus.write(0xFFE, 4, 0xDEADBEEF);   // straddles the end: dropped whole
    CHECK_U(bus.read(0xFFE, 2), 0);
    CHECK_U(bus.read(0xFFC, 4), 0);
    bus.write(0x800000, 1, 7);
    CHECK_U(bus.read(0x800000, 1), 0);
    CHECK_U(bus.read(0xFFF, 1), 0);
    bus.write(0xFFE, 2, 0x1234);
    CHECK_U(bus.read(0xFFE, 2), 0x1234);
});

CB_TEST("emulate.m68k.bus_ranges_go_to_callbacks_and_are_logged", [] {
    m::Bus bus(0x10000);
    std::vector<std::uint32_t> written;
    bus.map(0x4000, 0x40FF, [](std::uint32_t a, unsigned n) { return a * 0x10001u + n; },
            [&](std::uint32_t, unsigned, std::uint32_t v) { written.push_back(v); });
    bus.map(0x5000, 0x50FF);   // no callbacks: reads 0, ignores writes
    CHECK_U(bus.read(0x4002, 2), 0x4004);   // the callback answered 0x40024004: the low 16 bits
    CHECK_U(bus.read(0x4004, 1), 0x05);     // 0x40044005: the low byte
    bus.write(0x4010, 4, 0x01020304);
    bus.write(0x5000, 2, 0x55);
    CHECK_U(bus.read(0x5000, 2), 0);
    CB_CHECK_EQ(written.size(), std::size_t{1});
    CHECK_U(written[0], 0x01020304);
    const auto& log = bus.log();
    CB_CHECK_EQ(log.size(), std::size_t{5});
    CB_CHECK_EQ(log[0].text(), std::string("r2"));
    CHECK_U(log[0].address, 0x4002);
    CB_CHECK_EQ(log[1].text(), std::string("r1"));
    CB_CHECK_EQ(log[2].text(), std::string("w4"));
    CHECK_U(log[2].value, 0x01020304);
    CB_CHECK_EQ(log[3].text(), std::string("w2"));
    CHECK_U(log[3].address, 0x5000);
    CB_CHECK_EQ(log[4].text(), std::string("r2"));
    // the memory under a mapped range is not touched
    CHECK_U(bus.memory()[0x4010], 0);
});

CB_TEST("emulate.m68k.bus_over_uses_the_callers_memory", [] {
    cb::Bytes ram(0x100, 0);
    m::Bus bus = m::Bus::over(ram);
    bus.write(0x10, 2, 0xABCD);
    CHECK_U(ram[0x10], 0xAB);
    CHECK_U(ram[0x11], 0xCD);
    const cb::Bytes image = {1, 2, 3};
    m::Bus copy(image, 0x100);
    CHECK_U(copy.read(0, 2), 0x0102);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)m::Bus(image, 2));
});

// ---- arithmetic -------------------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.add_sets_all_five_flags", [] {
    {   // 0x7F + 1 in a byte overflows into the sign: N and V, no carry
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.B #$7F,D0", "ADDQ.B #1,D0"});
        CHECK_U(r.d[0], 0x80);
        CHECK_U(ccr(r), kN | kV);
    }
    {   // 0xFF + 1: zero with carry and extend
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.B #$FF,D0", "ADDQ.B #1,D0"});
        CHECK_U(r.d[0], 0x00);
        CHECK_U(ccr(r), kX | kZ | kC);
    }
    {   // 0xFFFFFFFF + 0xFFFFFFFF
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #-1,D1", "ADD.L D1,D1"});
        CHECK_U(r.d[1], 0xFFFFFFFE);
        CHECK_U(ccr(r), kX | kN | kC);
    }
    {   // only the low byte of a register is touched by a byte add
        Machine mc;
        mc.options.d = std::array<std::uint32_t, 8>{0x12345601, 0, 0, 0, 0, 0, 0, 0};
        const m::RunResult r = mc.run({"ADDQ.B #2,D0"});
        CHECK_U(r.d[0], 0x12345603);
    }
});

CB_TEST("emulate.m68k.sub_and_cmp_flags", [] {
    {   // 1 - 2 borrows
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #1,D0", "SUBQ.B #2,D0"});
        CHECK_U(r.d[0] & 0xFF, 0xFF);
        CHECK_U(ccr(r), kX | kN | kC);
    }
    {   // 0x80 - 1 overflows
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.B #$80,D0", "SUBQ.B #1,D0"});
        CHECK_U(r.d[0], 0x7F);
        CHECK_U(ccr(r), kV);
    }
    {   // CMP leaves the register and the extend flag alone
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #5,D0", "MOVE.W #$10,CCR", "CMPI.L #5,D0"});
        CHECK_U(r.d[0], 5);
        CHECK_U(ccr(r), kX | kZ);
    }
    {   // NEG: 0 - 1
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #1,D0", "NEG.B D0"});
        CHECK_U(r.d[0] & 0xFF, 0xFF);
        CHECK_U(ccr(r), kX | kN | kC);
    }
    {   // CMPA compares all 32 bits (a word source is sign-extended) and sets no X
        Machine mc;
        const m::RunResult r = mc.run({"MOVEA.L #$FFFF8000,A0", "CMPA.W #$8000,A0"});
        CHECK_U(ccr(r), kZ);
    }
});

CB_TEST("emulate.m68k.addx_subx_keep_z_unless_the_result_is_nonzero", [] {
    {   // 1 + 1 + X
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #1,D0", "MOVEQ #1,D1", "MOVE.W #$10,CCR", "ADDX.B D0,D1"});
        CHECK_U(r.d[1] & 0xFF, 3);
        CHECK_U(ccr(r), 0);
    }
    {   // a zero result keeps Z as it was: set
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #0,D0", "MOVEQ #0,D1", "MOVE.W #$04,CCR", "ADDX.B D0,D1"});
        CHECK_U(ccr(r), kZ);
    }
    {   // ... and clear
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #0,D0", "MOVEQ #0,D1", "MOVE.W #$00,CCR", "ADDX.B D0,D1"});
        CHECK_U(ccr(r), 0);
    }
    {   // SUBX: 5 - 2 - 1
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #2,D0", "MOVEQ #5,D1", "MOVE.W #$10,CCR", "SUBX.B D0,D1"});
        CHECK_U(r.d[1] & 0xFF, 2);
        CHECK_U(ccr(r), 0);
    }
    {   // NEGX: 0 - 0 - X
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #0,D0", "MOVE.W #$14,CCR", "NEGX.B D0"});
        CHECK_U(r.d[0] & 0xFF, 0xFF);
        CHECK_U(ccr(r), kX | kN | kC);
    }
});

CB_TEST("emulate.m68k.logic_clears_v_and_c_and_leaves_x", [] {
    Machine mc;
    const m::RunResult r = mc.run({"MOVE.W #$1F,CCR", "MOVE.L #$F0F0F0F0,D0", "ANDI.L #$80000000,D0"});
    CHECK_U(r.d[0], 0x80000000);
    CHECK_U(ccr(r), kX | kN);
    Machine m2;
    const m::RunResult r2 = m2.run({"MOVEQ #$0F,D0", "EORI.B #$0F,D0"});
    CHECK_U(r2.d[0], 0);
    CHECK_U(ccr(r2), kZ);
    Machine m3;
    const m::RunResult r3 = m3.run({"MOVEQ #$0F,D0", "ORI.B #$F0,D0", "NOT.B D0"});
    CHECK_U(r3.d[0] & 0xFF, 0);
    CHECK_U(ccr(r3), kZ);
});

CB_TEST("emulate.m68k.moves_set_n_and_z_and_clear_v_c", [] {
    Machine mc;
    const m::RunResult r = mc.run({"MOVE.W #$17,CCR", "MOVE.W #$8000,D0"});
    CHECK_U(r.d[0], 0x8000);
    CHECK_U(ccr(r), kX | kN);
    Machine m2;
    const m::RunResult r2 = m2.run({"MOVEQ #-1,D0", "CLR.L D0"});
    CHECK_U(r2.d[0], 0);
    CHECK_U(ccr(r2), kZ);
    // MOVEA does not touch the flags and sign-extends a word
    Machine m3;
    const m::RunResult r3 = m3.run({"MOVE.W #$1F,CCR", "MOVEA.W #$FFFE,A1"});
    CHECK_U(r3.a[1], 0xFFFFFFFE);
    CHECK_U(ccr(r3), 0x1F);
});

CB_TEST("emulate.m68k.ext_swap_exg_tst", [] {
    Machine mc;
    const m::RunResult r = mc.run({"MOVE.W #$FF80,D0", "EXT.L D0"});
    CHECK_U(r.d[0], 0xFFFFFF80);
    CHECK_U(ccr(r), kN);
    Machine m2;
    const m::RunResult r2 = m2.run({"MOVE.B #$80,D1", "EXT.W D1"});
    CHECK_U(r2.d[1], 0xFF80);
    Machine m3;
    m3.options.d = std::array<std::uint32_t, 8>{0, 0, 0x12345678, 0, 0, 0, 0, 0};
    const m::RunResult r3 = m3.run({"SWAP D2"});
    CHECK_U(r3.d[2], 0x56781234);
    Machine m4;
    m4.options.d = std::array<std::uint32_t, 8>{1, 2, 0, 0, 0, 0, 0, 0};
    m4.options.a = std::array<std::uint32_t, 8>{0, 0, 0, 0, 0, 0, 0, 0};
    const m::RunResult r4 = m4.run({"EXG D0,D1", "EXG D0,A3", "TST.L D1"});
    CHECK_U(r4.d[0], 0);
    CHECK_U(r4.d[1], 1);
    CHECK_U(r4.a[3], 2);
    CHECK_U(ccr(r4), 0);
});

// ---- shifts and rotates ---------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.shifts", [] {
    struct Case {
        std::vector<std::string> lines;
        std::uint32_t d0, flags;
    };
    const std::vector<Case> cases = {
        {{"MOVE.B #$81,D0", "LSL.B #1,D0"}, 0x02, kX | kC},
        {{"MOVE.B #$81,D0", "ASR.B #1,D0"}, 0xC0, kX | kN | kC},   // the sign is kept
        {{"MOVE.B #$40,D0", "ASL.B #1,D0"}, 0x80, kN | kV},        // the sign changed: V
        {{"MOVE.B #$81,D0", "LSR.B #1,D0"}, 0x40, kX | kC},
        {{"MOVE.B #$81,D0", "ROL.B #1,D0"}, 0x03, kC},             // rotate leaves X alone
        {{"MOVE.B #$81,D0", "ROR.B #1,D0"}, 0xC0, kN | kC},
        {{"MOVE.B #$80,D0", "MOVE.W #$10,CCR", "ROXL.B #1,D0"}, 0x01, kX | kC},   // X rotates in
        {{"MOVE.B #$01,D0", "MOVE.W #$10,CCR", "ROXR.B #1,D0"}, 0x80, kX | kN | kC},
        {{"MOVEQ #0,D1", "MOVE.B #$80,D0", "MOVE.W #$10,CCR", "LSL.B D1,D0"}, 0x80, kX | kN},   // a count of 0 leaves X
        {{"MOVEQ #0,D1", "MOVE.B #$80,D0", "MOVE.W #$10,CCR", "ROXL.B D1,D0"}, 0x80, kX | kN | kC},   // and ROX sets C from X
        {{"MOVEQ #9,D1", "MOVE.B #$FF,D0", "LSL.B D1,D0"}, 0x00, kZ},   // eight bits out, then a zero: C and X end clear
        {{"MOVEQ #3,D1", "MOVE.L #$80000000,D0", "ASR.L D1,D0"}, 0xF0000000, kN},
    };
    for (const Case& c : cases) {
        Machine mc;
        const m::RunResult r = mc.run(c.lines);
        CHECK_U(c.d0 > 0xFF ? r.d[0] : (r.d[0] & 0xFF), c.d0);
        CHECK_U(ccr(r), c.flags);
    }
});

CB_TEST("emulate.m68k.memory_shift_is_one_bit_of_a_word", [] {
    Machine mc;
    mc.bus.write(0x2000, 2, 0x0003);
    const m::RunResult r = mc.run({"LEA $2000,A0", "LSR.W (A0)"});
    CHECK_U(mc.bus.read(0x2000, 2), 0x0001);
    CHECK_U(ccr(r), kX | kC);
    CHECK_U(r.a[0], 0x2000);
});

// ---- multiply, divide, BCD ------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.multiply", [] {
    Machine mc;
    const m::RunResult r = mc.run({"MOVE.W #$FFFF,D0", "MOVE.W #2,D1", "MULU.W D1,D0"});
    CHECK_U(r.d[0], 0x0001FFFE);
    CHECK_U(ccr(r), 0);
    Machine m2;
    const m::RunResult r2 = m2.run({"MOVE.W #$FFFE,D0", "MOVE.W #3,D1", "MULS.W D1,D0"});
    CHECK_U(r2.d[0], 0xFFFFFFFA);   // -2 * 3
    CHECK_U(ccr(r2), kN);
});

CB_TEST("emulate.m68k.divide", [] {
    {   // 100000 / 7 = 14285 remainder 5
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.L #100000,D0", "MOVE.W #7,D1", "DIVU.W D1,D0"});
        CHECK_U(r.d[0], 0x000537CD);
        CHECK_U(ccr(r), 0);
    }
    {   // the quotient does not fit 16 bits: V, the destination is unchanged
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.L #$00100000,D0", "MOVE.W #2,D1", "DIVU.W D1,D0"});
        CHECK_U(r.d[0], 0x00100000);
        CHECK_U(ccr(r) & (kV | kC), kV);
    }
    {   // -100000 / 7 = -14285 remainder -5 (the remainder takes the dividend's sign)
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.L #-100000,D0", "MOVE.W #7,D1", "DIVS.W D1,D0"});
        CHECK_U(r.d[0], 0xFFFBC833);
        CHECK_U(ccr(r), kN);
    }
    {   // -32768 / -1 = 32768 does not fit: V
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.L #-32768,D0", "MOVE.W #-1,D1", "DIVS.W D1,D0"});
        CHECK_U(r.d[0], 0xFFFF8000);
        CHECK_U(ccr(r) & kV, kV);
    }
    {   // by zero: the run stops
        Machine mc;
        const m::RunResult r = mc.run({"MOVEQ #5,D0", "MOVEQ #0,D1", "DIVU.W D1,D0"});
        CB_CHECK(r.end == m::End::Stopped);
        CB_CHECK_EQ(r.exit(), std::string("stopped: division by zero"));
        CHECK_U(r.d[0], 5);
        CHECK_U(r.steps, 2);   // the faulting instruction is not counted
    }
});

CB_TEST("emulate.m68k.bcd", [] {
    {   // 19 + 28 = 47
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.B #$19,D0", "MOVE.B #$28,D1", "MOVE.W #0,CCR", "ABCD D0,D1"});
        CHECK_U(r.d[1] & 0xFF, 0x47);
        CHECK_U(ccr(r), 0);
    }
    {   // 99 + 01 = 00 carry out; Z keeps its value when the result is zero
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.B #$99,D1", "MOVE.B #$01,D0", "MOVE.W #4,CCR", "ABCD D0,D1"});
        CHECK_U(r.d[1] & 0xFF, 0x00);
        CHECK_U(ccr(r), kX | kZ | kC);
    }
    {   // 20 - 01 = 19
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.B #$20,D1", "MOVE.B #$01,D0", "MOVE.W #0,CCR", "SBCD D0,D1"});
        CHECK_U(r.d[1] & 0xFF, 0x19);
        CHECK_U(ccr(r), 0);
    }
    {   // NBCD: 0 - 01 = 99 with a borrow
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.B #$01,D0", "MOVE.W #0,CCR", "NBCD D0"});
        CHECK_U(r.d[0] & 0xFF, 0x99);
        CHECK_U(ccr(r), kX | kC);
    }
});

// ---- addressing, memory ---------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.addressing_modes", [] {
    Machine mc;
    mc.options.a = std::array<std::uint32_t, 8>{0x2000, 0, 0, 0, 0, 0, 0, 0};
    mc.options.d = std::array<std::uint32_t, 8>{0, 0x0001FFFF, 0, 0, 0, 0, 0, 0};
    const m::RunResult r = mc.run({"LEA $10(A0),A1", "LEA 2(A0,D1.W),A2", "LEA 2(A0,D1.L),A3", "LEA ($1234).W,A4", "LEA ($123456).L,A5"});
    CHECK_U(r.a[1], 0x2010);
    CHECK_U(r.a[2], 0x2001);    // D1.W is -1
    CHECK_U(r.a[3], 0x2000 + 2 + 0x1FFFF);
    CHECK_U(r.a[4], 0x1234);
    CHECK_U(r.a[5], 0x123456);
});

CB_TEST("emulate.m68k.post_increment_pre_decrement_and_the_byte_on_a7", [] {
    Machine mc;
    mc.options.a = std::array<std::uint32_t, 8>{0x2000, 0x3000, 0, 0, 0, 0, 0, 0};
    const m::RunResult r = mc.run({"MOVE.W #$1234,(A0)+", "MOVE.L #$AABBCCDD,-(A1)", "MOVE.B #$55,-(A7)", "ADDQ.L #2,A7"});
    CHECK_U(r.a[0], 0x2002);
    CHECK_U(mc.bus.read(0x2000, 2), 0x1234);
    CHECK_U(r.a[1], 0x2FFC);
    CHECK_U(mc.bus.read(0x2FFC, 4), 0xAABBCCDD);
    // a byte pushed through A7 moves it by two; the byte lies at the lower address
    CHECK_U(mc.bus.read(kStack - 4 - 2, 1), 0x55);
    CHECK_U(r.a[7], kStack);
});

CB_TEST("emulate.m68k.movem_orders_registers_and_sign_extends_words", [] {
    Machine mc;
    mc.options.d = std::array<std::uint32_t, 8>{0x11111111, 0x22222222, 0x33333333, 0, 0, 0, 0, 0};
    mc.options.a = std::array<std::uint32_t, 8>{0x44444444, 0, 0, 0, 0, 0, 0, 0};
    const m::RunResult r = mc.run({"MOVEM.L D0-D2/A0,-(A7)", "MOVEQ #0,D0", "MOVEQ #0,D1", "MOVEQ #0,D2", "MOVEA.L D0,A0",
                                   "MOVEM.L (A7)+,D0-D2/A0"});
    // memory, lowest first: D0 D1 D2 A0
    const std::uint32_t at = kStack - 4 - 16;
    CHECK_U(mc.bus.read(at, 4), 0x11111111);
    CHECK_U(mc.bus.read(at + 4, 4), 0x22222222);
    CHECK_U(mc.bus.read(at + 8, 4), 0x33333333);
    CHECK_U(mc.bus.read(at + 12, 4), 0x44444444);
    CHECK_U(r.d[0], 0x11111111);
    CHECK_U(r.d[2], 0x33333333);
    CHECK_U(r.a[0], 0x44444444);
    // a word load fills the whole register
    Machine m2;
    m2.bus.write(0x2000, 2, 0xFFFE);
    m2.bus.write(0x2002, 2, 0x0002);
    m2.options.a = std::array<std::uint32_t, 8>{0x2000, 0, 0, 0, 0, 0, 0, 0};
    const m::RunResult r2 = m2.run({"MOVEM.W (A0)+,D0/D1"});
    CHECK_U(r2.d[0], 0xFFFFFFFE);
    CHECK_U(r2.d[1], 2);
    CHECK_U(r2.a[0], 0x2004);
});

CB_TEST("emulate.m68k.movep_moves_alternate_bytes", [] {
    Machine mc;
    mc.options.d = std::array<std::uint32_t, 8>{0x1234, 0, 0, 0, 0, 0, 0, 0};
    mc.options.a = std::array<std::uint32_t, 8>{0x2000, 0, 0, 0, 0, 0, 0, 0};
    const m::RunResult r = mc.run({"MOVEP.W D0,0(A0)", "MOVEP.W 0(A0),D1"});
    CHECK_U(mc.bus.read(0x2000, 1), 0x12);
    CHECK_U(mc.bus.read(0x2001, 1), 0x00);
    CHECK_U(mc.bus.read(0x2002, 1), 0x34);
    CHECK_U(r.d[1], 0x1234);
});

CB_TEST("emulate.m68k.pea_link_unlk_and_the_stack", [] {
    Machine mc;
    mc.options.a = std::array<std::uint32_t, 8>{0, 0, 0, 0, 0, 0, 0x7777, 0};
    const m::RunResult r = mc.run({"LINK A6,#-8", "MOVEQ #9,D0", "PEA $1234", "MOVE.L (A7)+,D1", "UNLK A6"});
    CHECK_U(r.d[0], 9);
    CHECK_U(r.d[1], 0x1234);
    CHECK_U(r.a[6], 0x7777);   // UNLK gave A6 back
    CHECK_U(r.a[7], kStack);   // and the RTS popped the return address
});

CB_TEST("emulate.m68k.tas_and_bit_instructions", [] {
    Machine mc;
    const m::RunResult r = mc.run({"MOVEQ #0,D0", "TAS D0"});
    CHECK_U(r.d[0] & 0xFF, 0x80);
    CHECK_U(ccr(r), kZ);
    {   // BSET: Z reports the bit's old value
        Machine m2;
        const m::RunResult r2 = m2.run({"MOVEQ #0,D0", "BSET #3,D0"});
        CHECK_U(r2.d[0], 8);
        CHECK_U(ccr(r2) & kZ, kZ);
        Machine m3;
        const m::RunResult r3 = m3.run({"MOVEQ #8,D0", "BTST #3,D0"});
        CHECK_U(r3.d[0], 8);
        CHECK_U(ccr(r3) & kZ, 0);
        Machine m4;
        const m::RunResult r4 = m4.run({"MOVEQ #8,D0", "BCHG #3,D0"});
        CHECK_U(r4.d[0], 0);
        CHECK_U(ccr(r4) & kZ, 0);
        Machine m5;
        const m::RunResult r5 = m5.run({"MOVEQ #8,D0", "BCLR #4,D0"});
        CHECK_U(r5.d[0], 8);
        CHECK_U(ccr(r5) & kZ, kZ);
    }
    {   // a register takes the bit number mod 32, memory mod 8
        Machine m2;
        m2.options.a = std::array<std::uint32_t, 8>{0x2000, 0, 0, 0, 0, 0, 0, 0};
        const m::RunResult r2 = m2.run({"MOVEQ #33,D1", "MOVEQ #0,D0", "BSET D1,D0", "BSET D1,(A0)"});
        CHECK_U(r2.d[0], 2);
        CHECK_U(m2.bus.read(0x2000, 1), 2);
    }
});

// ---- the status register ----------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.sr_ccr_and_usp_moves", [] {
    Machine mc;
    const m::RunResult r = mc.run({"MOVE.W #$001F,CCR", "MOVE SR,D0", "ANDI #$0A,CCR", "MOVEA.L #$1234,A0", "MOVE.L A0,USP", "MOVE.L USP,A1"});
    CHECK_U(r.d[0] & 0xFFFF, 0x271F);
    CHECK_U(ccr(r), 0x0A);
    CHECK_U(r.a[1], 0x1234);
    // ORI to SR keeps only the bits the CPU has
    Machine m2;
    const m::RunResult r2 = m2.run({"ORI #$FFFF,SR"});
    CHECK_U(r2.sr, 0xA71F);
});

// ---- control flow ---------------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.dbf_loop", [] {
    Machine mc;
    Asm a;
    a.line("MOVEQ #0,D0").line("MOVEQ #4,D1");
    const std::uint32_t loop = a.here();
    a.line("ADDQ.L #3,D0").line(std::format("DBF D1,${:X}", loop)).line("RTS");
    a.load(mc.bus);
    const m::RunResult r = m::run(mc.bus, kCode, mc.options);
    CB_CHECK(r.end == m::End::Return);
    CHECK_U(r.d[0], 15);
    CHECK_U(r.d[1] & 0xFFFF, 0xFFFF);
    CHECK_U(r.steps, 2 + 5 * 2 + 1);
});

CB_TEST("emulate.m68k.conditions", [] {
    Machine mc;
    // 1 compared with 2: signed less, unsigned lower, not equal, carry set
    const m::RunResult r = mc.run({"MOVEQ #1,D0", "CMPI.L #2,D0", "SLT D1", "SGE D2", "SHI D3", "SCS D4", "SNE D5", "SEQ D6", "SLS D7"});
    CHECK_U(r.d[1] & 0xFF, 0xFF);
    CHECK_U(r.d[2] & 0xFF, 0x00);
    CHECK_U(r.d[3] & 0xFF, 0x00);
    CHECK_U(r.d[4] & 0xFF, 0xFF);
    CHECK_U(r.d[5] & 0xFF, 0xFF);
    CHECK_U(r.d[6] & 0xFF, 0x00);
    CHECK_U(r.d[7] & 0xFF, 0xFF);
});

CB_TEST("emulate.m68k.branches_jsr_rts_jmp", [] {
    Machine mc;
    Asm main;
    main.line("MOVEQ #1,D0").line("JSR $1100").line("BRA.S $1010");
    main.load(mc.bus);
    Asm skipped(0x1010);
    skipped.line("MOVEQ #3,D2").line("RTS");
    skipped.load(mc.bus);
    Asm sub(0x1100);
    sub.line("MOVEQ #7,D1").line("RTS");
    sub.load(mc.bus);
    const m::RunResult r = m::run(mc.bus, kCode, mc.options);
    CB_CHECK(r.end == m::End::Return);
    CHECK_U(r.d[0], 1);
    CHECK_U(r.d[1], 7);
    CHECK_U(r.d[2], 3);
    CHECK_U(r.a[7], kStack);
    Machine m2;   // BSR / JMP (An)
    Asm b;
    b.line("BSR.S $1030").line("MOVEA.L #$1020,A0").line("JMP (A0)");
    b.load(m2.bus);
    Asm sub2(0x1030);
    sub2.line("MOVEQ #5,D1").line("RTS");
    sub2.load(m2.bus);
    Asm tail(0x1020);
    tail.line("MOVEQ #6,D2").line("RTS");
    tail.load(m2.bus);
    const m::RunResult r2 = m::run(m2.bus, kCode, m2.options);
    CB_CHECK(r2.end == m::End::Return);
    CHECK_U(r2.d[1], 5);
    CHECK_U(r2.d[2], 6);
});

CB_TEST("emulate.m68k.rtr_and_rte_restore_the_status", [] {
    // push a return address and a status word by hand, then RTR / RTE
    Machine mc;
    Asm tail(0x1010);
    tail.line("MOVEA.L #1,A4").line("RTS");
    tail.load(mc.bus);
    Asm a;
    a.line("PEA $1010").line("MOVE.W #$001F,-(A7)").line("RTR");
    a.load(mc.bus);
    const m::RunResult r = m::run(mc.bus, kCode, mc.options);
    CB_CHECK(r.end == m::End::Return);
    CHECK_U(r.a[4], 1);
    CHECK_U(ccr(r), 0x1F);   // RTR restored the condition codes, only those
    CHECK_U(r.sr & 0xFF00, 0x2700);
    Machine m2;
    tail.load(m2.bus);
    Asm b;
    b.line("PEA $1010").line("MOVE.W #$2015,-(A7)").line("RTE");
    b.load(m2.bus);
    const m::RunResult r2 = m::run(m2.bus, kCode, m2.options);
    CB_CHECK(r2.end == m::End::Return);
    CHECK_U(r2.sr, 0x2015);   // RTE restored the whole status register (through the bits the CPU has)
});

// ---- exceptions end a run -------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.exceptions_stop_the_run_with_a_reason", [] {
    struct Case {
        std::vector<std::string> lines;
        std::string exit;
    };
    const std::vector<Case> cases = {
        {{"TRAP #1"}, "stopped: TRAP"},
        {{"ILLEGAL"}, "stopped: ILLEGAL"},
        {{"STOP #$2000"}, "stopped: STOP"},
        {{"MOVE.W #0,CCR", "TRAPV"}, "return"},   // V clear: no trap
    };
    for (const Case& c : cases) {
        Machine mc;
        CB_CHECK_EQ(mc.run(c.lines).exit(), c.exit);
    }
    {   // TRAPV with V set (after an overflowing add)
        Machine mc;
        CB_CHECK_EQ(mc.run({"MOVE.B #$7F,D0", "ADDQ.B #1,D0", "TRAPV"}).exit(), std::string("stopped: TRAPV exception"));
    }
    {   // CHK: a negative register traps and sets N; one in range does not
        Machine mc;
        const m::RunResult r = mc.run({"MOVE.W #-1,D0", "MOVE.W #5,D1", "CHK.W D1,D0"});
        CB_CHECK_EQ(r.exit(), std::string("stopped: CHK exception"));
        CHECK_U(ccr(r) & kN, kN);
        Machine m2;
        CB_CHECK_EQ(m2.run({"MOVE.W #5,D0", "MOVE.W #5,D1", "CHK.W D1,D0"}).exit(), std::string("return"));
        Machine m3;
        CB_CHECK_EQ(m3.run({"MOVE.W #6,D0", "MOVE.W #5,D1", "CHK.W D1,D0"}).exit(), std::string("stopped: CHK exception"));
    }
});

CB_TEST("emulate.m68k.bytes_that_are_no_instruction_stop_the_run", [] {
    Machine mc;
    Asm a;
    a.line("MOVEQ #1,D0").words({0xA000});   // a line-A opcode
    a.load(mc.bus);
    const m::RunResult r = m::run(mc.bus, kCode, mc.options);
    CB_CHECK(r.end == m::End::Stopped);
    CB_CHECK_EQ(r.exit(), std::string("stopped: no instruction at 001002"));
    CHECK_U(r.steps, 1);
    CHECK_U(r.d[0], 1);
});

// ---- running ------------------------------------------------------------------------------------------------------

CB_TEST("emulate.m68k.run_starts_from_the_given_registers_and_returns", [] {
    Machine mc;
    mc.options.d = std::array<std::uint32_t, 8>{10, 20, 0, 0, 0, 0, 0, 0};
    mc.options.a = std::array<std::uint32_t, 8>{0, 0, 0, 0, 0, 0, 0, 0x9999};   // A7 is replaced by sp
    mc.options.sr = 0x2004;
    const m::RunResult r = mc.run({"ADD.L D1,D0"});
    CB_CHECK(r.end == m::End::Return);
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CHECK_U(r.d[0], 30);
    CHECK_U(r.a[7], kStack);
    CHECK_U(r.steps, 2);   // the ADD and the RTS
    CHECK_U(r.sr, 0x2000);
});

CB_TEST("emulate.m68k.run_hits_the_step_limit", [] {
    Machine mc;
    Asm a;
    a.line("BRA $1000");
    a.load(mc.bus);
    mc.options.max_steps = 10;
    const m::RunResult r = m::run(mc.bus, kCode, mc.options);
    CB_CHECK(r.end == m::End::Limit);
    CB_CHECK_EQ(r.exit(), std::string("limit"));
    CHECK_U(r.steps, 10);
    const cb::emulate::Run rec = m::record(r);
    CB_CHECK(rec.truncated);
    CB_CHECK(rec.exit && *rec.exit == "limit");
});

CB_TEST("emulate.m68k.devices_are_called_and_logged_in_order", [] {
    Machine mc;
    std::vector<std::uint32_t> writes;
    mc.bus.map(0xC00000, 0xC0000F, [](std::uint32_t a, unsigned n) -> std::uint32_t { return (a & ~1u) == 0xC00004 ? 0x0008 : (n == 1 ? 0x12345678u : 0u); },
               [&](std::uint32_t, unsigned, std::uint32_t v) { writes.push_back(v); });
    const m::RunResult r = mc.run({"MOVE.W $C00004,D0", "MOVE.W #$8F02,$C00004", "MOVE.B $C00001,D1"});
    CHECK_U(r.d[0], 8);
    CHECK_U(r.d[1], 0x78);   // a byte read keeps the low byte of what the device answered
    CB_CHECK_EQ(writes.size(), std::size_t{1});
    CHECK_U(writes[0], 0x8F02);
    CB_CHECK_EQ(r.io.size(), std::size_t{3});
    CB_CHECK_EQ(r.io[0].text(), std::string("r2"));
    CHECK_U(r.io[0].address, 0xC00004);
    CHECK_U(r.io[0].value, 8);
    CB_CHECK_EQ(r.io[1].text(), std::string("w2"));
    CHECK_U(r.io[1].value, 0x8F02);
    CB_CHECK_EQ(r.io[2].text(), std::string("r1"));
    // a second run on the same bus reports only its own accesses
    const m::RunResult again = m::run(mc.bus, kCode, mc.options);
    CB_CHECK_EQ(again.io.size(), std::size_t{3});
    CB_CHECK_EQ(mc.bus.log().size(), std::size_t{6});
});

CB_TEST("emulate.m68k.code_is_decoded_once_per_address", [] {
    m::Bus bus(0x10000);
    Asm a;
    a.line("MOVEQ #1,D0");
    a.load(bus);
    m::Cpu cpu(bus);
    cpu.pc = kCode;
    CB_CHECK(!cpu.step());
    CHECK_U(cpu.d[0], 1);
    Asm b;
    b.line("MOVEQ #2,D0");
    b.load(bus);
    cpu.pc = kCode;
    CB_CHECK(!cpu.step());
    CHECK_U(cpu.d[0], 1);   // the instruction as first decoded (a function is run as it was loaded)
    CHECK_U(cpu.steps, 2);
});

CB_TEST("emulate.m68k.record_is_the_run_record", [] {
    Machine mc;
    mc.bus.map(0x4000, 0x40FF, [](std::uint32_t, unsigned) { return 0x42u; });
    mc.options.d = std::array<std::uint32_t, 8>{1, 2, 3, 4, 5, 6, 7, 8};
    const cb::emulate::Run rec = m::record(mc.run({"MOVE.B $4000,D0"}));
    CB_CHECK(rec.exit && *rec.exit == "return");
    CB_CHECK(rec.endian == cb::emulate::Endian::Big);
    CHECK_U(rec.steps.value_or(0), 2);
    CB_CHECK(!rec.truncated);
    CB_CHECK_EQ(rec.results.size(), std::size_t{17});
    CHECK_U(std::get<std::uint64_t>(rec.results.at("D0")), 0x42);   // a byte move replaces the low byte of D0
    CHECK_U(std::get<std::uint64_t>(rec.results.at("D7")), 8);
    CHECK_U(std::get<std::uint64_t>(rec.results.at("A7")), kStack);
    CHECK_U(std::get<std::uint64_t>(rec.results.at("SR")), 0x2700);
    CB_CHECK_EQ(rec.io.size(), std::size_t{1});
    CB_CHECK_EQ(rec.io[0].kind, std::string("r1"));
    CHECK_U(rec.io[0].port, 0x4000);
    CHECK_U(rec.io[0].value, 0x42);
    CHECK_U(rec.io[0].size.value_or(0), 1);
    // the order of the results is D0..D7, A0..A7, SR
    std::vector<std::string> order;
    for (const auto& [k, v] : rec.results) order.push_back(k);
    CB_CHECK_EQ(order.front(), std::string("D0"));
    CB_CHECK_EQ(order[8], std::string("A0"));
    CB_CHECK_EQ(order.back(), std::string("SR"));
    CB_CHECK(std::any_cast<m::RunResult>(&rec.raw) != nullptr);
});

// ---- every opcode word of the decoder is either run or stopped, never crashes -----------------------------------------

CB_TEST("emulate.m68k.every_opcode_word_steps_without_crashing", [] {
    // Each of the 65,536 opcode words, with extension words of a recognisable value, on a bus whose registers point into
    // memory: the step ends (ran, or a Fault), and never throws.
    m::Bus bus(0x4000);
    for (std::uint32_t w = 0; w < 0x10000; ++w) {
        bus.memory()[0x1000] = static_cast<std::uint8_t>(w >> 8);
        bus.memory()[0x1001] = static_cast<std::uint8_t>(w);
        for (unsigned i = 2; i < 10; i += 2) {
            bus.memory()[0x1000 + i] = 0x00;
            bus.memory()[0x1000 + i + 1] = 0x10;
        }
        m::Cpu cpu(bus);
        cpu.pc = 0x1000;
        for (unsigned i = 0; i < 8; ++i) {
            cpu.a[i] = 0x2000 + 0x100 * i;
            cpu.d[i] = 3 + i;
        }
        cpu.a[7] = 0x3000;
        (void)cpu.step();
    }
});
