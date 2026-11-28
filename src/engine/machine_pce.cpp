#include "engine/machine_pce.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "emulate/huc6280.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu_huc6280.hpp"
#include "formats/read.hpp"
#include "platforms/pce.hpp"
#include "formats/pce.hpp"
#include "source/pce_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_pce(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".pce", ".tg16", ".sgx"})) {   // a PC Engine / TurboGrafx-16 HuCard: whole 8 KB banks, a RESET vector into the top page
        try {
            const formats::pce::Rom rom = formats::pce::parse(file);
            return one("PC Engine HuCard", std::format("{} banks of 8 KB{}, RESET ${:04X}", rom.bank_count(), rom.header.empty() ? "" : " behind a 512-byte header", rom.vectors.reset), "pce", "reach");
        } catch (const FormatError&) {
        }
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace pce_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // chip accesses kept of a run

// The card read once for each version of its file (a rebuilt program is a new file; the lane runs each function many times).
std::shared_ptr<const formats::pce::Rom> loaded_rom(const stdfs::path& path) {
    static std::mutex lock;
    static std::map<std::string, std::pair<std::string, std::shared_ptr<const formats::pce::Rom>>> cache;
    std::error_code ec;
    const std::string stamp = std::format("{}:{}", stdfs::file_size(path, ec), stdfs::last_write_time(path, ec).time_since_epoch().count());
    const std::string key = fs::to_text(path);
    {
        std::lock_guard hold(lock);
        if (const auto it = cache.find(key); it != cache.end() && it->second.first == stamp) return it->second.second;
    }
    auto rom = std::make_shared<const formats::pce::Rom>(formats::pce::read(path));
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

vl::State pce_state(int a, int x, int y, int fill) { return {{"a", a}, {"x", x}, {"y", y}, {"fill", fill}}; }

vl::Effects pce_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const formats::pce::Rom> rom = loaded_rom(path);
    emulate::huc6280::Machine machine(rom->data);
    const std::uint16_t logical = static_cast<std::uint16_t>(entry & 0xFFFF);
    machine.mpr[logical >> 13] = static_cast<std::uint8_t>(entry >> 16);   // the function's own bank, where its address is
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        for (std::size_t i = 0; i < machine.ram.size(); ++i) machine.ram[i] = fill_byte(i, seed);
    }
    emulate::huc6280::Options options;
    options.a = static_cast<std::uint8_t>(at(state, "a"));
    options.x = static_cast<std::uint8_t>(at(state, "x"));
    options.y = static_cast<std::uint8_t>(at(state, "y"));
    options.max_steps = kSteps;
    const emulate::huc6280::Run run = emulate::pcode_cpu::run(machine, logical, options);
    std::vector<std::string> io;
    for (const emulate::huc6280::Access& x : run.io) {
        io.push_back(x.text());
        if (io.size() == kIoShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()),  number(run.regs.a), number(run.regs.x), number(run.regs.y), number(run.regs.p), number(run.regs.s),
            vl::Effect(Bytes(machine.ram.begin(), machine.ram.end())), vl::Effect(Bytes(machine.mpr.begin(), machine.mpr.end())), vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::pce_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace pce_lane_detail

vl::ImageLaneSpec pce_lane_spec() {
    namespace d = pce_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "pce";
    spec.rev = 1;
    spec.states = {d::pce_state(0, 0, 0, 0), d::pce_state(1, 2, 3, 1), d::pce_state(0x80, 0xFF, 0x7F, 2), d::pce_state(0x55, 0x10, 0xAA, 3)};
    spec.effects = d::pce_effects;
    spec.what = {"end", "A", "X", "Y", "P", "S", "RAM", "mapping registers", "chip accesses"};
    spec.start = [](const vl::State& s) { return std::format("from A={:02X} X={:02X} Y={:02X}", d::at(s, "a"), d::at(s, "x"), d::at(s, "y")); };
    spec.writer = d::written_by;
    spec.key_digits = 6;   // bank << 16 | address
    return spec;
}

void register_pce_lane() {
    register_lane({
        .name = "pce",
        .platform = platforms::pce,
        .probe = [](const stdfs::path& p) { return formats::pce::probe(p); },
        .make_spec = pce_lane_spec,
        .detectors = {{.priority = 220, .detect = formats::detect::info_pce}},
    });
}

}  // namespace cb::engine
