// The Game Boy Advance lane's ARM7TDMI on the one emulator (engine/pcode_cpu_arm.cpp): the lane's homebrew run in every state on both cores, and each function
// judged against edited copies of the program.
#include <array>
#include <chrono>
#include <cstdio>
#include <format>
#include <map>
#include <vector>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/arm.hpp"
#include "emulate/paged_memory.hpp"
#include "engine/lane_kit.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "engine/machine_n3ds.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_internal.hpp"
#include "formats/nds.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;
using engine::pcode_compare::compare_homebrew;

namespace {

// Where in the file a function's first bytes are (the ROM of the Game Boy Advance is mapped at $08000000; a DS program lies at its RAM address).
std::size_t gba_file_offset(const stdfs::path&, std::uint32_t address) { return address & 0x01FFFFFEu; }
std::size_t nds_file_offset(const stdfs::path& path, std::uint32_t address) {
    const formats::nds::Rom rom = formats::nds::read(path);
    const std::optional<formats::nds::Cpu> which = rom.cpu_of(address & ~1u);
    if (!which) return SIZE_MAX;
    const formats::nds::Binary& binary = rom.binary(*which);
    return binary.rom_offset + ((address & ~1u) - binary.ram_address);
}

// (the 3DS lane's function addresses are file offsets)
std::size_t n3ds_file_offset(const stdfs::path&, std::uint32_t address) { return address; }

}  // namespace

CB_TEST("engine.pcode_cpu.the_gba_lane_runs_homebrew_the_same_on_both_cores", [] { compare_homebrew("gba", "gba", ".gba", gba_file_offset); });

CB_TEST("engine.pcode_cpu.the_nds_lane_runs_homebrew_the_same_on_both_cores", [] { compare_homebrew("nds", "nds", ".nds", nds_file_offset); });

CB_TEST("engine.pcode_cpu.the_3ds_lane_runs_homebrew_the_same_on_both_cores", [] {
    compare_homebrew("n3ds", "3ds", ".3dsx", n3ds_file_offset);
    compare_homebrew("n3ds", "3ds", ".elf", n3ds_file_offset);
});

