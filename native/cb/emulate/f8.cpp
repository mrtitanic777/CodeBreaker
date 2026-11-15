// cb-unity: alone
#include "cb/emulate/f8.hpp"

#include <format>

#include "cb/core/error.hpp"

namespace cb::emulate::f8 {

namespace df = decompile::f8;

void Bus::load(std::uint16_t address, ByteView bytes, bool writable) {
    if (static_cast<std::size_t>(address) + bytes.size() > memory_.size()) throw UsageError("an F8 program is at most 64 KB");
    std::copy(bytes.begin(), bytes.end(), memory_.begin() + address);
    if (writable) make_writable(address, static_cast<std::uint16_t>(address + bytes.size() - 1));
}

void Bus::make_writable(std::uint16_t lo, std::uint16_t hi) {
    for (std::uint32_t a = lo; a <= hi; ++a) writable_[a] = true;
}

std::uint8_t Bus::port_in(std::uint8_t port) {
    const std::uint8_t v = in_ ? in_(port) : 0;
    log_.push_back({PortAccess::Kind::In, port, v});
    return v;
}

void Bus::port_out(std::uint8_t port, std::uint8_t value) {
    log_.push_back({PortAccess::Kind::Out, port, value});
    if (out_) out_(port, value);
}

namespace {

struct Exec {
    Registers& g;

