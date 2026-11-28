// cb-unity: alone
// The Game Boy lane's SM83 on the one emulator (engine/pcode_cpu_sm83.cpp): random instructions and programs as the retired own core (emulate/sm83's interpreter)
// ran them, kept as digests (the own core's unit tests are engine/pcode_cpu_sm83_unit_test).
#include <cstdio>
#include <cstdlib>
#include <format>
#include <functional>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/sm83.hpp"
#include "core/crypto.hpp"
#include "emulate/cpu/pcode_cpu_sm83.hpp"

namespace cb {

namespace sm83c = cb::emulate::sm83;

namespace {

std::string sm83_describe(const sm83c::RunResult& a, const sm83c::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    const auto reg = [&](const char* name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    reg("a", a.regs.a, b.regs.a);
    reg("f", a.regs.f, b.regs.f);
    reg("b", a.regs.b, b.regs.b);
    reg("c", a.regs.c, b.regs.c);
    reg("d", a.regs.d, b.regs.d);
    reg("e", a.regs.e, b.regs.e);
    reg("h", a.regs.h, b.regs.h);
    reg("l", a.regs.l, b.regs.l);
    reg("sp", a.regs.sp, b.regs.sp);
    reg("pc", a.regs.pc, b.regs.pc);
    reg("ime", a.regs.ime, b.regs.ime);
    reg("halted", a.regs.halted, b.regs.halted);
    if (a.fault.has_value() != b.fault.has_value()) out += "fault-presence ";
    else if (a.fault && (a.fault->kind != b.fault->kind || a.fault->pc != b.fault->pc)) out += std::format("fault[{} / {}] ", a.fault->text(), b.fault->text());
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].where != b.io[i].where || a.io[i].value != b.io[i].value) {
                out += std::format("io@{} ", i);
                break;
            }
    return out;
}

// A machine as the lane makes it: the device registers, the bank-select range and the stack in work RAM.
struct Sm83Machine {
    sm83c::Bus bus;
    explicit Sm83Machine(const Bytes& image) : bus(image) {
        bus.map(0xFF00, 0xFF7F, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 7 + 3); });
        bus.map(0xFFFF, 0xFFFF);
        sm83c::Bus* self = &bus;
        bus.map(0x2000, 0x3FFF, [self](std::uint16_t a) -> std::uint8_t { return self->memory()[a]; });
    }
};

constexpr std::uint16_t kSm83Code = 0xC000;   // where the instructions are (work RAM: a program that stores into itself changes what runs)

std::uint16_t sm83_pointer(std::mt19937& rng) {   // mostly into work RAM, now and then anywhere (the devices, the bank range, the program)
    switch (rng() % 8) {
        case 0: return static_cast<std::uint16_t>(rng());
        case 1: return static_cast<std::uint16_t>(0xFF00 + (rng() & 0xFF));
        case 2: return static_cast<std::uint16_t>(kSm83Code + (rng() & 0x3F));
        default: return static_cast<std::uint16_t>(0xC100 + (rng() & 0x7FF));
    }
}

using Sm83Runner = std::function<sm83c::RunResult(sm83c::Bus&, std::uint16_t, const sm83c::RunOptions&)>;

// Every opcode of both pages many times over (single), or programs of up to 40 steps, from random registers over random memory with the lane's devices: each
// run's exit, steps, registers and device accesses, and the memory, folded into a CRC-32.
std::uint32_t sm83_digest(bool single, const Sm83Runner& run) {
    std::mt19937 rng(single ? 20261122u : 20261123u);
    std::uint32_t digest = 0;
    constexpr unsigned total = 8000;
    for (unsigned n = 0; n < total; ++n) {
        Bytes image(0x10000, 0);
        for (std::size_t i = 0; i < 0x8000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        for (std::size_t i = 0xC000; i < 0xE000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        for (std::size_t i = 0xFF80; i < 0xFFFE; ++i) image[i] = static_cast<std::uint8_t>(rng());
        std::uint8_t first = 0, second = 0;
        if (single) {   // every opcode of both pages, each many times with other registers and operands
            const unsigned k = n % 512;
            if (k < 256) {
                first = static_cast<std::uint8_t>(k);
            } else {
                first = 0xCB;
                second = static_cast<std::uint8_t>(k - 256);
            }
            image[kSm83Code] = first;
            if (first == 0xCB) image[kSm83Code + 1] = second;
        } else {
            first = image[kSm83Code];
        }
        (void)first;
        (void)second;
        sm83c::RunOptions options;
        options.max_steps = single ? 1 : 40;
        options.sp = (rng() & 3) == 0 ? sm83_pointer(rng) : static_cast<std::uint16_t>(0xDFF0 - (rng() & 0xFE));
        if (rng() & 1) options.sp = static_cast<std::uint16_t>(options.sp | 1);
        const auto value = [&]() -> std::uint8_t { return (rng() & 3) == 0 ? static_cast<std::uint8_t>(rng() & 0x0F) : static_cast<std::uint8_t>(rng()); };
        options.regs.a = value();
        options.regs.f = static_cast<std::uint8_t>(rng());
        options.regs.b = value();
        options.regs.c = value();
        options.regs.d = value();
        options.regs.e = value();
        const std::uint16_t hl = sm83_pointer(rng);
        options.regs.h = static_cast<std::uint8_t>(hl >> 8);
        options.regs.l = static_cast<std::uint8_t>(hl);
        if (rng() & 1) {   // (the pairs BC and DE point somewhere too)
            const std::uint16_t bc = sm83_pointer(rng), de = sm83_pointer(rng);
            options.regs.b = static_cast<std::uint8_t>(bc >> 8);
            options.regs.c = static_cast<std::uint8_t>(bc);
            options.regs.d = static_cast<std::uint8_t>(de >> 8);
            options.regs.e = static_cast<std::uint8_t>(de);
        }
        if ((rng() & 31) == 0) options.regs.ime = true;
        if ((rng() & 63) == 0) options.regs.halted = true;

        Sm83Machine machine(image);
        const sm83c::RunResult r = run(machine.bus, kSm83Code, options);
        std::string text = sm83_describe(sm83c::RunResult{}, r);
        for (const sm83c::BusAccess& x : r.io) text += std::format("|{}{:X}={:X}", static_cast<int>(x.kind), x.where, x.value);
        digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
        digest = crypto::crc32(machine.bus.memory(), digest);
    }
    return digest;
}

sm83c::RunResult on_the_emulator(sm83c::Bus& bus, std::uint16_t entry, const sm83c::RunOptions& options) { return emulate::pcode_cpu::run(bus, entry, options); }

}  // namespace

// What 8,000 runs of each kind gave on the retired core, and on the emulator the same when the core was retired (2026-11-27).
CB_TEST("emulate.pcode_cpu.sm83_instructions_run_as_the_own_core_did", [] {
    const std::uint32_t digest = sm83_digest(true, on_the_emulator);
    std::printf("pcode_cpu sm83 instructions: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x4A902DE1u);
});

CB_TEST("emulate.pcode_cpu.sm83_programs_run_as_the_own_core_did", [] {
    const std::uint32_t digest = sm83_digest(false, on_the_emulator);
    std::printf("pcode_cpu sm83 programs: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x9EB75208u);
});

}  // namespace cb
