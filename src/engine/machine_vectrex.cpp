// cb-unity: alone
#include "engine/machine_vectrex.hpp"

#include <array>
#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/m6809.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/pcode_cpu_m6809.hpp"
#include "formats/read.hpp"
#include "platforms/vectrex.hpp"
#include "formats/vectrex.hpp"
#include "source/vectrex_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_vectrex(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".vec", ".bin", ".gam"}) && starts_with(head, 0, "g GCE")) {   // a Vectrex cartridge: the header the BIOS reads starts `g GCE`
        try {
            const formats::vectrex::Rom rom = formats::vectrex::parse(file);
            return one("Vectrex cartridge", std::format("\"{}\", {} bytes, program at ${:04X}{}", rom.copyright, size, rom.entry, rom.title.empty() ? std::string() : ", \"" + rom.title.front().text + "\""), "vectrex", "reach");
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

namespace vectrex_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kIoShown = 4000;
constexpr std::size_t kRamSize = 0x400;   // $C800-$CBFF
constexpr std::uint16_t kRamBase = 0xC800;
constexpr std::uint16_t kStack = 0xCBFF;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State vectrex_state(int a, int b, int x, int y, int u, int dp, int fill) { return {{"a", a}, {"b", b}, {"x", x}, {"y", y}, {"u", u}, {"dp", dp}, {"fill", fill}}; }

vl::Effects vectrex_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::vectrex::Rom rom = formats::vectrex::read(path);
    Bytes image = rom.memory();
    std::fill(image.begin() + 0xE000, image.end(), std::uint8_t{0x39});   // the system ROM: every byte an RTS
    emulate::m6809::Bus bus(image);
    auto ram = std::make_shared<std::array<std::uint8_t, kRamSize>>();
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        for (std::size_t i = 0; i < ram->size(); ++i) (*ram)[i] = fill_byte(i, seed);
    }
    const auto rom_bytes = std::make_shared<const Bytes>(rom.data);
    bus.map_quiet(
        0x0000, 0x7FFF, [rom_bytes](std::uint16_t a) -> std::uint8_t { return a < rom_bytes->size() ? (*rom_bytes)[a] : 0; }, [](std::uint16_t, std::uint8_t) {});   // (the cartridge is ROM)
    bus.map_quiet(
        0xC800, 0xCFFF, [ram](std::uint16_t a) { return (*ram)[(a - kRamBase) & (kRamSize - 1)]; },
        [ram](std::uint16_t a, std::uint8_t v) { (*ram)[(a - kRamBase) & (kRamSize - 1)] = v; });
    auto flag_reads = std::make_shared<unsigned>(0);
    bus.map(0xD000, 0xD7FF, [flag_reads](std::uint16_t a) -> std::uint8_t {
        switch (a & 0x0F) {
            case 0x01: case 0x0F: return 0xFF;                       // port A: no button down
            case 0x0D: return (*flag_reads)++ & 1 ? 0x40 : 0x00;     // the interrupt flags: timer 1 on every second read
            default: return 0;
        }
    });

    emulate::m6809::RunOptions options;
    options.a = static_cast<std::uint8_t>(at(state, "a"));
    options.b = static_cast<std::uint8_t>(at(state, "b"));
    options.x = static_cast<std::uint16_t>(at(state, "x"));
    options.y = static_cast<std::uint16_t>(at(state, "y"));
    options.u = static_cast<std::uint16_t>(at(state, "u"));
    options.dp = static_cast<std::uint8_t>(at(state, "dp"));
    options.s = kStack;
    options.max_steps = kSteps;
    std::vector<std::string> calls;
    options.on_instruction = [&calls](const emulate::m6809::Registers& r) {
        if (r.pc < 0xE000 || calls.size() >= kIoShown) return;
        const auto named = source::vectrex_writer::bios().find(r.pc);
        calls.push_back(named != source::vectrex_writer::bios().end() ? named->second : std::format("BIOS_{:04X}", r.pc));
    };
    const emulate::m6809::RunResult run = pcode_cpu::call(bus, static_cast<std::uint16_t>(entry), options);
    std::vector<std::string> io;
    for (const emulate::m6809::DeviceAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.kind == emulate::m6809::DeviceAccess::Kind::Read ? "r" : "w", x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.regs.a), number(run.regs.b), number(run.regs.x), number(run.regs.y), number(run.regs.u), number(run.regs.s), number(run.regs.dp),
            number(run.regs.cc), vl::Effect(Bytes(ram->begin(), ram->end())), vl::Effect(std::move(io)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::vectrex_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace vectrex_lane_detail

vl::ImageLaneSpec vectrex_lane_spec() {
    namespace d = vectrex_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "vectrex";
    spec.rev = 1;
    spec.states = {d::vectrex_state(0, 0, 0, 0, 0, 0xC8, 0), d::vectrex_state(1, 2, 0xC900, 0xCA00, 0xCB00, 0xD0, 1), d::vectrex_state(0x80, 0xFF, 0xC880, 0x7F, 0xCA80, 0xC8, 2)};
    spec.effects = d::vectrex_effects;
    spec.what = {"end", "A", "B", "X", "Y", "U", "S", "DP", "CC", "RAM", "VIA accesses", "BIOS calls"};
    spec.start = [](const vl::State& s) { return std::format("from A={:02X} B={:02X} X={:04X} DP={:02X}", d::at(s, "a"), d::at(s, "b"), d::at(s, "x"), d::at(s, "dp")); };
    spec.writer = d::written_by;
    return spec;
}

void register_vectrex_lane() {
    register_lane({
        .name = "vectrex",
        .platform = platforms::vectrex,
        .probe = [](const stdfs::path& p) { return formats::vectrex::probe(p); },
        .make_spec = vectrex_lane_spec,
        .detectors = {{.priority = 290, .detect = formats::detect::info_vectrex}},
    });
}

}  // namespace cb::engine
