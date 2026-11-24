// cb-unity: alone
// The Vectrex lane's 6809 on the one emulator (engine/pcode_cpu_m6809.cpp): the same run as the own core on random programs, and the lane's homebrew the same on both cores.
#include <array>
#include <cstdio>
#include <format>
#include <map>
#include <memory>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/m6809.hpp"
#include "emulate/m6809.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_m6809.hpp"

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

}  // namespace

CB_TEST("engine.pcode_cpu.the_6809_runs_random_programs_as_the_own_core_does", [] {
    std::mt19937 rng(20261122);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 20000u;
    }();
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
        Bytes mem[2];
        std::array<std::uint8_t, 0x400> ram[2];
        m::RunResult rr[2];
        for (int core = 0; core < 2; ++core) {
            Mc6809World w(image);
            rr[core] = core == 1 ? engine::pcode_cpu::run(w.bus, 0x1000, options) : m::run(w.bus, 0x1000, options);
            mem[core].assign(w.bus.memory().begin(), w.bus.memory().end());
            ram[core] = *w.ram;
        }
        ++runs;
        std::string fields = describe_6809(rr[0], rr[1]);
        if (fields.empty() && mem[0] == mem[1] && ram[0] == ram[1]) continue;
        ++different;
        for (std::size_t i = 0; i < mem[0].size() && mem[0] != mem[1]; ++i)
            if (mem[0][i] != mem[1][i]) {
                fields += std::format("mem@{:X}[own {:02X} emu {:02X}] ", i, mem[0][i], mem[1][i]);
                break;
            }
        for (std::size_t i = 0; i < ram[0].size() && ram[0] != ram[1]; ++i)
            if (ram[0][i] != ram[1][i]) {
                fields += std::format("ram@{:X}[own {:02X} emu {:02X}] ", i, ram[0][i], ram[1][i]);
                break;
            }
        const auto insn = decompile::m6809::decode(image, 0x1000, 0x1000);
        std::string key = std::string(single ? "1: " : "N: ") + (insn ? std::string(insn->mnemonic) : std::string("?"));
        if (!single && (rr[0].steps != 0 || rr[1].steps != 0)) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                m::RunOptions o2 = options;
                o2.max_steps = k;
                Mc6809World wa(image), wb(image);
                const m::RunResult ra = m::run(wa.bus, 0x1000, o2), rb = engine::pcode_cpu::run(wb.bus, 0x1000, o2);
                const bool same = std::equal(wa.bus.memory().begin(), wa.bus.memory().end(), wb.bus.memory().begin()) && *wa.ram == *wb.ram;
                if (!describe_6809(ra, rb).empty() || !same) k0 = k;
            }
            if (k0 != 0) {
                m::RunOptions o2 = options;
                o2.max_steps = k0 - 1;
                Mc6809World wa(image);
                const m::RunResult before = m::run(wa.bus, 0x1000, o2);
                const auto at = decompile::m6809::decode(wa.bus.memory(), before.regs.pc, before.regs.pc);
                key = "N: " + (at ? std::string(at->mnemonic) : std::string("?"));
                fields += std::format("[first differing step {} at {:04X}] ", k0, before.regs.pc);
            }
        }
        ++by_kind[key];
        if (!example.count(key))
            example[key] = std::format("{:02X}{:02X} {:02X}{:02X} {:02X}{:02X} {:02X}{:02X}: {}", image[0x1000], image[0x1001], image[0x1002], image[0x1003], image[0x1004], image[0x1005],
                                       image[0x1006], image[0x1007], fields);
    }
    std::printf("pcode_cpu 6809 instructions: %u runs, %u differ\n", runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 60) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
});

// The Vectrex lane's homebrew on both cores. The cartridge is the file from $0000: a function's address is its offset.
CB_TEST("engine.pcode_cpu.the_vectrex_lane_runs_homebrew_the_same_on_both_cores", [] {
    engine::pcode_compare::compare_homebrew("vectrex", "vectrex", ".bin", [](const std::filesystem::path&, std::uint32_t address) -> std::size_t { return address; });
});
