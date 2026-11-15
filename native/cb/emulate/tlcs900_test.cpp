// cb-unity: alone
// The TLCS-900/H interpreter: flags, the register banks, every addressing mode, multiply and divide, shifts, block moves, the stack, the BIOS stub and the device log. Programs are
// written in the assembler's own syntax.
#include "cb/emulate/tlcs900.hpp"

#include <string>

#include "cb/build/tlcs900_asm.hpp"
#include "cb/core/testing.hpp"

namespace cb::emulate::tlcs900 {
namespace {

constexpr std::uint32_t kCode = 0x200100;
constexpr std::uint32_t kRam = 0x004000;

struct Machine {
    Bus bus;
    Bytes rom;
    // `name:` lines define labels
    explicit Machine(const std::vector<std::string>& lines) {
        std::string source = "        ORG 0x200100\n";
        for (const std::string& l : lines) source += l.ends_with(':') ? l + "\n" : "        " + l + "\n";
        rom = build::tlcs900_asm::build(source).data;
        rom.insert(rom.begin(), 0x100, 0);
        rom.resize(0x400, 0);
        bus.memory(0x004000, 0x006FFF, {}, true, false);
        bus.device(0x000000, 0x0000FF, {}, {}, true);
        bus.memory(0x200000, 0x2003FF, rom, false, false);
    }
    RunResult run(RunOptions o = {}) { return tlcs900::run(bus, kCode, o); }
};

std::uint32_t xwa(const RunResult& r) { return r.regs.bank[3][0]; }
std::uint32_t xbc(const RunResult& r) { return r.regs.bank[3][1]; }
std::uint32_t xde(const RunResult& r) { return r.regs.bank[3][2]; }
std::uint32_t xhl(const RunResult& r) { return r.regs.bank[3][3]; }
std::uint8_t f(const RunResult& r) { return r.regs.f(); }

}  // namespace

CB_TEST("emulate.tlcs900.the_flags_of_the_arithmetic", [] {
    Machine m({"ld A,0x7F", "add A,1", "ld B,0xFF", "ld C,A", "ret"});
    const RunResult r = m.run();
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(xwa(r) & 0xFF, 0x80u);
    CB_CHECK((f(r) & (flag::S | flag::V | flag::H)) == (flag::S | flag::V | flag::H));   // (0x7F + 1: overflow, half carry from bit 3)
    Machine sub({"ld WA,0x0001", "sub WA,2", "ret"});
    const RunResult s = sub.run();
    CB_CHECK_EQ(xwa(s) & 0xFFFF, 0xFFFFu);
    CB_CHECK((f(s) & (flag::S | flag::C | flag::N)) == (flag::S | flag::C | flag::N));
    Machine cmp({"ld XWA,0x80000000", "cp XWA,1", "ret"});
    const RunResult c = cmp.run();
    CB_CHECK((f(c) & flag::V) != 0 && (f(c) & flag::N) != 0 && (f(c) & flag::S) == 0);   // (the signed overflow of a 32-bit compare)
    Machine logic({"ld A,0x0F", "and A,0x33", "ld B,0x01", "or B,0x02", "ld C,0xFF", "xor C,0xFF", "ret"});
    const RunResult l = logic.run();
    CB_CHECK_EQ(xwa(l) & 0xFF, 0x03u);
    CB_CHECK_EQ((xbc(l) >> 8) & 0xFF, 0x03u);
    CB_CHECK_EQ(xbc(l) & 0xFF, 0x00u);
    CB_CHECK((f(l) & flag::Z) != 0 && (f(l) & flag::V) != 0);   // (xor gave zero: parity even)
    Machine adc({"scf", "ld A,5", "adc A,5", "ld XBC,0", "sbc B,0", "ret"});
    const RunResult a = adc.run();
    CB_CHECK_EQ(xwa(a) & 0xFF, 11u);
});

CB_TEST("emulate.tlcs900.register_banks_and_parts", [] {
    // the program starts in bank 3; incf goes to bank 0, decf back
    Machine m({"ld XWA,0x11223344", "incf", "ld XWA,0x55", "ld XBC,0x66", "decf", "ld QA,0x77", "ld W,0xAB", "ld IXL,0xCD", "ld XIX,0xAABBCCDD", "ld IXH,0x01",
               "ld RA2,0x99", "ld XDE',0x7777", "ld XWA3,0x1234", "ld A,5", "ret"});
    const RunResult r = m.run();
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.bank[0][0], 0x55u);
    CB_CHECK_EQ(r.regs.bank[0][1], 0x66u);
    CB_CHECK_EQ(r.regs.bank[2][0], 0x99u);
    CB_CHECK_EQ(r.regs.bank[2][2], 0x7777u);   // (XDE' is the previous bank's: bank 2 when the current one is 3)
    CB_CHECK_EQ(r.regs.bank[3][0], 0x1205u);
    CB_CHECK_EQ(r.regs.xix, 0xAABB01DDu);
});

