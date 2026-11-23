// cb-unity: alone
// The Neo Geo Pocket lane's TLCS-900/H on the one emulator (engine/pcode_cpu_tlcs900.cpp): the same run as the own core (emulate/tlcs900) on random instructions and
// programs, and on the lane's homebrew.
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/tlcs900.hpp"
#include "emulate/tlcs900.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_tlcs900.hpp"
#include "formats/ngp.hpp"

namespace t = cb::emulate::tlcs900;

namespace {

constexpr std::uint32_t kRam = 0x004000, kRamSize = 0x3000, kDevice = 0x008000, kRom = 0x200000, kRomSize = 0x10000, kBios = 0xFF0000;

std::string describe(const t::RunResult& a, const t::RunResult& b, std::span<const std::uint8_t> ram_a, std::span<const std::uint8_t> ram_b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    const auto reg = [&](const char* name, unsigned long x, unsigned long y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    for (unsigned bank = 0; bank < 4; ++bank)
        for (unsigned k = 0; k < 4; ++k) reg(std::format("{}{}", "XWA\0XBC\0XDE\0XHL" + 4 * k, bank).c_str(), a.regs.bank[bank][k], b.regs.bank[bank][k]);
    reg("XIX", a.regs.xix, b.regs.xix);
    reg("XIY", a.regs.xiy, b.regs.xiy);
    reg("XIZ", a.regs.xiz, b.regs.xiz);
    reg("XSP", a.regs.xsp, b.regs.xsp);
    reg("PC", a.regs.pc, b.regs.pc);
    reg("SR", a.regs.sr, b.regs.sr);
    reg("F'", a.regs.f_alt, b.regs.f_alt);
    reg("INTNEST", a.regs.intnest, b.regs.intnest);
    for (unsigned k = 0; k < 4; ++k) {
        reg("DMAS", a.regs.dmas[k], b.regs.dmas[k]);
        reg("DMAD", a.regs.dmad[k], b.regs.dmad[k]);
        reg("DMAC", a.regs.dmac[k], b.regs.dmac[k]);
        reg("DMAM", a.regs.dmam[k], b.regs.dmam[k]);
    }
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].size != b.io[i].size || a.io[i].value != b.io[i].value) {
                out += std::format("io@{}[own {:X}/{:X}/{}/{:X} emu {:X}/{:X}/{}/{:X}] ", i, static_cast<int>(a.io[i].kind), a.io[i].address, a.io[i].size, a.io[i].value, static_cast<int>(b.io[i].kind),
                                   b.io[i].address, b.io[i].size, b.io[i].value);
                break;
            }
    if (a.swis.size() != b.swis.size()) out += std::format("swis[{}/{}] ", a.swis.size(), b.swis.size());
    else
        for (std::size_t i = 0; i < a.swis.size(); ++i)
            if (a.swis[i].number != b.swis[i].number || a.swis[i].pc != b.swis[i].pc || a.swis[i].function != b.swis[i].function || a.swis[i].regs != b.swis[i].regs) {
                out += std::format("swi@{} ", i);
                break;
            }
    if (!std::equal(ram_a.begin(), ram_a.end(), ram_b.begin(), ram_b.end()))
        for (std::size_t i = 0; i < ram_a.size(); ++i)
            if (ram_a[i] != ram_b[i]) {
                out += std::format("ram@{:X}[own {:02X} emu {:02X}] ", kRam + i, ram_a[i], ram_b[i]);
                break;
            }
    return out;
}

struct Case {
    cb::Bytes rom, ram, bios;
    t::RunOptions options;
};

// One run on a core: a bus of its own on the case's memory.
t::RunResult play(const Case& c, bool emulator, std::uint64_t max_steps, cb::Bytes& ram_after, cb::Bytes* at_pc = nullptr) {
    t::Bus bus;
    bus.device(kDevice, kDevice + 0x1FF, [](std::uint32_t a, unsigned size) -> std::uint32_t { return (a * 7 + 3) * (size + 1); }, [](std::uint32_t, unsigned, std::uint32_t) {});
    bus.memory(kRam, kRam + kRamSize - 1, c.ram, true, false);
    bus.memory(kRom, kRom + kRomSize - 1, c.rom, false, false);
    bus.memory(kBios, 0xFFFFFF, c.bios, false, false);
    t::RunOptions o = c.options;
    o.max_steps = max_steps;
    const t::RunResult run = emulator ? cb::engine::pcode_cpu::run(bus, kRom, o) : t::run(bus, kRom, o);
    const std::span<std::uint8_t> ram = bus.bytes(kRam);
    ram_after.assign(ram.begin(), ram.end());
    if (at_pc != nullptr) {   // (the twelve bytes at the pc the run ended at)
        at_pc->resize(12);
        for (unsigned k = 0; k < 12; ++k) (*at_pc)[k] = static_cast<std::uint8_t>(bus.read((run.regs.pc + k) & 0xFFFFFF, 1));
    }
    return run;
}

