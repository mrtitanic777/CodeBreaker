// cb-unity: alone
#include "emulate/i8048.hpp"

#include <format>

#include "core/error.hpp"

namespace cb::emulate::i8048 {

namespace di = decompile::i8048;

// ---- the bus --------------------------------------------------------------------------------------------------------

void Bus::load(std::uint16_t address, ByteView bytes) {
    if (static_cast<std::size_t>(address) + bytes.size() > program_.size()) throw UsageError("an 8048 program is at most 4 KB");
    std::copy(bytes.begin(), bytes.end(), program_.begin() + address);
}

std::uint8_t Bus::read(Place place, std::uint8_t address) {
    const std::uint8_t v = read_ ? read_(place, address) : 0;
    log_.push_back({DeviceAccess::Kind::Read, place, address, v});
    return v;
}

void Bus::write(Place place, std::uint8_t value, std::uint8_t address) {
    log_.push_back({DeviceAccess::Kind::Write, place, address, value});
    if (write_) write_(place, address, value);
}

// ---- the CPU --------------------------------------------------------------------------------------------------------

namespace {

std::string at(std::uint16_t pc) { return std::format(" at {:04X}", pc); }

}  // namespace

std::optional<Fault> Cpu::step() {
    Registers& g = regs;
    const std::uint16_t pc = g.pc;
    const std::array<std::uint8_t, 2> bytes = {bus_->program(pc), bus_->program(static_cast<std::uint16_t>(pc + 1))};
    const std::optional<di::Insn> decoded = di::decode(bytes, 0, pc);
    if (!decoded) return Fault{pc, std::format("undefined opcode {:02X}", bytes[0]) + at(pc)};
    const std::uint8_t op = decoded->op, imm = decoded->operand;
    std::uint16_t next = static_cast<std::uint16_t>((pc + decoded->size) & 0xFFF);
    const auto reg = [&](unsigned n) -> std::uint8_t& { return g.ram[(g.rb1 ? 24 : 0) + n]; };
    const auto indirect = [&](unsigned i) -> std::uint8_t& { return g.ram[reg(i) & 63]; };
    const auto page_jump = [&] { next = static_cast<std::uint16_t>(((pc + 1) & 0xF00) | imm); };
    const auto push = [&](std::uint16_t address) {
        g.ram[8 + 2 * g.sp] = static_cast<std::uint8_t>(address);
        g.ram[9 + 2 * g.sp] = static_cast<std::uint8_t>(((address >> 8) & 0xF) | (g.psw() & 0xF0));
        g.sp = (g.sp + 1) & 7;
    };
    const auto pop = [&](bool restore_psw) {
        g.sp = (g.sp - 1) & 7;
        const std::uint8_t hi = g.ram[9 + 2 * g.sp];
        next = static_cast<std::uint16_t>(g.ram[8 + 2 * g.sp] | (hi & 0xF) << 8);
        if (restore_psw) {
            g.c = (hi & 0x80) != 0;
            g.ac = (hi & 0x40) != 0;
            g.f0 = (hi & 0x20) != 0;
            g.rb1 = (hi & 0x10) != 0;
        }
    };
    // kind: 0 add, 1 addc, 2 orl, 3 anl, 4 xrl
    const auto acc = [&](int kind, std::uint8_t v) {
        switch (kind) {
            case 0:
            case 1: {
                const unsigned cin = kind == 1 && g.c ? 1 : 0;
                const unsigned sum = g.a + v + cin;
                g.ac = ((g.a & 0xF) + (v & 0xF) + cin) > 0xF;
                g.c = sum > 0xFF;
                g.a = static_cast<std::uint8_t>(sum);
                break;
            }
            case 2: g.a |= v; break;
            case 3: g.a &= v; break;
            default: g.a ^= v; break;
        }
    };
    const auto port_of = [](unsigned n) { return static_cast<Place>(static_cast<unsigned>(Place::P4) + n); };

    // the families that share an opcode pattern
    if ((op & 0x1F) == 0x04) {   // JMP
        next = static_cast<std::uint16_t>((g.mb1 ? 0x800 : 0) | (op >> 5) << 8 | imm);
    } else if ((op & 0x1F) == 0x14) {   // CALL
        push(next);
        next = static_cast<std::uint16_t>((g.mb1 ? 0x800 : 0) | (op >> 5) << 8 | imm);
    } else if ((op & 0x1F) == 0x12) {   // JBn
        if ((g.a >> (op >> 5)) & 1) page_jump();
    } else if ((op & 0xF8) == 0x18) { ++reg(op & 7); }
    else if ((op & 0xF8) == 0x28) { std::swap(g.a, reg(op & 7)); }
    else if ((op & 0xF8) == 0x48) { acc(2, reg(op & 7)); }
    else if ((op & 0xF8) == 0x58) { acc(3, reg(op & 7)); }
    else if ((op & 0xF8) == 0x68) { acc(0, reg(op & 7)); }
    else if ((op & 0xF8) == 0x78) { acc(1, reg(op & 7)); }
    else if ((op & 0xF8) == 0xA8) { reg(op & 7) = g.a; }
    else if ((op & 0xF8) == 0xB8) { reg(op & 7) = imm; }
    else if ((op & 0xF8) == 0xC8) { --reg(op & 7); }
    else if ((op & 0xF8) == 0xD8) { acc(4, reg(op & 7)); }
    else if ((op & 0xF8) == 0xE8) {
        if (--reg(op & 7) != 0) page_jump();
    } else if ((op & 0xF8) == 0xF8) { g.a = reg(op & 7); }
    else if ((op & 0xFE) == 0x10) { ++indirect(op & 1); }
    else if ((op & 0xFE) == 0x20) { std::swap(g.a, indirect(op & 1)); }
    else if ((op & 0xFE) == 0x30) {
        std::uint8_t& m = indirect(op & 1);
        const std::uint8_t lo = g.a & 0xF;
        g.a = static_cast<std::uint8_t>((g.a & 0xF0) | (m & 0xF));
        m = static_cast<std::uint8_t>((m & 0xF0) | lo);
    } else if ((op & 0xFE) == 0x40) { acc(2, indirect(op & 1)); }
    else if ((op & 0xFE) == 0x50) { acc(3, indirect(op & 1)); }
    else if ((op & 0xFE) == 0x60) { acc(0, indirect(op & 1)); }
    else if ((op & 0xFE) == 0x70) { acc(1, indirect(op & 1)); }
    else if ((op & 0xFE) == 0x80) { g.a = bus_->read(Place::Data, reg(op & 1)); }
    else if ((op & 0xFE) == 0x90) { bus_->write(Place::Data, g.a, reg(op & 1)); }
    else if ((op & 0xFE) == 0xA0) { indirect(op & 1) = g.a; }
    else if ((op & 0xFE) == 0xB0) { indirect(op & 1) = imm; }
    else if ((op & 0xFE) == 0xD0) { acc(4, indirect(op & 1)); }
    else if ((op & 0xFE) == 0xF0) { g.a = indirect(op & 1); }
    else if ((op & 0xFC) == 0x0C) { g.a = bus_->read(port_of(op & 3)) & 0xF; }
    else if ((op & 0xFC) == 0x3C) { bus_->write(port_of(op & 3), g.a & 0xF); }
    else if ((op & 0xFC) == 0x8C || (op & 0xFC) == 0x9C) { bus_->write(port_of(op & 3), g.a & 0xF); }   // ORLD / ANLD: the operand is written (the expander's latch is not modelled)
    else {
        switch (op) {
            case 0x00: break;
            case 0x02: bus_->write(Place::Bus, g.a); break;
            case 0x03: acc(0, imm); break;
            case 0x05: g.ext_int = true; break;
            case 0x07: --g.a; break;
            case 0x08: g.a = bus_->read(Place::Bus); break;
            case 0x09: g.a = bus_->read(Place::P1) & g.p1; break;
            case 0x0A: g.a = bus_->read(Place::P2) & g.p2; break;
            case 0x13: acc(1, imm); break;
            case 0x15: g.ext_int = false; break;
            case 0x16: if (g.timer_flag) { g.timer_flag = false; page_jump(); } break;
            case 0x17: ++g.a; break;
            case 0x22: g.a = bus_->read(Place::Dbb); break;
            case 0x23: g.a = imm; break;
            case 0x25: g.timer_int = true; break;
            case 0x26: if (bus_->read(Place::T0) == 0) page_jump(); break;
            case 0x27: g.a = 0; break;
            case 0x35: g.timer_int = false; break;
            case 0x36: if (bus_->read(Place::T0) != 0) page_jump(); break;
            case 0x37: g.a = static_cast<std::uint8_t>(~g.a); break;
            case 0x39: g.p1 = g.a; bus_->write(Place::P1, g.a); break;
            case 0x3A: g.p2 = g.a; bus_->write(Place::P2, g.a); break;
            case 0x42: g.a = g.timer; break;
            case 0x43: acc(2, imm); break;
            case 0x45: g.counter_on = true; break;
            case 0x46: if (bus_->read(Place::T1) == 0) page_jump(); break;
            case 0x47: g.a = static_cast<std::uint8_t>(g.a << 4 | g.a >> 4); break;
            case 0x53: acc(3, imm); break;
            case 0x55: g.timer_on = true; break;
            case 0x56: if (bus_->read(Place::T1) != 0) page_jump(); break;
            case 0x57: {
                if ((g.a & 0xF) > 9 || g.ac) {
                    const unsigned sum = g.a + 6;
                    if (sum > 0xFF) g.c = true;
                    g.a = static_cast<std::uint8_t>(sum);
                }
                if ((g.a >> 4) > 9 || g.c) {
                    const unsigned sum = g.a + 0x60;
                    g.c = true;
                    g.a = static_cast<std::uint8_t>(sum);
                }
                break;
            }
            case 0x62: g.timer = g.a; g.timer_flag = g.a == 0xFF; break;
            case 0x65: g.timer_on = false; g.counter_on = false; break;
            case 0x67: {
                const bool out = (g.a & 1) != 0;
                g.a = static_cast<std::uint8_t>((g.a >> 1) | (g.c ? 0x80 : 0));
                g.c = out;
                break;
            }
            case 0x75: break;
            case 0x76: if (g.f1) page_jump(); break;
            case 0x77: g.a = static_cast<std::uint8_t>((g.a >> 1) | (g.a << 7)); break;
            case 0x83: pop(false); break;
            case 0x85: g.f0 = false; break;
            case 0x86: if (bus_->read(Place::Int) == 0) page_jump(); break;
            case 0x88: bus_->write(Place::Bus, imm); break;
            case 0x89: g.p1 |= imm; bus_->write(Place::P1, g.p1); break;
            case 0x8A: g.p2 |= imm; bus_->write(Place::P2, g.p2); break;
            case 0x93: pop(true); break;
            case 0x95: g.f0 = !g.f0; break;
            case 0x96: if (g.a != 0) page_jump(); break;
            case 0x97: g.c = false; break;
            case 0x98: bus_->write(Place::Bus, imm); break;
            case 0x99: g.p1 &= imm; bus_->write(Place::P1, g.p1); break;
            case 0x9A: g.p2 &= imm; bus_->write(Place::P2, g.p2); break;
            case 0xA3: g.a = bus_->program(static_cast<std::uint16_t>((pc & 0xF00) | g.a)); break;
            case 0xA5: g.f1 = false; break;
            case 0xA7: g.c = !g.c; break;
            case 0xB3: next = static_cast<std::uint16_t>((pc & 0xF00) | bus_->program(static_cast<std::uint16_t>((pc & 0xF00) | g.a))); break;
            case 0xB5: g.f1 = !g.f1; break;
            case 0xB6: if (g.f0) page_jump(); break;
            case 0xC5: g.rb1 = false; break;
            case 0xC6: if (g.a == 0) page_jump(); break;
            case 0xC7: g.a = g.psw(); break;
            case 0xD5: g.rb1 = true; break;
            case 0xD7:
                g.c = (g.a & 0x80) != 0;
                g.ac = (g.a & 0x40) != 0;
                g.f0 = (g.a & 0x20) != 0;
                g.rb1 = (g.a & 0x10) != 0;
                g.sp = g.a & 7;
                break;
            case 0xE3: g.a = bus_->program(static_cast<std::uint16_t>(0x300 | g.a)); break;
            case 0xE5: g.mb1 = false; break;
            case 0xE6: if (!g.c) page_jump(); break;
            case 0xE7: g.a = static_cast<std::uint8_t>((g.a << 1) | (g.a >> 7)); break;
            case 0xF5: g.mb1 = true; break;
            case 0xF6: if (g.c) page_jump(); break;
            case 0xF7: {
                const bool out = (g.a & 0x80) != 0;
                g.a = static_cast<std::uint8_t>((g.a << 1) | (g.c ? 1 : 0));
                g.c = out;
                break;
            }
            default: return Fault{pc, std::format("undefined opcode {:02X}", op) + at(pc)};
        }
    }
    g.pc = next & 0xFFF;
    ++steps;
    return std::nullopt;
}

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("?"));
        default: return "limit";
    }
}

RunResult run(Bus& bus, std::uint16_t entry, const RunOptions& options) {
    Cpu cpu(bus);
    cpu.regs.a = options.a;
    for (std::size_t k = 0; k < 8; ++k) cpu.regs.ram[k] = options.r[k];
    cpu.regs.ram[8] = static_cast<std::uint8_t>(kReturnAddress);
    cpu.regs.ram[9] = static_cast<std::uint8_t>(kReturnAddress >> 8);
    cpu.regs.sp = 1;
    cpu.regs.pc = entry;
    const std::size_t logged_before = bus.log().size();
    RunResult result;
    while (cpu.steps < options.max_steps) {
        if (cpu.regs.pc == kReturnAddress) {
            result.end = End::Return;
            break;
        }
        if (options.on_instruction) options.on_instruction(cpu.regs);
        if (const std::optional<Fault> fault = cpu.step()) {
            result.end = End::Stopped;
            result.fault = fault;
            break;
        }
    }
    if (result.end == End::Limit && cpu.regs.pc == kReturnAddress) result.end = End::Return;
    result.regs = cpu.regs;
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(logged_before), bus.log().end());
    return result;
}

}  // namespace cb::emulate::i8048
