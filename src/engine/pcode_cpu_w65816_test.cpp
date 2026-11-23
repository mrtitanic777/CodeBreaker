// cb-unity: alone
// The Super Nintendo lane's 65C816 on the one emulator (engine/pcode_cpu_w65816.cpp): the same run as the own core (emulate/w65816) on random programs, and on the
// lane's homebrew.
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/w65816.hpp"
#include "emulate/w65816.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_w65816.hpp"
#include "formats/snes.hpp"
#include "formats/snes_test_image.hpp"
#include "source/snes_writer.hpp"

namespace w = cb::emulate::w65816;

namespace {

std::string describe(const w::Run& a, const w::Run& b, const w::Machine& ma, const w::Machine& mb) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    const auto reg = [&](const char* name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    reg("A", a.regs.a, b.regs.a);
    reg("X", a.regs.x, b.regs.x);
    reg("Y", a.regs.y, b.regs.y);
    reg("S", a.regs.s, b.regs.s);
    reg("D", a.regs.d, b.regs.d);
    reg("PC", a.regs.pc, b.regs.pc);
    reg("DBR", a.regs.dbr, b.regs.dbr);
    reg("PBR", a.regs.pbr, b.regs.pbr);
    reg("P", a.regs.p, b.regs.p);
    reg("E", a.regs.e, b.regs.e);
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].write != b.io[i].write || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) {
                out += std::format("io@{}[own '{}' emu '{}'] ", i, a.io[i].text(), b.io[i].text());
                break;
            }
    if (ma.wram != mb.wram)
        for (std::size_t i = 0; i < ma.wram.size(); ++i)
            if (ma.wram[i] != mb.wram[i]) {
                out += std::format("wram@{:X}[own {:02X} emu {:02X}] ", i, ma.wram[i], mb.wram[i]);
                break;
            }
    return out;
}

struct Case {
    cb::Bytes file;
    w::Options options;
    std::uint64_t wram_seed = 0;
};

// One run on a core: a machine of its own on the case's cartridge, its RAM filled the same way.
w::Run play(const Case& c, bool emulator, w::Machine** keep, std::unique_ptr<w::Machine>& holder, const cb::formats::snes::Rom& rom, std::uint64_t max_steps) {
    holder = std::make_unique<w::Machine>(rom);
    std::mt19937 fill(static_cast<std::uint32_t>(c.wram_seed));
    for (std::uint8_t& b : holder->wram) b = static_cast<std::uint8_t>(fill());
    w::Options o = c.options;
    o.max_steps = max_steps;
    *keep = holder.get();
    return emulator ? cb::engine::pcode_cpu::run(*holder, 0x008100, o) : w::run(*holder, 0x008100, o);
}

