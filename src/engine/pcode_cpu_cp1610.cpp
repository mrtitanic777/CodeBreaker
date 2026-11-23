// cb-unity: alone
// engine/pcode_cpu_cp1610.cpp -- the Intellivision lane's CP1610 on the one p-code emulator: emulate::cp1610::run's call, on the description languages/CP1600.sla.
#include <array>
#include <format>
#include <string>
#include <string_view>
#include <vector>

#include "decompile/cp1610.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_cp1610.hpp"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace c = cb::emulate::cp1610;
namespace sl = cb::decompile::sleigh;
namespace dc = cb::decompile::cp1610;

c::RunResult run(c::Bus& bus, std::uint16_t entry, const c::RunOptions& options) {
    sl::Emulator& emu = emulator_named("CP1600.sla");
    emu.clearStops();
    emu.keepAddressContext(false);
    emu.setContext("doublebyte", 0);

    // The emulator's memory is bytes, a word at twice its address: the three words of an instruction are handed over as they are read, and a word is written only when it changed.
    // What was handed over stays with the emulator from run to run (per thread, as the emulator is), so a word that is the same in the next run costs nothing, and what the emulator
    // decoded from it stays decoded; a word the program wrote is handed over again when code is next read from it.
    thread_local std::vector<std::uint16_t> handed;
    thread_local std::vector<bool> known;
    if (handed.empty()) {
        handed.assign(0x10000, 0);
        known.assign(0x10000, false);
        emu.clearMemory();
        emu.flushDecoded();
    }
    const auto hand_over = [&](std::uint16_t address, std::uint16_t value) {
        if (known[address] && handed[address] == value) return;
        known[address] = true;
        handed[address] = value;
        const std::uint8_t bytes[2] = {static_cast<std::uint8_t>(value >> 8), static_cast<std::uint8_t>(value)};
        emu.write(static_cast<std::uint64_t>(address) * 2, bytes, 2);
    };
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = 0;
        const std::uint16_t first = static_cast<std::uint16_t>(addr / 2);
        if (size < 2) {   // (half a word: the description's double-byte read through a register that does not count: the high byte)
            value = bus.read(first) >> 8;
            return true;
        }
        for (unsigned i = 0; i < size / 2; ++i) value = value << 16 | bus.read(static_cast<std::uint16_t>(first + i));
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        const std::uint16_t first = static_cast<std::uint16_t>(addr / 2);
        for (unsigned i = 0; i < size / 2; ++i) {
            const std::uint16_t word = static_cast<std::uint16_t>(value >> (16 * (size / 2 - 1 - i)));
            bus.write(static_cast<std::uint16_t>(first + i), word);
            known[static_cast<std::uint16_t>(first + i)] = false;   // (a word written where code was read from is handed over again at the next read)
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

    static constexpr const char* kNames[] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "S", "Z", "O", "C", "I"};
    enum : unsigned { kR, kS = 8, kZ, kO, kC, kI };
    const auto& refs = reg_refs(emu, kNames);
    for (unsigned i = 0; i < 8; ++i) emu.setReg(refs[kR + i], 0);
    for (std::size_t k = 0; k < options.r.size(); ++k) emu.setReg(refs[kR + k], options.r[k]);
    for (unsigned f : {kS, kZ, kO, kC, kI}) emu.setReg(refs[f], 0);
    emu.setReg(refs[kR + 5], c::kReturnAddress);
    bus.write(options.sp, c::kReturnAddress);
    emu.setReg(refs[kR + 6], static_cast<std::uint16_t>(options.sp + 1));
    emu.setPc(entry);
    const std::size_t log_start = bus.log().size();

    const auto registers = [&] {
        c::Registers g;
        for (unsigned i = 0; i < 7; ++i) g.r[i] = static_cast<std::uint16_t>(emu.reg(refs[kR + i]));
        g.r[7] = static_cast<std::uint16_t>(emu.pc());
        g.s = emu.reg(refs[kS]) != 0;
        g.z = emu.reg(refs[kZ]) != 0;
        g.ov = emu.reg(refs[kO]) != 0;
        g.c = emu.reg(refs[kC]) != 0;
        g.i = emu.reg(refs[kI]) != 0;
        return g;
    };

    c::RunResult out;
    out.end = c::End::Limit;
    std::uint64_t steps = 0;
    bool sdbd = false;
    bool context_on = false;   // the context the decoded instructions were made under
    while (steps < options.max_steps) {
        const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
        if (pc == c::kReturnAddress) {
            out.end = c::End::Return;
            break;
        }
        if (options.on_instruction) options.on_instruction(registers());
        std::array<std::uint16_t, 3> buf{};
        for (unsigned k = 0; k < 3; ++k) {
            buf[k] = bus.read(static_cast<std::uint16_t>(pc + k));
            hand_over(static_cast<std::uint16_t>(pc + k), buf[k]);
        }
        // The own decoder is asked only for what the description cannot say: the opcodes below 8 (HLT, and a jump whose flag bits are 3) and MVO through @R7 or an immediate that
        // is not two bytes after SDBD (which the description reads as it stands: masked to eight bits). Every other word is an instruction in both.
        const unsigned word = buf[0] & 0x3FF;
        if (word < 8 || (word >= 0x240 && (word & 0x38) == 0x38)) {
            const std::optional<dc::Insn> decoded = dc::decode(buf, 0, pc, sdbd);
            if (!decoded || decoded->op == dc::Op::Hlt) {
                out.end = c::End::Stopped;
                out.fault = c::Fault{pc, std::format("{} at {:04X}", decoded ? "halt" : "undefined instruction", pc)};
                break;
            }
        }
        if (sdbd != context_on) {
            emu.setContext("doublebyte", sdbd ? 1 : 0);   // (the emulator keeps what it decoded under each context)
            context_on = sdbd;
        }
        const sl::Emulator::Status status = emu.step();
        if (status != sl::Emulator::Status::Ok) {
            out.end = c::End::Stopped;
            out.fault = c::Fault{pc, std::format("undefined instruction at {:04X}", pc)};
            break;
        }
        sdbd = word == 1;
        ++steps;
    }
    if (out.end == c::End::Limit && emu.pc() == c::kReturnAddress) out.end = c::End::Return;
    out.regs = registers();
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

c::RunResult call(c::Bus& bus, std::uint16_t entry, const c::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : c::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
