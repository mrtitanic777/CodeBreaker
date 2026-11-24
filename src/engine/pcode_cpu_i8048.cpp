#include "engine/pcode_cpu_i8048.hpp"

#include "engine/pcode_cpu_internal.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"

namespace cb::engine::pcode_cpu {

namespace i8048cpu = cb::emulate::i8048;
namespace sl = cb::decompile::sleigh;

namespace {

// The registers, found once: the description's R0-R7 are those of the bank selected; BS is the bank bit.
constexpr const char* kI8048Names[] = {"A", "SP", "C", "AC", "F0", "F1", "BS", "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7"};
namespace i8048_reg {
enum : std::size_t { A, SP, C, AC, F0, F1, BS, R0 };   // (R1-R7 follow R0)
}

constexpr std::uint16_t kI8048Mask = 0xFFF;

// The own core's DA: the low digit adjusted when above 9 or on the auxiliary carry, then the high digit when above 9 or on the carry.
struct I8048Adjusted {
    std::uint8_t a;
    bool c;
};
I8048Adjusted i8048_decimal_adjust(std::uint8_t a, bool ac, bool c) {
    unsigned v = a;
    if ((v & 0xF) > 9 || ac) {
        if (v + 6 > 0xFF) c = true;
        v = (v + 6) & 0xFF;
    }
    if ((v >> 4) > 9 || c) {
        v = (v + 0x60) & 0xFF;
        c = true;
    }
    return {static_cast<std::uint8_t>(v), c};
}

// Whether an opcode is one of the page-relative jumps: JBn, DJNZ and the conditional jumps (JTF, JNT0, JT0, JNT1, JT1, JF1, JNI, JNZ, JF0, JZ, JNC, JC).
bool i8048_page_jump(std::uint8_t op) {
    if ((op & 0x1F) == 0x12 || (op & 0xF8) == 0xE8) return true;
    switch (op) {
        case 0x16: case 0x26: case 0x36: case 0x46: case 0x56: case 0x76: case 0x86: case 0x96: case 0xB6: case 0xC6: case 0xE6: case 0xF6: return true;
        default: return false;
    }
}

}  // namespace

i8048cpu::RunResult run(i8048cpu::Bus& bus, std::uint16_t entry, const i8048cpu::RunOptions& options) {
    sl::Emulator& emu = emulator_named("8048.sla");
    const auto& reg = reg_refs(emu, kI8048Names);
    const std::size_t log_start = bus.log().size();

    // The page map the emulator decodes from: the 4 KB of program memory, mirrored through the 64 KB the description's code space has.
    std::array<std::uint8_t, 4096> program{};
    for (std::size_t a = 0; a < program.size(); ++a) program[a] = bus.program(static_cast<std::uint16_t>(a));
    for (std::uint32_t base = 0; base < 0x10000; base += 0x1000) emu.write(base, program.data(), program.size());
    emu.keepAddressContext(false);   // (SEL MB0 and MB1 are followed here: a run must not be decoded under what an earlier flow left)
    emu.setContext("DBF", 0);
    emu.flushDecoded();
    emu.clearStops();

    // what the description leaves to the machine: the internal RAM (the registers of the bank selected are the description's), the ports' latches, the timer and its flags
    std::array<std::uint8_t, 64> ram{};
    std::uint8_t bank = 0, timer = 0, p1 = 0xFF, p2 = 0xFF, current = 0;
    bool timer_flag = false, timer_on = false, counter_on = false, ext_int = false, timer_int = false, mb1 = false;
    std::optional<bool> condition;   // what the last test operation (T0, T1, INT, the timer flag) gave

    const auto window = [&](std::uint64_t address) -> std::optional<std::size_t> {   // the register of the bank selected that an address of the internal RAM names
        const std::size_t at = address & 63, base = bank ? 24 : 0;
        return at >= base && at < base + 8 ? std::optional<std::size_t>(at - base) : std::nullopt;
    };
    const auto internal_read = [&](std::uint64_t at) -> std::uint8_t {
        const std::optional<std::size_t> r = window(at);
        return r ? static_cast<std::uint8_t>(emu.reg(reg[i8048_reg::R0 + *r])) : ram[at & 63];
    };
    const auto internal_write = [&](std::uint64_t at, std::uint8_t byte) {
        if (const std::optional<std::size_t> r = window(at)) emu.setReg(reg[i8048_reg::R0 + *r], byte);
        else ram[at & 63] = byte;
    };
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (is_ram(space)) return false;   // (the code space is the emulator's)
        if (space == "INTMEM") {
            value = 0;
            for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(internal_read(addr + i)) << (8 * i);
            return true;
        }
        if (space == "EXTMEM") {
            value = bus.read(i8048cpu::Place::Data, static_cast<std::uint8_t>(addr));
            return true;
        }
        if (space == "PORT") {
            switch (addr) {
                case 0:   // (BUS: INS reads it; ORL and ANL write the operand alone)
                    value = current == 0x88 ? 0 : current == 0x98 ? 0xFF : bus.read(i8048cpu::Place::Bus);
                    return true;
                case 1:   // (P1: IN reads the pins and the latch, ORL and ANL the latch)
                    value = current == 0x09 ? (bus.read(i8048cpu::Place::P1) & p1) : p1;
                    return true;
                case 2: value = current == 0x0A ? (bus.read(i8048cpu::Place::P2) & p2) : p2; return true;
                default:   // (the expander's ports: MOVD reads them, ORLD and ANLD write the operand alone)
                    value = (current & 0xFC) == 0x8C ? 0 : (current & 0xFC) == 0x9C ? 0xFF : bus.read(static_cast<i8048cpu::Place>(static_cast<unsigned>(i8048cpu::Place::P4) + (addr & 3)));
                    return true;
            }
        }
        return false;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (is_ram(space)) return false;
        if (space == "INTMEM") {
            for (unsigned i = 0; i < size; ++i) internal_write(addr + i, static_cast<std::uint8_t>(value >> (8 * i)));
            return true;
        }
        if (space == "EXTMEM") {
            bus.write(i8048cpu::Place::Data, static_cast<std::uint8_t>(value), static_cast<std::uint8_t>(addr));
            return true;
        }
        if (space == "PORT") {
            const std::uint8_t byte = static_cast<std::uint8_t>(value);
            switch (addr) {
                case 0: bus.write(i8048cpu::Place::Bus, byte); return true;
                case 1: p1 = byte; bus.write(i8048cpu::Place::P1, byte); return true;
                case 2: p2 = byte; bus.write(i8048cpu::Place::P2, byte); return true;
                default: bus.write(static_cast<i8048cpu::Place>(static_cast<unsigned>(i8048cpu::Place::P4) + (addr & 3)), byte); return true;
            }
        }
        return false;
    });
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t& result) {
        if (name == "getT0") result = bus.read(i8048cpu::Place::T0), condition = result != 0;
        else if (name == "getT1") result = bus.read(i8048cpu::Place::T1), condition = result != 0;
        else if (name == "getExtInt") result = bus.read(i8048cpu::Place::Int), condition = result != 0;
        else if (name == "getTF") {   // (JTF: the flag is read and cleared; the jump is taken exactly when it was set)
            result = timer_flag ? 1 : 0;
            condition = timer_flag;
            timer_flag = false;
        } else if (name == "getTmr") result = timer;
        else if (name == "setTmr" && !args.empty()) {
            timer = static_cast<std::uint8_t>(args[0]);
            timer_flag = timer == 0xFF;
        } else if (name == "enableExtInt") ext_int = true;
        else if (name == "disableExtInt") ext_int = false;
        else if (name == "enableTCntInt") timer_int = true;
        else if (name == "disableTCntInt") timer_int = false;
        else if (name == "startTimer") timer_on = true;
        else if (name == "startEventCounter") counter_on = true;
        else if (name == "stopTimerAndEventCounter") timer_on = counter_on = false;
        else if (name != "nop" && name != "enableClockOutput" && name != "setBank") return false;
        return true;
    });
    // an instruction the own core has and the description lacks: IN A,DBB (the 8041's bus, 0x22)
    emu.onBadInstruction([&](sl::Emulator& self, std::uint64_t at) {
        if (program[at & kI8048Mask] != 0x22) return false;
        self.setReg(reg[i8048_reg::A], bus.read(i8048cpu::Place::Dbb));
        self.setPc(at + 1);
        return true;
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

    // the registers: R0-R7 as given, the stack holding the return address (one level in), as a CALL leaves it
    for (std::size_t k = 0; k < 8; ++k) {
        emu.setReg(reg[i8048_reg::R0 + k], options.r[k]);
        ram[k] = options.r[k];
    }
    ram[8] = static_cast<std::uint8_t>(i8048cpu::kReturnAddress);
    ram[9] = static_cast<std::uint8_t>(i8048cpu::kReturnAddress >> 8);
    emu.setReg(reg[i8048_reg::A], options.a);
    emu.setReg(reg[i8048_reg::SP], 8 + 2 * 1);
    for (const std::size_t flag : {i8048_reg::C, i8048_reg::AC, i8048_reg::F0, i8048_reg::F1, i8048_reg::BS}) emu.setReg(reg[flag], 0);
    emu.setPc(entry);

    const auto level = [&] { return static_cast<std::uint8_t>(((emu.reg(reg[i8048_reg::SP]) - 8) >> 1) & 7); };   // the stack pointer as the own core counts it
    const auto snapshot = [&] {
        i8048cpu::Registers g;
        g.a = static_cast<std::uint8_t>(emu.reg(reg[i8048_reg::A]));
        g.pc = static_cast<std::uint16_t>(emu.pc());
        g.c = emu.reg(reg[i8048_reg::C]) != 0;
        g.ac = emu.reg(reg[i8048_reg::AC]) != 0;
        g.f0 = emu.reg(reg[i8048_reg::F0]) != 0;
        g.f1 = emu.reg(reg[i8048_reg::F1]) != 0;
        g.rb1 = bank != 0;
        g.mb1 = mb1;
        g.sp = level();
        g.timer = timer;
        g.timer_flag = timer_flag;
        g.timer_on = timer_on;
        g.counter_on = counter_on;
        g.ext_int = ext_int;
        g.timer_int = timer_int;
        g.p1 = p1;
        g.p2 = p2;
        g.ram = ram;
        for (std::size_t k = 0; k < 8; ++k) g.ram[(bank ? 24 : 0) + k] = static_cast<std::uint8_t>(emu.reg(reg[i8048_reg::R0 + k]));
        return g;
    };

    i8048cpu::RunResult out;
    out.end = i8048cpu::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
        if (pc == i8048cpu::kReturnAddress) {
            out.end = i8048cpu::End::Return;
            break;
        }
        if (options.on_instruction) options.on_instruction(snapshot());
        const std::uint8_t op = program[pc & kI8048Mask], operand = program[(pc + 1) & kI8048Mask];
        current = op;
        condition.reset();
        // what the instruction starts from, for what the adapter sets after it
        const std::uint8_t a = static_cast<std::uint8_t>(emu.reg(reg[i8048_reg::A]));
        const bool c = emu.reg(reg[i8048_reg::C]) != 0, ac = emu.reg(reg[i8048_reg::AC]) != 0;
        if (op == 0xD3) {   // XRL A,#data: the description has it, the own core does not (its table lacks it: the opcode stops the run)
            out.end = i8048cpu::End::Stopped;
            out.fault = i8048cpu::Fault{pc, std::format("undefined opcode {:02X} at {:04X}", op, pc)};
            break;
        }
        // (a RET at the bottom of the stack takes the entry the own core's wrapped pointer names: the description's pointer does not wrap)
        if ((op == 0x83 || op == 0x93) && emu.reg(reg[i8048_reg::SP]) == 8) emu.setReg(reg[i8048_reg::SP], 24);
        // XCHD: the memory the register points at, as it was
        const unsigned indirect = (op & 0xFE) == 0x30 ? static_cast<unsigned>(emu.reg(reg[i8048_reg::R0 + (op & 1)]) & 63) : 0;
        const std::uint8_t exchanged = (op & 0xFE) == 0x30 ? internal_read(indirect) : 0;
        const sl::Emulator::Status status = emu.step();
        if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
            out.end = i8048cpu::End::Stopped;
            out.fault = i8048cpu::Fault{pc, std::format("undefined opcode {:02X} at {:04X}", op, pc)};
            break;
        }
        ++steps;
        std::uint16_t next = static_cast<std::uint16_t>(emu.pc() & kI8048Mask);
        if ((op & 0x1F) == 0x14 && emu.reg(reg[i8048_reg::SP]) >= 24) emu.setReg(reg[i8048_reg::SP], emu.reg(reg[i8048_reg::SP]) - 16);   // (a CALL wraps the stack's eight levels)
        if ((op & 0xFE) == 0x30) {   // XCHD exchanges the low digits of A and of the memory (the description's exchange of two bit ranges leaves both as they were)
            emu.setReg(reg[i8048_reg::A], (a & 0xF0) | (exchanged & 0xF));
            internal_write(indirect, static_cast<std::uint8_t>((exchanged & 0xF0) | (a & 0xF)));
        } else if (op == 0x57) {
            const I8048Adjusted adjusted = i8048_decimal_adjust(a, ac, c);
            emu.setReg(reg[i8048_reg::A], adjusted.a);
            emu.setReg(reg[i8048_reg::C], adjusted.c ? 1 : 0);
        } else if (op == 0xC7) {
            emu.setReg(reg[i8048_reg::A], static_cast<std::uint8_t>((c ? 0x80 : 0) | (ac ? 0x40 : 0) | (emu.reg(reg[i8048_reg::F0]) ? 0x20 : 0) | (bank ? 0x10 : 0) | 0x08 | level()));
        } else if ((pc & 0xFF) == 0xFF && op == 0xA3) {   // MOVP in the last byte of a page reads that page (the description's is the next)
            emu.setReg(reg[i8048_reg::A], program[(pc & 0xF00) | a]);
        } else if ((pc & 0xFF) == 0xFF && op == 0xB3) {   // JMPP likewise
            next = static_cast<std::uint16_t>((pc & 0xF00) | program[(pc & 0xF00) | a]);
        } else if ((pc & 0xFF) == 0xFE && i8048_page_jump(op)) {   // a jump in the last two bytes of a page goes to the page of its second byte (the description's is the next page)
            bool taken = false;
            if ((op & 0x1F) == 0x12) taken = ((a >> (op >> 5)) & 1) != 0;
            else if ((op & 0xF8) == 0xE8) taken = emu.reg(reg[i8048_reg::R0 + (op & 7)]) != 0;
            else switch (op) {
                case 0x16: taken = condition.value_or(false); break;                        // JTF
                case 0x26: case 0x46: case 0x86: taken = !condition.value_or(true); break;   // JNT0, JNT1, JNI
                case 0x36: case 0x56: taken = condition.value_or(false); break;              // JT0, JT1
                case 0x76: taken = emu.reg(reg[i8048_reg::F1]) != 0; break;
                case 0x96: taken = a != 0; break;
                case 0xB6: taken = emu.reg(reg[i8048_reg::F0]) != 0; break;
                case 0xC6: taken = a == 0; break;
                case 0xE6: taken = !c; break;
                default: taken = c; break;   // JC
            }
            next = taken ? static_cast<std::uint16_t>(((pc + 1) & 0xF00) | operand) : static_cast<std::uint16_t>((pc + 2) & kI8048Mask);
        } else if (op == 0xE5 || op == 0xF5) {   // SEL MB0 / MB1: the next JMP and CALL are decoded in the bank chosen
            mb1 = op == 0xF5;
            emu.setContext("DBF", mb1 ? 1 : 0);
            emu.flushDecoded();
        }
        if (next != (emu.pc() & 0xFFFF)) emu.setPc(next);
        // a change of bank (SEL RB, MOV PSW,A, RETR): the registers of the bank left go to the RAM, those of the bank chosen come from it
        const std::uint8_t now = emu.reg(reg[i8048_reg::BS]) != 0 ? 1 : 0;
        if (now != bank) {
            for (std::size_t k = 0; k < 8; ++k) {
                ram[(bank ? 24 : 0) + k] = static_cast<std::uint8_t>(emu.reg(reg[i8048_reg::R0 + k]));
                emu.setReg(reg[i8048_reg::R0 + k], ram[(now ? 24 : 0) + k]);
            }
            bank = now;
        }
    }
    if (out.end == i8048cpu::End::Limit && emu.pc() == i8048cpu::kReturnAddress) out.end = i8048cpu::End::Return;
    out.regs = snapshot();
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

i8048cpu::RunResult call(i8048cpu::Bus& bus, std::uint16_t entry, const i8048cpu::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : i8048cpu::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
