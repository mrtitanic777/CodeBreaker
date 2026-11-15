// cb-unity: alone
#include "cb/emulate/tlcs900.hpp"

#include <algorithm>
#include <bit>
#include <format>

#include "cb/core/error.hpp"

namespace cb::emulate::tlcs900 {

namespace dt = decompile::tlcs900;
using dt::Op;
using dt::Tok;

// ---- the bus --------------------------------------------------------------------------------------------------------------------------------------------------------------

void Bus::memory(std::uint32_t lo, std::uint32_t hi, ByteView initial, bool writable, bool logged, std::size_t size) {
    if (size == 0) size = static_cast<std::size_t>(hi) - lo + 1;
    Range r{lo, hi, Bytes(size, 0), writable, logged, {}, {}};
    std::copy(initial.begin(), initial.begin() + static_cast<std::ptrdiff_t>(std::min(initial.size(), size)), r.data.begin());
    ranges_.push_back(std::move(r));
}

void Bus::device(std::uint32_t lo, std::uint32_t hi, ReadFn read, WriteFn write, bool logged) {
    ranges_.push_back({lo, hi, {}, true, logged, std::move(read), std::move(write)});
}

Bus::Range* Bus::find(std::uint32_t address) {
    for (Range& r : ranges_) {
        if (address >= r.lo && address <= r.hi) return &r;
    }
    return nullptr;
}

std::uint32_t Bus::read(std::uint32_t address, unsigned size) {
    const std::uint32_t a = address & 0xFFFFFF;
    Range* r = find(a);
    if (r != nullptr && r->data.empty()) {
        std::uint32_t v = r->read ? r->read(a, size) : 0;
        if (size < 4) v &= (1u << (size * 8)) - 1;
        if (r->logged) log_.push_back({DeviceAccess::Kind::Read, a, static_cast<std::uint8_t>(size), v});
        return v;
    }
    std::uint32_t v = 0;
    for (unsigned k = 0; k < size; ++k) {   // (byte by byte: an access may straddle two ranges)
        Range* byte_range = find((a + k) & 0xFFFFFF);
        if (byte_range == nullptr) continue;
        if (byte_range->data.empty()) {
            const std::uint32_t b = byte_range->read ? byte_range->read((a + k) & 0xFFFFFF, 1) : 0;
            v |= (b & 0xFF) << (8 * k);
            if (byte_range->logged) log_.push_back({DeviceAccess::Kind::Read, (a + k) & 0xFFFFFF, 1, b & 0xFF});
        } else {
            v |= static_cast<std::uint32_t>(byte_range->data[((a + k) - byte_range->lo) % byte_range->data.size()]) << (8 * k);
        }
    }
    if (r != nullptr && r->logged && !r->data.empty()) log_.push_back({DeviceAccess::Kind::Read, a, static_cast<std::uint8_t>(size), v});
    return v;
}

void Bus::write(std::uint32_t address, unsigned size, std::uint32_t value) {
    const std::uint32_t a = address & 0xFFFFFF;
    if (size < 4) value &= (1u << (size * 8)) - 1;
    Range* r = find(a);
    if (r != nullptr && r->data.empty()) {
        if (r->logged) log_.push_back({DeviceAccess::Kind::Write, a, static_cast<std::uint8_t>(size), value});
        if (r->write) r->write(a, size, value);
        return;
    }
    if (r != nullptr && r->logged) log_.push_back({DeviceAccess::Kind::Write, a, static_cast<std::uint8_t>(size), value});
    for (unsigned k = 0; k < size; ++k) {
        Range* byte_range = find((a + k) & 0xFFFFFF);
        if (byte_range == nullptr) continue;
        const std::uint8_t b = static_cast<std::uint8_t>(value >> (8 * k));
        if (byte_range->data.empty()) {
            if (byte_range != r && byte_range->logged) log_.push_back({DeviceAccess::Kind::Write, (a + k) & 0xFFFFFF, 1, b});
            if (byte_range->write) byte_range->write((a + k) & 0xFFFFFF, 1, b);
        } else if (byte_range->writable) {
            byte_range->data[((a + k) - byte_range->lo) % byte_range->data.size()] = b;
        }
    }
}

std::span<std::uint8_t> Bus::bytes(std::uint32_t lo) {
    for (Range& r : ranges_) {
        if (r.lo == lo && !r.data.empty()) return r.data;
    }
    return {};
}

// ---- the registers ------------------------------------------------------------------------------------------------------------------------------------------------------

namespace {

constexpr std::uint32_t mask_of(unsigned size) { return size >= 4 ? 0xFFFFFFFFu : (1u << (8 * size)) - 1; }
constexpr bool negative(std::uint32_t v, unsigned size) { return ((v >> (8 * size - 1)) & 1u) != 0; }
constexpr std::int32_t signed_of(std::uint32_t v, unsigned size) {
    return size == 1 ? static_cast<std::int8_t>(v) : size == 2 ? static_cast<std::int16_t>(v) : static_cast<std::int32_t>(v);
}

struct Slot {
    std::uint32_t* word;
    unsigned pos;
};

Slot slot_of(Registers& g, std::uint8_t code) {
    unsigned bank, group;
    if (code >= 0xE0) {
        bank = g.rfp();
        group = (code >> 2) & 7u;
    } else if (code >= 0xD0) {
        bank = (g.rfp() + 3) & 3u;
        group = (code >> 2) & 3u;
    } else {
        bank = (code >> 4) & 3u;
        group = (code & 0x0Fu) >> 2;
    }
    std::uint32_t* word = group < 4 ? &g.bank[bank][group] : group == 4 ? &g.xix : group == 5 ? &g.xiy : group == 6 ? &g.xiz : &g.xsp;
    return {word, code & 3u};
}

}  // namespace

std::uint32_t Registers::read(std::uint8_t code, unsigned size) const {
    const Slot s = slot_of(const_cast<Registers&>(*this), code);
    return (*s.word >> (8 * s.pos)) & mask_of(size);
}

void Registers::write(std::uint8_t code, unsigned size, std::uint32_t value) {
    const Slot s = slot_of(*this, code);
    const std::uint32_t mask = mask_of(size) << (8 * s.pos);
    *s.word = (*s.word & ~mask) | ((value << (8 * s.pos)) & mask);
}

// ---- the CPU ------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace {

bool condition(unsigned cc, std::uint8_t f) {
    const bool s = (f & flag::S) != 0, z = (f & flag::Z) != 0, v = (f & flag::V) != 0, c = (f & flag::C) != 0;
    switch (cc & 15u) {
        case 0: return false;
        case 1: return s != v;
        case 2: return (s != v) || z;
        case 3: return c || z;
        case 4: return v;
        case 5: return s;
        case 6: return z;
        case 7: return c;
        case 8: return true;
        case 9: return s == v;
        case 10: return !((s != v) || z);
        case 11: return !(c || z);
        case 12: return !v;
        case 13: return !s;
        case 14: return !z;
        default: return !c;
    }
}

unsigned op_size_of(const dt::Insn& i) {
    if (i.z) return i.z;
    const dt::Row& r = *i.row;
    if (r.sizes & dt::kSizeL) return 4;
    if (r.sizes & dt::kSizeW) return 2;
    if (r.sizes & dt::kSizeB) return 1;
    for (const Tok t : {r.a, r.b}) {
        switch (t) {
            case Tok::R8: case Tok::Ib: case Tok::A: case Tok::F: return 1;
            case Tok::R16: case Tok::Iw: case Tok::Sr: return 2;
            case Tok::R32: case Tok::Il: return 4;
            default: break;
        }
    }
    return 1;
}

struct Exec {
    Cpu& cpu;
    Registers& g;
    Bus& bus;
    const dt::Insn& i;
    unsigned sz;
    std::uint32_t ea = 0;
    std::uint32_t next;

