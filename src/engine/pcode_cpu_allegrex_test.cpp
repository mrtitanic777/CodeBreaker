// cb-unity: alone (file-local names that other files in a batch also use)
// The PSP lane's Allegrex on the one emulator (engine/pcode_cpu_allegrex.cpp): the same run as the own core on random instructions and programs, and on the lane's homebrew.
#include <cstdio>
#include <format>
#include <map>
#include <memory>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/mips.hpp"
#include "emulate/allegrex.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_allegrex.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_internal.hpp"
#include "formats/psp.hpp"

using namespace cb;
namespace al = cb::emulate::allegrex;

namespace {

constexpr std::uint32_t kRam = 0x08800000;   // the user area: code at the start, data from +0x10000
constexpr std::uint32_t kStub = kRam + 0x80;

// What differs between two runs, as text (empty: nothing).
std::string describe(const al::RunResult& a, const al::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    for (unsigned i = 0; i < 32; ++i)
        if (a.r[i] != b.r[i]) out += std::format("r{}[own {:X} emu {:X}] ", i, a.r[i], b.r[i]);
    if (a.hi != b.hi || a.lo != b.lo) out += std::format("hilo[own {:X}:{:X} emu {:X}:{:X}] ", a.hi[0], a.lo[0], b.hi[0], b.lo[0]);
    if (a.f0 != b.f0) out += "f0 ";
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].text() != b.io[i].text() || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) {
                out += std::format("io@{} ", i);
                break;
            }
    if (a.calls.size() != b.calls.size()) out += std::format("calls[{}/{}] ", a.calls.size(), b.calls.size());
    else
        for (std::size_t i = 0; i < a.calls.size(); ++i)
            if (a.calls[i].text() != b.calls[i].text()) {
                out += std::format("call@{} ", i);
                break;
            }
    return out;
}

// A word of the sort a program has: an opcode from the ones the processor runs most, the rest of the bits random. Some words are not instructions at all.
std::uint32_t random_word(std::mt19937& rng) {
    static const unsigned kOps[] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x14, 0x15, 0x16, 0x17,
                                    0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x30, 0x31, 0x38, 0x39, 0x10, 0x11};
    std::uint32_t w = static_cast<std::uint32_t>(rng());
    if (rng() % 8 == 0) return w;   // (anything)
    w = (w & 0x03FFFFFF) | (kOps[rng() % std::size(kOps)] << 26);
    if ((w >> 26) == 0 && rng() % 4 != 0) {   // a SPECIAL: mostly one with the fields it should have zero
        const unsigned fn = w & 63;
        if (fn != 0x0C && fn != 0x0D) w &= (fn == 0 || fn == 2 || fn == 3) ? ~(31u << 21) : ~(31u << 6);
    }
    return w;
}

// A run of `image` on one core (1: the emulator): its result and the bytes of memory that changed.
struct Run {
    al::RunResult result;
    std::vector<std::pair<std::uint32_t, std::uint8_t>> changed;
};

Run go(const Bytes& image, al::RunOptions options, std::uint64_t max_steps, int core) {
    auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
    regions->push_back({kRam, image});
    al::BusOptions bus_options;
    bus_options.imports = {{kStub, "sceDisplaySetMode"}, {kStub + 8, "sceKernelExitGame"}};
    al::Bus bus(regions, bus_options);
    options.max_steps = max_steps;
    Run r;
    r.result = core == 1 ? engine::pcode_cpu::run(bus, kRam, options) : al::run(bus, kRam, options);
    r.changed = bus.memory().changes(1 << 20);
    return r;
}

// The addresses the own core runs its first `k` steps at.
std::vector<std::uint32_t> pcs_of_steps(const Bytes& image, const al::RunOptions& options, std::uint64_t k) {
    auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
    regions->push_back({kRam, image});
    al::BusOptions bus_options;
    bus_options.imports = {{kStub, "sceDisplaySetMode"}, {kStub + 8, "sceKernelExitGame"}};
    al::Bus bus(regions, bus_options);
    emulate::mipsle::Cpu cpu(emulate::mipsle::Model::Allegrex, bus);
    const auto sx = [](std::uint32_t v) { return static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(v))); };
    for (std::size_t i = 0; i < 4; ++i) cpu.r[4 + i] = sx(options.a[i]);
    for (std::size_t i = 0; i < 8; ++i) {
        cpu.r[16 + i] = sx(options.s[i]);
        cpu.r[8 + i] = sx(options.t[i]);
    }
    cpu.r[28] = sx(options.gp);
    cpu.r[29] = sx(options.sp);
    cpu.r[31] = sx(al::kReturnAddress);
    cpu.jump_to(kRam);
    std::vector<std::uint32_t> pcs;
    for (std::uint64_t i = 0; i < k; ++i) {
        pcs.push_back(cpu.pc);
        if (cpu.step()) break;
    }
    return pcs;
}

