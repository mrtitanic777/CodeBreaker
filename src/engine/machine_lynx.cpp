#include "engine/machine_lynx.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "emulate/lynx.hpp"
#include "engine/lynx_boot.hpp"
#include "emulate/m6502.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "platforms/lynx.hpp"
#include "formats/lynx.hpp"
#include "source/lynx_writer.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace lynx_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // device accesses kept of a run

// What the boot ROM leaves of a file, found once for each version of it (a rebuilt program is a new file; the lane runs each function many times).
std::shared_ptr<const formats::lynx::Program> loaded_program(const stdfs::path& path) {
    static std::mutex lock;
    static std::map<std::string, std::pair<std::string, std::shared_ptr<const formats::lynx::Program>>> cache;
    std::error_code ec;
    const std::string stamp = std::format("{}:{}", stdfs::file_size(path, ec), stdfs::last_write_time(path, ec).time_since_epoch().count());
    const std::string key = fs::to_text(path);
    {
        std::lock_guard hold(lock);
        if (const auto it = cache.find(key); it != cache.end() && it->second.first == stamp) return it->second.second;
    }
    auto program = std::make_shared<formats::lynx::Program>(formats::lynx::read(path));
    const emulate::lynx::Boot boot = engine::lynx_boot::boot(*program);
    if (program->kind == formats::lynx::Kind::Cartridge) program->set_loaded(boot.segments, boot.entries);
    std::lock_guard hold(lock);
    cache[key] = {stamp, program};
    return program;
}

// How RAM is filled from a seed (0: left as it is): small numbers on odd seeds, as counters and indexes hold.
std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State lynx_state(int a, int x, int y, int fill) { return {{"a", a}, {"x", x}, {"y", y}, {"fill", fill}}; }

vl::Effects lynx_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const formats::lynx::Program> program = loaded_program(path);
    Bytes mem = program->memory();
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        std::vector<bool> program_bytes(formats::lynx::kRamEnd, false);   // RAM the loaders wrote is the program; the rest is free to be filled
        for (const formats::lynx::Segment& s : program->segments) {
            for (std::uint32_t i = 0; i < s.size && s.address + i < formats::lynx::kRamEnd; ++i) program_bytes[s.address + i] = true;
        }
        for (std::uint32_t a = 0; a < formats::lynx::kRamEnd; ++a) {
            const bool stage1 = a >= formats::lynx::kStage1Address && a < formats::lynx::kStage1Address + program->stage1.size();
            if (!program_bytes[a] && !stage1) mem[a] = fill_byte(a, seed);
        }
    }
    const Bytes before = mem;
    emulate::m6502::Bus bus = emulate::lynx::make_bus(mem);
    emulate::m6502::RunOptions options;
    options.a = static_cast<std::uint8_t>(at(state, "a"));
    options.x = static_cast<std::uint8_t>(at(state, "x"));
    options.y = static_cast<std::uint8_t>(at(state, "y"));
    options.max_steps = kSteps;
    options.decimal = true;
    options.variant = emulate::m6502::Variant::Cmos;
    const emulate::m6502::RunResult run = emulate::pcode_cpu::run(bus, static_cast<std::uint16_t>(entry), options);
    std::vector<std::string> io;
    for (const emulate::m6502::DeviceAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.kind == emulate::m6502::DeviceAccess::Kind::Read ? "r" : "w", x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    std::vector<std::string> changed;   // the RAM the function changed (the program lives in RAM: its own bytes are not an effect unless it rewrote them)
    for (std::uint32_t a = 0; a < formats::lynx::kRamEnd; ++a) {
        if (before[a] != bus.memory()[a]) changed.push_back(std::format("{:04x}={:02x}", a, bus.memory()[a]));
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.regs.a), number(run.regs.x), number(run.regs.y), number(run.regs.p), number(run.regs.s),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::lynx_writer::reconstruct(module, out, [](formats::lynx::Program& p) {
        const emulate::lynx::Boot boot = engine::lynx_boot::boot(p);
        p.set_loaded(boot.segments, boot.entries);
    });
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace lynx_lane_detail

using namespace lynx_lane_detail;

vl::ImageLaneSpec lynx_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "lynx";
    spec.rev = 1;
    spec.states = {lynx_state(0, 0, 0, 0), lynx_state(1, 2, 3, 1), lynx_state(0x80, 0xFF, 0x7F, 2), lynx_state(0x55, 0x10, 0xAA, 3)};
    spec.effects = lynx_effects;
    spec.what = {"end", "A", "X", "Y", "P", "S", "RAM changed", "Suzy and Mikey accesses"};
    spec.start = [](const vl::State& s) { return std::format("from A={:02X} X={:02X} Y={:02X}", lynx_lane_detail::at(s, "a"), lynx_lane_detail::at(s, "x"), lynx_lane_detail::at(s, "y")); };
    spec.writer = written_by;
    return spec;
}

void register_lynx_lane() {
    register_lane({
        .name = "lynx",
        .platform = platforms::lynx,
        .probe = [](const stdfs::path& p) { return formats::lynx::probe(p); },
        .make_spec = lynx_lane_spec,
    });
}

}  // namespace cb::engine