    Exec(Cpu& c, Registers& r, Bus& b, const dt::Insn& insn) : cpu(c), g(r), bus(b), i(insn), sz(op_size_of(insn)), next((insn.at + insn.size) & 0xFFFFFF) {}

    // ---- flags
    void set_flag(std::uint8_t bit, bool on) { g.sr = static_cast<std::uint16_t>(on ? (g.sr | bit) : (g.sr & ~bit)); }
    bool flag_on(std::uint8_t bit) const { return (g.sr & bit) != 0; }
    void parity(std::uint32_t v, unsigned size) { set_flag(flag::V, (std::popcount(v & mask_of(size)) & 1) == 0); }
    void sz_flags(std::uint32_t r, unsigned size) {
        set_flag(flag::S, negative(r, size));
        set_flag(flag::Z, (r & mask_of(size)) == 0);
    }

    // ---- the effective address of a memory operand (once, with its pre-decrement / post-increment)
    void eval_ea() {
        const dt::Mem& m = i.m;
        switch (m.kind) {
            case dt::MemKind::Reg: ea = g.read(m.reg, 4); break;
            case dt::MemKind::RegD8:
            case dt::MemKind::RegD16: ea = g.read(m.reg, 4) + static_cast<std::uint32_t>(m.disp); break;
            case dt::MemKind::RegIdx8: ea = g.read(m.reg, 4) + static_cast<std::uint32_t>(signed_of(g.read(m.idx, 1), 1)); break;
            case dt::MemKind::RegIdx16: ea = g.read(m.reg, 4) + static_cast<std::uint32_t>(signed_of(g.read(m.idx, 2), 2)); break;
            case dt::MemKind::Abs8:
            case dt::MemKind::Abs16:
            case dt::MemKind::Abs24:
            case dt::MemKind::PcRel: ea = m.addr; break;
            case dt::MemKind::PreDec:
                ea = g.read(m.reg, 4) - m.step;
                g.write(m.reg, 4, ea);
                break;
            case dt::MemKind::PostInc:
                ea = g.read(m.reg, 4);
                g.write(m.reg, 4, ea + m.step);
                break;
        }
        ea &= 0xFFFFFF;
    }

