// cb-unity: alone
#include "engine/machine_atari5200.hpp"

#include <array>
#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/m6502.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "formats/read.hpp"
#include "formats/atari5200.hpp"
#include "source/console_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_atari5200(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".a52", ".bin", ".rom"})) {   // an Atari 5200 cartridge: 4, 8, 16 or 32 KB ending with a start address inside the ROM
        try {
            const formats::atari5200::Rom rom = formats::atari5200::parse(file);
            const std::uint32_t first = *rom.offset(rom.start);
            if (name_ends_with(name, {".a52"}) || (rom.data[first] != 0x00 && rom.data[first] != 0xFF)) {
                return one("Atari 5200 cartridge", std::format("{} KB at ${:04X}, start ${:04X}", size / 1024, rom.base, rom.start), "atari5200", "reach");
            }
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

namespace atari5200_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // device accesses kept of a run
constexpr std::size_t kRamSize = 0x4000;  // $0000-$3FFF

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State a5200_state(int a, int x, int y, int fill) { return {{"a", a}, {"x", x}, {"y", y}, {"fill", fill}}; }

// GTIA as an idle console: no collisions, the triggers not pressed (the bit is set), no console key down, NTSC.
std::uint8_t gtia_read(std::uint16_t address) {
    const unsigned reg = address & 0x1F;
    if (reg >= 0x10 && reg <= 0x13) return 0x01;   // TRIG0-3
    if (reg == 0x14) return 0x0F;                  // PAL (NTSC)
    if (reg == 0x1F) return 0x0F;                  // CONSOL
    return 0;
}

}  // namespace atari5200_lane_detail

namespace {

namespace d = atari5200_lane_detail;

vl::Effects a5200_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::atari5200::Rom rom = formats::atari5200::read(path);
    emulate::m6502::Bus bus(rom.memory());
    auto ram = std::make_shared<std::array<std::uint8_t, d::kRamSize>>();
    if (const std::int64_t seed = d::at(state, "fill"); seed != 0) {
        for (std::size_t i = 0; i < ram->size(); ++i) (*ram)[i] = d::fill_byte(i, seed);
    }
    auto vcount = std::make_shared<unsigned>(0);   // ANTIC's scan line counter moves on each read
    auto random = std::make_shared<std::uint8_t>(static_cast<std::uint8_t>(0x5A));   // POKEY's RANDOM: a fixed sequence
    bus.map_quiet(
        0x0000, 0x3FFF, [ram](std::uint16_t a) { return (*ram)[a]; }, [ram](std::uint16_t a, std::uint8_t v) { (*ram)[a] = v; });
    bus.map(0xC000, 0xCFFF, d::gtia_read);
    bus.map(0xD400, 0xD7FF, [vcount](std::uint16_t a) -> std::uint8_t {
        switch (a & 0x0F) {
            case 0x0B: return static_cast<std::uint8_t>((*vcount)++ & 0x7F);   // VCOUNT
            case 0x0F: return 0x1F;                                             // NMIST
            default: return 0;
        }
    });
    bus.map(0xE800, 0xEFFF, [random](std::uint16_t a) -> std::uint8_t {
        const unsigned reg = a & 0x0F;
        if (reg == 0x0A) {   // RANDOM
            *random = static_cast<std::uint8_t>(*random * 5 + 17);
            return *random;
        }
        if (reg == 0x08 || reg == 0x09 || reg == 0x0E || reg == 0x0F || reg == 0x0D) return 0xFF;   // ALLPOT, KBCODE, IRQST, SKSTAT, SERIN: idle
        return 0;                                                                                  // POT0-7 at rest
    });
    bus.map(0xF800, 0xFFFF, [](std::uint16_t) -> std::uint8_t { return 0x60; });   // the BIOS: every fetch reads RTS
    emulate::m6502::RunOptions options;
    options.a = static_cast<std::uint8_t>(d::at(state, "a"));
    options.x = static_cast<std::uint8_t>(d::at(state, "x"));
    options.y = static_cast<std::uint8_t>(d::at(state, "y"));
    options.max_steps = d::kSteps;
    const emulate::m6502::RunResult run = emulate::pcode_cpu::run(bus, static_cast<std::uint16_t>(entry & 0xFFFF), options);
    std::vector<std::string> io;
    for (const emulate::m6502::DeviceAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.kind == emulate::m6502::DeviceAccess::Kind::Read ? "r" : "w", x.address, x.value));
        if (io.size() == d::kIoShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.regs.a), number(run.regs.x), number(run.regs.y), number(run.regs.p), number(run.regs.s),
            vl::Effect(Bytes(ram->begin(), ram->end())), vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::console_writer::reconstruct("atari5200", module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace

vl::ImageLaneSpec atari5200_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "atari5200";
    spec.rev = 1;
    spec.states = {d::a5200_state(0, 0, 0, 0), d::a5200_state(1, 2, 3, 1), d::a5200_state(0x80, 0xFF, 0x7F, 2), d::a5200_state(0x55, 0x10, 0xAA, 3)};
    spec.effects = a5200_effects;
    spec.what = {"end", "A", "X", "Y", "P", "S", "RAM", "GTIA, ANTIC, POKEY and BIOS accesses"};
    spec.start = [](const vl::State& s) { return std::format("from A={:02X} X={:02X} Y={:02X}", d::at(s, "a"), d::at(s, "x"), d::at(s, "y")); };
    spec.writer = written_by;
    return spec;
}

void register_atari5200_lane() {
    register_lane({
        .name = "atari5200",
        .platform = [] { return platforms::described("atari5200"); },
        .probe = [](const stdfs::path& p) { return formats::atari5200::probe(p); },
        .make_spec = atari5200_lane_spec,
        .detectors = {{.priority = 270, .detect = formats::detect::info_atari5200}},
    });
}

}  // namespace cb::engine
