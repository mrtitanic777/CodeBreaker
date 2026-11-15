#include "cb/engine/machine_snes.hpp"

#include <format>
#include <map>
#include <memory>
#include <mutex>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/emulate/w65816.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/snes.hpp"
#include "cb/source/snes_writer.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace snes_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // chip accesses kept of a run

// The cartridge read once for each version of its file (a rebuilt program is a new file; the lane runs each function many times).
std::shared_ptr<const formats::snes::Rom> loaded_rom(const stdfs::path& path) {
    static std::mutex lock;
    static std::map<std::string, std::pair<std::string, std::shared_ptr<const formats::snes::Rom>>> cache;
    std::error_code ec;
    const std::string stamp = std::format("{}:{}", stdfs::file_size(path, ec), stdfs::last_write_time(path, ec).time_since_epoch().count());
    const std::string key = fs::to_text(path);
    {
        std::lock_guard hold(lock);
        if (const auto it = cache.find(key); it != cache.end() && it->second.first == stamp) return it->second.second;
    }
    auto rom = std::make_shared<const formats::snes::Rom>(formats::snes::read(path));
    std::lock_guard hold(lock);
    cache[key] = {stamp, rom};
    return rom;
}

// How RAM is filled from a seed (0: left as it is): small numbers on odd seeds, as counters and indexes hold.
std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State snes_state(int a, int x, int y, int fill) { return {{"a", a}, {"x", x}, {"y", y}, {"fill", fill}}; }

vl::Effects snes_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const formats::snes::Rom> rom = loaded_rom(path);
    emulate::w65816::Machine machine(*rom);
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        for (std::size_t i = 0; i < machine.wram.size(); ++i) machine.wram[i] = fill_byte(i, seed);
    }
    emulate::w65816::Options options;
    options.a = static_cast<std::uint16_t>(at(state, "a"));
    options.x = static_cast<std::uint16_t>(at(state, "x"));
    options.y = static_cast<std::uint16_t>(at(state, "y"));
    options.native = (entry & source::snes_writer::kStartsNative) != 0;
    options.m16 = (entry & source::snes_writer::kStartsWith16BitAccumulator) != 0;
    options.x16 = (entry & source::snes_writer::kStartsWith16BitIndex) != 0;
    options.max_steps = kSteps;
    const emulate::w65816::Run run = emulate::w65816::run(machine, entry & source::snes_writer::kAddressMask, options);
    std::vector<std::string> io;
    for (const emulate::w65816::Access& x : run.io) {
        io.push_back(x.text());
        if (io.size() == kIoShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.regs.a),  number(run.regs.x),  number(run.regs.y), number(run.regs.p), number(run.regs.s),
            number(run.regs.d),     number(run.regs.dbr), vl::Effect(Bytes(machine.wram.begin(), machine.wram.end())), vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::snes_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace snes_lane_detail

vl::ImageLaneSpec snes_lane_spec() {
    namespace d = snes_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "snes";
    spec.rev = 1;
    spec.states = {d::snes_state(0, 0, 0, 0), d::snes_state(1, 2, 3, 1), d::snes_state(0x80, 0xFF, 0x7F, 2), d::snes_state(0x1234, 0x0102, 0x0304, 3)};
    spec.effects = d::snes_effects;
    spec.what = {"end", "A", "X", "Y", "P", "S", "D", "DBR", "RAM", "chip accesses"};
    spec.start = [](const vl::State& s) { return std::format("from A={:04X} X={:04X} Y={:04X}", d::at(s, "a"), d::at(s, "x"), d::at(s, "y")); };
    spec.writer = d::written_by;
    spec.key_digits = 8;   // the state a function starts in is above its 24-bit address
    return spec;
}

void register_snes_lane() {
    if (machine_lanes().find("snes")) return;
    MachineLane lane;
    lane.name = "snes";
    lane.probe = [](const stdfs::path& p) { return formats::snes::probe(p); };
    lane.make_spec = snes_lane_spec;
    try {
        machine_lanes().add(std::move(lane));
    } catch (const UsageError&) {   // (another thread added it first)
    }
}

}  // namespace cb::engine
