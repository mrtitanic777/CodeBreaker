#include "engine/pcode_cpu.hpp"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "decompile/engine/pcode_emulator.hh"

namespace cb::engine::pcode_cpu {

namespace m = cb::emulate::m6502;
namespace sl = cb::decompile::sleigh;

namespace {

std::atomic<int> g_force{-1};   // -1: the environment; 0 / 1: forced

std::filesystem::path languages() {
    if (const std::optional<std::string> env = cb::fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return cb::fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

// One emulator per thread and variant: reading the description is the costly part of a run, the state is all set again each time.
sl::Emulator& emulator_for(m::Variant variant) {
    thread_local std::map<int, std::unique_ptr<sl::Emulator>> cache;
    auto& slot = cache[static_cast<int>(variant)];
    if (!slot) slot = std::make_unique<sl::Emulator>(languages() / (variant == m::Variant::Cmos ? "65c02.sla" : "6502.sla"), std::vector<std::pair<std::string, int>>{});
    return *slot;
}

bool is_ram(std::string_view space) { return space.size() == 3 && (space[0] | 32) == 'r' && (space[1] | 32) == 'a' && (space[2] | 32) == 'm'; }

constexpr std::pair<const char*, std::uint8_t> kFlags[] = {{"N", m::status::N}, {"V", m::status::V}, {"B", m::status::B}, {"D", m::status::D},
                                                           {"I", m::status::I}, {"Z", m::status::Z}, {"C", m::status::C}};

}  // namespace

bool enabled() {
    if (const int f = g_force.load(); f >= 0) return f == 1;
    const std::optional<std::string> env = cb::fs::get_env("CB_PCODE_EMU");
    return env && (*env == "1" || *env == "true" || *env == "yes");
}

void force(std::optional<bool> on) { g_force = on ? (*on ? 1 : 0) : -1; }

m::RunResult run(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) {
    sl::Emulator& emu = emulator_for(options.variant);
    const std::size_t log_start = bus.log().size();
    const std::uint32_t window = static_cast<std::uint32_t>(bus.address_mask()) + 1;   // the bytes the chip's address lines reach
    const std::span<std::uint8_t> memory = bus.memory();

    // The page map the emulator decodes from: the bus's memory as the processor sees it, mirrors included.
    std::vector<std::uint8_t> shadow(memory.begin(), memory.begin() + window);
    for (std::uint32_t base = 0; base < 0x10000; base += window) emu.write(base, shadow.data(), std::min<std::uint32_t>(window, 0x10000 - base));
    emu.flushDecoded();
    emu.clearStops();

    bool touched = false;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        touched = true;
        value = 0;
        for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(bus.read(static_cast<std::uint16_t>(addr + i))) << (8 * i);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        touched = true;
        for (unsigned i = 0; i < size; ++i) bus.write(static_cast<std::uint16_t>(addr + i), static_cast<std::uint8_t>(value >> (8 * i)));
        return true;
    });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
        }
    } unhook{emu};

    // the registers: the flags are one byte each in the description, P is made of them
    emu.setReg("A", options.a);
    emu.setReg("X", options.x);
    emu.setReg("Y", options.y);
    for (const auto& [name, bit] : kFlags) emu.setReg(name, (options.p & bit) != 0 ? 1 : 0);
    emu.setReg("SH", 1);
    emu.setReg("S", 0xFD);
    // the call as JSR makes it: the return address less one pushed (through the bus like any write), RTS adds one
    const std::uint16_t ret = m::kReturnAddress - 1;
    bus.write(0x1FD, static_cast<std::uint8_t>(ret >> 8));
    bus.write(0x1FC, static_cast<std::uint8_t>(ret));
    emu.setReg("S", 0xFB);
    emu.setPc(entry);

    const auto sync = [&] {   // what the bus's memory now holds, to every mirror the emulator decodes from
        if (std::memcmp(shadow.data(), memory.data(), window) == 0) return;
        std::uint32_t first = 0, last = window - 1;
        while (shadow[first] == memory[first]) ++first;
        while (shadow[last] == memory[last]) --last;
        std::memcpy(shadow.data() + first, memory.data() + first, last - first + 1);
        for (std::uint32_t base = 0; base < 0x10000; base += window) emu.write(base + first, shadow.data() + first, std::min<std::uint32_t>(last - first + 1, 0x10000 - base - first));
        emu.flushDecoded();
    };

    m::RunResult out;
    out.end = m::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        if (emu.pc() == m::kReturnAddress) {
            out.end = m::End::Return;
            break;
        }
        touched = false;
        const sl::Emulator::Status status = emu.step();
        if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
            const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
            out.end = m::End::Stopped;
            out.fault = m::Fault{static_cast<std::uint8_t>(emu.readValue(pc, 1)), pc};
            break;
        }
        ++steps;
        if (emu.reg("SH") != 1) emu.setReg("SH", 1);   // (the description's SP is 16 bits wide: the chip's S wraps within page 1)
        if (touched) sync();
    }
    out.regs.a = static_cast<std::uint8_t>(emu.reg("A"));
    out.regs.x = static_cast<std::uint8_t>(emu.reg("X"));
    out.regs.y = static_cast<std::uint8_t>(emu.reg("Y"));
    out.regs.s = static_cast<std::uint8_t>(emu.reg("S"));
    out.regs.pc = static_cast<std::uint16_t>(emu.pc());
    // (B is no flag of the chip: the description sets it as PHP and BRK push it; the own core's P keeps the bit it was given)
    std::uint8_t p = static_cast<std::uint8_t>(m::status::U | (options.p & m::status::B));
    for (const auto& [name, bit] : kFlags)
        if (bit != m::status::B && emu.reg(name) != 0) p |= bit;
    out.regs.p = p;
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

m::RunResult call(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : m::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