CB_TEST("emulate.tlcs900.memory_in_every_addressing_mode", [] {
    Machine m({"ld XBC,0x4000", "ld (XBC),0x11", "ld (XBC+1),0x22", "ldw (XBC+2),0x4433", "ld XIX,0x4010", "ld A,0x55", "ld (XIX+),A", "ld (XIX+),A", "ld XIY,0x4020", "ld (-XIY),A",
               "ld E,2", "ld A,(XBC+E)", "ld (0x4030),A", "ld (0x004031),A", "ld WA,(XBC+2)", "ld (0x4040),WA", "ld XHL,0x4000", "ld XWA,(XHL)", "ld (0x4050),XWA",
               "lda XDE,(XBC+8)", "ret"});
    const RunResult r = m.run();
    CB_CHECK_EQ(r.exit(), "return");
    const auto ram = [&](std::uint32_t a) { return m.bus.bytes(0x004000)[a - 0x4000]; };
    CB_CHECK_EQ(ram(0x4000), 0x11);
    CB_CHECK_EQ(ram(0x4001), 0x22);
    CB_CHECK_EQ(ram(0x4002), 0x33);
    CB_CHECK_EQ(ram(0x4003), 0x44);
    CB_CHECK_EQ(ram(0x4010), 0x55);
    CB_CHECK_EQ(ram(0x4011), 0x55);
    CB_CHECK_EQ(r.regs.xix, 0x4012u);
    CB_CHECK_EQ(ram(0x401F), 0x55);   // (pre-decrement by 1 from 0x4020)
    CB_CHECK_EQ(r.regs.xiy, 0x401Fu);
    CB_CHECK_EQ(ram(0x4030), 0x33);   // (A was loaded from (XDE+B) = 0x4002)
    CB_CHECK_EQ(ram(0x4031), 0x33);
    CB_CHECK_EQ(xwa(r), 0x44332211u);
    CB_CHECK_EQ(ram(0x4040), 0x33);
    CB_CHECK_EQ(ram(0x4041), 0x44);
    CB_CHECK_EQ(ram(0x4050), 0x11);
    CB_CHECK_EQ(xde(r), 0x4008u);
});

CB_TEST("emulate.tlcs900.loops_calls_the_stack_and_link", [] {
    // sum 5+4+3+2+1 in A with djnz, then call sub, which keeps a frame with link / unlk and doubles A
    Machine m({"ld XWA,0", "ld C,5", "top:", "add A,C", "djnz C,top", "call sub", "push XWA", "pop XHL", "ret", "sub:", "push XIX", "link XIZ,-8", "ld (XIZ-4),A", "add A,A", "unlk XIZ", "pop XIX", "ret"});
    RunOptions o;
    o.xiz = 0x1234;
    o.xix = 0x5678;
    const RunResult r = m.run(o);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(xwa(r) & 0xFF, 30u);
    CB_CHECK_EQ(xhl(r), xwa(r));
    CB_CHECK_EQ(r.regs.xiz, 0x1234u);   // (unlk put the old value back)
    CB_CHECK_EQ(r.regs.xix, 0x5678u);
    CB_CHECK_EQ(r.regs.xsp, 0x006C00u);
});

