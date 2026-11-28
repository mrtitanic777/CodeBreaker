// cb-unity: alone
#include "engine/machine_intv.hpp"

#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/cp1610.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu_cp1610.hpp"
#include "formats/read.hpp"
#include "platforms/intv.hpp"
#include "formats/intv.hpp"
#include "source/intv_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_intv(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".rom", ".int"}) && size >= 519) {   // an Intellivision cartridge: an Intellicart .rom ($A8, N, ~N and the segments) or a raw .int
        try {
            const formats::intv::Rom rom = formats::intv::parse(file, !name_ends_with(name, {".rom"}));
            return one(rom.raw ? "Intellivision cartridge (raw words)" : "Intellivision cartridge (Intellicart .rom)",
                       std::format("CP1610, {} segment(s), {} words, \"{}\"{}", rom.segments.size(), rom.word_count(), rom.title(), rom.entry() ? std::format(", program at ${:04X}", *rom.entry()) : std::string()), "intv", "reach");
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

namespace intv_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kIoShown = 4000;
constexpr std::uint16_t kScratch = 0x0100, kScratchEnd = 0x01EF, kSystem = 0x0200, kSystemEnd = 0x035F;

std::uint16_t fill_word(std::uint64_t i, std::int64_t seed, std::uint16_t mask) {
    return static_cast<std::uint16_t>((((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) * 0x0101) & (seed % 2 != 0 ? 0x0F0F : 0xFFFF) & mask);
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State intv_state(int a, int b, int c, int d, int e, int fill) { return {{"a", a}, {"b", b}, {"c", c}, {"d", d}, {"e", e}, {"fill", fill}}; }

std::string place(std::uint32_t address) {
    const auto it = source::intv_writer::hardware().find(address);
    return it != source::intv_writer::hardware().end() ? it->second : std::format("{:04X}", address);
}

vl::Effects intv_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::intv::Rom rom = formats::intv::read(path);
    emulate::cp1610::Bus bus;
    const std::int64_t seed = at(state, "fill");
    std::vector<std::uint16_t> scratch(kScratchEnd - kScratch + 1, 0), system(kSystemEnd - kSystem + 1, 0);
    if (seed != 0) {
        for (std::size_t i = 0; i < scratch.size(); ++i) scratch[i] = fill_word(i, seed, 0xFF);
        for (std::size_t i = 0; i < system.size(); ++i) system[i] = fill_word(i + 1000, seed, 0xFFFF);
    }
    bus.device(0x0000, 0x007F, [](std::uint16_t) -> std::uint16_t { return 0; }, {}, true, 0x3FFF);                        // the STIC
    bus.memory(kScratch, kScratchEnd, scratch, true, false, 0xFF);
    bus.device(0x01F0, 0x01FF, [](std::uint16_t a) -> std::uint16_t { return a >= 0x01FE ? 0xFF : 0; }, {}, true, 0xFFFF);   // the PSG, its controller ports
    bus.memory(kSystem, kSystemEnd, system, true, false, 0xFFFF);
    bus.memory(0x1000, 0x1FFF, std::vector<std::uint16_t>(0x1000, 0x00AF), false, false);                                   // the executive ROM: every word a return through R5
    bus.memory(0x3800, 0x39FF, {}, true, true, 0xFF);                                                                       // the graphics RAM
    for (const formats::intv::Segment& s : rom.segments) bus.memory(static_cast<std::uint16_t>(s.start), static_cast<std::uint16_t>(s.end() - 1), s.words, false);

    emulate::cp1610::RunOptions options;
    options.r = {static_cast<std::uint16_t>(at(state, "a")), static_cast<std::uint16_t>(at(state, "b")), static_cast<std::uint16_t>(at(state, "c")),
                 static_cast<std::uint16_t>(at(state, "d")), static_cast<std::uint16_t>(at(state, "e"))};
    options.sp = 0x02F0;
    options.max_steps = kSteps;
    std::vector<std::string> calls;
    options.on_instruction = [&calls](const emulate::cp1610::Registers& r) {
        if (r.r[7] < 0x1000 || r.r[7] > 0x1FFF || calls.size() >= kIoShown) return;
        calls.push_back(std::format("EXEC_{:04X}", r.r[7]));
    };
    const emulate::cp1610::RunResult run = emulate::pcode_cpu::run(bus, static_cast<std::uint16_t>(entry), options);
    std::vector<std::string> io;
    for (const emulate::cp1610::DeviceAccess& x : run.io) {
        io.push_back(std::format("{} {} {:x}", x.kind == emulate::cp1610::DeviceAccess::Kind::Read ? "r" : "w", place(x.address), x.value));
        if (io.size() == kIoShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    const std::span<std::uint16_t> scratch_after = bus.words(kScratch), system_after = bus.words(kSystem);
    const auto as_bytes = [](std::span<const std::uint16_t> w) {
        Bytes b;
        for (const std::uint16_t v : w) {
            b.push_back(static_cast<std::uint8_t>(v));
            b.push_back(static_cast<std::uint8_t>(v >> 8));
        }
        return b;
    };
    const emulate::cp1610::Registers& g = run.regs;
    return {vl::Effect(run.exit()), number(g.r[0]), number(g.r[1]), number(g.r[2]), number(g.r[3]), number(g.r[4]), number(g.r[6]), number(g.status()),
            vl::Effect(as_bytes(scratch_after)), vl::Effect(as_bytes(system_after)), vl::Effect(std::move(io)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::intv_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace intv_lane_detail

vl::ImageLaneSpec intv_lane_spec() {
    namespace d = intv_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "intv";
    spec.rev = 2;
    spec.states = {d::intv_state(0, 0, 0, 0, 0, 0), d::intv_state(1, 2, 0x0200, 0x0100, 0x5000, 1), d::intv_state(0x8000, 0xFFFF, 0x0021, 0x01F0, 0x3800, 2)};
    spec.effects = d::intv_effects;
    spec.what = {"end", "R0", "R1", "R2", "R3", "R4", "R6", "flags", "scratch RAM", "system RAM", "STIC/PSG/GRAM accesses", "BIOS calls"};
    spec.start = [](const vl::State& s) { return std::format("from R0={:04X} R1={:04X} R2={:04X} R3={:04X} R4={:04X}", d::at(s, "a"), d::at(s, "b"), d::at(s, "c"), d::at(s, "d"), d::at(s, "e")); };
    spec.writer = d::written_by;
    return spec;
}

void register_intv_lane() {
    register_lane({
        .name = "intv",
        .platform = platforms::intv,
        .probe = [](const stdfs::path& p) { return formats::intv::probe(p); },
        .make_spec = intv_lane_spec,
        .detectors = {{.priority = 300, .detect = formats::detect::info_intv}},
    });
}

}  // namespace cb::engine