    static std::uint8_t current_register(unsigned size, unsigned n) {
        return size == 1 ? static_cast<std::uint8_t>(0xE0 + 4 * (n >> 1) + ((n & 1) ^ 1)) : static_cast<std::uint8_t>(0xE0 + 4 * n);
    }
    std::uint8_t low3_code(unsigned size) const { return static_cast<std::uint8_t>(current_register(size, i.op & 7u)); }
    // The register of twice the size that a byte / word code stands for (the destination of mul / div).
    std::uint8_t mul_dest_code() const {
        return i.z == 1 ? static_cast<std::uint8_t>(0xE0 + 4 * ((i.op & 7u) >> 1)) : static_cast<std::uint8_t>(0xE0 + 4 * (i.op & 7u));
    }

    // ---- operands
    std::uint32_t get(Tok t, unsigned slot) {
        switch (t) {
            case Tok::M: return i.row->ctx == dt::Ctx::Reg ? g.read(i.reg, i.z) : bus.read(ea, sz);
            case Tok::Mw: return g.read(i.reg, 2 * i.z);
            case Tok::Rz: return g.read(static_cast<std::uint8_t>(low3_code(i.z)), i.z);
            case Tok::R8: return g.read(static_cast<std::uint8_t>(low3_code(1)), 1);
            case Tok::R16: return g.read(static_cast<std::uint8_t>(low3_code(2)), 2);
            case Tok::R32: return g.read(static_cast<std::uint8_t>(low3_code(4)), 4);
            case Tok::Rmul: return g.read(mul_dest_code(), 2 * i.z);
            case Tok::A: return g.read(0xE0, 1);
            case Tok::Acc: return g.read(0xE0, i.z);
            case Tok::Ib: case Tok::Iw: case Tok::Il: case Tok::Iz: return i.v[slot];
            case Tok::L3: return i.op & 7u;
            case Tok::N3: return (i.op & 7u) ? (i.op & 7u) : 8u;
            case Tok::Bit4: return i.v[slot] & 15u;
            case Tok::Cnt4: return (i.v[slot] & 15u) ? (i.v[slot] & 15u) : 16u;
            case Tok::A8: case Tok::A16: return bus.read(i.v[slot], sz);
            case Tok::Sr: return g.sr;
            case Tok::F: return g.f();
            case Tok::Fp: return g.f_alt;
            default: return 0;
        }
    }

    void put(Tok t, unsigned slot, std::uint32_t v) {
        switch (t) {
            case Tok::M:
                if (i.row->ctx == dt::Ctx::Reg) g.write(i.reg, i.z, v);
                else bus.write(ea, sz, v);
                break;
            case Tok::Mw: g.write(i.reg, 2 * i.z, v); break;
            case Tok::Rz: g.write(low3_code(i.z), i.z, v); break;
            case Tok::R8: g.write(low3_code(1), 1, v); break;
            case Tok::R16: g.write(low3_code(2), 2, v); break;
            case Tok::R32: g.write(low3_code(4), 4, v); break;
            case Tok::Rmul: g.write(mul_dest_code(), 2 * i.z, v); break;
            case Tok::A: g.write(0xE0, 1, v); break;
            case Tok::Acc: g.write(0xE0, i.z, v); break;
            case Tok::A8: case Tok::A16: bus.write(i.v[slot], sz, v); break;
            case Tok::Sr: g.sr = static_cast<std::uint16_t>(v); break;
            case Tok::F: g.sr = static_cast<std::uint16_t>((g.sr & 0xFF00) | (v & 0xFF)); break;
            case Tok::Fp: g.f_alt = static_cast<std::uint8_t>(v); break;
            default: break;
        }
    }

    // ---- the stack
    void push(std::uint32_t v, unsigned size) {
        g.xsp -= size;
        bus.write(g.xsp, size, v);
    }
    std::uint32_t pop(unsigned size) {
        const std::uint32_t v = bus.read(g.xsp, size);
        g.xsp += size;
        return v;
    }

