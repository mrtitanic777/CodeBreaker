// The lanes' 6502 and Z80 on the one emulator (engine/pcode_cpu): the 6502's random programs as its retired core ran them (a digest), and the Z80
// lanes' verdicts on real homebrew the same with either core.
#include "engine/pcode_cpu.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <map>
#include <random>
#include <vector>

#include "source/processors.hpp"
#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/m6502.hpp"
#include "emulate/lynx.hpp"
#include "engine/lynx_boot.hpp"
#include "engine/lane_kit.hpp"
#include "engine/machine_atari.hpp"
#include "engine/machine_atari2600.hpp"
#include "engine/machine_atari5200.hpp"
#include "engine/machine_atari7800.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_lynx.hpp"
#include "formats/atari2600.hpp"
#include "formats/atari5200.hpp"
#include "formats/lynx.hpp"
#include "source/isa.hpp"
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

}  // namespace

// A random program of documented opcodes at $8000, with random bytes after it, a device range at $4000-$40FF and RAM elsewhere. What 1,500 such runs gave
// on the old core (emulate/m6502's interpreter) and on the emulator, the same when the core was retired, is kept as a digest (CRC-32 of every run's exit,
// registers, steps, device accesses and memory).
CB_TEST("engine.pcode_cpu.runs_random_programs_as_the_own_core_did", [] {
    std::vector<std::uint8_t> documented;
    const auto& table = decompile::m6502::opcode_table(decompile::m6502::Variant::Nmos);
    for (unsigned op = 0; op < 256; ++op)
        if (table[op]) documented.push_back(static_cast<std::uint8_t>(op));
    std::mt19937 rng(20261120);
    std::uint32_t digest = 0;
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
        m::Bus bus(image);
        bus.map(0x4000, 0x40FF, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 7 + 3); }, [](std::uint16_t, std::uint8_t) {});
        const m::RunResult r = engine::pcode_cpu::run(bus, 0x8000, options);
        std::string text = describe(r);
        for (const m::DeviceAccess& x : r.io) text += std::format("|{}{:X}={:X}", static_cast<int>(x.kind), x.address, x.value);
        digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
        digest = crypto::crc32(ByteView(bus.memory().data(), bus.memory().size()), digest);
    }
    std::printf("pcode_cpu random programs: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x15018420u);
});

// ---- the other 6502 and the Z80 lanes ---------------------------------------------------------------------------------------------------

