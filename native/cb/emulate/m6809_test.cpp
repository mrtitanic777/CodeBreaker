// cb-unity: alone
// The 6809 interpreter: flags and results of the arithmetic, the stack, the addressing modes, the device log and the ways a run ends.
#include "cb/emulate/m6809.hpp"

#include "cb/core/testing.hpp"
#include "cb/decompile/m6809.hpp"

namespace cb::emulate::m6809 {
namespace {

namespace dm = decompile::m6809;

// The program at $1000 from lines of source; a run of it from there.
Bus bus_of(std::initializer_list<const char*> lines, std::uint16_t at = 0x1000) {
    Bytes image(0x10000, 0);
    std::uint32_t pc = at;
    for (const char* l : lines) {
        const Bytes b = dm::assemble(l, pc);
        std::copy(b.begin(), b.end(), image.begin() + pc);
        pc += static_cast<std::uint32_t>(b.size());
    }
    return Bus(image);
}

RunResult go(Bus& bus, RunOptions options = {}) { return run(bus, 0x1000, options); }

}  // namespace

CB_TEST("emulate.m6809.arithmetic_and_flags", [] {
    Bus bus = bus_of({"LDA #$7F", "ADDA #$01", "RTS"});
    RunResult r = go(bus);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.a, 0x80);
    CB_CHECK((r.regs.cc & (cc::N | cc::V | cc::Z | cc::C)) == (cc::N | cc::V));   // 127 + 1 overflows
    Bus b2 = bus_of({"LDA #$00", "SUBA #$01", "RTS"});
    r = go(b2);
    CB_CHECK_EQ(r.regs.a, 0xFF);
    CB_CHECK((r.regs.cc & (cc::N | cc::Z | cc::C)) == (cc::N | cc::C));   // a borrow
    Bus b3 = bus_of({"LDB #$10", "LDA #$10", "MUL", "RTS"});
    r = go(b3);
    CB_CHECK_EQ(r.regs.a, 0x01);
    CB_CHECK_EQ(r.regs.b, 0x00);
    Bus b4 = bus_of({"LDD #$1234", "ADDD #$FFFF", "RTS"});
    r = go(b4);
    CB_CHECK_EQ((r.regs.a << 8) | r.regs.b, 0x1233);
    CB_CHECK(r.regs.cc & cc::C);
    Bus b5 = bus_of({"LDA #$99", "ADDA #$01", "DAA", "RTS"});
    r = go(b5);
    CB_CHECK_EQ(r.regs.a, 0x00);   // 99 + 1 in BCD
    CB_CHECK(r.regs.cc & cc::C);
});

CB_TEST("emulate.m6809.shifts_and_memory", [] {
    Bus bus = bus_of({"LDA #$81", "LSLA", "RORA", "STA $C800", "INC $C800", "LDB $C800", "CLR <$20", "RTS"});
    const RunResult r = go(bus);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(bus.memory()[0xC800], 0x82);   // $81 << 1 = $02 (C set), ROR with C = $81, then INC
    CB_CHECK_EQ(r.regs.b, 0x82);
});

CB_TEST("emulate.m6809.indexed_modes_and_the_stack", [] {
    Bus bus = bus_of({"LDX #$C900", "LDA #$11", "STA ,X+", "LDA #$22", "STA ,X++", "LDA #$33", "STA ,-X", "LEAY 5,X", "PSHS A,X", "LDX #$0000", "PULS A,X", "RTS"});
    const RunResult r = go(bus);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(bus.memory()[0xC900], 0x11);
    CB_CHECK_EQ(bus.memory()[0xC901], 0x22);
    CB_CHECK_EQ(bus.memory()[0xC902], 0x33);   // ,-X after ,X++ made X = $C903, then $C902
    CB_CHECK_EQ(r.regs.x, 0xC902);
    CB_CHECK_EQ(r.regs.y, 0xC907);
    CB_CHECK_EQ(r.regs.a, 0x33);
});

CB_TEST("emulate.m6809.calls_branches_and_a_loop", [] {
    // sub at $1010 doubles B; main calls it three times in a loop
    Bytes image(0x10000, 0);
    const char* main_lines[] = {"LDB #$01", "LDA #$03", "JSR $1010", "DECA", "BNE $1004", "RTS"};
    std::uint32_t pc = 0x1000;
    for (const char* l : main_lines) {
        const Bytes b = dm::assemble(l, pc);
        std::copy(b.begin(), b.end(), image.begin() + pc);
        pc += static_cast<std::uint32_t>(b.size());
    }
    const Bytes sub = dm::assemble("ASLB", 0x1010);
    std::copy(sub.begin(), sub.end(), image.begin() + 0x1010);
    image[0x1011] = 0x39;
    Bus bus(image);
    const RunResult r = go(bus);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.b, 0x08);
});

CB_TEST("emulate.m6809.devices_are_logged_and_a_run_ends_in_one_of_three_ways", [] {
    Bus bus = bus_of({"LDA #$7F", "STA $D00E", "LDA $D00D", "RTS"});
    bus.map(0xD000, 0xD7FF, [](std::uint16_t a) -> std::uint8_t { return (a & 0xF) == 0xD ? 0xC0 : 0; });
    RunResult r = go(bus);
    CB_CHECK_EQ(r.regs.a, 0xC0);
    CB_CHECK_EQ(r.io.size(), 2u);
    CB_CHECK(r.io[0].kind == DeviceAccess::Kind::Write && r.io[0].address == 0xD00E && r.io[0].value == 0x7F);
    CB_CHECK(r.io[1].kind == DeviceAccess::Kind::Read && r.io[1].address == 0xD00D);
    Bus loop = bus_of({"BRA $1000"});
    RunOptions few;
    few.max_steps = 50;
    CB_CHECK_EQ(go(loop, few).exit(), "limit");
    Bus bad = bus_of({"NOP"});   // $1001 is zero memory: NEG <$00 runs, but an undefined opcode stops
    bad.memory()[0x1001] = 0x01;
    CB_CHECK(go(bad).exit().starts_with("stopped: undefined opcode 01"));
    Bus waits = bus_of({"SWI"});
    CB_CHECK(go(waits).exit().starts_with("stopped: waits for an interrupt"));
});

}  // namespace cb::emulate::m6809
