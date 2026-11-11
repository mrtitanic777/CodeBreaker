// cb/emulate/z80_test.cpp -- the Z80 interpreter: flags of the arithmetic and logic groups (documented and the two
// undocumented bits), loads, exchanges, the stack, jumps and calls, rotates and bit operations, the index registers and
// their undocumented halves, the block instructions, ports, the bus, running a function, and the run record.
//
// Every expected value is worked out by hand from the Zilog documentation and "The Undocumented Z80 Documented" (not
// recorded from this code). The old harness (state/work/typed-port/emulate.z80: 11,680 cases made by the Python module)
// agrees on all of them that the chip defines.
//
// DIVERGES from the Python module:
//  - Registers are 8 and 16 bits wide types (Python's ints could hold more); the entry address is 16 bits.
//  - A Bus takes exactly 64 KB; plain memory without ports is Bus::bare (the Python told a bytearray from a Bus by hasattr).
//  - A run's `io` is what that run logged (the Python gave the bus's whole log, carried over a reused bus).
//  - Port numbers reach a callback whole (16 bits, as the chip puts BC on the bus); the log keeps the low byte, as before.
//  - A CPU that was told HALT, or cannot decode, returns a Fault from step() instead of raising.
#include "cb/emulate/z80.hpp"

#include <format>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace z = cb::emulate::z80;
using cb::Bytes;
namespace fl = z::flag;

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

void place(z::Bus& bus, std::uint16_t at, std::string_view code) {
    const Bytes b = hex(code);
    std::copy(b.begin(), b.end(), bus.memory().begin() + at);
}

// A bus with `code` at 0x8000 and a CPU about to run it.
struct Rig {
    z::Bus bus;
    z::Cpu cpu;
    explicit Rig(std::string_view code) : cpu(bus) {
        place(bus, 0x8000, code);
        cpu.regs.pc = 0x8000;
    }
    void step(int n = 1) {
        for (int i = 0; i < n; ++i) CB_CHECK(!cpu.step());
    }
    z::Registers& r() { return cpu.regs; }
};

}  // namespace

CB_TEST("emulate.z80.add_sub_flags", [] {
    Rig a("3E 7F C6 01");   // LD A,$7F ; ADD A,1: 127 + 1 overflows into the sign
    a.step(2);
    CB_CHECK_EQ(a.r().a, 0x80);
    CB_CHECK_EQ(a.r().f, fl::S | fl::H | fl::P);
    Rig b("3E 80 D6 01");   // SUB 1: 0x80 - 1 = 0x7F, half borrow, overflow, N; bits 5 and 3 of the result are set
    b.step(2);
    CB_CHECK_EQ(b.r().a, 0x7F);
    CB_CHECK_EQ(b.r().f, fl::H | fl::P | fl::N | fl::Y | fl::X);
    Rig c("3E FF C6 01");   // 0xFF + 1: zero, carry, half carry
    c.step(2);
    CB_CHECK_EQ(c.r().a, 0);
    CB_CHECK_EQ(c.r().f, fl::Z | fl::H | fl::C);
    Rig d("37 3E 10 CE 0F");   // SCF ; LD A,$10 ; ADC A,$0F: with the carry 0x10 + 0x0F + 1 = 0x20
    d.step(3);
    CB_CHECK_EQ(d.r().a, 0x20);
    CB_CHECK_EQ(d.r().f, fl::Y | fl::H);
    Rig e("37 3E 00 DE 00");   // SBC A,0 with carry: 0 - 0 - 1 = 0xFF
    e.step(3);
    CB_CHECK_EQ(e.r().a, 0xFF);
    CB_CHECK_EQ(e.r().f, fl::S | fl::Y | fl::H | fl::X | fl::N | fl::C);
});

CB_TEST("emulate.z80.logic_flags", [] {
    Rig a("3E F0 E6 0F");   // AND $0F: zero, H always set on AND, parity even
    a.step(2);
    CB_CHECK_EQ(a.r().a, 0);
    CB_CHECK_EQ(a.r().f, fl::Z | fl::H | fl::P);
    Rig b("AF");            // XOR A: zero, even parity
    b.step();
    CB_CHECK_EQ(b.r().a, 0);
    CB_CHECK_EQ(b.r().f, fl::Z | fl::P);
    Rig c("3E 07 F6 30");   // OR $30: 0x37, parity of 0x37 (5 ones) is odd; bit 5 set
    c.step(2);
    CB_CHECK_EQ(c.r().a, 0x37);
    CB_CHECK_EQ(c.r().f, fl::Y);
});

CB_TEST("emulate.z80.compare_takes_bits_5_and_3_from_the_operand", [] {
    Rig a("3E 10 FE 10");   // CP $10: equal; 0x10 has neither bit 5 nor 3
    a.step(2);
    CB_CHECK_EQ(a.r().a, 0x10);   // CP leaves A alone
    CB_CHECK_EQ(a.r().f, fl::Z | fl::N);
    Rig b("3E 28 FE 28");   // CP $28: equal; the operand's bits 5 and 3 are copied
    b.step(2);
    CB_CHECK_EQ(b.r().f, fl::Z | fl::N | fl::Y | fl::X);
});

