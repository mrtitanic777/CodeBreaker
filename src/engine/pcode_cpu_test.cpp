// The lanes' 6502 on the one emulator (engine/pcode_cpu): the same run as the own core on random programs, and the Atari 2600 lane's verdicts on real
// homebrew the same with either core.
#include "engine/pcode_cpu.hpp"

#include <algorithm>
#include <cstdio>
#include <format>
#include <random>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/m6502.hpp"
#include "engine/machine_atari2600.hpp"
#include "formats/atari2600.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;
namespace m = cb::emulate::m6502;

namespace {

struct Forced {
    explicit Forced(bool on) { engine::pcode_cpu::force(on); }
    ~Forced() { engine::pcode_cpu::force(std::nullopt); }
};

std::string describe(const m::RunResult& r) {
    return std::format("{} A={:02X} X={:02X} Y={:02X} P={:02X} S={:02X} pc={:04X} steps={} io={}", r.exit(), r.regs.a, r.regs.x, r.regs.y, r.regs.p, r.regs.s,
                       r.regs.pc, r.steps, r.io.size());
}

bool same_run(const m::RunResult& a, const m::RunResult& b) {
    if (a.exit() != b.exit() || a.regs.a != b.regs.a || a.regs.x != b.regs.x || a.regs.y != b.regs.y || a.regs.s != b.regs.s || a.regs.p != b.regs.p || a.steps != b.steps) return false;
    if (a.io.size() != b.io.size()) return false;
    for (std::size_t i = 0; i < a.io.size(); ++i)
        if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) return false;
    return true;
}

}  // namespace

// A random program of documented opcodes at $8000, with random bytes after it, a device range at $4000-$40FF and RAM elsewhere.
CB_TEST("engine.pcode_cpu.runs_random_programs_as_the_own_core_does", [] {
    std::vector<std::uint8_t> documented;
    const auto& table = decompile::m6502::opcode_table(decompile::m6502::Variant::Nmos);
    for (unsigned op = 0; op < 256; ++op)
        if (table[op]) documented.push_back(static_cast<std::uint8_t>(op));
    std::mt19937 rng(20261120);
    unsigned different = 0, runs = 0, stopped = 0, returned = 0;
    for (int n = 0; n < 1500; ++n) {
        Bytes image(0x10000, 0);
        for (std::size_t i = 0; i < 0x10000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        for (std::size_t i = 0x8000; i < 0x8040; ++i) image[i] = documented[rng() % documented.size()];
        m::RunOptions options;
        options.a = static_cast<std::uint8_t>(rng());
        options.x = static_cast<std::uint8_t>(rng());
        options.y = static_cast<std::uint8_t>(rng());
        options.p = static_cast<std::uint8_t>(m::status::U | (rng() & 0xCF));
        options.max_steps = 200;
        const auto go = [&](bool pcode, Bytes& memory) {
            m::Bus bus(memory);
            bus.map(0x4000, 0x40FF, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 7 + 3); }, [](std::uint16_t, std::uint8_t) {});
            const m::RunResult r = pcode ? engine::pcode_cpu::run(bus, 0x8000, options) : m::run(bus, 0x8000, options);
            memory.assign(bus.memory().begin(), bus.memory().end());
            return r;
        };
        Bytes ma = image, mb = image;
        const m::RunResult own = go(false, ma);
        const m::RunResult emu = go(true, mb);
        ++runs;
        (own.end == m::End::Stopped ? stopped : returned) += 1;
        const bool memory_same = ma == mb;
        if (!same_run(own, emu) || !memory_same) {
            if (++different <= 8) std::printf("  run %d differs\n    own: %s\n    emu: %s\n    memory %s\n", n, describe(own).c_str(), describe(emu).c_str(), memory_same ? "same" : "DIFFERS");
        }
    }
    std::printf("pcode_cpu random programs: %u runs (%u ended stopped), %u differ\n", runs, stopped, different);
    // What is left (0.5%): stack pushes at S=0 (the description's SP is 16 bits: a JSR there pushes its second byte below page 1) and code run
    // from inside a device range (the own core's instruction fetches are logged as device reads). Neither occurs in a lane's functions.
    CB_CHECK(different * 100 <= runs);
});

