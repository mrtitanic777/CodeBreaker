// cb-unity: alone
#include "cb/engine/machine_vb.hpp"

#include <format>
#include <optional>
#include <memory>

#include "cb/core/error.hpp"
#include "cb/emulate/v810.hpp"
#include "cb/engine/lane_registry.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/read.hpp"
#include "cb/platforms/vb.hpp"
#include "cb/formats/vb.hpp"
#include "cb/source/vb_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_vb(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".vb", ".vboy"}) && size >= 0x4000 && (size & (size - 1)) == 0) {   // a Virtual Boy cartridge: a power-of-two ROM whose reset slot (the last 16 bytes) holds code
        try {
            const formats::vb::Rom rom = formats::vb::parse(file);
            return one("Virtual Boy cartridge", std::format("V810, \"{}\", {} bytes, reset at 0x{:08X}", rom.header.title, size, rom.reset_address()), "vb", "reach");
        } catch (const FormatError&) {
        }
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace vb_lane_detail {

constexpr std::uint64_t kSteps = 20000;
constexpr std::size_t kIoShown = 4000;
constexpr std::uint32_t kWorkRam = 0x05000000;
constexpr std::size_t kWorkRamSize = 0x10000;
constexpr std::uint32_t kCartRam = 0x06000000;
constexpr std::size_t kCartRamSize = 0x2000;
constexpr std::uint32_t kRom = formats::vb::kBase;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State vb_state(int a, int b, int c, int d, int fill) { return {{"a", a}, {"b", b}, {"c", c}, {"d", d}, {"fill", fill}}; }

// A register of the machine's devices by name, else its address.
std::string place(std::uint32_t address) {
    const auto it = source::vb_writer::hardware().find(address);
    return it != source::vb_writer::hardware().end() ? it->second : std::format("{:08X}", address);
}

vl::Effects vb_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::vb::Rom rom = formats::vb::read(path);
    emulate::v810::Bus bus(0x07FFFFFF);
    const std::int64_t seed = at(state, "fill");
    Bytes work(kWorkRamSize, 0);
    if (seed != 0) {
        for (std::size_t i = 0; i < work.size(); ++i) work[i] = fill_byte(i, seed);
    }
    bus.memory(0x00000000, 0x0003FFFF, {}, true, true);                      // the VIP's frame buffers, background maps, world and object memory
    bus.memory(0x00078000, 0x0007FFFF, {}, true, true);                      // the character memory, seen again
    auto idle_reads = std::make_shared<unsigned>(0);
    bus.device(0x0005F800, 0x0005F87F, [idle_reads](std::uint32_t a, unsigned) -> std::uint32_t {
        switch (a) {
            case 0x5F800: case 0x5F820: case 0x5F840: return (*idle_reads)++ & 1 ? 0xFFFF : 0;   // INTPND, DPSTTS, XPSTTS
            default: return 0;
        }
    });
    bus.device(0x01000000, 0x010007FF);                                      // the VSU
    bus.device(0x02000000, 0x02FFFFFF);                                      // the hardware control registers
    bus.memory(kWorkRam, 0x05FFFFFF, work, true, false, kWorkRamSize);
    bus.memory(kCartRam, 0x06FFFFFF, {}, true, false, kCartRamSize);
    bus.memory(kRom, 0x07FFFFFF, rom.data, false, false, rom.data.size());

    emulate::v810::RunOptions options;
    options.r[6] = static_cast<std::uint32_t>(at(state, "a"));
    options.r[7] = static_cast<std::uint32_t>(at(state, "b"));
    options.r[8] = static_cast<std::uint32_t>(at(state, "c"));
    options.r[9] = static_cast<std::uint32_t>(at(state, "d"));
    options.sp = kWorkRam + kWorkRamSize - 0x10;
    options.max_steps = kSteps;
    const emulate::v810::RunResult run = emulate::v810::run(bus, entry, options);
    std::vector<std::string> io;
    for (const emulate::v810::DeviceAccess& x : run.io) {
        io.push_back(std::format("{} {} {} {:x}", x.kind == emulate::v810::DeviceAccess::Kind::Read ? "r" : "w", place(x.address), x.size, x.value));
        if (io.size() == kIoShown) break;
    }
    vl::Effects out;
    out.push_back(vl::Effect(run.exit()));
    for (unsigned r = 1; r < 32; ++r) out.push_back(vl::Effect(static_cast<std::int64_t>(run.regs.r[r])));
    out.push_back(vl::Effect(static_cast<std::int64_t>(run.regs.psw)));
    const std::span<std::uint8_t> work_after = bus.bytes(kWorkRam);
    out.push_back(vl::Effect(Bytes(work_after.begin(), work_after.end())));
    const std::span<std::uint8_t> cart_after = bus.bytes(kCartRam);
    out.push_back(vl::Effect(Bytes(cart_after.begin(), cart_after.end())));
    out.push_back(vl::Effect(std::move(io)));
    return out;
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::vb_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace vb_lane_detail

vl::ImageLaneSpec vb_lane_spec() {
    namespace d = vb_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "vb";
    spec.rev = 1;
    spec.key_digits = 8;
    spec.states = {d::vb_state(0, 0, 0, 0, 0), d::vb_state(1, 2, 3, 4, 1), d::vb_state(0x05000100, 0xFF, 0x07000100, -1, 2)};
    spec.effects = d::vb_effects;
    spec.what = {"end"};
    for (unsigned r = 1; r < 32; ++r) spec.what.push_back(std::format("r{}", r));
    for (const char* w : {"PSW", "work RAM", "cartridge RAM", "VIP/VSU/hardware accesses"}) spec.what.push_back(w);
    spec.start = [](const vl::State& s) { return std::format("from r6={:X} r7={:X} r8={:X} r9={:X}", d::at(s, "a"), d::at(s, "b"), d::at(s, "c"), d::at(s, "d")); };
    spec.writer = d::written_by;
    return spec;
}

void register_vb_lane() {
    register_lane({
        .name = "vb",
        .platform = platforms::vb,
        .probe = [](const stdfs::path& p) { return formats::vb::probe(p); },
        .make_spec = vb_lane_spec,
        .detectors = {{.priority = 340, .detect = formats::detect::info_vb}},
    });
}

}  // namespace cb::engine
