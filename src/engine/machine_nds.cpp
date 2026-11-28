// cb-unity: alone (file-local names that other files in a batch also use)
#include "engine/machine_nds.hpp"

#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/arm.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "formats/read.hpp"
#include "platforms/nds.hpp"
#include "formats/nds.hpp"
#include "source/nds_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_nds(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (head.size() >= 0x200 && head[0x15C] == 0x56 && head[0x15D] == 0xCF && head[0xC0] == 0x24 && head[0xC1] == 0xFF) {   // a Nintendo DS ROM: the logo, and its checksum $CF56 at $15C
        std::string title(as_text(clamped(head, 0, 12)));
        while (!title.empty() && title.back() == '\0') title.pop_back();
        std::string code(as_text(clamped(head, 0x0C, 4)));
        return one("Nintendo DS cartridge", std::format("\"{}\" ({}), {} bytes, ARM9 {} bytes at 0x{:08X}, ARM7 {} bytes at 0x{:08X}", title, code, size, get<std::uint32_t>(file, 0x2C, "detect"), get<std::uint32_t>(file, 0x28, "detect"), get<std::uint32_t>(file, 0x3C, "detect"), get<std::uint32_t>(file, 0x38, "detect")), "nds", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace nds_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // register accesses kept of a run
// Main RAM a run may be handed pointers into: seeded from `fill`, above where either program is loaded.
constexpr std::uint32_t kWork = 0x02200000;
constexpr std::size_t kWorkBytes = 0x10000;
constexpr std::uint32_t kStack9 = 0x027C3F00;   // the ARM9 stack: the top of the data TCM
constexpr std::uint32_t kStack7 = 0x0380FF00;   // the ARM7 stack: the top of its own WRAM

std::uint8_t nds_fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t nds_at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State nds_state(std::uint32_t a0, std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t s0, int fill) {
    return {{"a0", a0}, {"a1", a1}, {"a2", a2}, {"a3", a3}, {"s0", s0}, {"fill", fill}};
}

vl::Effects nds_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::nds::Rom rom = formats::nds::read(path);
    const std::optional<formats::nds::Cpu> which = rom.cpu_of(entry & ~1u);
    if (!which)
        return {vl::Effect(std::string("no program holds the address")), vl::Effect(std::int64_t{0}), vl::Effect(std::int64_t{0}), vl::Effect(Bytes()),
                vl::Effect(std::int64_t{0}), vl::Effect(std::vector<std::string>()), vl::Effect(std::vector<std::string>()), vl::Effect(std::vector<std::string>())};
    const bool nine = *which == formats::nds::Cpu::Arm9;
    emulate::arm::Bus bus(nine ? emulate::arm::System::Nds9 : emulate::arm::System::Nds7);
    bus.load(rom.binary(*which).ram_address, rom.memory(*which));
    if (const std::int64_t seed = nds_at(state, "fill"); seed != 0) {
        Bytes work(kWorkBytes);
        for (std::size_t i = 0; i < work.size(); ++i) work[i] = nds_fill_byte(i, seed);
        bus.load(kWork, work);
    }
    // the scanline counter moves on every read, so a wait for the vertical blank ends (263 lines, 192 of them visible); no key is down
    const auto scanline = std::make_shared<std::uint32_t>(0);
    bus.map(0x04000004, 0x04000007, [scanline](std::uint32_t address, unsigned) -> std::uint32_t {
        const std::uint32_t line = (*scanline)++ % 263;
        const std::uint32_t word = ((line >= 192 && line < 262) ? 1u : 0u) | line << 16;
        return word >> (8 * (address - 0x04000004));
    });
    bus.map(0x04000130, 0x04000131, [](std::uint32_t address, unsigned) -> std::uint32_t { return address == 0x04000130 ? 0xFF : 0x03; });   // KEYINPUT: nothing pressed
    struct Region {
        std::uint32_t base;
        ByteSpan memory;
    };
    std::vector<Region> regions = {{0x02000000, bus.ewram()}, {0x03000000, bus.iwram()}, {0x05000000, bus.palette()}, {0x06000000, bus.vram()}, {0x07000000, bus.oam()}};
    if (nine) {
        regions.push_back({0x00000000, bus.itcm()});
        regions.push_back({emulate::arm::kNdsDtcmBase, bus.dtcm()});
    } else {
        regions.push_back({emulate::arm::kNdsWram7Base, bus.wram7()});
    }
    std::vector<Bytes> before;
    for (const Region& r : regions) before.emplace_back(r.memory.begin(), r.memory.end());
    emulate::arm::RunOptions options;
    options.max_steps = kSteps;
    options.sp = nine ? kStack9 : kStack7;
    options.a = {static_cast<std::uint32_t>(nds_at(state, "a0")), static_cast<std::uint32_t>(nds_at(state, "a1")), static_cast<std::uint32_t>(nds_at(state, "a2")),
                 static_cast<std::uint32_t>(nds_at(state, "a3"))};
    options.s[0] = static_cast<std::uint32_t>(nds_at(state, "s0"));
    const emulate::arm::RunResult run = emulate::pcode_cpu::call(bus, entry, options);
    // what the run changed in the memories of the machine (the program is in main RAM: its own bytes are not an effect unless the run rewrote them)
    std::vector<std::string> changed;
    for (std::size_t k = 0; k < regions.size(); ++k)
        for (std::size_t i = 0; i < before[k].size() && changed.size() < kIoShown; ++i)
            if (before[k][i] != regions[k].memory[i]) changed.push_back(std::format("{:08x}={:02x}", regions[k].base + i, regions[k].memory[i]));
    Bytes saved;
    for (std::size_t i = 4; i < 12; ++i)
        for (int shift = 0; shift < 32; shift += 8) saved.push_back(static_cast<std::uint8_t>(run.r[i] >> shift));
    std::vector<std::string> io;
    for (const emulate::arm::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    std::vector<std::string> bios;
    for (const emulate::arm::BiosCall& c : run.bios) bios.push_back(c.text());
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.r[0]), number(run.r[1]), vl::Effect(std::move(saved)), number(run.r[13]),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io)), vl::Effect(std::move(bios))};
}

vl::Written nds_written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::nds_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace nds_lane_detail

namespace d = nds_lane_detail;

vl::ImageLaneSpec nds_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "nds";
    spec.rev = 1;
    spec.states = {d::nds_state(0, 0, 0, 0, 0, 0), d::nds_state(d::kWork, d::kWork + 0x1000, 5, 0x10, 0, 1), d::nds_state(d::kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2),
                   d::nds_state(2, 3, d::kWork + 0x100, d::kWork + 0x200, 7, 3)};
    spec.effects = d::nds_effects;
    spec.what = {"end", "R0", "R1", "R4-R11", "SP", "memory changes", "hardware accesses", "BIOS calls"};
    spec.start = [](const vl::State& s) { return std::format("from R0={:08X} R1={:08X} R2={:X}", d::nds_at(s, "a0"), d::nds_at(s, "a1"), d::nds_at(s, "a2")); };
    spec.writer = d::nds_written_by;
    spec.key_digits = 8;
    return spec;
}

void register_nds_lane() {
    register_lane({
        .name = "nds",
        .platform = platforms::nds,
        .probe = [](const stdfs::path& p) { return formats::nds::probe(p); },
        .make_spec = nds_lane_spec,
        .detectors = {{.priority = 200, .detect = formats::detect::info_nds}},
        .program_extensions = {".nds"},
    });
}

}  // namespace cb::engine
