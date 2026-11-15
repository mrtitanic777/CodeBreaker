// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/engine/machine_ps2.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "cb/core/error.hpp"
#include "cb/emulate/ee.hpp"
#include "cb/engine/lane_registry.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/read.hpp"
#include "cb/platforms/ps2.hpp"
#include "cb/formats/ps2.hpp"
#include "cb/source/ps2_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_ps2(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 52 && head.size() >= 52 && head[0] == 0x7F && head[1] == 'E' && head[2] == 'L' && head[3] == 'F' && head[4] == 1 && head[5] == 1 && (head[18] | head[19] << 8) == 8) {   // a little-endian MIPS ELF: the flags say which console
        const std::uint32_t flags = get<std::uint32_t>(file, 36, "detect");
        const std::uint32_t entry = get<std::uint32_t>(file, 24, "detect");
        if ((flags & 0x00FF0000u) == 0x00920000u) return one("PlayStation 2 executable (ELF)", std::format("Emotion Engine R5900, entry 0x{:08X}, {} bytes", entry, size), "ps2", "reach");
        if ((flags & 0x00FF0000u) == 0x00A20000u)
            return one((head[16] | head[17] << 8) == 0xFFA0 ? "PSP module (PRX)" : "PSP executable (ELF)", std::format("Allegrex, entry 0x{:08X}, {} bytes", entry, size), "psp", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace ps2_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kShown = 4000;
constexpr std::size_t kCacheSize = 6;
constexpr std::uint32_t kWork = 0x01800000;   // RAM a run may be handed pointers into, above any program
constexpr std::size_t kWorkBytes = 0x2000;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State make_state(std::uint32_t a0, std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t s0, int fill) {
    return {{"a0", a0}, {"a1", a1}, {"a2", a2}, {"a3", a3}, {"s0", s0}, {"fill", fill}};
}

// A program as the interpreter runs it: its segments as shared regions, built once.
struct Loaded {
    formats::ps2::Program program;
    emulate::paged_memory::Regions regions;
};

std::shared_ptr<const Loaded> load(const stdfs::path& path) {
    static std::mutex lock;
    static std::map<std::string, std::shared_ptr<const Loaded>> cache;
    std::error_code ec;
    const auto size = stdfs::file_size(path, ec);
    const auto time = stdfs::last_write_time(path, ec).time_since_epoch().count();
    const std::string key = std::format("{}|{}|{}", path.generic_string(), static_cast<std::uint64_t>(size), static_cast<std::int64_t>(time));
    {
        const std::lock_guard guard(lock);
        if (const auto it = cache.find(key); it != cache.end()) return it->second;
    }
    std::optional<formats::ps2::Program> program = formats::ps2::read(path);
    if (!program) throw FormatError("ps2", 0, "not a PlayStation 2 program");
    auto loaded = std::make_shared<Loaded>();
    auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
    for (const formats::mips_elf::ProgramHeader& p : program->elf.program_headers) {
        if (p.type != 1 || p.filesz == 0 || static_cast<std::uint64_t>(p.offset) + p.filesz > program->elf.data.size()) continue;
        const std::uint32_t va = p.vaddr >= 0x80000000u && p.vaddr < 0xC0000000u ? p.vaddr & 0x1FFFFFFFu : p.vaddr;
        regions->push_back({va, Bytes(program->elf.data.begin() + p.offset, program->elf.data.begin() + p.offset + p.filesz)});
    }
    loaded->program = std::move(*program);
    loaded->regions = regions;
    const std::lock_guard guard(lock);
    if (cache.size() >= kCacheSize) cache.erase(cache.begin());
    cache[key] = loaded;
    return loaded;
}

vl::Effects effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const Loaded> loaded = load(path);
    emulate::ee::Bus bus(loaded->regions);
    if (const std::int64_t seed = at(state, "fill"); seed != 0)
        for (std::size_t i = 0; i < kWorkBytes; ++i) bus.memory().write(kWork + static_cast<std::uint32_t>(i), 1, fill_byte(i, seed));
    const auto before = bus.memory().changes(1 << 20);
    emulate::ee::RunOptions options;
    options.max_steps = kSteps;
    for (std::size_t i = 0; i < 4; ++i) options.a[i] = static_cast<std::uint32_t>(at(state, i == 0 ? "a0" : i == 1 ? "a1" : i == 2 ? "a2" : "a3"));
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    const emulate::ee::RunResult run = emulate::ee::run(bus, entry, options);
    std::map<std::uint32_t, std::uint8_t> was(before.begin(), before.end());
    std::vector<std::string> changed;
    for (const auto& [address, value] : bus.memory().changes(1 << 20)) {
        if (changed.size() >= kShown) break;
        const auto it = was.find(address);
        if (it == was.end() || it->second != value) changed.push_back(std::format("{:08x}={:02x}", address, value));
    }
    Bytes saved;
    for (std::size_t i = 16; i < 24; ++i)
        for (int shift = 56; shift >= 0; shift -= 8) saved.push_back(static_cast<std::uint8_t>(run.r[i] >> shift));
    Bytes upper;
    for (std::size_t i : {2u, 3u})
        for (int shift = 56; shift >= 0; shift -= 8) upper.push_back(static_cast<std::uint8_t>(run.rh[i] >> shift));
    std::vector<std::string> io;
    for (const emulate::ee::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kShown) break;
    }
    std::vector<std::string> calls;
    for (const emulate::ee::Syscall& c : run.syscalls) calls.push_back(c.text());
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.r[2]), number(run.r[3]), vl::Effect(std::move(upper)), vl::Effect(std::move(saved)), number(run.r[29]), number(run.hi[0]),
            number(run.lo[0]), number(run.f0), vl::Effect(std::move(changed)), vl::Effect(std::move(io)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::ps2_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace ps2_lane_detail

namespace d = ps2_lane_detail;

vl::ImageLaneSpec ps2_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "ps2";
    spec.rev = 1;
    spec.states = {d::make_state(0, 0, 0, 0, 0, 0), d::make_state(d::kWork, d::kWork + 0x1000, 5, 0x10, 0, 1),
                   d::make_state(d::kWork + 0x800, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), d::make_state(2, 3, d::kWork + 0x100, d::kWork + 0x200, 7, 3)};
    spec.effects = d::effects;
    spec.what = {"end", "V0", "V1", "V0-V1 upper halves", "S0-S7", "SP", "HI", "LO", "F0", "memory changes", "hardware accesses", "system calls"};
    spec.start = [](const vl::State& s) { return std::format("from A0={:08X} A1={:08X} A2={:X}", d::at(s, "a0"), d::at(s, "a1"), d::at(s, "a2")); };
    spec.writer = d::written_by;
    spec.key_digits = 8;
    return spec;
}

void register_ps2_lane() {
    register_lane({
        .name = "ps2",
        .platform = platforms::ps2,
        .probe = [](const stdfs::path& p) { return formats::ps2::probe(p); },
        .make_spec = ps2_lane_spec,
        .detectors = {{.priority = 20, .detect = formats::detect::info_ps2}},
    });
}

}  // namespace cb::engine
