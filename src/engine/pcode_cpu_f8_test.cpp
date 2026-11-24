// cb-unity: alone
// The Channel F lane's F8 on the one emulator (engine/pcode_cpu_f8.cpp): the same run as the own core (emulate/f8) on random instructions and programs. The lane has no
// homebrew folder in the samples (a Channel F cartridge is not among them), so the random runs and the lane's own tests (machine_chf_test: a cartridge comes back identical and an
// edit diverges, now on the emulator) are the proof.
#include <cstdio>
#include <cstdlib>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/f8.hpp"
#include "engine/pcode_cpu_f8.hpp"

namespace cb {

namespace f8c = cb::emulate::f8;

namespace {

std::string f8_describe(const f8c::RunResult& a, const f8c::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    const auto reg = [&](const char* name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    reg("a", a.regs.a, b.regs.a);
    reg("status", a.regs.status, b.regs.status);
    reg("isar", a.regs.isar, b.regs.isar);
    reg("pc0", a.regs.pc0, b.regs.pc0);
    reg("pc1", a.regs.pc1, b.regs.pc1);
    reg("dc0", a.regs.dc0, b.regs.dc0);
    reg("dc1", a.regs.dc1, b.regs.dc1);
    for (std::size_t i = 0; i < a.regs.scratch.size(); ++i) reg(std::format("r{}", i).c_str(), a.regs.scratch[i], b.regs.scratch[i]);
    if (a.fault.has_value() != b.fault.has_value()) out += "fault-presence ";
    else if (a.fault && (a.fault->pc != b.fault->pc || a.fault->reason != b.fault->reason)) out += std::format("fault['{}' / '{}'] ", a.fault->reason, b.fault->reason);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].port != b.io[i].port || a.io[i].value != b.io[i].value) {
                out += std::format("io@{} ", i);
                break;
            }
    return out;
}

// A machine as the lane makes it: a ROM, some RAM that is written, ports that answer. `low_ram` makes the BIOS area writable too (the lane's is not).
struct F8Machine {
    f8c::Bus bus;
    F8Machine(const Bytes& image, bool low_ram) {
        bus.load(0, image);
        bus.make_writable(0x2800, 0x2FFF);
        if (low_ram) bus.make_writable(0, 0x7FF);
        bus.set_in([](std::uint8_t port) -> std::uint8_t { return static_cast<std::uint8_t>(port * 37 + 5); });
    }
};

constexpr std::uint16_t kF8Rom = 0x0800, kF8Ram = 0x2800;

void f8_compare_random(bool single, const char* title) {
    std::mt19937 rng(single ? 20261124u : 20261125u);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 8000u;
    }();
    for (unsigned n = 0; n < total; ++n) {
        Bytes image(0x10000, 0);
        for (std::size_t i = 0x800; i < 0x1000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        for (std::size_t i = kF8Ram; i < 0x3000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        std::uint8_t first = 0;
        std::uint16_t entry = kF8Rom;
        if (single) {   // every opcode, each many times with other registers and operands
            first = static_cast<std::uint8_t>(n % 256);
            entry = (n / 256) % 3 == 0 ? kF8Ram : kF8Rom;
            image[entry] = first;
        } else {
            entry = (rng() & 1) ? kF8Ram : kF8Rom;
            first = image[entry];
        }
        f8c::RunOptions options;
        options.max_steps = single ? 1 : 40;
        // values at the edges of the byte, where a flag of an add or a compare turns (the overflow of a subtraction whose partial sums wrap is one)
        const auto value = [&]() -> std::uint8_t {
            static constexpr std::uint8_t kEdges[] = {0x00, 0x01, 0x7F, 0x80, 0x81, 0xFE, 0xFF};
            return (rng() % 3) == 0 ? kEdges[rng() % sizeof kEdges] : (rng() & 3) == 0 ? static_cast<std::uint8_t>(rng() & 0x0F) : static_cast<std::uint8_t>(rng());
        };
        if (single) image[entry + 1] = value();
        image[0] = value();   // (what DC0 = 0 reads)
        options.a = value();
        for (std::uint8_t& r : options.r) r = value();
        const bool low_ram = (rng() & 3) == 0;

        Bytes mem[2];
        f8c::RunResult rr[2];
        for (int core = 0; core < 2; ++core) {
            F8Machine machine(image, low_ram);
            rr[core] = core == 1 ? engine::pcode_cpu::run(machine.bus, entry, options) : f8c::run(machine.bus, entry, options);
            mem[core].assign(machine.bus.memory().begin(), machine.bus.memory().end());
        }
        ++runs;
        const std::string what = f8_describe(rr[0], rr[1]);
        if (what.empty() && mem[0] == mem[1]) continue;
        ++different;
        std::string fields = what;
        for (std::size_t i = 0; i < mem[0].size() && mem[0] != mem[1]; ++i)
            if (mem[0][i] != mem[1][i]) {
                fields += std::format("mem@{:X}[own {:02X} emu {:02X}] ", i, mem[0][i], mem[1][i]);
                break;
            }
        if (!single && rr[0].exit() == rr[1].exit() && rr[0].steps != 0) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                f8c::RunOptions o2 = options;
                o2.max_steps = k;
                f8c::RunResult ra, rb;
                Bytes ma, mb;
                for (int core = 0; core < 2; ++core) {
                    F8Machine machine(image, low_ram);
                    (core == 1 ? rb : ra) = core == 1 ? engine::pcode_cpu::run(machine.bus, entry, o2) : f8c::run(machine.bus, entry, o2);
                    (core == 1 ? mb : ma).assign(machine.bus.memory().begin(), machine.bus.memory().end());
                }
                if (!f8_describe(ra, rb).empty() || ma != mb) k0 = k;
            }
            fields += std::format("[first differing step {}] ", k0);
        }
        std::string key = single ? std::format("1: {:02X}", first) : std::string("N: ") + what.substr(0, 24);
        ++by_kind[key];
        if (!example.count(key))
            example[key] = std::format("{:02X} {:02X} {:02X} {:02X}: {}", image[entry], image[entry + 1], image[entry + 2], image[entry + 3], fields);
    }
    std::printf("pcode_cpu f8 %s: %u runs, %u differ\n", title, runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 40) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.f8_instructions_run_as_the_own_core_does", [] { f8_compare_random(true, "instructions"); });

CB_TEST("engine.pcode_cpu.f8_programs_run_as_the_own_core_does", [] { f8_compare_random(false, "programs"); });

// Every pair of operands through the arithmetic that sets flags: an immediate, a byte of memory (DC0 is 0: the first byte of the image) and a scratchpad register, as the
// operand of a compare, an add and a decimal add, with the accumulator at each value.
CB_TEST("engine.pcode_cpu.f8_arithmetic_runs_as_the_own_core_does_on_every_pair_of_operands", [] {
    unsigned runs = 0, different = 0;
    for (const unsigned op : {0x25u, 0x24u, 0x8Du, 0x88u, 0x89u, 0xC0u, 0xD0u}) {   // CI, AI, CM, AM, AMD, AS R0, ASD R0
        for (unsigned a = 0; a < 256; ++a) {
            for (unsigned x = 0; x < 256; ++x) {
                Bytes image(0x10000, 0);
                image[kF8Rom] = static_cast<std::uint8_t>(op);
                image[kF8Rom + 1] = static_cast<std::uint8_t>(x);   // (the immediate of CI and AI)
                image[0] = static_cast<std::uint8_t>(x);            // (what CM, AM and AMD read at DC0 = 0)
                f8c::RunOptions options;
                options.max_steps = 1;
                options.a = static_cast<std::uint8_t>(a);
                options.r[0] = static_cast<std::uint8_t>(x);
                f8c::RunResult rr[2];
                for (int core = 0; core < 2; ++core) {
                    F8Machine machine(image, false);
                    rr[core] = core == 1 ? engine::pcode_cpu::run(machine.bus, kF8Rom, options) : f8c::run(machine.bus, kF8Rom, options);
                }
                ++runs;
                if (const std::string what = f8_describe(rr[0], rr[1]); !what.empty() && ++different <= 6) std::printf("  op %02X a %02X x %02X: %s\n", op, a, x, what.c_str());
            }
        }
    }
    std::printf("pcode_cpu f8 arithmetic: %u runs, %u differ\n", runs, different);
    CB_CHECK_EQ(different, 0u);
});

}  // namespace cb
