// cb-unity: alone (file-local names that other files in a batch also use)
#include "engine/machine_sh2.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/m68k.hpp"
#include "emulate/sh2.hpp"
#include "engine/lane_registry.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/read.hpp"
#include "platforms/package.hpp"
#include "formats/genesis.hpp"
#include "formats/saturn.hpp"
#include "formats/sega32x.hpp"
#include "source/console_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_sega32x(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    {   // a Sega 32X cartridge: a Genesis ROM whose header at $3C0 names the SH-2 programs
        const bool dot_32x = name.size() >= 4 && (name.substr(name.size() - 4) == ".32x" || name.substr(name.size() - 4) == ".32X");
        if (sega32x::is_rom_start(head, dot_32x)) {
            try {
                const sega32x::Rom rom = sega32x::parse(file);
                return one("Sega 32X cartridge", std::format("\"{}\", {} KB, master SH-2 at 0x{:08X}, slave at 0x{:08X}", rom.header.domestic_name, size / 1024,
                                                           rom.header.master_start, rom.header.slave_start),
                           "sega32x", "reach");
            } catch (const Error&) {
            }
        }
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_saturn(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (saturn::is_disc_start(head)) {   // a Saturn disc: "SEGA SEGASATURN " starts its first sector, which holds IP.BIN's header
        try {
            const saturn::Disc disc = saturn::parse(file);
            return one("Sega Saturn disc", std::format("\"{}\", {}, IP {} bytes, first program {} at 0x{:08X}", disc.header.title, disc.header.product, disc.ip_length(),
                                                     disc.first_read_name.empty() ? std::string("(none)") : disc.first_read_name, disc.load_address()),
                       "saturn", "reach");
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
namespace sh2 = emulate::sh2;

namespace sh2_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kShown = 4000;      // lines kept of what a run did to memory and to the devices

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State state_of(std::uint32_t a0, std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t s0, int fill) {
    return {{"a0", a0}, {"a1", a1}, {"a2", a2}, {"a3", a3}, {"s0", s0}, {"fill", fill}};
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

void put_word(Bytes& out, std::uint32_t v) {
    for (int shift = 24; shift >= 0; shift -= 8) out.push_back(static_cast<std::uint8_t>(v >> shift));
}

// The BIOS and boot ROM as stubs: every word of the region is `rts` or `nop`, so a call into it returns.
void stub_region(sh2::Bus& bus, std::uint32_t base, std::size_t size) {
    bus.add_ram(base, size);
    Bytes stubs(size);
    for (std::size_t i = 0; i + 1 < size; i += 4) {
        stubs[i] = 0x00;
        stubs[i + 1] = 0x0B;   // rts
        if (i + 3 < size) {
            stubs[i + 2] = 0x00;
            stubs[i + 3] = 0x09;   // nop (its delay slot)
        }
    }
    bus.load(base, stubs);
}

// A memory the comparison watches: where it is, and what it held before the run.
struct Watched {
    std::uint32_t base = 0;
    ByteSpan memory;
    Bytes before;
};

std::vector<Watched> watch(sh2::Bus& bus, std::initializer_list<std::uint32_t> bases) {
    std::vector<Watched> out;
    for (const std::uint32_t b : bases) {
        const ByteSpan m = bus.region(b);
        out.push_back({b, m, Bytes(m.begin(), m.end())});
    }
    return out;
}

// A run's outcome, the same six places for the 68000 side and the SH-2 side: how it ended, the results, the registers a caller keeps, the
// status, what the run changed in memory, and the device accesses in order.
vl::Effects outcome(const sh2::RunResult& run, const std::vector<Watched>& watched) {
    Bytes results, saved, status;
    put_word(results, run.r[0]);
    put_word(results, run.r[1]);
    for (std::size_t i = 8; i < 16; ++i) put_word(saved, run.r[i]);
    put_word(status, run.pr);
    put_word(status, run.gbr);
    put_word(status, run.mach);
    put_word(status, run.macl);
    put_word(status, run.sr & sh2::sr::kMask);
    std::vector<std::string> changed;
    for (const Watched& w : watched)
        for (std::size_t i = 0; i < w.before.size() && changed.size() < kShown; ++i)
            if (w.before[i] != w.memory[i]) changed.push_back(std::format("{:08x}={:02x}", w.base + i, w.memory[i]));
    std::vector<std::string> io;
    for (const sh2::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kShown) break;
    }
    return {vl::Effect(run.exit()), vl::Effect(std::move(results)), vl::Effect(std::move(saved)), vl::Effect(std::move(status)),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io))};
}

sh2::RunOptions options_of(const vl::State& state, std::uint32_t sp) {
    sh2::RunOptions options;
    options.max_steps = kSteps;
    options.sp = sp;
    for (std::size_t i = 0; i < 4; ++i) options.a[i] = static_cast<std::uint32_t>(at(state, std::array{"a0", "a1", "a2", "a3"}[i]));
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    return options;
}

void fill_region(sh2::Bus& bus, std::uint32_t base, std::size_t length, std::int64_t seed) {
    if (seed == 0) return;
    Bytes work(length);
    for (std::size_t i = 0; i < length; ++i) work[i] = fill_byte(i, seed);
    bus.load(base, work);
}

// ---- the 32X ----------------------------------------------------------------------------------------------------------

constexpr std::uint32_t k32Work = 0x06030000;
constexpr std::size_t k32WorkBytes = 0x8000;
constexpr std::uint32_t k32Stack = 0x0603FF00;
constexpr std::uint32_t kGenesisRam = 0xFF0000;

vl::Effects effects_68k(const formats::sega32x::Rom& rom, std::uint32_t entry, const vl::State& state) {
    Bytes mem = formats::genesis::parse(rom.data, ".32x").memory();
    const std::int64_t seed = at(state, "fill");
    if (seed != 0)
        for (std::size_t i = 0; i < 0x10000; ++i) mem[kGenesisRam + i] = fill_byte(i, seed);
    emulate::m68k::Bus bus(mem);
    const auto hardware = [](std::uint32_t a, unsigned) -> std::uint32_t { return (a & ~1u) == 0xC00004 ? 0x0008 : 0; };   // (VDP status: in vblank)
    bus.map(0xA00000, 0xA1FFFF, hardware);
    bus.map(0xC00000, 0xC0001F, hardware);
    emulate::m68k::RunOptions options;
    options.max_steps = kSteps;
    std::array<std::uint32_t, 8> d68{};
    std::array<std::uint32_t, 8> a68{};
    d68[0] = static_cast<std::uint32_t>(at(state, "s0"));
    d68[1] = static_cast<std::uint32_t>(at(state, "a1"));
    d68[2] = static_cast<std::uint32_t>(at(state, "a2"));
    d68[3] = static_cast<std::uint32_t>(at(state, "a3"));
    a68[0] = static_cast<std::uint32_t>(at(state, "a0"));
    options.d = d68;
    options.a = a68;
    const emulate::m68k::RunResult run = emulate::pcode_cpu::call(bus, entry, options);
    Bytes results, saved, status;
    put_word(results, run.d[0]);
    put_word(results, run.d[1]);
    for (std::size_t i = 2; i < 8; ++i) put_word(saved, run.d[i]);
    for (std::size_t i = 0; i < 7; ++i) put_word(saved, run.a[i]);
    put_word(status, run.sr & 0x1F);
    std::vector<std::string> changed;
    const ByteView after = ByteView(bus.memory().data() + kGenesisRam, 0x10000);
    for (std::size_t i = 0; i < after.size() && changed.size() < kShown; ++i) {
        const std::uint8_t was = seed != 0 ? fill_byte(i, seed) : 0;
        if (after[i] != was) changed.push_back(std::format("{:08x}={:02x}", kGenesisRam + i, after[i]));
    }
    std::vector<std::string> io;
    for (const emulate::m68k::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kShown) break;
    }
    return {vl::Effect(run.exit()), vl::Effect(std::move(results)), vl::Effect(std::move(saved)), vl::Effect(std::move(status)),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io))};
}

