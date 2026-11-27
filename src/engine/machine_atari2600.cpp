#include "engine/machine_atari2600.hpp"

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
#include "platforms/atari2600.hpp"
#include "formats/atari2600.hpp"
#include "source/atari2600_writer.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace atari2600_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // device accesses kept of a run
constexpr std::uint16_t kMask = 0x1FFF;   // the 6507's 13 address lines

// How the RAM is filled from a seed (0: left zero): small numbers on odd seeds, as counters and indexes hold.
std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State a2600_state(int a, int x, int y, int fill) { return {{"a", a}, {"x", x}, {"y", y}, {"fill", fill}}; }

// What the TIA answers a read: the collision latches are clear, the paddle inputs read 0, the joystick buttons (INPT4, INPT5) are not pressed.
std::uint8_t tia_read(std::uint16_t address) {
    const unsigned reg = address & 0x0F;
    return reg == 0x0C || reg == 0x0D ? 0x80 : 0;
}

// The RIOT: the joystick directions and console switches are at rest, the timer has run out.
std::uint8_t riot_read(std::uint16_t address) {
    if (address & 0x04) return (address & 1) ? 0x80 : 0;   // TIMINT, INTIM
    switch (address & 3) {
        case 0: return 0xFF;   // SWCHA
        case 2: return 0x3F;   // SWCHB
        default: return 0;     // the direction registers
    }
}

vl::Effects a2600_effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const auto rom = std::make_shared<const formats::atari2600::Rom>(formats::atari2600::read(path));
    const std::size_t first_bank = std::min<std::size_t>(entry >> 16, rom->bank_count() - 1);
    emulate::m6502::Bus bus;
    bus.set_address_mask(kMask);
    emulate::m6502::Bus* self = &bus;
    const auto show = [rom, self](std::size_t bank) {   // the bank in the cartridge's window (a 2 KB one twice)
        const ByteView b = rom->bank(bank);
        for (std::uint32_t base = 0x1000; base < 0x2000; base += rom->bank_size) std::copy(b.begin(), b.end(), self->memory().begin() + base);
    };
    show(first_bank);
    auto ram = std::make_shared<std::array<std::uint8_t, 128>>();
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        for (std::size_t i = 0; i < ram->size(); ++i) (*ram)[i] = fill_byte(i, seed);
    }
    // the cartridge: a touch of a hotspot (read or write) selects a bank
    if (rom->scheme != formats::atari2600::Scheme::None) {
        const std::uint16_t lo = static_cast<std::uint16_t>(0x1000 + rom->first_hotspot());
        const std::uint16_t hi = static_cast<std::uint16_t>(lo + rom->bank_count() - 1);
        bus.map(
            lo, hi,
            [rom, self, show](std::uint16_t a) {
                show(*rom->bank_of_hotspot(a));
                return self->memory()[a];
            },
            [rom, show](std::uint16_t a, std::uint8_t) { show(*rom->bank_of_hotspot(a)); });
    }
    // below the cartridge's half, in 128-byte blocks: the TIA where A7 is clear, then the RAM (A9 clear) or the RIOT (A9 set)
    for (std::uint32_t block = 0; block < 0x1000 / 128; ++block) {
        const std::uint16_t lo = static_cast<std::uint16_t>(block * 128);
        const std::uint16_t hi = static_cast<std::uint16_t>(lo + 127);
        if (block % 2 == 0) {
            bus.map(lo, hi, tia_read);
        } else if (((lo >> 9) & 1) == 0) {
            bus.map_quiet(lo, hi, [ram](std::uint16_t a) { return (*ram)[a & 0x7F]; }, [ram](std::uint16_t a, std::uint8_t v) { (*ram)[a & 0x7F] = v; });
        } else {
            bus.map(lo, hi, riot_read);
        }
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
    const source::image_writer::Written w = source::atari2600_writer::reconstruct(module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace atari2600_lane_detail

vl::ImageLaneSpec atari2600_lane_spec() {
    vl::ImageLaneSpec spec;
    namespace d = atari2600_lane_detail;
    spec.kind = "atari2600";
    spec.rev = 1;
    spec.states = {d::a2600_state(0, 0, 0, 0), d::a2600_state(1, 2, 3, 1), d::a2600_state(0x80, 0xFF, 0x7F, 2), d::a2600_state(0x55, 0x10, 0xAA, 3)};
    spec.effects = d::a2600_effects;
    spec.what = {"end", "A", "X", "Y", "P", "S", "RAM", "TIA, RIOT and bank-switch accesses"};
    spec.start = [](const vl::State& s) { return std::format("from A={:02X} X={:02X} Y={:02X}", d::at(s, "a"), d::at(s, "x"), d::at(s, "y")); };
    spec.writer = d::written_by;
    spec.key_digits = 6;   // bank << 16 | address
    return spec;
}

void register_atari2600_lane() {
    register_lane({
        .name = "atari2600",
        .platform = platforms::atari2600,
        .probe = [](const stdfs::path& p) { return formats::atari2600::probe(p); },
        .make_spec = atari2600_lane_spec,
    });
}

}  // namespace cb::engine