// A tool, not a check: CB_GBA_TRACE=<rom>,<hex entry> (state 0 of the lane: registers zero, memory as the cartridge leaves it) finds the first instruction after which the
// two cores' runs differ and prints it with both cores' registers.
CB_TEST("engine.pcode_cpu.arm_trace_the_first_divergence_of_a_gba_function", [] {
    const auto given = fs::get_env("CB_GBA_TRACE");
    if (!given || given->empty()) return;
    const std::size_t comma = given->find(',');
    const stdfs::path file = fs::from_text(given->substr(0, comma));
    const std::uint32_t entry = static_cast<std::uint32_t>(std::strtoul(given->c_str() + comma + 1, nullptr, 16));
    const Bytes rom_file = fs::read_file(file);
    const bool nds = file.extension() == ".nds";   // (the DS: the program of the processor the address lies in, loaded at its RAM address)
    std::optional<formats::nds::Rom> nds_rom;
    if (nds) nds_rom = formats::nds::read(file);
    const std::optional<formats::nds::Cpu> nds_cpu = nds ? nds_rom->cpu_of(entry & ~1u) : std::nullopt;
    const bool nine = nds_cpu && *nds_cpu == formats::nds::Cpu::Arm9;
    const bool n3ds = file.extension() == ".3dsx" || file.extension() == ".elf";   // (the 3DS: the lane's own machine)
    const emulate::arm::System system = n3ds ? emulate::arm::System::N3ds : !nds ? emulate::arm::System::Gba : nine ? emulate::arm::System::Nds9 : emulate::arm::System::Nds7;
    const verify::lanes::ImageLaneSpec n3ds_spec = n3ds ? engine::n3ds_lane_spec() : verify::lanes::ImageLaneSpec{};
    const auto prepare = [&](emulate::arm::Bus& bus) {
        if (n3ds) return;
        if (nds) {
            bus.load(nds_rom->binary(*nds_cpu).ram_address, nds_rom->memory(*nds_cpu));
        } else {
            bus.set_rom(rom_file);
        }
        const unsigned lines = nds ? 263 : 228, visible = nds ? 192 : 160;
        const auto scanline = std::make_shared<std::uint32_t>(0);
        bus.map(0x04000004, 0x04000007, [scanline, lines, visible](std::uint32_t address, unsigned) -> std::uint32_t {
            const std::uint32_t line = (*scanline)++ % lines;
            const std::uint32_t word = ((line >= visible && line < lines - 1) ? 1u : 0u) | line << 16;
            return word >> (8 * (address - 0x04000004));
        });
        bus.map(0x04000130, 0x04000131, [](std::uint32_t address, unsigned) -> std::uint32_t { return address == 0x04000130 ? 0xFF : 0x03; });
    };
    // the lane's states by number (CB_GBA_STATE, default 0): the registers and the seed the work RAM is filled from
    const unsigned state = [] {
        const auto s = fs::get_env("CB_GBA_STATE");
        return s && !s->empty() ? static_cast<unsigned>(std::strtoul(s->c_str(), nullptr, 10)) : 0u;
    }();
    const std::uint32_t kWork = nds ? 0x02200000 : 0x02010000;
    const std::array<std::array<std::uint32_t, 5>, 4> kStates = {{{0, 0, 0, 0, 0}, {kWork, kWork + 0x1000, 5, 0x10, 0}, {kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3}, {2, 3, kWork + 0x100, kWork + 0x200, 7}}};
    const auto fill = [&](emulate::arm::Bus& bus) {
        if (state == 0 || n3ds) return;
        for (std::size_t i = 0; i < 0x10000; ++i)
            bus.ewram()[kWork - emulate::arm::kEwramBase + i] =
                static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(state) * 13) ^ (i >> 3)) & (state % 2 != 0 ? 0x0F : 0xFF));
    };
    Bytes memory_a, memory_b;   // (the machine's RAMs after each core's last run)
    const auto go = [&](bool pcode, std::uint64_t steps) {
        emulate::arm::Bus bus(system);
        prepare(bus);
        fill(bus);
        emulate::arm::RunOptions options;
        std::uint32_t where = entry;
        if (n3ds) {
            engine::n3ds_prepare(file, entry, n3ds_spec.states[state], bus, options, where);
        } else {
            if (nds) options.sp = nine ? 0x027C3F00 : 0x0380FF00;
            options.a = {kStates[state][0], kStates[state][1], kStates[state][2], kStates[state][3]};
            options.s[0] = kStates[state][4];
        }
        options.max_steps = steps;
        const emulate::arm::RunResult result = pcode ? engine::pcode_cpu::run(bus, where, options) : emulate::arm::run(bus, where, options);
        Bytes& out = pcode ? memory_b : memory_a;
        out.clear();
        if (n3ds) {   // (each byte the run changed in the process's memory: its address, then the byte)
            for (const auto& [address, value] : bus.memory().changes(1 << 20)) {
                for (unsigned k = 0; k < 4; ++k) out.push_back(static_cast<std::uint8_t>(address >> (8 * k)));
                out.push_back(value);
            }
            return result;
        }
        for (const ByteSpan region : {bus.ewram(), bus.iwram(), bus.palette(), bus.vram(), bus.oam(), bus.sram(), bus.itcm(), bus.dtcm(), bus.wram7()})
            out.insert(out.end(), region.begin(), region.end());
        return result;
    };
    const auto same = [&](const emulate::arm::RunResult& a, const emulate::arm::RunResult& b) {
        if (memory_a != memory_b) return false;
        if (a.exit() != b.exit() || a.r != b.r || (a.cpsr & 0xF8000020u) != (b.cpsr & 0xF8000020u) || a.io.size() != b.io.size() || a.bios.size() != b.bios.size()) return false;
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value || a.io[i].kind != b.io[i].kind) return false;
        return true;
    };
    std::uint64_t lo = 0, hi = emulate::arm::kMaxSteps;
    const auto agree = [&](std::uint64_t steps) {
        const emulate::arm::RunResult a = go(false, steps);
        const emulate::arm::RunResult b = go(true, steps);
        return same(a, b);
    };
    if (agree(hi)) {
        std::printf("arm trace: no difference\n");
        return;
    }
    while (hi - lo > 1) {
        const std::uint64_t mid = (lo + hi) / 2;
        (agree(mid) ? lo : hi) = mid;
    }
    const emulate::arm::RunResult before = go(false, hi - 1);
    const emulate::arm::RunResult a = go(false, hi), b = go(true, hi);
    for (std::size_t i = 0; i < memory_a.size() && i < memory_b.size(); ++i)
        if (memory_a[i] != memory_b[i]) {
            std::printf("  memory differs at %s offset %zx: own %02X emulator %02X\n", n3ds ? "change-list" : "region", i, memory_a[i], memory_b[i]);
            if (n3ds) {   // (the entry: its address and byte, in each list)
                const std::size_t entry_at = i / 5 * 5;
                for (const Bytes* list : {&memory_a, &memory_b})
                    if (entry_at + 5 <= list->size()) {
                        const std::uint32_t address = (*list)[entry_at] | (*list)[entry_at + 1] << 8 | (*list)[entry_at + 2] << 16 | static_cast<std::uint32_t>((*list)[entry_at + 3]) << 24;
                        std::printf("    %s: %08X = %02X\n", list == &memory_a ? "own" : "emulator", address, (*list)[entry_at + 4]);
                    }
            }
            break;
        }
    if (memory_a.size() != memory_b.size()) std::printf("  memory changes: own %zu emulator %zu\n", memory_a.size(), memory_b.size());
    std::printf("arm trace: first difference after step %llu: own %s | emulator %s\n", static_cast<unsigned long long>(hi), a.exit().c_str(), b.exit().c_str());
    std::printf("  before: pc %X cpsr %08X r0-r3 %X %X %X %X\n", before.r[15], before.cpsr, before.r[0], before.r[1], before.r[2], before.r[3]);
    for (unsigned i = 0; i < 16; ++i)
        if (a.r[i] != b.r[i]) std::printf("  r%u own %X emu %X\n", i, a.r[i], b.r[i]);
    std::printf("  cpsr own %08X emu %08X; io %zu/%zu\n", a.cpsr, b.cpsr, a.io.size(), b.io.size());
    emulate::arm::Bus bus(system);
    prepare(bus);
    fill(bus);
    emulate::arm::Cpu cpu(bus);
    for (unsigned i = 0; i < 4; ++i) cpu.r[i] = kStates[state][i];
    cpu.r[4] = kStates[state][4];
    cpu.r[13] = nds ? (nine ? 0x027C3F00 : 0x0380FF00) : 0x03007F00;
    cpu.r[14] = emulate::arm::kReturnAddress;
    std::uint32_t where = entry;
    if (n3ds) {
        emulate::arm::RunOptions options;
        engine::n3ds_prepare(file, entry, n3ds_spec.states[state], bus, options, where);
        for (unsigned i = 0; i < 4; ++i) cpu.r[i] = options.a[i];
        cpu.r[4] = options.s[0];
        cpu.r[13] = options.sp;
        cpu.r[14] = options.return_address;
    }
    cpu.jump_to(where);
    std::uint32_t pc = 0;
    for (std::uint64_t i = 0; i < hi; ++i) {
        pc = cpu.r[15];
        if (hi - i <= 40) std::printf("    step %llu: %08X %08X\n", static_cast<unsigned long long>(i + 1), pc, bus.read(pc & ~3u, 4));   // (the last dozen)
        if (const auto fault = cpu.step(); fault && fault->kind != emulate::arm::Fault::Kind::Swi) break;
        else if (fault && n3ds) cpu.r[0] = 0;   // (a kernel call: answered with success)
    }
    std::printf("  instruction at %08X (thumb %d): halfword/word %08X\n", pc, cpu.thumb() ? 1 : 0, bus.read(pc & ~3u, 4));
    if (n3ds) {   // the VFP's registers after the step, where the two cores differ (the emulator's as the run before left them)
        emulate::arm::Bus again(system);
        emulate::arm::RunOptions options;
        std::uint32_t at = entry;
        engine::n3ds_prepare(file, entry, n3ds_spec.states[state], again, options, at);
        options.max_steps = hi;
        engine::pcode_cpu::run(again, at, options);
        auto& emu = engine::pcode_cpu::emulator_named("ARM6_le.sla");
        for (unsigned d = 0; d < 16; ++d) {
            const std::uint64_t own = static_cast<std::uint64_t>(cpu.vfp_single(2 * d + 1)) << 32 | cpu.vfp_single(2 * d);
            const std::uint64_t theirs = emu.reg("d" + std::to_string(d));
            if (own != theirs) std::printf("  d%u own %016llX emu %016llX\n", d, static_cast<unsigned long long>(own), static_cast<unsigned long long>(theirs));
        }
    }
});

