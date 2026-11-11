// cb/emulate/m6502_test.cpp -- the 6502 interpreter: instruction semantics (flags, wrap-arounds, stack), the bus and its
// device ranges, running a function, and the run record.
//
// Every expected value below is worked out by hand from the 6502 documentation (not recorded from this code). The old
// harness (state/work/typed-port/emulate.m6502: 19,510 cases made by the Python module) also agrees on every case with
// in-range inputs; the cases that gave the Python module registers outside a byte (a = -5 .. 600) have no meaning on a
// chip and are not carried over.
//
// DIVERGES from the Python module:
//  - Registers, the stack pointer and the entry address are 8 and 16 bits wide types; Python's ints could hold more, and a
//    run entered at 0x1FFF0 did not return at once (its pc never equalled 0xFFF0) while here the entry is 0xFFF0.
//  - A Bus takes exactly 64 KB (a shorter bytearray was an IndexError at the first access past its end).
//  - A run's `io` is what that run logged; the Python gave the bus's whole log, which a bus reused for a second run
//    carried over from the first.
//  - An undocumented opcode is a Fault returned by step(), not an exception.
#include "cb/emulate/m6502.hpp"

#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace m = cb::emulate::m6502;
using cb::Bytes;
namespace st = m::status;

namespace {

Bytes hex(std::string_view s) {
    Bytes out;
    int hi = -1;
    for (char c : s) {
        if (c == ' ') continue;
        const int d = c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10;
        if (hi < 0) {
            hi = d;
        } else {
            out.push_back(static_cast<std::uint8_t>(hi * 16 + d));
            hi = -1;
        }
    }
    return out;
}

void place(m::Bus& bus, std::uint16_t at, std::string_view code) {
    const Bytes b = hex(code);
    std::copy(b.begin(), b.end(), bus.memory().begin() + at);
}

// A bus with `code` at 0x8000 and a CPU about to run it.
struct Rig {
    m::Bus bus;
    m::Cpu cpu;
    explicit Rig(std::string_view code, bool decimal = false) : cpu(bus, decimal) {
        place(bus, 0x8000, code);
        cpu.regs.pc = 0x8000;
    }
    void step(int n = 1) {
        for (int i = 0; i < n; ++i) CB_CHECK(!cpu.step());
    }
};

}  // namespace

CB_TEST("emulate.m6502.load_and_flags", [] {
    Rig r("A9 80 A2 00 A0 7F");   // LDA #$80 ; LDX #0 ; LDY #$7F
    r.step();
    CB_CHECK_EQ(r.cpu.regs.a, 0x80);
    CB_CHECK(r.cpu.regs.p & st::N);
    CB_CHECK(!(r.cpu.regs.p & st::Z));
    r.step();
    CB_CHECK_EQ(r.cpu.regs.x, 0);
    CB_CHECK(r.cpu.regs.p & st::Z);
    CB_CHECK(!(r.cpu.regs.p & st::N));
    r.step();
    CB_CHECK_EQ(r.cpu.regs.y, 0x7F);
    CB_CHECK_EQ(r.cpu.regs.pc, 0x8006);
    CB_CHECK_EQ(r.cpu.steps, 3u);
});

CB_TEST("emulate.m6502.adc_binary", [] {
    // 0x50 + 0x50 = 0xA0: two positives make a negative: V and N, no carry
    Rig r("18 A9 50 69 50");   // CLC ; LDA #$50 ; ADC #$50
    r.step(3);
    CB_CHECK_EQ(r.cpu.regs.a, 0xA0);
    CB_CHECK(r.cpu.regs.p & st::V);
    CB_CHECK(r.cpu.regs.p & st::N);
    CB_CHECK(!(r.cpu.regs.p & st::C));
    // 0xFF + 0x01 = 0x100: zero, carry, no overflow
    Rig s("18 A9 FF 69 01");
    s.step(3);
    CB_CHECK_EQ(s.cpu.regs.a, 0);
    CB_CHECK(s.cpu.regs.p & st::C);
    CB_CHECK(s.cpu.regs.p & st::Z);
    CB_CHECK(!(s.cpu.regs.p & st::V));
    // the carry in counts: 0x10 + 0x20 + 1
    Rig t("38 A9 10 69 20");   // SEC
    t.step(3);
    CB_CHECK_EQ(t.cpu.regs.a, 0x31);
    // 0x80 + 0x80 = 0x100: negative + negative = positive: V and C and Z
    Rig u("18 A9 80 69 80");
    u.step(3);
    CB_CHECK_EQ(u.cpu.regs.a, 0);
    CB_CHECK(u.cpu.regs.p & st::V);
    CB_CHECK(u.cpu.regs.p & st::C);
});

