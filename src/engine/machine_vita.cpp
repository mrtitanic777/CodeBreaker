// cb-unity: alone
#include "engine/machine_vita.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "core/error.hpp"
#include "emulate/arm.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/pcode_cpu.hpp"
#include "formats/read.hpp"
#include "platforms/vita.hpp"
#include "formats/vita.hpp"
#include "source/vita_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_vita_1(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 0x60 && head.size() >= 0x60 && starts_with(head, 0, std::string_view("SCE\0", 4)) && get<std::uint32_t>(file, 4, "detect") == 3) {   // a Vita SELF: the segments' table says whether they are encrypted
        const std::uint64_t info = get<std::uint32_t>(file, 0x58, "detect");
        const bool encrypted = info + 32 <= size && get<std::uint32_t>(file, static_cast<std::size_t>(info) + 24, "detect") == 1;
        if (encrypted) return one("PlayStation Vita SELF (encrypted)", "not handled: the segments are encrypted", "vita", "gap");
        return one("PlayStation Vita SELF", std::format("ARMv7-A Thumb-2, {} bytes", size), "vita", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_vita_2(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 52 && head.size() >= 52 && head[0] == 0x7F && head[1] == 'E' && head[2] == 'L' && head[3] == 'F' && head[4] == 1 && head[5] == 1 && (head[18] | head[19] << 8) == 40 &&
        (head[16] | head[17] << 8) >= 0xFE00 && (head[16] | head[17] << 8) <= 0xFE04)   // an ARM ELF of a Sony type: VitaSDK's .velf
        return one("PlayStation Vita executable (ELF)", std::format("ARMv7-A Thumb-2, entry field 0x{:08X}, {} bytes", get<std::uint32_t>(file, 24, "detect"), size), "vita", "reach");
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace vita_lane_detail {

constexpr std::uint64_t kSteps = 20000;      // instructions a function may run in a comparison
constexpr std::size_t kShown = 4000;         // changes and accesses kept of a run
constexpr std::size_t kCacheSize = 6;
constexpr std::uint32_t kWork = 0x91000000;  // free memory a run may be handed pointers into, seeded from `fill`
constexpr std::size_t kWorkBytes = 0x10000;
constexpr std::uint32_t kStack = 0x90100000;
constexpr std::uint32_t kBase = 0x81000000;   // where a relocatable program is loaded (a fixed-address one is where its segments say)
constexpr std::uint32_t kReturn = 0x00000100;   // below the image: nothing is mapped there

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

struct Loaded {
    formats::vita::Program program;
    emulate::paged_memory::Regions regions;
    std::uint32_t base = 0;
    std::map<std::uint32_t, std::string> imports;
};

// The program as the loader leaves it: each segment at its address (a relocatable program at $81000000 plus it; its relocations are not applied).
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
    auto loaded = std::make_shared<Loaded>();
    loaded->program = formats::vita::read(path);
    const formats::vita::Program& p = loaded->program;
    loaded->base = p.segments.front().address < kBase ? kBase : 0;
    auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
    for (const formats::vita::Segment& s : p.segments)
        regions->push_back({s.address + loaded->base, Bytes(p.image.begin() + s.image_offset, p.image.begin() + s.image_offset + s.size)});
    for (const formats::vita::Import& i : p.imports)
        loaded->imports[i.stub + loaded->base] = i.name.empty() ? std::format("{}_{:08X}", i.library, i.nid) : i.name;
    loaded->regions = regions;
    const std::lock_guard guard(lock);
    if (cache.size() >= kCacheSize) cache.erase(cache.begin());
    cache[key] = loaded;
    return loaded;
}

vl::Effects effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const Loaded> loaded = load(path);
    emulate::arm::Bus bus(emulate::arm::System::Vita);
    bus.set_regions(loaded->regions);
    if (const std::int64_t seed = at(state, "fill"); seed != 0)
        for (std::size_t i = 0; i < kWorkBytes; ++i) bus.memory().write(kWork + static_cast<std::uint32_t>(i), 1, fill_byte(i, seed));
    const auto before = bus.memory().changes(1 << 20);

    emulate::arm::RunOptions options;
    options.max_steps = kSteps;
    options.sp = kStack;
    options.return_address = kReturn;
    options.imports = loaded->imports;
    options.ending_imports = {"sceKernelExitProcess", "sceKernelExitThread", "sceKernelExitDeleteThread"};
    options.a = {static_cast<std::uint32_t>(at(state, "a0")), static_cast<std::uint32_t>(at(state, "a1")), static_cast<std::uint32_t>(at(state, "a2")),
                 static_cast<std::uint32_t>(at(state, "a3"))};
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    const std::uint32_t address = (loaded->program.address_of(entry).value_or(entry) + loaded->base) | 1;   // (Thumb)
    const emulate::arm::RunResult run = pcode_cpu::call(bus, address, options);

    std::map<std::uint32_t, std::uint8_t> was(before.begin(), before.end());
    std::vector<std::string> changed;
    for (const auto& [a, value] : bus.memory().changes(1 << 20)) {
        if (changed.size() >= kShown) break;
        const auto it = was.find(a);
        if (it == was.end() || it->second != value) changed.push_back(std::format("{:08x}={:02x}", a, value));
    }
    Bytes saved;
    for (std::size_t i = 4; i < 12; ++i)
        for (int shift = 0; shift < 32; shift += 8) saved.push_back(static_cast<std::uint8_t>(run.r[i] >> shift));
    std::vector<std::string> io;
    for (const emulate::arm::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kShown) break;
    }
    std::vector<std::string> calls;
    for (const emulate::arm::BiosCall& c : run.bios) calls.push_back(c.name.empty() ? std::format("svc_{:X}", c.function) : c.text());
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.r[0]), number(run.r[1]), vl::Effect(std::move(saved)), number(run.r[13]),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::vita_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace vita_lane_detail

vl::ImageLaneSpec vita_lane_spec() {
    namespace d = vita_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "vita";
    spec.rev = 1;
    spec.states = {d::make_state(0, 0, 0, 0, 0, 0), d::make_state(d::kWork, d::kWork + 0x1000, 5, 0x10, 0, 1),
                   d::make_state(d::kWork + 0x800, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), d::make_state(2, 3, d::kWork + 0x100, d::kWork + 0x200, 7, 3)};
    spec.effects = d::effects;
    spec.what = {"end", "R0", "R1", "R4-R11", "SP", "memory changes", "hardware accesses", "imported calls"};
    spec.start = [](const vl::State& s) { return std::format("from R0={:08X} R1={:08X} R2={:X}", d::at(s, "a0"), d::at(s, "a1"), d::at(s, "a2")); };
    spec.writer = d::written_by;
    spec.key_digits = 6;
    return spec;
}

void register_vita_lane() {
    register_lane({
        .name = "vita",
        .platform = platforms::vita,
        .probe = [](const stdfs::path& p) { return formats::vita::probe(p); },
        .make_spec = vita_lane_spec,
        .detectors = {{.priority = 150, .detect = formats::detect::info_vita_1}, {.priority = 160, .detect = formats::detect::info_vita_2}},
    });
}

}  // namespace cb::engine
