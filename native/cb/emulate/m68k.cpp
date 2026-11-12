// cb/emulate/m68k.cpp -- see m68k.hpp.
#include "cb/emulate/m68k.hpp"

#include <algorithm>
#include <initializer_list>
#include <format>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "cb/core/error.hpp"

namespace cb::emulate::m68k {

namespace dec = cb::decompile::m68k;
using dec::Op;
using dec::OpKind;
using dec::Size;

namespace {

// ---- sizes -------------------------------------------------------------------------------------------------------

// The size an instruction works at; one with no size suffix works on words.
constexpr Size working_size(Size s) { return (s == Size::Byte || s == Size::Long) ? s : Size::Word; }
constexpr unsigned width_of(Size s) { return s == Size::Byte ? 1u : s == Size::Long ? 4u : 2u; }
constexpr std::uint32_t mask_of(Size s) { return s == Size::Byte ? 0xFFu : s == Size::Long ? 0xFFFFFFFFu : 0xFFFFu; }
constexpr std::uint32_t sign_of(Size s) { return s == Size::Byte ? 0x80u : s == Size::Long ? 0x80000000u : 0x8000u; }
constexpr std::uint32_t mask_bytes(unsigned n) { return n >= 4 ? 0xFFFFFFFFu : (1u << (8 * n)) - 1u; }

// v taken as a signed number of that size.
constexpr std::int64_t sx(std::uint32_t v, Size s) {
    switch (s) {
        case Size::Byte: return static_cast<std::int8_t>(v & 0xFF);
        case Size::Long: return static_cast<std::int32_t>(v);
        default: return static_cast<std::int16_t>(v & 0xFFFF);
    }
}

// ---- what the interpreter works out from a decoded instruction, once -----------------------------------------------

enum class Mn : std::uint8_t {
    Move, Movea, Moveq, Movem, Movep, Lea, Pea, Clr, Tst, Tas, Neg, Negx, Not, Ext, Swap, Exg,
    Alu, Adda, Suba, Cmpa, Addq, Subq, Addx, Subx, Cmpm, Mulu, Muls, Divu, Divs, Abcd, Sbcd, Nbcd, Shift,
    Btst, Bchg, Bclr, Bset, Jmp, Jsr, Rts, Rtr, Rte, Link, Unlk, Chk, Nop, Trap, Illegal, Stop, Trapv,
    Bcc, Dbcc, Scc, NotRun,
};
enum class AluOp : std::uint8_t { Add, Sub, Cmp, And, Or, Eor };
enum class ShiftKind : std::uint8_t { Arithmetic, Logical, RotateExtend, Rotate };
enum class Cond : std::uint8_t { T, F, HI, LS, CC, CS, NE, EQ, VC, VS, PL, MI, GE, LT, GT, LE, RA, SR };

struct Entry {
    Mn mn;
    AluOp alu = AluOp::Add;
};
const std::unordered_map<std::string_view, Entry>& mnemonic_table() {
    static const std::unordered_map<std::string_view, Entry> t = {
        {"MOVE", {Mn::Move}}, {"MOVEA", {Mn::Movea}}, {"MOVEQ", {Mn::Moveq}}, {"MOVEM", {Mn::Movem}}, {"MOVEP", {Mn::Movep}},
        {"LEA", {Mn::Lea}}, {"PEA", {Mn::Pea}}, {"CLR", {Mn::Clr}}, {"TST", {Mn::Tst}}, {"TAS", {Mn::Tas}}, {"NEG", {Mn::Neg}},
        {"NEGX", {Mn::Negx}}, {"NOT", {Mn::Not}}, {"EXT", {Mn::Ext}}, {"SWAP", {Mn::Swap}}, {"EXG", {Mn::Exg}},
        {"ADD", {Mn::Alu, AluOp::Add}}, {"ADDI", {Mn::Alu, AluOp::Add}}, {"SUB", {Mn::Alu, AluOp::Sub}},
        {"SUBI", {Mn::Alu, AluOp::Sub}}, {"CMP", {Mn::Alu, AluOp::Cmp}}, {"CMPI", {Mn::Alu, AluOp::Cmp}},
        {"AND", {Mn::Alu, AluOp::And}}, {"ANDI", {Mn::Alu, AluOp::And}}, {"OR", {Mn::Alu, AluOp::Or}},
        {"ORI", {Mn::Alu, AluOp::Or}}, {"EOR", {Mn::Alu, AluOp::Eor}}, {"EORI", {Mn::Alu, AluOp::Eor}},
        {"ADDA", {Mn::Adda}}, {"SUBA", {Mn::Suba}}, {"CMPA", {Mn::Cmpa}}, {"ADDQ", {Mn::Addq}}, {"SUBQ", {Mn::Subq}},
        {"ADDX", {Mn::Addx}}, {"SUBX", {Mn::Subx}}, {"CMPM", {Mn::Cmpm}}, {"MULU", {Mn::Mulu}}, {"MULS", {Mn::Muls}},
        {"DIVU", {Mn::Divu}}, {"DIVS", {Mn::Divs}}, {"ABCD", {Mn::Abcd}}, {"SBCD", {Mn::Sbcd}}, {"NBCD", {Mn::Nbcd}},
        {"ASL", {Mn::Shift}}, {"ASR", {Mn::Shift}}, {"LSL", {Mn::Shift}}, {"LSR", {Mn::Shift}}, {"ROL", {Mn::Shift}},
        {"ROR", {Mn::Shift}}, {"ROXL", {Mn::Shift}}, {"ROXR", {Mn::Shift}},
        {"BTST", {Mn::Btst}}, {"BCHG", {Mn::Bchg}}, {"BCLR", {Mn::Bclr}}, {"BSET", {Mn::Bset}},
        {"JMP", {Mn::Jmp}}, {"JSR", {Mn::Jsr}}, {"RTS", {Mn::Rts}}, {"RTR", {Mn::Rtr}}, {"RTE", {Mn::Rte}},
        {"LINK", {Mn::Link}}, {"UNLK", {Mn::Unlk}}, {"CHK", {Mn::Chk}}, {"NOP", {Mn::Nop}}, {"RESET", {Mn::Nop}},
        {"TRAP", {Mn::Trap}}, {"ILLEGAL", {Mn::Illegal}}, {"STOP", {Mn::Stop}}, {"TRAPV", {Mn::Trapv}},
    };
    return t;
}

std::optional<Cond> condition_named(std::string_view cc, bool allow_subroutine) {
    static const std::pair<std::string_view, Cond> names[] = {
        {"T", Cond::T}, {"F", Cond::F}, {"HI", Cond::HI}, {"LS", Cond::LS}, {"CC", Cond::CC}, {"CS", Cond::CS},
        {"NE", Cond::NE}, {"EQ", Cond::EQ}, {"VC", Cond::VC}, {"VS", Cond::VS}, {"PL", Cond::PL}, {"MI", Cond::MI},
        {"GE", Cond::GE}, {"LT", Cond::LT}, {"GT", Cond::GT}, {"LE", Cond::LE}, {"RA", Cond::RA}};
    for (const auto& [name, c] : names) {
        if (cc == name) return c;
    }
    if (allow_subroutine && cc == "SR") return Cond::SR;   // BSR
    return std::nullopt;
}

}  // namespace

struct Decoded {
    dec::Insn insn;
    Mn mn = Mn::NotRun;
    AluOp alu = AluOp::Add;
    ShiftKind shift = ShiftKind::Logical;
    bool shift_left = false;
    Cond cc = Cond::T;
    Size size = Size::Word;
    std::uint32_t target = 0;   // where a branch goes (the decoder gives every branch one)
};

struct DecodedCache {
    std::unordered_map<std::uint32_t, Decoded> by_address;
};

namespace {

Decoded prepare(dec::Insn insn) {
    Decoded out;
    out.size = working_size(insn.size);
    out.target = insn.target.value_or(0);
    const std::string_view mn = insn.mnemonic;
    if (auto it = mnemonic_table().find(mn); it != mnemonic_table().end()) {
        out.mn = it->second.mn;
        out.alu = it->second.alu;
        if (out.mn == Mn::Shift) {
            out.shift_left = mn.back() == 'L';
            const std::string_view kind = mn.substr(0, mn.size() - 1);
            out.shift = kind == "AS" ? ShiftKind::Arithmetic : kind == "LS" ? ShiftKind::Logical
                      : kind == "ROX" ? ShiftKind::RotateExtend : ShiftKind::Rotate;
        }
    } else {
        // the conditional families, by prefix: DB<cc>, B<cc> (BRA and BSR too), S<cc>
        out.mn = Mn::NotRun;
        struct Family {
            std::string_view prefix;
            Mn mn;
        };
        for (const Family& f : {Family{"DB", Mn::Dbcc}, Family{"B", Mn::Bcc}, Family{"S", Mn::Scc}}) {
            if (!mn.starts_with(f.prefix)) continue;
            if (const auto cc = condition_named(mn.substr(f.prefix.size()), f.mn == Mn::Bcc)) {
                out.mn = f.mn;
                out.cc = *cc;
            }
            break;
        }
    }
    out.insn = std::move(insn);
    return out;
}

// The executing state of one step: the CPU's registers and its bus, and everything an instruction does with them.
class Executor {
public:
    Executor(Cpu& cpu, Bus& bus) : cpu_(cpu), bus_(bus), d_(cpu.d), a_(cpu.a) {}