CB_TEST("emulate.m6502.sbc_carry_is_no_borrow", [] {
    Rig r("38 A9 50 E9 F0");   // SEC ; LDA #$50 ; SBC #$F0: 80 - (-16) = 96 (0x60); 0x50 < 0xF0 unsigned: borrow, C clear
    r.step(3);
    CB_CHECK_EQ(r.cpu.regs.a, 0x60);
    CB_CHECK(!(r.cpu.regs.p & st::C));
    CB_CHECK(!(r.cpu.regs.p & st::V));
    Rig s("38 A9 50 E9 30");   // 0x50 - 0x30 = 0x20, no borrow
    s.step(3);
    CB_CHECK_EQ(s.cpu.regs.a, 0x20);
    CB_CHECK(s.cpu.regs.p & st::C);
    Rig t("18 A9 50 E9 30");   // carry clear: one more taken away
    t.step(3);
    CB_CHECK_EQ(t.cpu.regs.a, 0x1F);
    Rig u("38 A9 80 E9 01");   // -128 - 1 overflows
    u.step(3);
    CB_CHECK_EQ(u.cpu.regs.a, 0x7F);
    CB_CHECK(u.cpu.regs.p & st::V);
});

CB_TEST("emulate.m6502.decimal_mode_only_when_asked", [] {
    // SED CLC LDA #$19 ADC #$28 -> 0x47 (BCD) on a stock 6502 ...
    Rig bcd("F8 18 A9 19 69 28", true);
    bcd.step(4);
    CB_CHECK_EQ(bcd.cpu.regs.a, 0x47);
    // ... and 0x41 binary on the NES's 2A03, which has no decimal mode: the D flag is set but ignored
    Rig nes("F8 18 A9 19 69 28", false);
    nes.step(4);
    CB_CHECK_EQ(nes.cpu.regs.a, 0x41);
    // BCD carry: 0x99 + 0x01 = 0x00 with C
    Rig carry("F8 18 A9 99 69 01", true);
    carry.step(4);
    CB_CHECK_EQ(carry.cpu.regs.a, 0x00);
    CB_CHECK(carry.cpu.regs.p & st::C);
    // BCD subtraction: 0x40 - 0x13 = 0x27
    Rig sub("F8 38 A9 40 E9 13", true);
    sub.step(4);
    CB_CHECK_EQ(sub.cpu.regs.a, 0x27);
    CB_CHECK(sub.cpu.regs.p & st::C);
});

CB_TEST("emulate.m6502.compare", [] {
    Rig r("A9 40 C9 40 C9 41 C9 3F");
    r.step(2);   // CMP equal: Z and C
    CB_CHECK(r.cpu.regs.p & st::Z);
    CB_CHECK(r.cpu.regs.p & st::C);
    r.step();    // 0x40 - 0x41: borrow: no C, negative
    CB_CHECK(!(r.cpu.regs.p & st::C));
    CB_CHECK(r.cpu.regs.p & st::N);
    CB_CHECK(!(r.cpu.regs.p & st::Z));
    CB_CHECK_EQ(r.cpu.regs.a, 0x40);   // a compare changes no register
    r.step();    // 0x40 - 0x3F = 1
    CB_CHECK(r.cpu.regs.p & st::C);
    CB_CHECK(!(r.cpu.regs.p & st::N));
});