void compare_random(bool single, const char* title) {
    std::mt19937 rng(single ? 20261122 : 20261123);
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
        c.file = cb::formats::snes_test_image::cartridge();
        for (std::size_t i = 0x100; i < 0x7000; ++i) c.file[i] = static_cast<std::uint8_t>(rng());
        if (rng() % 3 == 0)   // (data in the ROM that points at the RAM, the chips and the ROM itself)
            for (std::size_t i = 0x100; i < 0x180; i += 2) c.file[i + 1] = static_cast<std::uint8_t>((rng() % 4) == 0 ? 0x21 + rng() % 2 * 0x20 : rng() % 0x20);
        const std::uint8_t first = static_cast<std::uint8_t>(single ? n % 256 : rng());
        c.file[0x100] = first;
        c.wram_seed = rng();
        auto word = [&] { return static_cast<std::uint16_t>((rng() & 1) ? rng() & 0xFF : (rng() & 3) == 0 ? 0x2100 + (rng() & 0xFF) : rng() & 0xFFFF); };
        c.options.a = word();
        c.options.x = word();
        c.options.y = word();
        c.options.native = (rng() % 5) != 0;
        c.options.m16 = (rng() & 1) != 0;
        c.options.x16 = (rng() & 1) != 0;
        const std::uint64_t steps = single ? 1 : 40;
        const cb::formats::snes::Rom rom = cb::formats::snes::parse(c.file);
        std::unique_ptr<w::Machine> holder[2];
        w::Machine* machine[2];
        w::Run run[2] = {play(c, false, &machine[0], holder[0], rom, steps), play(c, true, &machine[1], holder[1], rom, steps)};
        ++runs;
        executed += run[0].steps;
        std::string fields = describe(run[0], run[1], *machine[0], *machine[1]);
        if (fields.empty()) continue;
        ++different;
        std::uint8_t culprit = first;   // (the opcode of the instruction after which the cores differ)
        if (!single && run[0].exit() == run[1].exit() && run[0].steps != 0) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= steps && k0 == 0; ++k) {
                std::unique_ptr<w::Machine> h[2];
                w::Machine* m[2];
                const w::Run ra = play(c, false, &m[0], h[0], rom, k), rb = play(c, true, &m[1], h[1], rom, k);
                if (!describe(ra, rb, *m[0], *m[1]).empty()) k0 = k;
            }
            fields += std::format("[first differing step {}] ", k0);
            if (k0 > 1) {
                std::unique_ptr<w::Machine> h[2];
                w::Machine* m[2];
                const w::Run before = play(c, false, &m[0], h[0], rom, k0 - 1);
                const std::uint32_t at = static_cast<std::uint32_t>(before.regs.pbr) << 16 | before.regs.pc;
                const std::uint8_t op = m[0]->read(at);   // (the instruction, wherever it is: ROM or RAM)
                culprit = op;
                fields += std::format("(op {:02X} at {:06X}, before: A{:X} X{:X} Y{:X} S{:X} D{:X} DB{:X} P{:02X} e{}) ", op, at, before.regs.a, before.regs.x, before.regs.y,
                                      before.regs.s, before.regs.d, before.regs.dbr, before.regs.p, before.regs.e);
                std::unique_ptr<w::Machine> ha[2];
                w::Machine* mm[2];
                const w::Run ra = play(c, false, &mm[0], ha[0], rom, k0), rb = play(c, true, &mm[1], ha[1], rom, k0);
                fields += "after it: " + describe(ra, rb, *mm[0], *mm[1]);
            }
        }
        const auto& entry = cb::decompile::w65816::opcode_table()[culprit];
        const std::string key = std::string(single ? "1: " : "N: ") + std::string(entry.mnemonic);
        ++by_kind[key];
        if (!example.count(key)) example[key] = std::format("{:02X} {:02X} {:02X} {:02X}: {}", first, c.file[0x101], c.file[0x102], c.file[0x103], fields);
    }
    std::printf("pcode_cpu %s: %u runs, %llu instructions, %u differ\n", title, runs, static_cast<unsigned long long>(executed), different);
    unsigned shown = 0;
    for (const auto& [k, count] : by_kind)
        if (++shown <= 40) std::printf("    %u x %s   e.g. %s\n", count, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.w65816_instructions_run_as_the_own_core_does", [] { compare_random(true, "w65816 instructions"); });

CB_TEST("engine.pcode_cpu.w65816_programs_run_as_the_own_core_does", [] { compare_random(false, "w65816 programs"); });

// The lane's homebrew on both cores. (The file offset is the ROM's own, behind the copier header: nothing is packed.)
CB_TEST("engine.pcode_cpu.the_snes_lane_runs_homebrew_the_same_on_both_cores", [] {
    cb::engine::pcode_compare::compare_homebrew("snes", "snes", ".sfc", [](const std::filesystem::path& path, std::uint32_t address) -> std::size_t {
        const cb::formats::snes::Rom rom = cb::formats::snes::read(path);
        const auto offset = rom.offset(address & cb::source::snes_writer::kAddressMask);
        if (!offset) return SIZE_MAX;
        return rom.copier_header.size() + *offset;
    });
});