    std::optional<Fault> execute(const Decoded& dc);

private:
    using Flt = std::optional<Fault>;

    // ---- the flags
    bool flag(std::uint16_t f) const { return (cpu_.sr & f) != 0; }
    void set_flag(std::uint16_t f, bool on) {
        cpu_.sr = static_cast<std::uint16_t>(on ? (cpu_.sr | f) : (cpu_.sr & ~f));
    }
    void set_nz(std::uint32_t r, Size size) {
        set_flag(flag::N, (r & sign_of(size)) != 0);
        set_flag(flag::Z, (r & mask_of(size)) == 0);
    }
    void logic(std::uint32_t r, Size size) {
        set_nz(r, size);
        set_flag(flag::V, false);
        set_flag(flag::C, false);
    }
    bool condition(Cond cc) const;

    // ---- memory
    std::uint32_t rd(std::uint32_t address, Size size) { return bus_.read(address, width_of(size)); }
    void wr(std::uint32_t address, Size size, std::uint32_t v) { bus_.write(address, width_of(size), v & mask_of(size)); }
    void push(std::uint32_t v, Size size = Size::Long) {
        a_[7] -= width_of(size);
        wr(a_[7], size, v);
    }
    std::uint32_t pop(Size size = Size::Long) {
        const std::uint32_t v = rd(a_[7], size);
        a_[7] += width_of(size);
        return v;
    }