CB_TEST("emulate.m6502.shifts_and_rotates", [] {
    Rig r("38 A9 81 2A 6A 0A 4A");   // SEC ; LDA #$81 ; ROL A ; ROR A ; ASL A ; LSR A
    r.step(3);                        // ROL: 0x81 << 1 | 1 = 0x03, carry out 1
    CB_CHECK_EQ(r.cpu.regs.a, 0x03);
    CB_CHECK(r.cpu.regs.p & st::C);
    r.step();                         // ROR: 0x03 >> 1 | carry<<7 = 0x81, carry out 1
    CB_CHECK_EQ(r.cpu.regs.a, 0x81);
    CB_CHECK(r.cpu.regs.p & st::C);
    r.step();                         // ASL: 0x02, carry out 1
    CB_CHECK_EQ(r.cpu.regs.a, 0x02);
    CB_CHECK(r.cpu.regs.p & st::C);
    r.step();                         // LSR: 0x01, carry out 0
    CB_CHECK_EQ(r.cpu.regs.a, 0x01);
    CB_CHECK(!(r.cpu.regs.p & st::C));
    // in memory: ASL $10
    Rig s("06 10");
    s.bus.memory()[0x10] = 0xC0;
    s.step();
    CB_CHECK_EQ(s.bus.memory()[0x10], 0x80);
    CB_CHECK(s.cpu.regs.p & st::C);
    CB_CHECK(s.cpu.regs.p & st::N);
});

CB_TEST("emulate.m6502.inc_dec_wrap_and_transfers", [] {
    Rig r("E6 10 C6 11 A2 FF E8 A0 00 88 A9 7F AA 9A BA");
    r.bus.memory()[0x10] = 0xFF;
    r.bus.memory()[0x11] = 0x00;
    r.step();   // INC $10: 0xFF -> 0, Z
    CB_CHECK_EQ(r.bus.memory()[0x10], 0);
    CB_CHECK(r.cpu.regs.p & st::Z);
    r.step();   // DEC $11: 0 -> 0xFF, N
    CB_CHECK_EQ(r.bus.memory()[0x11], 0xFF);
    CB_CHECK(r.cpu.regs.p & st::N);
    r.step(2);  // LDX #$FF ; INX -> 0
    CB_CHECK_EQ(r.cpu.regs.x, 0);
    r.step(2);  // LDY #0 ; DEY -> 0xFF
    CB_CHECK_EQ(r.cpu.regs.y, 0xFF);
    r.step(3);  // LDA #$7F ; TAX ; TXS (TXS sets no flags)
    CB_CHECK_EQ(r.cpu.regs.x, 0x7F);
    CB_CHECK_EQ(r.cpu.regs.s, 0x7F);
    r.cpu.regs.x = 0;
    r.step();   // TSX: x = s = 0x7F, flags follow
    CB_CHECK_EQ(r.cpu.regs.x, 0x7F);
});

CB_TEST("emulate.m6502.zero_page_indexing_wraps_in_page_zero", [] {
    Rig r("B5 F0");   // LDA $F0,X with X = $20: the address wraps to $10, not $110
    r.cpu.regs.x = 0x20;
    r.bus.memory()[0x10] = 0xAB;
    r.bus.memory()[0x110] = 0xCD;
    r.step();
    CB_CHECK_EQ(r.cpu.regs.a, 0xAB);
    // (zp),Y: the pointer at $FF reads its high byte from $00
    Rig s("B1 FF");
    s.cpu.regs.y = 2;
    s.bus.memory()[0xFF] = 0x34;
    s.bus.memory()[0x00] = 0x12;
    s.bus.memory()[0x100] = 0x99;
    s.bus.memory()[0x1234 + 2] = 0x5E;
    s.step();
    CB_CHECK_EQ(s.cpu.regs.a, 0x5E);
    // (zp,X): the pointer is at zp + X, wrapped
    Rig t("A1 FE");
    t.cpu.regs.x = 3;   // $FE + 3 = $101 -> $01
    t.bus.memory()[0x01] = 0x00;
    t.bus.memory()[0x02] = 0x30;
    t.bus.memory()[0x3000] = 0x77;
    t.step();
    CB_CHECK_EQ(t.cpu.regs.a, 0x77);
    // absolute,X crosses a page and wraps at 64 KB
    Rig u("BD F0 FF");
    u.cpu.regs.x = 0x20;   // $FFF0 + $20 = $10010 -> $0010
    u.bus.memory()[0x0010] = 0x42;
    u.step();
    CB_CHECK_EQ(u.cpu.regs.a, 0x42);
});

CB_TEST("emulate.m6502.indirect_jmp_page_bug", [] {
    Rig r("6C FF 20");   // JMP ($20FF)
    r.bus.memory()[0x20FF] = 0x34;
    r.bus.memory()[0x2100] = 0x99;   // a later chip would read the high byte here
    r.bus.memory()[0x2000] = 0x12;   // the NMOS chip reads it from the start of the same page
    r.step();
    CB_CHECK_EQ(r.cpu.regs.pc, 0x1234);
});