CB_TEST("emulate.z80.inc_dec_keep_carry", [] {
    Rig a("37 3E 7F 3C 3D");   // SCF ; LD A,$7F ; INC A ; DEC A
    a.step(3);
    CB_CHECK_EQ(a.r().a, 0x80);
    CB_CHECK_EQ(a.r().f, fl::S | fl::H | fl::P | fl::C);   // overflow 7F -> 80; carry untouched
    a.step();
    CB_CHECK_EQ(a.r().a, 0x7F);
    CB_CHECK_EQ(a.r().f, fl::H | fl::P | fl::N | fl::Y | fl::X | fl::C);
    Rig b("01 FF FF 03 0B");   // LD BC,$FFFF ; INC BC ; DEC BC: 16-bit inc and dec touch no flags
    b.r().f = 0xA5;
    b.step(2);
    CB_CHECK_EQ(b.r().bc(), 0);
    b.step();
    CB_CHECK_EQ(b.r().bc(), 0xFFFF);
    CB_CHECK_EQ(b.r().f, 0xA5);
});

CB_TEST("emulate.z80.sixteen_bit_arithmetic", [] {
    Rig a("21 FF 0F 01 01 00 09");   // LD HL,$0FFF ; LD BC,1 ; ADD HL,BC: half carry from bit 11, bits 5 and 3 from H
    a.step(3);
    CB_CHECK_EQ(a.r().hl(), 0x1000);
    CB_CHECK_EQ(a.r().f, fl::H);
    Rig b("21 00 80 ED 62");   // LD HL,$8000 ; SBC HL,HL: zero, N
    b.step(2);
    CB_CHECK_EQ(b.r().hl(), 0);
    CB_CHECK_EQ(b.r().f, fl::Z | fl::N);
    Rig c("21 FF 7F 11 01 00 ED 5A");   // ADC HL,DE: 0x7FFF + 1 = 0x8000: S, overflow, half carry
    c.step(3);
    CB_CHECK_EQ(c.r().hl(), 0x8000);
    CB_CHECK_EQ(c.r().f, fl::S | fl::H | fl::P);
    Rig d("DD 21 00 10 DD 09");   // LD IX,$1000 ; ADD IX,BC with BC = 0
    d.step(2);
    CB_CHECK_EQ(d.r().ix, 0x1000);
});

CB_TEST("emulate.z80.daa", [] {
    Rig a("3E 15 C6 27 27");   // 15 + 27 = 3C -> BCD 42
    a.step(3);
    CB_CHECK_EQ(a.r().a, 0x42);
    Rig b("3E 99 C6 01 27");   // 99 + 01 = 9A -> BCD 00 with carry
    b.step(3);
    CB_CHECK_EQ(b.r().a, 0x00);
    CB_CHECK(b.r().f & fl::C);
    CB_CHECK(b.r().f & fl::Z);
    Rig c("3E 42 D6 15 27");   // 42 - 15 = 2D -> BCD 27 (after a subtraction DAA subtracts)
    c.step(3);
    CB_CHECK_EQ(c.r().a, 0x27);
});

CB_TEST("emulate.z80.accumulator_group", [] {
    Rig a("3E 81 07");   // RLCA: 0x81 -> 0x03, carry out
    a.step(2);
    CB_CHECK_EQ(a.r().a, 0x03);
    CB_CHECK_EQ(a.r().f, fl::C);
    Rig b("3E 01 ED 44");   // NEG: 0 - 1 = 0xFF: S, H, N, C and bits 5, 3 of the result
    b.step(2);
    CB_CHECK_EQ(b.r().a, 0xFF);
    CB_CHECK_EQ(b.r().f, fl::S | fl::Y | fl::H | fl::X | fl::N | fl::C);
    Rig c("3E 55 2F");   // CPL: 0xAA, H and N set, bits 5 and 3 of A
    c.step(2);
    CB_CHECK_EQ(c.r().a, 0xAA);
    CB_CHECK_EQ(c.r().f, fl::H | fl::N | fl::Y | fl::X);
    Rig d("37 3F");   // SCF then CCF: the old carry goes to H, the carry is toggled off
    d.step(2);
    CB_CHECK_EQ(d.r().f, fl::H);
});