    // ---- arithmetic
    std::uint32_t alu(Op id, std::uint32_t a, std::uint32_t b, unsigned size) {
        const std::uint32_t m = mask_of(size);
        a &= m;
        b &= m;
        const std::uint64_t cy = flag_on(flag::C) ? 1 : 0;
        std::uint32_t r = 0;
        switch (id) {
            case Op::Add:
            case Op::Adc: {
                const std::uint64_t wide = static_cast<std::uint64_t>(a) + b + (id == Op::Adc ? cy : 0);
                r = static_cast<std::uint32_t>(wide) & m;
                set_flag(flag::C, wide > m);
                set_flag(flag::V, negative((r ^ a) & (r ^ b), size));
                set_flag(flag::N, false);
                break;
            }
            case Op::Sub:
            case Op::Sbc:
            case Op::Cp: {
                const std::uint64_t sub = static_cast<std::uint64_t>(b) + (id == Op::Sbc ? cy : 0);
                r = static_cast<std::uint32_t>(a - sub) & m;
                set_flag(flag::C, sub > a);
                set_flag(flag::V, negative((r ^ a) & (a ^ b), size));
                set_flag(flag::N, true);
                break;
            }
            case Op::And:
                r = a & b;
                set_flag(flag::C, false);
                set_flag(flag::N, false);
                set_flag(flag::V, size <= 2 && (std::popcount(r) & 1) == 0);
                break;
            case Op::Or:
            case Op::Xor:
                r = id == Op::Or ? (a | b) : (a ^ b);
                set_flag(flag::C, false);
                set_flag(flag::N, false);
                set_flag(flag::V, size <= 2 && (std::popcount(r) & 1) == 0);
                break;
            default: break;
        }
        sz_flags(r, size);
        if (id == Op::And) {
            set_flag(flag::H, true);
        } else if (id == Op::Or || id == Op::Xor) {
            set_flag(flag::H, false);
        } else {
            set_flag(flag::H, size <= 2 && (((a ^ b) ^ r) & 0x10) != 0);
        }
        return r;
    }

    std::uint32_t shift(Op id, std::uint32_t a, unsigned count, unsigned size) {
        const std::uint32_t m = mask_of(size);
        const unsigned top = 8 * size - 1;
        a &= m;
        bool c = flag_on(flag::C);
        for (unsigned n = 0; n < count; ++n) {
            switch (id) {
                case Op::Rlc: c = (a >> top) & 1u; a = ((a << 1) | (c ? 1u : 0u)) & m; break;
                case Op::Rrc: c = a & 1u; a = (a >> 1) | (c ? (1u << top) : 0u); break;
                case Op::Rl: {
                    const bool out = (a >> top) & 1u;
                    a = ((a << 1) | (c ? 1u : 0u)) & m;
                    c = out;
                    break;
                }
                case Op::Rr: {
                    const bool out = a & 1u;
                    a = (a >> 1) | (c ? (1u << top) : 0u);
                    c = out;
                    break;
                }
                case Op::Sla:
                case Op::Sll: c = (a >> top) & 1u; a = (a << 1) & m; break;
                case Op::Sra: c = a & 1u; a = (a & (1u << top)) | (a >> 1); break;
                default: c = a & 1u; a = a >> 1; break;   // Srl
            }
        }
        set_flag(flag::C, c);
        sz_flags(a, size);
        set_flag(flag::H, false);
        set_flag(flag::N, false);
        parity(a, size);
        return a;
    }

