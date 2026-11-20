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
sl::Emulator& emulator_named(const char* description) {
    thread_local std::map<std::string, std::unique_ptr<sl::Emulator>> cache;
    auto& slot = cache[description];
    if (!slot) slot = std::make_unique<sl::Emulator>(languages() / description, std::vector<std::pair<std::string, int>>{});
    return *slot;
}

sl::Emulator& emulator_for(m::Variant variant) { return emulator_named(variant == m::Variant::Cmos ? "65c02.sla" : "6502.sla"); }

bool is_ram(std::string_view space) { return space.size() == 3 && (space[0] | 32) == 'r' && (space[1] | 32) == 'a' && (space[2] | 32) == 'm'; }

constexpr std::pair<const char*, std::uint8_t> kFlags[] = {{"N", m::status::N}, {"V", m::status::V}, {"B", m::status::B}, {"D", m::status::D},
                                                           {"I", m::status::I}, {"Z", m::status::Z}, {"C", m::status::C}};

}  // namespace

bool enabled() {
    if (const int f = g_force.load(); f >= 0) return f == 1;
    const std::optional<std::string> env = cb::fs::get_env("CB_PCODE_EMU");
    return !env || !(*env == "0" || *env == "false" || *env == "no");   // the emulator unless CB_PCODE_EMU=0
}

void force(std::optional<bool> on) { g_force = on ? (*on ? 1 : 0) : -1; }

m::RunResult run(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) {
    sl::Emulator& emu = emulator_for(options.variant);
    const std::size_t log_start = bus.log().size();
    const std::uint32_t window = static_cast<std::uint32_t>(bus.address_mask()) + 1;   // the bytes the chip's address lines reach
    const std::span<std::uint8_t> memory = bus.memory();

    // The page map the emulator decodes from: what the processor finds in memory (the bus's memory, and the quiet RAM a machine keeps behind
    // callbacks: Bus::peek), mirrors included. `plain` is the bus's own memory as it was last looked at.
    std::vector<std::uint8_t> plain(memory.begin(), memory.begin() + window);
    std::vector<std::uint8_t> shadow(window);
    for (std::uint32_t a = 0; a < window; ++a) shadow[a] = bus.peek(static_cast<std::uint16_t>(a));
    for (std::uint32_t base = 0; base < 0x10000; base += window) emu.write(base, shadow.data(), std::min<std::uint32_t>(window, 0x10000 - base));
    emu.flushDecoded();
    emu.clearStops();

    const auto put = [&](std::uint32_t index, std::uint8_t value) {   // one byte of the page map, to every mirror
        shadow[index] = value;
        for (std::uint32_t base = 0; base < 0x10000; base += window)
            if (base + index < 0x10000) emu.write(base + index, &value, 1);
    };
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = 0;
        for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(bus.read(static_cast<std::uint16_t>(addr + i))) << (8 * i);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        for (unsigned i = 0; i < size; ++i) {
            const std::uint16_t at = static_cast<std::uint16_t>(addr + i);
            bus.write(at, static_cast<std::uint8_t>(value >> (8 * i)));
            const std::uint32_t index = at & bus.address_mask();
            if (const std::uint8_t now = bus.peek(at); now != shadow[index]) put(index, now);   // (a store into RAM that code runs from)
        }
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

    const auto sync = [&] {   // what a device did to the bus's memory (a bank switch), to every mirror the emulator decodes from
        if (std::memcmp(plain.data(), memory.data(), window) == 0) return;
        std::uint32_t first = 0, last = window - 1;
        while (plain[first] == memory[first]) ++first;
        while (plain[last] == memory[last]) --last;
        for (std::uint32_t i = first; i <= last; ++i) {
            plain[i] = memory[i];
            if (const std::uint8_t now = bus.peek(static_cast<std::uint16_t>(i)); now != shadow[i]) put(i, now);
        }
        // (a write to a page code was decoded from drops the decoded instructions by itself)
    };
    sync();   // (the return address just pushed)
    std::size_t seen = bus.log().size();

    m::RunResult out;
    out.end = m::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        if (emu.pc() == m::kReturnAddress) {
            out.end = m::End::Return;
            break;
        }
        const sl::Emulator::Status status = emu.step();
        if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
            const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
            out.end = m::End::Stopped;
            out.fault = m::Fault{static_cast<std::uint8_t>(emu.readValue(pc, 1)), pc};
            break;
        }
        ++steps;
        if (emu.reg("SH") != 1) emu.setReg("SH", 1);   // (the description's SP is 16 bits wide: the chip's S wraps within page 1)
        if (bus.log().size() != seen) {   // a device was touched: it may have changed the memory under the code
            sync();
            seen = bus.log().size();
        }
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