CB_TEST("emulate.z80.loads_and_exchanges", [] {
    Rig r("01 34 12 11 78 56 21 BC 9A EB 08 D9");   // LD BC,$1234 ; LD DE,$5678 ; LD HL,$9ABC ; EX DE,HL ; EX AF,AF' ; EXX
    r.step(3);
    CB_CHECK_EQ(r.r().bc(), 0x1234);
    CB_CHECK_EQ(r.r().de(), 0x5678);
    CB_CHECK_EQ(r.r().hl(), 0x9ABC);
    r.step();
    CB_CHECK_EQ(r.r().de(), 0x9ABC);
    CB_CHECK_EQ(r.r().hl(), 0x5678);
    r.r().a = 7;
    r.r().a2 = 9;
    r.step();
    CB_CHECK_EQ(r.r().a, 9);
    CB_CHECK_EQ(r.r().a2, 7);
    r.step();   // EXX swaps BC DE HL with the alternates (zeros)
    CB_CHECK_EQ(r.r().bc(), 0);
    CB_CHECK_EQ(r.r().b2, 0x12);
    CB_CHECK_EQ(r.r().hl(), 0);
    CB_CHECK_EQ(r.r().h2, 0x56);
});

CB_TEST("emulate.z80.memory_loads_and_stores", [] {
    Rig r("21 00 90 36 5A 7E 32 10 90 3A 10 90 22 20 90 2A 20 90 02");
    // LD HL,$9000 ; LD (HL),$5A ; LD A,(HL) ; LD ($9010),A ; LD A,($9010) ; LD ($9020),HL ; LD HL,($9020) ; LD (BC),A
    r.step(2);
    CB_CHECK_EQ(r.bus.memory()[0x9000], 0x5A);
    r.step();
    CB_CHECK_EQ(r.r().a, 0x5A);
    r.step();
    CB_CHECK_EQ(r.bus.memory()[0x9010], 0x5A);
    r.r().a = 0;
    r.step();
    CB_CHECK_EQ(r.r().a, 0x5A);
    r.step();   // 16-bit store: low byte first
    CB_CHECK_EQ(r.bus.memory()[0x9020], 0x00);
    CB_CHECK_EQ(r.bus.memory()[0x9021], 0x90);
    r.r().h = r.r().l = 0;
    r.step();
    CB_CHECK_EQ(r.r().hl(), 0x9000);
    r.r().b = 0x91;
    r.r().c = 0x00;
    r.step();
    CB_CHECK_EQ(r.bus.memory()[0x9100], 0x5A);
});

CB_TEST("emulate.z80.stack_and_calls", [] {
    Rig r("31 00 A0 01 CD AB C5 CD 10 80 F1");   // LD SP,$A000 ; LD BC,$ABCD ; PUSH BC ; CALL $8010 ; POP AF
    place(r.bus, 0x8010, "C9");                   // RET
    r.step(2);
    CB_CHECK_EQ(r.r().sp, 0xA000);
    r.step();
    CB_CHECK_EQ(r.r().sp, 0x9FFE);
    CB_CHECK_EQ(r.bus.memory()[0x9FFE], 0xCD);   // low byte at the lower address
    CB_CHECK_EQ(r.bus.memory()[0x9FFF], 0xAB);
    r.step();
    CB_CHECK_EQ(r.r().pc, 0x8010);
    CB_CHECK_EQ(r.r().sp, 0x9FFC);
    CB_CHECK_EQ(r.bus.memory()[0x9FFC], 0x0A);   // the address after the CALL
    CB_CHECK_EQ(r.bus.memory()[0x9FFD], 0x80);
    r.step();
    CB_CHECK_EQ(r.r().pc, 0x800A);
    r.step();   // POP AF
    CB_CHECK_EQ(r.r().a, 0xAB);
    CB_CHECK_EQ(r.r().f, 0xCD);
    CB_CHECK_EQ(r.r().sp, 0xA000);
});

CB_TEST("emulate.z80.conditional_flow", [] {
    struct Case {
        const char* code;
        std::uint8_t f;
        std::uint16_t pc;   // after one instruction
    };
    const Case cases[] = {
        {"C2 34 12", 0, 0x1234},          {"C2 34 12", fl::Z, 0x8003},      {"CA 34 12", fl::Z, 0x1234},
        {"D2 34 12", 0, 0x1234},          {"DA 34 12", fl::C, 0x1234},      {"E2 34 12", 0, 0x1234},
        {"EA 34 12", fl::P, 0x1234},      {"F2 34 12", 0, 0x1234},          {"FA 34 12", fl::S, 0x1234},
        {"18 05", 0, 0x8007},             {"20 05", fl::Z, 0x8002},         {"20 05", 0, 0x8007},
        {"28 FE", fl::Z, 0x8000},         {"30 05", fl::C, 0x8002},         {"38 05", fl::C, 0x8007},
        {"E9", 0, 0x0000},                {"C7", 0, 0x0000},                {"FF", 0, 0x0038},
    };
    for (const Case& c : cases) {
        Rig r(c.code);
        r.r().f = c.f;
        r.r().sp = 0xA000;
        r.step();
        CB_CHECK_EQ(r.r().pc, c.pc);
    }
    Rig d("10 FE");   // DJNZ to itself
    d.r().b = 3;
    d.step();
    CB_CHECK_EQ(d.r().b, 2);
    CB_CHECK_EQ(d.r().pc, 0x8000);
    d.step(2);
    CB_CHECK_EQ(d.r().b, 0);
    CB_CHECK_EQ(d.r().pc, 0x8002);   // falls through when B reaches 0
    Rig f("C4 00 90");   // CALL NZ taken
    f.r().sp = 0xA000;
    f.step();
    CB_CHECK_EQ(f.r().pc, 0x9000);
    CB_CHECK_EQ(f.r().sp, 0x9FFE);
    Rig g("CC 00 90");   // CALL Z not taken
    g.r().sp = 0xA000;
    g.step();
    CB_CHECK_EQ(g.r().pc, 0x8003);
    CB_CHECK_EQ(g.r().sp, 0xA000);
});