    // ---- the work
    std::optional<Fault> run() {
        const dt::Row& row = *i.row;
        if (row.ctx == dt::Ctx::Src || row.ctx == dt::Ctx::Dst) {
            if (!(row.flags & dt::kBare)) eval_ea();
        }
        const Tok a = row.a, b = row.b;
        const unsigned cc = a == Tok::Cc ? (i.op & 15u) : 8u;
        const unsigned bit_mask = 8 * sz - 1;
        switch (row.id) {
            case Op::Nop: break;
            case Op::Push: push(get(a, 0), sz); break;
            case Op::Pop: put(a, 0, pop(sz)); break;
            case Op::Ei: g.sr = static_cast<std::uint16_t>((g.sr & 0x8FFF) | ((i.v[0] & 7u) << 12)); break;
            case Op::Halt: return Fault{i.at, std::format("halt at {:06X}", i.at)};
            case Op::Reti:
                g.sr = static_cast<std::uint16_t>(pop(2));
                next = pop(4) & 0xFFFFFF;
                break;
            case Op::Ld:
            case Op::Ldx: put(a, 0, get(b, 1)); break;
            case Op::Incf: g.sr = static_cast<std::uint16_t>((g.sr & ~0x0300) | (((g.rfp() + 1) & 3u) << 8)); break;
            case Op::Decf: g.sr = static_cast<std::uint16_t>((g.sr & ~0x0300) | (((g.rfp() + 3) & 3u) << 8)); break;
            case Op::Ret:
                if (condition(cc, g.f())) next = pop(4) & 0xFFFFFF;
                break;
            case Op::Retd:
                next = pop(4) & 0xFFFFFF;
                g.xsp += static_cast<std::uint32_t>(signed_of(i.v[0], 2));
                break;
            case Op::Rcf: g.sr &= static_cast<std::uint16_t>(~(flag::H | flag::N | flag::C)); break;
            case Op::Scf:
                g.sr &= static_cast<std::uint16_t>(~(flag::H | flag::N));
                g.sr |= flag::C;
                break;
            case Op::Ccf:
                g.sr &= static_cast<std::uint16_t>(~flag::N);
                g.sr ^= flag::C;
                break;
            case Op::Zcf:
                g.sr &= static_cast<std::uint16_t>(~(flag::N | flag::C));
                if (!flag_on(flag::Z)) g.sr |= flag::C;
                break;
            case Op::Ex: {
                const std::uint32_t x = get(a, 0), y = get(b, 1);
                put(a, 0, y);
                put(b, 1, x);
                break;
            }
            case Op::Ldf: g.sr = static_cast<std::uint16_t>((g.sr & ~0x0300) | ((i.v[0] & 3u) << 8)); break;
            case Op::Jp:
                if (a == Tok::T16 || a == Tok::T24) next = i.v[0];
                else if (condition(cc, g.f())) next = ea;
                break;
            case Op::Call:
                if (a == Tok::T16 || a == Tok::T24) {
                    push(next, 4);
                    next = i.v[0];
                } else if (condition(cc, g.f())) {
                    push(next, 4);
                    next = ea;
                }
                break;
            case Op::Calr:
                push(next, 4);
                next = i.v[0];
                break;
            case Op::Jr:
            case Op::Jrl:
                if (condition(cc, g.f())) next = i.v[1];
                break;
            case Op::Swi: {
                SoftwareInterrupt call;
                call.number = static_cast<std::uint8_t>(i.op & 7u);
                call.pc = i.at;
                call.function = static_cast<std::uint8_t>(g.read(0x31, 1));   // RW3
                call.regs = {g.bank[g.rfp()][0], g.bank[g.rfp()][1], g.bank[g.rfp()][2], g.bank[g.rfp()][3]};
                cpu.swis.push_back(call);
                if (cpu.on_swi) cpu.on_swi(g, call);
                break;
            }
            case Op::Lda: put(a, 0, ea); break;
            case Op::Link:
                push(get(a, 0), 4);
                put(a, 0, g.xsp);
                g.xsp += static_cast<std::uint32_t>(signed_of(i.v[1], 2));
                break;
            case Op::Unlk:
                g.xsp = get(a, 0);
                put(a, 0, pop(4));
                break;
            case Op::Cpl:
                put(a, 0, ~get(a, 0));
                set_flag(flag::H, true);
                set_flag(flag::N, true);
                break;
            case Op::Neg: put(a, 0, alu(Op::Sub, 0, get(a, 0), sz)); break;
            case Op::Mul:
            case Op::Muls: {
                const Tok dest = a == Tok::Mw || a == Tok::Rmul ? a : Tok::None;
                const unsigned half = i.z;
                const std::uint32_t x = get(dest, 0) & mask_of(half), y = get(b, 1) & mask_of(half);
                const std::int64_t wide = row.id == Op::Mul ? static_cast<std::int64_t>(static_cast<std::uint64_t>(x) * y)
                                                            : static_cast<std::int64_t>(signed_of(x, half)) * signed_of(y, half);
                put(dest, 0, static_cast<std::uint32_t>(wide));
                break;
            }
            case Op::Div:
            case Op::Divs: {
                const Tok dest = a == Tok::Mw || a == Tok::Rmul ? a : Tok::None;
                const unsigned half = i.z, full = 2 * half;
                const std::uint32_t d = get(dest, 0), y = get(b, 1) & mask_of(half);
                const std::uint32_t m = mask_of(half);
                std::uint32_t result;
                if (y == 0) {
                    set_flag(flag::V, true);
                    result = static_cast<std::uint32_t>((static_cast<std::uint64_t>(d) << (8 * half)) | (((d >> (8 * half)) ^ m) & m)) & mask_of(full);
                } else if (row.id == Op::Div) {
                    const std::uint64_t q = static_cast<std::uint64_t>(d) / y, rem = static_cast<std::uint64_t>(d) % y;
                    set_flag(flag::V, q > m);
                    result = static_cast<std::uint32_t>((q & m) | ((rem & m) << (8 * half)));
                } else {
                    const std::int64_t dd = signed_of(d, full), ys = signed_of(y, half);
                    const std::int64_t q = dd / ys, rem = dd % ys;
                    set_flag(flag::V, q > static_cast<std::int64_t>(m) || q < -static_cast<std::int64_t>(m) - 1);
                    result = static_cast<std::uint32_t>((static_cast<std::uint64_t>(q) & m) | ((static_cast<std::uint64_t>(rem) & m) << (8 * half)));
                }
                put(dest, 0, result);
                break;
            }
            case Op::Daa: daa(); break;
            case Op::Extz: put(a, 0, get(a, 0) & mask_of(sz / 2)); break;
            case Op::Exts: put(a, 0, static_cast<std::uint32_t>(signed_of(get(a, 0), sz / 2))); break;
            case Op::Paa:
                if (get(a, 0) & 1u) put(a, 0, get(a, 0) + 1);
                break;
            case Op::Mirr: {
                std::uint32_t r = get(a, 0), s = 0;
                for (unsigned k = 0; k < 16; ++k) s |= ((r >> k) & 1u) << (15 - k);
                put(a, 0, s);
                break;
            }
            case Op::Mula: {
                const std::uint8_t code = i.reg;
                const std::int32_t p = signed_of(bus.read(g.xde(), 2), 2) * signed_of(bus.read(g.xhl(), 2), 2);
                const std::uint32_t r = g.read(code, 4) + static_cast<std::uint32_t>(p);
                g.write(code, 4, r);
                g.xhl() -= 2;
                set_flag(flag::S, negative(r, 4));
                set_flag(flag::Z, r == 0);
                set_flag(flag::V, false);
                break;
            }
            case Op::Djnz: {
                const std::uint32_t v = (get(a, 0) - 1) & mask_of(sz);
                put(a, 0, v);
                if (v != 0) next = i.v[1];
                break;
            }
            case Op::Andcf:
            case Op::Orcf:
            case Op::Xorcf:
            case Op::Ldcf: {
                const unsigned n = (a == Tok::A ? g.read(0xE0, 1) : get(a, 0)) & bit_mask;
                const bool on = (get(Tok::M, 1) >> n) & 1u;
                if (row.id == Op::Andcf) set_flag(flag::C, on && flag_on(flag::C));
                else if (row.id == Op::Orcf) set_flag(flag::C, on || flag_on(flag::C));
                else if (row.id == Op::Xorcf) set_flag(flag::C, on != flag_on(flag::C));
                else set_flag(flag::C, on);
                break;
            }
            case Op::Stcf: {
                const unsigned n = (a == Tok::A ? g.read(0xE0, 1) : get(a, 0)) & bit_mask;
                const std::uint32_t v = get(Tok::M, 1);
                put(Tok::M, 1, flag_on(flag::C) ? (v | (1u << n)) : (v & ~(1u << n)));
                break;
            }
            case Op::Res:
            case Op::Set:
            case Op::Chg:
            case Op::Bit:
            case Op::Tset: {
                const unsigned n = get(a, 0) & bit_mask;
                const std::uint32_t v = get(Tok::M, 1);
                if (row.id == Op::Res) {
                    put(Tok::M, 1, v & ~(1u << n));
                } else if (row.id == Op::Set) {
                    put(Tok::M, 1, v | (1u << n));
                } else if (row.id == Op::Chg) {
                    put(Tok::M, 1, v ^ (1u << n));
                } else {
                    set_flag(flag::Z, ((v >> n) & 1u) == 0);
                    set_flag(flag::H, true);
                    set_flag(flag::N, false);
                    if (row.id == Op::Tset) put(Tok::M, 1, v | (1u << n));
                }
                break;
            }
            case Op::Ldc:
                if (a == Tok::Cr) control_write(i.v[0] & 0xFF, get(b, 1));
                else put(a, 0, control_read(i.v[1] & 0xFF));
                break;
            case Op::Minc1: case Op::Minc2: case Op::Minc4: case Op::Mdec1: case Op::Mdec2: case Op::Mdec4: {
                const std::uint32_t m = i.v[0] & 0xFFFF, r = get(Tok::M, 1) & 0xFFFF;
                const unsigned step = row.id == Op::Minc1 || row.id == Op::Mdec1 ? 1 : row.id == Op::Minc2 || row.id == Op::Mdec2 ? 2 : 4;
                const bool inc = row.id == Op::Minc1 || row.id == Op::Minc2 || row.id == Op::Minc4;
                const bool wrap = (r & m) == m;
                put(Tok::M, 1, inc ? (wrap ? r - m : r + step) : (wrap ? r + m : r - step));
                break;
            }
            case Op::Inc:
            case Op::Dec: {
                const std::uint32_t n = get(a, 0), v = get(Tok::M, 1);
                const bool flags = row.ctx != dt::Ctx::Reg || sz == 1;   // (a word or long register keeps every flag; the others keep the carry)
                const std::uint16_t saved = g.sr;
                const std::uint32_t r = alu(row.id == Op::Inc ? Op::Add : Op::Sub, v, n, sz);
                g.sr = flags ? static_cast<std::uint16_t>((g.sr & ~flag::C) | (saved & flag::C)) : saved;
                put(Tok::M, 1, r);
                break;
            }
            case Op::Scc: put(b, 1, condition(cc, g.f()) ? 1 : 0); break;
            case Op::Add: case Op::Adc: case Op::Sub: case Op::Sbc: case Op::And: case Op::Xor: case Op::Or: case Op::Cp: {
                const std::uint32_t r = alu(row.id, get(a, 0), get(b, 1), sz);
                if (row.id != Op::Cp) put(a, 0, r);
                break;
            }
            case Op::Rlc: case Op::Rrc: case Op::Rl: case Op::Rr: case Op::Sla: case Op::Sra: case Op::Sll: case Op::Srl: {
                unsigned count = 1;
                if (a == Tok::Cnt4) count = get(a, 0);
                else if (a == Tok::A) count = (g.read(0xE0, 1) & 15u) ? (g.read(0xE0, 1) & 15u) : 16u;
                put(Tok::M, 1, shift(row.id, get(Tok::M, 1), count, sz));
                break;
            }
            case Op::Rld:
            case Op::Rrd: {
                const std::uint32_t acc = g.read(0xE0, 1), mem = bus.read(ea, 1);
                std::uint32_t newa, newm;
                if (row.id == Op::Rld) {
                    newa = (acc & 0xF0) | ((mem & 0xF0) >> 4);
                    newm = ((mem & 0x0F) << 4) | (acc & 0x0F);
                } else {
                    newa = (acc & 0xF0) | (mem & 0x0F);
                    newm = ((acc & 0x0F) << 4) | ((mem & 0xF0) >> 4);
                }
                g.write(0xE0, 1, newa);
                bus.write(ea, 1, newm);
                sz_flags(newa, 1);
                set_flag(flag::H, false);
                set_flag(flag::N, false);
                parity(newa, 1);
                break;
            }
            case Op::Ldi: case Op::Ldir: case Op::Ldd: case Op::Lddr: block_move(); break;
            case Op::Cpi: case Op::Cpir: case Op::Cpd: case Op::Cpdr: block_compare(); break;
            case Op::Bs1f:
            case Op::Bs1b: {
                const std::uint32_t r = get(b, 1) & 0xFFFF;
                if (r == 0) {
                    set_flag(flag::V, true);
                } else {
                    set_flag(flag::V, false);
                    unsigned n = 0;
                    if (row.id == Op::Bs1f) {
                        while (!((r >> n) & 1u)) ++n;
                    } else {
                        n = 15;
                        while (!((r >> n) & 1u)) --n;
                    }
                    put(a, 0, n);
                }
                break;
            }
        }
        g.pc = next;
        return std::nullopt;
    }