vl::Effects sega32x_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::sega32x::Rom rom = formats::sega32x::read(path);
    if (entry < 0x01000000u) return effects_68k(rom, entry, state);

    sh2::Bus bus;
    stub_region(bus, 0, 0x4000);                                              // the boot ROM
    bus.add_ram(0x4200, 0x200);                                               // the palette
    const std::size_t rom_window = std::min<std::size_t>(rom.data.size(), formats::sega32x::kCartWindowSize);
    bus.add_ram(formats::sega32x::kCartWindow, formats::sega32x::kCartWindowSize);
    bus.load(formats::sega32x::kCartWindow, ByteView(rom.data).subspan(0, rom_window));
    bus.add_ram(formats::sega32x::kFrameBuffer, formats::sega32x::kFrameBufferSize);
    bus.add_ram(formats::sega32x::kSdram, formats::sega32x::kSdramSize);
    bus.load(formats::sega32x::kSdram, rom.sh2_program());
    fill_region(bus, k32Work, k32WorkBytes, at(state, "fill"));

    // the system registers: the communication ports hold what is written, the rest read zero; the VDP's frame buffer control shows the vertical
    // blank and flips the frame swap bit on each read
    const auto comm = std::make_shared<std::array<std::uint16_t, 8>>();
    bus.map(0x4000, 0x40FF,
            [comm](std::uint32_t a, unsigned size) -> std::uint32_t {
                if (a >= 0x4020 && a < 0x4030) return (*comm)[(a - 0x4020) / 2] >> (size == 1 && (a & 1) == 0 ? 8 : 0);
                return 0;
            },
            [comm](std::uint32_t a, unsigned size, std::uint32_t v) {
                if (a >= 0x4020 && a < 0x4030 && size == 2) (*comm)[(a - 0x4020) / 2] = static_cast<std::uint16_t>(v);
            });
    const auto swaps = std::make_shared<unsigned>(0);
    bus.map(0x4100, 0x41FF, [swaps](std::uint32_t a, unsigned) -> std::uint32_t { return (a & ~1u) == 0x410A ? 0x8000u | ((*swaps)++ & 1u) : 0u; });

    const std::vector<Watched> watched = watch(bus, {formats::sega32x::kSdram, formats::sega32x::kFrameBuffer, 0x4200});
    const sh2::RunResult run = emulate::pcode_cpu::call(bus, entry, options_of(state, k32Stack));
    return outcome(run, watched);
}