CB_TEST("emulate.z80.rotate_shift_and_bit", [] {
    Rig a("06 81 CB 00 CB 08 CB 20 CB 38 CB 30");   // LD B,$81 ; RLC B ; RRC B ; SLA B ; SRL B ; SLL B
    a.step(2);
    CB_CHECK_EQ(a.r().b, 0x03);   // RLC
    CB_CHECK_EQ(a.r().f, fl::P | fl::C);
    a.step();
    CB_CHECK_EQ(a.r().b, 0x81);   // RRC
    a.step();
    CB_CHECK_EQ(a.r().b, 0x02);   // SLA
    CB_CHECK(a.r().f & fl::C);
    a.step();
    CB_CHECK_EQ(a.r().b, 0x01);   // SRL
    a.step();
    CB_CHECK_EQ(a.r().b, 0x03);   // SLL (undocumented): shifts a 1 in
    Rig b("CB 7E CB 46 CB C6 CB 86");   // BIT 7,(HL) ; BIT 0,(HL) ; SET 0,(HL) ; RES 0,(HL)
    b.r().h = 0x90;
    b.r().l = 0x00;
    b.bus.memory()[0x9000] = 0x80;
    b.step();   // bit 7 is set: Z clear, S set, H set
    CB_CHECK_EQ(b.r().f, fl::S | fl::H);
    b.step();   // bit 0 is clear: Z and P set
    CB_CHECK_EQ(b.r().f, fl::Z | fl::P | fl::H);
    b.step();
    CB_CHECK_EQ(b.bus.memory()[0x9000], 0x81);
    b.step();
    CB_CHECK_EQ(b.bus.memory()[0x9000], 0x80);
    Rig c("37 CB 17");   // SCF ; RL A
    c.r().a = 0x80;
    c.step(2);
    CB_CHECK_EQ(c.r().a, 0x01);
    CB_CHECK(c.r().f & fl::C);
    Rig d("CB 2F");      // SRA A keeps the sign
    d.r().a = 0x81;
    d.step();
    CB_CHECK_EQ(d.r().a, 0xC0);
});

CB_TEST("emulate.z80.index_registers", [] {
    Rig r("DD 21 00 90 DD 36 05 42 DD 7E 05 FD 21 06 90 FD 77 FA DD 34 05");
    // LD IX,$9000 ; LD (IX+5),$42 ; LD A,(IX+5) ; LD IY,$9006 ; LD (IY-6),A ; INC (IX+5)
    r.step(2);
    CB_CHECK_EQ(r.bus.memory()[0x9005], 0x42);
    r.step();
    CB_CHECK_EQ(r.r().a, 0x42);
    r.step(2);
    CB_CHECK_EQ(r.bus.memory()[0x9000], 0x42);   // IY - 6
    r.step();
    CB_CHECK_EQ(r.bus.memory()[0x9005], 0x43);
    CB_CHECK_EQ(r.r().pc, 0x8000 + 4 + 4 + 3 + 4 + 3 + 3);
});

CB_TEST("emulate.z80.undocumented_index_halves_and_copies", [] {
    Rig r("DD 26 12 DD 2E 34 DD 7C DD 85");   // LD IXH,$12 ; LD IXL,$34 ; LD A,IXH ; ADD A,IXL
    r.step(2);
    CB_CHECK_EQ(r.r().ix, 0x1234);
    r.step();
    CB_CHECK_EQ(r.r().a, 0x12);
    r.step();
    CB_CHECK_EQ(r.r().a, 0x46);
    // DDCB: RLC (IX+5),B also copies the result into B (undocumented)
    Rig s("DD CB 05 00");
    s.r().ix = 0x9000;
    s.bus.memory()[0x9005] = 0x81;
    s.step();
    CB_CHECK_EQ(s.bus.memory()[0x9005], 0x03);
    CB_CHECK_EQ(s.r().b, 0x03);
    // SET 7,(IX+d),C
    Rig t("DD CB 01 F9");
    t.r().ix = 0x9000;
    t.step();
    CB_CHECK_EQ(t.bus.memory()[0x9001], 0x80);
    CB_CHECK_EQ(t.r().c, 0x80);
    // BIT n,(IX+d): bits 5 and 3 of the flags come from the address's high byte
    Rig u("DD CB 00 46");
    u.r().ix = 0x2800;
    u.step();
    CB_CHECK_EQ(u.r().f, fl::Z | fl::P | fl::H | fl::Y | fl::X);
});

