// cb-unity: alone
// The PC Engine lane's HuC6280 on the one emulator (engine/pcode_cpu_huc6280.cpp): the same run as the own core on random programs, and the lane's homebrew the same on both cores.
#include <array>
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/huc6280.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_huc6280.hpp"
#include "formats/pce.hpp"

using namespace cb;
namespace h = cb::emulate::huc6280;

namespace {

std::string describe_huc(const h::Run& a, const h::Run& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    const auto eight = [&](const char* name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    eight("a", a.regs.a, b.regs.a);
    eight("x", a.regs.x, b.regs.x);
    eight("y", a.regs.y, b.regs.y);
    eight("s", a.regs.s, b.regs.s);
    eight("p", a.regs.p, b.regs.p);
    eight("pc", a.regs.pc, b.regs.pc);
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].write != b.io[i].write || a.io[i].offset != b.io[i].offset || a.io[i].value != b.io[i].value || a.io[i].vdc != b.io[i].vdc) {
                out += std::format("io@{}[own {} emu {}] ", i, a.io[i].text(), b.io[i].text());
                break;
            }
    return out;
}

// Where a function's first bytes are in the card's file: its address is the physical bank and the logical address ($BBAAAA, as the lane keys it), the bytes are after the copier header, in
// the bank the card's size gives it.
std::size_t huc_file_offset(const std::filesystem::path& path, std::uint32_t address) {
    const formats::pce::Rom rom = formats::pce::read(path);
    const std::optional<std::size_t> bank = rom.rom_bank(address >> 16);
    if (!bank) return SIZE_MAX;
    return rom.header.size() + *bank * h::kBankSize + (address & 0x1FFF);
}

}  // namespace