    void daa() {
        const std::uint32_t old = get(Tok::M, 0) & 0xFF, high = old & 0xF0, low = old & 0x0F;
        const bool c = flag_on(flag::C), h = flag_on(flag::H);
        std::uint32_t fix = 0;
        bool carry = false;
        if (c) {
            fix = h ? 0x66 : (low < 0x0A ? 0x60 : 0x66);
            carry = true;
        } else if (h) {
            fix = old < 0x9A ? 0x06 : 0x66;
        } else if (high < 0x90 && low > 0x09) {
            fix = 0x06;
        } else if (high > 0x80 && low > 0x09) {
            fix = 0x66;
        } else if (high > 0x90 && low < 0x0A) {
            fix = 0x60;
        }
        const bool subtract = flag_on(flag::N);
        const std::uint32_t r = (subtract ? old - fix : old + fix) & 0xFF;
        set_flag(flag::C, carry || (subtract ? r > old : r < old));
        sz_flags(r, 1);
        set_flag(flag::H, (((old ^ fix) ^ r) & 0x10) != 0);
        parity(r, 1);
        put(Tok::M, 0, r);
    }

    // ldi ldir ldd lddr: (dest pointer) <- (source pointer), both pointers step, BC counts down
    void block_move() {
        const Op id = i.id();
        const unsigned size = i.z;
        const bool down = id == Op::Ldd || id == Op::Lddr, repeat = id == Op::Ldir || id == Op::Lddr;
        const unsigned src_group = (i.m.reg >> 2) & 7u;
        const std::uint8_t src_code = i.m.reg, dest_code = src_group == 3 ? 0xE8 : 0xF0;   // XDE / XIX
        for (unsigned n = 0; n < 0x10000; ++n) {
            bus.write(g.read(dest_code, 4), size, bus.read(g.read(src_code, 4), size));
            const std::uint32_t d = down ? 0u - size : size;
            g.write(src_code, 4, g.read(src_code, 4) + d);
            g.write(dest_code, 4, g.read(dest_code, 4) + d);
            const std::uint32_t bc = (g.read(0xE4, 2) - 1) & 0xFFFF;
            g.write(0xE4, 2, bc);
            set_flag(flag::H, false);
            set_flag(flag::N, false);
            set_flag(flag::V, bc != 0);
            ++cpu.steps;
            if (!repeat || bc == 0) break;
        }
    }

