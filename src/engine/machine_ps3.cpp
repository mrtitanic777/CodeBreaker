// cb-unity: alone
#include "engine/machine_ps3.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "core/error.hpp"
#include "emulate/powerpc.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/read.hpp"
#include "platforms/ps3.hpp"
#include "formats/ps3.hpp"
#include "source/ps3_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_ps3_1(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 64 && head.size() >= 64 && head[0] == 0x7F && head[1] == 'E' && head[2] == 'L' && head[3] == 'F' && head[4] == 2 && head[5] == 2 && (head[18] << 8 | head[19]) == 21) {   // a 64-bit big-endian PowerPC ELF: PSL1GHT's output
        const std::uint64_t entry = static_cast<std::uint64_t>(be32_or_zero(head, 24)) << 32 | be32_or_zero(head, 28);
        return one("PlayStation 3 executable (ELF)", std::format("Cell PPU, entry descriptor 0x{:X}, {} bytes", entry, size), "ps3", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_ps3_2(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 0x60 && head.size() >= 0x60 && starts_with(head, 0, std::string_view("SCE\0", 4)) && be32_or_zero(head, 4) == 2) {   // a PS3 SELF: the segment table says whether they are encrypted
        const std::uint64_t info = static_cast<std::uint64_t>(be32_or_zero(head, 0x48)) << 32 | be32_or_zero(head, 0x4C);
        const bool encrypted = info + 32 <= size && info + 32 <= head.size() ? be32_or_zero(head, static_cast<std::size_t>(info) + 28) == 1 : true;
        if (encrypted) return one("PlayStation 3 SELF (encrypted)", "not handled: the segments are encrypted", "ps3", "gap");
        return one("PlayStation 3 SELF", std::format("Cell PPU, {} bytes", size), "ps3", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;
namespace pw = emulate::powerpc;

namespace ps3_lane_detail {

constexpr std::size_t kShown = 4000;
constexpr std::size_t kCacheSize = 6;

struct Loaded {
    formats::ps3::Program program;
    pw::PageImage image;
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
    auto loaded = std::make_shared<Loaded>();
    loaded->program = formats::ps3::read(path);
    std::vector<pw::Region> regions;
    for (const formats::ps3::Segment& s : loaded->program.segments)
        regions.push_back({static_cast<std::uint32_t>(s.address), Bytes(loaded->program.image.begin() + s.image_offset, loaded->program.image.begin() + s.image_offset + s.size)});
    loaded->image = pw::PageImage::of(regions);
    const std::lock_guard guard(lock);
    if (cache.size() >= kCacheSize) cache.erase(cache.begin());
    cache[key] = loaded;
    return loaded;
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State make_state(std::uint32_t r3, std::uint32_t r4, std::uint32_t r5, std::uint32_t r6, std::uint32_t r7, std::int64_t f1, std::int64_t f2, int fill) {
    return {{"r3", r3}, {"r4", r4}, {"r5", r5}, {"r6", r6}, {"r7", r7}, {"f1", f1}, {"f2", f2}, {"fill", fill}};
}

vl::Effects effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const Loaded> loaded = load(path);
    pw::RunOptions options;
    options.shared = loaded->image;
    options.fill = static_cast<pw::Fill>(at(state, "fill"));
    options.r2 = loaded->program.toc;
    options.fargs = {static_cast<double>(at(state, "f1")) / 1000.0, static_cast<double>(at(state, "f2")) / 1000.0};
    const std::array<std::uint64_t, 5> args = {static_cast<std::uint32_t>(at(state, "r3")), static_cast<std::uint32_t>(at(state, "r4")), static_cast<std::uint32_t>(at(state, "r5")),
                                               static_cast<std::uint32_t>(at(state, "r6")), static_cast<std::uint32_t>(at(state, "r7"))};
    const std::uint32_t address = static_cast<std::uint32_t>(loaded->program.address_of(entry).value_or(entry));
    const pw::RunResult run = pw::run({}, address, args, options);
    const pw::Machine& m = *run.machine;
    std::vector<std::string> changed;
    for (const pw::ByteWrite& w : run.writes) {
        if (changed.size() == kShown) break;
        changed.push_back(std::format("{:08x}={:02x}", w.address, w.after));
    }
    Bytes saved;
    for (std::size_t i = 14; i < 32; ++i)
        for (int shift = 56; shift >= 0; shift -= 8) saved.push_back(static_cast<std::uint8_t>(m.r[i] >> shift));
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.truncated ? std::string("limit") : run.exit_text()), number(run.ret), number(m.r[4]), number(run.fret), number(m.r[1]), vl::Effect(std::move(saved)),
            vl::Effect(std::move(changed))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::ps3_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace ps3_lane_detail

vl::ImageLaneSpec ps3_lane_spec() {
    namespace d = ps3_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "ps3";
    spec.rev = 1;
    constexpr std::uint32_t kWork = pw::kArgsBase;
    spec.states = {d::make_state(0, 0, 0, 0, 0, 0, 0, 0), d::make_state(kWork, kWork + 0x1000, 5, 0x10, 1, 1500, -2250, 1),
                   d::make_state(kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, kWork + 0x40, 10000000, 3, 2), d::make_state(2, 3, kWork + 0x100, kWork + 0x200, 7, 500, 125, 3)};
    spec.effects = d::effects;
    spec.what = {"end", "r3", "r4", "f1", "r1", "r14-r31", "memory changes"};
    spec.start = [](const vl::State& s) { return std::format("from r3={:08X} r4={:08X} r5={:X}", d::at(s, "r3"), d::at(s, "r4"), d::at(s, "r5")); };
    spec.writer = d::written_by;
    spec.key_digits = 8;
    return spec;
}

void register_ps3_lane() {
    register_lane({
        .name = "ps3",
        .platform = platforms::ps3,
        .probe = [](const stdfs::path& p) { return formats::ps3::probe(p); },
        .make_spec = ps3_lane_spec,
        .detectors = {{.priority = 100, .detect = formats::detect::info_ps3_1}, {.priority = 110, .detect = formats::detect::info_ps3_2}},
    });
}

}  // namespace cb::engine
