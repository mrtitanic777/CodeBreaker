// cb-unity: alone
// The Game Boy lane's SM83 on the one emulator (engine/pcode_cpu_sm83.cpp): the same run as the own core (emulate/sm83) on random instructions and programs, and the lane's
// homebrew the same on both cores.
#include <cstdio>
#include <cstdlib>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/sm83.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_sm83.hpp"

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

void sm83_compare_random(bool single, const char* title) {
    std::mt19937 rng(single ? 20261122u : 20261123u);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 8000u;
    }();
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

        Bytes mem[2];
        sm83c::RunResult rr[2];
        for (int core = 0; core < 2; ++core) {
            Sm83Machine machine(image);
            rr[core] = core == 1 ? engine::pcode_cpu::run(machine.bus, kSm83Code, options) : sm83c::run(machine.bus, kSm83Code, options);
            mem[core].assign(machine.bus.memory().begin(), machine.bus.memory().end());
        }
        ++runs;
        const std::string what = sm83_describe(rr[0], rr[1]);
        if (what.empty() && mem[0] == mem[1]) continue;
        ++different;
        std::string fields = what;
        for (std::size_t i = 0; i < mem[0].size() && mem[0] != mem[1]; ++i)
            if (mem[0][i] != mem[1][i]) {
                fields += std::format("mem@{:X}[own {:02X} emu {:02X}] ", i, mem[0][i], mem[1][i]);
                break;
            }
        if (!single && (rr[0].steps != 0 || rr[1].steps != 0)) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                sm83c::RunOptions o2 = options;
                o2.max_steps = k;
                sm83c::RunResult ra, rb;
                Bytes ma, mb;
                for (int core = 0; core < 2; ++core) {
                    Sm83Machine machine(image);
                    (core == 1 ? rb : ra) = core == 1 ? engine::pcode_cpu::run(machine.bus, kSm83Code, o2) : sm83c::run(machine.bus, kSm83Code, o2);
                    (core == 1 ? mb : ma).assign(machine.bus.memory().begin(), machine.bus.memory().end());
                }
                if (!sm83_describe(ra, rb).empty() || ma != mb) k0 = k;
            }
            fields += std::format("[first differing step {}] ", k0);
            if (k0 > 0 && fs::get_env("CB_PCODE_TRACE")) {   // (the steps up to the first difference, on both cores: pc, A, F, SP)
                for (unsigned k = k0 > 6 ? k0 - 6 : 1; k <= k0; ++k) {
                    sm83c::RunOptions o3 = options;
                    o3.max_steps = k;
                    Sm83Machine m1(image), m2(image);
                    const sm83c::RunResult x = sm83c::run(m1.bus, kSm83Code, o3), y = engine::pcode_cpu::run(m2.bus, kSm83Code, o3);
                    std::printf("      step %u: own pc %04X a %02X f %02X sp %04X | emu pc %04X a %02X f %02X sp %04X\n", k, x.regs.pc, x.regs.a, x.regs.f, x.regs.sp, y.regs.pc, y.regs.a,
                                y.regs.f, y.regs.sp);
                }
            }
            if (k0 > 0) {   // (what the own core was about to run: where, and the bytes there)
                sm83c::RunOptions o3 = options;
                o3.max_steps = k0 - 1;
                Sm83Machine machine(image);
                const sm83c::RunResult before = sm83c::run(machine.bus, kSm83Code, o3);
                const auto m = machine.bus.memory();
                fields += std::format("[before it: pc {:04X} bytes {:02X} {:02X} {:02X} sp {:04X}] ", before.regs.pc, m[before.regs.pc], m[static_cast<std::uint16_t>(before.regs.pc + 1)],
                                      m[static_cast<std::uint16_t>(before.regs.pc + 2)], before.regs.sp);
            }
        }
        std::string key = single ? std::format("1: {:02X}{}", first, first == 0xCB ? std::format(" {:02X}", second) : std::string()) : std::string("N: ") + what.substr(0, 24);
        ++by_kind[key];
        if (!example.count(key))
            example[key] = std::format("{:02X} {:02X} {:02X} {:02X} {:02X}: {}", image[kSm83Code], image[kSm83Code + 1], image[kSm83Code + 2], image[kSm83Code + 3], image[kSm83Code + 4], fields);
    }
    std::printf("pcode_cpu sm83 %s: %u runs, %u differ\n", title, runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 40) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.sm83_instructions_run_as_the_own_core_does", [] { sm83_compare_random(true, "instructions"); });

CB_TEST("engine.pcode_cpu.sm83_programs_run_as_the_own_core_does", [] { sm83_compare_random(false, "programs"); });

// The Game Boy lane's homebrew on both cores. The ROM is the memory's first bytes as they are in the file (no packing): a function's address is its file offset.
CB_TEST("engine.pcode_cpu.the_game_boy_lane_runs_homebrew_the_same_on_both_cores", [] {
    engine::pcode_compare::compare_homebrew("gb", "gb-ucity", ".gbc", [](const std::filesystem::path&, std::uint32_t address) -> std::size_t { return address; });
});

}  // namespace cb
