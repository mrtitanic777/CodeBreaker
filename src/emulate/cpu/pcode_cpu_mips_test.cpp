// The PlayStation lane's R3000A on the one emulator (engine/pcode_cpu_mips.cpp): one instruction at a time from random registers and memory on each core,
// and the PS1 lane's homebrew run in every state on both cores.
#include <chrono>
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <vector>

#include "base/config.hpp"
#include "core/cpu_time.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/mips.hpp"
#include "emulate/mips.hpp"
#include "formats/psx.hpp"
#include "engine/lane_kit.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "emulate/cpu/pcode_ops_mips.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;
namespace mp = cb::emulate::mips;

namespace {

struct MipsForced {
    explicit MipsForced(bool on) { emulate::pcode_cpu::force(on); }
    ~MipsForced() { emulate::pcode_cpu::force(std::nullopt); }
};

bool mips_same(const mp::RunResult& a, const mp::RunResult& b) {
    if (a.exit() != b.exit() || a.r != b.r || a.hi != b.hi || a.lo != b.lo || a.gte_data != b.gte_data || a.gte_control != b.gte_control || a.steps != b.steps || a.io.size() != b.io.size() || a.bios.size() != b.bios.size()) return false;
    for (std::size_t i = 0; i < a.io.size(); ++i)
        if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) return false;
    return true;
}

}  // namespace

