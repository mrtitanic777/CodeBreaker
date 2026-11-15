// cb-unity: alone
#include "cb/engine/machine_cdi.hpp"

#include <array>
#include <format>
#include <optional>
#include <memory>

#include "cb/core/error.hpp"
#include "cb/emulate/m68k.hpp"
#include "cb/engine/lane_registry.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/read.hpp"
#include "cb/platforms/cdi.hpp"
#include "cb/formats/cdi.hpp"
#include "cb/source/cdi_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_cdi(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".iso", ".bin", ".cdi", ".img", ".mod", ".rtf"})) {   // a Philips CD-i disc (the Disc Label "CD-I " in an early sector) or a CD-RTOS module
        bool label = false;
        for (std::size_t at = 0; at + 7 <= file.size() && at <= 16 * formats::cdi::kSector; at += formats::cdi::kSector) label = label || (file[at] == 1 && starts_with(file, at + 1, "CD-I ") && file[at + 6] == 1);
        if (label || formats::cdi::module_at(file, 0)) {
            try {
                const formats::cdi::Program program = formats::cdi::parse(file);
                return one(program.disc ? "Philips CD-i disc" : "CD-RTOS module file", std::format("{} CD-RTOS module(s), {} executable", program.modules.size(), program.placed().size()), "cdi", "reach");
            } catch (const FormatError&) {
            }
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

namespace cdi_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kIoShown = 4000;
constexpr std::uint32_t kRamFrom = 0x100000;
constexpr std::uint32_t kLow = 0x20000;      // the RAM compared: [kRamFrom, kRamFrom + kLow) and [kTop, kRamFrom + 0x100000)
constexpr std::uint32_t kTop = 0x1F0000;
constexpr std::uint32_t kStack = 0x1FFF00;
constexpr std::uint32_t kStubs = 0x7000;     // the stub of call number n is at kStubs + 8 n
constexpr std::uint32_t kCallPort = 0x7F00;  // each stub writes its call number here

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const std::string& key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State cdi_state(const std::array<std::uint32_t, 8>& d, const std::array<std::uint32_t, 7>& a, int fill_seed) {
    vl::State s{{"fill", fill_seed}};
    for (std::size_t i = 0; i < 8; ++i) s["d" + std::to_string(i)] = d[i];
    for (std::size_t i = 0; i < 7; ++i) s["a" + std::to_string(i)] = a[i];
    return s;
}

void put_long(Bytes& out, std::uint32_t v) {
    for (int shift = 24; shift >= 0; shift -= 8) out.push_back(static_cast<std::uint8_t>(v >> shift));
}

vl::Effect bytes_of(const Bytes& mem, std::uint32_t from, std::size_t length) { return vl::Effect(Bytes(mem.begin() + from, mem.begin() + from + length)); }

vl::Effects cdi_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::cdi::Program program = formats::cdi::read(path);
    Bytes mem = program.memory();
    // every `TRAP #0` and the call word after it becomes `JSR stub.W` (the same four bytes); the stub logs the call number and returns
    for (const formats::cdi::Module& m : program.modules) {
        if (!m.runs()) continue;
        for (std::uint32_t a = m.address + static_cast<std::uint32_t>(formats::cdi::kHeader); a + 4 <= m.address + m.size; a += 2) {
            if (mem[a] != 0x4E || mem[a + 1] != 0x40) continue;
            const std::uint32_t call = static_cast<std::uint32_t>(mem[a + 2] << 8 | mem[a + 3]);
            if (!source::cdi_writer::system_calls().contains(call)) continue;
            const std::uint32_t stub = kStubs + 8 * call;
            mem[a] = 0x4E;
            mem[a + 1] = 0xB8;
            mem[a + 2] = static_cast<std::uint8_t>(stub >> 8);
            mem[a + 3] = static_cast<std::uint8_t>(stub);
            const std::array<std::uint8_t, 8> code = {0x31, 0xFC, static_cast<std::uint8_t>(call >> 8), static_cast<std::uint8_t>(call), 0x7F, 0x00, 0x4E, 0x75};
            std::copy(code.begin(), code.end(), mem.begin() + stub);
        }
    }
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        for (std::uint32_t a = kRamFrom; a < kRamFrom + kLow; ++a) mem[a] = fill_byte(a - kRamFrom, seed);
    }
    m68k::Bus bus(mem);
    bus.map(kCallPort, kCallPort + 1);
    bus.map(0x200000, 0x4FFFFF, [](std::uint32_t, unsigned) -> std::uint32_t { return 0; });

    m68k::RunOptions options;
    options.max_steps = kSteps;
    options.sp = kStack;
    std::array<std::uint32_t, 8> d{}, a{};
    for (std::size_t i = 0; i < 8; ++i) d[i] = static_cast<std::uint32_t>(at(state, "d" + std::to_string(i)));
    for (std::size_t i = 0; i < 7; ++i) a[i] = static_cast<std::uint32_t>(at(state, "a" + std::to_string(i)));
    options.d = d;
    options.a = a;
    const m68k::RunResult run = m68k::run(bus, entry, options);

    Bytes dregs, aregs;
    for (std::size_t i = 0; i < 8; ++i) put_long(dregs, run.d[i]);
    for (std::size_t i = 0; i < 7; ++i) put_long(aregs, run.a[i]);
    std::vector<std::string> io;
    for (const m68k::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    const Bytes after(bus.memory().begin(), bus.memory().end());
    return {vl::Effect(run.exit()), vl::Effect(std::move(dregs)), vl::Effect(std::move(aregs)), vl::Effect(static_cast<std::int64_t>(run.sr & 0x1F)),
            bytes_of(after, kRamFrom, kLow), bytes_of(after, kTop, 0x10000), vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::cdi_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace cdi_lane_detail

vl::ImageLaneSpec cdi_lane_spec() {
    namespace d = cdi_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "cdi";
    spec.rev = 1;
    spec.states = {d::cdi_state({0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0x108000}, 0),
                   d::cdi_state({1, 2, 3, 4, 5, 6, 7, 8}, {0x100100, 0x100200, 0x100300, 0x100400, 0x100500, 0x100600, 0x108000}, 1),
                   d::cdi_state({0x8000, 0xFFFF, 0x7F, 0x80000000, 0x10, 0x20, 0x30, 0x40}, {0x108000, 0x109000, 0x10A000, 0x10B000, 0x10C000, 0x10D000, 0x108000}, 2)};
    spec.effects = d::cdi_effects;
    spec.what = {"end", "D registers", "A registers", "condition codes", "RAM", "stack RAM", "system calls and device accesses"};
    spec.start = [](const vl::State& s) { return std::format("from D0={:08X} A0={:06X}", d::at(s, "d0"), d::at(s, "a0")); };
    spec.writer = d::written_by;
    spec.key_digits = 6;
    return spec;
}

void register_cdi_lane() {
    register_lane({
        .name = "cdi",
        .platform = platforms::cdi,
        .probe = [](const stdfs::path& p) { return formats::cdi::probe(p); },
        .make_spec = cdi_lane_spec,
        .detectors = {{.priority = 280, .detect = formats::detect::info_cdi}},
    });
}

}  // namespace cb::engine
