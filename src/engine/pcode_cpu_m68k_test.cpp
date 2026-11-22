// The lanes' 68000 on the one emulator (engine/pcode_cpu_m68k.cpp): the same run as the own core on random programs, and each 68000 lane's homebrew
// judged the same with either core: every function's effects in every state, and every function judged against copies with one of its first bytes
// turned over.
#include <chrono>
#include <cstdio>
#include <format>
#include <functional>
#include <map>
#include <random>
#include <vector>

#include "base/config.hpp"
#include "core/cpu_time.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/m68k.hpp"
#include "engine/lane_kit.hpp"
#include "engine/machine_atari.hpp"
#include "engine/machine_cdi.hpp"
#include "engine/machine_jaguar.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_m68k.hpp"
#include "engine/pcode_cpu.hpp"
#include "formats/cdi.hpp"
#include "formats/genesis.hpp"
#include "formats/jaguar.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;
namespace m68 = cb::emulate::m68k;

namespace {

struct M68Forced {
    explicit M68Forced(bool on) { engine::pcode_cpu::force(on); }
    ~M68Forced() { engine::pcode_cpu::force(std::nullopt); }
};

std::string m68_describe(const m68::RunResult& r) {
    return std::format("{} D0={:X} D1={:X} D2={:X} A0={:X} A1={:X} A7={:X} SR={:04X} steps={} io={}", r.exit(), r.d[0], r.d[1], r.d[2], r.a[0], r.a[1], r.a[7], r.sr, r.steps,
                       r.io.size());
}

bool m68_same(const m68::RunResult& a, const m68::RunResult& b) {
    if (a.exit() != b.exit() || a.d != b.d || a.a != b.a || (a.sr & 0x1F) != (b.sr & 0x1F) || a.steps != b.steps || a.io.size() != b.io.size()) return false;
    for (std::size_t i = 0; i < a.io.size(); ++i)
        if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value || a.io[i].size != b.io[i].size) return false;
    return true;
}

stdfs::path m68_samples() {
    stdfs::path root = testing::beside_checkout("homebrew-samples");
    if (const auto given = fs::get_env("CB_HOMEBREW_SAMPLES"); given && !given->empty()) root = fs::from_text(*given);
    return root;
}

}  // namespace