    void block_compare() {
        const Op id = i.id();
        const unsigned size = i.z;
        const bool down = id == Op::Cpd || id == Op::Cpdr, repeat = id == Op::Cpir || id == Op::Cpdr;
        for (unsigned n = 0; n < 0x10000; ++n) {
            const std::uint32_t acc = g.read(0xE0, size), mem = bus.read(g.read(i.m.reg, 4), size);
            const std::uint32_t r = (acc - mem) & mask_of(size);
            g.write(i.m.reg, 4, g.read(i.m.reg, 4) + (down ? 0u - size : size));
            const std::uint32_t bc = (g.read(0xE4, 2) - 1) & 0xFFFF;
            g.write(0xE4, 2, bc);
            set_flag(flag::S, negative(r, size));
            set_flag(flag::Z, r == 0);
            set_flag(flag::H, false);
            set_flag(flag::N, true);
            set_flag(flag::V, bc != 0);
            ++cpu.steps;
            if (!repeat || bc == 0 || r == 0) break;
        }
    }

    std::uint32_t control_read(unsigned code) const {
        if (code < 0x10) return g.dmas[(code >> 2) & 3];
        if (code < 0x20) return g.dmad[(code >> 2) & 3];
        if (code < 0x30) return (code & 2) ? g.dmam[(code >> 2) & 3] : g.dmac[(code >> 2) & 3];
        return code == 0x30 ? g.intnest : 0;
    }
    void control_write(unsigned code, std::uint32_t v) {
        if (code < 0x10) g.dmas[(code >> 2) & 3] = v;
        else if (code < 0x20) g.dmad[(code >> 2) & 3] = v;
        else if (code < 0x30) {
            if (code & 2) g.dmam[(code >> 2) & 3] = static_cast<std::uint8_t>(v);
            else g.dmac[(code >> 2) & 3] = static_cast<std::uint16_t>(v);
        } else if (code == 0x30) {
            g.intnest = static_cast<std::uint16_t>(v);
        }
    }
};

}  // namespace