CB_TEST("emulate.m6502.stack_instructions", [] {
    Rig r("48 08 68 28");   // PHA ; PHP ; PLA ; PLP
    r.cpu.regs.a = 0x5A;
    r.cpu.regs.p = st::U | st::C;
    r.step();
    CB_CHECK_EQ(r.cpu.regs.s, 0xFC);
    CB_CHECK_EQ(r.bus.memory()[0x1FD], 0x5A);
    r.step();   // PHP pushes B and bit 5 set
    CB_CHECK_EQ(r.bus.memory()[0x1FC], st::U | st::C | st::B);
    r.step();   // PLA gets the status byte, with flags from it
    CB_CHECK_EQ(r.cpu.regs.a, st::U | st::C | st::B);
    r.step();   // PLP gets 0x5A: B is ignored, U is always set
    CB_CHECK_EQ(r.cpu.regs.p, (0x5A & ~st::B) | st::U);
    CB_CHECK_EQ(r.cpu.regs.s, 0xFD);
    // the stack pointer wraps within page 1
    Rig s("48");
    s.cpu.regs.s = 0x00;
    s.cpu.regs.a = 0x11;
    s.step();
    CB_CHECK_EQ(s.bus.memory()[0x100], 0x11);
    CB_CHECK_EQ(s.cpu.regs.s, 0xFF);
});

CB_TEST("emulate.m6502.jsr_rts_brk_rti", [] {
    Rig r("20 10 80");   // JSR $8010
    place(r.bus, 0x8010, "60");   // RTS
    r.step();
    CB_CHECK_EQ(r.cpu.regs.pc, 0x8010);
    // JSR pushes the address of its own last byte ($8002), high byte first
    CB_CHECK_EQ(r.bus.memory()[0x1FD], 0x80);
    CB_CHECK_EQ(r.bus.memory()[0x1FC], 0x02);
    r.step();   // RTS adds one to what it pops
    CB_CHECK_EQ(r.cpu.regs.pc, 0x8003);
    CB_CHECK_EQ(r.cpu.regs.s, 0xFD);

    Rig b("00");   // BRK: pushes pc + 2, then the status with B set, sets I, vectors through $FFFE
    b.bus.memory()[0xFFFE] = 0x00;
    b.bus.memory()[0xFFFF] = 0x90;
    b.cpu.regs.p = st::U;
    b.step();
    CB_CHECK_EQ(b.cpu.regs.pc, 0x9000);
    CB_CHECK_EQ(b.bus.memory()[0x1FD], 0x80);
    CB_CHECK_EQ(b.bus.memory()[0x1FC], 0x02);
    CB_CHECK_EQ(b.bus.memory()[0x1FB], st::U | st::B);
    CB_CHECK(b.cpu.regs.p & st::I);
    place(b.bus, 0x9000, "40");   // RTI: status (B ignored), then the exact return address
    b.step();
    CB_CHECK_EQ(b.cpu.regs.pc, 0x8002);
    CB_CHECK_EQ(b.cpu.regs.p, st::U);
});

CB_TEST("emulate.m6502.branches", [] {
    Rig r("A9 00 F0 02 A9 01 EA");   // LDA #0 ; BEQ +2 (taken) ; (skipped LDA #1) ; NOP
    r.step(2);
    CB_CHECK_EQ(r.cpu.regs.pc, 0x8006);
    Rig s("A9 01 F0 02 EA");         // not taken
    s.step(2);
    CB_CHECK_EQ(s.cpu.regs.pc, 0x8004);
    Rig t("D0 FE");                  // BNE to itself (the offset is signed: -2)
    t.cpu.regs.p = st::U;
    t.step();
    CB_CHECK_EQ(t.cpu.regs.pc, 0x8000);
    // each branch tests its own flag
    struct Case {
        const char* code;
        std::uint8_t p;
        bool taken;
    };
    const Case cases[] = {{"90 05", st::U, true},  {"90 05", st::U | st::C, false}, {"B0 05", st::U | st::C, true},
                          {"30 05", st::U | st::N, true}, {"10 05", st::U | st::N, false}, {"50 05", st::U, true},
                          {"70 05", st::U | st::V, true}, {"F0 05", st::U | st::Z, true}, {"D0 05", st::U | st::Z, false}};
    for (const Case& c : cases) {
        Rig x(c.code);
        x.cpu.regs.p = c.p;
        x.step();
        CB_CHECK_EQ(x.cpu.regs.pc, c.taken ? 0x8007 : 0x8002);
    }
});