// A random program in RAM at $2400 (and in ROM at $E000), the ROM and the rest of RAM random: single instructions and short programs, the mapping registers as the machine starts and as
// TAM leaves them.
CB_TEST("engine.pcode_cpu.the_huc6280_runs_random_programs_as_the_own_core_does", [] {
    std::mt19937 rng(20261122);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 20000u;
    }();
    for (unsigned n = 0; n < 2 * total; ++n) {
        const bool single = n < total;
        Bytes rom(0x20000);
        for (auto& b : rom) b = static_cast<std::uint8_t>(rng());
        std::array<std::uint8_t, h::kBankSize> ram_init{};
        for (auto& b : ram_init) b = static_cast<std::uint8_t>(rng());
        for (std::size_t i = 0x400; i < 0x440; ++i) ram_init[i] = static_cast<std::uint8_t>(rng());
        if (single) ram_init[0x400] = static_cast<std::uint8_t>(n * 7 + n / 256);
        else {
            // A program that stays in its RAM: the own core executes bytes it reads from the hardware page (the chips' registers, read twice for the opcode, the status bit changing on every
            // read), which no program does, and the page is where a jump to a random address lands. The branches take small forward offsets; the jumps, calls, returns, BRK, the
            // bit-test branches, TAM (a register that shows the hardware page where the program runs) and the block transfers (which write over the program) become NOPs (each of them is run as the one instruction of the single-instruction runs).
            for (std::size_t i = 0x400; i < 0x440; ++i) {
                const std::uint8_t op = ram_init[i];
                if (op == 0x00 || op == 0x20 || op == 0x40 || op == 0x60 || op == 0x4C || op == 0x6C || op == 0x7C || op == 0x44 || (op & 0x0F) == 0x0F || op == 0x53 || op == 0x73 || op == 0xC3 || op == 0xD3 || op == 0xE3 || op == 0xF3) ram_init[i] = 0xEA;
                else if (op == 0x80 || (op & 0x1F) == 0x10) ram_init[i + 1] = static_cast<std::uint8_t>(ram_init[i + 1] & 7);
            }
            for (std::size_t i = 0x440; i < 0x4C0; ++i) ram_init[i] = 0xEA;   // (what a program that runs off its end finds)
        }
        h::Options options;
        options.max_steps = single ? 1 : 40;
        options.a = static_cast<std::uint8_t>(rng());
        options.x = static_cast<std::uint8_t>(rng());
        options.y = static_cast<std::uint8_t>(rng());
        if ((rng() & 7) == 0) options.a = static_cast<std::uint8_t>(0xF8 + (rng() & 3));
        h::Run rr[2];
        std::array<std::uint8_t, h::kBankSize> ram[2];
        std::array<std::uint8_t, 8> mpr[2];
        for (int core = 0; core < 2; ++core) {
            h::Machine machine(rom);
            machine.ram = ram_init;
            if (!single) machine.mpr[0] = 0xF8;
            rr[core] = core == 1 ? engine::pcode_cpu::run(machine, 0x2400, options) : h::run(machine, 0x2400, options);
            ram[core] = machine.ram;
            mpr[core] = machine.mpr;
        }
        ++runs;
        std::string fields = describe_huc(rr[0], rr[1]);
        if (ram[0] != ram[1]) {
            for (std::size_t i = 0; i < ram[0].size(); ++i)
                if (ram[0][i] != ram[1][i]) {
                    fields += std::format("ram@{:X}[own {:02X} emu {:02X}] ", i, ram[0][i], ram[1][i]);
                    break;
                }
        }
        if (mpr[0] != mpr[1]) fields += "mpr ";
        if (fields.empty()) continue;
        ++different;
        std::string key = std::string(single ? "1: " : "N: ") + std::format("{:02X}", ram_init[0x400]);
        if (!single && (rr[0].steps != 0 || rr[1].steps != 0)) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                h::Options o2 = options;
                o2.max_steps = k;
                h::Run ra, rb;
                std::array<std::uint8_t, h::kBankSize> ma, mb;
                for (int core = 0; core < 2; ++core) {
                    h::Machine machine(rom);
                    machine.ram = ram_init;
                    if (!single) machine.mpr[0] = 0xF8;
                    (core == 1 ? rb : ra) = core == 1 ? engine::pcode_cpu::run(machine, 0x2400, o2) : h::run(machine, 0x2400, o2);
                    (core == 1 ? mb : ma) = machine.ram;
                }
                if (!describe_huc(ra, rb).empty() || ma != mb) k0 = k;
            }
            if (k0 != 0) {
                h::Options o2 = options;
                o2.max_steps = k0 - 1;
                h::Machine machine(rom);
                machine.ram = ram_init;
                if (!single) machine.mpr[0] = 0xF8;
                const h::Run before = h::run(machine, 0x2400, o2);
                key = std::format("N: {:02X}", machine.read(before.regs.pc));
                fields += std::format("[first differing step {} at {:04X}: {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} a={:02X} x={:02X} y={:02X} p={:02X}] ", k0, before.regs.pc,
                                      machine.read(before.regs.pc), machine.read(static_cast<std::uint16_t>(before.regs.pc + 1)), machine.read(static_cast<std::uint16_t>(before.regs.pc + 2)),
                                      machine.read(static_cast<std::uint16_t>(before.regs.pc + 3)), machine.read(static_cast<std::uint16_t>(before.regs.pc + 4)),
                                      machine.read(static_cast<std::uint16_t>(before.regs.pc + 5)), machine.read(static_cast<std::uint16_t>(before.regs.pc + 6)), before.regs.a, before.regs.x,
                                      before.regs.y, before.regs.p);
            }
        }
        ++by_kind[key];
        if (!example.count(key))
            example[key] = std::format("{:02X} {:02X} {:02X} {:02X} {:02X} {:02X} a={:02X} x={:02X}: {}", ram_init[0x400], ram_init[0x401], ram_init[0x402], ram_init[0x403], ram_init[0x404],
                                       ram_init[0x405], options.a, options.x, fields);
    }
    std::printf("pcode_cpu huc6280 instructions: %u runs, %u differ\n", runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 60) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
});

// The PC Engine lane's homebrew on both cores.
CB_TEST("engine.pcode_cpu.the_pce_lane_runs_homebrew_the_same_on_both_cores", [] {
    engine::pcode_compare::compare_homebrew("pce", "pce", ".pce", huc_file_offset);
});

// The development kit's programs are named in capitals.
CB_TEST("engine.pcode_cpu.the_pce_lane_runs_the_capitals_named_homebrew_the_same_on_both_cores", [] {
    engine::pcode_compare::compare_homebrew("pce", "pce", ".PCE", huc_file_offset);
});