// A random word (one the R3000A has) then random words, at $80010000 in RAM, registers pointing into RAM or random; the run of one instruction (its delay slot with it)
// then of a few dozen. What differs is listed by the word's opcode and function.
CB_TEST("emulate.pcode_cpu.mips_instructions_run_as_the_own_core_does", [] {
    std::mt19937 rng(20261120);
    std::map<std::string, unsigned> by_kind, tried;
    std::map<std::string, std::string> example;
    unsigned different = 0, runs = 0, known_class = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 6000u;
    }();
    for (unsigned n = 0; n < total; ++n) {
        Bytes image(0x200000, 0);
        for (std::size_t i = 0x100000; i < 0x110000; ++i) image[i] = static_cast<std::uint8_t>(rng());   // data
        const auto valid_word = [&] {
            std::uint32_t w = static_cast<std::uint32_t>(rng());
            while (!decompile::mips::decode_word(w, 0x80010000)) w = static_cast<std::uint32_t>(rng());
            return w;
        };
        const std::uint32_t word = valid_word();
        for (std::size_t i = 0; i < 0x40; i += 4) {
            const std::uint32_t w = i == 0 ? word : valid_word();
            for (int b = 0; b < 4; ++b) image[0x10000 + i + b] = static_cast<std::uint8_t>(w >> (8 * b));
        }
        mp::RunOptions options;
        options.max_steps = n % 2 == 0 ? 2 : 40;
        options.sp = 0x801FFF00;
        for (auto& v : options.a) v = (rng() & 1) ? 0x80100000u + (rng() & 0xFFFC) : static_cast<std::uint32_t>(rng());
        for (auto& v : options.s) v = (rng() & 1) ? 0x80100000u + (rng() & 0xFFFC) : static_cast<std::uint32_t>(rng() & 0xFF);
        for (auto& v : options.t) v = (rng() & 1) ? 0x80100000u + (rng() & 0xFFFC) : static_cast<std::uint32_t>(rng() & 0xFF);
        const auto go = [&](bool pcode) {
            Bytes memory = image;
            mp::Bus bus{ByteView(memory)};
            const mp::RunResult r = pcode ? emulate::pcode_cpu::run(bus, 0x80010000, options) : mp::run(bus, 0x80010000, options);
            return std::make_pair(r, Bytes(bus.memory().begin(), bus.memory().end()));
        };
        const auto [own, ma] = go(false);
        const auto [emu, mb] = go(true);
        ++runs;
        const std::string plain_key = std::format("{}{:02X}.{:02X}", options.max_steps == 2 ? "1:" : "N:", word >> 26, (word >> 26) == 0 ? word & 0x3F : (word >> 16) & 31);
        ++tried[plain_key];
        if (mips_same(own, emu) && ma == mb) continue;
        ++different;
        // A branch whose delay slot is a branch: the description cannot decode the pair (SLEIGH has no nested delay slots); real code does not have it.
        const auto branch_word = [](std::uint32_t w) {
            const unsigned o = w >> 26;
            return (o == 0 && ((w & 0x3F) == 0x08 || (w & 0x3F) == 0x09)) || (o >= 1 && o <= 7);
        };
        const auto word_at = [&](std::size_t i) {
            return static_cast<std::uint32_t>(image[0x10000 + 4 * i]) | (static_cast<std::uint32_t>(image[0x10001 + 4 * i]) << 8) | (static_cast<std::uint32_t>(image[0x10002 + 4 * i]) << 16) |
                   (static_cast<std::uint32_t>(image[0x10003 + 4 * i]) << 24);
        };
        bool nested = false;
        for (std::size_t i = 0; i + 1 < 16; ++i) nested = nested || (branch_word(word_at(i)) && (branch_word(word_at(i + 1)) || (word_at(i + 1) >> 26) == 0x10));
        const std::string key = nested ? "a branch or a coprocessor 0 instruction in a branch's delay slot" : plain_key;
        if (key != plain_key) ++known_class;
        ++by_kind[key];
        if (!example.count(key)) {
            std::string fields;
            if (own.exit() != emu.exit()) fields += std::format("exit('{}' / '{}') ", own.exit(), emu.exit());
            for (unsigned i = 0; i < 32; ++i)
                if (own.r[i] != emu.r[i]) fields += std::format("r{}[own {:X} emu {:X}] ", i, own.r[i], emu.r[i]);
            if (own.hi != emu.hi || own.lo != emu.lo) fields += std::format("hilo[own {:X}:{:X} emu {:X}:{:X}] ", own.hi, own.lo, emu.hi, emu.lo);
            if (own.steps != emu.steps) fields += std::format("steps {}/{} ", own.steps, emu.steps);
            if (ma != mb) fields += "memory ";
            const std::uint32_t slot = static_cast<std::uint32_t>(image[0x10004]) | (static_cast<std::uint32_t>(image[0x10005]) << 8) | (static_cast<std::uint32_t>(image[0x10006]) << 16) | (static_cast<std::uint32_t>(image[0x10007]) << 24);
            if (!nested && options.max_steps != 2) {   // the first instruction after which the two cores' runs differ, and the word there
                unsigned k0 = 0;
                for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                    mp::RunOptions o2 = options;
                    o2.max_steps = k;
                    Bytes xa = image, xb = image;
                    mp::Bus busa{ByteView(xa)}, busb{ByteView(xb)};
                    const mp::RunResult ra = mp::run(busa, 0x80010000, o2), rb = emulate::pcode_cpu::run(busb, 0x80010000, o2);
                    if (!mips_same(ra, rb)) {
                        k0 = k;
                        for (unsigned i = 0; i < 32; ++i)
                            if (ra.r[i] != rb.r[i]) fields += std::format("{{at step {}: r{} own {:X} emu {:X} steps {}/{}}} ", k, i, ra.r[i], rb.r[i], ra.steps, rb.steps);
                    }
                }
                if (k0 != 0) {
                    Bytes xa = image;
                    mp::Bus bus{ByteView(xa)};
                    mp::Cpu cpu(bus);
                    for (unsigned i = 0; i < 4; ++i) cpu.r[4 + i] = options.a[i];
                    for (unsigned i = 0; i < 8; ++i) cpu.r[16 + i] = options.s[i];
                    for (unsigned i = 0; i < 8; ++i) cpu.r[8 + i] = options.t[i];
                    cpu.r[29] = options.sp;
                    cpu.r[31] = mp::kReturnAddress;
                    cpu.jump_to(0x80010000);
                    std::uint32_t at = 0;
                    for (unsigned i = 0; i < k0; ++i) {
                        at = cpu.pc;
                        if (cpu.step()) break;
                    }
                    const std::uint32_t p = at & 0x1FFFFFFF;
                    const std::uint32_t w = p + 8 < image.size() ? word_at((p - 0x10000) / 4) : 0;
                    const std::uint32_t before = p >= 0x10004 ? word_at((p - 0x10000) / 4 - 1) : 0;
                    fields += std::format("[step {} at {:X}: word {:08X} after {:08X}] ", k0, at, w, before);
                }
            }
            example[key] = std::format("{:08X} {:08X}: {}", word, slot, fields);
        }
    }
    std::printf("pcode_cpu mips instructions: %u runs, %u differ\n", runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 60) std::printf("    %s %u of %u: %s\n", k.c_str(), c, tried[k], example[k].c_str());
    CB_CHECK_EQ(different, 0u);
    CB_CHECK_EQ(known_class, 0u);
});