CB_TEST("emulate.tlcs900.a_function_is_called_with_registers_and_stack_arguments", [] {
    // inc 3 words of the arguments: add the first stack argument (WA) and BC
    Machine m({"ld WA,(XSP+4)", "add WA,BC", "ret"});
    RunOptions o;
    o.xbc = 0x0010;
    o.args = {0x0005};
    const RunResult r = m.run(o);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(xwa(r) & 0xFFFF, 0x0015u);
});

CB_TEST("emulate.tlcs900.multiply_divide_and_the_shifts", [] {
    Machine m({"ld A,0x10", "ld C,0x12", "mul WA,C", "ld XBC,0", "ld WA,0x0123", "ld DE,0x0010", "div XWA,DE", "ret"});
    (void)m;
    Machine mul({"ld WA,0x0007", "ld C,6", "ld A,7", "mul WA,C", "ret"});
    const RunResult r = mul.run();
    CB_CHECK_EQ(xwa(r) & 0xFFFF, 42u);
    Machine div({"ld XWA,100", "ld BC,7", "div XWA,BC", "ret"});
    const RunResult d = div.run();
    CB_CHECK_EQ(xwa(d), (2u << 16) | 14u);   // (quotient 14 in the low half, remainder 2 in the high half)
    Machine zero({"ld XWA,100", "ld BC,0", "div XWA,BC", "ret"});
    CB_CHECK((f(zero.run()) & flag::V) != 0);
    Machine sh({"ld A,0x81", "rlc 1,A", "ld B,0x81", "srl 1,B", "ld C,0x80", "sra 2,C", "ld WA,3", "sla 4,WA", "ret"});
    const RunResult s = sh.run();
    CB_CHECK_EQ(xbc(s) & 0xFF, 0xE0u);   // (sra 0x80 by 2)
    CB_CHECK_EQ((xbc(s) >> 8) & 0xFF, 0x40u);
});

CB_TEST("emulate.tlcs900.block_moves_and_compares_and_the_bios_stub", [] {
    Machine m({"ld XHL,0x4000", "ld XDE,0x4100", "ld BC,4", "ldir (XDE+),(XHL+)", "ld XHL,0x4100", "ld BC,8", "ld A,0x03", "cpir A,(XHL+)", "ret"});
    for (unsigned k = 0; k < 8; ++k) m.bus.write(0x4000 + k, 1, k);
    const RunResult r = m.run();
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(m.bus.read(0x4103, 1), 3u);
    CB_CHECK((f(r) & flag::Z) != 0);
    CB_CHECK_EQ(xhl(r), 0x4104u);
    CB_CHECK_EQ(xbc(r) & 0xFFFF, 4u);
    Machine bios({"ld RW3,5", "ld XBC,0x1234", "swi 1", "ld A,1", "ret"});
    const RunResult b = bios.run();
    CB_CHECK_EQ(b.exit(), "return");
    CB_CHECK_EQ(b.swis.size(), 1u);
    CB_CHECK_EQ(b.swis[0].number, 1u);
    CB_CHECK_EQ(b.swis[0].function, 5u);
    CB_CHECK_EQ(b.swis[0].regs[1], 0x1234u);
});