CB_TEST("emulate.z80.block_instructions", [] {
    Rig a("21 00 90 11 00 91 01 04 00 ED B0");   // LD HL,$9000 ; LD DE,$9100 ; LD BC,4 ; LDIR
    for (int i = 0; i < 4; ++i) a.bus.memory()[0x9000 + i] = static_cast<std::uint8_t>(10 + i);
    a.step(3);
    a.step(4);   // one LDIR instruction runs once per byte
    CB_CHECK_EQ(a.r().bc(), 0);
    CB_CHECK_EQ(a.r().hl(), 0x9004);
    CB_CHECK_EQ(a.r().de(), 0x9104);
    CB_CHECK_EQ(a.r().pc, 0x800B);
    for (int i = 0; i < 4; ++i) CB_CHECK_EQ(a.bus.memory()[0x9100 + i], 10 + i);
    CB_CHECK(!(a.r().f & fl::P));   // BC = 0 at the end
    Rig b("21 04 90 11 0A 91 01 02 00 ED B8");   // LDDR copies downwards
    b.bus.memory()[0x9004] = 1;
    b.bus.memory()[0x9003] = 2;
    b.step(3);
    b.step(2);
    CB_CHECK_EQ(b.bus.memory()[0x910A], 1);
    CB_CHECK_EQ(b.bus.memory()[0x9109], 2);
    CB_CHECK_EQ(b.r().hl(), 0x9002);
    Rig c("21 00 90 01 05 00 3E 07 ED B1");   // CPIR: A = 7 searched for through 9000..
    c.bus.memory()[0x9000] = 1;
    c.bus.memory()[0x9001] = 7;
    c.step(3);
    c.step(2);   // finds it at the second byte
    CB_CHECK_EQ(c.r().hl(), 0x9002);
    CB_CHECK_EQ(c.r().bc(), 3);
    CB_CHECK(c.r().f & fl::Z);
    CB_CHECK(c.r().f & fl::P);   // BC is not 0 yet
    CB_CHECK_EQ(c.r().pc, 0x800A);
});

CB_TEST("emulate.z80.rld_and_rrd", [] {
    Rig a("21 00 90 ED 6F");   // RLD
    a.r().a = 0x7A;
    a.bus.memory()[0x9000] = 0x31;
    a.step(2);
    CB_CHECK_EQ(a.bus.memory()[0x9000], 0x1A);
    CB_CHECK_EQ(a.r().a, 0x73);
    Rig b("21 00 90 ED 67");   // RRD
    b.r().a = 0x84;
    b.bus.memory()[0x9000] = 0x20;
    b.step(2);
    CB_CHECK_EQ(b.bus.memory()[0x9000], 0x42);
    CB_CHECK_EQ(b.r().a, 0x80);
});

CB_TEST("emulate.z80.ld_a_i_and_r", [] {
    Rig a("ED 57 ED 5F");
    a.r().i = 0x80;
    a.r().iff2 = true;
    a.step();   // LD A,I: S from the value, P/V is IFF2
    CB_CHECK_EQ(a.r().a, 0x80);
    CB_CHECK_EQ(a.r().f, fl::S | fl::P);
    a.step();   // LD A,R: R counts fetches: two for each ED instruction, and this one has counted itself when it reads R
    CB_CHECK_EQ(a.r().a, 4);
});

CB_TEST("emulate.z80.r_counts_fetches_and_keeps_bit_7", [] {
    Rig a("00 DD 21 00 00 CB 00 ED 44");   // NOP ; LD IX,0 ; RLC B ; NEG: one fetch, then two for each prefixed instruction
    a.r().r = 0x80;
    a.step();
    CB_CHECK_EQ(a.r().r, 0x81);
    a.step();
    CB_CHECK_EQ(a.r().r, 0x83);
    a.step();
    CB_CHECK_EQ(a.r().r, 0x85);
    a.step();
    CB_CHECK_EQ(a.r().r, 0x87);
    Rig b("00 00");
    b.r().r = 0x7F;
    b.step();   // the low seven bits wrap, bit 7 stays
    CB_CHECK_EQ(b.r().r, 0x00);
    b.r().r = 0xFF;
    b.step();
    CB_CHECK_EQ(b.r().r, 0x80);
});