vl::Written sega32x_written_by(const stdfs::path& module, const stdfs::path& out) { return written_of(source::console_writer::reconstruct("sega32x", module, out)); }

// ---- the Saturn --------------------------------------------------------------------------------------------------------

constexpr std::uint32_t kSatWork = 0x06080000;
constexpr std::size_t kSatWorkBytes = 0x10000;
constexpr std::uint32_t kSatStack = 0x060FFF00;

vl::Effects saturn_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::saturn::Disc disc = formats::saturn::read(path);
    sh2::Bus bus;
    stub_region(bus, 0, 0x80000);                                  // the BIOS
    bus.add_ram(0x00200000, 0x100000);                             // low work RAM
    bus.add_ram(formats::saturn::kHighRam, formats::saturn::kHighRamSize);
    bus.load(formats::saturn::kHighRam, disc.memory());
    bus.add_ram(0x05A00000, 0x80000);                              // sound RAM
    bus.add_ram(0x05C00000, 0x80000);                              // VDP1 video RAM
    bus.add_ram(0x05C80000, 0x40000);                              // VDP1 frame buffer
    bus.add_ram(0x05E00000, 0x80000);                              // VDP2 video RAM
    bus.add_ram(0x05F00000, 0x1000);                               // VDP2 colour RAM
    fill_region(bus, kSatWork, kSatWorkBytes, at(state, "fill"));

    // the devices: logged; the status registers answer as a console in the middle of a frame does, so a wait on them ends
    const auto frame = std::make_shared<unsigned>(0);
    bus.map(0x00100000, 0x0010007F);                                                                                // SMPC: ready (SF reads 0)
    bus.map(0x01000000, 0x01FFFFFF);                                                                                // MINIT / SINIT
    bus.map(0x02000000, 0x057FFFFF);                                                                                // the A-bus
    bus.map(0x05800000, 0x058FFFFF, [](std::uint32_t, unsigned size) -> std::uint32_t { return size == 2 ? 0xFFFFu : 0xFFu; });   // the CD block: every flag set
    bus.map(0x05B00000, 0x05B00FFF);                                                                                // SCSP registers
    bus.map(0x05D00000, 0x05D0001F, [](std::uint32_t a, unsigned) -> std::uint32_t { return (a & ~1u) == 0x05D00010 ? 3u : 0u; });   // VDP1: drawing done
    bus.map(0x05F80000, 0x05F8011F, [frame](std::uint32_t a, unsigned) -> std::uint32_t {
        if ((a & ~1u) == 0x05F80004) return ((*frame)++ >> 3) & 1u ? 0x0008u : 0u;   // TVSTAT: the vertical blank comes and goes
        if ((a & ~1u) == 0x05F80008 || (a & ~1u) == 0x05F8000A) return (*frame)++ & 0x1FF;   // HCNT, VCNT
        return 0u;
    });
    bus.map(0x05FE0000, 0x05FEFFFF, [frame](std::uint32_t a, unsigned) -> std::uint32_t {
        if ((a & ~3u) == 0x05FE00A4) return ((*frame)++ & 1u) ? 2u : 1u;   // IST: the V-blank in, then out
        return 0u;
    });

    const std::vector<Watched> watched = watch(bus, {0x00200000, formats::saturn::kHighRam, 0x05A00000, 0x05C00000, 0x05C80000, 0x05E00000, 0x05F00000});
    const sh2::RunResult run = emulate::pcode_cpu::call(bus, entry, options_of(state, kSatStack));
    return outcome(run, watched);
}