// The judged functions of real homebrew: each function of each cartridge judged against itself and against a copy with its first byte turned over,
// once on each core; the counts are printed and the verdicts, reasons and the effects of every run must agree.
CB_TEST("engine.pcode_cpu.the_atari2600_lane_judges_homebrew_the_same_on_both_cores", [] {
    const stdfs::path root = "C:/Users/mrtit/Documents/GitHub/homebrew-samples";
    if (!stdfs::exists(root)) return;
    const std::vector<stdfs::path> files = {root / "atari2600-pipes/pipe2600.bin", root / "atari2600-kibalchish/kibalchish_NTSC.a26", root / "atari2600-kibalchish/kibalchish_PAL.a26",
                                            root / "atari2600-kibalchish/kibalchish_PAL60.a26", root / "atari2600-flappyboot/boot.bin", root / "atari2600-legendary-spear/LegendarySpear_NTSC.bin"};
    const verify::lanes::ImageLaneSpec spec = engine::atari2600_lane_spec();
    const verify::lanes::ImageLane lane(spec);
    fs::ScopedTempDir dir("cb_pcode_cpu");
    unsigned functions = 0, disagree = 0, effects_differ = 0;
    unsigned verdicts[2][2] = {};   // [core][equivalent, divergent]
    unsigned file_no = 0;
    for (const stdfs::path& file : files) {
        if (!stdfs::exists(file)) continue;
        const stdfs::path work = dir.path() / std::to_string(file_no++);
        stdfs::create_directories(work);
        const verify::lanes::Written written = spec.writer(file, work);
        const formats::atari2600::Rom rom = formats::atari2600::read(file);
        for (const auto& [entry, name] : written.functions) {
            ++functions;
            Bytes changed = fs::read_file(file);
            const std::size_t offset = (entry >> 16) * rom.bank_size + ((entry & 0xFFFF) & 0xFFF) % rom.bank_size;
            if (offset >= changed.size()) continue;
            changed[offset] ^= 0xFF;
            const stdfs::path edited = work / "edited.bin";
            fs::write_file(edited, changed);
            verify::Judgement judged[2][2];
            for (int core = 0; core < 2; ++core) {
                Forced forced(core == 1);
                judged[core][0] = lane.judge_function(false, file, file, entry);
                judged[core][1] = lane.judge_function(false, file, edited, entry);
                for (int k = 0; k < 2; ++k) ++verdicts[core][judged[core][k].verdict == verify::Verdict::Equivalent ? 0 : 1];
            }
            for (int k = 0; k < 2; ++k)
                if (judged[0][k].verdict != judged[1][k].verdict || judged[0][k].reason != judged[1][k].reason) ++disagree;
            for (const verify::lanes::State& state : spec.states) {
                verify::lanes::Effects e[2];
                for (int core = 0; core < 2; ++core) {
                    Forced forced(core == 1);
                    e[core] = spec.effects(file, entry, state);
                }
                if (e[0] != e[1]) {
                    if (++effects_differ <= 5) std::printf("  effects differ: %s entry %x\n", file.filename().string().c_str(), entry);
                }
            }
        }
    }
    std::printf("pcode_cpu atari2600 homebrew: %u functions; own core equivalent %u divergent %u; emulator equivalent %u divergent %u; verdicts disagreeing %u; runs with other effects %u\n",
                functions, verdicts[0][0], verdicts[0][1], verdicts[1][0], verdicts[1][1], disagree, effects_differ);
    CB_CHECK_EQ(disagree, 0u);
    CB_CHECK_EQ(effects_differ, 0u);
});
