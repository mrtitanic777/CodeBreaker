// cb-unity: alone
// The Vectrex lane's 6809 on the one emulator (engine/pcode_cpu_m6809.cpp). The own core (emulate/m6809's interpreter) was retired once the emulator matched it on
// random instructions and programs (400,000 runs) and on the lane's whole homebrew corpus (every function in every state, and the verdicts on edited copies). What the
// default 40,000 random runs gave on both is kept as a digest (CRC-32 of every run's exit, registers, steps, device accesses, memory and RAM), so a change in what the
// emulator does shows. The own core's unit tests follow, on the emulator.
#include <array>
#include <cstdio>
#include <format>
#include <memory>
#include <random>
#include <string>

#include "core/crypto.hpp"
#include "core/testing.hpp"
#include "emulate/m6809.hpp"
#include "engine/pcode_cpu_m6809.hpp"
#include "source/processors.hpp"

using namespace cb;
namespace m = cb::emulate::m6809;

namespace {

std::string describe_6809(const m::RunResult& a, const m::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    const auto eight = [&](const char* name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    eight("a", a.regs.a, b.regs.a);
    eight("b", a.regs.b, b.regs.b);
    eight("dp", a.regs.dp, b.regs.dp);
    eight("cc", a.regs.cc, b.regs.cc);
    eight("x", a.regs.x, b.regs.x);
    eight("y", a.regs.y, b.regs.y);
    eight("u", a.regs.u, b.regs.u);
    eight("s", a.regs.s, b.regs.s);
    eight("pc", a.regs.pc, b.regs.pc);
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) {
                out += std::format("io@{}[own {} {:X} {:X} emu {} {:X} {:X}] ", i, static_cast<int>(a.io[i].kind), a.io[i].address, a.io[i].value, static_cast<int>(b.io[i].kind),
                                   b.io[i].address, b.io[i].value);
                break;
            }
    return out;
}

// A bus as the Vectrex lane makes one: a device range that answers by a counter (the order of reads shows) and RAM behind callbacks.
struct Mc6809World {
    m::Bus bus;
    std::shared_ptr<std::array<std::uint8_t, 0x400>> ram = std::make_shared<std::array<std::uint8_t, 0x400>>();
    std::shared_ptr<unsigned> reads = std::make_shared<unsigned>(0);
    explicit Mc6809World(ByteView image) : bus(image) {
        auto r = ram;
        auto n = reads;
        bus.map_quiet(0xC800, 0xCFFF, [r](std::uint16_t a) { return (*r)[(a - 0xC800) & 0x3FF]; }, [r](std::uint16_t a, std::uint8_t v) { (*r)[(a - 0xC800) & 0x3FF] = v; });
        bus.map(0xD000, 0xD0FF, [n](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 7 + 3 + (*n)++ * 5); }, [](std::uint16_t, std::uint8_t) {});
    }
};

// A run folded into the digest: everything describe_6809 compares (against an empty run), each device access, the memory and the RAM.
std::uint32_t fold_6809(std::uint32_t digest, const m::RunResult& r, Mc6809World& w) {
    std::string text = describe_6809(m::RunResult{}, r);
    for (const auto& x : r.io) text += std::format("|{}{:X}={:X}", static_cast<int>(x.kind), x.address, x.value);
    digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
    digest = crypto::crc32(ByteView(w.bus.memory().data(), w.bus.memory().size()), digest);
    return crypto::crc32(ByteView(w.ram->data(), w.ram->size()), digest);
}

}  // namespace