vl::Written saturn_written_by(const stdfs::path& module, const stdfs::path& out) { return written_of(source::console_writer::reconstruct("saturn", module, out)); }

}  // namespace sh2_lane_detail

namespace d = sh2_lane_detail;

namespace {

const std::vector<std::string>& places() {
    static const std::vector<std::string> p = {"end", "results R0 R1", "R8-R15", "PR GBR MACH MACL SR", "memory changes", "hardware accesses"};
    return p;
}

std::string start_text(const vl::State& s) {
    return std::format("from R4={:08X} R5={:08X} R6={:X}", d::at(s, "a0"), d::at(s, "a1"), d::at(s, "a2"));
}

}  // namespace

vl::ImageLaneSpec sega32x_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "sega32x";
    spec.rev = 1;
    spec.states = {d::state_of(0, 0, 0, 0, 0, 0), d::state_of(d::k32Work, d::k32Work + 0x1000, 5, 0x10, 0, 1),
                   d::state_of(d::k32Work + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), d::state_of(2, 3, d::k32Work + 0x100, d::k32Work + 0x200, 7, 3)};
    spec.effects = d::sega32x_effects;
    spec.what = places();
    spec.start = start_text;
    spec.writer = d::sega32x_written_by;
    spec.key_digits = 8;
    return spec;
}

vl::ImageLaneSpec saturn_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "saturn";
    spec.rev = 1;
    spec.states = {d::state_of(0, 0, 0, 0, 0, 0), d::state_of(d::kSatWork, d::kSatWork + 0x1000, 5, 0x10, 0, 1),
                   d::state_of(d::kSatWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), d::state_of(2, 3, d::kSatWork + 0x100, d::kSatWork + 0x200, 7, 3)};
    spec.effects = d::saturn_effects;
    spec.what = places();
    spec.start = start_text;
    spec.writer = d::saturn_written_by;
    spec.key_digits = 8;
    return spec;
}

void register_sh2_lanes() {
    register_lane({
        .name = "sega32x",
        .platform = [] { return platforms::described("sega32x"); },
        .probe = [](const stdfs::path& p) { return formats::sega32x::probe(p); },
        .make_spec = sega32x_lane_spec,
        .detectors = {{.priority = 400, .detect = formats::detect::info_sega32x}},
    });
    register_lane({
        .name = "saturn",
        .platform = [] { return platforms::described("saturn"); },
        .probe = [](const stdfs::path& p) { return formats::saturn::probe(p); },
        .make_spec = saturn_lane_spec,
        .detectors = {{.priority = 390, .detect = formats::detect::info_saturn}},
    });
}

}  // namespace cb::engine
