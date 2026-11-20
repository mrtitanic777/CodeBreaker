// The lanes' 6502 on the one emulator (engine/pcode_cpu): the same run as the own core on random programs, and the Atari 2600 lane's verdicts on real
// homebrew the same with either core.
#include "engine/pcode_cpu.hpp"

#include <algorithm>
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <vector>

#include "build/m6502_asm.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/m6502.hpp"
#include "emulate/lynx.hpp"
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

constexpr std::size_t kCap = 80;   // functions judged per program

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

CB_TEST("engine.pcode_cpu.the_atari5200_lane_judges_homebrew_the_same_on_both_cores", [] {
    const stdfs::path file = samples_root() / "atari5200" / "culomin5200.a52";
    if (!stdfs::is_regular_file(file)) return;
    engine::register_atari_lanes();
    fs::ScopedTempDir dir("cb_pcode_cpu_5200");
    const verify::lanes::ImageLane lane(engine::atari5200_lane_spec());
    const formats::atari5200::Rom rom = formats::atari5200::read(file);
    const Tally t = judge_on_both_cores(lane, file, [&](std::uint32_t entry) -> std::optional<std::size_t> { return rom.offset(entry); }, kCap, dir.path(), "culomin5200");
    report("atari5200", t);
    CB_CHECK(t.judged != 0);
    CB_CHECK_EQ(t.disagree, 0u);
});

// (no 7800 homebrew on this machine: the lane's own test cartridge, a reset, a subroutine and an interrupt handler)
CB_TEST("engine.pcode_cpu.the_atari7800_lane_judges_its_cartridge_the_same_on_both_cores", [] {
    engine::register_atari_lanes();
    fs::ScopedTempDir dir("cb_pcode_cpu_7800");
    Bytes file(128, 0);
    file[0] = 1;
    const std::string magic = "ATARI7800";
    std::copy(magic.begin(), magic.end(), file.begin() + 1);
    file[51] = 0x40;
    Bytes rom(0x4000, 0xFF);
    const Bytes code = {0x78, 0xD8, 0xA9, 0x40, 0x85, 0x3C, 0x20, 0x20, 0xC0, 0x4C, 0x06, 0xC0};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes sub = {0xA5, 0x28, 0x10, 0xFC, 0x8D, 0x24, 0x00, 0x60};
    std::copy(sub.begin(), sub.end(), rom.begin() + 0x20);
    rom[0x30] = 0x40;
    const Bytes vectors = {0x30, 0xC0, 0x00, 0xC0, 0x30, 0xC0};
    std::copy(vectors.begin(), vectors.end(), rom.begin() + 0x3FFA);
    file.insert(file.end(), rom.begin(), rom.end());
    const stdfs::path path = dir.path() / "game.a78";
    fs::write_file(path, file);
    const verify::lanes::ImageLane lane(engine::atari7800_lane_spec());
    const Tally t = judge_on_both_cores(lane, path, [](std::uint32_t entry) -> std::optional<std::size_t> { return entry >= 0xC000 ? std::optional<std::size_t>(128 + entry - 0xC000) : std::nullopt; }, kCap, dir.path(), "a78");
    report("atari7800 (own cartridge)", t);
    CB_CHECK(t.judged != 0);
    CB_CHECK_EQ(t.disagree, 0u);
});

CB_TEST("engine.pcode_cpu.the_lynx_lane_judges_homebrew_the_same_on_both_cores", [] {
    const verify::lanes::ImageLane lane(engine::lynx_lane_spec());
    fs::ScopedTempDir dir("cb_pcode_cpu_lynx");
    Tally total;
    unsigned n = 0;
    for (const char* name : {"loader.lnx", "new_bll.lnx", "overlay.lnx", "boing.o", "fire.o"}) {
        const stdfs::path file = samples_root() / "lynx" / name;
        if (!stdfs::is_regular_file(file)) continue;
        const formats::lynx::Program program = formats::lynx::read(file);
        formats::lynx::Program loaded = program;
        if (loaded.kind == formats::lynx::Kind::Cartridge) {
            const emulate::lynx::Boot boot = emulate::lynx::boot(loaded);
            loaded.set_loaded(boot.segments, boot.entries);
        }
        const Offset offset = [&](std::uint32_t address) -> std::optional<std::size_t> {
            for (const formats::lynx::Segment& s : loaded.segments)
                if (address >= s.address && address < s.address + s.size) return s.offset + (address - s.address);
            return std::nullopt;
        };
        const stdfs::path work = dir.path() / std::to_string(n++);
        stdfs::create_directories(work);
        total.add(judge_on_both_cores(lane, file, offset, kCap, work, name));
    }
    if (n == 0) return;
    report("lynx", total);
    CB_CHECK(total.judged != 0);
    CB_CHECK_EQ(total.disagree, 0u);
});

