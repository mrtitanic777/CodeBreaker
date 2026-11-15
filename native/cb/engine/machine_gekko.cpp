// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/engine/machine_gekko.hpp"

#include <format>
#include <map>
#include <memory>
#include <mutex>

#include "cb/core/error.hpp"
#include "cb/decompile/ppc.hpp"
#include "cb/emulate/powerpc.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/gekko_map.hpp"
#include "cb/formats/ppc_program.hpp"
#include "cb/source/gekko_writer.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;
namespace pp = formats::ppc_program;
namespace pw = emulate::powerpc;

namespace gekko_lane_detail {

constexpr std::size_t kShown = 4000;      // memory changes and hardware accesses kept of a run
constexpr std::size_t kCacheSize = 6;     // programs kept loaded (the original and the rebuilt one of a few modules)

// A program as the interpreter runs it: its memory as pages built once (every run of every thread shares them), and the small data area bases
// its start-up code sets.
struct Loaded {
    pp::Program program;
    pw::PageImage image;
    std::optional<std::uint64_t> r2, r13;
};

// The values the start-up code puts in r2 and r13 (`lis rD,hi` then `ori` or `addi`, before its first branch): the bases small variables are
// addressed from. A program that does not set them there gets the interpreter's defaults.
void infer_small_data(Loaded& loaded) {
    std::array<std::optional<std::uint32_t>, 32> value;
    std::uint32_t pc = loaded.program.entry;
    for (unsigned count = 0; count < 96; ++count, pc += 4) {
        const pp::Region* region = loaded.program.code_at(pc);
        if (!region || pc - region->address + 4 > region->size) break;
        const std::size_t at = region->offset + (pc - region->address);
        const ByteView f = loaded.program.file;
        const std::uint32_t w = static_cast<std::uint32_t>(f[at]) << 24 | static_cast<std::uint32_t>(f[at + 1]) << 16 | static_cast<std::uint32_t>(f[at + 2]) << 8 | f[at + 3];
        const unsigned op = w >> 26, rt = (w >> 21) & 31, ra = (w >> 16) & 31;
        if (op == 15 && ra == 0) value[rt] = (w & 0xFFFF) << 16;                                                                                  // lis
        else if (op == 14 && ra != 0 && value[ra]) value[rt] = *value[ra] + static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(w & 0xFFFF)));   // addi
        else if (op == 24 && value[rt]) value[ra] = *value[rt] | (w & 0xFFFF);                                                                    // ori
        else if (op == 18 || op == 16 || (op == 19 && (w >> 1 & 0x3FF) != 0)) break;                                                             // a branch: the set-up is over
    }
    if (value[13]) loaded.r13 = *value[13];
    if (value[2]) loaded.r2 = *value[2];
}

