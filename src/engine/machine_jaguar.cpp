// cb-unity: alone
#include "engine/machine_jaguar.hpp"

#include <array>
#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/m68k.hpp"
#include "engine/lane_registry.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/read.hpp"
#include "platforms/jaguar.hpp"
#include "formats/jaguar.hpp"
#include "source/jaguar_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_jaguar(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".j64", ".jag", ".rom", ".cof", ".abs"})) {   // an Atari Jaguar cartridge (the start address at $404) or COFF executable
        try {
            const formats::jaguar::Program program = formats::jaguar::parse(file);
            return one(program.kind == formats::jaguar::Kind::Cartridge ? "Atari Jaguar cartridge" : "Atari Jaguar COFF executable",
                       std::format("{} bytes, start ${:06X}, {} segment(s)", size, program.entry, program.segments.size()), "jaguar", "reach");
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
namespace m68k = emulate::m68k;

namespace jaguar_lane_detail {

constexpr std::uint64_t kSteps = 20000;      // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;       // device accesses kept of a run
constexpr std::uint32_t kLowRam = 0x1000;    // the DRAM compared: [kLowRam, kLowEnd) and [kTop, 0x200000)
constexpr std::uint32_t kLowEnd = 0x20000;
constexpr std::uint32_t kTop = 0x1F0000;
constexpr std::uint32_t kStack = 0x1FFF00;
constexpr std::uint32_t kGpuRam = 0xF03000;  // 4 KB
constexpr std::uint32_t kDspRam = 0xF1B000;  // 8 KB

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const std::string& key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State jaguar_state(const std::array<std::uint32_t, 8>& d, const std::array<std::uint32_t, 7>& a, int fill_seed) {
    vl::State s{{"fill", fill_seed}};
    for (std::size_t i = 0; i < 8; ++i) s["d" + std::to_string(i)] = d[i];
    for (std::size_t i = 0; i < 7; ++i) s["a" + std::to_string(i)] = a[i];
    return s;
}

void put_long(Bytes& out, std::uint32_t v) {
    for (int shift = 24; shift >= 0; shift -= 8) out.push_back(static_cast<std::uint8_t>(v >> shift));
}

vl::Effect bytes_of(const Bytes& mem, std::uint32_t from, std::size_t length) {
    return vl::Effect(Bytes(mem.begin() + from, mem.begin() + from + length));
}

vl::Effects jaguar_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::jaguar::Program program = formats::jaguar::read(path);
    Bytes mem = program.memory();
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {   // the DRAM the program is not in
        for (std::uint32_t a = kLowRam; a < 0x200000; a = (a == kLowEnd - 1 ? kTop : a + 1)) {
            if (!program.segment_at(a)) mem[a] = fill_byte(a, seed);
        }
    }
    m68k::Bus bus(mem);
    const auto lines = std::make_shared<unsigned>(0);   // the raster counters move on each read
    const auto tom = [lines](std::uint32_t a, unsigned) -> std::uint32_t {
        if (a == 0xF00004) return (*lines)++ & 0x7FF;   // HC
        if (a == 0xF00006) return (*lines)++ & 0x7FF;   // VC
        return 0;
    };
    const auto jerry = [](std::uint32_t a, unsigned) -> std::uint32_t { return a == 0xF14000 || a == 0xF14002 ? 0xFFFF : 0; };   // JOYSTICK, JOYBUTS: nothing pressed
    bus.map(0xF00000, kGpuRam - 1, tom);
    bus.map(kGpuRam + 0x1000, 0xF0FFFF, tom);
    bus.map(0xF10000, kDspRam - 1, jerry);
    bus.map(kDspRam + 0x2000, 0xF1FFFF, jerry);

    m68k::RunOptions options;
    options.max_steps = kSteps;
    options.sp = kStack;
    std::array<std::uint32_t, 8> d{}, a{};
    for (std::size_t i = 0; i < 8; ++i) d[i] = static_cast<std::uint32_t>(at(state, "d" + std::to_string(i)));
    for (std::size_t i = 0; i < 7; ++i) a[i] = static_cast<std::uint32_t>(at(state, "a" + std::to_string(i)));
    options.d = d;
    options.a = a;
    const m68k::RunResult run = cb::engine::pcode_cpu::call(bus, entry, options, cb::engine::pcode_cpu::M68kMachine::Jaguar);

    Bytes dregs, aregs;
    for (std::size_t i = 0; i < 8; ++i) put_long(dregs, run.d[i]);
    for (std::size_t i = 0; i < 7; ++i) put_long(aregs, run.a[i]);
    std::vector<std::string> io;
    for (const m68k::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    const Bytes after(bus.memory().begin(), bus.memory().end());
    return {vl::Effect(run.exit()),
            vl::Effect(std::move(dregs)),
            vl::Effect(std::move(aregs)),
            vl::Effect(static_cast<std::int64_t>(run.sr & 0x1F)),
            bytes_of(after, kLowRam, kLowEnd - kLowRam),
            bytes_of(after, kTop, 0x10000),
            bytes_of(after, kGpuRam, 0x1000),
            bytes_of(after, kDspRam, 0x2000),
            vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::jaguar_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace jaguar_lane_detail

vl::ImageLaneSpec jaguar_lane_spec() {
    namespace d = jaguar_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "jaguar";
    spec.rev = 1;
    spec.states = {d::jaguar_state({0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, 0),
                   d::jaguar_state({1, 2, 3, 4, 5, 6, 7, 8}, {0x100100, 0x100200, 0x100300, 0x100400, 0x100500, 0x100600, 0x100700}, 1),
                   d::jaguar_state({0x8000, 0xFFFF, 0x7F, 0x80000000, 0x10, 0x20, 0x30, 0x40}, {0x108000, 0x109000, 0x10A000, 0x10B000, 0x10C000, 0x10D000, 0x10E000}, 2)};
    spec.effects = d::jaguar_effects;
    spec.what = {"end", "D registers", "A registers", "condition codes", "DRAM", "stack DRAM", "GPU RAM", "DSP RAM", "Tom and Jerry accesses"};
    spec.start = [](const vl::State& s) { return std::format("from D0={:08X} A0={:06X}", d::at(s, "d0"), d::at(s, "a0")); };
    spec.writer = d::written_by;
    spec.key_digits = 6;
    return spec;
}

void register_jaguar_lane() {
    register_lane({
        .name = "jaguar",
        .platform = platforms::jaguar,
        .probe = [](const stdfs::path& p) { return formats::jaguar::probe(p); },
        .make_spec = jaguar_lane_spec,
        .detectors = {{.priority = 250, .detect = formats::detect::info_jaguar}},
    });
}

}  // namespace cb::engine