CB_TEST("emulate.m6502.bit", [] {
    Rig r("A9 0F 24 10");   // LDA #$0F ; BIT $10 with $10 = $C0: Z (0x0F & 0xC0 = 0), N and V from the operand
    r.bus.memory()[0x10] = 0xC0;
    r.step(2);
    CB_CHECK(r.cpu.regs.p & st::Z);
    CB_CHECK(r.cpu.regs.p & st::N);
    CB_CHECK(r.cpu.regs.p & st::V);
    CB_CHECK_EQ(r.cpu.regs.a, 0x0F);
});

CB_TEST("emulate.m6502.undocumented_opcode_stops", [] {
    Rig r("02");
    const auto fault = r.cpu.step();
    CB_CHECK(fault.has_value());
    CB_CHECK_EQ(fault->opcode, 0x02);
    CB_CHECK_EQ(fault->pc, 0x8000);
    CB_CHECK_EQ(fault->text(), "undocumented opcode 02 at 8000");
    CB_CHECK_EQ(r.cpu.steps, 0u);          // nothing ran
    CB_CHECK_EQ(r.cpu.regs.pc, 0x8000);
});

CB_TEST("emulate.m6502.flags_view", [] {
    Rig r("");
    r.cpu.regs.p = st::N | st::Z | st::C;
    const auto f = r.cpu.flags();
    CB_CHECK_EQ(f.n, 1);
    CB_CHECK_EQ(f.v, 0);
    CB_CHECK_EQ(f.b, 0);
    CB_CHECK_EQ(f.d, 0);
    CB_CHECK_EQ(f.i, 0);
    CB_CHECK_EQ(f.z, 1);
    CB_CHECK_EQ(f.c, 1);
});

// ---- the bus ----------------------------------------------------------------------------------------------------

CB_TEST("emulate.m6502.bus_ranges_and_log", [] {
    m::Bus bus;
    std::vector<std::pair<int, int>> written;
    bus.map(0x2000, 0x2007, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 7 + 3); },
            [&](std::uint16_t a, std::uint8_t v) { written.emplace_back(a, v); });
    bus.map(0x4000, 0x4017, {}, {});   // a range with no callbacks: reads 0, swallows writes
    bus.write(0x10, 0x55);
    CB_CHECK_EQ(bus.read(0x10), 0x55);
    CB_CHECK(bus.log().empty());       // plain RAM is not logged
    CB_CHECK_EQ(bus.read(0x2003), static_cast<std::uint8_t>(0x2003 * 7 + 3));
    bus.write(0x2001, 0x1FF & 0xFF);
    bus.write(0x4005, 9);
    CB_CHECK_EQ(bus.read(0x4005), 0);
    CB_CHECK_EQ(bus.memory()[0x2001], 0);   // the RAM behind a device range is untouched
    CB_CHECK_EQ(bus.memory()[0x4005], 0);
    CB_CHECK_EQ(written.size(), 1u);
    CB_CHECK_EQ(written[0].first, 0x2001);
    CB_CHECK_EQ(written[0].second, 0xFF);
    CB_CHECK_EQ(bus.log().size(), 4u);
    CB_CHECK(bus.log()[0].kind == m::DeviceAccess::Kind::Read);
    CB_CHECK_EQ(bus.log()[0].address, 0x2003);
    CB_CHECK(bus.log()[1].kind == m::DeviceAccess::Kind::Write);
    CB_CHECK_EQ(bus.log()[1].value, 0xFF);
    CB_CHECK_EQ(bus.log()[3].address, 0x4005);
});

CB_TEST("emulate.m6502.bus_first_range_wins", [] {
    m::Bus bus;
    bus.map(0x100, 0x1FF, [](std::uint16_t) -> std::uint8_t { return 1; });
    bus.map(0x100, 0x1FF, [](std::uint16_t) -> std::uint8_t { return 2; });
    CB_CHECK_EQ(bus.read(0x150), 1);
});

