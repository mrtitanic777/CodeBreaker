// cb-unity: alone
#include "engine/machine_wonderswan.hpp"

#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/unicorn_uc.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu_x86.hpp"
#include "formats/read.hpp"
#include "platforms/wonderswan.hpp"
#include "formats/wonderswan.hpp"
#include "source/console_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_wonderswan(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".ws", ".wsc"})) {   // a WonderSwan cartridge: a power-of-two ROM whose last 16 bytes start with the far jump
        try {
            const formats::wonderswan::Rom rom = formats::wonderswan::parse(file);
            return one(rom.is_color() ? "WonderSwan Color cartridge" : "WonderSwan cartridge",
                       std::format("{} KB, start {:04X}:{:04X}, developer {:02X}, game {:02X}, checksum {}", size / 1024, rom.reset_segment, rom.reset_offset, rom.developer, rom.game_id,
                                   rom.checksum_ok ? "right" : "wrong"),
                       "wonderswan", "reach");
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
namespace uni = emulate::unicorn_uc;

namespace wonderswan_lane_detail {

constexpr std::uint64_t kSteps = 20000;       // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;        // port accesses kept of a run
constexpr std::uint32_t kStackTop = 0x3FF0;
constexpr std::uint32_t kReturnIp = 0xFFFE;   // where the pushed return address points: reaching it ends the run
constexpr std::uint32_t kRamFrom = 0x0400;    // the interrupt vector table ($0-$3FF) is left as it is

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State ws_state(int ax, int bx, int cx, int dx, int si, int di, int fill) {
    return {{"ax", ax}, {"bx", bx}, {"cx", cx}, {"dx", dx}, {"si", si}, {"di", di}, {"fill", fill}};
}

int reg(const char* name) { return uni::x86_register(name).value(); }

vl::Effects ws_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::wonderswan::Rom rom = formats::wonderswan::read(path);
    const std::unique_ptr<core16::Uc> uc = emulate::pcode_cpu::open_x86(16);   // (Unicorn, unless the one emulator is asked for: pcode_cpu_x86.hpp)
    uc->mem_map(0, formats::wonderswan::kEnd);
    const Bytes mem = rom.memory();
    uc->mem_write(rom.base(), ByteView(mem).subspan(rom.base(), rom.mapped()));
    const std::uint32_t ram_size = rom.is_color() ? 0x10000 : 0x4000;
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        Bytes ram(ram_size - kRamFrom);
        for (std::size_t i = 0; i < ram.size(); ++i) ram[i] = fill_byte(i, seed);
        uc->mem_write(kRamFrom, ram);
    }

    const auto log = std::make_shared<std::vector<std::string>>();
    const auto lines = std::make_shared<std::uint32_t>(0);   // the line counter moves on each read
    uc->hook_add_insn_in([log, lines](core16::Uc&, std::uint32_t port, std::uint32_t size) -> std::uint32_t {
        std::uint32_t value = 0;
        if (port == 0x02) value = (*lines)++ % 159;   // LINE_CURRENT
        if (log->size() < kIoShown) log->push_back(std::format("in {:x} {} {:x}", port, size, value));
        return value;
    });
    uc->hook_add_insn_out([log](core16::Uc&, std::uint32_t port, std::uint32_t size, std::uint32_t value) {
        if (log->size() < kIoShown) log->push_back(std::format("out {:x} {} {:x}", port, size, value));
    });

    // the code segment: the reset jump's when the function is in it, else the segment its address is in
    std::uint32_t cs = rom.reset_segment;
    if (entry < rom.code_base() || entry - rom.code_base() > 0xFFFF) cs = (entry >> 4) & 0xF000;
    const std::uint32_t ip = entry - (cs << 4);
    for (const char* seg : {"DS", "ES", "SS"}) uc->reg_write(reg(seg), 0);
    uc->reg_write(reg("CS"), cs);
    std::uint32_t sp = kStackTop;
    const auto push = [&](std::uint16_t v) {
        sp -= 2;
        uc->mem_write(sp, Bytes{static_cast<std::uint8_t>(v & 0xFF), static_cast<std::uint8_t>(v >> 8)});
    };
    push(static_cast<std::uint16_t>(cs));   // (a far return finds the segment, a near one leaves it)
    push(static_cast<std::uint16_t>(kReturnIp));
    uc->reg_write(reg("SP"), sp);
    uc->reg_write(reg("BP"), 0);
    for (const char* r : {"AX", "BX", "CX", "DX", "SI", "DI"}) {
        std::string key = r;
        for (char& c : key) c = static_cast<char>(c - 'A' + 'a');
        uc->reg_write(reg(r), static_cast<std::uint64_t>(at(state, key.c_str())) & 0xFFFF);
    }
    uc->reg_write(reg("EFLAGS"), 0x202);

    std::string end = "limit";
    const std::uint64_t until = (static_cast<std::uint64_t>(cs) << 4) + kReturnIp;
    try {
        uc->emu_start((static_cast<std::uint64_t>(cs) << 4) + ip, until, 0, kSteps);
        const std::uint64_t now = (uc->reg_read(reg("CS")) << 4) + uc->reg_read(reg("IP"));
        if (now == until) end = "return";
    } catch (const uni::UcFault& fault) {
        end = std::string("stopped: ") + fault.what();
    }

    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    vl::Effects out = {vl::Effect(end)};
    for (const char* r : {"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP"}) out.push_back(number(uc->reg_read(reg(r)) & 0xFFFF));
    out.push_back(number(uc->reg_read(reg("EFLAGS")) & 0x8D5));   // the arithmetic flags
    out.emplace_back(uc->mem_read(0, ram_size));
    out.emplace_back(std::move(*log));
    return out;
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::console_writer::reconstruct("wonderswan", module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace wonderswan_lane_detail

vl::ImageLaneSpec wonderswan_lane_spec() {
    namespace d = wonderswan_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "wonderswan";
    spec.rev = 1;
    spec.states = {d::ws_state(0, 0, 0, 0, 0, 0, 0), d::ws_state(1, 2, 3, 4, 0x1000, 0x2000, 1), d::ws_state(0x8000, 0xFFFF, 0x7F, 0x8000, 0x3000, 0x3800, 2)};
    spec.effects = d::ws_effects;
    spec.what = {"end", "AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "flags", "RAM", "port accesses"};
    spec.start = [](const vl::State& s) { return std::format("from AX={:04X} BX={:04X} CX={:04X}", d::at(s, "ax"), d::at(s, "bx"), d::at(s, "cx")); };
    spec.writer = d::written_by;
    spec.key_digits = 5;
    return spec;
}

void register_wonderswan_lane() {
    register_lane({
        .name = "wonderswan",
        .platform = platforms::wonderswan,
        .probe = [](const stdfs::path& p) { return formats::wonderswan::probe(p); },
        .make_spec = wonderswan_lane_spec,
        .detectors = {{.priority = 240, .detect = formats::detect::info_wonderswan}},
    });
}

}  // namespace cb::engine
