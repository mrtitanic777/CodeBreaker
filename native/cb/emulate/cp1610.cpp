// cb-unity: alone
#include "cb/emulate/cp1610.hpp"

#include <format>

namespace cb::emulate::cp1610 {

namespace dc = decompile::cp1610;

// ---- the bus --------------------------------------------------------------------------------------------------------

void Bus::memory(std::uint16_t lo, std::uint16_t hi, std::span<const std::uint16_t> initial, bool writable, bool logged, std::uint16_t mask) {
    Range r{lo, hi, std::vector<std::uint16_t>(static_cast<std::size_t>(hi) - lo + 1, 0), true, writable, logged, mask, {}, {}};
    std::copy(initial.begin(), initial.begin() + static_cast<std::ptrdiff_t>(std::min(initial.size(), r.data.size())), r.data.begin());
    ranges_.push_back(std::move(r));
}

void Bus::device(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write, bool logged, std::uint16_t mask) {
    ranges_.push_back({lo, hi, {}, false, true, logged, mask, std::move(read), std::move(write)});
}

std::uint16_t Bus::read(std::uint16_t address) {
    for (Range& r : ranges_) {
        if (address < r.lo || address > r.hi) continue;
        const std::uint16_t v = r.is_memory ? r.data[address - r.lo] : (r.read ? r.read(address) : 0);
        if (r.logged) log_.push_back({DeviceAccess::Kind::Read, address, v});
        return v;
    }
    return 0;
}

void Bus::write(std::uint16_t address, std::uint16_t value) {
    for (Range& r : ranges_) {
        if (address < r.lo || address > r.hi) continue;
        value &= r.mask;
        if (r.logged) log_.push_back({DeviceAccess::Kind::Write, address, value});
        if (r.is_memory) {
            if (r.writable) r.data[address - r.lo] = value;
        } else if (r.write) {
            r.write(address, value);
        }
        return;
    }
}

std::span<std::uint16_t> Bus::words(std::uint16_t lo) {
    for (Range& r : ranges_) {
        if (r.lo == lo && r.is_memory) return r.data;
    }
    return {};
}

// ---- the CPU --------------------------------------------------------------------------------------------------------

namespace {

struct Exec {
    Registers& g;
    Bus& bus;
    std::uint32_t pc;     // the instruction's address
    std::uint32_t next;   // where the next instruction is: written by a write to R7

    std::uint16_t get(unsigned n) const { return n == 7 ? static_cast<std::uint16_t>(pc + 1) : g.r[n]; }
    void put(unsigned n, std::uint16_t v) {
        if (n == 7) next = v;
        else g.r[n] = v;
    }
    void sz(std::uint16_t v) {
        g.s = (v & 0x8000) != 0;
        g.z = v == 0;
    }
    std::uint16_t add(std::uint16_t a, std::uint16_t b, unsigned carry = 0) {
        const std::uint32_t wide = static_cast<std::uint32_t>(a) + b + carry;
        const std::uint16_t res = static_cast<std::uint16_t>(wide);
        sz(res);
        g.ov = (((a ^ res) & (b ^ res)) & 0x8000) != 0;
        g.c = wide > 0xFFFF;
        return res;
    }
    std::uint16_t sub(std::uint16_t a, std::uint16_t b) {   // a - b by a + ~b + 1: C is set when there is no borrow
        const std::uint16_t nb = static_cast<std::uint16_t>(~b);
        return add(a, nb, 1);
    }

    // The value a memory instruction reads: direct, indirect (with its side effect), or immediate.
    std::uint16_t operand(const dc::Insn& i, bool sdbd) {
        switch (i.access) {
            case dc::Access::Direct: return bus.read(static_cast<std::uint16_t>(i.value));
            case dc::Access::Immediate: return static_cast<std::uint16_t>(i.value);
            default: break;
        }
        const unsigned p = i.r1;
        if (p == 6) {
            g.r[6] = static_cast<std::uint16_t>(g.r[6] - 1);
            return bus.read(g.r[6]);
        }
        const std::uint16_t a = g.r[p];
        std::uint16_t v = bus.read(a);
        unsigned step = 1;
        if (sdbd) {
            v = static_cast<std::uint16_t>((v & 0xFF) | (bus.read(static_cast<std::uint16_t>(a + 1)) & 0xFF) << 8);
            step = 2;
        }
        if (p == 4 || p == 5) g.r[p] = static_cast<std::uint16_t>(g.r[p] + step);
        return v;
    }