    // ---- operands
    static bool in_memory(OpKind k) {
        switch (k) {
            case OpKind::Indirect: case OpKind::PostInc: case OpKind::PreDec: case OpKind::Disp: case OpKind::Index:
            case OpKind::AbsW: case OpKind::AbsL: case OpKind::PcDisp: case OpKind::PcIndex:
                return true;
            default:
                return false;
        }
    }
    // The address a memory operand names, applying -(An) and (An)+.
    std::uint32_t address(const Op& o, Size size);
    std::uint32_t get(const Op& o, Size size);
    std::uint32_t get_at(Size size, std::uint32_t at);
    void put(const Op& o, Size size, std::uint32_t v);
    void put_at(Size size, std::uint32_t v, std::uint32_t at);
    // Read-modify-write one operand (its address worked out once); fn returns the new value, or nullopt to leave it.
    template <class Fn>
    void modify(const Op& o, Size size, Fn&& fn) {
        if (in_memory(o.kind)) {
            const std::uint32_t at = address(o, size);
            if (const std::optional<std::uint32_t> r = fn(get_at(size, at))) put_at(size, *r, at);
        } else {
            if (const std::optional<std::uint32_t> r = fn(get(o, size))) put(o, size, *r);
        }
    }

    // ---- arithmetic
    std::uint32_t add(std::uint32_t x, std::uint32_t y, Size size, std::uint32_t extend = 0, bool keep_z = false);
    // x - y - extend.
    std::uint32_t sub(std::uint32_t x, std::uint32_t y, Size size, std::uint32_t extend = 0, bool keep_z = false,
                      bool set_x = true);
    std::uint32_t bcd_add(std::uint32_t x, std::uint32_t y);
    std::uint32_t bcd_sub(std::uint32_t x, std::uint32_t y);   // x - y - X in BCD
    std::uint32_t shift(const Decoded& dc, std::uint32_t v, unsigned count, Size size);

    Flt movem(const Decoded& dc);
    Flt divide(const Decoded& dc, bool is_signed);

