// cb-unity: alone
#include "cb/engine/machine_threedo.hpp"

#include <format>
#include <memory>

#include "cb/core/error.hpp"
#include "cb/emulate/arm.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/threedo.hpp"
#include "cb/source/threedo_writer.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace threedo_lane_detail {

constexpr std::uint64_t kSteps = 20000;      // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;       // changes and accesses kept of a run
constexpr std::uint32_t kLoad = 0x02000000;  // where the image is loaded: the interpreter's main RAM
constexpr std::uint32_t kWork = 0x02200000;  // free memory a run may be handed pointers into, seeded from `fill`
constexpr std::size_t kWorkBytes = 0x10000;
constexpr std::uint32_t kStack = 0x023FFF00;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State threedo_state(std::uint32_t a0, std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t s0, int fill) {
    return {{"a0", a0}, {"a1", a1}, {"a2", a2}, {"a3", a3}, {"s0", s0}, {"fill", fill}};
}

vl::Effects threedo_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::threedo::Program program = formats::threedo::read(path);
    emulate::arm::Bus bus(emulate::arm::System::Nds7);
    bus.set_big_endian(true);
    bus.load(kLoad, program.native());
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        Bytes work(kWorkBytes);
        for (std::size_t i = 0; i < work.size(); ++i) work[i] = fill_byte(i, seed);
        bus.load(kWork, work);
    }
    const auto lines = std::make_shared<std::uint32_t>(0);   // Clio's vertical counter moves on each read
    bus.map(0x03300000, 0x033FFFFF, [](std::uint32_t, unsigned) -> std::uint32_t { return 0; });
    bus.map(0x03400000, 0x034FFFFF, [lines](std::uint32_t address, unsigned) -> std::uint32_t { return address == 0x03400014 ? (*lines)++ % 263 : 0; });
    const ByteSpan ram = bus.ewram();
    const Bytes before(ram.begin(), ram.end());

    emulate::arm::RunOptions options;
    options.max_steps = kSteps;
    options.sp = kStack;
    options.a = {static_cast<std::uint32_t>(at(state, "a0")), static_cast<std::uint32_t>(at(state, "a1")), static_cast<std::uint32_t>(at(state, "a2")),
                 static_cast<std::uint32_t>(at(state, "a3"))};
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    const emulate::arm::RunResult run = emulate::arm::run(bus, kLoad + entry, options);

    std::vector<std::string> changed;
    for (std::size_t i = 0; i < before.size() && changed.size() < kIoShown; ++i) {
        if (before[i] != ram[i]) changed.push_back(std::format("{:08x}={:02x}", kLoad + i, ram[i]));
    }
    Bytes saved;
    for (std::size_t i = 4; i < 12; ++i) {
        for (int shift = 0; shift < 32; shift += 8) saved.push_back(static_cast<std::uint8_t>(run.r[i] >> shift));
    }
    std::vector<std::string> io;
    for (const emulate::arm::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    std::vector<std::string> calls;
    for (const emulate::arm::BiosCall& c : run.bios) calls.push_back(c.text());
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.r[0]), number(run.r[1]), vl::Effect(std::move(saved)), number(run.r[13]),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::threedo_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace threedo_lane_detail

vl::ImageLaneSpec threedo_lane_spec() {
    namespace d = threedo_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "threedo";
    spec.rev = 1;
    spec.states = {d::threedo_state(0, 0, 0, 0, 0, 0), d::threedo_state(d::kWork, d::kWork + 0x1000, 5, 0x10, 0, 1),
                   d::threedo_state(d::kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), d::threedo_state(2, 3, d::kWork + 0x100, d::kWork + 0x200, 7, 3)};
    spec.effects = d::threedo_effects;
    spec.what = {"end", "R0", "R1", "R4-R11", "SP", "memory changes", "Madam and Clio accesses", "operating-system calls"};
    spec.start = [](const vl::State& s) { return std::format("from R0={:08X} R1={:08X} R2={:X}", d::at(s, "a0"), d::at(s, "a1"), d::at(s, "a2")); };
    spec.writer = d::written_by;
    spec.key_digits = 6;
    return spec;
}

void register_threedo_lane() {
    if (machine_lanes().find("threedo")) return;
    MachineLane lane;
    lane.name = "threedo";
    lane.probe = [](const stdfs::path& p) { return formats::threedo::probe(p); };
    lane.make_spec = threedo_lane_spec;
    try {
        machine_lanes().add(std::move(lane));
    } catch (const UsageError&) {   // (another thread added it first)
    }
}

}  // namespace cb::engine
