// cb-unity: alone
#include "cb/engine/machine_switch.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/emulate/a64.hpp"
#include "cb/engine/lane_registry.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/read.hpp"
#include "cb/platforms/nswitch.hpp"
#include "cb/formats/nro.hpp"
#include "cb/source/switch_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_switch_1(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 0x80 && head.size() >= 0x80 && starts_with(head, 0x10, "NRO0")) {   // a Switch homebrew executable: the header gives the segment sizes
        return one("Nintendo Switch homebrew executable (NRO)",
                   std::format("AArch64, text {} bytes, rodata {} bytes, data {} bytes, bss {} bytes, {} bytes in all", get<std::uint32_t>(file, 0x24, "detect"), get<std::uint32_t>(file, 0x2C, "detect"),
                               get<std::uint32_t>(file, 0x34, "detect"), get<std::uint32_t>(file, 0x38, "detect"), size),
                   "switch", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_switch_2(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 0x100 && head.size() >= 0x100 && starts_with(head, 0, "NSO0")) return one("Nintendo Switch executable (NSO)", std::format("AArch64, {} bytes (the segments may be LZ4-compressed)", size), "switch", "reach");
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_switch_3(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 64 && head.size() >= 64 && head[0] == 0x7F && head[1] == 'E' && head[2] == 'L' && head[3] == 'F' && head[4] == 2 && head[5] == 1 && (head[18] | head[19] << 8) == 183)   // an AArch64 ELF: devkitA64's output
        return one("AArch64 executable (ELF)", std::format("ARMv8-A, entry 0x{:X}, {} bytes", get<std::uint32_t>(file, 24, "detect"), size), "switch", "reach");
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_switch_4(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 4 && head.size() >= 4 && starts_with(head, 0, "PFS0")) return one("Nintendo Switch NSP (PFS0)", "not handled: the contents are encrypted", "switch", "gap");
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_switch_5(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 0x104 && head.size() >= 0x104 && starts_with(head, 0x100, "HEAD")) return one("Nintendo Switch cartridge image (XCI)", "not handled: the contents are encrypted", "switch", "gap");
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;
namespace ea = emulate::a64;

namespace switch_lane_detail {

constexpr std::uint64_t kSteps = 20000;      // instructions a function may run in a comparison
constexpr std::size_t kShown = 4000;         // memory changes and calls kept of a run
constexpr std::size_t kCacheSize = 6;
constexpr std::uint64_t kBase = 0x08000000;  // where the loader puts the image
constexpr std::uint64_t kStackTop = 0x100020000ull;
constexpr std::uint64_t kStackSize = 0x20000;
constexpr std::uint64_t kTlsRo = 0x110000000ull;      // the 0x200-byte thread-local region TPIDRRO_EL0 points at (the IPC buffer is its first 0x100 bytes)
constexpr std::uint64_t kTlsData = 0x110010000ull;    // what TPIDR_EL0 points at
constexpr std::uint64_t kConfig = 0x110020000ull;     // the loader's configuration entries
constexpr std::uint64_t kWork = 0x120000000ull;       // free memory states hand out pointers into, seeded from `fill`
constexpr std::size_t kWorkBytes = 0x10000;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State make_state(std::int64_t a0, std::int64_t a1, std::int64_t a2, std::int64_t a3, std::int64_t s0, int fill) {
    return {{"a0", a0}, {"a1", a1}, {"a2", a2}, {"a3", a3}, {"s0", s0}, {"fill", fill}};
}

struct Loaded {
    ea::Regions regions;
    std::uint64_t base = kBase;
};

// The program as the loader leaves it: each segment at its address, the relative relocations added to.
std::shared_ptr<const Loaded> load(const stdfs::path& path) {
    static std::mutex lock;
    static std::map<std::string, std::shared_ptr<const Loaded>> cache;
    std::error_code ec;
    const auto file_size = stdfs::file_size(path, ec);
    const auto time = stdfs::last_write_time(path, ec).time_since_epoch().count();
    const std::string key = std::format("{}|{}|{}", path.generic_string(), static_cast<std::uint64_t>(file_size), static_cast<std::int64_t>(time));
    {
        const std::lock_guard guard(lock);
        if (const auto it = cache.find(key); it != cache.end()) return it->second;
    }
    auto loaded = std::make_shared<Loaded>();
    auto regions = std::make_shared<std::vector<ea::Region>>();
    try {
        const formats::nro::Program p = formats::nro::read(path);
        for (const formats::nro::Segment& s : p.segments) {
            ea::Region r;
            r.address = kBase + s.address;
            r.bytes.assign(p.flat.begin() + static_cast<std::ptrdiff_t>(s.address), p.flat.begin() + static_cast<std::ptrdiff_t>(s.address + s.size));
            r.bytes.resize(static_cast<std::size_t>(s.memory_size), 0);
            r.writable = s.writable;
            regions->push_back(std::move(r));
        }
        for (const formats::nro::Relocation& rel : p.relocations) {
            if (rel.type != formats::nro::kRelativeReloc) continue;
            for (ea::Region& r : *regions) {
                if (kBase + rel.offset < r.address || kBase + rel.offset + 8 > r.address + r.bytes.size()) continue;
                const std::uint64_t v = kBase + static_cast<std::uint64_t>(rel.addend);
                for (unsigned i = 0; i < 8; ++i) r.bytes[static_cast<std::size_t>(kBase + rel.offset - r.address) + i] = static_cast<std::uint8_t>(v >> (8 * i));
                break;
            }
        }
    } catch (const Error&) {
        // not a program that parses (the loaded image of a NSO or an ELF the writer rebuilt): one flat region, everything writable
        regions->clear();
        ea::Region r;
        r.address = kBase;
        r.bytes = fs::read_file(path);
        r.bytes.resize(r.bytes.size() + 0x10000, 0);
        r.writable = true;
        regions->push_back(std::move(r));
    }
    loaded->regions = regions;
    const std::lock_guard guard(lock);
    if (cache.size() >= kCacheSize) cache.erase(cache.begin());
    cache[key] = loaded;
    return loaded;
}

void put64(ea::Memory& m, std::uint64_t a, std::uint64_t v) { m.write(a, 8, v); }

// The loader's configuration entries (libnx's ConfigEntry: key, flags, two values).
void write_config(ea::Memory& m) {
    struct Entry {
        std::uint32_t key, flags;
        std::uint64_t v0, v1;
    };
    const Entry entries[] = {{1, 1, 0xD0000010, 0},            // MainThreadHandle
                             {6, 0, ~0ull, ~0ull},             // SyscallAvailableHint
                             {7, 0, 0, 0},                     // AppletType: application
                             {9, 0, 0xD0000011, 0},            // ProcessHandle
                             {14, 0, 0x0123456789ABCDEFull, 0x0FEDCBA987654321ull},   // RandomSeed
                             {16, 0, 0x0F0000, 0},             // HosVersion
                             {0, 0, 0, 0}};                    // EndOfList
    std::uint64_t at = kConfig;
    for (const Entry& e : entries) {
        m.write(at, 4, e.key);
        m.write(at + 4, 4, e.flags);
        put64(m, at + 8, e.v0);
        put64(m, at + 16, e.v1);
        at += 24;
    }
}

vl::Effects effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const Loaded> loaded = load(path);
    ea::Memory memory(loaded->regions);
    memory.map_writable(kStackTop - kStackSize, kStackSize);
    memory.map_writable(kTlsRo, 0x1000);
    memory.map_writable(kTlsData, 0x2000);
    memory.map_writable(kConfig, 0x1000);
    memory.map_writable(kWork, kWorkBytes);
    write_config(memory);
    if (const std::int64_t seed = at(state, "fill"); seed != 0)
        for (std::size_t i = 0; i < kWorkBytes; ++i) memory.write(kWork + i, 1, fill_byte(i, seed));
    const auto before = memory.changes(1 << 20);

    ea::RunOptions options;
    options.max_steps = kSteps;
    options.sp = kStackTop - 0x100;
    options.tpidrro_el0 = kTlsRo;
    options.tpidr_el0 = kTlsData;
    for (unsigned i = 0; i < 4; ++i) options.x[i] = static_cast<std::uint64_t>(at(state, std::format("a{}", i).c_str()));
    options.x19 = static_cast<std::uint64_t>(at(state, "s0"));
    const ea::RunResult run = ea::run(memory, loaded->base + entry, options);

    std::map<std::uint64_t, std::uint8_t> was(before.begin(), before.end());
    std::vector<std::string> changed;
    for (const auto& [a, value] : memory.changes(1 << 20)) {
        if (changed.size() >= kShown) break;
        const auto it = was.find(a);
        if (it == was.end() || it->second != value) changed.push_back(std::format("{:x}={:02x}", a, value));
    }
    Bytes saved;
    for (std::size_t i = 19; i < 29; ++i)
        for (int shift = 0; shift < 64; shift += 8) saved.push_back(static_cast<std::uint8_t>(run.regs.x[i] >> shift));
    std::vector<std::string> calls;
    for (const ea::SvcCall& c : run.calls) {
        const std::string_view name = ea::svc_name(c.number);
        std::string line = std::format("svc{}({:x},{:x},{:x},{:x})", name.empty() ? std::format("_{:02X}", c.number) : std::string(name), c.args[0], c.args[1], c.args[2], c.args[3]);
        if (!c.note.empty()) line += " " + c.note;
        calls.push_back(std::move(line));
        if (calls.size() == kShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.regs.x[0]), number(run.regs.x[1]), vl::Effect(std::move(saved)), number(run.regs.sp), vl::Effect(std::move(changed)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::switch_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace switch_lane_detail

vl::ImageLaneSpec switch_lane_spec() {
    namespace d = switch_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "switch";
    spec.rev = 1;
    // the first state is what the homebrew loader gives `_start`; the others hand out pointers into the work area
    spec.states = {d::make_state(static_cast<std::int64_t>(d::kConfig), -1, 0, 0, 0, 0), d::make_state(static_cast<std::int64_t>(d::kWork), static_cast<std::int64_t>(d::kWork + 0x1000), 5, 0x10, 1, 1),
                   d::make_state(static_cast<std::int64_t>(d::kWork + 0x800), -1, 0x7F, static_cast<std::int64_t>(0x80000000), 3, 2),
                   d::make_state(2, 3, static_cast<std::int64_t>(d::kWork + 0x100), static_cast<std::int64_t>(d::kWork + 0x200), 7, 3)};
    spec.effects = d::effects;
    spec.what = {"end", "x0", "x1", "x19-x28", "SP", "memory changes", "kernel calls"};
    spec.start = [](const vl::State& s) { return std::format("from x0={:X} x1={:X} x2={:X}", static_cast<std::uint64_t>(d::at(s, "a0")), static_cast<std::uint64_t>(d::at(s, "a1")), static_cast<std::uint64_t>(d::at(s, "a2"))); };
    spec.writer = d::written_by;
    spec.key_digits = 6;
    return spec;
}

void register_switch_lane() {
    register_lane({
        .name = "switch",
        .platform = platforms::nswitch,
        .probe = [](const stdfs::path& p) { return formats::nro::probe(p); },
        .make_spec = switch_lane_spec,
        .detectors = {{.priority = 50, .detect = formats::detect::info_switch_1}, {.priority = 60, .detect = formats::detect::info_switch_2}, {.priority = 70, .detect = formats::detect::info_switch_3}, {.priority = 80, .detect = formats::detect::info_switch_4}, {.priority = 90, .detect = formats::detect::info_switch_5}},
    });
}

}  // namespace cb::engine