// A tool, not a check: CB_PROBE_WORDS=<hex ARM word>,... prints how the ARMv6 description reads each one (an empty text: no instruction).
CB_TEST("engine.pcode_cpu.arm_probe_how_the_description_reads_words", [] {
    const auto given = fs::get_env("CB_PROBE_WORDS");
    if (!given || given->empty()) return;
    auto& emu = engine::pcode_cpu::emulator_named("ARM6_le.sla");
    std::size_t at = 0;
    while (at < given->size()) {
        const std::size_t end = given->find(',', at);
        const std::uint32_t w = static_cast<std::uint32_t>(std::strtoul(given->substr(at, end - at).c_str(), nullptr, 16));
        std::uint8_t b[4] = {static_cast<std::uint8_t>(w), static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w >> 16), static_cast<std::uint8_t>(w >> 24)};
        emu.write(0x1000, b, 4);
        emu.flushDecoded();
        emu.setContext("TMode", 0);
        std::printf("%08X -> '%s'\n", w, emu.text(0x1000).c_str());
        if (end == std::string::npos) break;
        at = end + 1;
    }
});

// The VFP's arithmetic on the ARMv6 description and on the own core: a two-operand instruction on single and double values of every kind (zeros, denormals, infinities,
// quiet and signalling NaNs, ordinary numbers) must leave the same bits.
CB_TEST("engine.pcode_cpu.the_vfp_arithmetic_is_the_same_on_both_cores", [] {
    namespace ar = emulate::arm;
    constexpr std::uint32_t kCode = 0x00100000, kData = 0x00200000, kOut = 0x00200100;
    const std::uint64_t doubles[] = {0, 0x8000000000000000ull, 0x3FF0000000000000ull, 0xBFF0000000000000ull, 0x0000000000000001ull, 0x000FFFFFFFFFFFFFull, 0x7FF0000000000000ull,
                                     0xFFF0000000000000ull, 0x7FF8000000000000ull, 0x7FF8000000001234ull, 0x7FF0000000000001ull, 0xFFF8000000000000ull, 0x4059000000000000ull,
                                     0x3FD5555555555555ull, 0x7FEFFFFFFFFFFFFFull, 0xC008000000000000ull};
    const std::uint32_t singles[] = {0, 0x80000000u, 0x3F800000u, 0xBF800000u, 1u, 0x007FFFFFu, 0x7F800000u, 0xFF800000u, 0x7FC00000u, 0x7FC01234u, 0x7F800001u, 0xFFC00000u, 0x42C80000u,
                                     0x3EAAAAABu, 0x7F7FFFFFu, 0xC0400000u};
    struct Op {
        const char* name;
        std::uint32_t word;
        bool dbl;
    };
    // d7 = d1 op d7, s15 = s16 op s14
    const Op ops[] = {{"vmul.f64", 0xEE217B07, true}, {"vadd.f64", 0xEE317B07, true}, {"vsub.f64", 0xEE317B47, true}, {"vdiv.f64", 0xEE817B07, true},
                      {"vmla.f64", 0xEE017B07, true}, {"vnmul.f64", 0xEE217B47, true}, {"vmul.f32", 0xEE687A07, false}, {"vadd.f32", 0xEE787A07, false},
                      {"vsub.f32", 0xEE787A47, false}, {"vdiv.f32", 0xEEC87A07, false}, {"vmla.f32", 0xEE487A07, false}};
    const auto code_for = [](const Op& op) {
        std::vector<std::uint32_t> w;
        if (op.dbl) w = {0xED901B00, 0xED907B02, op.word, 0xED817B00, 0xE12FFF1E};          // vldr d1,[r0]; vldr d7,[r0,#8]; op; vstr d7,[r1]; bx lr
        else w = {0xED908A00, 0xED907A01, 0xEDD07A02, op.word, 0xEDC17A00, 0xE12FFF1E};   // vldr s16,[r0]; vldr s14,[r0,#4]; vldr s15,[r0,#8]; op; vstr s15,[r1]; bx lr
        Bytes b;
        for (const std::uint32_t v : w)
            for (unsigned k = 0; k < 4; ++k) b.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
        return b;
    };
    unsigned runs = 0, differing = 0;
    for (const Op& op : ops) {
        const Bytes code = code_for(op);
        const auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
        regions->push_back({kCode, code});
        const std::size_t count = op.dbl ? std::size(doubles) : std::size(singles);
        for (std::size_t i = 0; i < count; ++i)
            for (std::size_t j = 0; j < count; ++j) {
                std::uint64_t result[2] = {};
                std::string ended[2];
                for (int core = 0; core < 2; ++core) {
                    ar::Bus bus(ar::System::N3ds);
                    bus.set_regions(regions);
                    if (op.dbl) {
                        bus.memory().write(kData, 8, doubles[i]);
                        bus.memory().write(kData + 8, 8, doubles[j]);
                        bus.memory().write(kData + 16, 8, 0x3FE0000000000000ull);   // the accumulator of a multiply-accumulate (d7 starts as the second operand)
                    } else {
                        bus.memory().write(kData, 4, singles[i]);
                        bus.memory().write(kData + 4, 4, singles[j]);
                        bus.memory().write(kData + 8, 4, 0x3F000000u);
                    }
                    ar::RunOptions options;
                    options.a = {kData, kOut, 0, 0};
                    options.sp = 0x10100000;
                    options.return_address = 0x100;
                    const ar::RunResult run = core == 1 ? engine::pcode_cpu::run(bus, kCode, options) : ar::run(bus, kCode, options);
                    ended[core] = run.exit();
                    result[core] = bus.memory().read(kOut, op.dbl ? 8 : 4);
                }
                ++runs;
                if (result[0] != result[1] || ended[0] != ended[1]) {
                    if (++differing <= 12)
                        std::printf("  %s %llx %llx: own %llx (%s) emulator %llx (%s)\n", op.name, static_cast<unsigned long long>(op.dbl ? doubles[i] : singles[i]),
                                    static_cast<unsigned long long>(op.dbl ? doubles[j] : singles[j]), static_cast<unsigned long long>(result[0]), ended[0].c_str(),
                                    static_cast<unsigned long long>(result[1]), ended[1].c_str());
                }
            }
    }
    std::printf("pcode_cpu vfp arithmetic: %u runs, %u differing\n", runs, differing);
    CB_CHECK_EQ(differing, 0u);
});

