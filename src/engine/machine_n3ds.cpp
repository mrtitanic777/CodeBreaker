// cb-unity: alone
#include "engine/machine_n3ds.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "core/error.hpp"
#include "emulate/arm.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/read.hpp"
#include "platforms/n3ds.hpp"
#include "formats/n3ds.hpp"
#include "source/n3ds_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_n3ds_1(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 32 && head.size() >= 32 && starts_with(head, 0, "3DSX")) {   // a 3DS homebrew executable: the header gives the segment sizes
        return one("Nintendo 3DS homebrew executable (3DSX)",
                   std::format("ARM11, code {} bytes, rodata {} bytes, data {} bytes, bss {} bytes", get<std::uint32_t>(file, 16, "detect"), get<std::uint32_t>(file, 20, "detect"),
                               get<std::uint32_t>(file, 24, "detect") - get<std::uint32_t>(file, 28, "detect"), get<std::uint32_t>(file, 28, "detect")),
                   "n3ds", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_n3ds_2(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 52 && head.size() >= 52 && head[0] == 0x7F && head[1] == 'E' && head[2] == 'L' && head[3] == 'F' && head[4] == 1 && head[5] == 1 && (head[18] | head[19] << 8) == 40 &&
        (head[16] | head[17] << 8) == 2) {   // an ARM ELF whose entry is in the 3DS homebrew's range ($00100000 up): devkitARM's output
        const std::uint32_t entry = get<std::uint32_t>(file, 24, "detect");
        if (entry >= 0x00100000u && entry < 0x01000000u) return one("Nintendo 3DS executable (ELF)", std::format("ARM11, entry 0x{:08X}, {} bytes", entry, size), "n3ds", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_n3ds_3(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 0x104 && head.size() >= 0x104 && starts_with(head, 0x100, "NCSD")) return one("Nintendo 3DS cartridge image (NCSD)", "not handled: the contents are encrypted", "n3ds", "gap");
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace n3ds_lane_detail {

constexpr std::uint64_t kSteps = 20000;      // instructions a function may run in a comparison
constexpr std::size_t kShown = 4000;         // changes and accesses kept of a run
constexpr std::size_t kCacheSize = 6;
constexpr std::uint32_t kWork = 0x08100000;  // free memory a run may be handed pointers into, seeded from `fill` (the application heap)
constexpr std::size_t kWorkBytes = 0x10000;
constexpr std::uint32_t kStack = 0x10100000;
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
    formats::n3ds::Program program;
    emulate::paged_memory::Regions regions;
};

// The program as the loader leaves it: each segment at its address, the absolute relocations added to.
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
    loaded->program = formats::n3ds::read(path);
    const formats::n3ds::Program& p = loaded->program;
    auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
    for (const formats::n3ds::Segment& s : p.segments)
        regions->push_back({s.address, Bytes(p.file.begin() + s.file_offset, p.file.begin() + s.file_offset + s.file_size)});
    if (p.kind == formats::n3ds::Kind::X3dsx) {
        for (const std::uint32_t w : p.pointers) {
            for (std::size_t k = 0; k < p.segments.size(); ++k) {
                const formats::n3ds::Segment& s = p.segments[k];
                if (w < s.file_offset || w - s.file_offset + 4 > s.file_size) continue;
                Bytes& bytes = (*regions)[k].bytes;
                const std::size_t at = w - s.file_offset;
                std::uint32_t v = static_cast<std::uint32_t>(bytes[at]) | bytes[at + 1] << 8 | bytes[at + 2] << 16 | static_cast<std::uint32_t>(bytes[at + 3]) << 24;
                v += formats::n3ds::kBase;
                for (unsigned i = 0; i < 4; ++i) bytes[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
                break;
            }
        }
    }
    loaded->regions = regions;
    const std::lock_guard guard(lock);
    if (cache.size() >= kCacheSize) cache.erase(cache.begin());
    cache[key] = loaded;
    return loaded;
}

// The hardware ranges the ARM11 sees: named, logged; a register reads what was last written to it.
void map_hardware(emulate::arm::Bus& bus) {
    const auto store = std::make_shared<std::map<std::uint32_t, std::uint32_t>>();
    const emulate::arm::Bus::ReadFn read = [store](std::uint32_t a, unsigned) -> std::uint32_t {
        const auto it = store->find(a & ~3u);
        return it == store->end() ? 0 : it->second;
    };
    const emulate::arm::Bus::WriteFn write = [store](std::uint32_t a, unsigned, std::uint32_t v) { (*store)[a & ~3u] = v; };
    bus.map(0x1EC00000, 0x1EFFFFFF, read, write);   // the I/O register windows (the GPU registers are in it at $1EF00000)
    bus.map(0x1F000000, 0x1F5FFFFF, read, write);   // video memory
    bus.map(0x1FF80000, 0x1FF82FFF, read, write);   // the kernel's configuration and shared pages, the thread-local storage
}

vl::Effects effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const Loaded> loaded = load(path);
    emulate::arm::Bus bus(emulate::arm::System::N3ds);
    bus.set_regions(loaded->regions);
    map_hardware(bus);
    bus.cp15_write(0xD03, 0x1FF82000);   // the thread-local storage pointer (c13, c0, 3)
    if (const std::int64_t seed = at(state, "fill"); seed != 0)
        for (std::size_t i = 0; i < kWorkBytes; ++i) bus.memory().write(kWork + static_cast<std::uint32_t>(i), 1, fill_byte(i, seed));
    const auto before = bus.memory().changes(1 << 20);

    emulate::arm::RunOptions options;
    options.max_steps = kSteps;
    options.sp = kStack;
    options.return_address = kReturn;
    options.ending = {0x03, 0x09, 0x3C};   // ExitProcess, ExitThread, Break
    options.a = {static_cast<std::uint32_t>(at(state, "a0")), static_cast<std::uint32_t>(at(state, "a1")), static_cast<std::uint32_t>(at(state, "a2")),
                 static_cast<std::uint32_t>(at(state, "a3"))};
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    const std::uint32_t address = loaded->program.address_of(entry).value_or(entry);
    const emulate::arm::RunResult run = emulate::arm::run(bus, address, options);

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
    for (const emulate::arm::BiosCall& c : run.bios) {
        const std::string_view name = source::n3ds_writer::svc_name(c.function);
        calls.push_back(std::format("svc{}({:x},{:x},{:x},{:x})", name.empty() ? std::format("_{:02X}", c.function) : std::string(name), c.args[0], c.args[1], c.args[2], c.args[3]));
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.r[0]), number(run.r[1]), vl::Effect(std::move(saved)), number(run.r[13]),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::n3ds_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace n3ds_lane_detail

vl::ImageLaneSpec n3ds_lane_spec() {
    namespace d = n3ds_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "n3ds";
    spec.rev = 1;
    spec.states = {d::make_state(0, 0, 0, 0, 0, 0), d::make_state(d::kWork, d::kWork + 0x1000, 5, 0x10, 0, 1),
                   d::make_state(d::kWork + 0x800, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), d::make_state(2, 3, d::kWork + 0x100, d::kWork + 0x200, 7, 3)};
    spec.effects = d::effects;
    spec.what = {"end", "R0", "R1", "R4-R11", "SP", "memory changes", "hardware accesses", "kernel calls"};
    spec.start = [](const vl::State& s) { return std::format("from R0={:08X} R1={:08X} R2={:X}", d::at(s, "a0"), d::at(s, "a1"), d::at(s, "a2")); };
    spec.writer = d::written_by;
    spec.key_digits = 6;
    return spec;
}

void register_n3ds_lane() {
    register_lane({
        .name = "n3ds",
        .platform = platforms::n3ds,
        .probe = [](const stdfs::path& p) { return formats::n3ds::probe(p); },
        .make_spec = n3ds_lane_spec,
        .detectors = {{.priority = 30, .detect = formats::detect::info_n3ds_1}, {.priority = 40, .detect = formats::detect::info_n3ds_2}, {.priority = 170, .detect = formats::detect::info_n3ds_3}},
    });
}

}  // namespace cb::engine