CB_TEST("emulate.tlcs900.devices_are_logged_and_a_bad_instruction_stops_the_run", [] {
    Machine m({"ld (0x6F),0x4E", "ld A,(0x20)", "ret"});
    const RunResult r = m.run();
    CB_CHECK_EQ(r.io.size(), 2u);
    CB_CHECK(r.io[0].kind == DeviceAccess::Kind::Write && r.io[0].address == 0x6F && r.io[0].value == 0x4E);
    CB_CHECK(r.io[1].kind == DeviceAccess::Kind::Read && r.io[1].address == 0x20);
    Machine bad({"nop", "halt"});
    const RunResult h = bad.run();
    CB_CHECK(h.end == End::Stopped);
    CB_CHECK(h.exit().starts_with("stopped: halt at"));
    Machine undefined({"nop"});
    undefined.rom[0x101] = 0x1F;
    Bus b2;
    b2.memory(0x200000, 0x2003FF, undefined.rom, false);
    const RunResult u = tlcs900::run(b2, kCode);
    CB_CHECK(u.exit().starts_with("stopped: undefined instruction"));
});

CB_TEST("emulate.tlcs900.decimal_adjust_bit_search_conditions_and_the_bit_instructions", [] {
    Machine daa({"ld A,0x19", "add A,0x28", "daa A", "ret"});
    CB_CHECK_EQ(xwa(daa.run()) & 0xFF, 0x47u);
    Machine scc({"ld A,5", "cp A,5", "scc Z,B", "scc NZ,C", "ret"});
    const RunResult s = scc.run();
    CB_CHECK_EQ((xbc(s) >> 8) & 0xFF, 1u);
    CB_CHECK_EQ(xbc(s) & 0xFF, 0u);
    Machine bs({"ld BC,0x0120", "bs1f A,BC", "ld D,A", "bs1b A,BC", "ld E,A", "ret"});
    CB_CHECK_EQ(xde(bs.run()) & 0xFFFF, 0x0508u);
    Machine keep({"ld A,0xFF", "add A,1", "inc 1,BC", "ret"});   // (a word register's inc leaves every flag)
    const RunResult k = keep.run();
    CB_CHECK((f(k) & (flag::Z | flag::C)) == (flag::Z | flag::C));
    CB_CHECK_EQ(xbc(k) & 0xFFFF, 1u);
    Machine minc({"ld BC,2", "minc1 0x0003,BC", "ld DE,BC", "minc1 0x0003,BC", "ret"});
    const RunResult mi = minc.run();
    CB_CHECK_EQ(xde(mi) & 0xFFFF, 3u);
    CB_CHECK_EQ(xbc(mi) & 0xFFFF, 0u);
    Machine bits({"ld A,0x05", "ldcf 2,A", "ld B,0", "stcf 7,B", "set 3,A", "res 0,A", "chg 7,A", "ret"});
    const RunResult b = bits.run();
    CB_CHECK_EQ((xbc(b) >> 8) & 0xFF, 0x80u);
    CB_CHECK_EQ(xwa(b) & 0xFF, 0x8Cu);
    Machine shift({"ld A,3", "ld BC,1", "sla A,BC", "ret"});
    CB_CHECK_EQ(xbc(shift.run()) & 0xFFFF, 8u);
});

CB_TEST("emulate.tlcs900.signed_multiply_and_divide_push_pop_of_memory_and_the_control_registers", [] {
    Machine muls({"ld A,-3", "ld C,2", "muls WA,C", "ret"});
    CB_CHECK_EQ(xwa(muls.run()) & 0xFFFF, 0xFFFAu);
    Machine divs({"ld XWA,-7", "ld BC,2", "divs XWA,BC", "ret"});
    CB_CHECK_EQ(xwa(divs.run()), 0xFFFFFFFDu);
    Machine mem({"ldw (0x4000),0x1234", "pushw (0x4000)", "popw (0x4010)", "ld WA,(0x4010)", "ex WA,BC", "ld DE,0x5678", "ldc DMAC0,DE", "ldc HL,DMAC0", "ret"});
    const RunResult r = mem.run();
    CB_CHECK_EQ(xbc(r) & 0xFFFF, 0x1234u);
    CB_CHECK_EQ(xhl(r) & 0xFFFF, 0x5678u);
    CB_CHECK_EQ(r.regs.xsp, 0x006C00u);
});

}  // namespace cb::emulate::tlcs900