CB_TEST("emulate.m6502.bus_over_the_callers_memory", [] {
    Bytes ram(m::kMemorySize, 0);
    {
        m::Bus bus = m::Bus::over(ram);
        bus.write(0x1234, 0xEE);
        CB_CHECK_EQ(bus.read(0x1234), 0xEE);
    }
    CB_CHECK_EQ(ram[0x1234], 0xEE);   // changed in place
    Bytes small(100, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)m::Bus::over(small));
    Bytes big(m::kMemorySize + 1, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)m::Bus(big));
    const Bytes image = {1, 2, 3};
    m::Bus own(image);
    CB_CHECK_EQ(own.read(2), 3);
    CB_CHECK_EQ(own.read(3), 0);
    CB_CHECK_EQ(own.memory().size(), m::kMemorySize);
    m::Bus moved = std::move(own);   // the memory goes with it
    CB_CHECK_EQ(moved.read(1), 2);
});

// ---- running a function ------------------------------------------------------------------------------------------

CB_TEST("emulate.m6502.run_multiplies", [] {
    // zero-page multiply: $10 * $11 into $14 ($15 on carry): A5 10 85 12 A5 11 85 13 A9 00 85 14 A2 08 46 13 90 0B ...
    const Bytes code = hex("A5 10 85 12 A5 11 85 13 A9 00 85 14 A2 08 46 13 90 0B 18 A5 14 65 12 85 14 90 02 E6 15 06 12 CA D0 EC 60");
    m::Bus bus;
    std::copy(code.begin(), code.end(), bus.memory().begin() + 0x8000);
    bus.memory()[0x10] = 12;
    bus.memory()[0x11] = 13;
    const m::RunResult r = m::run(bus, 0x8000);
    CB_CHECK(r.end == m::End::Return);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(bus.memory()[0x14], 156);
    CB_CHECK_EQ(bus.memory()[0x15], 0);
    CB_CHECK_EQ(r.regs.s, 0xFD);       // the return address was popped
    CB_CHECK(r.io.empty());
    CB_CHECK(r.steps > 40 && r.steps < 200);
});

CB_TEST("emulate.m6502.run_registers_in_and_out", [] {
    m::Bus bus;
    place(bus, 0x8000, "8A 18 69 01 AA 98 60");   // TXA ; CLC ; ADC #1 ; TAX ; TYA ; RTS: a = x + 1, x = a, a = y
    m::RunOptions o;
    o.x = 41;
    o.y = 9;
    const m::RunResult r = m::run(bus, 0x8000, o);
    CB_CHECK(r.end == m::End::Return);
    CB_CHECK_EQ(r.regs.x, 42);
    CB_CHECK_EQ(r.regs.a, 9);
    CB_CHECK_EQ(r.regs.y, 9);
});

CB_TEST("emulate.m6502.run_limit_and_fault_and_entry_at_return", [] {
    m::Bus bus;
    place(bus, 0x8000, "4C 00 80");   // JMP $8000
    m::RunOptions o;
    o.max_steps = 37;
    const m::RunResult spin = m::run(bus, 0x8000, o);
    CB_CHECK(spin.end == m::End::Limit);
    CB_CHECK_EQ(spin.steps, 37u);
    CB_CHECK_EQ(spin.exit(), "limit");

    m::Bus bad;
    place(bad, 0x8000, "A9 01 02 60");   // LDA #1 ; (undocumented) ; RTS
    const m::RunResult fault = m::run(bad, 0x8000);
    CB_CHECK(fault.end == m::End::Stopped);
    CB_CHECK_EQ(fault.steps, 1u);
    CB_CHECK_EQ(fault.regs.a, 1);
    CB_CHECK_EQ(fault.exit(), "stopped: undocumented opcode 02 at 8002");

    m::Bus nothing;
    const m::RunResult at_once = m::run(nothing, m::kReturnAddress);   // entered at the return address: done
    CB_CHECK(at_once.end == m::End::Return);
    CB_CHECK_EQ(at_once.steps, 0u);

    const m::RunResult none = m::run(nothing, 0x8000, {.max_steps = 0});
    CB_CHECK(none.end == m::End::Limit);
});