    Cpu& cpu_;
    Bus& bus_;
    std::array<std::uint32_t, 8>& d_;
    std::array<std::uint32_t, 8>& a_;
};

bool Executor::condition(Cond cc) const {
    const bool n = flag(flag::N), z = flag(flag::Z), v = flag(flag::V), c = flag(flag::C);
    switch (cc) {
        case Cond::T: case Cond::RA: case Cond::SR: return true;
        case Cond::F: return false;
        case Cond::HI: return !c && !z;
        case Cond::LS: return c || z;
        case Cond::CC: return !c;
        case Cond::CS: return c;
        case Cond::NE: return !z;
        case Cond::EQ: return z;
        case Cond::VC: return !v;
        case Cond::VS: return v;
        case Cond::PL: return !n;
        case Cond::MI: return n;
        case Cond::GE: return n == v;
        case Cond::LT: return n != v;
        case Cond::GT: return !z && n == v;
        case Cond::LE: return z || n != v;
    }
    return false;
}

std::uint32_t Executor::address(const Op& o, Size size) {
    const std::uint32_t step = (o.reg == 7 && size == Size::Byte) ? 2u : width_of(size);   // a byte moves A7 by two
    switch (o.kind) {
        case OpKind::Indirect: return a_[o.reg];
        case OpKind::PostInc: {
            const std::uint32_t at = a_[o.reg];
            a_[o.reg] += step;
            return at;
        }
        case OpKind::PreDec: a_[o.reg] -= step; return a_[o.reg];
        case OpKind::Disp: return static_cast<std::uint32_t>(a_[o.reg] + o.value);
        case OpKind::Index:
        case OpKind::PcIndex: {
            const std::uint32_t xn = o.index.address ? a_[o.index.number] : d_[o.index.number];
            const std::int64_t x = sx(xn, o.index.size == Size::Word ? Size::Word : Size::Long);
            const std::int64_t base = o.kind == OpKind::Index ? static_cast<std::int64_t>(a_[o.reg]) + o.value : o.value;
            return static_cast<std::uint32_t>(base + x);
        }
        case OpKind::AbsW: case OpKind::AbsL: case OpKind::PcDisp: return static_cast<std::uint32_t>(o.value);
        default: break;
    }
    CB_ASSERT(false, "an operand that names no address was asked for one");
    return 0;
}

std::uint32_t Executor::get(const Op& o, Size size) {
    switch (o.kind) {
        case OpKind::Dreg: return d_[o.reg] & mask_of(size);
        case OpKind::Areg: return a_[o.reg] & mask_of(size);
        case OpKind::Imm: case OpKind::Quick: return static_cast<std::uint32_t>(o.value) & mask_of(size);
        case OpKind::Sr: return cpu_.sr;
        case OpKind::Ccr: return cpu_.sr & 0x1Fu;
        default: return rd(address(o, size), size);
    }
}

std::uint32_t Executor::get_at(Size size, std::uint32_t at) { return rd(at, size); }

void Executor::put(const Op& o, Size size, std::uint32_t v) {
    switch (o.kind) {
        case OpKind::Dreg: d_[o.reg] = (d_[o.reg] & ~mask_of(size)) | (v & mask_of(size)); break;
        case OpKind::Areg:   // a word is sign-extended to the whole register
            a_[o.reg] = size == Size::Word ? static_cast<std::uint32_t>(sx(v, Size::Word)) : v;
            break;
        case OpKind::Sr: cpu_.sr = static_cast<std::uint16_t>(v & 0xA71Fu); break;
        case OpKind::Ccr: cpu_.sr = static_cast<std::uint16_t>((cpu_.sr & 0xFF00u) | (v & 0x1Fu)); break;
        default: wr(address(o, size), size, v); break;
    }
}

void Executor::put_at(Size size, std::uint32_t v, std::uint32_t at) { wr(at, size, v); }

std::uint32_t Executor::add(std::uint32_t x, std::uint32_t y, Size size, std::uint32_t extend, bool keep_z) {
    const std::uint32_t m = mask_of(size), s = sign_of(size);
    const std::uint64_t sum = static_cast<std::uint64_t>(x) + y + extend;
    const std::uint32_t r = static_cast<std::uint32_t>(sum) & m;
    const bool c = sum > m;
    set_flag(flag::X, c);
    set_flag(flag::N, (r & s) != 0);
    set_flag(flag::Z, r == 0 && (!keep_z || flag(flag::Z)));
    set_flag(flag::V, ((~(x ^ y) & (x ^ r)) & s) != 0);
    set_flag(flag::C, c);
    return r;
}

std::uint32_t Executor::sub(std::uint32_t x, std::uint32_t y, Size size, std::uint32_t extend, bool keep_z, bool set_x) {
    const std::uint32_t m = mask_of(size), s = sign_of(size);
    const std::uint32_t r = (x - y - extend) & m;
    const bool c = static_cast<std::uint64_t>(y) + extend > x;
    if (set_x) set_flag(flag::X, c);
    set_flag(flag::N, (r & s) != 0);
    set_flag(flag::Z, r == 0 && (!keep_z || flag(flag::Z)));
    set_flag(flag::V, (((x ^ y) & (x ^ r)) & s) != 0);
    set_flag(flag::C, c);
    return r;
}

std::uint32_t Executor::bcd_add(std::uint32_t x, std::uint32_t y) {
    const std::uint32_t extend = flag(flag::X) ? 1u : 0u;
    const std::uint32_t lo = (x & 0xF) + (y & 0xF) + extend;
    std::uint32_t r = (x & 0xF0) + (y & 0xF0) + lo;
    if (lo > 9) r += 6;
    const bool c = r > 0x99;
    if (c) r += 0x60;
    r &= 0xFF;
    set_flag(flag::X, c);
    set_flag(flag::C, c);
    set_flag(flag::Z, r == 0 && flag(flag::Z));
    return r;
}

std::uint32_t Executor::bcd_sub(std::uint32_t x, std::uint32_t y) {
    const std::int32_t extend = flag(flag::X) ? 1 : 0;
    const std::int32_t lo = static_cast<std::int32_t>(x & 0xF) - static_cast<std::int32_t>(y & 0xF) - extend;
    std::int32_t r = static_cast<std::int32_t>(x & 0xF0) - static_cast<std::int32_t>(y & 0xF0) + lo;
    if (lo < 0) r -= 6;
    const bool c = r < 0;
    if (c) r -= 0x60;
    const std::uint32_t out = static_cast<std::uint32_t>(r) & 0xFF;
    set_flag(flag::X, c);
    set_flag(flag::C, c);
    set_flag(flag::Z, out == 0 && flag(flag::Z));
    return out;
}

// The shifts and rotates, one bit at a time (as the CPU does, so that X, C and V follow each step).
std::uint32_t Executor::shift(const Decoded& dc, std::uint32_t v, unsigned count, Size size) {
    const std::uint32_t m = mask_of(size), s = sign_of(size);
    const bool left = dc.shift_left;
    bool x = flag(flag::X), c = false, overflow = false;
    if (count == 0) {
        set_flag(flag::N, (v & s) != 0);
        set_flag(flag::Z, (v & m) == 0);
        set_flag(flag::V, false);
        set_flag(flag::C, dc.shift == ShiftKind::RotateExtend ? x : false);
        return v;
    }
    for (unsigned i = 0; i < count; ++i) {
        switch (dc.shift) {
            case ShiftKind::Arithmetic:
            case ShiftKind::Logical: {
                std::uint32_t nv;
                if (left) {
                    c = (v & s) != 0;
                    nv = (v << 1) & m;
                    if (dc.shift == ShiftKind::Arithmetic && ((nv ^ v) & s)) overflow = true;
                } else {
                    c = (v & 1) != 0;
                    nv = (v >> 1) | (dc.shift == ShiftKind::Arithmetic ? (v & s) : 0u);
                }
                v = nv;
                x = c;
                break;
            }
            case ShiftKind::Rotate:
                if (left) {
                    c = (v & s) != 0;
                    v = ((v << 1) | (c ? 1u : 0u)) & m;
                } else {
                    c = (v & 1) != 0;
                    v = (v >> 1) | (c ? s : 0u);
                }
                break;
            case ShiftKind::RotateExtend:
                if (left) {
                    c = (v & s) != 0;
                    v = ((v << 1) | (x ? 1u : 0u)) & m;
                } else {
                    c = (v & 1) != 0;
                    v = (v >> 1) | (x ? s : 0u);
                }
                x = c;
                break;
        }
    }
    set_flag(flag::N, (v & s) != 0);
    set_flag(flag::Z, v == 0);
    set_flag(flag::V, dc.shift == ShiftKind::Rotate ? false : overflow);
    set_flag(flag::C, c);
    if (dc.shift != ShiftKind::Rotate) set_flag(flag::X, x);
    return v;
}

Executor::Flt Executor::movem(const Decoded& dc) {
    const auto& ops = dc.insn.ops;
    const Size size = dc.size;
    const std::uint32_t n = width_of(size);
    if (ops[0].kind == OpKind::Regs) {   // registers to memory
        const std::uint32_t mask = static_cast<std::uint32_t>(ops[0].value);
        const Op& dst = ops[1];
        std::array<std::uint32_t, 16> regs{};
        for (unsigned i = 0; i < 8; ++i) {
            regs[i] = d_[i];
            regs[8 + i] = a_[i];
        }
        if (dst.kind == OpKind::PreDec) {
            std::uint32_t at = a_[dst.reg];
            for (int i = 15; i >= 0; --i) {   // the mask is reversed: bit 0 is A7
                if (mask & (1u << (15 - i))) {
                    at -= n;
                    wr(at, size, regs[static_cast<std::size_t>(i)]);
                }
            }
            a_[dst.reg] = at;
        } else {
            std::uint32_t at = address(dst, size);
            for (unsigned i = 0; i < 16; ++i) {
                if (mask & (1u << i)) {
                    wr(at, size, regs[i]);
                    at += n;
                }
            }
        }
        return std::nullopt;
    }
    const Op& src = ops[0];
    const std::uint32_t mask = static_cast<std::uint32_t>(ops[1].value);
    std::uint32_t at = src.kind == OpKind::PostInc ? a_[src.reg] : address(src, size);
    for (unsigned i = 0; i < 16; ++i) {
        if (mask & (1u << i)) {
            std::uint32_t v = rd(at, size);
            if (size == Size::Word) v = static_cast<std::uint32_t>(sx(v, Size::Word));   // a word fills the register
            (i < 8 ? d_[i] : a_[i - 8]) = v;
            at += n;
        }
    }
    if (src.kind == OpKind::PostInc) a_[src.reg] = at;
    return std::nullopt;
}

// DIVU / DIVS: a 32-bit dividend by a 16-bit divisor, quotient low and remainder high. An overflowing quotient leaves the
// destination alone and sets V.
Executor::Flt Executor::divide(const Decoded& dc, bool is_signed) {
    const auto& ops = dc.insn.ops;
    const std::uint32_t divisor = get(ops[0], Size::Word);
    const std::uint32_t dividend = d_[ops[1].reg];
    if (divisor == 0) return Fault{Fault::Kind::DivideByZero, dc.insn.pc, {}};
    std::int64_t q, rem;
    if (is_signed) {
        const std::int64_t sa = sx(dividend, Size::Long), sb = sx(divisor, Size::Word);
        q = (sa < 0 ? -sa : sa) / (sb < 0 ? -sb : sb) * ((sa < 0) == (sb < 0) ? 1 : -1);   // truncated toward zero
        rem = sa - q * sb;
        if (q < -0x8000 || q > 0x7FFF) {
            set_flag(flag::V, true);
            set_flag(flag::C, false);
            return std::nullopt;
        }
    } else {
        q = dividend / divisor;
        rem = dividend % divisor;
        if (q > 0xFFFF) {
            set_flag(flag::V, true);
            set_flag(flag::C, false);
            return std::nullopt;
        }
    }
    d_[ops[1].reg] = (static_cast<std::uint32_t>(rem & 0xFFFF) << 16) | static_cast<std::uint32_t>(q & 0xFFFF);
    logic(static_cast<std::uint32_t>(q & 0xFFFF), Size::Word);
    return std::nullopt;
}

Executor::Flt Executor::execute(const Decoded& dc) {
    const dec::Insn& insn = dc.insn;
    const Size size = dc.size;
    const Op* ops = insn.ops.data();
    auto here = [&](Fault::Kind kind) { return Fault{kind, insn.pc, {}}; };
    switch (dc.mn) {
        case Mn::Move: {
            const Op &src = ops[0], &dst = ops[1];
            if (dst.kind == OpKind::Sr || dst.kind == OpKind::Ccr || dst.kind == OpKind::Usp || src.kind == OpKind::Sr ||
                src.kind == OpKind::Usp) {
                if (dst.kind == OpKind::Usp) {
                    cpu_.usp = a_[src.reg];
                } else if (src.kind == OpKind::Usp) {
                    a_[dst.reg] = cpu_.usp;
                } else {
                    put(dst, Size::Word, get(src, Size::Word));
                }
                break;
            }
            const std::uint32_t v = get(src, size);
            put(dst, size, v);
            logic(v, size);
            break;
        }
        case Mn::Movea: put(ops[1], size, get(ops[0], size)); break;
        case Mn::Moveq: {
            const std::uint32_t v = static_cast<std::uint32_t>(ops[0].value);
            d_[ops[1].reg] = v;
            logic(v, Size::Long);
            break;
        }
        case Mn::Movem: return movem(dc);
        case Mn::Movep: {
            const unsigned n = width_of(size);
            if (ops[0].kind == OpKind::Disp) {   // memory to register: every other byte
                const std::uint32_t at = address(ops[0], size);
                std::uint32_t v = 0;
                for (unsigned i = 0; i < n; ++i) v = (v << 8) | rd(at + 2 * i, Size::Byte);
                put(ops[1], size, v);
            } else {
                const std::uint32_t v = get(ops[0], size);
                const std::uint32_t at = address(ops[1], size);
                for (unsigned i = 0; i < n; ++i) wr(at + 2 * i, Size::Byte, v >> (8 * (n - 1 - i)));
            }
            break;
        }
        case Mn::Lea: a_[ops[1].reg] = address(ops[0], Size::Long); break;
        case Mn::Pea: push(address(ops[0], Size::Long)); break;
        case Mn::Clr:
            put(ops[0], size, 0);
            set_flag(flag::N, false);
            set_flag(flag::Z, true);
            set_flag(flag::V, false);
            set_flag(flag::C, false);
            break;
        case Mn::Tst: logic(get(ops[0], size), size); break;
        case Mn::Tas:
            modify(ops[0], Size::Byte, [&](std::uint32_t v) -> std::optional<std::uint32_t> {
                logic(v, Size::Byte);
                return v | 0x80;
            });
            break;
        case Mn::Neg:
            modify(ops[0], size, [&](std::uint32_t v) -> std::optional<std::uint32_t> { return sub(0, v, size); });
            break;
        case Mn::Negx:
            modify(ops[0], size, [&](std::uint32_t v) -> std::optional<std::uint32_t> {
                return sub(0, v, size, flag(flag::X) ? 1u : 0u, true);
            });
            break;
        case Mn::Not:
            modify(ops[0], size, [&](std::uint32_t v) -> std::optional<std::uint32_t> {
                const std::uint32_t r = ~v & mask_of(size);
                logic(r, size);
                return r;
            });
            break;
        case Mn::Ext: {
            std::uint32_t& dn = d_[ops[0].reg];
            std::uint32_t r;
            if (size == Size::Word) {
                r = static_cast<std::uint32_t>(sx(dn, Size::Byte)) & 0xFFFFu;
                dn = (dn & 0xFFFF0000u) | r;
            } else {
                r = static_cast<std::uint32_t>(sx(dn, Size::Word));
                dn = r;
            }
            logic(r, size);
            break;
        }
        case Mn::Swap: {
            std::uint32_t& dn = d_[ops[0].reg];
            dn = (dn << 16) | (dn >> 16);
            logic(dn, Size::Long);
            break;
        }
        case Mn::Exg: {
            const std::uint32_t va = get(ops[0], Size::Long), vb = get(ops[1], Size::Long);
            put(ops[0], Size::Long, vb);
            put(ops[1], Size::Long, va);
            break;
        }
        case Mn::Alu: {
            const Op &src = ops[0], &dst = ops[1];
            if (dst.kind == OpKind::Ccr || dst.kind == OpKind::Sr) {   // ANDI / ORI / EORI to CCR or SR
                const std::uint32_t v = get(src, size), cur = get(dst, Size::Word);
                put(dst, Size::Word, dc.alu == AluOp::And ? (cur & v) : dc.alu == AluOp::Or ? (cur | v) : (cur ^ v));
                break;
            }
            const std::uint32_t b = get(src, size);
            modify(dst, size, [&](std::uint32_t x) -> std::optional<std::uint32_t> {
                switch (dc.alu) {
                    case AluOp::Add: return add(x, b, size);
                    case AluOp::Sub: return sub(x, b, size);
                    case AluOp::Cmp: sub(x, b, size, 0, false, false); return std::nullopt;
                    default: {
                        const std::uint32_t r = dc.alu == AluOp::And ? (x & b) : dc.alu == AluOp::Or ? (x | b) : (x ^ b);
                        logic(r, size);
                        return r;
                    }
                }
            });
            break;
        }
        case Mn::Adda: case Mn::Suba: case Mn::Cmpa: {
            const std::int64_t v = sx(get(ops[0], size), size);
            const std::uint32_t an = a_[ops[1].reg];
            if (dc.mn == Mn::Cmpa) {
                sub(an, static_cast<std::uint32_t>(v), Size::Long, 0, false, false);
            } else {
                a_[ops[1].reg] = static_cast<std::uint32_t>(dc.mn == Mn::Adda ? an + v : an - v);
            }
            break;
        }
        case Mn::Addq: case Mn::Subq: {
            const std::uint32_t q = static_cast<std::uint32_t>(ops[0].value);
            const Op& dst = ops[1];
            if (dst.kind == OpKind::Areg) {   // to an address register: all 32 bits, no flags
                a_[dst.reg] = dc.mn == Mn::Addq ? a_[dst.reg] + q : a_[dst.reg] - q;
                break;
            }
            modify(dst, size, [&](std::uint32_t x) -> std::optional<std::uint32_t> {
                return dc.mn == Mn::Addq ? add(x, q, size) : sub(x, q, size);
            });
            break;
        }
        case Mn::Addx: case Mn::Subx: {
            const std::uint32_t b = get(ops[0], size);
            modify(ops[1], size, [&](std::uint32_t x) -> std::optional<std::uint32_t> {
                const std::uint32_t extend = flag(flag::X) ? 1u : 0u;
                return dc.mn == Mn::Addx ? add(x, b, size, extend, true) : sub(x, b, size, extend, true);
            });
            break;
        }
        case Mn::Cmpm: {
            const std::uint32_t b = get(ops[0], size);
            const std::uint32_t x = get(ops[1], size);
            sub(x, b, size, 0, false, false);
            break;
        }
        case Mn::Mulu: case Mn::Muls: {
            const std::uint32_t b = get(ops[0], Size::Word);
            const std::uint32_t x = d_[ops[1].reg] & 0xFFFFu;
            const std::int64_t product = dc.mn == Mn::Muls ? sx(x, Size::Word) * sx(b, Size::Word)
                                                            : static_cast<std::int64_t>(x) * b;
            const std::uint32_t r = static_cast<std::uint32_t>(product);
            d_[ops[1].reg] = r;
            logic(r, Size::Long);
            break;
        }
        case Mn::Divu: return divide(dc, false);
        case Mn::Divs: return divide(dc, true);
        case Mn::Nbcd:
            modify(ops[0], Size::Byte, [&](std::uint32_t v) -> std::optional<std::uint32_t> { return bcd_sub(0, v); });
            break;
        case Mn::Abcd: case Mn::Sbcd: {
            const std::uint32_t b = get(ops[0], Size::Byte);
            modify(ops[1], Size::Byte, [&](std::uint32_t x) -> std::optional<std::uint32_t> {
                return dc.mn == Mn::Abcd ? bcd_add(x, b) : bcd_sub(x, b);
            });
            break;
        }
        case Mn::Shift:
            if (insn.op_count == 1) {   // memory: one bit, a word
                modify(ops[0], Size::Word, [&](std::uint32_t v) -> std::optional<std::uint32_t> {
                    return shift(dc, v, 1, Size::Word);
                });
            } else {
                const unsigned count = ops[0].kind == OpKind::Quick ? static_cast<unsigned>(ops[0].value) : (d_[ops[0].reg] & 63u);
                modify(ops[1], size, [&](std::uint32_t v) -> std::optional<std::uint32_t> { return shift(dc, v, count, size); });
            }
            break;
        case Mn::Btst: case Mn::Bchg: case Mn::Bclr: case Mn::Bset: {
            const Op& dst = ops[1];
            const bool on_register = dst.kind == OpKind::Dreg;
            const unsigned bit = get(ops[0], Size::Long) % (on_register ? 32u : 8u);   // a register takes the bit number mod 32, memory mod 8
            modify(dst, on_register ? Size::Long : Size::Byte, [&](std::uint32_t v) -> std::optional<std::uint32_t> {
                set_flag(flag::Z, ((v >> bit) & 1) == 0);
                switch (dc.mn) {
                    case Mn::Btst: return std::nullopt;
                    case Mn::Bchg: return v ^ (1u << bit);
                    case Mn::Bclr: return v & ~(1u << bit);
                    default: return v | (1u << bit);
                }
            });
            break;
        }
        case Mn::Bcc:
            if (dc.cc == Cond::SR) {
                push(cpu_.pc);
                cpu_.pc = dc.target;
            } else if (condition(dc.cc)) {
                cpu_.pc = dc.target;
            }
            break;
        case Mn::Dbcc: {
            if (condition(dc.cc)) break;
            std::uint32_t& dn = d_[ops[0].reg];
            const std::uint32_t v = (dn - 1) & 0xFFFFu;
            dn = (dn & 0xFFFF0000u) | v;
            if (v != 0xFFFFu) cpu_.pc = dc.target;
            break;
        }
        case Mn::Scc: put(ops[0], Size::Byte, condition(dc.cc) ? 0xFFu : 0u); break;
        case Mn::Jmp: cpu_.pc = address(ops[0], Size::Long); break;
        case Mn::Jsr: {
            const std::uint32_t t = address(ops[0], Size::Long);
            push(cpu_.pc);
            cpu_.pc = t;
            break;
        }
        case Mn::Rts: cpu_.pc = pop(); break;
        case Mn::Rtr:
            cpu_.sr = static_cast<std::uint16_t>((cpu_.sr & 0xFF00u) | (pop(Size::Word) & 0x1Fu));
            cpu_.pc = pop();
            break;
        case Mn::Rte:
            cpu_.sr = static_cast<std::uint16_t>(pop(Size::Word) & 0xA71Fu);
            cpu_.pc = pop();
            break;
        case Mn::Link: {
            const unsigned reg = ops[0].reg;
            push(a_[reg]);
            a_[reg] = a_[7];
            a_[7] += static_cast<std::uint32_t>(sx(static_cast<std::uint32_t>(ops[1].value), Size::Word));
            break;
        }
        case Mn::Unlk: {
            const unsigned reg = ops[0].reg;
            a_[7] = a_[reg];
            a_[reg] = pop();
            break;
        }
        case Mn::Chk: {
            const std::int64_t bound = sx(get(ops[0], Size::Word), Size::Word);
            const std::int64_t v = sx(d_[ops[1].reg], Size::Word);
            if (v < 0 || v > bound) {
                set_flag(flag::N, v < 0);
                return here(Fault::Kind::Check);
            }
            break;
        }
        case Mn::Nop: break;
        case Mn::Trap: return here(Fault::Kind::Trap);
        case Mn::Illegal: return here(Fault::Kind::Illegal);
        case Mn::Stop: return here(Fault::Kind::Stop);
        case Mn::Trapv:
            if (flag(flag::V)) return here(Fault::Kind::TrapOverflow);
            break;
        case Mn::NotRun: return Fault{Fault::Kind::NotRun, insn.pc, insn.mnemonic};
    }
    return std::nullopt;
}

}  // namespace

// ---- BusAccess, Bus -----------------------------------------------------------------------------------------------

std::string BusAccess::text() const { return std::format("{}{}", kind == Kind::Read ? 'r' : 'w', size); }

Bus::Bus(std::size_t size) : owned_(size, 0), ram_(owned_) {}

Bus::Bus(ByteView image, std::size_t size) : Bus(size) {
    if (image.size() > size) throw UsageError(std::format("an image of {} bytes does not fit a bus of {} bytes", image.size(), size));
    std::copy(image.begin(), image.end(), owned_.begin());
}

Bus Bus::over(ByteSpan ram) {
    Bus bus(std::size_t{0});
    bus.ram_ = ram;
    return bus;
}

void Bus::map(std::uint32_t lo, std::uint32_t hi, ReadFn read, WriteFn write) {
    ranges_.push_back({lo, hi, std::move(read), std::move(write)});
}

std::uint32_t Bus::read(std::uint32_t address, unsigned size) {
    address &= kAddressMask;
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            const std::uint32_t v = r.read ? (r.read(address, size) & mask_bytes(size)) : 0;
            log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), address, v});
            return v;
        }
    }
    if (static_cast<std::uint64_t>(address) + size > ram_.size()) return 0;   // beyond the machine's memory
    std::uint32_t v = 0;
    for (unsigned i = 0; i < size; ++i) v = (v << 8) | ram_[address + i];
    return v;
}

