#include "engine/machine_m68k.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/m68k.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/read.hpp"
#include "platforms/neogeo.hpp"
#include "platforms/segacd.hpp"
#include "formats/neogeo.hpp"
#include "formats/segacd.hpp"
#include "source/neogeo_writer.hpp"
#include "source/segacd_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_neogeo(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (head.size() >= 0x108 && (starts_with(head, 0x100, "NEO-GEO") || starts_with(head, 0x100, "EN-OEG"))) {   // a Neo Geo P-ROM: "NEO-GEO" at $100, or with its words swapped
        const bool swapped = starts_with(head, 0x100, "EN-OEG");
        const unsigned ngh = swapped ? head[0x109] << 8 | head[0x108] : head[0x108] << 8 | head[0x109];
        return one("Neo Geo cartridge (P-ROM)", std::format("NGH {:04X}, {} KB{}", ngh, size / 1024, swapped ? ", words swapped" : ""), "neogeo", "reach");
    }
    if (starts_with(h4, 0, "NEO") && size >= 4096) {   // a .neo file: a 4 KB header, then the P, S, M, V and C ROMs
        return one("Neo Geo .neo file", std::format("{} KB P-ROM, {} KB sound program, {} KB sprites", get<std::uint32_t>(file, 4, "detect") / 1024,
                                                      get<std::uint32_t>(file, 12, "detect") / 1024, get<std::uint32_t>(file, 24, "detect") / 1024),
                   "neogeo", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_segacd(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (segacd::is_disc_start(head)) {   // a Sega CD disc: "SEGADISCSYSTEM" starts its first sector, which holds the boot header
        try {
            const segacd::Disc disc = segacd::parse(file);
            return one("Sega CD disc", std::format("\"{}\", {}-byte sectors, IP {} bytes at 0x{:X}, SP {} bytes at 0x{:X}", disc.header.domestic_name, disc.sector_size,
                                                 disc.header.ip.size, disc.header.ip.start, disc.header.sp.size, disc.header.sp.start),
                       "segacd", "reach");
        } catch (const Error&) {   // a damaged boot header is no disc of this machine
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

namespace m68k_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // device accesses kept of a run
constexpr std::size_t kRamShown = 0x10000;

// How RAM is filled from a seed (0: left as it is): small numbers on odd seeds, as counters and indexes hold.
std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

void fill(Bytes& mem, std::uint32_t at, std::size_t length, std::int64_t seed) {
    if (seed == 0) return;
    for (std::size_t i = 0; i < length; ++i) mem[at + i] = fill_byte(i, seed);
}

std::int64_t at(const vl::State& s, const std::string& key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State m68k_state(const std::array<std::uint32_t, 8>& d, const std::array<std::uint32_t, 7>& a, int fill_seed) {
    vl::State s{{"fill", fill_seed}};
    for (std::size_t i = 0; i < 8; ++i) s["d" + std::to_string(i)] = d[i];
    for (std::size_t i = 0; i < 7; ++i) s["a" + std::to_string(i)] = a[i];
    return s;
}

vl::Written written_of(const source::image_writer::Written& w) {
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

// The BIOS as stubs: every word an RTS.
void put_rts(Bytes& mem, std::uint32_t lo, std::uint32_t hi) {
    for (std::uint32_t a = lo; a + 1 < hi; a += 2) {
        mem[a] = 0x4E;
        mem[a + 1] = 0x75;
    }
}

// One call of a BIOS jump table (a 6-byte slot): MOVE.W D0,(port).W ; RTS -- the call is a logged write of D0 to a port of its own.
void put_call_stub(Bytes& mem, std::uint32_t at_slot, std::uint16_t port) {
    const std::array<std::uint8_t, 6> stub = {0x31, 0xC0, static_cast<std::uint8_t>(port >> 8), static_cast<std::uint8_t>(port), 0x4E, 0x75};
    std::copy(stub.begin(), stub.end(), mem.begin() + at_slot);
}

constexpr std::uint32_t kCallPorts = 0xFFE000;   // the ports the stubs write (an absolute short address reaches $FFE000 as $E000)
constexpr std::uint32_t kCallPortsEnd = 0xFFE1FF;

std::string io_line(const m68k::BusAccess& x) { return std::format("{} {:x} {:x}", x.text(), x.address, x.value); }

vl::Effect number(std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); }

vl::Effect bytes_of(ByteView v) { return vl::Effect(Bytes(v.begin(), v.end())); }

void put_long(Bytes& out, std::uint32_t v) {
    for (int shift = 24; shift >= 0; shift -= 8) out.push_back(static_cast<std::uint8_t>(v >> shift));
}

// A run's outcome: how it ended, D0-D7, A0-A6, the condition codes, each RAM given, then the device accesses in order.
vl::Effects outcome(const m68k::RunResult& run, const std::vector<vl::Effect>& rams) {
    Bytes dregs, aregs;
    for (std::size_t i = 0; i < 8; ++i) put_long(dregs, run.d[i]);
    for (std::size_t i = 0; i < 7; ++i) put_long(aregs, run.a[i]);
    std::vector<std::string> io;
    for (const m68k::BusAccess& x : run.io) {
        io.push_back(io_line(x));
        if (io.size() == kIoShown) break;
    }
    vl::Effects out = {vl::Effect(run.exit()), vl::Effect(std::move(dregs)), vl::Effect(std::move(aregs)), number(run.sr & 0x1F)};
    out.insert(out.end(), rams.begin(), rams.end());
    out.emplace_back(std::move(io));
    return out;
}

m68k::RunOptions options_of(const vl::State& state, std::uint32_t sp, bool sub_pointers) {
    m68k::RunOptions options;
    options.max_steps = kSteps;
    options.sp = sp;
    std::array<std::uint32_t, 8> d{}, a{};
    for (std::size_t i = 0; i < 8; ++i) d[i] = static_cast<std::uint32_t>(at(state, "d" + std::to_string(i)));
    for (std::size_t i = 0; i < 7; ++i) {
        a[i] = static_cast<std::uint32_t>(at(state, "a" + std::to_string(i)));
        if (sub_pointers && a[i] >= 0xFF0000) a[i] = 0x10000 + (a[i] & 0x7FFF);   // the sub CPU's RAM is below $80000: the state's pointers go to its free PRG-RAM
    }
    options.d = d;
    options.a = a;
    return options;
}

// ---- the Neo Geo -----------------------------------------------------------------------------------------------------

constexpr std::uint32_t kNeoWorkRam = 0x100000;
constexpr std::uint32_t kNeoPalette = 0x400000;
constexpr std::size_t kVramWords = 0x8800;

// The LSPC: its VRAM, address and modulo registers, and a raster line that advances on each status read.
struct Lspc {
    std::vector<std::uint16_t> vram = std::vector<std::uint16_t>(kVramWords, 0);
    std::uint16_t address = 0, modulo = 0;
    unsigned reads = 0;
};

vl::Effects neogeo_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::neogeo::Rom rom = formats::neogeo::read(path);
    Bytes mem = rom.memory();
    put_rts(mem, 0xC00000, 0xC20000);
    for (std::uint32_t slot = 0xC00438; slot <= 0xC004CE; slot += 6) put_call_stub(mem, slot, static_cast<std::uint16_t>(0xE000 + (slot - 0xC00400)));
    fill(mem, kNeoWorkRam, kRamShown, at(state, "fill"));
    m68k::Bus bus(mem);

    const auto lspc = std::make_shared<Lspc>();
    const auto lspc_read = [lspc](std::uint32_t a, unsigned) -> std::uint32_t {
        switch (a & 0xE) {
            case 0: return lspc->address;
            case 2: return lspc->address < lspc->vram.size() ? lspc->vram[lspc->address] : 0;
            case 4: return lspc->modulo;
            case 6: return ((lspc->reads++ & 0x1FF) << 7) | ((lspc->reads >> 9) & 7);   // the raster line, then the frame
            default: return 0;
        }
    };
    const auto lspc_write = [lspc](std::uint32_t a, unsigned size, std::uint32_t v) {
        if (size != 2) return;   // the ports are words
        switch (a & 0xE) {
            case 0: lspc->address = static_cast<std::uint16_t>(v); break;
            case 2:
                if (lspc->address < lspc->vram.size()) lspc->vram[lspc->address] = static_cast<std::uint16_t>(v);
                lspc->address = static_cast<std::uint16_t>((lspc->address & 0x8000) | ((lspc->address + lspc->modulo) & 0x7FFF));
                break;
            case 4: lspc->modulo = static_cast<std::uint16_t>(v); break;
            default: break;
        }
    };
    const auto idle = [](std::uint32_t, unsigned) -> std::uint32_t { return 0xFFFFFFFF; };   // no input, no coin, no card
    bus.map(kCallPorts, kCallPortsEnd);
    bus.map(formats::neogeo::kBankSelect, formats::neogeo::kBankSelect + 0xF);
    bus.map(0x3C0000, 0x3C00FF, lspc_read, lspc_write);
    bus.map(0x300000, 0x3BFFFF, idle);
    bus.map(0x800000, 0x800FFF, idle);

    const m68k::RunResult run = m68k::run(bus, entry, options_of(state, 0x10F300, false));
    Bytes vram;
    for (const std::uint16_t w : lspc->vram) {
        vram.push_back(static_cast<std::uint8_t>(w >> 8));
        vram.push_back(static_cast<std::uint8_t>(w));
    }
    return outcome(run, {bytes_of(ByteView(bus.memory().data() + kNeoWorkRam, kRamShown)), bytes_of(ByteView(bus.memory().data() + kNeoPalette, 0x2000)),
                         vl::Effect(std::move(vram))});
}

vl::Written neogeo_written_by(const stdfs::path& module, const stdfs::path& out) { return written_of(source::neogeo_writer::reconstruct(module, out)); }

// ---- the Sega CD -----------------------------------------------------------------------------------------------------

constexpr std::uint32_t kSubFree = 0x10000;       // the sub CPU's PRG-RAM past the program, where fills and pointers go
constexpr std::uint32_t kMainWordRam = 0x200000;
constexpr std::uint32_t kSubWordRam = 0x080000;
constexpr std::uint32_t kMainFree = 0xFF8000;

vl::Effects segacd_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::segacd::Disc disc = formats::segacd::read(path);
    const bool sub = formats::segacd::Disc::is_sub(entry);
    Bytes mem = sub ? disc.sub_memory() : disc.main_memory();
    const std::int64_t seed = at(state, "fill");
    const auto flag = std::make_shared<unsigned>(0);   // the other CPU's flag byte: alternates on each read, so a handshake loop ends
    const auto gate_read = [flag](std::uint32_t a, unsigned) -> std::uint32_t {
        const std::uint32_t reg = a & 0x1F;
        return reg == 0x0E || reg == 0x0F ? (*flag)++ & 1 : 0;
    };

    if (sub) {
        put_rts(mem, 0, formats::segacd::kSubBase);
        for (std::uint32_t slot = 0x5F0A; slot <= 0x5F3A; slot += 6) put_call_stub(mem, slot, static_cast<std::uint16_t>(0xE000 + (slot - 0x5F00)));
        fill(mem, kSubFree, kRamShown, seed);
        fill(mem, kSubWordRam, 0x4000, seed);
    } else {
        put_rts(mem, 0, 0x20000);
        fill(mem, kMainFree, 0x7E00, seed);
        fill(mem, kMainWordRam, 0x4000, seed);
    }
    m68k::Bus bus(mem);
    if (sub) {
        bus.map(kCallPorts, kCallPortsEnd);
        bus.map(0xFF0000, 0xFF3FFF);                  // the PCM
        bus.map(0xFF8000, 0xFF81FF, gate_read);
    } else {
        const auto hardware = [](std::uint32_t a, unsigned) -> std::uint32_t { return (a & ~1u) == 0xC00004 ? 0x0008 : 0; };   // (VDP status: in vblank)
        bus.map(0xA12000, 0xA1203F, gate_read);
        bus.map(0xA00000, 0xA1FFFF, hardware);
        bus.map(0xC00000, 0xC0001F, hardware);
    }
    const m68k::RunResult run = m68k::run(bus, entry, options_of(state, sub ? 0x1F000 : 0xFFFF00, sub));
    const std::uint32_t ram = sub ? kSubFree : kMainFree;   // (the free RAM: the program's own bytes are not part of what a run did)
    const std::uint32_t word = sub ? kSubWordRam : kMainWordRam;
    return outcome(run, {bytes_of(ByteView(bus.memory().data() + ram, sub ? 0x10000 : 0x8000)), bytes_of(ByteView(bus.memory().data() + word, 0x10000))});
}

vl::Written segacd_written_by(const stdfs::path& module, const stdfs::path& out) { return written_of(source::segacd_writer::reconstruct(module, out)); }


}  // namespace m68k_lane_detail

vl::ImageLaneSpec neogeo_lane_spec() {
    namespace d = m68k_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "neogeo";
    spec.rev = 1;
    spec.states = {d::m68k_state({0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, 0),
                   d::m68k_state({1, 2, 3, 4, 5, 6, 7, 8}, {0x100100, 0x100200, 0x100300, 0x100400, 0x100500, 0x100600, 0x100700}, 1),
                   d::m68k_state({0x8000, 0xFFFF, 0x7F, 0x80000000, 0x10, 0x20, 0x30, 0x40}, {0x108000, 0x109000, 0x10A000, 0x10B000, 0x10C000, 0x10D000, 0x10E000}, 2)};
    spec.effects = d::neogeo_effects;
    spec.what = {"end", "D registers", "A registers", "condition codes", "work RAM", "palette RAM", "video RAM", "hardware and BIOS accesses"};
    spec.start = [](const vl::State& s) { return std::format("from D0={:08X} A0={:06X}", d::at(s, "d0"), d::at(s, "a0")); };
    spec.writer = d::neogeo_written_by;
    spec.key_digits = 6;
    return spec;
}

vl::ImageLaneSpec segacd_lane_spec() {
    namespace d = m68k_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "segacd";
    spec.rev = 1;
    spec.states = {d::m68k_state({0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, 0),
                   d::m68k_state({1, 2, 3, 4, 5, 6, 7, 8}, {0xFF8100, 0xFF8200, 0xFF8300, 0xFF8400, 0xFF8500, 0xFF8600, 0xFF8700}, 1),
                   d::m68k_state({0x8000, 0xFFFF, 0x7F, 0x80000000, 0x10, 0x20, 0x30, 0x40}, {0xFFA000, 0xFFB000, 0xFFC000, 0xFFD000, 0xFFE000, 0xFFF000, 0xFFF800}, 2)};
    spec.effects = d::segacd_effects;
    spec.what = {"end", "D registers", "A registers", "condition codes", "RAM", "word RAM", "hardware and BIOS accesses"};
    spec.start = [](const vl::State& s) { return std::format("from D0={:08X} A0={:06X}", d::at(s, "d0"), d::at(s, "a0")); };
    spec.writer = d::segacd_written_by;
    spec.key_digits = 6;
    return spec;
}

void register_m68k_lanes() {
    register_lane({
        .name = "neogeo",
        .platform = platforms::neogeo,
        .probe = [](const stdfs::path& p) { return formats::neogeo::probe(p); },
        .make_spec = neogeo_lane_spec,
        .detectors = {{.priority = 350, .detect = formats::detect::info_neogeo}},
    });
    register_lane({
        .name = "segacd",
        .platform = platforms::segacd,
        .probe = [](const stdfs::path& p) { return formats::segacd::probe(p); },
        .make_spec = segacd_lane_spec,
        .detectors = {{.priority = 370, .detect = formats::detect::info_segacd}},
    });
}

}  // namespace cb::engine