    void flags(bool z, bool s_positive, bool c, bool o) { g.status = static_cast<std::uint8_t>((g.status & w::ICB) | (s_positive ? w::S : 0) | (c ? w::C : 0) | (z ? w::Z : 0) | (o ? w::O : 0)); }
    void logic(std::uint8_t v) { flags(v == 0, (v & 0x80) == 0, false, false); }
    std::uint8_t add(std::uint8_t a, std::uint8_t b, unsigned carry = 0) {
        const unsigned sum = a + b + carry;
        const std::uint8_t res = static_cast<std::uint8_t>(sum);
        flags(res == 0, (res & 0x80) == 0, sum > 0xFF, (((a ^ res) & (b ^ res)) & 0x80) != 0);
        return res;
    }
    // x - a as x + ~a + 1 (the flags of CI and CM)
    void compare(std::uint8_t x, std::uint8_t a) { (void)add(x, static_cast<std::uint8_t>(~a), 1); }
    std::uint8_t add_decimal(std::uint8_t a, std::uint8_t b) {
        const std::uint8_t biased = static_cast<std::uint8_t>(a + 0x66);
        const unsigned sum = biased + b;
        const bool carry = a + 0x66 + b > 0xFF || a + 0x66 > 0xFF;
        const bool low_carry = ((biased & 0xF) + (b & 0xF)) > 0xF;
        std::uint8_t res = static_cast<std::uint8_t>(sum);
        if (!low_carry) res = static_cast<std::uint8_t>((res & 0xF0) | ((res - 6) & 0x0F));
        if (!carry) res = static_cast<std::uint8_t>(((res - 0x60) & 0xF0) | (res & 0x0F));
        flags(res == 0, (res & 0x80) == 0, carry, false);
        return res;
    }
    // scratchpad register code: 0-11 direct, 12 S, 13 I (then ISAR's low three bits up), 14 D (then down)
    std::uint8_t& reg(unsigned r) {
        if (r < 12) return g.scratch[r];
        std::uint8_t& v = g.scratch[g.isar & 63];
        if (r == 13) g.isar = static_cast<std::uint8_t>((g.isar & 0x38) | ((g.isar + 1) & 7));
        if (r == 14) g.isar = static_cast<std::uint8_t>((g.isar & 0x38) | ((g.isar - 1) & 7));
        return v;
    }
    std::uint16_t pair(unsigned hi) const { return static_cast<std::uint16_t>(g.scratch[hi] << 8 | g.scratch[hi + 1]); }
    void set_pair(unsigned hi, std::uint16_t v) {
        g.scratch[hi] = static_cast<std::uint8_t>(v >> 8);
        g.scratch[hi + 1] = static_cast<std::uint8_t>(v);
    }
};

std::string at(std::uint16_t pc) { return std::format(" at {:04X}", pc); }

}  // namespace

std::optional<Fault> Cpu::step() {
    Registers& g = regs;
    const std::uint16_t pc = g.pc0;
    const std::array<std::uint8_t, 3> bytes = {bus_->read(pc), bus_->read(static_cast<std::uint16_t>(pc + 1)), bus_->read(static_cast<std::uint16_t>(pc + 2))};
    const std::optional<df::Insn> decoded = df::decode(bytes, 0, pc);
    if (!decoded) return Fault{pc, std::format("undefined opcode {:02X}", bytes[0]) + at(pc)};
    const df::Insn& i = *decoded;
    const unsigned op = i.op;
    std::uint16_t next = static_cast<std::uint16_t>(pc + i.size);
    Exec x{g};
    const auto branch = [&] { next = static_cast<std::uint16_t>(*i.target); };
    const auto dc_read = [&] {
        const std::uint8_t v = bus_->read(g.dc0);
        ++g.dc0;
        return v;
    };
    if (op < 0x20) {
        switch (op) {
            case 0x00: case 0x01: case 0x02: case 0x03: g.a = g.scratch[12 + op]; break;
            case 0x04: case 0x05: case 0x06: case 0x07: g.scratch[12 + (op - 4)] = g.a; break;
            case 0x08: x.set_pair(12, g.pc1); break;
            case 0x09: g.pc1 = x.pair(12); break;
            case 0x0A: g.a = g.isar; break;
            case 0x0B: g.isar = g.a & 63; break;
            case 0x0C: g.pc1 = next; next = x.pair(12); break;
            case 0x0D: next = x.pair(14); break;
            case 0x0E: x.set_pair(14, g.dc0); break;
            case 0x0F: g.dc0 = x.pair(14); break;
            case 0x10: g.dc0 = x.pair(10); break;
            case 0x11: x.set_pair(10, g.dc0); break;
            case 0x12: g.a >>= 1; x.logic(g.a); break;
            case 0x13: g.a = static_cast<std::uint8_t>(g.a << 1); x.logic(g.a); break;
            case 0x14: g.a >>= 4; x.logic(g.a); break;
            case 0x15: g.a = static_cast<std::uint8_t>(g.a << 4); x.logic(g.a); break;
            case 0x16: g.a = dc_read(); break;
            case 0x17: bus_->write(g.dc0, g.a); ++g.dc0; break;
            case 0x18: g.a = static_cast<std::uint8_t>(~g.a); x.logic(g.a); break;
            case 0x19: g.a = x.add(g.a, 0, (g.status & w::C) ? 1 : 0); break;
            case 0x1A: g.status &= static_cast<std::uint8_t>(~w::ICB); break;
            case 0x1B: g.status |= w::ICB; break;
            case 0x1C: next = g.pc1; break;
            case 0x1D: g.status = g.scratch[9] & 0x1F; break;
            case 0x1E: g.scratch[9] = g.status; break;
            default: g.a = x.add(g.a, 1); break;   // 0x1F INC
        }
    } else if (op < 0x30) {
        switch (op) {
            case 0x20: g.a = static_cast<std::uint8_t>(i.value); break;
            case 0x21: g.a &= static_cast<std::uint8_t>(i.value); x.logic(g.a); break;
            case 0x22: g.a |= static_cast<std::uint8_t>(i.value); x.logic(g.a); break;
            case 0x23: g.a ^= static_cast<std::uint8_t>(i.value); x.logic(g.a); break;
            case 0x24: g.a = x.add(g.a, static_cast<std::uint8_t>(i.value)); break;
            case 0x25: x.compare(static_cast<std::uint8_t>(i.value), g.a); break;
            case 0x26: g.a = bus_->port_in(static_cast<std::uint8_t>(i.value)); x.logic(g.a); break;
            case 0x27: bus_->port_out(static_cast<std::uint8_t>(i.value), g.a); break;
            case 0x28: g.a = static_cast<std::uint8_t>(i.value >> 8); g.pc1 = next; next = static_cast<std::uint16_t>(i.value); break;
            case 0x29: g.a = static_cast<std::uint8_t>(i.value >> 8); next = static_cast<std::uint16_t>(i.value); break;
            case 0x2A: g.dc0 = static_cast<std::uint16_t>(i.value); break;
            case 0x2B: break;
            default: std::swap(g.dc0, g.dc1); break;   // 0x2C XDC
        }
    } else if (op < 0x40) { std::uint8_t& r = x.reg(op & 15); r = x.add(r, 0xFF); }
    else if (op < 0x50) { g.a = x.reg(op & 15); }
    else if (op < 0x60) { x.reg(op & 15) = g.a; }
    else if (op < 0x68) { g.isar = static_cast<std::uint8_t>((g.isar & 7) | (i.value << 3)); }
    else if (op < 0x70) { g.isar = static_cast<std::uint8_t>((g.isar & 0x38) | i.value); }
    else if (op < 0x80) { g.a = static_cast<std::uint8_t>(i.value); }
    else if (op < 0x88) {
        if ((op & 7) & (g.status & 7)) branch();
    } else if (op == 0x8E) {
        g.dc0 = static_cast<std::uint16_t>(g.dc0 + static_cast<std::int8_t>(g.a));   // ADC adds the accumulator, as a signed number, to DC0
    } else if (op < 0x8F) {
        const std::uint8_t m = dc_read();
        switch (op) {
            case 0x88: g.a = x.add(g.a, m); break;
            case 0x89: g.a = x.add_decimal(g.a, m); break;
            case 0x8A: g.a &= m; x.logic(g.a); break;
            case 0x8B: g.a |= m; x.logic(g.a); break;
            case 0x8C: g.a ^= m; x.logic(g.a); break;
            case 0x8D: x.compare(m, g.a); break;
            default: break;
        }
    } else if (op == 0x8F) {
        if ((g.isar & 7) != 7) branch();
    } else if (op < 0xA0) {
        if (op == 0x90 || (((op & 15) & (g.status & 15)) == 0)) branch();
    } else if (op < 0xB0) { g.a = bus_->port_in(static_cast<std::uint8_t>(op & 15)); x.logic(g.a); }
    else if (op < 0xC0) { bus_->port_out(static_cast<std::uint8_t>(op & 15), g.a); }
    else if (op < 0xD0) { g.a = x.add(g.a, x.reg(op & 15)); }
    else if (op < 0xE0) { g.a = x.add_decimal(g.a, x.reg(op & 15)); }
    else if (op < 0xF0) { g.a ^= x.reg(op & 15); x.logic(g.a); }
    else { g.a &= x.reg(op & 15); x.logic(g.a); }
    g.pc0 = next;
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
    for (std::size_t k = 0; k < options.r.size(); ++k) cpu.regs.scratch[k] = options.r[k];
    cpu.regs.pc0 = entry;
    cpu.regs.pc1 = kReturnAddress;
    const std::size_t logged_before = bus.log().size();
    RunResult result;
    while (cpu.steps < options.max_steps) {
        if (cpu.regs.pc0 == kReturnAddress) {
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
    if (result.end == End::Limit && cpu.regs.pc0 == kReturnAddress) result.end = End::Return;
    result.regs = cpu.regs;
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(logged_before), bus.log().end());
    return result;
}

}  // namespace cb::emulate::f8
