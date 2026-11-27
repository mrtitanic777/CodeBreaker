// cb-unity: alone
#include "engine/machine_o2.hpp"

#include <array>
#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/i8048.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/pcode_cpu_i8048.hpp"
#include "formats/read.hpp"
#include "platforms/o2.hpp"
#include "formats/o2.hpp"
#include "source/o2_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_o2(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".o2"}) && size >= 256 && size <= formats::o2::kMaxSize && size % 256 == 0) {   // an Odyssey 2 cartridge: no header; the file name and a whole number of pages up to 3 KB
        try {
            const formats::o2::Rom rom = formats::o2::parse(file);
            return one("Odyssey 2 cartridge", std::format("8048, {} bytes at $0400", rom.data.size()), "o2", "reach");
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
namespace e48 = emulate::i8048;

namespace o2_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kIoShown = 4000;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State o2_state(int a, int r0, int r1, int r2, int r3, int fill) { return {{"a", a}, {"r0", r0}, {"r1", r1}, {"r2", r2}, {"r3", r3}, {"fill", fill}}; }

std::string place_text(const e48::DeviceAccess& x) {
    static const char* names[] = {"DATA", "BUS", "P1", "P2", "DBB", "P4", "P5", "P6", "P7", "T0", "T1", "INT"};
    if (x.place == e48::Place::Data) {
        const auto it = formats::o2::vdc().find(x.address);
        return it != formats::o2::vdc().end() ? it->second : std::format("DATA_{:02X}", x.address);
    }
    return names[static_cast<unsigned>(x.place)];
}

vl::Effects o2_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::o2::Rom rom = formats::o2::read(path);
    e48::Bus bus;
    bus.load(0, Bytes(0x400, 0x83));   // the BIOS: every byte a RET
    bus.load(formats::o2::kBase, rom.data);
    auto data = std::make_shared<std::array<std::uint8_t, 256>>();
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        for (std::size_t i = 0; i < data->size(); ++i) (*data)[i] = fill_byte(i, seed);
    }
    auto reads = std::make_shared<unsigned>(0);
    bus.set_read([data, reads](e48::Place place, std::uint8_t address) -> std::uint8_t {
        switch (place) {
            case e48::Place::Data: return address == 0xA1 ? ((*reads)++ & 1 ? 0xFF : 0x00) : (*data)[address];
            case e48::Place::T1: return (*reads)++ & 1 ? 1 : 0;
            case e48::Place::Int: return 1;
            case e48::Place::T0: case e48::Place::Dbb: return 0;
            default: return 0xFF;   // the ports: no key down
        }
    });
    bus.set_write([data](e48::Place place, std::uint8_t address, std::uint8_t value) {
        if (place == e48::Place::Data) (*data)[address] = value;
    });
    e48::RunOptions options;
    options.a = static_cast<std::uint8_t>(at(state, "a"));
    options.r[0] = static_cast<std::uint8_t>(at(state, "r0"));
    options.r[1] = static_cast<std::uint8_t>(at(state, "r1"));
    options.r[2] = static_cast<std::uint8_t>(at(state, "r2"));
    options.r[3] = static_cast<std::uint8_t>(at(state, "r3"));
    options.max_steps = kSteps;
    std::vector<std::string> calls;
    options.on_instruction = [&calls](const e48::Registers& r) {
        if (r.pc >= formats::o2::kBase || calls.size() >= kIoShown) return;
        calls.push_back(std::format("BIOS_{:04X}", r.pc));
    };
    const e48::RunResult run = pcode_cpu::run(bus, static_cast<std::uint16_t>(entry), options);
    std::vector<std::string> io;
    for (const e48::DeviceAccess& x : run.io) {
        io.push_back(std::format("{} {} {:x}", x.kind == e48::DeviceAccess::Kind::Read ? "r" : "w", place_text(x), x.value));
        if (io.size() == kIoShown) break;
    }
    const e48::Registers& g = run.regs;
    return {vl::Effect(run.exit()), vl::Effect(static_cast<std::int64_t>(g.a)), vl::Effect(static_cast<std::int64_t>(g.psw())), vl::Effect(static_cast<std::int64_t>(g.p1)),
            vl::Effect(static_cast<std::int64_t>(g.p2)), vl::Effect(Bytes(g.ram.begin(), g.ram.end())), vl::Effect(Bytes(data->begin(), data->end())), vl::Effect(std::move(io)),
            vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::o2_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace o2_lane_detail

vl::ImageLaneSpec o2_lane_spec() {
    namespace d = o2_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "o2";
    spec.rev = 1;
    spec.states = {d::o2_state(0, 0, 0, 0, 0, 0), d::o2_state(1, 0x20, 0x21, 3, 4, 1), d::o2_state(0x80, 0xFF, 0x3F, 0xA0, 0x7F, 2)};
    spec.effects = d::o2_effects;
    spec.what = {"end", "A", "PSW", "P1 latch", "P2 latch", "internal RAM", "data space", "port and data accesses", "BIOS calls"};
    spec.start = [](const vl::State& s) { return std::format("from A={:02X} R0={:02X} R1={:02X} R2={:02X} R3={:02X}", d::at(s, "a"), d::at(s, "r0"), d::at(s, "r1"), d::at(s, "r2"), d::at(s, "r3")); };
    spec.writer = d::written_by;
    return spec;
}

void register_o2_lane() {
    register_lane({
        .name = "o2",
        .platform = platforms::o2,
        .probe = [](const stdfs::path& p) { return formats::o2::probe(p); },
        .make_spec = o2_lane_spec,
        .detectors = {{.priority = 320, .detect = formats::detect::info_o2}},
    });
}

}  // namespace cb::engine