CB_TEST("emulate.z80.ports", [] {
    std::vector<std::pair<int, int>> out_calls;
    z::Bus bus(z::Bus::PortIn([](std::uint16_t port) -> std::uint8_t { return static_cast<std::uint8_t>(port >> 8 == 0x12 ? 0x55 : 0x66); }),
               [&](std::uint16_t port, std::uint8_t v) { out_calls.emplace_back(port, v); });
    place(bus, 0x8000, "3E 12 DB 34 06 12 0E 99 ED 78 ED 41 D3 40");   // LD A,$12 ; IN A,($34) ; LD B,$12 ; LD C,$99 ; IN A,(C) ; OUT (C),B ; OUT ($40),A
    z::Cpu cpu(bus);
    cpu.regs.pc = 0x8000;
    for (int i = 0; i < 7; ++i) CB_CHECK(!cpu.step());
    // IN A,(n): the port is A:n = $1234 so the callback sees the whole 16 bits; the log keeps the low byte
    CB_CHECK_EQ(bus.log().size(), 4u);
    CB_CHECK(bus.log()[0].kind == z::BusAccess::Kind::In);
    CB_CHECK_EQ(bus.log()[0].where, 0x34);
    CB_CHECK_EQ(bus.log()[0].value, 0x55);
    CB_CHECK_EQ(bus.log()[1].where, 0x99);   // IN A,(C): BC = $1299
    CB_CHECK_EQ(bus.log()[1].value, 0x55);
    CB_CHECK(bus.log()[2].kind == z::BusAccess::Kind::Out);
    CB_CHECK_EQ(bus.log()[2].value, 0x12);
    CB_CHECK_EQ(out_calls.size(), 2u);
    CB_CHECK_EQ(out_calls[0].first, 0x1299);
    CB_CHECK_EQ(out_calls[0].second, 0x12);
    CB_CHECK_EQ(out_calls[1].first, 0x5540);   // OUT (n),A: A (now 0x55) is the high byte
    CB_CHECK_EQ(out_calls[1].second, 0x55);
    // IN r,(C) sets flags from the value (0x55: four ones, even parity) and keeps the carry
    CB_CHECK_EQ(cpu.regs.a, 0x55);
});

CB_TEST("emulate.z80.in_with_no_callback_reads_ff_and_bare_has_no_ports", [] {
    z::Bus bus;
    place(bus, 0x8000, "DB 10 ED 78");
    z::Cpu cpu(bus);
    cpu.regs.pc = 0x8000;
    CB_CHECK(!cpu.step());
    CB_CHECK_EQ(cpu.regs.a, 0xFF);
    CB_CHECK_EQ(bus.log().size(), 1u);     // an unwired port is still a logged access
    CB_CHECK_EQ(bus.log()[0].value, 0xFF);

    cb::Bytes ram(z::kMemorySize, 0);
    z::Bus bare = z::Bus::bare(ram);
    CB_CHECK_EQ(bare.in(0x10), 0xFF);
    bare.out(0x10, 5);
    CB_CHECK(bare.log().empty());
});

CB_TEST("emulate.z80.otir_and_inir", [] {
    std::vector<int> sent;
    z::Bus bus(z::Bus::PortIn{}, [&](std::uint16_t, std::uint8_t v) { sent.push_back(v); });
    place(bus, 0x8000, "21 00 90 06 03 0E 7F ED B3");   // LD HL,$9000 ; LD B,3 ; LD C,$7F ; OTIR
    bus.memory()[0x9000] = 1;
    bus.memory()[0x9001] = 2;
    bus.memory()[0x9002] = 3;
    z::Cpu cpu(bus);
    cpu.regs.pc = 0x8000;
    for (int i = 0; i < 6; ++i) CB_CHECK(!cpu.step());   // three loads, then the repeat runs once per byte
    CB_CHECK_EQ(sent.size(), 3u);
    CB_CHECK_EQ(sent[0], 1);
    CB_CHECK_EQ(sent[2], 3);
    CB_CHECK_EQ(cpu.regs.b, 0);
    CB_CHECK(cpu.regs.f & fl::Z);
    CB_CHECK_EQ(cpu.regs.hl(), 0x9003);

    z::Bus in_bus([](std::uint16_t) -> std::uint8_t { return 0x42; });
    place(in_bus, 0x8000, "21 00 90 06 02 0E 7F ED B2");   // INIR
    z::Cpu in_cpu(in_bus);
    in_cpu.regs.pc = 0x8000;
    for (int i = 0; i < 5; ++i) CB_CHECK(!in_cpu.step());
    CB_CHECK_EQ(in_bus.memory()[0x9000], 0x42);
    CB_CHECK_EQ(in_bus.memory()[0x9001], 0x42);
});

CB_TEST("emulate.z80.halt_and_flags_of_interrupt_state", [] {
    Rig a("F3 FB ED 5E 76 00");   // DI ; EI ; IM 2 ; HALT
    a.r().iff1 = a.r().iff2 = true;
    a.step();
    CB_CHECK(!a.r().iff1);
    CB_CHECK(!a.r().iff2);
    a.step();
    CB_CHECK(a.r().iff1);
    a.step();
    CB_CHECK_EQ(a.r().im, 2);
    a.step();   // HALT runs ...
    CB_CHECK(a.r().halted);
    const auto fault = a.cpu.step();   // ... and the CPU goes no further
    CB_CHECK(fault.has_value());
    CB_CHECK(fault->kind == z::Fault::Kind::Halt);
    CB_CHECK_EQ(fault->text(), "HALT");
    CB_CHECK_EQ(a.cpu.steps, 4u);
});