// The VFP's moves and conversions: integers to floats and back, floats to floats, the core registers to the VFP's and back, on values of every kind.
CB_TEST("engine.pcode_cpu.the_vfp_conversions_are_the_same_on_both_cores", [] {
    namespace ar = emulate::arm;
    constexpr std::uint32_t kCode = 0x00100000, kData = 0x00200000, kOut = 0x00200100;
    struct Snippet {
        const char* name;
        std::vector<std::uint32_t> words;
        bool double_input;
    };
    const Snippet snippets[] = {
        {"vcvt.f64.s32", {0xEE092A10, 0xEEB89BC9, 0xED819B00, 0xE12FFF1E}, false},   // vmov s18,r2; vcvt.f64.s32 d9,s18; vstr d9,[r1]
        {"vcvt.f64.u32", {0xEE092A10, 0xEEB89B49, 0xED819B00, 0xE12FFF1E}, false},   // ... vcvt.f64.u32 d9,s18
        {"vcvt.f32.s32", {0xEE092A10, 0xEEF89AC9, 0xEDC19A00, 0xE12FFF1E}, false},   // vmov s18,r2; vcvt.f32.s32 s19,s18; vstr s19,[r1]
        {"vcvt.f32.u32", {0xEE092A10, 0xEEF89A49, 0xEDC19A00, 0xE12FFF1E}, false},
        {"vcvt.s32.f64", {0xED909B00, 0xEEBD9BC9, 0xEE190A10, 0xE12FFF1E}, true},    // vldr d9,[r0]; vcvt.s32.f64 s18,d9; vmov r0,s18
        {"vcvt.u32.f64", {0xED909B00, 0xEEBC9BC9, 0xEE190A10, 0xE12FFF1E}, true},
        {"vcvt.f32.f64", {0xED909B00, 0xEEB79BC9, 0xEE190A10, 0xE12FFF1E}, true},    // vcvt.f32.f64 s18,d9; vmov r0,s18
        {"vcvt.f64.f32", {0xED908A00, 0xEEB79AC8, 0xED819B00, 0xE12FFF1E}, false},   // vldr s16,[r0]; vcvt.f64.f32 d9,s16; vstr d9,[r1]
        {"vmov.s,r", {0xEE092A10, 0xEE190A10, 0xE12FFF1E}, false},                   // vmov s18,r2; vmov r0,s18
        {"vneg.f64", {0xED909B00, 0xEEB19B49, 0xED819B00, 0xE12FFF1E}, true},        // vldr d9,[r0]; vneg.f64 d9,d9; vstr d9,[r1]
        {"vabs.f64", {0xED909B00, 0xEEB09BC9, 0xED819B00, 0xE12FFF1E}, true},
        {"vsqrt.f64", {0xED909B00, 0xEEB19BC9, 0xED819B00, 0xE12FFF1E}, true},
    };
    const std::uint64_t doubles[] = {0, 0x8000000000000000ull, 0x3FF0000000000000ull, 0xBFF8000000000000ull, 0x0000000000000001ull, 0x7FF0000000000000ull, 0xFFF0000000000000ull,
                                     0x7FF8000000001234ull, 0x7FF0000000000001ull, 0x4059000000000000ull, 0x41DFFFFFFFC00000ull, 0xC1E0000000000000ull, 0x41F0000000000000ull,
                                     0x3FE0000000000000ull, 0xBFE0000000000000ull, 0x400C000000000000ull, 0x7FEFFFFFFFFFFFFFull};
    const std::uint32_t integers[] = {0, 1, 0xFFFFFFFFu, 0x7FFFFFFFu, 0x80000000u, 127, 0xFFFFFF80u, 0x12345678u, 0x7F800000u, 0xFF800001u};
    unsigned runs = 0, differing = 0;
    std::map<std::string, unsigned> per_snippet;   // (the first difference of each instruction is shown)
    for (const Snippet& snippet : snippets) {
        Bytes code;
        for (const std::uint32_t v : snippet.words)
            for (unsigned k = 0; k < 4; ++k) code.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
        const auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
        regions->push_back({kCode, code});
        const std::size_t count = snippet.double_input ? std::size(doubles) : std::size(integers);
        for (std::size_t i = 0; i < count; ++i) {
            std::uint64_t result[2] = {};
            std::uint32_t r0[2] = {};
            std::string ended[2];
            for (int core = 0; core < 2; ++core) {
                ar::Bus bus(ar::System::N3ds);
                bus.set_regions(regions);
                bus.memory().write(kData, 8, snippet.double_input ? doubles[i] : integers[i]);
                ar::RunOptions options;
                options.a = {kData, kOut, snippet.double_input ? 0u : integers[i], 0};
                options.sp = 0x10100000;
                options.return_address = 0x100;
                const ar::RunResult run = core == 1 ? engine::pcode_cpu::run(bus, kCode, options) : ar::run(bus, kCode, options);
                ended[core] = run.exit();
                result[core] = bus.memory().read(kOut, 8);
                r0[core] = run.r[0];
            }
            ++runs;
            if (result[0] != result[1] || r0[0] != r0[1] || ended[0] != ended[1]) {
                ++differing;
                if (++per_snippet[snippet.name] == 1)
                    std::printf("  %s input %llx: own %llx r0 %x (%s) emulator %llx r0 %x (%s)\n", snippet.name,
                                static_cast<unsigned long long>(snippet.double_input ? doubles[i] : integers[i]), static_cast<unsigned long long>(result[0]), r0[0], ended[0].c_str(),
                                static_cast<unsigned long long>(result[1]), r0[1], ended[1].c_str());
            }
        }
    }
    std::printf("pcode_cpu vfp conversions: %u runs, %u differing\n", runs, differing);
    CB_CHECK_EQ(differing, 0u);
});
