// The Game Boy Advance lane's ARM7TDMI on the one emulator (engine/pcode_cpu_arm.cpp): the lane's homebrew run in every state on both cores, and each function
// judged against edited copies of the program.
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <bit>
#include <format>
#include <random>
#include <optional>
#include <map>
#include <vector>

#include "base/config.hpp"
#include "core/cpu_time.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/arm.hpp"
#include "decompile/arm_ext.hpp"
#include "emulate/arm.hpp"
#include "emulate/paged_memory.hpp"
#include "engine/lane_kit.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "engine/machine_n3ds.hpp"
#include "engine/machine_threedo.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "emulate/cpu/pcode_cpu_arm.hpp"
#include "emulate/cpu/pcode_cpu_compare_test.hpp"
#include "emulate/cpu/pcode_cpu_internal.hpp"
#include "formats/nds.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;
using emulate::pcode_compare::compare_homebrew;

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

// What the adapter handed to the own core in the runs since the counts were cleared: nothing, now; a run that did is listed by kind.
void report_handover(const char* lane) {
    const emulate::pcode_cpu::ArmHandover& handover = emulate::pcode_cpu::arm_handover();
    std::uint64_t handed = 0;
    for (const auto& [kind, steps] : handover.by_class) {
        handed += steps;
        std::printf("  handed to the own core: %llu x %s\n", static_cast<unsigned long long>(steps), kind.c_str());
    }
    std::printf("pcode_cpu %s: %llu steps on the emulator's side, %llu handed to the own core\n", lane, static_cast<unsigned long long>(handover.steps), static_cast<unsigned long long>(handed));
    CB_CHECK_EQ(handed, 0u);
}

CB_TEST("emulate.pcode_cpu.the_gba_lane_runs_homebrew_the_same_on_both_cores", [] {
    emulate::pcode_cpu::arm_handover() = {};
    compare_homebrew("gba", "gba", ".gba", gba_file_offset);
    report_handover("gba");
});

CB_TEST("emulate.pcode_cpu.the_nds_lane_runs_homebrew_the_same_on_both_cores", [] {
    emulate::pcode_cpu::arm_handover() = {};
    compare_homebrew("nds", "nds", ".nds", nds_file_offset);
    report_handover("nds");
});

CB_TEST("emulate.pcode_cpu.the_3ds_lane_runs_homebrew_the_same_on_both_cores", [] {
    emulate::pcode_cpu::arm_handover() = {};
    compare_homebrew("n3ds", "3ds", ".3dsx", n3ds_file_offset);
    compare_homebrew("n3ds", "3ds", ".elf", n3ds_file_offset);
    report_handover("n3ds");
});

CB_TEST("emulate.pcode_cpu.the_vita_lane_runs_homebrew_the_same_on_both_cores", [] {
    emulate::pcode_cpu::arm_handover() = {};
    compare_homebrew("vita", "vita", ".bin", [](const stdfs::path&, std::uint32_t) { return SIZE_MAX; });   // (its files are packed: no edited copies)
    // What the adapter did not run itself: the steps handed to the own core by kind, and the runs it ended where the own core refuses a 32-bit Thumb instruction.
    const emulate::pcode_cpu::ArmHandover& handover = emulate::pcode_cpu::arm_handover();
    std::uint64_t handed = 0, refused = 0;
    for (const auto& [kind, steps] : handover.by_class) {
        handed += steps;
        std::printf("  handed to the own core: %llu x %s\n", static_cast<unsigned long long>(steps), kind.c_str());
    }
    for (const auto& [mnemonic, runs] : handover.refused) {
        refused += runs;
        std::printf("  run ended at a refused instruction: %llu x thumb2 %s\n", static_cast<unsigned long long>(runs), mnemonic.c_str());
    }
    std::printf("pcode_cpu vita: %llu steps on the emulator's side, %llu handed to the own core, %llu runs ended at a refused instruction\n",
                static_cast<unsigned long long>(handover.steps), static_cast<unsigned long long>(handed), static_cast<unsigned long long>(refused));
    CB_CHECK_EQ(handed, 0u);
});

// A tool, not a check: CB_GBA_TRACE=<rom>,<hex entry> (state 0 of the lane: registers zero, memory as the cartridge leaves it) finds the first instruction after which the
// two cores' runs differ and prints it with both cores' registers.
CB_TEST("emulate.pcode_cpu.arm_trace_the_first_divergence_of_a_gba_function", [] {
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
        const emulate::arm::RunResult result = pcode ? emulate::pcode_cpu::run(bus, where, options) : emulate::arm::run(bus, where, options);
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
        emulate::pcode_cpu::run(again, at, options);
        auto& emu = emulate::pcode_cpu::emulator_named("ARM6_le.sla");
        for (unsigned d = 0; d < 16; ++d) {
            const std::uint64_t own = static_cast<std::uint64_t>(cpu.vfp_single(2 * d + 1)) << 32 | cpu.vfp_single(2 * d);
            const std::uint64_t theirs = emu.reg("d" + std::to_string(d));
            if (own != theirs) std::printf("  d%u own %016llX emu %016llX\n", d, static_cast<unsigned long long>(own), static_cast<unsigned long long>(theirs));
        }
    }
});