std::shared_ptr<const Loaded> load(const stdfs::path& path) {
    static std::mutex lock;
    static std::map<std::string, std::shared_ptr<const Loaded>> cache;
    std::error_code ec;
    const auto size = stdfs::file_size(path, ec);
    const auto time = stdfs::last_write_time(path, ec).time_since_epoch().count();
    const std::string key = std::format("{}|{}|{}", path.generic_string(), static_cast<std::uint64_t>(size), static_cast<std::int64_t>(time));
    {
        const std::lock_guard guard(lock);
        if (const auto it = cache.find(key); it != cache.end()) return it->second;
    }
    std::optional<pp::Program> program = pp::read(path);
    if (!program) throw FormatError("gekko", 0, "not a GameCube, Wii or Wii U program");
    auto loaded = std::make_shared<Loaded>();
    loaded->program = std::move(*program);
    std::vector<pw::Region> regions;
    for (const pp::Program::Piece& piece : loaded->program.loaded()) regions.push_back({piece.address, Bytes(piece.bytes.begin(), piece.bytes.end())});
    loaded->image = pw::PageImage::of(regions);
    infer_small_data(*loaded);
    const std::lock_guard guard(lock);
    if (cache.size() >= kCacheSize) cache.erase(cache.begin());
    cache[key] = loaded;
    return loaded;
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

// A starting state: the integer arguments, the first three floating arguments in thousandths, and how the work area at 0x60000000 is filled.
vl::State make_state(std::uint32_t r3, std::uint32_t r4, std::uint32_t r5, std::uint32_t r6, std::uint32_t r7, std::int64_t f1, std::int64_t f2, int fill) {
    return {{"r3", r3}, {"r4", r4}, {"r5", r5}, {"r6", r6}, {"r7", r7}, {"f1", f1}, {"f2", f2}, {"fill", fill}};
}

pp::Console console_of(std::string_view name) { return name == "gamecube" ? pp::Console::GameCube : name == "wii" ? pp::Console::Wii : pp::Console::WiiU; }

vl::Effects effects(pp::Console console, const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const Loaded> loaded = load(path);
    pw::RunOptions options;
    options.paired_singles = true;
    for (const formats::gekko_map::Range& r : formats::gekko_map::devices(console)) options.devices.push_back({r.lo, r.hi});
    options.shared = loaded->image;
    options.fill = static_cast<pw::Fill>(at(state, "fill"));
    options.r2 = loaded->r2;
    options.r13 = loaded->r13;
    options.fargs = {static_cast<double>(at(state, "f1")) / 1000.0, static_cast<double>(at(state, "f2")) / 1000.0};
    const std::array<std::uint64_t, 5> args = {static_cast<std::uint32_t>(at(state, "r3")), static_cast<std::uint32_t>(at(state, "r4")), static_cast<std::uint32_t>(at(state, "r5")),
                                               static_cast<std::uint32_t>(at(state, "r6")), static_cast<std::uint32_t>(at(state, "r7"))};
    const pw::RunResult run = pw::run({}, entry, args, options);
    const pw::Machine& m = *run.machine;
    std::vector<std::string> changed;
    for (const pw::ByteWrite& w : run.writes) {
        if (changed.size() == kShown) break;
        changed.push_back(std::format("{:08x}={:02x}", w.address, w.after));
    }
    std::vector<std::string> io;
    for (const pw::Machine::IoAccess& x : m.io()) {
        if (io.size() == kShown) break;
        io.push_back(std::format("{} {:x} {} {:x}", x.write ? "write" : "read", x.address, x.size, x.value));
    }
    Bytes saved;
    for (std::size_t i = 14; i < 32; ++i)
        for (int shift = 24; shift >= 0; shift -= 8) saved.push_back(static_cast<std::uint8_t>(m.r[i] >> shift));
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.truncated ? std::string("limit") : run.exit_text()), number(run.ret & 0xFFFFFFFF), number(m.r[4] & 0xFFFFFFFF), number(run.fret), number(m.ps1[1]), number(m.r[1] & 0xFFFFFFFF),
            vl::Effect(std::move(saved)), vl::Effect(std::move(changed)), vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::gekko_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace gekko_lane_detail

namespace d = gekko_lane_detail;

vl::ImageLaneSpec gekko_lane_spec(std::string_view console_name) {
    const pp::Console console = d::console_of(console_name);
    vl::ImageLaneSpec spec;
    spec.kind = std::string(console_name);
    spec.rev = 1;
    constexpr std::uint32_t kWork = pw::kArgsBase;
    spec.states = {d::make_state(0, 0, 0, 0, 0, 0, 0, 0), d::make_state(kWork, kWork + 0x1000, 5, 0x10, 1, 1500, -2250, 1),
                   d::make_state(kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, kWork + 0x40, 10000000, 3, 2), d::make_state(2, 3, kWork + 0x100, kWork + 0x200, 7, 500, 125, 3)};
    spec.effects = [console](const stdfs::path& path, std::uint32_t entry, const vl::State& state) { return d::effects(console, path, entry, state); };
    spec.what = {"end", "r3", "r4", "f1", "f1 (second half)", "r1", "r14-r31", "memory changes", "hardware accesses"};
    spec.start = [](const vl::State& s) { return std::format("from r3={:08X} r4={:08X} r5={:X}", d::at(s, "r3"), d::at(s, "r4"), d::at(s, "r5")); };
    spec.writer = d::written_by;
    spec.key_digits = 8;
    return spec;
}

void register_gekko_lanes() {
    for (const char* name : {"gamecube", "wii", "wiiu"}) {
        if (machine_lanes().find(name)) continue;
        MachineLane lane;
        lane.name = name;
        const pp::Console console = d::console_of(name);
        lane.probe = [console](const stdfs::path& p) { return pp::probe(p, console); };
        lane.make_spec = [name = std::string(name)] { return gekko_lane_spec(name); };
        try {
            machine_lanes().add(std::move(lane));
        } catch (const UsageError&) {   // (another thread added it first)
        }
    }
}

}  // namespace cb::engine
