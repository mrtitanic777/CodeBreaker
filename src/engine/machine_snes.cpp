#include "engine/machine_snes.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "emulate/w65816.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/pcode_cpu_w65816.hpp"
#include "formats/read.hpp"
#include "platforms/snes.hpp"
#include "formats/snes.hpp"
#include "source/snes_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_snes(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".sfc", ".smc", ".swc", ".fig"})) {   // a Super Nintendo cartridge: an internal header at $7FC0 or $FFC0 that reads as one
        try {
            const formats::snes::Rom rom = formats::snes::parse(file);
            return one("Super Nintendo cartridge", std::format("\"{}\", {}, {} bytes{}, RESET ${:04X}", rom.title, formats::snes::to_string(rom.mapping), rom.data.size(),
                                                              rom.copier_header.empty() ? "" : " behind a 512-byte header", rom.emulation.reset), "snes", "reach");
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
    const emulate::w65816::Run run = pcode_cpu::call(machine, entry & source::snes_writer::kAddressMask, options);
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
    register_lane({
        .name = "snes",
        .platform = platforms::snes,
        .probe = [](const stdfs::path& p) { return formats::snes::probe(p); },
        .make_spec = snes_lane_spec,
        .detectors = {{.priority = 230, .detect = formats::detect::info_snes}},
        .program_extensions = {".sfc", ".smc"},
    });
}

}  // namespace cb::engine