CB_TEST("emulate.z80.self_modifying_code_is_decoded_again", [] {
    // 8000: LD A,1 ; 8002: LD HL,$8001 ; 8005: INC (HL) ; 8006: JP $8000
    Rig r("3E 01 21 01 80 34 C3 00 80");
    r.step();
    CB_CHECK_EQ(r.r().a, 1);
    r.step(3);
    CB_CHECK_EQ(r.r().pc, 0x8000);
    r.step();   // the byte under LD A,n changed: 2
    CB_CHECK_EQ(r.r().a, 2);
    r.step(4);   // LD HL ; INC (HL) ; JP ; LD A,3
    CB_CHECK_EQ(r.r().a, 3);
});

CB_TEST("emulate.z80.every_instruction_of_the_table_runs", [] {
    namespace d = cb::decompile::z80;
    struct Form {
        d::Page page;
        const char* prefix;
    };
    const Form forms[] = {{d::Page::Main, ""},         {d::Page::CB, "CB"},         {d::Page::ED, "ED"},          {d::Page::DD, "DD"},
                          {d::Page::FD, "FD"},         {d::Page::DDCB, "DD CB 01"}, {d::Page::FDCB, "FD CB 01"}};
    int ran = 0;
    for (const Form& form : forms) {
        for (int op = 0; op < 256; ++op) {
            if (d::pattern_of(form.page, static_cast<std::uint8_t>(op)).empty()) continue;
            if (form.page == d::Page::Main && (op == 0xCB || op == 0xDD || op == 0xED || op == 0xFD)) continue;
            Rig r(std::string(form.prefix) + " " + (op < 16 ? "0" : "") +
                  std::format("{:x}", op) + " 05 80");
            r.r().sp = 0xA000;
            r.r().ix = r.r().iy = 0x9000;
            r.r().h = 0x90;
            r.r().l = 0x10;
            r.r().b = 0x90;
            r.r().c = 0x20;
            r.r().d = 0x91;
            r.r().e = 0x00;
            CB_CHECK(!r.cpu.step());
            ++ran;
        }
    }
    CB_CHECK(ran > 1100);
});

// ---- the bus ----------------------------------------------------------------------------------------------------

CB_TEST("emulate.z80.bus_ranges_and_log", [] {
    z::Bus bus;
    std::vector<std::pair<int, int>> written;
    bus.map(0xFF00, 0xFF0F, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a + 1); },
            [&](std::uint16_t a, std::uint8_t v) { written.emplace_back(a, v); });
    bus.map(0xFE00, 0xFE00);   // no callbacks: reads 0, swallows writes
    bus.write(0x10, 7);
    CB_CHECK_EQ(bus.read(0x10), 7);
    CB_CHECK(bus.log().empty());
    CB_CHECK_EQ(bus.read(0xFF03), 4);
    bus.write(0xFF04, 9);
    bus.write(0xFE00, 1);
    CB_CHECK_EQ(bus.read(0xFE00), 0);
    CB_CHECK_EQ(written.size(), 1u);
    CB_CHECK_EQ(written[0].first, 0xFF04);
    CB_CHECK_EQ(bus.log().size(), 4u);
    CB_CHECK(bus.log()[0].kind == z::BusAccess::Kind::Read);
    CB_CHECK(bus.log()[1].kind == z::BusAccess::Kind::Write);
    CB_CHECK_EQ(z::to_text(z::BusAccess::Kind::In), "in");
    CB_CHECK_EQ(z::to_text(z::BusAccess::Kind::Write), "w");
});

CB_TEST("emulate.z80.bus_memory_ownership", [] {
    cb::Bytes ram(z::kMemorySize, 0);
    {
        z::Bus bus = z::Bus::over(ram);
        bus.write(0x4321, 0x99);
    }
    CB_CHECK_EQ(ram[0x4321], 0x99);   // in place
    cb::Bytes small(10, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)z::Bus::over(small));
    cb::Bytes big(z::kMemorySize + 1, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)z::Bus(big));
    const cb::Bytes image = {9, 8, 7};
    z::Bus own(image);
    CB_CHECK_EQ(own.read(1), 8);
    z::Bus moved = std::move(own);
    CB_CHECK_EQ(moved.read(2), 7);
});

// ---- running a function ------------------------------------------------------------------------------------------

CB_TEST("emulate.z80.run_a_function", [] {
    // 16-bit sum of an array: HL = pointer, B = count; returns DE = sum
    z::Bus bus;
    place(bus, 0x8000, "11 00 00 7E 83 5F 30 01 14 23 10 F7 C9");
    // LD DE,0 ; loop: LD A,(HL) ; ADD A,E ; LD E,A ; JR NC,+1 ; INC D ; INC HL ; DJNZ loop ; RET
    for (int i = 0; i < 4; ++i) bus.memory()[0x9000 + i] = 100;
    z::RunOptions o;
    o.regs.h = std::uint8_t{0x90};
    o.regs.l = std::uint8_t{0x00};
    o.regs.b = std::uint8_t{4};
    const z::RunResult r = z::run(bus, 0x8000, o);
    CB_CHECK(r.end == z::End::Return);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.de(), 400);
    CB_CHECK_EQ(r.regs.sp, 0xDFF0);   // the return address was pushed below it and popped again
    CB_CHECK(r.io.empty());
});

