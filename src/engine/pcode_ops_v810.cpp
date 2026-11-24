// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_ops_v810.cpp -- what the Virtual Boy adapter (pcode_cpu_v810.cpp) does itself for the instructions the description of the V810 does not carry the way the
// processor does: the floating-point instructions (the flags the reserved operand, the zero divide and the overflow set), the ten bit strings, ldsr and stsr (the system
// registers and the writable bits of the status word), caxi, trap, reti and halt (they end a run), a division by zero and the division that overflows, and a bit
// pattern that is no instruction (the faults). It works on the registers as the adapter hands them over, through the Bus, and gives the fault a run ends with.
#include "engine/pcode_ops_v810.hpp"

#include <algorithm>
#include <bit>
#include <climits>
#include <cmath>
#include <format>

namespace cb::engine::pcode_ops_v810 {

namespace dv = cb::decompile::v810;
using cb::emulate::v810::Bus;
using cb::emulate::v810::Fault;
using cb::emulate::v810::Registers;
namespace psw = cb::emulate::v810::psw;

namespace {

float as_float(std::uint32_t v) { return std::bit_cast<float>(v); }
std::uint32_t as_bits(float f) { return std::bit_cast<std::uint32_t>(f); }

bool reserved_operand(std::uint32_t v) {   // a NaN, an infinity is fine; a denormal is not
    const std::uint32_t exp = (v >> 23) & 0xFF, mant = v & 0x7FFFFF;
    return (exp == 0xFF && mant != 0) || (exp == 0 && mant != 0);
}

struct BitRef {
    std::uint32_t address;
    int offset;
};

struct Exec {
    Registers& g;
    Bus& bus;

    std::uint32_t& r(unsigned n) { return g.r[n]; }
    void flag(std::uint32_t bit, bool on) { g.psw = on ? (g.psw | bit) : (g.psw & ~bit); }
    void zs(std::uint32_t v) {
        flag(psw::Z, v == 0);
        flag(psw::S, (v >> 31) != 0);
    }
    void logic(std::uint32_t v) {
        zs(v);
        flag(psw::OV, false);
    }
    std::uint32_t add(std::uint32_t a, std::uint32_t b) {
        const std::uint64_t wide = static_cast<std::uint64_t>(a) + b;
        const std::uint32_t res = static_cast<std::uint32_t>(wide);
        zs(res);
        flag(psw::OV, (((a ^ res) & (b ^ res)) >> 31) != 0);
        flag(psw::CY, (wide >> 32) != 0);
        return res;
    }
    std::uint32_t sub(std::uint32_t a, std::uint32_t b) {
        const std::uint32_t res = a - b;
        zs(res);
        flag(psw::OV, (((a ^ b) & (a ^ res)) >> 31) != 0);
        flag(psw::CY, a < b);
        return res;
    }
    // a shift of `v` by `n` (0..31): kind 0 left, 1 logical right, 2 arithmetic right
    std::uint32_t shift(std::uint32_t v, unsigned n, int kind) {
        std::uint32_t res = v;
        bool carry = false;
        if (n != 0) {
            if (kind == 0) {
                carry = ((v >> (32 - n)) & 1) != 0;
                res = v << n;
            } else {
                carry = ((v >> (n - 1)) & 1) != 0;
                res = kind == 1 ? v >> n : static_cast<std::uint32_t>(static_cast<std::int32_t>(v) >> n);
            }
        }
        zs(res);
        flag(psw::OV, false);
        flag(psw::CY, carry);
        return res;
    }
    bool condition(unsigned c) const {
        const bool z = g.psw & psw::Z, s = g.psw & psw::S, ov = g.psw & psw::OV, cy = g.psw & psw::CY;
        switch (c & 15) {
            case 0: return ov;
            case 1: return cy;
            case 2: return z;
            case 3: return cy || z;
            case 4: return s;
            case 5: return true;
            case 6: return s != ov;
            case 7: return (s != ov) || z;
            case 8: return !ov;
            case 9: return !cy;
            case 10: return !z;
            case 11: return !(cy || z);
            case 12: return !s;
            case 13: return false;
            case 14: return s == ov;
            default: return !((s != ov) || z);
        }
    }
    std::uint32_t system_register(unsigned n) const {
        switch (n) {
            case 0: return g.eipc;
            case 1: return g.eipsw;
            case 2: return g.fepc;
            case 3: return g.fepsw;
            case 4: return g.ecr;
            case 5: return g.psw;
            case 6: return 0x00005346;
            case 7: return 0x000000E0;
            case 24: return g.chcw;
            case 25: return g.adtre;
            default: return 0;
        }
    }
    void set_system_register(unsigned n, std::uint32_t v) {
        switch (n) {
            case 0: g.eipc = v; break;
            case 1: g.eipsw = v; break;
            case 2: g.fepc = v; break;
            case 3: g.fepsw = v; break;
            case 5: g.psw = v & psw::kWritable; break;
            case 24: g.chcw = v; break;
            case 25: g.adtre = v; break;
            default: break;   // (ecr, pir and tkcw are read-only here)
        }
    }

