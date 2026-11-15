// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/engine/machine_n64.hpp"

#include <format>
#include <memory>

#include "cb/core/error.hpp"
#include "cb/emulate/r4300.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/n64.hpp"
#include "cb/source/n64_writer.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace n64_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kShown = 4000;      // register accesses and memory changes kept of a run
// RDRAM a run may be handed pointers into: seeded from `fill`, above the program (1 MB from the load address) and the stack.
constexpr std::uint32_t kWork = 0x80600000;
constexpr std::size_t kWorkBytes = 0x10000;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State make_state(std::uint32_t a0, std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t s0, int fill) {
    return {{"a0", a0}, {"a1", a1}, {"a2", a2}, {"a3", a3}, {"s0", s0}, {"fill", fill}};
}

vl::Effects effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::n64::Rom rom = formats::n64::read(path);
    Bytes ram = rom.memory();
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        const std::size_t work = kWork & 0x1FFFFFFF;
        for (std::size_t i = 0; i < kWorkBytes; ++i) ram[work + i] = fill_byte(i, seed);
    }
    emulate::r4300::Bus bus(ram, rom.data);
    const Bytes ram_before = ram;
    const Bytes sp_before(bus.sp_memory().begin(), bus.sp_memory().end());
    emulate::r4300::RunOptions options;
    options.max_steps = kSteps;
    options.a = {static_cast<std::uint32_t>(at(state, "a0")), static_cast<std::uint32_t>(at(state, "a1")), static_cast<std::uint32_t>(at(state, "a2")),
                 static_cast<std::uint32_t>(at(state, "a3"))};
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    const emulate::r4300::RunResult run = emulate::r4300::run(bus, entry, options);
    // what the run changed in RDRAM and SP memory (the program is in RDRAM: its own bytes are not an effect unless the run rewrote them)
    std::vector<std::string> changed;
    for (std::size_t i = 0; i < ram.size() && changed.size() < kShown; ++i)
        if (ram_before[i] != bus.rdram()[i]) changed.push_back(std::format("{:08x}={:02x}", i, bus.rdram()[i]));
    for (std::size_t i = 0; i < sp_before.size() && changed.size() < kShown; ++i)
        if (sp_before[i] != bus.sp_memory()[i]) changed.push_back(std::format("{:08x}={:02x}", emulate::r4300::kDmem + i, bus.sp_memory()[i]));
    Bytes saved;
    for (std::size_t i = 16; i < 24; ++i)
        for (int shift = 56; shift >= 0; shift -= 8) saved.push_back(static_cast<std::uint8_t>(run.r[i] >> shift));
    std::vector<std::string> io;
    for (const emulate::r4300::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.r[2]), number(run.r[3]), vl::Effect(std::move(saved)), number(run.r[29]), number(run.hi), number(run.lo), number(run.fpr[0]),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::n64_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace n64_lane_detail

namespace d = n64_lane_detail;

vl::ImageLaneSpec n64_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "n64";
    spec.rev = 1;
    spec.states = {d::make_state(0, 0, 0, 0, 0, 0), d::make_state(d::kWork, d::kWork + 0x1000, 5, 0x10, 0, 1),
                   d::make_state(d::kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), d::make_state(2, 3, d::kWork + 0x100, d::kWork + 0x200, 7, 3)};
    spec.effects = d::effects;
    spec.what = {"end", "V0", "V1", "S0-S7", "SP", "HI", "LO", "F0", "memory changes", "hardware accesses"};
    spec.start = [](const vl::State& s) { return std::format("from A0={:08X} A1={:08X} A2={:X}", d::at(s, "a0"), d::at(s, "a1"), d::at(s, "a2")); };
    spec.writer = d::written_by;
    spec.key_digits = 8;
    return spec;
}

void register_n64_lane() {
    if (machine_lanes().find("n64")) return;
    MachineLane lane;
    lane.name = "n64";
    lane.probe = [](const stdfs::path& p) { return formats::n64::probe(p); };
    lane.make_spec = n64_lane_spec;
    try {
        machine_lanes().add(std::move(lane));
    } catch (const UsageError&) {   // (another thread added it first)
    }
}

}  // namespace cb::engine