CB_TEST("emulate.z80.run_ends", [] {
    z::Bus spin;
    place(spin, 0x8000, "18 FE");   // JR to itself
    const z::RunResult limit = z::run(spin, 0x8000, {.max_steps = 25});
    CB_CHECK(limit.end == z::End::Limit);
    CB_CHECK_EQ(limit.steps, 25u);
    CB_CHECK_EQ(limit.exit(), "limit");

    z::Bus halt;
    place(halt, 0x8000, "76");
    const z::RunResult stopped = z::run(halt, 0x8000);
    CB_CHECK(stopped.end == z::End::Stopped);
    CB_CHECK_EQ(stopped.exit(), "stopped: HALT");
    CB_CHECK_EQ(stopped.steps, 1u);

    z::Bus none;
    const z::RunResult at_once = z::run(none, z::kReturnAddress);
    CB_CHECK(at_once.end == z::End::Return);
    CB_CHECK_EQ(at_once.steps, 0u);

    z::Bus sp_bus;
    place(sp_bus, 0x8000, "C9");
    z::RunOptions o;
    o.sp = 0x4000;
    const z::RunResult r = z::run(sp_bus, 0x8000, o);
    CB_CHECK_EQ(r.regs.sp, 0x4000);
    CB_CHECK_EQ(sp_bus.memory()[0x3FFE], 0xFF);   // the return address went to sp - 2
    CB_CHECK_EQ(sp_bus.memory()[0x3FFF], 0xFF);
});

CB_TEST("emulate.z80.run_registers_in", [] {
    z::Bus bus;
    place(bus, 0x8000, "C9");
    z::RunOptions o;
    o.regs.a = std::uint8_t{1};
    o.regs.f = std::uint8_t{2};
    o.regs.b = std::uint8_t{3};
    o.regs.c = std::uint8_t{4};
    o.regs.d = std::uint8_t{5};
    o.regs.e = std::uint8_t{6};
    o.regs.h = std::uint8_t{7};
    o.regs.l = std::uint8_t{8};
    o.regs.ix = std::uint16_t{0x1234};
    o.regs.iy = std::uint16_t{0x5678};
    const z::RunResult r = z::run(bus, 0x8000, o);
    CB_CHECK_EQ(r.regs.a, 1);
    CB_CHECK_EQ(r.regs.f, 2);
    CB_CHECK_EQ(r.regs.bc(), 0x0304);
    CB_CHECK_EQ(r.regs.de(), 0x0506);
    CB_CHECK_EQ(r.regs.hl(), 0x0708);
    CB_CHECK_EQ(r.regs.ix, 0x1234);
    CB_CHECK_EQ(r.regs.iy, 0x5678);
    const z::RunResult defaults = z::run(bus, 0x8000);
    CB_CHECK_EQ(defaults.regs.ix, 0xFFFF);   // the chip's reset values
    CB_CHECK_EQ(defaults.regs.iy, 0xFFFF);
});

CB_TEST("emulate.z80.record", [] {
    z::Bus bus;
    bus.map(0xC000, 0xC000, [](std::uint16_t) -> std::uint8_t { return 0x3C; });
    place(bus, 0x8000, "3A 00 C0 D3 10 C9");   // LD A,($C000) ; OUT ($10),A ; RET
    const cb::emulate::Run run = z::recorded(bus, 0x8000);
    CB_CHECK_EQ(run.ended(), "return");
    CB_CHECK(!run.truncated);
    CB_CHECK(run.endian == cb::emulate::Endian::Little);
    CB_CHECK_EQ(run.steps.value_or(0), 3u);
    std::string order;
    for (const auto& kv : run.results) order += kv.first + ",";
    CB_CHECK_EQ(order, "A,F,BC,DE,HL,IX,IY,SP,");
    CB_CHECK_EQ(std::get<std::uint64_t>(run.results.at("A")), 0x3C);
    CB_CHECK_EQ(std::get<std::uint64_t>(run.results.at("SP")), 0xDFF0);
    CB_CHECK_EQ(run.io.size(), 2u);
    CB_CHECK_EQ(run.io[0].kind, "r");
    CB_CHECK_EQ(run.io[0].port, 0xC000);
    CB_CHECK_EQ(run.io[0].value, 0x3C);
    CB_CHECK_EQ(run.io[1].kind, "out");
    CB_CHECK_EQ(run.io[1].port, 0x10);
    CB_CHECK_EQ(run.io[1].value, 0x3C);
    CB_CHECK_EQ(std::any_cast<const z::RunResult&>(run.raw).steps, 3u);

    z::Bus spin;
    place(spin, 0x8000, "18 FE");
    const cb::emulate::Run cut = z::recorded(spin, 0x8000, {.max_steps = 4});
    CB_CHECK(cut.truncated);
    CB_CHECK_EQ(cut.ended(), "limit");
});
