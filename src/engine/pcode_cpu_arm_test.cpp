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
#include "engine/lane_kit.hpp"
#include "engine/pcode_cpu.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

struct ArmForced {
    explicit ArmForced(bool on) { engine::pcode_cpu::force(on); }
    ~ArmForced() { engine::pcode_cpu::force(std::nullopt); }
};

}  // namespace

CB_TEST("engine.pcode_cpu.the_gba_lane_runs_homebrew_the_same_on_both_cores", [] {
    fs::ScopedTempDir dir("cb_pcode_arm_gba");
    std::optional<fs::ScopedEnv> scoped;
    std::unique_ptr<engine::LaneKit> kit;
    try {
        const config::Config config = config::Config::load(config::current().main_checkout());
        scoped.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        kit = std::make_unique<engine::LaneKit>(config::Config::load(config.checkout_root()));
    } catch (const Error&) {
        return;
    }
    const std::shared_ptr<const verify::lanes::Kind> kind = kit->machine_lane("gba");
    const auto* lane = dynamic_cast<const verify::lanes::ImageLane*>(kind.get());
    if (lane == nullptr) return;
    const verify::lanes::ImageLaneSpec& spec = lane->spec();
    const stdfs::path root = testing::beside_checkout("homebrew-samples");
    const stdfs::path samples = stdfs::exists(root) ? root : testing::beside_checkout("test-inputs/homebrew-samples");
    unsigned functions = 0, runs = 0, differ = 0, files = 0, judged = 0, disagree = 0;
    unsigned verdicts[2][2] = {};
    double seconds[2] = {0, 0};
    std::map<std::string, unsigned> reasons;
    std::map<std::string, std::string> reason_example;
    std::error_code ec;
    for (const auto& dirent : stdfs::recursive_directory_iterator(samples / "gba", ec)) {
        if (dirent.path().extension() != ".gba") continue;
        if (++files > 16) break;
        const stdfs::path work = dir.path() / std::to_string(files);
        stdfs::create_directories(work);
        std::unique_ptr<verify::lanes::Prepared> prepared;
        try {
            prepared = lane->prepare(dirent.path(), work / "work");
        } catch (const Error&) {
            continue;
        }
        const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
        const Bytes original_bytes = fs::read_file(dirent.path());
        unsigned per_file = 0;
        for (const auto& [address, name] : image.written.functions) {
            if (per_file++ == 20) break;
            ++functions;
            const std::size_t at = (address & 0x01FFFFFEu);
            for (std::size_t k = 0; k < 3 && at + k < original_bytes.size(); ++k) {   // against copies with one of the function's first three bytes turned over
                Bytes changed = original_bytes;
                changed[at + k] ^= 0xFF;
                const stdfs::path edited = work / "edited.gba";
                fs::write_file(edited, changed);
                verify::Judgement j[2];
                for (int core = 0; core < 2; ++core) {
                    ArmForced forced(core == 1);
                    j[core] = lane->judge_function(false, dirent.path(), edited, address);
                    ++verdicts[core][j[core].verdict == verify::Verdict::Equivalent ? 0 : 1];
                }
                ++judged;
                if (j[0].verdict != j[1].verdict && ++disagree <= 4)
                    std::printf("  %s: function %x byte +%zu: own core %s (%s), emulator %s (%s)\n", dirent.path().filename().string().c_str(), address, k,
                                std::string(verify::word(j[0].verdict)).c_str(), j[0].reason.c_str(), std::string(verify::word(j[1].verdict)).c_str(), j[1].reason.c_str());
            }
            for (const verify::lanes::State& state : spec.states) {
                verify::lanes::Effects e[2];
                for (int core = 0; core < 2; ++core) {
                    ArmForced forced(core == 1);
                    const auto start = std::chrono::steady_clock::now();
                    e[core] = spec.effects(dirent.path(), address, state);
                    seconds[core] += std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
                }
                ++runs;
                if (e[0] == e[1]) continue;
                ++differ;
                std::string what;
                for (std::size_t i = 0; i < e[0].size() && i < e[1].size(); ++i) {
                    if (e[0][i] == e[1][i]) continue;
                    what += std::format(" [{}]", i < spec.what.size() ? spec.what[i] : std::to_string(i));
                    if (const auto* a = std::get_if<std::string>(&e[0][i]))
                        if (const auto* b = std::get_if<std::string>(&e[1][i])) what += std::format(" own '{}' emulator '{}'", *a, *b);
                    if (const auto* la = std::get_if<std::vector<std::string>>(&e[0][i]))
                        if (const auto* lb = std::get_if<std::vector<std::string>>(&e[1][i]))
                            if (differ <= 8) {
                                std::size_t j = 0;
                                while (j < la->size() && j < lb->size() && (*la)[j] == (*lb)[j]) ++j;
                                std::printf("  %s %x: [%s] first line %zu own '%s' (%zu) emulator '%s' (%zu)\n", dirent.path().filename().string().c_str(), address, spec.what[i].c_str(), j,
                                            j < la->size() ? (*la)[j].c_str() : "-", la->size(), j < lb->size() ? (*lb)[j].c_str() : "-", lb->size());
                            }
                }
                ++reasons[what.substr(0, 130)];
                if (!reason_example.count(what.substr(0, 130))) reason_example[what.substr(0, 130)] = std::format("{} {:x} state {}", dirent.path().filename().string(), address, &state - &spec.states[0]);
            }
        }
    }
    std::printf("pcode_cpu gba homebrew: %u programs, %u functions, %u runs, effects differing %u; edited programs judged %u: own core equivalent %u divergent %u, emulator equivalent %u "
                "divergent %u, verdicts disagreeing %u; time own core %.2fs emulator %.2fs\n",
                files, functions, runs, differ, judged, verdicts[0][0], verdicts[0][1], verdicts[1][0], verdicts[1][1], disagree, seconds[0], seconds[1]);
    unsigned shown = 0;
    for (const auto& [k, c] : reasons)
        if (++shown <= 14) std::printf("    %u x%s   e.g. %s\n", c, k.c_str(), reason_example[k].c_str());
    CB_CHECK_EQ(differ, 0u);
    CB_CHECK_EQ(disagree, 0u);
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
    const auto prepare = [&](emulate::arm::Bus& bus) {
        bus.set_rom(rom_file);
        const auto scanline = std::make_shared<std::uint32_t>(0);
        bus.map(0x04000004, 0x04000007, [scanline](std::uint32_t address, unsigned) -> std::uint32_t {
            const std::uint32_t line = (*scanline)++ % 228;
            const std::uint32_t word = ((line >= 160 && line < 227) ? 1u : 0u) | line << 16;
            return word >> (8 * (address - 0x04000004));
        });
        bus.map(0x04000130, 0x04000131, [](std::uint32_t address, unsigned) -> std::uint32_t { return address == 0x04000130 ? 0xFF : 0x03; });
    };
    // the lane's states by number (CB_GBA_STATE, default 0): the registers and the seed the work RAM is filled from
    const unsigned state = [] {
        const auto s = fs::get_env("CB_GBA_STATE");
        return s && !s->empty() ? static_cast<unsigned>(std::strtoul(s->c_str(), nullptr, 10)) : 0u;
    }();
    constexpr std::uint32_t kWork = 0x02010000;
    const std::array<std::array<std::uint32_t, 5>, 4> kStates = {{{0, 0, 0, 0, 0}, {kWork, kWork + 0x1000, 5, 0x10, 0}, {kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3}, {2, 3, kWork + 0x100, kWork + 0x200, 7}}};
    const auto fill = [&](emulate::arm::Bus& bus) {
        if (state == 0) return;
        for (std::size_t i = 0; i < 0x10000; ++i)
            bus.ewram()[kWork - emulate::arm::kEwramBase + i] =
                static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(state) * 13) ^ (i >> 3)) & (state % 2 != 0 ? 0x0F : 0xFF));
    };
    Bytes memory_a, memory_b;   // (the machine's RAMs after each core's last run)
    const auto go = [&](bool pcode, std::uint64_t steps) {
        emulate::arm::Bus bus;
        prepare(bus);
        fill(bus);
        emulate::arm::RunOptions options;
        options.max_steps = steps;
        options.a = {kStates[state][0], kStates[state][1], kStates[state][2], kStates[state][3]};
        options.s[0] = kStates[state][4];
        const emulate::arm::RunResult result = pcode ? engine::pcode_cpu::run(bus, entry, options) : emulate::arm::run(bus, entry, options);
        Bytes& out = pcode ? memory_b : memory_a;
        out.clear();
        for (const ByteSpan region : {bus.ewram(), bus.iwram(), bus.palette(), bus.vram(), bus.oam(), bus.sram()}) out.insert(out.end(), region.begin(), region.end());
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
            std::printf("  memory differs at region offset %zx: own %02X emulator %02X\n", i, memory_a[i], memory_b[i]);
            break;
        }
    std::printf("arm trace: first difference after step %llu: own %s | emulator %s\n", static_cast<unsigned long long>(hi), a.exit().c_str(), b.exit().c_str());
    std::printf("  before: pc %X cpsr %08X r0-r3 %X %X %X %X\n", before.r[15], before.cpsr, before.r[0], before.r[1], before.r[2], before.r[3]);
    for (unsigned i = 0; i < 16; ++i)
        if (a.r[i] != b.r[i]) std::printf("  r%u own %X emu %X\n", i, a.r[i], b.r[i]);
    std::printf("  cpsr own %08X emu %08X; io %zu/%zu\n", a.cpsr, b.cpsr, a.io.size(), b.io.size());
    emulate::arm::Bus bus;
    prepare(bus);
    fill(bus);
    emulate::arm::Cpu cpu(bus);
    for (unsigned i = 0; i < 4; ++i) cpu.r[i] = kStates[state][i];
    cpu.r[4] = kStates[state][4];
    cpu.r[13] = 0x03007F00;
    cpu.r[14] = emulate::arm::kReturnAddress;
    cpu.jump_to(entry);
    std::uint32_t pc = 0;
    for (std::uint64_t i = 0; i < hi; ++i) {
        pc = cpu.r[15];
        if (cpu.step()) break;
    }
    std::printf("  instruction at %08X (thumb %d): halfword/word %08X\n", pc, cpu.thumb() ? 1 : 0, bus.read(pc & ~3u, 4));
});