namespace {

stdfs::path samples_root() {
    stdfs::path root = testing::beside_checkout("homebrew-samples");
    if (const auto given = fs::get_env("CB_HOMEBREW_SAMPLES"); given && !given->empty()) root = fs::from_text(*given);
    return root;
}

struct Tally {
    unsigned functions = 0, judged = 0, disagree = 0, reasons = 0;
    unsigned verdicts[2][2] = {};   // [core: own, emulator][equivalent, divergent]
    void add(const Tally& o) {
        reasons += o.reasons;
        functions += o.functions;
        judged += o.judged;
        disagree += o.disagree;
        for (int c = 0; c < 2; ++c)
            for (int v = 0; v < 2; ++v) verdicts[c][v] += o.verdicts[c][v];
    }
};

// Where in the file a function's first byte is; none: the function is not edited.
using Offset = std::function<std::optional<std::size_t>(std::uint32_t entry)>;

// Every function of `module` (up to `cap`) judged against three copies of the program, each with one of the function's first three bytes turned over,
// once on each core; the verdicts and their reasons must agree. An empty `offset`: a flat Z80 machine, whose two programs are laid out in files of an
// 8-byte header and the 64 KB of memory, edited at the function's address.
Tally judge_on_both_cores(const verify::lanes::ImageLane& lane, const stdfs::path& module, const Offset& offset, std::size_t cap, const stdfs::path& work_dir, const char* label) {
    Tally tally;
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(module, work_dir / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    stdfs::path original = module;
    if (!offset) original = lane.subjects_of(module, image.written).first;
    const Bytes bytes = fs::read_file(original);
    const stdfs::path edited = work_dir / "edited.bin";
    for (const auto& [entry, name] : image.written.functions) {
        if (tally.functions == cap) break;
        std::size_t at = 0;
        if (offset) {
            const std::optional<std::size_t> o = offset(entry);
            if (!o) continue;
            at = *o;
        } else {
            at = 8 + entry;
        }
        ++tally.functions;
        for (std::size_t k = 0; k < 3; ++k) {
            if (at + k >= bytes.size()) continue;
            Bytes changed = bytes;
            changed[at + k] ^= 0xFF;
            fs::write_file(edited, changed);
            verify::Judgement j[2];
            for (int core = 0; core < 2; ++core) {
                Forced forced(core == 1);
                j[core] = lane.judge_function(false, original, edited, entry);
                ++tally.verdicts[core][j[core].verdict == verify::Verdict::Equivalent ? 0 : 1];
            }
            ++tally.judged;
            if (j[0].verdict == j[1].verdict && j[0].reason != j[1].reason) ++tally.reasons;   // (both divergent, the first place that differs is another one)
            if (j[0].verdict != j[1].verdict) {
                if (++tally.disagree <= 6)
                    std::printf("  %s: function %x byte +%zu: own core %s (%s %s), emulator %s (%s %s)\n", label, entry, k, std::string(verify::word(j[0].verdict)).c_str(), j[0].reason.c_str(),
                                j[0].detail.c_str(), std::string(verify::word(j[1].verdict)).c_str(), j[1].reason.c_str(), j[1].detail.c_str());
            }
        }
    }
    return tally;
}

void report(const char* lane, const Tally& t) {
    std::printf("pcode_cpu %s homebrew: %u functions, %u edited programs judged; own core equivalent %u divergent %u; emulator equivalent %u divergent %u; verdicts disagreeing %u; same verdict, another first difference %u\n", lane,
                t.functions, t.judged, t.verdicts[0][0], t.verdicts[0][1], t.verdicts[1][0], t.verdicts[1][1], t.disagree, t.reasons);
}

// A machine lane of the lane kit, with its own state folder.
struct Kit {
    fs::ScopedTempDir dir{"cb_pcode_cpu_kit"};
    std::optional<fs::ScopedEnv> scoped;
    std::unique_ptr<engine::LaneKit> kit;
    Kit() {
        try {
            const config::Config config = config::Config::load(config::current().main_checkout());
            scoped.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
            kit = std::make_unique<engine::LaneKit>(config::Config::load(config.checkout_root()));
        } catch (const Error&) {
        }
    }
    const verify::lanes::ImageLane* lane(const char* name) const {
        if (!kit) return nullptr;
        const std::shared_ptr<const verify::lanes::Kind> kind = kit->machine_lane(name);
        return dynamic_cast<const verify::lanes::ImageLane*>(kind.get());
    }
};

// Functions judged per program: 80, or CB_PCODE_FUNCTIONS (the whole corpus with a large number).
const std::size_t kCap = [] {
    const auto given = fs::get_env("CB_PCODE_FUNCTIONS");
    return given && !given->empty() ? static_cast<std::size_t>(std::strtoull(given->c_str(), nullptr, 10)) : std::size_t{80};
}();

void judge_files(const char* label, const char* lane_name, const std::vector<std::string>& files, bool flat, std::uint32_t origin = 0) {
    const Kit kit;
    const verify::lanes::ImageLane* lane = kit.lane(lane_name);
    if (lane == nullptr) return;
    Tally total;
    unsigned n = 0;
    for (const std::string& relative : files) {
        const stdfs::path file = samples_root() / relative;
        if (!stdfs::is_regular_file(file)) continue;
        const stdfs::path work = kit.dir.path() / std::to_string(n++);
        stdfs::create_directories(work);
        const std::size_t size = fs::read_file(file).size();
        Offset offset;
        if (!flat) offset = [&](std::uint32_t entry) -> std::optional<std::size_t> { return entry >= origin && entry - origin < size ? std::optional<std::size_t>(entry - origin) : std::nullopt; };
        total.add(judge_on_both_cores(*lane, file, offset, kCap, work, relative.c_str()));
    }
    if (n == 0) return;
    report(label, total);
    CB_CHECK(total.judged != 0);
    CB_CHECK_EQ(total.disagree, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.the_master_system_and_sg1000_lanes_judge_homebrew_the_same_on_both_cores", [] {
    judge_files("master system / sg-1000", "sms", {"sms-peterlemon/HelloWorld.sms", "sms-peterlemon/SMSAudio.sms", "sms-peterlemon/Sprite.sms", "sg1000-240p/suitesg.sg"}, false);
});
// One instruction of the Z80 at a time, from random registers and memory, on each core; what differs is listed by opcode. (F bits 3 and 5 are not in the description, R
// is not kept, and the repeating block instructions go on for many steps: those are left out.)
// The default run is systematic: every opcode of every page (plain, CB, ED, DD, FD, DD CB, FD CB: 1,792 instructions) from kStates register/flag states (the
// flags all clear, all set, and random ones; accumulators at their edge values), each with fresh random operand bytes and memory. CB_PCODE_EXHAUSTIVE=<n> instead
// runs n (default 120000) random instructions, as the test did before.
CB_TEST("engine.pcode_cpu.z80_instructions_one_by_one_match_the_own_core", [] {
    std::mt19937 rng(20261120);
    std::map<std::string, unsigned> by_opcode, tried;
    std::string first_report;
    std::map<std::string, unsigned> missing;
    unsigned different = 0, runs = 0, lacking = 0;
    constexpr unsigned kStates = 6;
    unsigned random_runs = 0;
    if (const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE"); given && !given->empty()) random_runs = *given == "1" ? 120000u : static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10));
    const unsigned total = random_runs != 0 ? random_runs : 7u * 256u * kStates;
    Bytes base(0x10000);
    for (auto& b : base) b = static_cast<std::uint8_t>(rng());
    for (unsigned n = 0; n < total; ++n) {
        Bytes image = base;
        std::uint8_t code[4];
        for (auto& b : code) b = static_cast<std::uint8_t>(rng());
        unsigned state = 0;
        if (random_runs != 0) {
            for (auto& b : image) b = static_cast<std::uint8_t>(rng());
            if (rng() % 3 == 0) code[0] = "\xCB\xDD\xED\xFD"[rng() % 4];
        } else {
            const unsigned page = n / (256 * kStates), op = (n / kStates) % 256;
            state = n % kStates;
            for (unsigned i = 0; i < 64; ++i) image[0x9000 + rng() % 0x1000] = static_cast<std::uint8_t>(rng());   // (some fresh memory around the stack and the pointers)
            switch (page) {
                case 0: code[0] = static_cast<std::uint8_t>(op); break;
                case 1: code[0] = 0xCB; code[1] = static_cast<std::uint8_t>(op); break;
                case 2: code[0] = 0xED; code[1] = static_cast<std::uint8_t>(op); break;
                case 3: code[0] = 0xDD; code[1] = static_cast<std::uint8_t>(op); break;
                case 4: code[0] = 0xFD; code[1] = static_cast<std::uint8_t>(op); break;
                case 5: code[0] = 0xDD; code[1] = 0xCB; code[3] = static_cast<std::uint8_t>(op); break;
                default: code[0] = 0xFD; code[1] = 0xCB; code[3] = static_cast<std::uint8_t>(op); break;
            }
        }
        if (code[0] == 0xED && (code[1] & 0xF4) == 0xB0 && (code[1] & 3) != 0xFF && code[1] >= 0xB0) continue;   // LDIR CPIR INIR OTIR LDDR ...
        std::copy(code, code + 4, image.begin() + 0x8000);
        const auto rd = [&] { return static_cast<std::uint8_t>(rng()); };
        emulate::z80::RunOptions options;
        options.max_steps = 1;
        options.sp = static_cast<std::uint16_t>(0x9000 + (rng() & 0xFFF));
        options.regs = {rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd(), static_cast<std::uint16_t>(rng()), static_cast<std::uint16_t>(rng())};
        if (random_runs == 0) {   // the state: flags (all clear, all set, the carry alone, random) and the accumulator (0, 0xFF, 0x80, 0x7F, random)
            static constexpr std::uint8_t kFlagsOf[4] = {0x00, 0xFF, 0x01, 0xD7};   // (states 4 and 5 keep their random flags)
            static constexpr std::uint8_t kAccOf[kStates] = {0x00, 0xFF, 0x80, 0x7F, 0x01, 0xFE};
            if (state < 4) options.regs.f = kFlagsOf[state];
            options.regs.a = kAccOf[state];
        }
        const auto go = [&](bool pcode, Bytes& memory) {
            emulate::z80::Bus bus(ByteView(memory), [](std::uint16_t port) -> std::uint8_t { return static_cast<std::uint8_t>(port * 5 + 1); });
            const emulate::z80::RunResult r = pcode ? engine::pcode_cpu::run(bus, 0x8000, options) : emulate::z80::run(bus, 0x8000, options);
            memory.assign(bus.memory().begin(), bus.memory().end());
            return r;
        };
        Bytes ma = image, mb = image;
        const emulate::z80::RunResult own = go(false, ma);
        const emulate::z80::RunResult emu = go(true, mb);
        std::string key = std::format("{:02X}", code[0]);
        if (code[0] == 0xCB || code[0] == 0xED) key += std::format("{:02X}", code[1]);
        if (code[0] == 0xDD || code[0] == 0xFD) key += std::format("{:02X}", code[1] == 0xCB ? code[3] : code[1]) + (code[1] == 0xCB ? "(cb)" : "");
        ++tried[key];
        ++runs;
        const emulate::z80::Registers &a = own.regs, &b = emu.regs;
        if (own.end != emu.end) {
            ++lacking;
            ++missing[key];
            continue;
        }
        std::string fields;
        if ((a.f & 0xD7) != (b.f & 0xD7)) fields += "F ";
        if (a.a != b.a) fields += "A ";
        if (a.bc() != b.bc()) fields += "BC ";
        if (a.de() != b.de()) fields += "DE ";
        if (a.hl() != b.hl()) fields += "HL ";
        if (a.ix != b.ix) fields += "IX ";
        if (a.iy != b.iy) fields += "IY ";
        if (a.sp != b.sp) fields += "SP ";
        if (a.pc != b.pc) fields += "PC ";
        if (a.a2 != b.a2 || a.f2 != b.f2 || a.b2 != b.b2 || a.c2 != b.c2 || a.h2 != b.h2 || a.l2 != b.l2) fields += "alt ";
        if (ma != mb) fields += "memory ";
        bool io_same = own.io.size() == emu.io.size();
        for (std::size_t i = 0; io_same && i < own.io.size(); ++i) io_same = own.io[i].kind == emu.io[i].kind && own.io[i].where == emu.io[i].where && own.io[i].value == emu.io[i].value;
        if (!io_same) fields += "io ";
        if (!fields.empty()) {
            ++different;
            ++by_opcode[key + ": " + fields];
        }
    }
    std::printf("pcode_cpu z80 one instruction: %u runs, %u differ, %u the description has no instruction for (%zu opcodes)\n", runs, different, lacking, missing.size());
    for (const auto& [key, count] : by_opcode) std::printf("    %s %u\n", key.c_str(), count);
    std::string lacks;
    for (const auto& [key, count] : missing) lacks += key + " ";
    std::printf("  no instruction for: %s\n", lacks.c_str());
    CB_CHECK_EQ(different, 0u);
});

CB_TEST("engine.pcode_cpu.the_msx_lane_judges_homebrew_the_same_on_both_cores", [] { judge_files("msx", "msx", {"msx-nohzdyve/nohzdyve.rom", "msx-wrally/rally-demo.rom"}, true); });
CB_TEST("engine.pcode_cpu.the_colecovision_lane_judges_homebrew_the_same_on_both_cores", [] { judge_files("colecovision", "coleco", {"coleco-240p/suitecv.rom"}, true); });
CB_TEST("engine.pcode_cpu.the_zx_spectrum_lane_judges_homebrew_the_same_on_both_cores", [] { judge_files("zx spectrum", "zx", {"zx-icarus/Icarus.tap"}, true); });
CB_TEST("engine.pcode_cpu.the_amstrad_cpc_lane_judges_homebrew_the_same_on_both_cores", [] { judge_files("amstrad cpc", "cpc", {"cpc/romdump/ROMDump.dsk", "cpc/8kcopier/8KCopier.dsk"}, true); });
