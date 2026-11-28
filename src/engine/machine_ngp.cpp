// cb-unity: alone
#include "engine/machine_ngp.hpp"

#include <array>
#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/tlcs900.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/pcode_cpu_tlcs900.hpp"
#include "formats/read.hpp"
#include "platforms/ngp.hpp"
#include "formats/ngp.hpp"
#include "source/ngp_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_ngp(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".ngp", ".ngc", ".npc"}) && size >= 64) {   // a Neo Geo Pocket cartridge: the SNK licence text, then the start address; the lane reads it with the TLCS-900/H core
        try {
            const formats::ngp::Rom rom = formats::ngp::parse(file);
            return one(rom.header.colour ? "Neo Geo Pocket Color cartridge" : "Neo Geo Pocket cartridge",
                       std::format("TLCS-900/H, \"{}\", {} bytes, start at 0x{:06X}", rom.header.title, size, rom.header.start), "ngp", "reach");
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
namespace et = emulate::tlcs900;

namespace ngp_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kIoShown = 4000;
constexpr std::uint32_t kWorkRam = 0x004000;
constexpr std::size_t kWorkRamSize = 0x3000;      // $004000-$006FFF: the program's RAM, the battery-backed RAM and the BIOS's variables
constexpr std::uint32_t kZ80Ram = 0x007000;
constexpr std::size_t kZ80RamSize = 0x1000;
constexpr std::uint32_t kBiosBase = 0xFF0000;
constexpr std::uint32_t kBiosStubs = 0xFF0100;    // vector n's stub: the `ret` at kBiosStubs + n
constexpr std::uint32_t kBiosTable = 0xFFFE00;    // 64 entries of 4 bytes
constexpr unsigned kBiosVectors = 64;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State ngp_state(int a, int b, int c, int d, int fill) { return {{"a", a}, {"b", b}, {"c", c}, {"d", d}, {"fill", fill}}; }

// A register of the machine's devices by name, else its address.
std::string place(std::uint32_t address) {
    const auto it = formats::ngp::hardware().find(address);
    return it != formats::ngp::hardware().end() ? it->second : std::format("{:06X}", address);
}

// The K2GE's registers and the CPU's I/O: they give back what was written; the raster line and the status read as a display that is drawing, then in VBlank.
struct Registers {
    std::array<std::uint8_t, 0x200> k2ge{};
    std::array<std::uint8_t, 0x100> io{};
    unsigned reads = 0;
};

vl::Effects ngp_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::ngp::Rom rom = formats::ngp::read(path);
    et::Bus bus;
    const std::int64_t seed = at(state, "fill");
    Bytes work(kWorkRamSize, 0);
    if (seed != 0) {
        for (std::size_t i = 0; i < work.size(); ++i) work[i] = fill_byte(i, seed);
    }
    auto regs = std::make_shared<Registers>();
    bus.device(
        0x000000, 0x0000FF,
        [regs](std::uint32_t a, unsigned size) -> std::uint32_t {
            std::uint32_t v = 0;
            for (unsigned k = 0; k < size; ++k) v |= static_cast<std::uint32_t>(regs->io[(a + k) & 0xFF]) << (8 * k);
            return v;
        },
        [regs](std::uint32_t a, unsigned size, std::uint32_t v) {
            for (unsigned k = 0; k < size; ++k) regs->io[(a + k) & 0xFF] = static_cast<std::uint8_t>(v >> (8 * k));
        });
    bus.device(
        0x008000, 0x0081FF,
        [regs](std::uint32_t a, unsigned size) -> std::uint32_t {
            std::uint32_t v = 0;
            for (unsigned k = 0; k < size; ++k) {
                const std::uint32_t r = a + k;
                std::uint8_t b = regs->k2ge[r - 0x8000];
                if (r == 0x8009) b = (regs->reads++ & 1) ? 0xA0 : 0x00;   // K2GE_RAS_V
                else if (r == 0x8010) b = (regs->reads++ & 1) ? 0x40 : 0x00;   // K2GE_STATUS: VBlank
                v |= static_cast<std::uint32_t>(b) << (8 * k);
            }
            return v;
        },
        [regs](std::uint32_t a, unsigned size, std::uint32_t v) {
            for (unsigned k = 0; k < size; ++k) regs->k2ge[(a + k - 0x8000) & 0x1FF] = static_cast<std::uint8_t>(v >> (8 * k));
        });
    bus.memory(0x008200, 0x00BFFF, {}, true, true);   // palettes, sprites, maps, tiles
    bus.memory(kWorkRam, kWorkRam + kWorkRamSize - 1, work, true, false);
    bus.memory(kZ80Ram, kZ80Ram + kZ80RamSize - 1, {}, true, false);
    bus.memory(0x200000, 0x3FFFFF, rom.data, false, false, rom.data.size());
    Bytes bios(0x10000, 0x0E);   // every byte a `ret`
    for (unsigned n = 0; n < kBiosVectors; ++n) {
        const std::uint32_t target = kBiosStubs + n;
        for (unsigned k = 0; k < 4; ++k) bios[kBiosTable - kBiosBase + 4 * n + k] = static_cast<std::uint8_t>(target >> (8 * k));
    }
    bus.memory(kBiosBase, 0xFFFFFF, bios, false, false);

    et::RunOptions options;
    options.xwa = static_cast<std::uint32_t>(at(state, "a"));
    options.xbc = static_cast<std::uint32_t>(at(state, "b"));
    options.xde = static_cast<std::uint32_t>(at(state, "c"));
    options.xhl = static_cast<std::uint32_t>(at(state, "d"));
    options.args = {static_cast<std::uint16_t>(at(state, "d")), static_cast<std::uint16_t>(at(state, "c")), static_cast<std::uint16_t>(at(state, "b")),
                    static_cast<std::uint16_t>(at(state, "a"))};
    options.sp = 0x006C00;
    options.max_steps = kSteps;
    std::vector<std::string> calls;   // the BIOS: a call through the vector table, by number
    options.on_instruction = [&calls](const et::Registers& r) {
        if (r.pc >= kBiosStubs && r.pc < kBiosStubs + kBiosVectors && calls.size() < kIoShown) calls.push_back(std::format("BIOS_VECTOR_{}", r.pc - kBiosStubs));
    };
    const et::RunResult run = pcode_cpu::run(bus, entry, options);
    std::vector<std::string> io;
    for (const et::DeviceAccess& x : run.io) {
        io.push_back(std::format("{} {} {} {:x}", x.kind == et::DeviceAccess::Kind::Read ? "r" : "w", place(x.address), x.size, x.value));
        if (io.size() == kIoShown) break;
    }
    for (const et::SoftwareInterrupt& s : run.swis) {
        if (calls.size() == kIoShown) break;
        calls.push_back(std::format("SWI_{} fn={} {:x} {:x} {:x} {:x}", s.number, s.function, s.regs[0], s.regs[1], s.regs[2], s.regs[3]));
    }
    vl::Effects out;
    out.push_back(vl::Effect(run.exit()));
    for (const auto& bank : run.regs.bank) {
        for (const std::uint32_t r : bank) out.push_back(vl::Effect(static_cast<std::int64_t>(r)));
    }
    for (const std::uint32_t r : {run.regs.xix, run.regs.xiy, run.regs.xiz, run.regs.xsp}) out.push_back(vl::Effect(static_cast<std::int64_t>(r)));
    out.push_back(vl::Effect(static_cast<std::int64_t>(run.regs.sr)));
    out.push_back(vl::Effect(static_cast<std::int64_t>(run.regs.f_alt)));
    const std::span<std::uint8_t> work_after = bus.bytes(kWorkRam);
    out.push_back(vl::Effect(Bytes(work_after.begin(), work_after.end())));
    const std::span<std::uint8_t> z80_after = bus.bytes(kZ80Ram);
    out.push_back(vl::Effect(Bytes(z80_after.begin(), z80_after.end())));
    out.push_back(vl::Effect(std::move(io)));
    out.push_back(vl::Effect(std::move(calls)));
    return out;
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::ngp_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace ngp_lane_detail

vl::ImageLaneSpec ngp_lane_spec() {
    namespace d = ngp_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "ngp";
    spec.rev = 1;
    spec.key_digits = 6;
    spec.states = {d::ngp_state(0, 0, 0, 0, 0), d::ngp_state(1, 2, 3, 4, 1), d::ngp_state(0x004100, 0xFF, 0x200100, -1, 2)};
    spec.effects = d::ngp_effects;
    spec.what = {"end"};
    for (unsigned bank = 0; bank < 4; ++bank) {
        for (const char* r : {"XWA", "XBC", "XDE", "XHL"}) spec.what.push_back(std::format("{}{}", r, bank));
    }
    for (const char* w : {"XIX", "XIY", "XIZ", "XSP", "SR", "F'", "work RAM", "Z80 RAM", "I/O, K2GE and video memory accesses", "BIOS calls"}) spec.what.push_back(w);
    spec.start = [](const vl::State& s) { return std::format("from XWA={:X} XBC={:X} XDE={:X} XHL={:X}", d::at(s, "a"), d::at(s, "b"), d::at(s, "c"), d::at(s, "d")); };
    spec.writer = d::written_by;
    return spec;
}

void register_ngp_lane() {
    register_lane({
        .name = "ngp",
        .platform = platforms::ngp,
        .probe = [](const stdfs::path& p) { return formats::ngp::probe(p); },
        .make_spec = ngp_lane_spec,
        .detectors = {{.priority = 310, .detect = formats::detect::info_ngp}},
    });
}

}  // namespace cb::engine