// A tool, not a check: CB_PROBE_WORDS=<hex ARM word>,... prints how the ARMv6 description reads each one (an empty text: no instruction).
CB_TEST("emulate.pcode_cpu.arm_probe_how_the_description_reads_words", [] {
    const auto given = fs::get_env("CB_PROBE_WORDS");
    if (!given || given->empty()) return;
    auto& emu = emulate::pcode_cpu::emulator_named("ARM6_le.sla");
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
CB_TEST("emulate.pcode_cpu.the_vfp_arithmetic_is_the_same_on_both_cores", [] {
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
                    const ar::RunResult run = core == 1 ? emulate::pcode_cpu::run(bus, kCode, options) : ar::run(bus, kCode, options);
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
CB_TEST("emulate.pcode_cpu.the_vfp_conversions_are_the_same_on_both_cores", [] {
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
                const ar::RunResult run = core == 1 ? emulate::pcode_cpu::run(bus, kCode, options) : ar::run(bus, kCode, options);
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

// ---- the Vita's ARMv7-A (Thumb-2, IT blocks, the VFP and NEON rows) ---------------------------------------------------------------------------------------------

namespace {

namespace da = cb::decompile::arm;

constexpr std::uint32_t kVitaCode = 0x81000000, kVitaData = 0x82000000, kVitaStack = 0x83000000;   // (the Vita maps from $81000000)
constexpr std::uint32_t kVitaReturn = 0x100;

using Changes = std::vector<std::pair<std::uint32_t, std::uint8_t>>;

// What differs between two runs (empty: nothing); `ma` and `mb` are each core's list of changed bytes.
std::string describe_arm(const emulate::arm::RunResult& a, const emulate::arm::RunResult& b, const Changes& ma, const Changes& mb) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    for (unsigned i = 0; i < 16; ++i)
        if (a.r[i] != b.r[i]) out += std::format("r{}[own {:X} emu {:X}] ", i, a.r[i], b.r[i]);
    if ((a.cpsr & 0xF8000020u) != (b.cpsr & 0xF8000020u)) out += std::format("cpsr[{:08X}/{:08X}] ", a.cpsr, b.cpsr);
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (ma != mb) out += std::format("memory[{}/{}] ", ma.size(), mb.size());
    bool same_io = a.io.size() == b.io.size();
    for (std::size_t i = 0; same_io && i < a.io.size(); ++i)
        same_io = a.io[i].address == b.io[i].address && a.io[i].value == b.io[i].value && a.io[i].kind == b.io[i].kind && a.io[i].size == b.io[i].size;
    if (!same_io) out += "io ";
    bool same_calls = a.bios.size() == b.bios.size();
    for (std::size_t i = 0; same_calls && i < a.bios.size(); ++i) same_calls = a.bios[i].function == b.bios[i].function && a.bios[i].args == b.bios[i].args;
    if (!same_calls) out += "calls ";
    return out;
}

// A value for a register: a pointer into the data (aligned, or not), a small number, or any word.
std::uint32_t random_value(std::mt19937& rng, std::uint32_t data = kVitaData, std::uint32_t stack = kVitaStack) {
    switch (rng() % 8) {
        case 0: return 0;
        case 1: return rng() % 40;
        case 2: return data + (rng() % 0x3000 & ~3u);
        case 3: return data + (rng() % 0x3000);
        case 4: return stack + 0x100 + (rng() % 0x600 & ~3u);
        case 5: return 0xFFFFFFFFu - rng() % 8;
        default: return static_cast<std::uint32_t>(rng());
    }
}

// Whether an ARM instruction is one the manual defines for the registers it names: the pc as an operand or the base of a writing access, and the like, are UNPREDICTABLE
// there (and the cores need not agree on what they do), so the random programs leave them out.
bool defined_operands(const da::Insn& i, std::uint32_t w) {
    const auto pc = [](unsigned r) { return r == 15; };
    if (static_cast<unsigned>(i.op) < 16) {
        if (i.s && !(i.op == da::Op::Tst || i.op == da::Op::Teq || i.op == da::Op::Cmp || i.op == da::Op::Cmn) && pc(i.rd)) return false;   // (a return from an exception)
        if (pc(i.rd) && !i.imm_form && i.shift_by_reg) return false;
        if (!i.imm_form && (pc(i.rm) && i.shift_by_reg)) return false;
        if (!i.imm_form && i.shift_by_reg && pc(i.rs)) return false;
        return !(i.shift_by_reg && pc(i.rn));
    }
    switch (i.op) {
        case da::Op::Ldr: case da::Op::Str:
            if ((i.wb || !i.pre) && (pc(i.rn) || i.rn == i.rd)) return false;
            if ((i.wb || !i.pre) && !i.imm_form && i.rm == i.rn) return false;
            if (i.op == da::Op::Str && pc(i.rd)) return false;
            if (i.op == da::Op::Ldr && pc(i.rd) && (i.size != 4 || i.wb || !i.pre)) return false;
            return !(!i.imm_form && pc(i.rm));
        case da::Op::Ldm: case da::Op::Stm: return !pc(i.rn) && !i.hat && !(i.op == da::Op::Stm && (i.list & 0x8000));
        case da::Op::B: case da::Op::Bl: case da::Op::Swi: case da::Op::Mrs: case da::Op::Msr: case da::Op::Bkpt: case da::Op::Pld: case da::Op::Mcr: case da::Op::Mrc: return true;
        case da::Op::Blx: return i.target.has_value() || !pc(i.rm);
        case da::Op::Bx: return !pc(i.rm);
        case da::Op::Ext:
            if (i.ext < 0 || i.ext >= da::t2::kIt) return false;
            if (da::ext::mnemonic(i.ext).starts_with('v')) return true;   // (the VFP's and NEON's fields are register numbers of their own)
            return !(pc(w & 15) || pc(w >> 8 & 15) || pc(w >> 12 & 15) || pc(w >> 16 & 15));
        default: return !(pc(i.rd) || pc(i.rn) || pc(i.rm) || pc(i.rs));
    }
}

// One random instruction of the ARMv7-A that the own core's decoder reads, as bytes (Thumb: one or two halfwords; ARM: a word), and its text.
std::string random_instruction(std::mt19937& rng, bool thumb, Bytes& out, da::Arch arch = da::Arch::V7A, std::uint32_t where = kVitaCode, bool wide = false) {
    for (;;) {
        if (thumb) {
            const std::uint32_t a = wide ? 0xE800u + (rng() % 24) * 0x100u + rng() % 0x100u : rng() % 4 == 0 ? 0xE800u + rng() % 0x1800u : rng() % 0xE800u, b = rng() & 0xFFFF;   // (wide: only 32-bit instructions, every family of them alike)
            const std::optional<da::Insn> insn = da::decode_thumb(static_cast<std::uint16_t>(a), static_cast<std::uint16_t>(b), kVitaCode | 1, da::Arch::V7A);
            if (!insn || (insn->length == 2 && a >= 0xE800)) continue;
            if (insn->op == da::Op::Ext && insn->ext == da::t2::kIt) continue;   // (the programs put their own)
            out.push_back(static_cast<std::uint8_t>(a));
            out.push_back(static_cast<std::uint8_t>(a >> 8));
            if (insn->length == 4) {
                out.push_back(static_cast<std::uint8_t>(b));
                out.push_back(static_cast<std::uint8_t>(b >> 8));
            }
            return da::text(*insn);
        }
        std::uint32_t w = static_cast<std::uint32_t>(rng());
        if (wide) {   // (wide: the ARM families the plain random words rarely reach, the condition mostly "always")
            struct Family {
                std::uint32_t mask, value;
            };
            static constexpr Family kFamilies[] = {
                {0x0E000000, 0x08000000},   // LDM, STM
                {0x0C000000, 0x04000000},   // LDR, STR, LDRB, STRB
                {0x0E000090, 0x00000090},   // halfword, signed and doubleword transfers, SWP, multiplies
                {0x0F000010, 0x06000010},   // the media instructions
                {0x0F000010, 0x07000010},
                {0x0F9000F0, 0x01000050},   // QADD, QSUB, QDADD, QDSUB
                {0x0F9000F0, 0x01000080},   // SMLAxy, SMULxy
                {0x0FE000F0, 0x00000090},   // MUL, MLA
                {0x0F8000F0, 0x00800090},   // long multiplies
                {0x0E000000, 0x02000000},   // data processing, immediate
                {0x0E000010, 0x00000000},   // data processing, shifted by an immediate
                {0x0E000090, 0x00000010},   // data processing, shifted by a register
                {0x0FB00000, 0x03000000},   // MOVW, MOVT
                {0x0FFFFFFF, 0x01000000},
                {0xFF100000, 0xF4000000},   // Advanced SIMD structure stores and loads
                {0xFF100000, 0xF4000000},
                {0xFE800090, 0xF2800010},   // Advanced SIMD one register and a modified immediate
                {0x0FB00EF0, 0x0EB00A00},   // VMOV (immediate)
            };
            const Family& f = kFamilies[rng() % std::size(kFamilies)];
            w = (w & ~f.mask) | f.value;
            if (rng() % 8 != 0 && (w >> 28) != 15) w = (w & 0x0FFFFFFF) | 0xE0000000u;
        }
        const std::optional<da::Insn> insn = da::decode_arm(w, where, arch);
        if (!insn || !defined_operands(*insn, w)) continue;
        for (unsigned k = 0; k < 4; ++k) out.push_back(static_cast<std::uint8_t>(w >> (8 * k)));
        return da::text(*insn);
    }
}

// One program and the registers and memory it starts from.
struct VitaCase {
    Bytes code;
    bool thumb = true;
    std::array<std::uint32_t, 4> a{};
    std::array<std::uint32_t, 8> s{};
    unsigned seed = 0;   // (what the data and stack are filled from)
    std::uint64_t max_steps = 64;
};

// The case on both cores: results and the bytes each changed.
void run_vita_case(const VitaCase& one, emulate::arm::RunResult result[2], Changes memory[2]) {
    namespace ar = emulate::arm;
    std::mt19937 fill(one.seed * 7919u + 17u);
    Bytes data(0x4000), stack(0x1000);
    for (std::uint8_t& b : data) b = static_cast<std::uint8_t>(fill());
    for (std::uint8_t& b : stack) b = static_cast<std::uint8_t>(fill());
    const auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
    regions->push_back({kVitaCode, one.code});
    regions->push_back({kVitaData, data});
    regions->push_back({kVitaStack, stack});
    ar::RunOptions options;
    options.a = one.a;
    options.s = one.s;
    options.sp = kVitaStack + 0x800;
    options.return_address = kVitaReturn;
    options.max_steps = one.max_steps;
    const std::uint32_t entry = kVitaCode | (one.thumb ? 1u : 0u);
    for (int core = 0; core < 2; ++core) {
        ar::Bus bus(ar::System::Vita);
        bus.set_regions(regions);
        result[core] = core == 1 ? emulate::pcode_cpu::run(bus, entry, options) : ar::run(bus, entry, options);
        memory[core] = bus.memory().changes(1 << 20);
    }
}

// Programs of random instructions (Thumb ones with IT blocks of their own; `single`: one instruction, or one IT block, after a compare that sets the flags) run on both
// cores from random registers and memory; every result field and each byte changed is compared.
void compare_vita_random(const char* title, bool thumb, bool single, unsigned count, bool wide = false) {
    namespace ar = emulate::arm;
    std::mt19937 rng(thumb ? (single ? 31 : 32) + (wide ? 100 : 0) : 33);
    unsigned runs = 0, differing = 0;
    std::map<std::string, unsigned> per_instruction, ends;   // (how the runs ended: a test whose runs all stop at once proves nothing)
    for (unsigned n = 0; n < count; ++n) {
        Bytes code;
        std::string texts;
        const auto put = [&](std::uint16_t half) {
            code.push_back(static_cast<std::uint8_t>(half));
            code.push_back(static_cast<std::uint8_t>(half >> 8));
        };
        if (thumb) put(0x4288);   // cmp r0, r1: the flags the conditions meet
        const unsigned body = single ? 1 : 1 + rng() % 8;
        for (unsigned k = 0; k < body; ++k) {
            if (thumb && (single ? rng() % 2 == 0 : rng() % 4 == 0)) {   // IT <first condition> <mask>, then the instructions it covers
                const unsigned first = rng() % 15;
                const unsigned mask = 1 + rng() % 15;
                put(static_cast<std::uint16_t>(0xBF00 | first << 4 | mask));
                texts += std::format("it {:x} {:x}; ", first, mask);
                for (unsigned inner = 0, left = 4 - std::countr_zero(mask); inner < left; ++inner) texts += random_instruction(rng, true, code, da::Arch::V7A, kVitaCode, wide) + "; ";
                continue;
            }
            texts += random_instruction(rng, thumb, code, da::Arch::V7A, kVitaCode, wide) + "; ";
        }
        if (thumb) {
            put(0x4770);   // bx lr
        } else {
            const std::uint32_t bx_lr = 0xE12FFF1E;
            for (unsigned k = 0; k < 4; ++k) code.push_back(static_cast<std::uint8_t>(bx_lr >> (8 * k)));
        }
        VitaCase one;
        one.code = code;
        one.thumb = thumb;
        one.seed = n;
        for (std::uint32_t& v : one.a) v = random_value(rng);
        for (std::uint32_t& v : one.s) v = random_value(rng);
        ar::RunResult result[2];
        Changes memory[2];
        run_vita_case(one, result, memory);
        ++runs;
        std::string how = result[0].exit();
        if (how.starts_with("stopped: bus error")) how = "stopped: bus error";
        else if (how.starts_with("stopped: no instruction")) how = "stopped: no instruction";
        ++ends[how];
        const std::string what = describe_arm(result[0], result[1], memory[0], memory[1]);
        if (what.empty()) continue;
        ++differing;
        if (++per_instruction[texts.substr(0, 60)] == 1 && differing <= 25) {
            std::string hex;
            for (const std::uint8_t b : code) hex += std::format("{:02X}", b);
            std::printf("  %s: %s\n      code %s\n      %s\n", title, texts.c_str(), hex.c_str(), what.c_str());
            std::string regs = std::format("{}", thumb ? "thumb" : "arm");
            regs += "," + hex;
            for (const std::uint32_t v : one.a) regs += std::format(",{:x}", v);
            for (const std::uint32_t v : one.s) regs += std::format(",{:x}", v);
            std::printf("      CB_ARM_CASE=%s,%u\n", regs.c_str(), one.seed);
        }
    }
    std::printf("pcode_cpu %s: %u runs, %u differing\n", title, runs, differing);
    for (const auto& [how, times] : ends)
        if (times * 50 >= runs) std::printf("    %u x %s\n", times, how.c_str());
    CB_CHECK_EQ(differing, 0u);
}

}  // namespace

CB_TEST("emulate.pcode_cpu.vita_thumb_instructions_run_as_the_own_core_does", [] { compare_vita_random("vita thumb instructions", true, true, 20000); });

CB_TEST("emulate.pcode_cpu.vita_thumb_programs_run_as_the_own_core_does", [] { compare_vita_random("vita thumb programs", true, false, 10000); });

CB_TEST("emulate.pcode_cpu.vita_thumb32_instructions_run_as_the_own_core_does", [] { compare_vita_random("vita thumb 32-bit instructions", true, true, 40000, true); });

CB_TEST("emulate.pcode_cpu.vita_thumb32_programs_run_as_the_own_core_does", [] { compare_vita_random("vita thumb 32-bit programs", true, false, 20000, true); });

CB_TEST("emulate.pcode_cpu.vita_arm_families_run_as_the_own_core_does", [] { compare_vita_random("vita arm families", false, true, 40000, true); });

CB_TEST("emulate.pcode_cpu.vita_arm_instructions_run_as_the_own_core_does", [] { compare_vita_random("vita arm instructions", false, true, 10000); });

// A tool, not a check: CB_ARM_CASE=<thumb|arm>,<hex code>,<a0>,<a1>,<a2>,<a3>,<s0>...<s7>,<seed> (the values in hex, the seed in decimal; what a failure of the random
// tests prints) runs the one program on both cores and prints each instruction as the decoder reads it and every difference.
CB_TEST("emulate.pcode_cpu.arm_tool_runs_one_vita_case_on_both_cores", [] {
    namespace ar = emulate::arm;
    const auto given = fs::get_env("CB_ARM_CASE");
    if (!given || given->empty()) return;
    std::vector<std::string> field;
    for (std::size_t at = 0;;) {
        const std::size_t end = given->find(',', at);
        field.push_back(given->substr(at, end - at));
        if (end == std::string::npos) break;
        at = end + 1;
    }
    if (field.size() < 15) return;
    VitaCase one;
    one.thumb = field[0] == "thumb";
    for (std::size_t i = 0; i + 1 < field[1].size(); i += 2) one.code.push_back(static_cast<std::uint8_t>(std::strtoul(field[1].substr(i, 2).c_str(), nullptr, 16)));
    for (unsigned i = 0; i < 4; ++i) one.a[i] = static_cast<std::uint32_t>(std::strtoul(field[2 + i].c_str(), nullptr, 16));
    for (unsigned i = 0; i < 8; ++i) one.s[i] = static_cast<std::uint32_t>(std::strtoul(field[6 + i].c_str(), nullptr, 16));
    one.seed = static_cast<unsigned>(std::strtoul(field[14].c_str(), nullptr, 10));
    for (std::size_t at = 0; at < one.code.size();) {
        const std::uint32_t a = one.code[at] | one.code[at + 1] << 8, b = at + 3 < one.code.size() ? one.code[at + 2] | one.code[at + 3] << 8 : 0;
        const std::optional<da::Insn> insn = one.thumb ? da::decode_thumb(static_cast<std::uint16_t>(a), static_cast<std::uint16_t>(b), static_cast<std::uint32_t>(kVitaCode + at + 1), da::Arch::V7A)
                                                       : da::decode_arm(a | b << 16, static_cast<std::uint32_t>(kVitaCode + at), da::Arch::V7A);
        if (!insn) {
            std::printf("  +%zx: no instruction\n", at);
            break;
        }
        std::printf("  +%zx: op %d ext %d cond %u rd %u rn %u rm %u s %d imm_form %d len %u  %s\n", at, static_cast<int>(insn->op), static_cast<int>(insn->ext), insn->cond, insn->rd,
                    insn->rn, insn->rm, insn->s ? 1 : 0, insn->imm_form ? 1 : 0, insn->length, da::text(*insn).c_str());
        at += insn->length;
    }
    ar::RunResult result[2];
    Changes memory[2];
    run_vita_case(one, result, memory);
    for (int core = 0; core < 2; ++core) {
        std::printf("  %s: %s steps %llu cpsr %08X r0-r3 %X %X %X %X r4-r7 %X %X %X %X sp %X lr %X pc %X, %zu bytes changed\n", core == 0 ? "own" : "emu", result[core].exit().c_str(),
                    static_cast<unsigned long long>(result[core].steps), result[core].cpsr, result[core].r[0], result[core].r[1], result[core].r[2], result[core].r[3], result[core].r[4],
                    result[core].r[5], result[core].r[6], result[core].r[7], result[core].r[13], result[core].r[14], result[core].r[15], memory[core].size());
    }
    std::printf("  differences: %s\n", describe_arm(result[0], result[1], memory[0], memory[1]).c_str());
    if (describe_arm(result[0], result[1], memory[0], memory[1]).empty()) return;
    // the first step after which the two cores differ, and the instruction it ran (as the own core's state said it was)
    std::mt19937 fill(one.seed * 7919u + 17u);
    Bytes data(0x4000), stack(0x1000);
    for (std::uint8_t& b : data) b = static_cast<std::uint8_t>(fill());
    for (std::uint8_t& b : stack) b = static_cast<std::uint8_t>(fill());
    const auto byte_at = [&](std::uint32_t address) -> std::uint8_t {
        if (address >= kVitaCode && address - kVitaCode < one.code.size()) return one.code[address - kVitaCode];
        if (address >= kVitaData && address - kVitaData < data.size()) return data[address - kVitaData];
        if (address >= kVitaStack && address - kVitaStack < stack.size()) return stack[address - kVitaStack];
        return 0;
    };
    std::uint32_t before_pc = kVitaCode;
    bool before_thumb = one.thumb;
    for (std::uint64_t n = 1; n <= 64; ++n) {
        VitaCase part = one;
        part.max_steps = n;
        ar::RunResult r[2];
        Changes m[2];
        run_vita_case(part, r, m);
        const std::string what = describe_arm(r[0], r[1], m[0], m[1]);
        if (!what.empty()) {
            std::uint32_t w = 0;
            for (unsigned k = 0; k < 4; ++k) w |= static_cast<std::uint32_t>(byte_at((before_pc & ~1u) + k)) << (8 * k);
            const std::optional<da::Insn> insn = before_thumb ? da::decode_thumb(static_cast<std::uint16_t>(w), static_cast<std::uint16_t>(w >> 16), (before_pc & ~1u) | 1, da::Arch::V7A) : da::decode_arm(w, before_pc & ~3u, da::Arch::V7A);
            std::printf("  first difference at step %llu, running %08X (%s, word %08X): %s | %s\n", static_cast<unsigned long long>(n), before_pc, before_thumb ? "thumb" : "arm", w,
                        insn ? da::text(*insn).c_str() : "?", what.c_str());
            break;
        }
        before_pc = r[0].r[15];
        before_thumb = (r[0].cpsr & 0x20) != 0;
    }
});

// ---- the 3DO's ARM60 (big-endian words on the DS's ARM7 map, the machine_threedo lane) ------------------------------------------------------------------------------

namespace {

constexpr std::uint32_t kThreedoCode = 0x02000000, kThreedoData = 0x02200000, kThreedoStack = 0x023FFF00;

// The words of a big-endian program as the lane's bus holds them (each four bytes reversed).
Bytes reversed_words(const Bytes& big_endian) {
    Bytes out(big_endian.size() + (4 - big_endian.size() % 4) % 4, 0);
    std::copy(big_endian.begin(), big_endian.end(), out.begin());
    for (std::size_t i = 0; i < out.size(); i += 4) std::reverse(out.begin() + static_cast<std::ptrdiff_t>(i), out.begin() + static_cast<std::ptrdiff_t>(i) + 4);
    return out;
}

// What the 3DO lane's bus is: the main RAM holds the program and the work memory, Clio's registers answer a read with 0 and its vertical counter moves with each read.
void threedo_bus(emulate::arm::Bus& bus, const Bytes& program, const Bytes& data) {
    bus.set_big_endian(true);
    bus.load(kThreedoCode, reversed_words(program));
    bus.load(kThreedoData, reversed_words(data));
    const auto lines = std::make_shared<std::uint32_t>(0);
    bus.map(0x03300000, 0x033FFFFF, [](std::uint32_t, unsigned) -> std::uint32_t { return 0; });
    bus.map(0x03400000, 0x034FFFFF, [lines](std::uint32_t address, unsigned) -> std::uint32_t { return address == 0x03400014 ? (*lines)++ % 263 : 0; });
}

}  // namespace

// Programs of random ARM instructions (the ARMv4T the old core reads, so a BX into Thumb is run too) on the big-endian lane's bus, on both cores.
CB_TEST("emulate.pcode_cpu.threedo_programs_run_as_the_own_core_does", [] {
    namespace ar = emulate::arm;
    std::mt19937 rng(3003);
    unsigned runs = 0, differing = 0;
    double seconds[2] = {0, 0};
    std::map<std::string, unsigned> ends;
    const unsigned count = [] {
        const auto given = fs::get_env("CB_THREEDO_RANDOM");
        return given && !given->empty() ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 10000u;
    }();
    for (unsigned n = 0; n < count; ++n) {
        const bool single = n % 2 == 0;
        Bytes code;
        std::string texts;
        const auto put = [&](std::uint32_t word) {
            for (int k = 3; k >= 0; --k) code.push_back(static_cast<std::uint8_t>(word >> (8 * k)));   // (big-endian)
        };
        put(0xE1500001);   // cmp r0, r1: the flags the conditions meet
        const unsigned body = single ? 1 : 1 + rng() % 8;
        for (unsigned k = 0; k < body; ++k) {
            Bytes word;
            texts += random_instruction(rng, false, word, da::Arch::V4T, kThreedoCode) + "; ";
            for (int b = 3; b >= 0; --b) code.push_back(word[static_cast<std::size_t>(b)]);   // (the generator gives little-endian bytes)
        }
        put(0xE1A0F00E);   // mov pc, lr
        Bytes data(0x4000);
        std::mt19937 fill(n * 7919u + 5u);
        for (std::uint8_t& b : data) b = static_cast<std::uint8_t>(fill());
        ar::RunOptions options;
        for (std::uint32_t& v : options.a) v = random_value(rng, kThreedoData, kThreedoStack - 0x800);
        for (std::uint32_t& v : options.s) v = random_value(rng, kThreedoData, kThreedoStack - 0x800);
        options.sp = kThreedoStack;
        options.max_steps = 64;
        ar::RunResult result[2];
        Bytes memory[2];
        for (int core = 0; core < 2; ++core) {
            ar::Bus bus(ar::System::Nds7);
            threedo_bus(bus, code, data);
            const double start = cb::thread_cpu_seconds();
            result[core] = core == 1 ? emulate::pcode_cpu::run(bus, kThreedoCode, options) : ar::run(bus, kThreedoCode, options);
            seconds[core] += cb::thread_cpu_seconds() - start;
            const ByteSpan ram = bus.ewram();
            memory[core].assign(ram.begin(), ram.end());
        }
        ++runs;
        std::string how = result[0].exit();
        if (how.starts_with("stopped: bus error")) how = "stopped: bus error";
        else if (how.starts_with("stopped: no instruction")) how = "stopped: no instruction";
        ++ends[how];
        std::string what = describe_arm(result[0], result[1], {}, {});
        if (memory[0] != memory[1]) what += "memory ";
        if (what.empty()) continue;
        if (++differing <= 20) {
            std::string hex;
            for (const std::uint8_t b : code) hex += std::format("{:02X}", b);
            std::printf("  threedo: %s\n      code %s\n      %s\n", texts.c_str(), hex.c_str(), what.c_str());
        }
    }
    std::printf("pcode_cpu threedo programs: %u runs, %u differing; time own core %.2fs emulator %.2fs (the runs alone)\n", runs, differing, seconds[0], seconds[1]);
    for (const auto& [how, times] : ends)
        if (times * 50 >= runs) std::printf("    %u x %s\n", times, how.c_str());
    CB_CHECK_EQ(differing, 0u);
});

// The speed of the big-endian description against the own core on a loop that does memory and arithmetic (the lane has no homebrew to time), and the same result from both.
CB_TEST("emulate.pcode_cpu.threedo_loop_runs_as_the_own_core_does_and_how_fast", [] {
    namespace ar = emulate::arm;
    Bytes code;
    const auto put = [&](std::uint32_t word) {
        for (int k = 3; k >= 0; --k) code.push_back(static_cast<std::uint8_t>(word >> (8 * k)));
    };
    put(0xE3A03000);   // mov r3, #0
    put(0xE5921000);   // loop: ldr r1, [r2]
    put(0xE0833001);   // add r3, r3, r1
    put(0xE5C23004);   // strb r3, [r2, #4]
    put(0xE2800000 | 0x1);   // add r0, r0, #1 -- (the counter counts up to the limit in r4)
    put(0xE1500004);   // cmp r0, r4
    put(0x1AFFFFF9);   // bne loop
    put(0xE1A0F00E);   // mov pc, lr
    Bytes data(0x4000, 0x5A);
    ar::RunResult result[2];
    double seconds[2] = {0, 0};
    for (int core = 0; core < 2; ++core) {
        ar::Bus bus(ar::System::Nds7);
        threedo_bus(bus, code, data);
        ar::RunOptions options;
        options.a = {0, 0, kThreedoData, 0};
        options.s[0] = 30000;
        options.sp = kThreedoStack;
        options.max_steps = 1000000;
        const double start = cb::thread_cpu_seconds();
        result[core] = core == 1 ? emulate::pcode_cpu::run(bus, kThreedoCode, options) : ar::run(bus, kThreedoCode, options);
        seconds[core] = cb::thread_cpu_seconds() - start;
    }
    std::printf("pcode_cpu threedo loop: %llu steps, own core %.3fs emulator %.3fs\n", static_cast<unsigned long long>(result[0].steps), seconds[0], seconds[1]);
    CB_CHECK_EQ(describe_arm(result[0], result[1], {}, {}), std::string());
    CB_CHECK(result[0].steps > 100000);
});

// The speed on the lane's kind of work: 100 functions (random programs that reach their return), each run in four states with the same code, the way the lane runs a
// function (the lane has no homebrew to time). The runs alone are timed, not the making of the buses.
CB_TEST("emulate.pcode_cpu.threedo_functions_run_as_the_own_core_does_and_how_fast", [] {
    namespace ar = emulate::arm;
    std::mt19937 rng(77);
    unsigned functions = 0, runs = 0, differing = 0;
    double seconds[2] = {0, 0};
    std::uint64_t steps = 0;
    while (functions < 100) {
        Bytes code;
        const auto put = [&](std::uint32_t word) {
            for (int k = 3; k >= 0; --k) code.push_back(static_cast<std::uint8_t>(word >> (8 * k)));
        };
        put(0xE1500001);
        const unsigned body = 2 + rng() % 12;
        for (unsigned k = 0; k < body; ++k) {
            Bytes word;
            random_instruction(rng, false, word, da::Arch::V4T, kThreedoCode);
            for (int b = 3; b >= 0; --b) code.push_back(word[static_cast<std::size_t>(b)]);
        }
        put(0xE1A0F00E);
        Bytes data(0x4000);
        for (std::uint8_t& b : data) b = static_cast<std::uint8_t>(rng());
        std::array<std::array<std::uint32_t, 5>, 4> states{};
        for (auto& state : states)
            for (std::uint32_t& v : state) v = random_value(rng, kThreedoData, kThreedoStack - 0x800);
        // a function the lane would keep: it reaches its return in the first state
        {
            ar::Bus bus(ar::System::Nds7);
            threedo_bus(bus, code, data);
            ar::RunOptions options;
            options.a = {states[0][0], states[0][1], states[0][2], states[0][3]};
            options.s[0] = states[0][4];
            options.sp = kThreedoStack;
            options.max_steps = 64;
            if (ar::run(bus, kThreedoCode, options).end != ar::End::Return) continue;
        }
        ++functions;
        for (const auto& state : states) {
            ar::RunResult result[2];
            for (int core = 0; core < 2; ++core) {
                ar::Bus bus(ar::System::Nds7);
                threedo_bus(bus, code, data);
                ar::RunOptions options;
                options.a = {state[0], state[1], state[2], state[3]};
                options.s[0] = state[4];
                options.sp = kThreedoStack;
                options.max_steps = 20000;
                const double start = cb::thread_cpu_seconds();
                result[core] = core == 1 ? emulate::pcode_cpu::run(bus, kThreedoCode, options) : ar::run(bus, kThreedoCode, options);
                seconds[core] += cb::thread_cpu_seconds() - start;
            }
            ++runs;
            steps += result[0].steps;
            if (!describe_arm(result[0], result[1], {}, {}).empty()) ++differing;
        }
    }
    std::printf("pcode_cpu threedo functions: %u functions, %u runs, %llu steps, %u differing; time own core %.3fs emulator %.3fs\n", functions, runs,
                static_cast<unsigned long long>(steps), differing, seconds[0], seconds[1]);
    CB_CHECK_EQ(differing, 0u);
});

// The lane's whole effects call (the program read, the bus made, the memory compared) on the two functions of a small AIF, 13 times each in each of the lane's states
// (104 runs), on both cores: what a lane run costs the user (no 3DO homebrew exists to take 100 functions from).
CB_TEST("emulate.pcode_cpu.threedo_lane_effects_are_the_same_on_both_cores_and_how_fast", [] {
    fs::ScopedTempDir dir("cb_pcode_threedo_effects");
    Bytes f(0x68, 0);
    const auto put_be = [&](std::size_t at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) f[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
    };
    for (const std::size_t at : {0u, 4u, 8u}) put_be(at, 0xE1A00000);
    put_be(12, 0xEB00000B);
    put_be(16, 0xEF000011);
    put_be(20, 0x68);
    put_be(48, 0x20);
    put_be(0x40, 0xE92D4010);   // push {r4, lr}
    put_be(0x44, 0xE3A04001);   // mov r4, #1
    put_be(0x48, 0xEB000003);   // bl sub
    put_be(0x4C, 0xE59F1004);   // ldr r1, [pc, #4]
    put_be(0x50, 0xE8BD8010);   // pop {r4, pc}
    put_be(0x54, 0x12345678);
    put_be(0x58, 0x03400014);
    put_be(0x5C, 0xE5910000);   // sub: ldr r0, [r1]
    put_be(0x60, 0xE5C20001);   // strb r0, [r2, #1]
    put_be(0x64, 0xE1A0F00E);   // mov pc, lr
    const stdfs::path path = dir.path() / "LaunchMe";
    fs::write_file(path, f);
    const verify::lanes::ImageLaneSpec spec = engine::threedo_lane_spec();
    double seconds[2] = {0, 0};
    unsigned runs = 0, differing = 0;
    for (unsigned rep = 0; rep < 13; ++rep)
        for (const std::uint32_t address : {0x40u, 0x5Cu})
            for (const verify::lanes::State& state : spec.states) {
                verify::lanes::Effects e[2];
                for (int core = 0; core < 2; ++core) {
                    emulate::pcode_compare::ForcedCore forced(core == 1);
                    const double start = cb::thread_cpu_seconds();
                    e[core] = spec.effects(path, address, state);
                    seconds[core] += cb::thread_cpu_seconds() - start;
                }
                ++runs;
                if (!(e[0] == e[1])) ++differing;
            }
    std::printf("pcode_cpu threedo lane: %u runs of 2 functions, %u differing; time own core %.3fs emulator %.3fs\n", runs, differing, seconds[0], seconds[1]);
    CB_CHECK_EQ(differing, 0u);
});