std::optional<Fault> Cpu::step() {
    Bytes window(12);
    for (unsigned k = 0; k < window.size(); ++k) window[k] = static_cast<std::uint8_t>(bus_->read((regs.pc + k) & 0xFFFFFF, 1));
    const std::optional<dt::Insn> insn = dt::decode(window, 0, regs.pc);
    if (!insn) return Fault{regs.pc, std::format("undefined instruction at {:06X}", regs.pc)};
    Exec x(*this, regs, *bus_, *insn);
    if (std::optional<Fault> fault = x.run()) return fault;
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

RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options) {
    Cpu cpu(bus);
    cpu.on_swi = options.on_swi;
    cpu.regs.sr = options.sr;
    cpu.regs.xwa() = options.xwa;
    cpu.regs.xbc() = options.xbc;
    cpu.regs.xde() = options.xde;
    cpu.regs.xhl() = options.xhl;
    cpu.regs.xix = options.xix;
    cpu.regs.xiy = options.xiy;
    cpu.regs.xiz = options.xiz;
    cpu.regs.xsp = options.sp;
    for (const std::uint16_t w : options.args) {
        cpu.regs.xsp -= 2;
        bus.write(cpu.regs.xsp, 2, w);
    }
    cpu.regs.xsp -= 4;
    bus.write(cpu.regs.xsp, 4, kReturnAddress);
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
    result.swis = cpu.swis;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(logged_before), bus.log().end());
    return result;
}

}  // namespace cb::emulate::tlcs900
