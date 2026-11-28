// cb-unity: alone (file-local names that other files in a batch also use)
#include "engine/machine_dreamcast.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "core/error.hpp"
#include "emulate/sh2.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "formats/read.hpp"
#include "platforms/package.hpp"
#include "formats/dreamcast.hpp"
#include "source/console_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_dreamcast(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (dreamcast::is_disc_start(head) || name_ends_with(name, {".gdi", ".cdi"})) {   // a Dreamcast disc: "SEGA SEGAKATANA " starts the boot track's first sector (a .gdi names its tracks; a .cdi is searched)
        try {
            const dreamcast::Disc disc = name_ends_with(name, {".gdi"}) ? dreamcast::read(std::filesystem::path(std::string(name))) : dreamcast::parse(file);
            const char* container = disc.container == dreamcast::Container::Gdi ? "GD-ROM image" : disc.container == dreamcast::Container::Cdi ? "DiscJuggler image" : disc.container == dreamcast::Container::Raw ? "raw sectors" : "2048-byte sectors";
            return one("Sega Dreamcast disc", std::format("\"{}\", {}, {}, IP.BIN {} bytes, first program {}{} at 0x{:08X}", disc.header.title, disc.header.product, container, disc.ip_length(),
                                                        disc.first_read_name.empty() ? std::string("(none)") : disc.first_read_name, disc.scrambled ? " (scrambled)" : "", dreamcast::kFirstReadBase + dreamcast::kCachedView),
                       "dreamcast", "reach");
        } catch (const Error&) {   // not a disc of this machine
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
namespace fm = formats::dreamcast;

namespace dc_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kShown = 4000;      // lines kept of what a run did to memory and to the devices

constexpr std::uint32_t kWork = 0x0C800000;         // the window of main RAM the comparison fills with a pattern
constexpr std::size_t kWorkBytes = 0x10000;
constexpr std::uint32_t kStackWindow = 0x0CFF0000;
constexpr std::uint32_t kStack = 0x8CFFFF00;        // (cached view: how a program names it)
constexpr std::uint32_t kVram = 0x04000000;
constexpr std::size_t kVramSize = 0x800000;
constexpr std::uint32_t kAicaRam = 0x00800000;
constexpr std::size_t kAicaRamSize = 0x200000;
constexpr std::uint32_t kBiosSize = 0x200000;
constexpr std::uint32_t kFlash = 0x00200000;
constexpr std::size_t kFlashSize = 0x20000;
constexpr std::size_t kWatchedLow = 0x1000;         // main RAM from its start (the system-call vectors, the BIOS's variables)
constexpr std::size_t kWatchedVideo = 0x10000;      // and the first part of video and sound memory

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State state_of(std::uint32_t a0, std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t s0, int fill, std::uint32_t f0, std::uint32_t f1,
                   std::uint32_t fpscr) {
    return {{"a0", a0}, {"a1", a1}, {"a2", a2}, {"a3", a3}, {"s0", s0}, {"fill", fill}, {"f0", f0}, {"f1", f1}, {"fpscr", fpscr}};
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

// A disc read once: the rebuilt disc and the original are each asked for by every function and every state, and a .cdi or .gdi is not cheap to read.
std::shared_ptr<const fm::Disc> disc_of(const stdfs::path& path) {
    static std::mutex mutex;
    static std::map<std::string, std::pair<std::pair<std::uintmax_t, std::int64_t>, std::shared_ptr<const fm::Disc>>> cache;
    std::error_code ec;
    const std::pair<std::uintmax_t, std::int64_t> stamp = {stdfs::file_size(path, ec), static_cast<std::int64_t>(stdfs::last_write_time(path, ec).time_since_epoch().count())};
    const std::string key = path.generic_string();
    {
        const std::lock_guard lock(mutex);
        const auto it = cache.find(key);
        if (it != cache.end() && it->second.first == stamp) return it->second.second;
    }
    auto disc = std::make_shared<const fm::Disc>(fm::read(path));
    const std::lock_guard lock(mutex);
    if (cache.size() > 8) cache.clear();
    cache[key] = {stamp, disc};
    return disc;
}

// The BIOS ROM as stubs: every longword of it is `rts` and `mov #0,r0` (little-endian), so a call into it returns zero.
void bios_stubs(sh2::Bus& bus) {
    bus.add_ram(0, kBiosSize);
    Bytes stubs(kBiosSize);
    for (std::size_t i = 0; i + 3 < kBiosSize; i += 4) {
        stubs[i] = 0x0B;
        stubs[i + 1] = 0x00;   // rts
        stubs[i + 2] = 0x00;
        stubs[i + 3] = 0xE0;   // mov #0,r0 (its delay slot)
    }
    bus.load(0, stubs);
}

// The system-call vectors the BIOS leaves in RAM: each points at a stub of its own.
struct Syscall {
    std::uint32_t vector;
    std::uint32_t stub;
};
constexpr std::array<Syscall, 5> kSyscalls = {{{0x0C0000B0, 0x100}, {0x0C0000B4, 0x110}, {0x0C0000B8, 0x120}, {0x0C0000BC, 0x130}, {0x0C0000E0, 0x140}}};

void install_syscalls(sh2::Bus& bus) {
    for (const Syscall& s : kSyscalls) {
        const std::uint32_t pointer = 0x80000000u | s.stub;
        const std::array<std::uint8_t, 4> le = {static_cast<std::uint8_t>(pointer), static_cast<std::uint8_t>(pointer >> 8), static_cast<std::uint8_t>(pointer >> 16),
                                               static_cast<std::uint8_t>(pointer >> 24)};
        bus.load(s.vector, ByteView(le.data(), le.size()));
    }
    // a call is logged where it enters: the stub's physical address and the selectors the BIOS reads (r6 the function group, r7 the command)
    bus.on_enter(0, kBiosSize - 1, [&bus](std::uint32_t pc, const std::array<std::uint32_t, 16>& regs) {
        bus.note(sh2::BusAccess::Kind::Call, pc & sh2::kPhysicalMask, regs[6] << 16 | (regs[7] & 0xFFFF));
    });
}

void fill_region(sh2::Bus& bus, std::uint32_t base, std::size_t length, std::int64_t seed) {
    if (seed == 0) return;
    Bytes work(length);
    for (std::size_t i = 0; i < length; ++i) work[i] = fill_byte(i, seed);
    bus.load(base, work);
}

// A stretch of memory the comparison watches, and what it held before the run.
struct Watched {
    std::uint32_t base = 0;
    ByteSpan memory;
    Bytes before;
};

Watched watch_window(sh2::Bus& bus, std::uint32_t base, std::size_t length) {
    const ByteSpan region = bus.region(base);
    const std::size_t offset = base - bus.region_base(base);
    const ByteSpan window = region.subspan(offset, std::min(length, region.size() - offset));
    return {base, window, Bytes(window.begin(), window.end())};
}

vl::Effects outcome(const sh2::RunResult& run, const std::vector<Watched>& watched) {
    Bytes results, saved, status;
    put_word(results, run.r[0]);
    put_word(results, run.r[1]);
    put_word(results, run.fr[0]);
    for (std::size_t i = 8; i < 16; ++i) put_word(saved, run.r[i]);
    for (std::size_t i = 12; i < 16; ++i) put_word(saved, run.fr[i]);
    put_word(status, run.pr);
    put_word(status, run.gbr);
    put_word(status, run.mach);
    put_word(status, run.macl);
    put_word(status, run.sr & sh2::sr::kMaskSh4);
    put_word(status, run.fpscr);
    put_word(status, run.fpul);
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

vl::Effects dreamcast_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const fm::Disc> disc = disc_of(path);
    sh2::Bus bus(sh2::Variant::Sh4);
    bios_stubs(bus);
    bus.add_ram(kFlash, kFlashSize);
    bus.add_ram(fm::kRamBase, fm::kRamSize);
    bus.load(fm::kIpBase, disc->memory());
    bus.add_ram(kVram, kVramSize);
    bus.add_ram(kAicaRam, kAicaRamSize);
    install_syscalls(bus);
    fill_region(bus, kWork, kWorkBytes, at(state, "fill"));

    // the devices: logged; the status registers answer as a machine in the middle of a frame does, so a wait on them ends
    const auto frame = std::make_shared<unsigned>(0);
    bus.map(0x005F6800, 0x005F9FFF, [frame](std::uint32_t a, unsigned) -> std::uint32_t {
        switch (a & ~3u) {
            case 0x005F8000: return 0x17FD11DBu;                                                          // PVR_ID
            case 0x005F8004: return 0x11u;                                                                // PVR_REVISION
            case 0x005F810C: {                                                                            // PVR_SPG_STATUS: the scan line, and the blanking coming and going
                const unsigned n = (*frame)++;
                return (n & 0x3FFu) | ((n >> 3) & 1u ? 0x2800u : 0u);
            }
            case 0x005F6900: return ((*frame)++ & 1u) ? 0x10u : 0x08u;                                    // SB_ISTNRM: the V-blank in, then out
            case 0x005F7018: case 0x005F709C: return 0x50u;                                               // the GD-ROM: ready (DRDY, DSC)
            default: return 0u;
        }
    });
    bus.map(0x00700000, 0x007100FF);                  // the AICA's channels and common registers, and the clock
    bus.map(0x00600000, 0x0060FFFF);                  // the modem
    bus.map(0x01000000, 0x01FFFFFF);                  // the G2 external area
    bus.map(0x05000000, 0x057FFFFF);                  // the video memory's 32-bit view
    bus.map(0x10000000, 0x13FFFFFF);                  // the tile accelerator's FIFOs

    std::vector<Watched> watched;
    watched.push_back(watch_window(bus, fm::kRamBase, kWatchedLow));
    watched.push_back(watch_window(bus, fm::kIpBase, fm::kSystemBytes + disc->program.size()));
    watched.push_back(watch_window(bus, kWork, kWorkBytes));
    watched.push_back(watch_window(bus, kStackWindow, 0x10000));
    watched.push_back(watch_window(bus, kVram, kWatchedVideo));
    watched.push_back(watch_window(bus, kAicaRam, kWatchedVideo));

    sh2::RunOptions options;
    options.max_steps = kSteps;
    options.sp = kStack;
    for (std::size_t i = 0; i < 4; ++i) options.a[i] = static_cast<std::uint32_t>(at(state, std::array{"a0", "a1", "a2", "a3"}[i]));
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    options.fpscr = static_cast<std::uint32_t>(at(state, "fpscr"));
    options.fa[0] = static_cast<std::uint32_t>(at(state, "f0"));
    options.fa[1] = static_cast<std::uint32_t>(at(state, "f1"));
    const sh2::RunResult run = emulate::pcode_cpu::call(bus, entry, options);
    return outcome(run, watched);
}

vl::Written dreamcast_written_by(const stdfs::path& module, const stdfs::path& out) { return written_of(source::console_writer::reconstruct("dreamcast", module, out)); }

}  // namespace dc_lane_detail

namespace d = dc_lane_detail;

vl::ImageLaneSpec dreamcast_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "dreamcast";
    spec.rev = 1;
    spec.states = {d::state_of(0, 0, 0, 0, 0, 0, 0, 0, 0), d::state_of(d::kWork, d::kWork + 0x1000, 5, 0x10, 0, 1, 0x3FC00000, 0x40490FDB, 0),
                   d::state_of(d::kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2, 0xC0100000, 0x42C80000, 0),
                   d::state_of(2, 3, d::kWork + 0x100, d::kWork + 0x200, 7, 3, 0x3F800000, 0, 0x00100000)};
    spec.effects = d::dreamcast_effects;
    spec.what = {"end", "results R0 R1 FR0", "R8-R15 FR12-FR15", "PR GBR MACH MACL SR FPSCR FPUL", "memory changes", "hardware accesses"};
    spec.start = [](const vl::State& s) {
        return std::format("from R4={:08X} R5={:08X} R6={:X} FR4={:08X}", d::at(s, "a0"), d::at(s, "a1"), d::at(s, "a2"), d::at(s, "f0"));
    };
    spec.writer = d::dreamcast_written_by;
    spec.key_digits = 8;
    return spec;
}

void register_dreamcast_lane() {
    register_lane({
        .name = "dreamcast",
        .platform = [] { return platforms::described("dreamcast"); },
        .probe = [](const stdfs::path& p) { return formats::dreamcast::probe(p); },
        .make_spec = dreamcast_lane_spec,
        .detectors = {{.priority = 380, .detect = formats::detect::info_dreamcast}},
    });
}

}  // namespace cb::engine