CB_TEST("emulate.m6502.run_logs_device_accesses_in_order", [] {
    m::Bus bus;
    int polls = 0;
    // 8000: LDA $2002 ; BPL $8000 (loops until bit 7 is up) ; STA $2000 ; RTS
    place(bus, 0x8000, "AD 02 20 10 FB 8D 00 20 60");
    std::vector<int> wrote;
    bus.map(0x2000, 0x2007, [&](std::uint16_t) -> std::uint8_t { return ++polls >= 3 ? 0x80 : 0x00; },
            [&](std::uint16_t, std::uint8_t v) { wrote.push_back(v); });
    const m::RunResult r = m::run(bus, 0x8000);
    CB_CHECK(r.end == m::End::Return);
    CB_CHECK_EQ(polls, 3);
    CB_CHECK_EQ(wrote.size(), 1u);
    CB_CHECK_EQ(wrote[0], 0x80);
    // three reads, then the write
    CB_CHECK_EQ(r.io.size(), 4u);
    for (int i = 0; i < 3; ++i) {
        CB_CHECK(r.io[static_cast<std::size_t>(i)].kind == m::DeviceAccess::Kind::Read);
        CB_CHECK_EQ(r.io[static_cast<std::size_t>(i)].address, 0x2002);
    }
    CB_CHECK(r.io[3].kind == m::DeviceAccess::Kind::Write);
    // a second run on the same bus reports its own accesses only
    polls = 5;
    const m::RunResult again = m::run(bus, 0x8000);
    CB_CHECK_EQ(again.io.size(), 2u);
    CB_CHECK_EQ(bus.log().size(), 6u);
});

CB_TEST("emulate.m6502.run_decimal_flag", [] {
    const char* program = "F8 18 A9 19 69 28 38 E9 02 D8 60";   // SED CLC LDA #$19 ADC #$28 SEC SBC #$02 CLD RTS
    m::Bus a;
    place(a, 0x8000, program);
    CB_CHECK_EQ(m::run(a, 0x8000, {.decimal = true}).regs.a, 0x45);    // BCD: 19 + 28 - 02 = 45
    m::Bus b;
    place(b, 0x8000, program);
    CB_CHECK_EQ(m::run(b, 0x8000, {.decimal = false}).regs.a, 0x3F);   // binary: 0x41 - 0x02
});

CB_TEST("emulate.m6502.record", [] {
    m::Bus bus;
    place(bus, 0x8000, "AD 00 20 60");   // LDA $2000 ; RTS
    bus.map(0x2000, 0x2000, [](std::uint16_t) -> std::uint8_t { return 0x9C; });
    const cb::emulate::Run run = m::recorded(bus, 0x8000, {.x = 7});
    CB_CHECK_EQ(run.exit.value_or(""), "return");
    CB_CHECK_EQ(run.ended(), "return");
    CB_CHECK(!run.truncated);
    CB_CHECK(run.endian == cb::emulate::Endian::Little);
    CB_CHECK_EQ(run.steps.value_or(0), 2u);
    CB_CHECK_EQ(run.results.size(), 5u);
    auto reg = [&](const char* name) { return std::get<std::uint64_t>(run.results.at(name)); };
    CB_CHECK_EQ(reg("A"), 0x9C);
    CB_CHECK_EQ(reg("X"), 7);
    CB_CHECK_EQ(reg("Y"), 0);
    CB_CHECK_EQ(reg("S"), 0xFD);
    CB_CHECK_EQ(reg("P"), (st::U | st::I | st::N));
    // results come in the order the emulator names them
    std::string order;
    for (const auto& kv : run.results) order += kv.first;
    CB_CHECK_EQ(order, "AXYPS");
    CB_CHECK_EQ(run.io.size(), 1u);
    CB_CHECK_EQ(run.io[0].kind, "r");
    CB_CHECK_EQ(run.io[0].port, 0x2000);
    CB_CHECK_EQ(run.io[0].value, 0x9C);
    CB_CHECK(!run.io[0].step.has_value());
    // the emulator's own record is kept for the lane that wants it
    CB_CHECK_EQ(std::any_cast<const m::RunResult&>(run.raw).steps, 2u);

    m::Bus loop;
    place(loop, 0x8000, "4C 00 80");
    const cb::emulate::Run cut = m::recorded(loop, 0x8000, {.max_steps = 10});
    CB_CHECK(cut.truncated);
    CB_CHECK_EQ(cut.ended(), "limit");
});