    // ---- floating point ---------------------------------------------------------------------------------------------
    void float_result(float res) {
        std::uint32_t bits = as_bits(res);
        if (std::isinf(res)) flag(psw::FOV, true);
        if (res != 0 && std::fpclassify(res) == FP_SUBNORMAL) {   // (flushed to zero, with the underflow flag)
            flag(psw::FUD, true);
            bits = 0;
        }
        zs(bits);
        flag(psw::OV, false);
        flag(psw::CY, (bits >> 31) != 0);
    }
    // r2 = r2 op r1 for the four arithmetic ones; kind: 0 add, 1 sub, 2 mul, 3 div; 4 compare
    void float_op(unsigned a_reg, unsigned b_reg, int kind) {
        const std::uint32_t av = r(a_reg), bv = r(b_reg);
        if (reserved_operand(av) || reserved_operand(bv)) {
            flag(psw::FRO, true);
            return;
        }
        const float a = as_float(av), b = as_float(bv);
        if (kind == 4) {
            flag(psw::Z, a == b);
            flag(psw::S, a < b);
            flag(psw::OV, false);
            flag(psw::CY, a < b);
            return;
        }
        if (kind == 3 && b == 0) {
            flag(psw::FZD, true);
            return;
        }
        const float res = kind == 0 ? a + b : kind == 1 ? a - b : kind == 2 ? a * b : a / b;
        float_result(res);
        r(a_reg) = std::bit_cast<std::uint32_t>(std::fpclassify(res) == FP_SUBNORMAL ? 0.0f : res);
    }