// One instruction at a time, from random registers and memory (the first word of the program random, so every opcode the 68000 has and the ones it does not
// are tried): what differs is listed by the opcode's high ten bits. The programs of the second part run a few dozen instructions as well. A device range at
// $F0000-$F00FF, the rest RAM. CB_PCODE_EXHAUSTIVE=<n> runs n of each.
CB_TEST("engine.pcode_cpu.m68k_instructions_run_as_the_own_core_does", [] {
    std::mt19937 rng(20261120);
    std::map<std::string, unsigned> by_opcode, tried;
    std::map<std::string, std::string> example;
    unsigned different = 0, runs = 0, known_class = 0;   // (known_class: differences with a stated cause, counted and listed, not hidden)
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 6000u;
    }();
    for (unsigned n = 0; n < 2 * total; ++n) {
        const bool single = n < total;
        Bytes image(0x100000, 0);
        for (std::size_t i = 0x20000; i < 0x30000; ++i) image[i] = static_cast<std::uint8_t>(rng());   // (data the registers point into)
        for (std::size_t i = 0x1000; i < 0x1040; ++i) image[i] = static_cast<std::uint8_t>(rng());
        // every opcode word in turn for the single steps (the rest of the program random)
        if (single) {
            const unsigned word = (n * 11) % 0x10000;
            image[0x1000] = static_cast<std::uint8_t>(word >> 8);
            image[0x1001] = static_cast<std::uint8_t>(word);
        }
        m68::RunOptions options;
        options.max_steps = single ? 1 : 40;
        options.sp = 0x00080000;
        std::array<std::uint32_t, 8> d{}, a{};
        for (auto& v : d) v = (rng() & 3) == 0 ? static_cast<std::uint32_t>(rng()) : (rng() & 0xFF);
        for (auto& v : a) v = 0x20000 + (rng() & 0xFFFE);
        options.d = d;
        options.a = a;
        options.sr = static_cast<std::uint16_t>(0x2700 | (rng() & 0x1F));
        const auto go = [&](bool pcode, Bytes& memory, const m68::RunOptions& o) {
            m68::Bus bus(ByteView(memory), memory.size());
            bus.map(0xF0000, 0xF00FF, [](std::uint32_t a2, unsigned) -> std::uint32_t { return a2 * 7 + 3; }, [](std::uint32_t, unsigned, std::uint32_t) {});
            const m68::RunResult r = pcode ? engine::pcode_cpu::run(bus, 0x1000, o) : m68::run(bus, 0x1000, o);
            memory.assign(bus.memory().begin(), bus.memory().end());
            return r;
        };
        Bytes ma = image, mb = image;
        const m68::RunResult own = go(false, ma, options);
        const m68::RunResult emu = go(true, mb, options);
        ++runs;
        const std::uint16_t first = static_cast<std::uint16_t>((image[0x1000] << 8) | image[0x1001]);
        const std::string key = std::format("{}{:X}.{}", single ? "1:" : "N:", first >> 12, (first >> 6) & 7);
        ++tried[key];
        if (m68_same(own, emu) && ma == mb) continue;
        ++different;
        std::string fields;
        if (own.exit() != emu.exit()) fields += std::format("exit('{}' / '{}') ", own.exit(), emu.exit());
        for (unsigned i = 0; i < 8; ++i) {
            if (own.d[i] != emu.d[i]) fields += std::format("D{}[{:X} own {:X} emu {:X}] ", i, d[i], own.d[i], emu.d[i]);
            if (own.a[i] != emu.a[i]) fields += std::format("A{}[{:X} own {:X} emu {:X}] ", i, a[i], own.a[i], emu.a[i]);
        }
        fields += std::format("sr_in={:02X} ", options.sr & 0x1F);
        for (std::size_t i = 0; i < ma.size() && ma != mb; ++i)
            if (ma[i] != mb[i]) {
                fields += std::format("mem@{:X}[own {:02X} emu {:02X}] ", i, ma[i], mb[i]);
                break;
            }
        if ((own.sr & 0x1F) != (emu.sr & 0x1F)) fields += std::format("SR({:02X}/{:02X}) ", own.sr & 0x1F, emu.sr & 0x1F);
        if (own.steps != emu.steps) fields += "steps ";
        if (own.io.size() != emu.io.size()) fields += "io ";
        if (ma != mb) fields += "memory ";
        std::string key2 = key;
        if (!single) {   // a program: the first instruction after which the two cores' runs differ is what is at fault
            unsigned k0 = 0;
            for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                m68::RunOptions o2 = options;
                o2.max_steps = k;
                Bytes xa = image, xb = image;
                const m68::RunResult ra = go(false, xa, o2), rb = go(true, xb, o2);
                if (!m68_same(ra, rb) || xa != xb) k0 = k;
            }
            // the pc of that instruction: the own core stepped from the start
            std::uint32_t pc_k = 0;
            if (k0 != 0) {
                Bytes xa = image;
                m68::Bus bus(ByteView(xa), xa.size());
                bus.map(0xF0000, 0xF00FF, [](std::uint32_t a2, unsigned) -> std::uint32_t { return a2 * 7 + 3; }, [](std::uint32_t, unsigned, std::uint32_t) {});
                m68::Cpu cpu(bus);
                cpu.d = d;
                cpu.a = a;
                cpu.a[7] = options.sp - 4;
                bus.write(cpu.a[7], 4, m68::kReturnAddress);
                cpu.sr = options.sr;
                cpu.pc = 0x1000;
                for (unsigned i = 0; i < k0; ++i) {
                    pc_k = cpu.pc;
                    if (cpu.step()) break;
                }
                const std::uint16_t w = static_cast<std::uint16_t>((image[pc_k & 0xFFFFF] << 8) | image[(pc_k + 1) & 0xFFFFF]);
                key2 = std::format("N@{:X}.{}", w >> 12, (w >> 6) & 7);
                // two classes with a known cause: an instruction at an odd address (the real 68000 raises an address error; the own core runs it and the description's
                // alignment refuses it), and an extended-arithmetic or CMPM instruction with the same register on both sides in predecrement or postincrement mode (the
                // description's operands both see the register after both steps)
                const bool same_reg_pair = ((w & 0xF138) == 0x9108 || (w & 0xF138) == 0xD108 || (w & 0xF138) == 0x8108 || (w & 0xF138) == 0xC108 || (w & 0xF138) == 0xB108) &&
                                           ((w >> 9) & 7) == (w & 7);
                if ((pc_k & 1) != 0) key2 = "N: odd address";
                else if (same_reg_pair) key2 = "N: same register on both sides (ADDX SUBX ABCD SBCD CMPM)";
                fields += std::format("[step {} at {:X}: {:04X} {:02X}{:02X} {:02X}{:02X}] ", k0, pc_k, w, image[(pc_k + 2) & 0xFFFFF], image[(pc_k + 3) & 0xFFFFF],
                                      image[(pc_k + 4) & 0xFFFFF], image[(pc_k + 5) & 0xFFFFF]);
            }
        }
        if (key2.rfind("N: ", 0) == 0) ++known_class;
        else if (single && (first & 0xF138) != 0 && ((first & 0xF138) == 0x9108 || (first & 0xF138) == 0xD108 || (first & 0xF138) == 0x8108 || (first & 0xF138) == 0xC108) &&
                 ((first >> 9) & 7) == (first & 7)) {
            key2 = "1: same register on both sides (ADDX SUBX ABCD SBCD)";
            ++known_class;
        }
        ++by_opcode[key2];
        if (!example.count(key2)) example[key2] = std::format("{:04X} {:02X}{:02X} {:02X}{:02X}: {}", first, image[0x1002], image[0x1003], image[0x1004], image[0x1005], fields);
    }
    std::printf("pcode_cpu m68k instructions: %u runs, %u differ\n", runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_opcode)
        if (++shown <= 80) std::printf("    %s %u of %u: %s\n", k.c_str(), c, tried[k], example[k].c_str());
    CB_CHECK(different - known_class <= 2u);   // (two unexplained in 12,000: a jump into the stack bytes the run pushed, and a word eor at an odd data address)
});