CB_TEST("emulate.pcode_cpu.the_ps1_lane_runs_homebrew_the_same_on_both_cores", [] {
    fs::ScopedTempDir dir("cb_pcode_mips_ps1");
    std::optional<fs::ScopedEnv> scoped;
    std::unique_ptr<engine::LaneKit> kit;
    try {
        const config::Config config = config::Config::load(config::current().main_checkout());
        scoped.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        kit = std::make_unique<engine::LaneKit>(config::Config::load(config.checkout_root()));
    } catch (const Error&) {
        return;
    }
    const std::shared_ptr<const verify::lanes::Kind> kind = kit->machine_lane("ps1");
    const auto* lane = dynamic_cast<const verify::lanes::ImageLane*>(kind.get());
    if (lane == nullptr) return;
    const verify::lanes::ImageLaneSpec& spec = lane->spec();
    const stdfs::path root = testing::beside_checkout("homebrew-samples");
    const stdfs::path samples = stdfs::exists(root) ? root : testing::beside_checkout("test-inputs/homebrew-samples");
    unsigned functions = 0, runs = 0, differ = 0, unexplained = 0, files = 0, judged = 0, disagree = 0;
    unsigned verdicts[2][2] = {};
    double seconds[2] = {0, 0};
    std::map<std::string, unsigned> reasons;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(samples / "ps1" / "peterlemon", ec)) {
        if (entry.path().extension() != ".exe") continue;
        if (++files > 24) break;
        const stdfs::path work = dir.path() / std::to_string(files);
        stdfs::create_directories(work);
        std::unique_ptr<verify::lanes::Prepared> prepared;
        try {
            prepared = lane->prepare(entry.path(), work / "work");
        } catch (const Error&) {
            continue;
        }
        const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
        unsigned per_file = 0;
        for (const auto& [address, name] : image.written.functions) {
            if (per_file++ == 20) break;
            ++functions;
            // the function judged against copies of the program with one of its first three bytes turned over, once on each core: the verdicts must agree
            const Bytes original_bytes = fs::read_file(entry.path());
            const formats::psx::Exe exe = formats::psx::parse(original_bytes);
            const std::size_t at = formats::psx::kHeaderSize + ((address & formats::psx::kRamMask) - (exe.header.t_addr & formats::psx::kRamMask));
            for (std::size_t k = 0; k < 3 && at + k < original_bytes.size(); ++k) {
                Bytes changed = original_bytes;
                changed[at + k] ^= 0xFF;
                const stdfs::path edited = work / "edited.exe";
                fs::write_file(edited, changed);
                store::Judgement j[2];
                for (int core = 0; core < 2; ++core) {
                    MipsForced forced(core == 1);
                    j[core] = lane->judge_function(false, entry.path(), edited, address);
                    ++verdicts[core][j[core].verdict == store::Verdict::Equivalent ? 0 : 1];
                }
                ++judged;
                if (j[0].verdict != j[1].verdict) {
                    ++disagree;
                    if (disagree <= 6) std::printf("  %s: function %x byte +%zu: own core %s (%s), emulator %s (%s)\n", entry.path().filename().string().c_str(), address, k,
                                                   std::string(store::word(j[0].verdict)).c_str(), j[0].reason.c_str(), std::string(store::word(j[1].verdict)).c_str(), j[1].reason.c_str());
                }
            }
            for (const verify::lanes::State& state : spec.states) {
                verify::lanes::Effects e[2];
                for (int core = 0; core < 2; ++core) {
                    MipsForced forced(core == 1);
                    const double start = cb::thread_cpu_seconds();
                    e[core] = spec.effects(entry.path(), address, state);
                    seconds[core] += (cb::thread_cpu_seconds() - start);
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
                }
                ++reasons[what.substr(0, 120)];
                if (differ <= 12) std::printf("  %s: effects differ at %x:%s\n", entry.path().filename().string().c_str(), address, what.c_str());
            }
        }
    }
    (void)unexplained;
    std::printf("pcode_cpu ps1 homebrew: %u programs, %u functions, %u runs, effects differing %u; time own core %.2fs emulator %.2fs\n", files, functions, runs, differ, seconds[0], seconds[1]);
    for (const auto& [k, c] : reasons) std::printf("    %u x%s\n", c, k.c_str());
    std::printf("  edited programs judged %u: own core equivalent %u divergent %u, emulator equivalent %u divergent %u, verdicts disagreeing %u\n", judged, verdicts[0][0], verdicts[0][1], verdicts[1][0], verdicts[1][1], disagree);
    CB_CHECK_EQ(differ, 0u);
    CB_CHECK_EQ(disagree, 0u);
});