CB_TEST("engine.pcode_cpu.the_6809_runs_random_programs_as_the_own_core_did", [] {
    std::mt19937 rng(20261122);
    constexpr unsigned total = 20000;
    std::uint32_t digest = 0;
    const auto pointer = [&]() -> std::uint16_t {
        switch (rng() % 6) {
            case 0: return static_cast<std::uint16_t>(0xD000 + (rng() & 0xFF));
            case 1: return static_cast<std::uint16_t>(0xC800 + (rng() & 0x3FF));
            case 2: return static_cast<std::uint16_t>(rng());
            default: return static_cast<std::uint16_t>(0x2000 + (rng() & 0x7FF));
        }
    };
    for (unsigned n = 0; n < 2 * total; ++n) {
        const bool single = n < total;
        Bytes image(0x10000, 0);
        for (std::size_t i = 0x2000; i < 0x3000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        for (std::size_t i = 0x1000; i < 0x1040; ++i) image[i] = static_cast<std::uint8_t>(rng());
        if (single) {
            const std::uint16_t first = static_cast<std::uint16_t>((n * 11) % 0x10000);
            image[0x1000] = static_cast<std::uint8_t>(first >> 8);
            image[0x1001] = static_cast<std::uint8_t>(first);
        }
        m::RunOptions options;
        options.max_steps = single ? 1 : 40;
        options.a = static_cast<std::uint8_t>(rng());
        options.b = static_cast<std::uint8_t>(rng());
        options.dp = (rng() & 1) ? 0x20 : static_cast<std::uint8_t>(rng());
        options.x = pointer();
        options.y = pointer();
        options.u = pointer();
        options.s = (rng() & 3) == 0 ? pointer() : static_cast<std::uint16_t>(0x3800 + (rng() & 0xFF));
        Mc6809World w(image);
        const m::RunResult rr = engine::pcode_cpu::run(w.bus, 0x1000, options);
        digest = fold_6809(digest, rr, w);
    }
    std::printf("pcode_cpu 6809 instructions: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x5B9580D7u);
});

namespace cb::emulate::m6809 {
namespace {

// One instruction of source at `pc`, in the 6809 description's syntax (the one assembler).
Bytes assemble_at(const std::string& line, std::uint32_t pc) {
    return source::processors::assemble_described(source::processors::find("m6809"), std::format(".org 0x{:x}\n        {}\n", pc, line), ".").data;
}

// The program at $1000 from lines of source; a run of it from there.
Bus bus_of(std::initializer_list<const char*> lines, std::uint16_t at = 0x1000) {
    Bytes image(0x10000, 0);
    std::uint32_t pc = at;
    for (const char* l : lines) {
        const Bytes b = assemble_at(l, pc);
        std::copy(b.begin(), b.end(), image.begin() + pc);
        pc += static_cast<std::uint32_t>(b.size());
    }
    return Bus(image);
}

RunResult go(Bus& bus, RunOptions options = {}) { return engine::pcode_cpu::run(bus, 0x1000, options); }

}  // namespace

CB_TEST("engine.pcode_cpu.m6809_arithmetic_and_flags", [] {
    Bus bus = bus_of({"LDA #0x7f", "ADDA #0x1", "RTS"});
    RunResult r = go(bus);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.a, 0x80);
    CB_CHECK((r.regs.cc & (cc::N | cc::V | cc::Z | cc::C)) == (cc::N | cc::V));   // 127 + 1 overflows
    Bus b2 = bus_of({"LDA #0x0", "SUBA #0x1", "RTS"});
    r = go(b2);
    CB_CHECK_EQ(r.regs.a, 0xFF);
    CB_CHECK((r.regs.cc & (cc::N | cc::Z | cc::C)) == (cc::N | cc::C));   // a borrow
    Bus b3 = bus_of({"LDB #0x10", "LDA #0x10", "MUL", "RTS"});
    r = go(b3);
    CB_CHECK_EQ(r.regs.a, 0x01);
    CB_CHECK_EQ(r.regs.b, 0x00);
    Bus b4 = bus_of({"LDD #0x1234", "ADDD #0xffff", "RTS"});
    r = go(b4);
    CB_CHECK_EQ((r.regs.a << 8) | r.regs.b, 0x1233);
    CB_CHECK(r.regs.cc & cc::C);
    Bus b5 = bus_of({"LDA #0x99", "ADDA #0x1", "DAA", "RTS"});
    r = go(b5);
    CB_CHECK_EQ(r.regs.a, 0x00);   // 99 + 1 in BCD
    CB_CHECK(r.regs.cc & cc::C);
});

CB_TEST("engine.pcode_cpu.m6809_shifts_and_memory", [] {
    Bus bus = bus_of({"LDA #0x81", "LSLA", "RORA", "STA 0xc800", "INC 0xc800", "LDB 0xc800", "CLR <0x20", "RTS"});
    const RunResult r = go(bus);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(bus.memory()[0xC800], 0x82);   // $81 << 1 = $02 (C set), ROR with C = $81, then INC
    CB_CHECK_EQ(r.regs.b, 0x82);
});

CB_TEST("engine.pcode_cpu.m6809_indexed_modes_and_the_stack", [] {
    Bus bus = bus_of({"LDX #0xc900", "LDA #0x11", "STA ,X+", "LDA #0x22", "STA ,X++", "LDA #0x33", "STA ,-X", "LEAY 0x5,X", "PSHS X A", "LDX #0x0", "PULS A X", "RTS"});
    const RunResult r = go(bus);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(bus.memory()[0xC900], 0x11);
    CB_CHECK_EQ(bus.memory()[0xC901], 0x22);
    CB_CHECK_EQ(bus.memory()[0xC902], 0x33);   // ,-X after ,X++ made X = $C903, then $C902
    CB_CHECK_EQ(r.regs.x, 0xC902);
    CB_CHECK_EQ(r.regs.y, 0xC907);
    CB_CHECK_EQ(r.regs.a, 0x33);
});

CB_TEST("engine.pcode_cpu.m6809_calls_branches_and_a_loop", [] {
    // sub at $1010 doubles B; main calls it three times in a loop
    Bytes image(0x10000, 0);
    const char* main_lines[] = {"LDB #0x1", "LDA #0x3", "JSR 0x1010", "DECA", "BNE 0x1004", "RTS"};
    std::uint32_t pc = 0x1000;
    for (const char* l : main_lines) {
        const Bytes b = assemble_at(l, pc);
        std::copy(b.begin(), b.end(), image.begin() + pc);
        pc += static_cast<std::uint32_t>(b.size());
    }
    const Bytes sub = assemble_at("LSLB", 0x1010);
    std::copy(sub.begin(), sub.end(), image.begin() + 0x1010);
    image[0x1011] = 0x39;
    Bus bus(image);
    const RunResult r = go(bus);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.b, 0x08);
});

CB_TEST("engine.pcode_cpu.m6809_devices_are_logged_and_a_run_ends_in_one_of_three_ways", [] {
    Bus bus = bus_of({"LDA #0x7f", "STA 0xd00e", "LDA 0xd00d", "RTS"});
    bus.map(0xD000, 0xD7FF, [](std::uint16_t a) -> std::uint8_t { return (a & 0xF) == 0xD ? 0xC0 : 0; });
    RunResult r = go(bus);
    CB_CHECK_EQ(r.regs.a, 0xC0);
    CB_CHECK_EQ(r.io.size(), 2u);
    CB_CHECK(r.io[0].kind == DeviceAccess::Kind::Write && r.io[0].address == 0xD00E && r.io[0].value == 0x7F);
    CB_CHECK(r.io[1].kind == DeviceAccess::Kind::Read && r.io[1].address == 0xD00D);
    Bus loop = bus_of({"BRA 0x1000"});
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