// ---- the Z80 ----------------------------------------------------------------------------------------------------------------------

namespace z = cb::emulate::z80;

z::RunResult run(z::Bus& bus, std::uint16_t entry, const z::RunOptions& options) {
    sl::Emulator& emu = emulator_named("z80.sla");
    const std::size_t log_start = bus.log().size();
    const std::span<std::uint8_t> memory = bus.memory();
    constexpr std::uint32_t kSize = 0x10000;

    std::vector<std::uint8_t> shadow(memory.begin(), memory.begin() + kSize);
    emu.write(0, shadow.data(), kSize);
    emu.flushDecoded();
    emu.clearStops();

    bool touched = false;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        value = 0;
        if (is_ram(space)) {
            touched = true;
            for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(bus.read(static_cast<std::uint16_t>(addr + i))) << (8 * i);
            return true;
        }
        if (space == "io") {   // the port as the CPU puts it on the bus: all sixteen lines (the description's IN A,(n) puts A on the high ones)
            touched = true;
            value = bus.in(static_cast<std::uint16_t>(addr));
            return true;
        }
        return false;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (is_ram(space)) {
            touched = true;
            for (unsigned i = 0; i < size; ++i) bus.write(static_cast<std::uint16_t>(addr + i), static_cast<std::uint8_t>(value >> (8 * i)));
            return true;
        }
        if (space == "io") {
            touched = true;
            bus.out(static_cast<std::uint16_t>(addr), static_cast<std::uint8_t>(value));
            return true;
        }
        return false;
    });
    // what the description leaves to the machine: HALT, the interrupt flip-flops and the mode (the p-code has no register for them)
    bool halted = false, iff1 = false, iff2 = false;
    std::uint8_t im = 0;
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t& result) {
        if (name == "getIFF2") result = iff2 ? 1 : 0;
        else if (name == "halt") halted = true;
        else if (name == "disableMaskableInterrupts") iff1 = iff2 = false;
        else if (name == "enableMaskableInterrupts") iff1 = iff2 = true;
        else if (name == "setInterruptMode" && !args.empty()) im = static_cast<std::uint8_t>(args[0]);
        else return false;
        return true;
    });
    // what the description has no instruction for but the CPU executes: a DD or FD in front of an instruction that does not use IX or IY is
    // ignored (the instruction follows), and most of the ED page is two-byte NOPs
    emu.onBadInstruction([&](sl::Emulator& self, std::uint64_t at) {
        const std::uint8_t first = shadow[at & 0xFFFF], second = shadow[(at + 1) & 0xFFFF];
        if ((first == 0xDD || first == 0xFD) && second != 0xCB) {   // (DD CB d op with no description: the undocumented copies into a register)
            self.setPc((at + 1) & 0xFFFF);
            return true;
        }
        const bool block_gap = second >= 0xA0 && second < 0xC0 && (second & 4) != 0;   // A4-A7 AC-AF B4-B7 BC-BF
        if (first == 0xED && (second < 0x40 || (second >= 0x80 && second < 0xA0) || block_gap || second >= 0xC0 || second == 0x77 || second == 0x7F)) {
            self.setPc((at + 2) & 0xFFFF);
            return true;
        }
        return false;
    });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
            e.onUserOp({});
            e.onBadInstruction({});
        }
    } unhook{emu};

    const z::RegisterInit& init = options.regs;
    const auto byte = [](const std::optional<std::uint8_t>& v) -> std::uint64_t { return v.value_or(0); };
    emu.setReg("A", byte(init.a));
    emu.setReg("F", byte(init.f));
    emu.setReg("B", byte(init.b));
    emu.setReg("C", byte(init.c));
    emu.setReg("D", byte(init.d));
    emu.setReg("E", byte(init.e));
    emu.setReg("H", byte(init.h));
    emu.setReg("L", byte(init.l));
    for (const char* name : {"A_", "F_", "B_", "C_", "D_", "E_", "H_", "L_", "I", "R"}) emu.setReg(name, 0);
    emu.setReg("IX", init.ix.value_or(0xFFFF));
    emu.setReg("IY", init.iy.value_or(0xFFFF));
    // the call as CALL makes it: the return address pushed through the bus like any write
    const std::uint16_t sp = static_cast<std::uint16_t>(options.sp - 2);
    bus.write(sp, static_cast<std::uint8_t>(z::kReturnAddress));
    bus.write(static_cast<std::uint16_t>(sp + 1), static_cast<std::uint8_t>(z::kReturnAddress >> 8));
    emu.setReg("SP", sp);
    emu.setPc(entry);

    const auto sync = [&] {
        if (std::memcmp(shadow.data(), memory.data(), kSize) == 0) return;
        std::uint32_t first = 0, last = kSize - 1;
        while (shadow[first] == memory[first]) ++first;
        while (shadow[last] == memory[last]) --last;
        std::memcpy(shadow.data() + first, memory.data() + first, last - first + 1);
        emu.write(first, shadow.data() + first, last - first + 1);
    };

    z::RunResult out;
    out.end = z::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        if (emu.pc() == z::kReturnAddress) {
            out.end = z::End::Return;
            break;
        }
        const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
        if (options.before_step) options.before_step(pc);
        if (halted) {   // an earlier HALT ran: nothing runs any more
            out.end = z::End::Stopped;
            out.fault = z::Fault{z::Fault::Kind::Halt, pc};
            break;
        }
        touched = false;
        {   // R counts the instruction fetches: its low seven bits, two for a prefixed instruction (the description has no fetch count of its own)
            const std::uint8_t first = shadow[pc];
            const std::uint64_t r = emu.reg("R");
            emu.setReg("R", (r & 0x80) | ((r + ((first == 0xDD || first == 0xFD || first == 0xCB || first == 0xED) ? 2u : 1u)) & 0x7F));
        }
        const sl::Emulator::Status status = emu.step();
        if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
            out.end = z::End::Stopped;
            out.fault = z::Fault{z::Fault::Kind::Undecodable, pc};
            break;
        }
        ++steps;
        if (touched) sync();
    }
    z::Registers& r = out.regs;
    r.a = static_cast<std::uint8_t>(emu.reg("A"));
    r.f = static_cast<std::uint8_t>(emu.reg("F"));
    r.b = static_cast<std::uint8_t>(emu.reg("B"));
    r.c = static_cast<std::uint8_t>(emu.reg("C"));
    r.d = static_cast<std::uint8_t>(emu.reg("D"));
    r.e = static_cast<std::uint8_t>(emu.reg("E"));
    r.h = static_cast<std::uint8_t>(emu.reg("H"));
    r.l = static_cast<std::uint8_t>(emu.reg("L"));
    r.a2 = static_cast<std::uint8_t>(emu.reg("A_"));
    r.f2 = static_cast<std::uint8_t>(emu.reg("F_"));
    r.b2 = static_cast<std::uint8_t>(emu.reg("B_"));
    r.c2 = static_cast<std::uint8_t>(emu.reg("C_"));
    r.d2 = static_cast<std::uint8_t>(emu.reg("D_"));
    r.e2 = static_cast<std::uint8_t>(emu.reg("E_"));
    r.h2 = static_cast<std::uint8_t>(emu.reg("H_"));
    r.l2 = static_cast<std::uint8_t>(emu.reg("L_"));
    r.ix = static_cast<std::uint16_t>(emu.reg("IX"));
    r.iy = static_cast<std::uint16_t>(emu.reg("IY"));
    r.sp = static_cast<std::uint16_t>(emu.reg("SP"));
    r.pc = static_cast<std::uint16_t>(emu.pc());
    r.i = static_cast<std::uint8_t>(emu.reg("I"));
    r.r = static_cast<std::uint8_t>(emu.reg("R"));
    r.iff1 = iff1;
    r.iff2 = iff2;
    r.im = im;
    r.halted = halted;
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

z::RunResult call(z::Bus& bus, std::uint16_t entry, const z::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : z::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