    // ---- bit strings ------------------------------------------------------------------------------------------------
    bool get(const BitRef& b) { return ((bus.read(b.address & ~3u, 4) >> b.offset) & 1) != 0; }
    void put(const BitRef& b, bool v) {
        std::uint32_t w = bus.read(b.address & ~3u, 4);
        w = v ? (w | 1u << b.offset) : (w & ~(1u << b.offset));
        bus.write(b.address & ~3u, 4, w);
    }
    static void step_up(BitRef& b) {
        if (++b.offset == 32) {
            b.offset = 0;
            b.address += 4;
        }
    }
    static void step_down(BitRef& b) {
        if (--b.offset < 0) {
            b.offset = 31;
            b.address -= 4;
        }
    }
    void bit_string(unsigned sub) {
        BitRef src{r(30), static_cast<int>(r(27) & 31)}, dst{r(29), static_cast<int>(r(26) & 31)};
        std::uint32_t len = r(28);
        if (sub < 4) {   // search for a 0 (sub 0, 1) or a 1 (2, 3), upward (even) or downward (odd)
            const bool want = sub >= 2, down = (sub & 1) != 0;
            const std::uint32_t total = len;
            bool found = false;
            while (len > 0) {
                if (get(src) == want) {
                    found = true;
                    break;
                }
                down ? step_down(src) : step_up(src);
                --len;
            }
            r(29) = total - len;
            r(30) = src.address;
            r(27) = static_cast<std::uint32_t>(src.offset);
            r(28) = len;
            flag(psw::Z, !found);
            flag(psw::S, false);
            flag(psw::OV, false);
            flag(psw::CY, false);
            return;
        }
        for (; len > 0; --len) {
            const bool s = get(src), d = get(dst);
            bool n = false;
            switch (sub) {
                case 8: n = d || s; break;
                case 9: n = d && s; break;
                case 10: n = d != s; break;
                case 11: n = s; break;
                case 12: n = d || !s; break;
                case 13: n = d && !s; break;
                case 14: n = d == s; break;
                default: n = !s; break;
            }
            put(dst, n);
            step_up(src);
            step_up(dst);
        }
        r(30) = src.address;
        r(27) = static_cast<std::uint32_t>(src.offset);
        r(29) = dst.address;
        r(26) = static_cast<std::uint32_t>(dst.offset);
        r(28) = 0;
    }
};

std::string at(std::uint32_t pc) { return std::format(" at {:08X}", pc); }

}  // namespace

// The instruction at g.pc, fetched through the bus as the processor does (a halfword, and a second when the first says there is one): what the bus logs is logged.
std::optional<dv::Insn> fetch(const Registers& g, Bus& bus) {
    std::array<std::uint8_t, 4> buf{};
    const std::uint32_t hw0 = bus.read(g.pc, 2);
    buf[0] = static_cast<std::uint8_t>(hw0);
    buf[1] = static_cast<std::uint8_t>(hw0 >> 8);
    std::size_t n = 2;
    if ((hw0 >> 13) != 4 && (hw0 >> 10) >= 0x28) {
        const std::uint32_t hw1 = bus.read(g.pc + 2, 2);
        buf[2] = static_cast<std::uint8_t>(hw1);
        buf[3] = static_cast<std::uint8_t>(hw1 >> 8);
        n = 4;
    }
    return dv::decode(ByteView(buf.data(), n), 0, g.pc);
}

std::optional<Fault> step(Registers& g, Bus& bus, std::uint64_t& steps) { return step(g, bus, steps, fetch(g, bus)); }

std::optional<Fault> step(Registers& g, Bus& bus, std::uint64_t& steps, const std::optional<dv::Insn>& decoded) {
    Bus* bus_ = &bus;
    if (!decoded) return Fault{g.pc, "undefined instruction" + at(g.pc)};
    const dv::Insn& i = *decoded;
    Exec x{g, *bus_};
    std::uint32_t next = g.pc + i.size;
    const auto addr = [&] { return x.r(i.r1) + static_cast<std::uint32_t>(i.imm); };
    const auto sext8 = [](std::uint32_t v) { return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(v))); };
    const auto sext16 = [](std::uint32_t v) { return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v))); };
    const std::uint32_t a = x.r(i.r2), b = x.r(i.r1);   // (for the register forms)
    using namespace dv::id;
    switch (i.id) {
        case kMov: x.r(i.r2) = b; break;
        case kAdd: x.r(i.r2) = x.add(a, b); break;
        case kSub: x.r(i.r2) = x.sub(a, b); break;
        case kCmp: (void)x.sub(a, b); break;
        case kShl: x.r(i.r2) = x.shift(a, b & 31, 0); break;
        case kShr: x.r(i.r2) = x.shift(a, b & 31, 1); break;
        case kSar: x.r(i.r2) = x.shift(a, b & 31, 2); break;
        case kJmp: next = b & ~1u; break;
        case kMul: {
            const std::int64_t p = static_cast<std::int64_t>(static_cast<std::int32_t>(a)) * static_cast<std::int32_t>(b);
            const std::uint32_t lo = static_cast<std::uint32_t>(p);
            x.r(30) = static_cast<std::uint32_t>(p >> 32);
            x.r(i.r2) = lo;
            x.zs(lo);
            x.flag(psw::OV, p != static_cast<std::int64_t>(static_cast<std::int32_t>(lo)));
            break;
        }
        case kMulu: {
            const std::uint64_t p = static_cast<std::uint64_t>(a) * b;
            const std::uint32_t lo = static_cast<std::uint32_t>(p);
            x.r(30) = static_cast<std::uint32_t>(p >> 32);
            x.r(i.r2) = lo;
            x.zs(lo);
            x.flag(psw::OV, (p >> 32) != 0);
            break;
        }
        case kDiv: {
            if (b == 0) return Fault{g.pc, "divide by zero" + at(g.pc)};
            const std::int32_t sa = static_cast<std::int32_t>(a), sb = static_cast<std::int32_t>(b);
            std::int32_t q = 0, rem = 0;
            bool ov = false;
            if (sa == INT32_MIN && sb == -1) {
                q = INT32_MIN;
                ov = true;
            } else {
                q = sa / sb;
                rem = sa % sb;
            }
            x.r(30) = static_cast<std::uint32_t>(rem);
            x.r(i.r2) = static_cast<std::uint32_t>(q);
            x.zs(static_cast<std::uint32_t>(q));
            x.flag(psw::OV, ov);
            break;
        }
        case kDivu: {
            if (b == 0) return Fault{g.pc, "divide by zero" + at(g.pc)};
            x.r(30) = a % b;
            x.r(i.r2) = a / b;
            x.zs(x.r(i.r2));
            x.flag(psw::OV, false);
            break;
        }
        case kOr: x.r(i.r2) = a | b; x.logic(x.r(i.r2)); break;
        case kAnd: x.r(i.r2) = a & b; x.logic(x.r(i.r2)); break;
        case kXor: x.r(i.r2) = a ^ b; x.logic(x.r(i.r2)); break;
        case kNot: x.r(i.r2) = ~b; x.logic(x.r(i.r2)); break;
        case kMovI: x.r(i.r2) = static_cast<std::uint32_t>(i.imm); break;
        case kAddI5: x.r(i.r2) = x.add(a, static_cast<std::uint32_t>(i.imm)); break;
        case kCmpI5: (void)x.sub(a, static_cast<std::uint32_t>(i.imm)); break;
        case kSetf: x.r(i.r2) = x.condition(static_cast<unsigned>(i.imm)) ? 1 : 0; break;
        case kShlI5: x.r(i.r2) = x.shift(a, static_cast<unsigned>(i.imm), 0); break;
        case kShrI5: x.r(i.r2) = x.shift(a, static_cast<unsigned>(i.imm), 1); break;
        case kSarI5: x.r(i.r2) = x.shift(a, static_cast<unsigned>(i.imm), 2); break;
        case kCli: g.psw &= ~psw::ID; break;
        case kSei: g.psw |= psw::ID; break;
        case kTrap: return Fault{g.pc, std::format("trap {}", i.imm) + at(g.pc)};
        case kReti: return Fault{g.pc, "return from interrupt" + at(g.pc)};
        case kHalt: return Fault{g.pc, "halt" + at(g.pc)};
        case kLdsr: x.set_system_register(static_cast<unsigned>(i.imm), a); break;
        case kStsr: x.r(i.r2) = x.system_register(static_cast<unsigned>(i.imm)); break;
        case kMovea: x.r(i.r2) = b + static_cast<std::uint32_t>(i.imm); break;
        case kAddi: x.r(i.r2) = x.add(b, static_cast<std::uint32_t>(i.imm)); break;
        case kOri: x.r(i.r2) = b | static_cast<std::uint32_t>(i.imm); x.logic(x.r(i.r2)); break;
        case kAndi: x.r(i.r2) = b & static_cast<std::uint32_t>(i.imm); x.logic(x.r(i.r2)); break;
        case kXori: x.r(i.r2) = b ^ static_cast<std::uint32_t>(i.imm); x.logic(x.r(i.r2)); break;
        case kMovhi: x.r(i.r2) = b + (static_cast<std::uint32_t>(i.imm) << 16); break;
        case kJr: next = *i.target; break;
        case kJal: x.r(31) = g.pc + 4; next = *i.target; break;
        case kLdB: x.r(i.r2) = sext8(bus_->read(addr(), 1)); break;
        case kLdH: x.r(i.r2) = sext16(bus_->read(addr(), 2)); break;
        case kLdW:
        case kInW: x.r(i.r2) = bus_->read(addr(), 4); break;
        case kInB: x.r(i.r2) = bus_->read(addr(), 1); break;
        case kInH: x.r(i.r2) = bus_->read(addr(), 2); break;
        case kStB:
        case kOutB: bus_->write(addr(), 1, a); break;
        case kStH:
        case kOutH: bus_->write(addr(), 2, a); break;
        case kStW:
        case kOutW: bus_->write(addr(), 4, a); break;
        case kCaxi: {
            const std::uint32_t at_address = addr();
            const std::uint32_t old = bus_->read(at_address, 4);
            (void)x.sub(a, old);
            if (a == old) bus_->write(at_address, 4, x.r(30));
            x.r(i.r2) = old;
            break;
        }
        case kCmpfS: x.float_op(i.r2, i.r1, 4); break;
        case kAddfS: x.float_op(i.r2, i.r1, 0); break;
        case kSubfS: x.float_op(i.r2, i.r1, 1); break;
        case kMulfS: x.float_op(i.r2, i.r1, 2); break;
        case kDivfS: x.float_op(i.r2, i.r1, 3); break;
        case kCvtWs: x.float_result(static_cast<float>(static_cast<std::int32_t>(b))); x.r(i.r2) = as_bits(static_cast<float>(static_cast<std::int32_t>(b))); break;
        case kCvtSw:
        case kTrncSw: {
            if (reserved_operand(b)) {
                x.flag(psw::FRO, true);
                break;
            }
            const float f = as_float(b);
            const float rounded = i.id == kCvtSw ? std::nearbyint(f) : std::trunc(f);
            if (!(rounded >= -2147483648.0f && rounded < 2147483648.0f)) {
                x.flag(psw::FIV, true);
                break;
            }
            x.r(i.r2) = static_cast<std::uint32_t>(static_cast<std::int32_t>(rounded));
            x.zs(x.r(i.r2));
            x.flag(psw::OV, false);
            break;
        }
        case kXb: x.r(i.r2) = (a & 0xFFFF0000u) | (a & 0xFF) << 8 | ((a >> 8) & 0xFF); break;
        case kXh: x.r(i.r2) = std::rotl(a, 16); break;
        case kRev: {
            std::uint32_t v = 0;
            for (unsigned k = 0; k < 32; ++k) v |= ((b >> k) & 1u) << (31 - k);
            x.r(i.r2) = v;
            break;
        }
        case kMpyhw: x.r(i.r2) = a * sext16(b); break;
        default:
            if (i.id == kBranch) {
                if (x.condition(i.cond)) next = *i.target;
            } else if (i.id >= kBitStringBase && i.id < kExtendedBase) {
                if (x.r(28) > (1u << 20)) return Fault{g.pc, "bit string over 1 Mbit" + at(g.pc)};
                x.bit_string(i.id - kBitStringBase);
            } else {
                return Fault{g.pc, "undefined instruction" + at(g.pc)};
            }
            break;
    }
    g.r[0] = 0;
    g.pc = next;
    ++steps;
    return std::nullopt;
}

}  // namespace cb::engine::pcode_ops_v810