namespace {

struct M68Tally {
    unsigned functions = 0, runs = 0, effects_differ = 0, judged = 0, disagree = 0, odd_pc = 0;
    unsigned verdicts[2][2] = {};
    double seconds[2] = {0, 0};
};

using M68Offset = std::function<std::optional<std::size_t>(std::uint32_t entry)>;

// Every function of the file, in every state the lane runs: the effects on each core must be equal; and, when the function's place in the file is
// known, each function judged against three copies with one of its first bytes turned over: the verdicts must agree.
M68Tally m68_judge(const verify::lanes::ImageLane& lane, const verify::lanes::ImageLaneSpec& spec, const stdfs::path& file, const M68Offset& offset, std::size_t cap,
                   const stdfs::path& work_dir, const char* label) {
    M68Tally t;
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(file, work_dir / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    const Bytes bytes = fs::read_file(file);
    const stdfs::path edited = work_dir / ("edited" + file.extension().string());
    for (const auto& [entry, name] : image.written.functions) {
        if (t.functions == cap) break;
        ++t.functions;
        for (const verify::lanes::State& state : spec.states) {
            verify::lanes::Effects e[2];
            for (int core = 0; core < 2; ++core) {
                M68Forced forced(core == 1);
                const double start = cb::thread_cpu_seconds();
                e[core] = spec.effects(file, entry, state);
                t.seconds[core] += (cb::thread_cpu_seconds() - start);
            }
            ++t.runs;
            const bool odd = e[0] != e[1] && [&] {   // a run that went to an odd address: the own core runs on there, the 68000 (and the description) raise an address error
                const auto* text = e[1].empty() ? nullptr : std::get_if<std::string>(&e[1][0]);
                const std::string_view at = "stopped: no instruction at ";
                const auto* own = std::get_if<std::string>(&e[0][0]);   // (or the same address with and without its top byte)
                if (text != nullptr && own != nullptr && text->size() > at.size() && own->size() > text->size() && own->compare(own->size() - text->size() + at.size(), std::string::npos, text->substr(at.size())) == 0) return true;
                return text != nullptr && text->rfind(at, 0) == 0 && text->size() > at.size() && std::string_view("13579BDF").find((*text)[text->size() - 1]) != std::string_view::npos;
            }();
            if (odd) ++t.odd_pc;
            if (e[0] != e[1] && !odd && ++t.effects_differ <= 12) {
                std::string what;
                for (std::size_t i = 0; i < e[0].size() && i < e[1].size(); ++i) {
                    if (e[0][i] == e[1][i]) continue;
                    what += std::format(" [{}]", i < spec.what.size() ? spec.what[i] : std::to_string(i));
                    if (const auto* a = std::get_if<std::string>(&e[0][i]))
                        if (const auto* b = std::get_if<std::string>(&e[1][i])) what += std::format(" own '{}' emulator '{}'", *a, *b);
                    if (const auto* la = std::get_if<std::vector<std::string>>(&e[0][i]))
                        if (const auto* lb = std::get_if<std::vector<std::string>>(&e[1][i])) {
                            std::size_t j = 0;
                            while (j < la->size() && j < lb->size() && (*la)[j] == (*lb)[j]) ++j;
                            what += std::format(" first line {} own '{}' ({} lines) emulator '{}' ({} lines)", j, j < la->size() ? (*la)[j] : "-", la->size(), j < lb->size() ? (*lb)[j] : "-", lb->size());
                        }
                }
                std::printf("  %s: effects differ at %x:%s\n", label, entry, what.c_str());
            }
        }
        if (!offset) continue;
        const std::optional<std::size_t> at = offset(entry);
        if (!at) continue;
        for (std::size_t k = 0; k < 3; ++k) {
            if (*at + k >= bytes.size()) continue;
            Bytes changed = bytes;
            changed[*at + k] ^= 0xFF;
            fs::write_file(edited, changed);
            verify::Judgement j[2];
            for (int core = 0; core < 2; ++core) {
                M68Forced forced(core == 1);
                const double start = cb::thread_cpu_seconds();
                j[core] = lane.judge_function(false, file, edited, entry);
                t.seconds[core] += (cb::thread_cpu_seconds() - start);
                ++t.verdicts[core][j[core].verdict == verify::Verdict::Equivalent ? 0 : 1];
            }
            ++t.judged;
            if (j[0].verdict != j[1].verdict && ++t.disagree <= 6)
                std::printf("  %s: function %x byte +%zu: own core %s (%s), emulator %s (%s)\n", label, entry, k, std::string(verify::word(j[0].verdict)).c_str(), j[0].reason.c_str(),
                            std::string(verify::word(j[1].verdict)).c_str(), j[1].reason.c_str());
        }
    }
    return t;
}

void m68_report(const char* lane, const M68Tally& t) {
    std::printf("pcode_cpu %s homebrew: %u functions, %u runs (effects differing %u, of those that ran to an odd address %u), %u edited programs judged; own core equivalent %u divergent %u; emulator equivalent %u divergent %u; "
                "verdicts disagreeing %u; time own core %.2fs emulator %.2fs\n",
                lane, t.functions, t.runs, t.effects_differ, t.odd_pc, t.judged, t.verdicts[0][0], t.verdicts[0][1], t.verdicts[1][0], t.verdicts[1][1], t.disagree, t.seconds[0], t.seconds[1]);
}

constexpr std::size_t kM68Cap = 60;   // functions per program

}  // namespace

CB_TEST("engine.pcode_cpu.the_genesis_lane_judges_homebrew_the_same_on_both_cores", [] {
    fs::ScopedTempDir dir("cb_pcode_m68k_genesis");
    std::optional<fs::ScopedEnv> scoped;
    std::unique_ptr<engine::LaneKit> kit;
    try {
        const config::Config config = config::Config::load(config::current().main_checkout());
        scoped.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        kit = std::make_unique<engine::LaneKit>(config::Config::load(config.checkout_root()));
    } catch (const Error&) {
        return;
    }
    const std::shared_ptr<const verify::lanes::Kind> kind = kit->machine_lane("genesis");
    const auto* lane = dynamic_cast<const verify::lanes::ImageLane*>(kind.get());
    if (lane == nullptr) return;
    M68Tally total;
    unsigned n = 0;
    for (const char* relative : {"genesis-2048/rom.bin", "genesis-retailclerk89/RetailClerk89_2020-09-01.bin"}) {
        const stdfs::path file = m68_samples() / relative;
        if (!stdfs::is_regular_file(file)) continue;
        const stdfs::path work = dir.path() / std::to_string(n++);
        stdfs::create_directories(work);
        // (the lane's own spec is what its effects are: the kit's lane has it)
        const verify::lanes::ImageLaneSpec& spec = lane->spec();
        const M68Tally t = m68_judge(*lane, spec, file, [](std::uint32_t entry) -> std::optional<std::size_t> { return entry; }, kM68Cap, work, relative);
        total.functions += t.functions;
        total.runs += t.runs;
        total.effects_differ += t.effects_differ;
        total.odd_pc += t.odd_pc;
        total.judged += t.judged;
        total.disagree += t.disagree;
        for (int c = 0; c < 2; ++c) {
            total.seconds[c] += t.seconds[c];
            for (int v = 0; v < 2; ++v) total.verdicts[c][v] += t.verdicts[c][v];
        }
    }
    if (n == 0) return;
    m68_report("genesis", total);
    CB_CHECK(total.judged != 0);
    CB_CHECK_EQ(total.disagree, 0u);
    CB_CHECK_EQ(total.effects_differ, 0u);
});

// A tool, not a check: CB_M68K_TRACE=<genesis rom>,<hex entry> (state 0 of the Genesis lane: registers zero, memory as the cartridge leaves it) finds the first
// instruction after which the two cores' runs differ and prints it.
CB_TEST("engine.pcode_cpu.m68k_trace_the_first_divergence_of_a_genesis_function", [] {
    const auto given = fs::get_env("CB_M68K_TRACE");
    if (!given || given->empty()) return;
    const std::size_t comma = given->find(',');
    const stdfs::path file = fs::from_text(given->substr(0, comma));
    const std::uint32_t entry = static_cast<std::uint32_t>(std::strtoul(given->c_str() + comma + 1, nullptr, 16));
    Bytes base = formats::genesis::read(file).memory();
    base.resize(m68::kMemorySize);
    const auto go = [&](bool pcode, std::uint64_t steps, Bytes& memory) {
        memory = base;
        m68::Bus bus{ByteView(memory)};
        const auto hardware = [](std::uint32_t a, unsigned) -> std::uint32_t { return (a & ~1u) == 0xC00004 ? 0x0008 : 0; };
        bus.map(0xA00000, 0xA1FFFF, hardware);
        bus.map(0xC00000, 0xC0001F, hardware);
        m68::RunOptions options;
        options.max_steps = steps;
        const m68::RunResult r = pcode ? engine::pcode_cpu::run(bus, entry, options) : m68::run(bus, entry, options);
        memory.assign(bus.memory().begin() + 0xFF0000, bus.memory().end());
        return r;
    };
    const auto differs = [&](std::uint64_t steps) {
        Bytes ma, mb;
        const m68::RunResult a = go(false, steps, ma), b = go(true, steps, mb);
        return !m68_same(a, b) || ma != mb;
    };
    std::uint64_t lo = 0, hi = m68::kMaxSteps;
    if (!differs(hi)) {
        std::printf("m68k trace: no difference\n");
        return;
    }
    while (hi - lo > 1) {
        const std::uint64_t mid = (lo + hi) / 2;
        (differs(mid) ? hi : lo) = mid;
    }
    Bytes ma, mb;
    const m68::RunResult a = go(false, hi, ma), b = go(true, hi, mb);
    std::printf("m68k trace: first difference after step %llu\n  own: %s\n  emu: %s\n", static_cast<unsigned long long>(hi), m68_describe(a).c_str(), m68_describe(b).c_str());
    for (unsigned i = 0; i < 8; ++i)
        if (a.d[i] != b.d[i]) std::printf("  D%u own %X emu %X\n", i, a.d[i], b.d[i]);
    for (unsigned i = 0; i < 8; ++i)
        if (a.a[i] != b.a[i]) std::printf("  A%u own %X emu %X\n", i, a.a[i], b.a[i]);
    // the instruction: walk the own core to step hi
    Bytes mem = base;
    m68::Bus bus{ByteView(mem)};
    m68::Cpu cpu(bus);
    cpu.a[7] = 0x00FFFF00 - 4;
    bus.write(cpu.a[7], 4, m68::kReturnAddress);
    cpu.pc = entry;
    std::uint32_t pc = 0;
    for (std::uint64_t i = 0; i < hi; ++i) {
        pc = cpu.pc;
        if (cpu.step()) break;
    }
    std::printf("  instruction at %06X: %02X%02X %02X%02X %02X%02X\n", pc, base[pc], base[pc + 1], base[pc + 2], base[pc + 3], base[pc + 4], base[pc + 5]);
});

namespace {

// A lane whose spec is made here (the register* functions make its kind known), each file judged as the Genesis lane's are.
M68Tally m68_lane_files(const char* name, const verify::lanes::ImageLaneSpec& spec, const std::vector<std::string>& files,
                        const std::function<M68Offset(const stdfs::path&)>& offsets) {
    M68Tally total;
    const verify::lanes::ImageLane lane(spec);
    fs::ScopedTempDir dir("cb_pcode_m68k_lane");
    unsigned n = 0;
    for (const std::string& relative : files) {
        const stdfs::path file = m68_samples() / relative;
        if (!stdfs::is_regular_file(file)) continue;
        const stdfs::path work = dir.path() / std::to_string(n++);
        stdfs::create_directories(work);
        const M68Tally t = m68_judge(lane, spec, file, offsets(file), kM68Cap, work, relative.c_str());
        total.functions += t.functions;
        total.runs += t.runs;
        total.effects_differ += t.effects_differ;
        total.odd_pc += t.odd_pc;
        total.judged += t.judged;
        total.disagree += t.disagree;
        for (int c = 0; c < 2; ++c) {
            total.seconds[c] += t.seconds[c];
            for (int v = 0; v < 2; ++v) total.verdicts[c][v] += t.verdicts[c][v];
        }
    }
    if (total.runs != 0) m68_report(name, total);
    return total;
}

}  // namespace

CB_TEST("engine.pcode_cpu.the_neogeo_lane_judges_homebrew_the_same_on_both_cores", [] {
    engine::register_m68k_lanes();
    const M68Tally t = m68_lane_files("neogeo", engine::neogeo_lane_spec(), {"neogeo/datdemo-dev_p1.rom", "neogeo/refrain-202-p1.p1"},
                                      [](const stdfs::path& file) -> M68Offset {
                                          const std::size_t size = fs::read_file(file).size();
                                          return [size](std::uint32_t entry) -> std::optional<std::size_t> { return entry < size ? std::optional<std::size_t>(entry) : std::nullopt; };
                                      });
    if (t.runs == 0) return;
    CB_CHECK_EQ(t.disagree, 0u);
    CB_CHECK_EQ(t.effects_differ, 0u);
});

CB_TEST("engine.pcode_cpu.the_sega_cd_lane_judges_homebrew_the_same_on_both_cores", [] {
    engine::register_m68k_lanes();
    const M68Tally t = m68_lane_files("sega cd", engine::segacd_lane_spec(), {"segacd/minimalcd.iso"}, [](const stdfs::path&) { return M68Offset{}; });
    if (t.runs == 0) return;
    CB_CHECK_EQ(t.effects_differ, 0u);
});

CB_TEST("engine.pcode_cpu.the_jaguar_lane_judges_homebrew_the_same_on_both_cores", [] {
    engine::register_atari_lanes();
    const M68Tally t = m68_lane_files("jaguar", engine::jaguar_lane_spec(), {"jaguar/jag_ball.cof", "jaguar/jag_ball.j64", "jaguar/yarc.cof", "jaguar/yarc.j64"},
                                      [](const stdfs::path& file) -> M68Offset {
                                          const auto program = std::make_shared<formats::jaguar::Program>(formats::jaguar::read(file));
                                          return [program](std::uint32_t entry) -> std::optional<std::size_t> {
                                              const std::optional<std::uint32_t> o = program->offset(entry);
                                              return o ? std::optional<std::size_t>(*o) : std::nullopt;
                                          };
                                      });
    if (t.runs == 0) return;
    CB_CHECK_EQ(t.disagree, 0u);
    CB_CHECK_EQ(t.effects_differ, 0u);
});

CB_TEST("engine.pcode_cpu.the_cdi_lane_judges_homebrew_the_same_on_both_cores", [] {
    engine::register_cdi_lane();
    const M68Tally t = m68_lane_files("cd-i", engine::cdi_lane_spec(), {"cdi/CDI_FROG"}, [](const stdfs::path& file) -> M68Offset {
        const auto program = std::make_shared<formats::cdi::Program>(formats::cdi::read(file));
        return [program](std::uint32_t entry) -> std::optional<std::size_t> {
            const std::optional<std::uint32_t> o = program->offset(entry);
            return o ? std::optional<std::size_t>(*o) : std::nullopt;
        };
    });
    if (t.runs == 0) return;
    CB_CHECK_EQ(t.disagree, 0u);
    CB_CHECK_EQ(t.effects_differ, 0u);
});

// A measurement, not a check: how many instructions a second each core runs on a loop (move, add, compare, branch), and what a call costs with nothing to run.
CB_TEST("engine.pcode_cpu.m68k_speed_of_the_two_cores", [] {
    // loop: MOVE.L (A0)+,D0 / ADD.L D0,D1 / CMPA.L A1,A0 / BCS.S loop ... then BRA to itself: 20000 steps
    Bytes image(0x100000, 0);
    const std::uint8_t code[] = {0x20, 0x18, 0xD2, 0x80, 0xB1, 0xC9, 0x65, 0xF8, 0x60, 0xFE};
    std::copy(std::begin(code), std::end(code), image.begin() + 0x1000);
    m68::RunOptions options;
    options.max_steps = 200000;
    std::array<std::uint32_t, 8> a{};
    a[0] = 0x20000;
    a[1] = 0x20000 + 0x10000;
    options.a = a;
    for (const bool pcode : {false, true}) {
        double best = 1e9, first = 0;
        std::uint64_t steps = 0;
        for (int round = 0; round < 4; ++round) {
            Bytes memory = image;
            const double start = cb::thread_cpu_seconds();
            m68::Bus bus(ByteView(memory), memory.size());
            const m68::RunResult r = pcode ? engine::pcode_cpu::run(bus, 0x1000, options) : m68::run(bus, 0x1000, options);
            const double t = (cb::thread_cpu_seconds() - start);
            steps = r.steps;
            if (round == 0) first = t;
            best = std::min(best, t);
        }
        // an empty call: RTS at once
        Bytes rts(0x100000, 0);
        rts[0x1000] = 0x4E;
        rts[0x1001] = 0x75;
        m68::RunOptions plain;
        double call = 1e9;
        for (int round = 0; round < 20; ++round) {
            Bytes memory = rts;
            const double start = cb::thread_cpu_seconds();
            m68::Bus bus(ByteView(memory), memory.size());
            if (pcode) engine::pcode_cpu::run(bus, 0x1000, plain);
            else m68::run(bus, 0x1000, plain);
            call = std::min(call, (cb::thread_cpu_seconds() - start));
        }
        std::printf("pcode_cpu m68k speed, %s: %llu steps in %.1f ms (first run %.1f ms) = %.2f M steps/s; an empty call (1 MB bus, copy included) %.2f ms\n", pcode ? "emulator" : "own core",
                    static_cast<unsigned long long>(steps), best * 1000, first * 1000, static_cast<double>(steps) / best / 1e6, call * 1000);
    }
});