// The adapter's own GTE (engine/pcode_ops_mips.cpp) against the lane's core on random registers: every data and control register read after a write, and each command the core has
// code for, from random register files and command words (shift, saturation and matrix selects). 0 differences.
CB_TEST("emulate.pcode_cpu.the_adapters_gte_does_what_the_lane_core_does", [] {
    std::mt19937 rng(20261123);
    mp::Bus bus;
    unsigned compared = 0, different = 0;
    for (unsigned n = 0; n < 4000; ++n) {
        mp::Cpu cpu(bus);
        emulate::pcode_ops_mips::Gte gte;
        for (unsigned i = 0; i < 32; ++i) {
            const std::uint32_t v = static_cast<std::uint32_t>(rng()) >> (rng() % 4 == 0 ? 16 : 0);
            cpu.gte_data[i] = v;
            gte.data[i] = v;
            const std::uint32_t c = static_cast<std::uint32_t>(rng()) >> (rng() % 3 == 0 ? 16 : 0);
            cpu.gte_control[i] = c;
            gte.control[i] = c;
        }
        for (unsigned i = 0; i < 32; ++i) {   // (a write, then every register read)
            const std::uint32_t v = static_cast<std::uint32_t>(rng());
            mp::gte_set_register(cpu, i, v);
            emulate::pcode_ops_mips::gte_set_register(gte, i, v);
        }
        for (unsigned i = 0; i < 32; ++i) {
            ++compared;
            if (mp::gte_register(cpu, i) != emulate::pcode_ops_mips::gte_register(gte, i) || mp::gte_control_register(cpu, i) != emulate::pcode_ops_mips::gte_control_register(gte, i)) ++different;
        }
        static const char* const kCommands[] = {"nclip", "avsz3", "avsz4", "sqr", "op", "gpf", "gpl", "mvmva", "rtps", "ncds"};
        const char* name = kCommands[rng() % std::size(kCommands)];
        const std::uint32_t command = static_cast<std::uint32_t>(rng()) & 0x1FFFFFF;
        const bool a = mp::gte_run_command(cpu, command, name), b = emulate::pcode_ops_mips::gte_run_command(gte, command, name);
        ++compared;
        if (a != b || cpu.gte_data != gte.data || cpu.gte_control != gte.control) ++different;
    }
    std::printf("pcode_cpu gte: %u comparisons, %u differ\n", compared, different);
    CB_CHECK_EQ(different, 0u);
});