// (no NES homebrew on this machine: the lane kit's demo cartridge, built with CodeBreaker's own assembler)
CB_TEST("engine.pcode_cpu.the_nes_lane_judges_its_demo_cartridge_the_same_on_both_cores", [] {
    fs::ScopedTempDir dir("cb_pcode_cpu_nes");
    const build::m6502_asm::Assembler assembler(source::isa::m6502_encoder());
    const Bytes rom = assembler.build(fs::read_text(testing::data_path("decompile/testdata/m6502_demo.s")), fs::read_text(testing::data_path("engine/testdata/lane_kit/nes_demo.cfg")), dir.path());
    const stdfs::path cartridge = dir.path() / "demo.nes";
    fs::write_file(cartridge, rom);
    const Kit kit;
    const verify::lanes::ImageLane* lane = kit.lane("nes");
    if (lane == nullptr) return;
    const Tally t = judge_on_both_cores(*lane, cartridge, [&](std::uint32_t entry) -> std::optional<std::size_t> { return entry >= 0xBFF0 && entry - 0xBFF0 < rom.size() ? std::optional<std::size_t>(entry - 0xBFF0) : std::nullopt; }, kCap, dir.path(), "demo.nes");
    report("nes (demo cartridge)", t);
    CB_CHECK(t.judged != 0);
    CB_CHECK_EQ(t.disagree, 0u);
});

CB_TEST("engine.pcode_cpu.the_master_system_and_sg1000_lanes_judge_homebrew_the_same_on_both_cores", [] {
    judge_files("master system / sg-1000", "sms", {"sms-peterlemon/HelloWorld.sms", "sms-peterlemon/SMSAudio.sms", "sms-peterlemon/Sprite.sms", "sg1000-240p/suitesg.sg"}, false);
});
// One instruction of the Z80 at a time, from random registers and memory, on each core; what differs is listed by opcode. (F bits 3 and 5 are not in the description, R
// is not kept, and the repeating block instructions go on for many steps: those are left out.)
CB_TEST("engine.pcode_cpu.z80_instructions_one_by_one_match_the_own_core", [] {
    std::mt19937 rng(20261120);
    std::map<std::string, unsigned> by_opcode, tried;
    std::string first_report;
    std::map<std::string, unsigned> missing;
    unsigned different = 0, runs = 0, lacking = 0;
    for (int n = 0; n < 120000; ++n) {
        Bytes image(0x10000);
        for (auto& b : image) b = static_cast<std::uint8_t>(rng());
        std::uint8_t code[4];
        for (auto& b : code) b = static_cast<std::uint8_t>(rng());
        if (rng() % 3 == 0) code[0] = "\xCB\xDD\xED\xFD"[rng() % 4];
        if (code[0] == 0xED && (code[1] & 0xF4) == 0xB0 && (code[1] & 3) != 0xFF && code[1] >= 0xB0) continue;   // LDIR CPIR INIR OTIR LDDR ...
        std::copy(code, code + 4, image.begin() + 0x8000);
        const auto rd = [&] { return static_cast<std::uint8_t>(rng()); };
        emulate::z80::RunOptions options;
        options.max_steps = 1;
        options.sp = static_cast<std::uint16_t>(0x9000 + (rng() & 0xFFF));
        options.regs = {rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd(), static_cast<std::uint16_t>(rng()), static_cast<std::uint16_t>(rng())};
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
