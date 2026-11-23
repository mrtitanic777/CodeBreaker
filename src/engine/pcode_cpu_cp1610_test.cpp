// cb-unity: alone
// The Intellivision lane's CP1610 on the one emulator (engine/pcode_cpu_cp1610.cpp): the same run as the own core on random programs, and the lane's homebrew the same on both cores.
#include <algorithm>
#include <array>
#include <cstdio>
#include <format>
#include <map>
#include <memory>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/cp1610.hpp"
#include "emulate/cp1610.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_cp1610.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "formats/intv.hpp"

using namespace cb;
namespace c = cb::emulate::cp1610;

namespace {

std::string describe_cp1610(const c::RunResult& a, const c::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    for (unsigned i = 0; i < 8; ++i)
        if (a.regs.r[i] != b.regs.r[i]) out += std::format("r{}[own {:X} emu {:X}] ", i, a.regs.r[i], b.regs.r[i]);
    if (a.regs.status() != b.regs.status()) out += std::format("flags[own {:X} emu {:X}] ", a.regs.status(), b.regs.status());
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

// A bus as the Intellivision lane makes one, with a ROM of random words at $5000 and a counter in the STIC's reads (the order of reads shows).
struct Cp1610World {
    c::Bus bus;
    std::shared_ptr<unsigned> reads = std::make_shared<unsigned>(0);
    explicit Cp1610World(const std::vector<std::uint16_t>& rom, const std::vector<std::uint16_t>& ram, std::uint16_t base = 0x5000) {
        auto n = reads;
        bus.device(0x0000, 0x007F, [n](std::uint16_t a) -> std::uint16_t { return static_cast<std::uint16_t>((a * 3 + (*n)++ * 5) & 0x3FFF); }, {}, true, 0x3FFF);
        bus.memory(0x0100, 0x01EF, std::span<const std::uint16_t>(ram).subspan(0, 0xF0), true, false, 0xFF);
        bus.device(0x01F0, 0x01FF, [](std::uint16_t a) -> std::uint16_t { return a >= 0x01FE ? 0xFF : 0; }, {}, true, 0xFFFF);
        bus.memory(0x0200, 0x035F, std::span<const std::uint16_t>(ram).subspan(0xF0, 0x160), true, false, 0xFFFF);
        bus.memory(0x1000, 0x1FFF, std::vector<std::uint16_t>(0x1000, 0x00AF), false, false);
        bus.memory(0x3800, 0x39FF, {}, true, true, 0xFF);
        bus.memory(base, static_cast<std::uint16_t>(base + 0xFFF), rom, false);   // (a cartridge's ROM sits at $5000; one at $D000 has addresses with the top bit set)
    }
};

}  // namespace

CB_TEST("engine.pcode_cpu.the_cp1610_runs_random_programs_as_the_own_core_does", [] {
    std::mt19937 rng(20261122);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 12000u;
    }();
    const auto pointer = [&]() -> std::uint16_t {
        switch (rng() % 6) {
            case 0: return static_cast<std::uint16_t>(rng() & 0x7F);
            case 1: return static_cast<std::uint16_t>(0x3800 + (rng() & 0x1FF));
            case 2: return static_cast<std::uint16_t>(rng());
            case 3: return static_cast<std::uint16_t>((rng() & 1 ? 0xD000 : 0x5000) + (rng() & 0xFFF));
            default: return static_cast<std::uint16_t>(0x0100 + (rng() % 0x260));
        }
    };
    for (unsigned n = 0; n < 2 * total; ++n) {
        const bool single = n < total;
        std::vector<std::uint16_t> rom(0x1000), ram(0x250);
        for (auto& w : rom) w = static_cast<std::uint16_t>(rng() & 0x3FF);
        for (auto& w : ram) w = static_cast<std::uint16_t>(rng() & 0xFFFF);
        rom[0xF00] = 0x00AF;   // (a return through R5 after the program)
        const std::uint16_t base = (rng() & 1) != 0 ? 0xD000 : 0x5000;
        c::RunOptions options;
        options.max_steps = single ? 1 : 40;
        if (single) {
            rom[0] = static_cast<std::uint16_t>(n % 1024);
            if ((n / 1024) % 4 == 3) {   // (after SDBD)
                rom[1] = rom[0];
                rom[0] = 0x001;
                options.max_steps = 2;
            }
        }
        for (auto& r : options.r) r = (rng() & 1) ? pointer() : static_cast<std::uint16_t>(rng());
        options.sp = (rng() & 3) == 0 ? pointer() : static_cast<std::uint16_t>(0x02F0);
        c::RunResult rr[2];
        std::vector<std::uint16_t> mem[2];
        for (int core = 0; core < 2; ++core) {
            Cp1610World w(rom, ram, base);
            rr[core] = core == 1 ? engine::pcode_cpu::run(w.bus, base, options) : c::run(w.bus, base, options);
            for (const std::uint16_t lo : std::array<std::uint16_t, 3>{0x0100, 0x0200, 0x3800}) {
                const std::span<std::uint16_t> words = w.bus.words(static_cast<std::uint16_t>(lo));
                mem[core].insert(mem[core].end(), words.begin(), words.end());
            }
        }
        ++runs;
        std::string fields = describe_cp1610(rr[0], rr[1]);
        if (fields.empty() && mem[0] == mem[1]) continue;
        ++different;
        for (std::size_t i = 0; i < mem[0].size() && mem[0] != mem[1]; ++i)
            if (mem[0][i] != mem[1][i]) {
                fields += std::format("mem@{:X}[own {:X} emu {:X}] ", i, mem[0][i], mem[1][i]);
                break;
            }
        const auto name = [&](std::uint32_t at, bool sdbd) {
            const std::array<std::uint16_t, 3> words = {rom[at - base], rom[at - base + 1], rom[at - base + 2]};
            const auto insn = decompile::cp1610::decode(words, 0, at, sdbd);
            return insn ? decompile::cp1610::mnemonic(*insn) : std::string("?");
        };
        std::string key = std::string(single ? "1: " : "N: ") + name(base, false);
        if (single && options.max_steps == 2) key = "SDBD 1: " + name(base + 1, true);
        if (!single && (rr[0].steps != 0 || rr[1].steps != 0)) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                c::RunOptions o2 = options;
                o2.max_steps = k;
                Cp1610World wa(rom, ram, base), wb(rom, ram, base);
                const c::RunResult ra = c::run(wa.bus, base, o2), rb = engine::pcode_cpu::run(wb.bus, base, o2);
                bool same = true;
                for (const std::uint16_t lo : std::array<std::uint16_t, 3>{0x0100, 0x0200, 0x3800}) {
                    const auto x = wa.bus.words(static_cast<std::uint16_t>(lo)), y = wb.bus.words(static_cast<std::uint16_t>(lo));
                    same = same && std::equal(x.begin(), x.end(), y.begin());
                }
                if (!describe_cp1610(ra, rb).empty() || !same) k0 = k;
            }
            if (k0 != 0) {
                c::RunOptions o2 = options;
                o2.max_steps = k0 - 1;
                Cp1610World wa(rom, ram, base);
                const c::RunResult before = c::run(wa.bus, base, o2);
                const std::uint32_t at = before.regs.r[7];
                if (at >= base && at < base + 0xFFDu) key = "N: " + name(at, false);
                fields += std::format("[first differing step {} at {:04X}] ", k0, at);
            }
        }
        ++by_kind[key];
        if (!example.count(key))
            example[key] = std::format("{:03X} {:03X} {:03X} {:03X}: {}", rom[0], rom[1], rom[2], rom[3], fields);
    }
    std::printf("pcode_cpu cp1610 instructions: %u runs, %u differ\n", runs, different);
    unsigned shown = 0;
    for (const auto& [k, cnt] : by_kind)
        if (++shown <= 80) std::printf("    %u x %s   e.g. %s\n", cnt, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
});

// The Intellivision lane's homebrew on both cores. The words are in the file as segments (a 3-byte signature, then per segment two page bytes, the words as 16-bit big-endian values and a
// 2-byte CRC): a function's address is a word address, found by walking the segments.
CB_TEST("engine.pcode_cpu.the_intellivision_lane_runs_homebrew_the_same_on_both_cores", [] {
    engine::pcode_compare::compare_homebrew("intv", "intellivision", ".rom", [](const std::filesystem::path& path, std::uint32_t address) -> std::size_t {
        const formats::intv::Rom rom = formats::intv::read(path);
        if (rom.raw) return SIZE_MAX;
        std::size_t offset = 3;
        for (const formats::intv::Segment& s : rom.segments) {
            offset += 2;
            if (address >= s.start && address < s.end()) return offset + 2 * static_cast<std::size_t>(address - s.start);
            offset += 2 * s.words.size() + 2;
        }
        return SIZE_MAX;
    });
});
