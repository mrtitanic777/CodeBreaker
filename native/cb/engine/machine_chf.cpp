// cb-unity: alone
#include "cb/engine/machine_chf.hpp"

#include <format>
#include <optional>

#include "cb/core/error.hpp"
#include "cb/emulate/f8.hpp"
#include "cb/engine/lane_registry.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/read.hpp"
#include "cb/platforms/chf.hpp"
#include "cb/formats/chf.hpp"
#include "cb/source/chf_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_chf(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if ((name_ends_with(name, {".chf"}) || (name_ends_with(name, {".bin"}) && starts_with(head, 0, std::string_view("U+", 2)))) && size >= 256 && size <= formats::chf::kMaxSize && size % 256 == 0) {   // a Channel F cartridge: no header; `.chf`, or a `.bin` that starts $55 $2B
        try {
            const formats::chf::Rom rom = formats::chf::parse(file);
            return one("Channel F cartridge", std::format("F8, {} bytes at $0800", rom.data.size()), "chf", "reach");
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
namespace ef8 = emulate::f8;

namespace chf_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kIoShown = 4000;
constexpr std::uint16_t kRam = 0x2800, kRamEnd = 0x2FFF;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State chf_state(int a, int r0, int r1, int r2, int r3, int fill) { return {{"a", a}, {"r0", r0}, {"r1", r1}, {"r2", r2}, {"r3", r3}, {"fill", fill}}; }

vl::Effects chf_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::chf::Rom rom = formats::chf::read(path);
    ef8::Bus bus;
    bus.load(0, Bytes(0x800, 0x1C));   // the BIOS: every byte a POP
    bus.load(formats::chf::kBase, rom.data);
    Bytes ram(kRamEnd - kRam + 1, 0);
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        for (std::size_t i = 0; i < ram.size(); ++i) ram[i] = fill_byte(i, seed);
    }
    bus.load(kRam, ram, true);
    bus.set_in([](std::uint8_t) -> std::uint8_t { return 0xFF; });   // no key down
    ef8::RunOptions options;
    options.a = static_cast<std::uint8_t>(at(state, "a"));
    options.r[0] = static_cast<std::uint8_t>(at(state, "r0"));
    options.r[1] = static_cast<std::uint8_t>(at(state, "r1"));
    options.r[2] = static_cast<std::uint8_t>(at(state, "r2"));
    options.r[3] = static_cast<std::uint8_t>(at(state, "r3"));
    options.max_steps = kSteps;
    std::vector<std::string> calls;
    options.on_instruction = [&calls](const ef8::Registers& r) {
        if (r.pc0 >= formats::chf::kBase || calls.size() >= kIoShown) return;
        const auto named = formats::chf::bios().find(r.pc0);
        calls.push_back(named != formats::chf::bios().end() ? named->second : std::format("BIOS_{:04X}", r.pc0));
    };
    const ef8::RunResult run = ef8::run(bus, static_cast<std::uint16_t>(entry), options);
    std::vector<std::string> io;
    for (const ef8::PortAccess& x : run.io) {
        io.push_back(std::format("{} PORT_{} {:x}", x.kind == ef8::PortAccess::Kind::In ? "in" : "out", x.port, x.value));
        if (io.size() == kIoShown) break;
    }
    const ef8::Registers& g = run.regs;
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    const ByteView memory = bus.memory();
    return {vl::Effect(run.exit()), number(g.a), number(g.status), number(g.isar), number(g.dc0), number(g.dc1), vl::Effect(Bytes(g.scratch.begin(), g.scratch.end())),
            vl::Effect(Bytes(memory.begin() + kRam, memory.begin() + kRamEnd + 1)), vl::Effect(std::move(io)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::chf_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace chf_lane_detail

vl::ImageLaneSpec chf_lane_spec() {
    namespace d = chf_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "chf";
    spec.rev = 1;
    spec.states = {d::chf_state(0, 0, 0, 0, 0, 0), d::chf_state(1, 0x10, 0x20, 3, 4, 1), d::chf_state(0x80, 0xFF, 0x3F, 0xA0, 0x7F, 2)};
    spec.effects = d::chf_effects;
    spec.what = {"end", "A", "W", "ISAR", "DC0", "DC1", "scratchpad", "cartridge RAM", "port accesses", "BIOS calls"};
    spec.start = [](const vl::State& s) { return std::format("from A={:02X} R0={:02X} R1={:02X} R2={:02X} R3={:02X}", d::at(s, "a"), d::at(s, "r0"), d::at(s, "r1"), d::at(s, "r2"), d::at(s, "r3")); };
    spec.writer = d::written_by;
    return spec;
}

void register_chf_lane() {
    register_lane({
        .name = "chf",
        .platform = platforms::chf,
        .probe = [](const stdfs::path& p) { return formats::chf::probe(p); },
        .make_spec = chf_lane_spec,
        .detectors = {{.priority = 330, .detect = formats::detect::info_chf}},
    });
}

}  // namespace cb::engine
