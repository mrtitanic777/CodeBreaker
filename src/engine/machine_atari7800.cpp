#include "engine/machine_atari7800.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/m6502.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/pcode_cpu.hpp"
#include "formats/atari7800.hpp"
#include "source/console_writer.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace atari7800_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // device accesses kept of a run
constexpr std::size_t kRamSize = 0x1000;  // $1800-$27FF
constexpr std::uint16_t kRamBase = 0x1800;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State a7800_state(int a, int x, int y, int fill) { return {{"a", a}, {"x", x}, {"y", y}, {"fill", fill}}; }

std::uint8_t riot_read(std::uint16_t address) {
    if (address & 0x04) return (address & 1) ? 0x80 : 0;   // TIMINT, INTIM
    switch (address & 3) {
        case 0: return 0xFF;   // SWCHA
        case 2: return 0x3F;   // SWCHB
        default: return 0;
    }
}

vl::Effects a7800_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const auto rom = std::make_shared<const formats::atari7800::Rom>(formats::atari7800::read(path));
    const std::size_t bank = std::min<std::size_t>(entry >> 16, rom->fixed_bank());
    emulate::m6502::Bus bus(rom->memory(bank));
    emulate::m6502::Bus* self = &bus;
    auto ram = std::make_shared<std::array<std::uint8_t, kRamSize>>();
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        for (std::size_t i = 0; i < ram->size(); ++i) (*ram)[i] = fill_byte(i, seed);
    }
    auto vblank = std::make_shared<unsigned>(0);   // MSTAT alternates its VBLANK bit on each read
    const auto tia_read = [](std::uint16_t a) -> std::uint8_t {
        const unsigned reg = a & 0x1F;
        return reg == 0x0C || reg == 0x0D ? 0x80 : 0;   // INPT4, INPT5: not pressed
    };
    const auto maria_read = [vblank](std::uint16_t a) -> std::uint8_t { return (a & 0x1F) == 0x08 ? ((*vblank)++ & 1 ? 0 : 0x80) : 0; };   // MSTAT
    const auto quiet_ram = [&](std::uint16_t lo, std::uint16_t hi, std::uint16_t first_index) {   // the RAM window [lo, hi] begins at ram[first_index]
        bus.map_quiet(
            lo, hi, [ram, lo, first_index](std::uint16_t a) { return (*ram)[first_index + a - lo]; },
            [ram, lo, first_index](std::uint16_t a, std::uint8_t v) { (*ram)[first_index + a - lo] = v; });
    };
    bus.map(0x0000, 0x001F, tia_read);
    bus.map(0x0020, 0x003F, maria_read);
    quiet_ram(0x0040, 0x00FF, 0x2040 - kRamBase);
    bus.map(0x0100, 0x011F, tia_read);
    bus.map(0x0120, 0x013F, maria_read);
    quiet_ram(0x0140, 0x01FF, 0x2140 - kRamBase);
    bus.map(0x0280, 0x02FF, riot_read);
    quiet_ram(kRamBase, 0x27FF, 0);
    if (rom->supergame) {   // a write to the ROM selects the bank at $8000
        bus.map_writes(0x8000, 0xFFFF, [rom, self](std::uint16_t, std::uint8_t v) {
            const ByteView b = ByteView(rom->data).subspan(std::min<std::size_t>(v, rom->fixed_bank() - 1) * formats::atari7800::kBankSize, formats::atari7800::kBankSize);
            std::copy(b.begin(), b.end(), self->memory().begin() + 0x8000);
        });
    }
    emulate::m6502::RunOptions options;
    options.a = static_cast<std::uint8_t>(at(state, "a"));
    options.x = static_cast<std::uint8_t>(at(state, "x"));
    options.y = static_cast<std::uint8_t>(at(state, "y"));
    options.max_steps = kSteps;
    const emulate::m6502::RunResult run = pcode_cpu::run(bus, static_cast<std::uint16_t>(entry & 0xFFFF), options);
    std::vector<std::string> io;
    for (const emulate::m6502::DeviceAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.kind == emulate::m6502::DeviceAccess::Kind::Read ? "r" : "w", x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.regs.a), number(run.regs.x), number(run.regs.y), number(run.regs.p), number(run.regs.s),
            vl::Effect(Bytes(ram->begin(), ram->end())), vl::Effect(std::move(io))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::console_writer::reconstruct("atari7800", module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace atari7800_lane_detail

vl::ImageLaneSpec atari7800_lane_spec() {
    namespace d = atari7800_lane_detail;
    vl::ImageLaneSpec spec;
    spec.kind = "atari7800";
    spec.rev = 1;
    spec.states = {d::a7800_state(0, 0, 0, 0), d::a7800_state(1, 2, 3, 1), d::a7800_state(0x80, 0xFF, 0x7F, 2), d::a7800_state(0x55, 0x10, 0xAA, 3)};
    spec.effects = d::a7800_effects;
    spec.what = {"end", "A", "X", "Y", "P", "S", "RAM", "TIA, MARIA, RIOT and bank-select accesses"};
    spec.start = [](const vl::State& s) { return std::format("from A={:02X} X={:02X} Y={:02X}", d::at(s, "a"), d::at(s, "x"), d::at(s, "y")); };
    spec.writer = d::written_by;
    spec.key_digits = 6;   // bank << 16 | address
    return spec;
}

void register_atari7800_lane() {
    register_lane({
        .name = "atari7800",
        .platform = [] { return platforms::described("atari7800"); },
        .probe = [](const stdfs::path& p) { return formats::atari7800::probe(p); },
        .make_spec = atari7800_lane_spec,
    });
}

}  // namespace cb::engine