    bool condition(unsigned c) const {
        switch (c & 15) {
            case 0: return true;
            case 1: return g.c;
            case 2: return g.ov;
            case 3: return !g.s;
            case 4: return g.z;
            case 5: return g.s != g.ov;
            case 6: return g.z || g.s != g.ov;
            case 7: return g.c != g.s;
            case 8: return false;
            case 9: return !g.c;
            case 10: return !g.ov;
            case 11: return g.s;
            case 12: return !g.z;
            case 13: return g.s == g.ov;
            case 14: return !(g.z || g.s != g.ov);
            default: return g.c == g.s;
        }
    }

    // A shift or rotate of `v` by `n`; op is the decompile Op.
    std::uint16_t shift(dc::Op op, std::uint16_t v, unsigned n) {
        switch (op) {
            case dc::Op::Swap: {
                const std::uint16_t res = n == 2 ? static_cast<std::uint16_t>((v & 0xFF) * 0x101) : static_cast<std::uint16_t>(v << 8 | v >> 8);
                g.s = (res & 0x80) != 0;
                g.z = (res & 0xFF) == 0;
                return res;
            }
            case dc::Op::Sll: { const std::uint16_t res = static_cast<std::uint16_t>(v << n); sz(res); return res; }
            case dc::Op::Slr: { const std::uint16_t res = static_cast<std::uint16_t>(v >> n); sz(res); return res; }
            case dc::Op::Sar: { const std::uint16_t res = static_cast<std::uint16_t>(static_cast<std::int16_t>(v) >> n); sz(res); return res; }
            case dc::Op::Sllc: {
                const bool carry = ((v >> (16 - n)) & 1) != 0;
                const std::uint16_t res = static_cast<std::uint16_t>(v << n);
                g.ov = ((v ^ res) & 0x8000) != 0;
                g.c = carry;
                sz(res);
                return res;
            }
            case dc::Op::Sarc: {
                g.c = ((v >> (n - 1)) & 1) != 0;
                const std::uint16_t res = static_cast<std::uint16_t>(static_cast<std::int16_t>(v) >> n);
                sz(res);
                return res;
            }
            case dc::Op::Rlc: {
                for (unsigned k = 0; k < n; ++k) {
                    const bool out = (v & 0x8000) != 0;
                    v = static_cast<std::uint16_t>(v << 1 | (g.c ? 1 : 0));
                    g.c = out;
                }
                sz(v);
                return v;
            }
            default: {   // Rrc
                for (unsigned k = 0; k < n; ++k) {
                    const bool out = (v & 1) != 0;
                    v = static_cast<std::uint16_t>(v >> 1 | (g.c ? 0x8000 : 0));
                    g.c = out;
                }
                sz(v);
                return v;
            }
        }
    }
};

std::string at(std::uint32_t pc) { return std::format(" at {:04X}", pc); }

}  // namespace

std::optional<Fault> Cpu::step() {
    Registers& g = regs;
    const std::uint16_t pc = g.r[7];
    std::array<std::uint16_t, 3> buf{};
    for (unsigned k = 0; k < 3; ++k) buf[k] = bus_->read(static_cast<std::uint16_t>(pc + k));
    const std::optional<dc::Insn> decoded = dc::decode(buf, 0, pc, sdbd);
    if (!decoded) return Fault{pc, "undefined instruction" + at(pc)};
    const dc::Insn& i = *decoded;
    if (i.op == dc::Op::Hlt) return Fault{pc, "halt" + at(pc)};
    Exec x{g, *bus_, pc, static_cast<std::uint32_t>(pc + i.size)};
    const bool was_sdbd = sdbd;
    sdbd = false;
    switch (i.op) {
        case dc::Op::Sdbd: sdbd = true; break;
        case dc::Op::Eis: g.i = true; break;
        case dc::Op::Dis: g.i = false; break;
        case dc::Op::Clrc: g.c = false; break;
        case dc::Op::Setc: g.c = true; break;
        case dc::Op::Tci: case dc::Op::Nop: case dc::Op::Sin: break;
        case dc::Op::Incr: { const std::uint16_t v = static_cast<std::uint16_t>(x.get(i.r2) + 1); x.sz(v); x.put(i.r2, v); break; }
        case dc::Op::Decr: { const std::uint16_t v = static_cast<std::uint16_t>(x.get(i.r2) - 1); x.sz(v); x.put(i.r2, v); break; }
        case dc::Op::Comr: { const std::uint16_t v = static_cast<std::uint16_t>(~x.get(i.r2)); x.sz(v); x.put(i.r2, v); break; }
        case dc::Op::Negr: { const std::uint16_t v = x.sub(0, x.get(i.r2)); x.put(i.r2, v); break; }
        case dc::Op::Adcr: { const std::uint16_t v = x.add(x.get(i.r2), 0, g.c ? 1 : 0); x.put(i.r2, v); break; }
        case dc::Op::Gswd: {
            const std::uint16_t f = static_cast<std::uint16_t>((g.s ? 0x80 : 0) | (g.z ? 0x40 : 0) | (g.ov ? 0x20 : 0) | (g.c ? 0x10 : 0));
            x.put(i.r2, static_cast<std::uint16_t>(f | f << 8));
            break;
        }
        case dc::Op::Rswd: {
            const std::uint16_t v = x.get(i.r2);
            g.s = (v & 0x80) != 0;
            g.z = (v & 0x40) != 0;
            g.ov = (v & 0x20) != 0;
            g.c = (v & 0x10) != 0;
            break;
        }
        case dc::Op::Swap: case dc::Op::Sll: case dc::Op::Rlc: case dc::Op::Sllc: case dc::Op::Slr: case dc::Op::Sar: case dc::Op::Rrc: case dc::Op::Sarc:
            g.r[i.r2] = x.shift(i.op, g.r[i.r2], i.cond);
            break;
        case dc::Op::Movr: { const std::uint16_t v = x.get(i.r1); x.sz(v); x.put(i.r2, v); break; }
        case dc::Op::Addr: x.put(i.r2, x.add(x.get(i.r2), x.get(i.r1))); break;
        case dc::Op::Subr: x.put(i.r2, x.sub(x.get(i.r2), x.get(i.r1))); break;
        case dc::Op::Cmpr: (void)x.sub(x.get(i.r2), x.get(i.r1)); break;
        case dc::Op::Andr: { const std::uint16_t v = x.get(i.r2) & x.get(i.r1); x.sz(v); x.put(i.r2, v); break; }
        case dc::Op::Xorr: { const std::uint16_t v = x.get(i.r2) ^ x.get(i.r1); x.sz(v); x.put(i.r2, v); break; }
        case dc::Op::Branch:
        case dc::Op::Bext:
            if (i.op == dc::Op::Bext ? (i.cond & 8) != 0 : x.condition(i.cond)) x.next = *i.target;
            break;
        case dc::Op::Jump:
            if (i.cond != 7) g.r[i.cond] = static_cast<std::uint16_t>(pc + 3);
            if (i.flag == 1) g.i = true;
            if (i.flag == 2) g.i = false;
            x.next = i.value;
            break;
        case dc::Op::Mvo: {
            const std::uint16_t v = x.get(i.r2);
            if (i.access == dc::Access::Direct) {
                bus_->write(static_cast<std::uint16_t>(i.value), v);
            } else {
                const unsigned p = i.r1;
                bus_->write(g.r[p], v);
                if (p == 4 || p == 5 || p == 6) g.r[p] = static_cast<std::uint16_t>(g.r[p] + 1);
            }
            break;
        }
        default: {   // Mvi Add Sub Cmp And Xor
            const std::uint16_t src = x.operand(i, was_sdbd);
            const std::uint16_t dst = x.get(i.r2);
            switch (i.op) {
                case dc::Op::Mvi: x.sz(src); x.put(i.r2, src); break;
                case dc::Op::Add: x.put(i.r2, x.add(dst, src)); break;
                case dc::Op::Sub: x.put(i.r2, x.sub(dst, src)); break;
                case dc::Op::Cmp: (void)x.sub(dst, src); break;
                case dc::Op::And: { const std::uint16_t v = dst & src; x.sz(v); x.put(i.r2, v); break; }
                default: { const std::uint16_t v = dst ^ src; x.sz(v); x.put(i.r2, v); break; }
            }
            break;
        }
    }
    g.r[7] = static_cast<std::uint16_t>(x.next);
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
    for (std::size_t k = 0; k < options.r.size(); ++k) cpu.regs.r[k] = options.r[k];
    cpu.regs.r[5] = kReturnAddress;
    bus.write(options.sp, kReturnAddress);
    cpu.regs.r[6] = static_cast<std::uint16_t>(options.sp + 1);
    cpu.regs.r[7] = entry;
    const std::size_t logged_before = bus.log().size();
    RunResult result;
    while (cpu.steps < options.max_steps) {
        if (cpu.regs.r[7] == kReturnAddress) {
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
    if (result.end == End::Limit && cpu.regs.r[7] == kReturnAddress) result.end = End::Return;
    result.regs = cpu.regs;
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(logged_before), bus.log().end());
    return result;
}

}  // namespace cb::emulate::cp1610