void compare_random(bool single, const char* title) {
    std::mt19937 rng(single ? 20261124 : 20261125);
    const unsigned total = [] {
        const auto given = cb::fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 6000u;
    }();
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    std::uint64_t executed = 0;   // (instructions the own core ran: a random byte is often no instruction)
    for (unsigned n = 0; n < total; ++n) {
        Case c;
        c.rom.resize(kRomSize);
        for (std::uint8_t& b : c.rom) b = static_cast<std::uint8_t>(rng());
        if (rng() % 3 == 0)   // (code that is mostly instructions that name a register or a short address)
            for (std::size_t i = 0; i < 64; ++i) c.rom[i] = static_cast<std::uint8_t>((rng() & 1) ? 0xC8 + rng() % 0x28 : rng());
        c.ram.resize(kRamSize);
        for (std::uint8_t& b : c.ram) b = static_cast<std::uint8_t>(rng());
        c.bios.assign(0x10000, 0x0E);
        const auto value = [&]() -> std::uint32_t {
            switch (rng() % 6) {
                case 0: return kRam + rng() % kRamSize;
                case 1: return kRom + rng() % kRomSize;
                case 2: return kDevice + rng() % 0x200;
                case 3: return rng() & 0xFF;
                default: return static_cast<std::uint32_t>(rng());
            }
        };
        t::RunOptions& o = c.options;
        o.xwa = value();
        o.xbc = value();
        o.xde = value();
        o.xhl = value();
        o.xix = value();
        o.xiy = value();
        o.xiz = value();
        o.sr = static_cast<std::uint16_t>((rng() & 0xF3FF) | ((rng() & 3) << 8));
        o.sp = kRam + 0x2000 + (rng() & 0xFF);
        for (unsigned k = 0; k < rng() % 3; ++k) o.args.push_back(static_cast<std::uint16_t>(rng()));
        const std::uint64_t steps = single ? 1 : 40;
        cb::Bytes ram[2];
        const t::RunResult run[2] = {play(c, false, steps, ram[0]), play(c, true, steps, ram[1])};
        ++runs;
        executed += run[0].steps;
        std::string fields = describe(run[0], run[1], ram[0], ram[1]);
        if (fields.empty()) continue;
        ++different;
        std::string culprit = "?";
        const auto name_at = [&](const t::RunResult& before, const cb::Bytes& window) {
            const auto insn = cb::decompile::tlcs900::decode(window, 0, before.regs.pc);
            return insn ? std::string(cb::decompile::tlcs900::op_name(insn->row->id)) + "." + std::to_string(insn->z) + std::format(" ({:02X} {:02X} {:02X} {:02X}) at {:X}", window[0], window[1], window[2], window[3], before.regs.pc) : std::string("undefined");
        };
        if (single) {
            cb::Bytes window;
            culprit = name_at(play(c, false, 0, ram[0], &window), window);
        } else if (run[0].exit() == run[1].exit() && run[0].steps != 0) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= steps && k0 == 0; ++k) {
                cb::Bytes a, b;
                const t::RunResult ra = play(c, false, k, a), rb = play(c, true, k, b);
                if (!describe(ra, rb, a, b).empty()) k0 = k;
            }
            fields += std::format("[first differing step {}] ", k0);
            if (k0 != 0) {
                cb::Bytes a, b;
                cb::Bytes window;
                culprit = name_at(play(c, false, k0 - 1, a, &window), window);
                const t::RunResult ra = play(c, false, k0, a), rb = play(c, true, k0, b);
                fields += "after it: " + describe(ra, rb, a, b);
            }
        }
        const std::string key = std::string(single ? "1: " : "N: ") + culprit.substr(0, culprit.find(' '));
        ++by_kind[key];
        if (!example.count(key)) example[key] = culprit + ": " + fields;
    }
    std::printf("pcode_cpu %s: %u runs, %llu instructions, %u differ\n", title, runs, static_cast<unsigned long long>(executed), different);
    unsigned shown = 0;
    for (const auto& [k, count] : by_kind)
        if (++shown <= 60) std::printf("    %u x %s   e.g. %s\n", count, k.c_str(), example[k].substr(0, 420).c_str());
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.tlcs900_instructions_run_as_the_own_core_does", [] { compare_random(true, "tlcs900 instructions"); });

CB_TEST("engine.pcode_cpu.tlcs900_programs_run_as_the_own_core_does", [] { compare_random(false, "tlcs900 programs"); });

// The lane's homebrew on both cores. (The ROM is mapped at $200000 as the file is; nothing is packed.)
CB_TEST("engine.pcode_cpu.the_ngp_lane_runs_homebrew_the_same_on_both_cores", [] {
    cb::engine::pcode_compare::compare_homebrew("ngp", "ngp", ".ngp", [](const std::filesystem::path& path, std::uint32_t address) -> std::size_t {
        const std::uint32_t at = address & 0xFFFFFF;
        if (at < kRom) return SIZE_MAX;
        const std::size_t offset = at - kRom;
        return offset < cb::fs::file_size(path) ? offset : SIZE_MAX;
    });
});