void compare_random_programs(const char* title, bool single) {
    std::mt19937 rng(20261122);
    std::map<std::string, unsigned> by_kind, tried;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const std::uint64_t emulator_before = engine::pcode_cpu::emulator_named("mips32le.sla").steps();
    const unsigned total = [&] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        if (given && !given->empty() && *given != "0") return static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10));
        return single ? 30000u : 8000u;
    }();
    for (unsigned n = 0; n < total; ++n) {
        Bytes image(0x20000, 0);
        for (std::size_t i = 0x10000; i < 0x14000; ++i) image[i] = static_cast<std::uint8_t>(rng());   // data
        const std::uint32_t first = random_word(rng);
        for (std::size_t i = 0; i < 0x80; i += 4) {
            std::uint32_t w = i == 0 ? first : random_word(rng);
            if (i != 0 && rng() % 16 == 0) w = 0x0C000000u | ((kStub >> 2) & 0x03FFFFFF);   // a call to an imported function
            for (int b = 0; b < 4; ++b) image[i + b] = static_cast<std::uint8_t>(w >> (8 * b));
        }
        al::RunOptions options;
        options.max_steps = single ? 2 : 40;
        options.sp = kRam + 0x18000;
        const auto value = [&]() -> std::uint32_t {
            switch (rng() % 8) {
                case 0: case 1: case 2: return kRam + 0x10000 + (rng() & 0x3FFC);
                case 3: return kRam + 0x10000 + (rng() & 0x3FFF);   // (maybe unaligned)
                case 4: return 0x1C000000u + (rng() & 0xFFC);       // (hardware)
                case 5: return static_cast<std::uint32_t>(rng());
                case 6: return rng() % 40;
                default: return static_cast<std::uint32_t>(-static_cast<int>(rng() % 40));
            }
        };
        for (auto& v : options.a) v = value();
        for (auto& v : options.s) v = value();
        for (auto& v : options.t) v = value();
        options.gp = value();
        const Run own = go(image, options, options.max_steps, 0), emu = go(image, options, options.max_steps, 1);
        ++runs;
        const std::string plain_key = std::format("{}{:02X}.{:02X}", single ? "1:" : "N:", first >> 26, (first >> 26) == 0 ? first & 0x3F : (first >> 16) & 31);
        ++tried[plain_key];
        std::string fields = describe(own.result, emu.result);
        if (fields.empty() && own.changed == emu.changed) continue;
        ++different;
        fields += std::format("(own '{}' emu '{}') ", own.result.exit(), emu.result.exit());
        if (own.changed != emu.changed) fields += "memory ";
        if (!single) {   // the first instruction after which the two cores' runs differ: where it is, and the word there
            for (std::uint64_t k = 1; k <= options.max_steps; ++k) {
                const Run a = go(image, options, k, 0), b = go(image, options, k, 1);
                if (describe(a.result, b.result).empty() && a.changed == b.changed) continue;
                const std::vector<std::uint32_t> pcs = pcs_of_steps(image, options, k);
                std::string history;
                for (std::size_t i = pcs.size() > 5 ? pcs.size() - 5 : 0; i < pcs.size(); ++i) {
                    const std::uint32_t index = (pcs[i] - kRam) / 4;
                    const std::uint32_t word = pcs[i] >= kRam && index * 4 + 4 <= image.size() ? static_cast<std::uint32_t>(image[index * 4]) | static_cast<std::uint32_t>(image[index * 4 + 1]) << 8 | static_cast<std::uint32_t>(image[index * 4 + 2]) << 16 | static_cast<std::uint32_t>(image[index * 4 + 3]) << 24 : 0;
                    history += std::format(" {:08X}:{:08X}", pcs[i], word);
                }
                fields += std::format("[first differs at step {}, the steps before:{}: {}] ", k, history, describe(a.result, b.result));
                break;
            }
        }
        ++by_kind[plain_key];
        if (!example.count(plain_key)) {
            std::uint32_t slot = 0;
            for (int b = 0; b < 4; ++b) slot |= static_cast<std::uint32_t>(image[4 + b]) << (8 * b);
            example[plain_key] = std::format("{:08X} {:08X}: {}", first, slot, fields);
        }
    }
    const std::uint64_t on_emulator = engine::pcode_cpu::emulator_named("mips32le.sla").steps() - emulator_before;   // (the instructions the description ran: the rest were the own core's)
    std::printf("pcode_cpu %s: %u runs, %u differ, %llu instructions on the emulator\n", title, runs, different, static_cast<unsigned long long>(on_emulator));
    CB_CHECK(on_emulator > runs / 4);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 50) std::printf("    %s %u of %u: %s\n", k.c_str(), c, tried[k], example[k].c_str());
    CB_CHECK_EQ(different, 0u);
}

// Where a function's first bytes are in a PSP program's file: its segment's place in the module, and the module's in the file (a packed module: nowhere).
std::size_t psp_file_offset(const std::filesystem::path& path, std::uint32_t address) {
    const std::optional<formats::psp::Program> program = formats::psp::read(path);
    if (!program) return SIZE_MAX;
    for (const formats::mips_elf::ProgramHeader& p : program->elf.program_headers)
        if (p.type == 1 && address >= p.vaddr && address - p.vaddr < p.filesz) return program->elf_offset + p.offset + (address - p.vaddr);
    return SIZE_MAX;
}

}  // namespace

CB_TEST("engine.pcode_cpu.allegrex_instructions_run_as_the_own_core_does", [] { compare_random_programs("allegrex instructions", true); });

CB_TEST("engine.pcode_cpu.allegrex_programs_run_as_the_own_core_does", [] { compare_random_programs("allegrex programs", false); });

// The PSP lane's homebrew on both cores: the EBOOT.PBP files (an unencrypted module: the encrypted ones are not handled by the lane) and the modules of PSPLINK.
CB_TEST("engine.pcode_cpu.the_psp_lane_runs_homebrew_the_same_on_both_cores", [] { engine::pcode_compare::compare_homebrew("psp", "psp", ".PBP", psp_file_offset); });

CB_TEST("engine.pcode_cpu.the_psp_lane_runs_prx_modules_the_same_on_both_cores", [] { engine::pcode_compare::compare_homebrew("psp", "psp", ".prx", psp_file_offset); });
