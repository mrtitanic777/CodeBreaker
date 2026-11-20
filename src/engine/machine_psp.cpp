// cb-unity: alone (file-local names that other files in a batch also use)
#include "engine/machine_psp.hpp"

#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

#include "core/error.hpp"
#include "emulate/allegrex.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/read.hpp"
#include "platforms/psp.hpp"
#include "formats/psp.hpp"
#include "source/psp_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_psp(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (size >= 40 && head.size() >= 40 && starts_with(head, 0, std::string_view("\0PBP", 4))) {   // an EBOOT.PBP: its DATA.PSP is an ELF, or an encrypted ~PSP module (recorded, not handled)
        const std::uint32_t at = get<std::uint32_t>(file, 8 + 4 * 6, "detect");
        if (at + 4 <= size && starts_with(file, at, "~PSP")) return one("PSP EBOOT.PBP (encrypted ~PSP module)", "not handled: the module is encrypted", "psp", "gap");
        if (at + 4 <= size && starts_with(file, at, "" "ELF")) return one("PSP EBOOT.PBP", std::format("DATA.PSP at 0x{:X}, {} bytes", at, size), "psp", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace psp_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kShown = 4000;
constexpr std::size_t kCacheSize = 6;
constexpr std::uint32_t kWork = 0x09800000;
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

struct Loaded {
    formats::psp::Program program;
    emulate::paged_memory::Regions regions;
    emulate::allegrex::BusOptions options;
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
    std::optional<formats::psp::Program> program = formats::psp::read(path);
    if (!program) throw FormatError("psp", 0, "not an unencrypted PSP program");
    auto loaded = std::make_shared<Loaded>();
    auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
    for (const formats::mips_elf::ProgramHeader& p : program->elf.program_headers) {
        if (p.type != 1 || p.filesz == 0 || static_cast<std::uint64_t>(p.offset) + p.filesz > program->elf.data.size()) continue;
        const std::uint32_t va = p.vaddr >= 0x80000000u && p.vaddr < 0xC0000000u ? p.vaddr & 0x1FFFFFFFu : p.vaddr;
        regions->push_back({va, Bytes(program->elf.data.begin() + p.offset, program->elf.data.begin() + p.offset + p.filesz)});
    }
    for (const formats::psp::Import& i : formats::psp::imports(program->elf)) {
        if (i.variable) continue;
        loaded->options.imports[i.stub] = i.name.empty() ? std::format("{}_{:08X}", i.library, i.nid) : i.name;
    }
    loaded->options.scratchpad = !program->prx();   // a module linked at 0 is loaded where the file says: the scratchpad would lie under it
    loaded->program = std::move(*program);
    loaded->regions = regions;
    const std::lock_guard guard(lock);
    if (cache.size() >= kCacheSize) cache.erase(cache.begin());
    cache[key] = loaded;
    return loaded;
}

vl::Effects effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const Loaded> loaded = load(path);
    emulate::allegrex::Bus bus(loaded->regions, loaded->options);
    if (const std::int64_t seed = at(state, "fill"); seed != 0)
        for (std::size_t i = 0; i < kWorkBytes; ++i) bus.memory().write(kWork + static_cast<std::uint32_t>(i), 1, fill_byte(i, seed));
    const auto before = bus.memory().changes(1 << 20);
    emulate::allegrex::RunOptions options;
    options.max_steps = kSteps;
    for (std::size_t i = 0; i < 4; ++i) options.a[i] = static_cast<std::uint32_t>(at(state, i == 0 ? "a0" : i == 1 ? "a1" : i == 2 ? "a2" : "a3"));
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    const emulate::allegrex::RunResult run = emulate::allegrex::run(bus, entry, options);
    std::map<std::uint32_t, std::uint8_t> was(before.begin(), before.end());
    std::vector<std::string> changed;
    for (const auto& [address, value] : bus.memory().changes(1 << 20)) {
        if (changed.size() >= kShown) break;
        const auto it = was.find(address);
        if (it == was.end() || it->second != value) changed.push_back(std::format("{:08x}={:02x}", address, value));
    }
    Bytes saved;
    for (std::size_t i = 16; i < 24; ++i)
        for (int shift = 24; shift >= 0; shift -= 8) saved.push_back(static_cast<std::uint8_t>(run.r[i] >> shift));
    std::vector<std::string> io;
    for (const emulate::allegrex::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kShown) break;
    }
    std::vector<std::string> calls;
    for (const emulate::allegrex::Call& c : run.calls) calls.push_back(c.text());
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.r[2]), number(run.r[3]), vl::Effect(std::move(saved)), number(run.r[29]), number(run.hi[0]), number(run.lo[0]),
            number(run.f0), vl::Effect(std::move(changed)), vl::Effect(std::move(io)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::psp_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace psp_lane_detail

namespace pd = psp_lane_detail;

vl::ImageLaneSpec psp_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "psp";
    spec.rev = 1;
    spec.states = {pd::make_state(0, 0, 0, 0, 0, 0), pd::make_state(pd::kWork, pd::kWork + 0x1000, 5, 0x10, 0, 1),
                   pd::make_state(pd::kWork + 0x800, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), pd::make_state(2, 3, pd::kWork + 0x100, pd::kWork + 0x200, 7, 3)};
    spec.effects = pd::effects;
    spec.what = {"end", "V0", "V1", "S0-S7", "SP", "HI", "LO", "F0", "memory changes", "hardware accesses", "calls"};
    spec.start = [](const vl::State& s) { return std::format("from A0={:08X} A1={:08X} A2={:X}", pd::at(s, "a0"), pd::at(s, "a1"), pd::at(s, "a2")); };
    spec.writer = pd::written_by;
    spec.key_digits = 8;
    return spec;
}

void register_psp_lane() {
    register_lane({
        .name = "psp",
        .platform = platforms::psp,
        .probe = [](const stdfs::path& p) { return formats::psp::probe(p); },
        .make_spec = psp_lane_spec,
        .detectors = {{.priority = 180, .detect = formats::detect::info_psp}},
    });
}

}  // namespace cb::engine