void Bus::write(std::uint32_t address, unsigned size, std::uint32_t value) {
    address &= kAddressMask;
    value &= mask_bytes(size);
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), address, value});
            if (r.write) r.write(address, size, value);
            return;
        }
    }
    if (static_cast<std::uint64_t>(address) + size > ram_.size()) return;
    for (unsigned i = 0; i < size; ++i) ram_[address + i] = static_cast<std::uint8_t>(value >> (8 * (size - 1 - i)));
}

// ---- Fault, Cpu ---------------------------------------------------------------------------------------------------

std::string Fault::text() const {
    switch (kind) {
        case Kind::NoInstruction: return std::format("no instruction at {:06X}", pc);
        case Kind::NotRun: return "not run: " + mnemonic;
        case Kind::DivideByZero: return "division by zero";
        case Kind::Check: return "CHK exception";
        case Kind::TrapOverflow: return "TRAPV exception";
        case Kind::Trap: return "TRAP";
        case Kind::Illegal: return "ILLEGAL";
        case Kind::Stop: return "STOP";
    }
    return {};
}

Cpu::Cpu(Bus& bus) : bus_(&bus), cache_(std::make_unique<DecodedCache>()) {}
Cpu::~Cpu() = default;

std::optional<Fault> Cpu::step() {
    const std::uint32_t at = pc;
    auto& table = cache_->by_address;
    auto it = table.find(at);
    if (it == table.end()) {
        // the bytes under it, read as the CPU fetches them (one at a time: a device sees each access)
        std::array<std::uint8_t, 10> code{};
        for (std::size_t i = 0; i < code.size(); ++i) code[i] = static_cast<std::uint8_t>(bus_->read(at + static_cast<std::uint32_t>(i), 1));
        std::optional<dec::Insn> insn = dec::decode(code, 0, at);
        if (!insn) return Fault{Fault::Kind::NoInstruction, at, {}};
        it = table.emplace(at, prepare(std::move(*insn))).first;
    }
    const Decoded& dc = it->second;
    pc = at + dc.insn.length;
    if (std::optional<Fault> fault = Executor(*this, *bus_).execute(dc)) return fault;
    ++steps;
    return std::nullopt;
}

// ---- running a function -------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string());
        case End::Limit: break;
    }
    return "limit";
}

RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options) {
    Cpu cpu(bus);
    const std::size_t log_start = bus.log().size();
    if (options.d) cpu.d = *options.d;
    if (options.a) cpu.a = *options.a;
    cpu.a[7] = options.sp;
    cpu.sr = options.sr;
    // the call: the return address on the stack (a mapped stack would log this write)
    cpu.a[7] -= 4;
    bus.write(cpu.a[7], 4, kReturnAddress);
    cpu.pc = entry;

    RunResult out;
    while (cpu.steps < options.max_steps) {
        if (cpu.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        if (std::optional<Fault> fault = cpu.step()) {
            out.end = End::Stopped;
            out.fault = std::move(fault);
            break;
        }
    }
    out.d = cpu.d;
    out.a = cpu.a;
    out.sr = cpu.sr;
    out.steps = cpu.steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

cb::emulate::Run record(const RunResult& result) {
    cb::emulate::Run run;
    run.exit = result.exit();
    for (unsigned i = 0; i < 8; ++i) run.results[std::format("D{}", i)] = static_cast<std::uint64_t>(result.d[i]);
    for (unsigned i = 0; i < 8; ++i) run.results[std::format("A{}", i)] = static_cast<std::uint64_t>(result.a[i]);
    run.results["SR"] = static_cast<std::uint64_t>(result.sr);
    for (const BusAccess& access : result.io) {
        cb::emulate::IoAccess io;
        io.kind = access.text();
        io.port = access.address;
        io.value = access.value;
        io.size = access.size;
        run.io.push_back(std::move(io));
    }
    run.steps = result.steps;
    run.truncated = result.end == End::Limit;
    run.endian = cb::emulate::Endian::Big;
    run.raw = result;
    return run;
}

cb::emulate::Run recorded(Bus& bus, std::uint32_t entry, const RunOptions& options) { return record(run(bus, entry, options)); }

}  // namespace cb::emulate::m68k
