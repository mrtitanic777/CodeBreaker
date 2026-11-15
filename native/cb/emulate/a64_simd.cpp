// cb-unity: alone
// cb/emulate/a64_simd.cpp -- the Advanced SIMD instructions of the AArch64 interpreter (emulate/a64.hpp): structure loads and stores, the copy group, modified immediates,
// the integer and floating-point arithmetic of vectors and their scalar forms, shifts, narrowing and widening, permutes, table lookups, reductions. What is not here stops the
// run (the caller says "not run").
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>
#include <limits>

#include "cb/decompile/a64.hpp"
#include "cb/emulate/a64.hpp"

namespace cb::emulate::a64::detail {

namespace da = decompile::a64;
using u64 = std::uint64_t;
using i64 = std::int64_t;
using u32 = std::uint32_t;
using B16 = std::array<std::uint8_t, 16>;

namespace {

B16 to_b(const V128& v) {
    B16 b;
    std::memcpy(b.data(), &v.lo, 8);
    std::memcpy(b.data() + 8, &v.hi, 8);
    return b;
}
V128 from_b(const B16& b) {
    V128 v;
    std::memcpy(&v.lo, b.data(), 8);
    std::memcpy(&v.hi, b.data() + 8, 8);
    return v;
}
u64 get(const B16& b, unsigned eb, unsigned i) {
    u64 v = 0;
    std::memcpy(&v, b.data() + i * eb, eb);
    return v;
}
void put(B16& b, unsigned eb, unsigned i, u64 v) { std::memcpy(b.data() + i * eb, &v, eb); }
u64 mask_bits(unsigned bits) { return bits >= 64 ? ~0ull : (1ull << bits) - 1; }
i64 sext(u64 v, unsigned bits) {
    if (bits >= 64) return static_cast<i64>(v);
    const u64 m = 1ull << (bits - 1);
    return static_cast<i64>(((v & mask_bits(bits)) ^ m) - m);
}
i64 smax_of(unsigned bits) { return bits >= 64 ? std::numeric_limits<i64>::max() : (i64{1} << (bits - 1)) - 1; }
i64 smin_of(unsigned bits) { return bits >= 64 ? std::numeric_limits<i64>::min() : -(i64{1} << (bits - 1)); }
i64 clamp_signed(i64 v, unsigned bits) { return std::clamp(v, smin_of(bits), smax_of(bits)); }
u64 clamp_unsigned(i64 v, unsigned bits) { return v < 0 ? 0 : (static_cast<u64>(v) > mask_bits(bits) ? mask_bits(bits) : static_cast<u64>(v)); }

struct Fields {
    std::uint32_t w;
    unsigned q() const { return (w >> 30) & 1; }
    unsigned u() const { return (w >> 29) & 1; }
    unsigned size() const { return (w >> 22) & 3; }
    unsigned rd() const { return w & 31; }
    unsigned rn() const { return (w >> 5) & 31; }
    unsigned rm() const { return (w >> 16) & 31; }
    unsigned data() const { return q() ? 16 : 8; }   // bytes in the vector
    unsigned bits(unsigned pos, unsigned n) const { return (w >> pos) & ((1u << n) - 1); }
};

using Fn2 = i64 (*)(i64, i64);

}  // namespace

std::optional<std::string> run_simd(Registers& R, Memory& mem, const da::Insn& insn) {
    const std::uint32_t w = insn.word;
    const da::Op op = insn.op;
    const Fields f{w};
    const auto unsupported = [&](const char* why) -> std::optional<std::string> { return std::format("simd: {} is not run ({}) at {:#x}", da::text(insn), why, insn.at); };
    const unsigned rd = f.rd(), rn = f.rn(), rm = f.rm();
    const unsigned size = f.size(), eb = 1u << size, elems = f.data() / eb;
    B16 d = to_b(R.v[rd]), n = to_b(R.v[rn]), m = to_b(R.v[rm]);
    B16 out{};
    const auto finish = [&](const B16& r) {
        B16 x = r;
        if (!f.q()) std::fill(x.begin() + 8, x.end(), std::uint8_t{0});
        R.v[rd] = from_b(x);
        return std::optional<std::string>();
    };
    const auto base_reg = [&](unsigned r) -> u64 { return r == 31 ? R.sp : R.x[r]; };
    const auto set_base = [&](unsigned r, u64 v) {
        if (r == 31) R.sp = v;
        else R.x[r] = v;
    };
    const auto fault = [&](u64 a) { return std::optional<std::string>(std::format("data abort: simd access at {:#x}", a)); };

    switch (op) {
        // ---- structure loads and stores ----
        case da::Op::SimdLdStMulti: {
            const unsigned opcode = f.bits(12, 4), t = rd;
            const bool load = f.bits(22, 1);
            const unsigned ls = f.bits(10, 2), ebytes = 1u << ls, el = f.data() / ebytes;
            unsigned rpt, selem;
            switch (opcode) {
                case 0: rpt = 1; selem = 4; break;
                case 2: rpt = 4; selem = 1; break;
                case 4: rpt = 1; selem = 3; break;
                case 6: rpt = 3; selem = 1; break;
                case 7: rpt = 1; selem = 1; break;
                case 8: rpt = 1; selem = 2; break;
                default: rpt = 2; selem = 1; break;
            }
            u64 addr = base_reg(rn);
            const u64 start = addr;
            std::array<B16, 32> regs;
            for (unsigned r = 0; r < 32; ++r) regs[r] = to_b(R.v[r]);
            for (unsigned r = 0; r < rpt; ++r)
                for (unsigned e = 0; e < el; ++e)
                    for (unsigned s = 0; s < selem; ++s) {
                        const unsigned reg = (t + r + s) % 32;
                        if (load) {
                            u64 v;
                            if (!mem.read(addr, ebytes, v)) return fault(addr);
                            put(regs[reg], ebytes, e, v);
                        } else if (!mem.write(addr, ebytes, get(regs[reg], ebytes, e))) {
                            return fault(addr);
                        }
                        addr += ebytes;
                    }
            if (load)
                for (unsigned r = 0; r < rpt; ++r)
                    for (unsigned s = 0; s < selem; ++s) {
                        B16 x = regs[(t + r + s) % 32];
                        if (!f.q()) std::fill(x.begin() + 8, x.end(), std::uint8_t{0});
                        R.v[(t + r + s) % 32] = from_b(x);
                    }
            if ((w >> 23) & 1) {
                const unsigned mreg = f.rm();
                set_base(rn, mreg == 31 ? start + static_cast<u64>(rpt) * selem * f.data() : start + (mreg == 31 ? 0 : R.x[mreg]));
            }
            return std::nullopt;
        }
        case da::Op::SimdLdStSingle: {
            const bool load = f.bits(22, 1) != 0, r_bit = f.bits(21, 1) != 0, replicate = f.bits(13, 3) >= 6;
            const unsigned opc3 = f.bits(13, 3), s_bit = f.bits(12, 1), ls = f.bits(10, 2);
            const unsigned selem = ((opc3 & 1u) << 1 | (r_bit ? 1u : 0u)) + 1;
            unsigned ebytes, index = 0;
            if (replicate) {
                ebytes = 1u << ls;
            } else {
                const unsigned scale = opc3 >> 1;
                if (scale == 0) {
                    ebytes = 1;
                    index = f.q() << 3 | s_bit << 2 | ls;
                } else if (scale == 1) {
                    ebytes = 2;
                    index = f.q() << 2 | s_bit << 1 | (ls >> 1);
                } else if (ls == 0) {
                    ebytes = 4;
                    index = f.q() << 1 | s_bit;
                } else {
                    ebytes = 8;
                    index = f.q();
                }
            }
            u64 addr = base_reg(rn);
            const u64 start = addr;
            for (unsigned s = 0; s < selem; ++s) {
                const unsigned reg = (rd + s) % 32;
                B16 x = to_b(R.v[reg]);
                if (replicate) {
                    u64 v;
                    if (!mem.read(addr, ebytes, v)) return fault(addr);
                    for (unsigned e = 0; e < f.data() / ebytes; ++e) put(x, ebytes, e, v);
                    if (!f.q()) std::fill(x.begin() + 8, x.end(), std::uint8_t{0});
                    R.v[reg] = from_b(x);
                } else if (load) {
                    u64 v;
                    if (!mem.read(addr, ebytes, v)) return fault(addr);
                    put(x, ebytes, index, v);
                    R.v[reg] = from_b(x);
                } else if (!mem.write(addr, ebytes, get(x, ebytes, index))) {
                    return fault(addr);
                }
                addr += ebytes;
            }
            if ((w >> 23) & 1) set_base(rn, start + (rm == 31 ? static_cast<u64>(selem) * ebytes : R.x[rm]));
            return std::nullopt;
        }
        // ---- copy ----
        case da::Op::SimdCopy: {
            const unsigned imm5 = f.bits(16, 5), imm4 = f.bits(11, 4), sz = static_cast<unsigned>(std::countr_zero(imm5)), ebytes = 1u << sz;
            if (imm5 == 0 || sz > 3) return unsupported("reserved");
            const unsigned index = imm5 >> (sz + 1);
            const u64 gp = rn == 31 ? 0 : R.x[rn];
            if (f.bits(29, 1) == 1) {   // ins (element)
                const unsigned src_index = imm4 >> sz;
                put(d, ebytes, index, get(n, ebytes, src_index));
                R.v[rd] = from_b(d);
                return std::nullopt;
            }
            switch (imm4) {
                case 0: {   // dup (element)
                    const u64 v = get(n, ebytes, index);
                    for (unsigned e = 0; e < f.data() / ebytes; ++e) put(out, ebytes, e, v);
                    return finish(out);
                }
                case 1: {   // dup (general)
                    for (unsigned e = 0; e < f.data() / ebytes; ++e) put(out, ebytes, e, gp);
                    return finish(out);
                }
                case 3:
                    put(d, ebytes, index, gp);
                    R.v[rd] = from_b(d);
                    return std::nullopt;
                case 5:
                case 7: {
                    u64 v = get(n, ebytes, index);
                    if (imm4 == 5) v = static_cast<u64>(sext(v, 8 * ebytes));
                    if (!f.q() || imm4 == 7) v &= mask_bits(8 * ebytes);
                    if (!f.q() && imm4 == 5) v &= 0xFFFFFFFFull;
                    if (rd != 31) R.x[rd] = v;
                    return std::nullopt;
                }
                default: return unsupported("reserved");
            }
        }
        case da::Op::SimdScalarCopy: {   // dup (element), scalar
            const unsigned imm5 = f.bits(16, 5), sz = static_cast<unsigned>(std::countr_zero(imm5)), ebytes = 1u << sz;
            if (imm5 == 0 || sz > 3) return unsupported("reserved");
            B16 x{};
            put(x, ebytes, 0, get(n, ebytes, imm5 >> (sz + 1)));
            R.v[rd] = from_b(x);
            return std::nullopt;
        }
        // ---- modified immediate ----
        case da::Op::SimdModImm: {
            const unsigned cmode = f.bits(12, 4), opbit = f.bits(29, 1), imm8 = (f.bits(16, 3) << 5) | f.bits(5, 5);
            u64 imm;
            const unsigned top = cmode >> 1;
            switch (top) {
                case 0: case 1: case 2: case 3: imm = static_cast<u64>(imm8) << (8 * top); imm |= imm << 32; break;
                case 4: case 5: imm = static_cast<u64>(imm8) << (8 * (top & 1)); imm |= imm << 16; imm |= imm << 32; break;
                case 6: imm = (cmode & 1) ? ((static_cast<u64>(imm8) << 16) | 0xFFFF) : ((static_cast<u64>(imm8) << 8) | 0xFF); imm |= imm << 32; break;
                default:
                    if ((cmode & 1) == 0 && opbit == 0) {
                        imm = imm8;
                        for (unsigned i = 1; i < 8; ++i) imm |= static_cast<u64>(imm8) << (8 * i);
                    } else if ((cmode & 1) == 0) {
                        imm = 0;
                        for (unsigned i = 0; i < 8; ++i)
                            if ((imm8 >> i) & 1) imm |= 0xFFull << (8 * i);
                    } else if (opbit == 0) {
                        const u32 bitsv = static_cast<u32>(((imm8 >> 7) << 31) | ((imm8 >> 6 & 1) ? 0x3E000000u : 0x40000000u) | ((imm8 & 0x3F) << 19));
                        imm = bitsv | (static_cast<u64>(bitsv) << 32);
                    } else {
                        imm = (static_cast<u64>(imm8 >> 7) << 63) | ((imm8 >> 6 & 1) ? 0x3FC0000000000000ull : 0x4000000000000000ull) | (static_cast<u64>(imm8 & 0x3F) << 48);
                    }
                    break;
            }
            const bool bitwise = (cmode & 1) && cmode < 12;   // orr / bic
            if (bitwise) {
                u64 dlo, dhi;
                std::memcpy(&dlo, d.data(), 8);
                std::memcpy(&dhi, d.data() + 8, 8);
                const u64 val = opbit ? ~imm : imm;
                if (opbit) {
                    dlo &= val;
                    dhi &= val;
                } else {
                    dlo |= val;
                    dhi |= val;
                }
                R.v[rd] = {dlo, f.q() ? dhi : 0};
                return std::nullopt;
            }
            if (opbit && cmode < 14 && !(cmode == 12 || cmode == 13)) imm = ~imm;   // mvni (the shifted forms)
            if (opbit && (cmode == 12 || cmode == 13)) imm = ~imm;
            R.v[rd] = {imm, f.q() ? imm : 0};
            return std::nullopt;
        }
        default: break;
    }

    const bool fp_size = f.bits(22, 1);   // sz of the floating-point forms
    const unsigned fb = fp_size ? 8 : 4, felems = f.data() / fb;

    switch (op) {
        // ---- three registers, same type ----
        case da::Op::SimdThree: {
            const unsigned opcode = f.bits(11, 5), u = f.u();
            if (opcode == 3) {   // logical
                const unsigned sel = size;
                for (unsigned i = 0; i < 16; ++i) {
                    const std::uint8_t a = n[i], b = m[i], x = d[i];
                    std::uint8_t r;
                    if (!u) r = sel == 0 ? (a & b) : sel == 1 ? (a & ~b) : sel == 2 ? (a | b) : (a | ~b);
                    else r = sel == 0 ? (a ^ b) : sel == 1 ? ((x & a) | (~x & b)) : sel == 2 ? ((x & ~b) | (a & b)) : ((x & b) | (a & ~b));
                    if (!u && sel == 2) r = a | b;
                    if (u && sel == 1) r = (x & a) | (~x & b);     // bsl: d selects between n and m
                    if (u && sel == 2) r = (a & b) | (x & ~b);     // bit: insert n where m is set
                    if (u && sel == 3) r = (a & ~b) | (x & b);     // bif: insert n where m is clear
                    out[i] = r;
                }
                return finish(out);
            }
            if (opcode >= 24) {   // floating point
                const unsigned a = f.bits(23, 1);
                const auto run = [&](auto zero) -> std::optional<std::string> {
                    using T = decltype(zero);
                    using I = std::conditional_t<sizeof(T) == 4, std::uint32_t, u64>;
                    const unsigned count = f.data() / sizeof(T);
                    std::array<T, 4> r{};
                    const auto rd_ = [&](const B16& b, unsigned i) { I v = static_cast<I>(get(b, sizeof(T), i)); return std::bit_cast<T>(v); };
                    const bool pairwise = u && (opcode == 24 || opcode == 26 || opcode == 30) && !(a && opcode == 26);
                    for (unsigned i = 0; i < count; ++i) {
                        T x, y;
                        if (pairwise) {
                            const unsigned k = 2 * i;
                            const B16& src = k < count ? n : m;
                            x = rd_(src, k % count);
                            y = rd_(src, k % count + 1);
                        } else {
                            x = rd_(n, i);
                            y = rd_(m, i);
                        }
                        const T acc = rd_(d, i);
                        T v;
                        const unsigned key = (u << 6) | (a << 5) | opcode;
                        switch (key) {
                            case (0u << 6) | (0u << 5) | 24: v = std::isnan(x) ? y : std::isnan(y) ? x : std::fmax(x, y); break;   // fmaxnm
                            case (0u << 6) | (1u << 5) | 24: v = std::isnan(x) ? y : std::isnan(y) ? x : std::fmin(x, y); break;   // fminnm
                            case (0u << 6) | (0u << 5) | 25: v = std::fma(x, y, acc); break;                                          // fmla
                            case (0u << 6) | (1u << 5) | 25: v = std::fma(-x, y, acc); break;                                         // fmls
                            case (0u << 6) | (0u << 5) | 26: v = x + y; break;
                            case (0u << 6) | (1u << 5) | 26: v = x - y; break;
                            case (0u << 6) | (0u << 5) | 27: v = x * y; break;                                                         // fmulx
                            case (1u << 6) | (0u << 5) | 27: v = x * y; break;
                            case (1u << 6) | (0u << 5) | 26: v = x + y; break;                                                         // faddp
                            case (1u << 6) | (0u << 5) | 31: v = x / y; break;
                            case (1u << 6) | (1u << 5) | 26: v = std::fabs(x - y); break;                                              // fabd
                            case (0u << 6) | (0u << 5) | 30: v = std::isnan(x) || std::isnan(y) ? std::numeric_limits<T>::quiet_NaN() : std::fmax(x, y); break;
                            case (0u << 6) | (1u << 5) | 30: v = std::isnan(x) || std::isnan(y) ? std::numeric_limits<T>::quiet_NaN() : std::fmin(x, y); break;
                            case (1u << 6) | (0u << 5) | 30: v = std::isnan(x) || std::isnan(y) ? std::numeric_limits<T>::quiet_NaN() : std::fmax(x, y); break;
                            case (1u << 6) | (1u << 5) | 30: v = std::isnan(x) || std::isnan(y) ? std::numeric_limits<T>::quiet_NaN() : std::fmin(x, y); break;
                            case (1u << 6) | (0u << 5) | 24: v = std::fmax(x, y); break;
                            case (1u << 6) | (1u << 5) | 24: v = std::fmin(x, y); break;
                            default: {
                                bool t = false;
                                switch (key) {
                                    case (0u << 6) | (0u << 5) | 28: t = x == y; break;
                                    case (1u << 6) | (0u << 5) | 28: t = x >= y; break;
                                    case (1u << 6) | (1u << 5) | 28: t = x > y; break;
                                    case (1u << 6) | (0u << 5) | 29: t = std::fabs(x) >= std::fabs(y); break;
                                    case (1u << 6) | (1u << 5) | 29: t = std::fabs(x) > std::fabs(y); break;
                                    default: return unsupported("floating-point operation");
                                }
                                put(out, sizeof(T), i, t ? mask_bits(8 * sizeof(T)) : 0);
                                continue;
                            }
                        }
                        r[i] = v;
                        put(out, sizeof(T), i, std::bit_cast<I>(v));
                    }
                    return std::optional<std::string>();
                };
                if (fp_size && !f.q()) return unsupported("reserved");
                const std::optional<std::string> e = fp_size ? run(0.0) : run(0.0f);
                if (e) return e;
                (void)felems;
                (void)fb;
                return finish(out);
            }
            // integer
            const unsigned bitsz = 8 * eb;
            const auto pair_op = [&](unsigned i, bool second) -> std::pair<u64, u64> {
                const unsigned k = 2 * i;
                const B16& src = k < elems ? n : m;
                (void)second;
                return {get(src, eb, k % elems), get(src, eb, k % elems + 1)};
            };
            for (unsigned i = 0; i < elems; ++i) {
                const u64 a = get(n, eb, i), b = get(m, eb, i), acc = get(d, eb, i);
                const i64 sa = sext(a, bitsz), sb = sext(b, bitsz);
                u64 r = 0;
                const unsigned key = (u << 5) | opcode;
                switch (key) {
                    case 16: r = a + b; break;                       // add
                    case 32 | 16: r = a - b; break;                  // sub
                    case 19: r = a * b; break;                       // mul
                    case 18: r = acc + a * b; break;                 // mla
                    case 32 | 18: r = acc - a * b; break;            // mls
                    case 32 | 17: r = a == b ? mask_bits(bitsz) : 0; break;   // cmeq
                    case 17: r = (a & b) ? mask_bits(bitsz) : 0; break;      // cmtst
                    case 6: r = sa > sb ? mask_bits(bitsz) : 0; break;
                    case 7: r = sa >= sb ? mask_bits(bitsz) : 0; break;
                    case 32 | 6: r = a > b ? mask_bits(bitsz) : 0; break;
                    case 32 | 7: r = a >= b ? mask_bits(bitsz) : 0; break;
                    case 12: r = static_cast<u64>(std::max(sa, sb)); break;
                    case 13: r = static_cast<u64>(std::min(sa, sb)); break;
                    case 32 | 12: r = std::max(a, b); break;
                    case 32 | 13: r = std::min(a, b); break;
                    case 14: r = static_cast<u64>(sa > sb ? sa - sb : sb - sa); break;
                    case 15: r = acc + static_cast<u64>(sa > sb ? sa - sb : sb - sa); break;
                    case 32 | 14: r = a > b ? a - b : b - a; break;
                    case 32 | 15: r = acc + (a > b ? a - b : b - a); break;
                    case 0: r = static_cast<u64>((sa + sb) >> 1); break;                          // shadd
                    case 32 | 0: r = (a + b) >> 1; break;                                          // uhadd (esize < 8)
                    case 2: r = static_cast<u64>((sa + sb + 1) >> 1); break;                       // srhadd
                    case 32 | 2: r = (a + b + 1) >> 1; break;                                      // urhadd
                    case 4: r = static_cast<u64>((sa - sb) >> 1); break;                           // shsub
                    case 32 | 4: r = (a - b) >> 1; break;                                          // uhsub
                    case 1: r = static_cast<u64>(clamp_signed(sa + sb, bitsz)); break;             // sqadd
                    case 5: r = static_cast<u64>(clamp_signed(sa - sb, bitsz)); break;             // sqsub
                    case 32 | 1: r = std::min<u64>(a + b, mask_bits(bitsz)); break;                // uqadd
                    case 32 | 5: r = a > b ? a - b : 0; break;                                     // uqsub
                    case 8:
                    case 32 | 8: {   // sshl / ushl: the shift is the low byte of b, signed
                        const i64 sh = static_cast<std::int8_t>(b & 0xFF);
                        if (sh >= 0) r = sh >= static_cast<i64>(bitsz) ? 0 : a << sh;
                        else if (opcode == 8 && !u) r = static_cast<u64>(sa >> std::min<i64>(-sh, bitsz - 1));
                        else r = -sh >= static_cast<i64>(bitsz) ? 0 : a >> -sh;
                        break;
                    }
                    case 22:
                    case 32 | 22: {   // sqdmulh / sqrdmulh
                        if (bitsz > 32) return unsupported("64-bit multiply");
                        const i64 prod = 2 * sa * sb + (u ? (i64{1} << (bitsz - 1)) : 0);
                        r = static_cast<u64>(clamp_signed(prod >> bitsz, bitsz));
                        break;
                    }
                    case 32 | 19: {   // pmul: carry-less
                        u64 acc2 = 0;
                        for (unsigned k = 0; k < 8; ++k)
                            if ((b >> k) & 1) acc2 ^= a << k;
                        r = acc2;
                        break;
                    }
                    case 23:   // addp
                    case 20: case 21: case 32 | 20: case 32 | 21: {
                        const auto [x, y] = pair_op(i, false);
                        const i64 sx = sext(x, bitsz), sy = sext(y, bitsz);
                        if (key == 23) r = x + y;
                        else if (key == 20) r = static_cast<u64>(std::max(sx, sy));
                        else if (key == 21) r = static_cast<u64>(std::min(sx, sy));
                        else if (key == (32 | 20)) r = std::max(x, y);
                        else r = std::min(x, y);
                        break;
                    }
                    default: return unsupported("integer operation");
                }
                put(out, eb, i, r & mask_bits(bitsz));
            }
            return finish(out);
        }
        // ---- three registers, different sizes ----
        case da::Op::SimdThreeDiff: {
            const unsigned opcode = f.bits(12, 4), u = f.u(), hi = f.q();
            if (size > 2) return unsupported("reserved");
            const unsigned wb = 2 * eb, nb = 8 * eb, wbits = 8 * wb, n_elems = 8 / eb;
            for (unsigned i = 0; i < n_elems; ++i) {
                const unsigned si = hi ? i + n_elems : i;
                const u64 a = get(n, eb, si), b = get(m, eb, si);
                const i64 sa = sext(a, nb), sb = sext(b, nb);
                const u64 wa = get(n, wb, i), wbv = get(m, wb, i);   // (wide operands: the whole register's elements)
                const u64 acc = get(d, wb, i);
                const i64 va = u ? static_cast<i64>(a) : sa, vb = u ? static_cast<i64>(b) : sb;
                u64 r;
                switch (opcode) {
                    case 0: r = static_cast<u64>(va + vb); break;                       // saddl / uaddl
                    case 1: r = (u ? wa + b : static_cast<u64>(sext(wa, wbits) + sb)); break;   // saddw / uaddw
                    case 2: r = static_cast<u64>(va - vb); break;
                    case 3: r = (u ? wa - b : static_cast<u64>(sext(wa, wbits) - sb)); break;
                    case 5: r = acc + static_cast<u64>(va > vb ? va - vb : vb - va); break;
                    case 7: r = static_cast<u64>(va > vb ? va - vb : vb - va); break;
                    case 8: r = acc + static_cast<u64>(va * vb); break;
                    case 10: r = acc - static_cast<u64>(va * vb); break;
                    case 12: r = static_cast<u64>(va * vb); break;
                    case 14:
                        if (u || size != 0) return unsupported("polynomial multiply");
                        r = 0;
                        for (unsigned k = 0; k < 8; ++k)
                            if ((b >> k) & 1) r ^= a << k;
                        break;
                    case 4: case 6: {   // addhn / subhn (narrowing): handled below
                        r = 0;
                        break;
                    }
                    default: return unsupported("long operation");
                }
                if (opcode == 4 || opcode == 6) {
                    const u64 x = get(n, wb, i), y = get(m, wb, i);
                    u64 s = opcode == 4 ? x + y : x - y;
                    if (u) s += u64{1} << (wbits - 1);
                    put(out, eb, hi ? i + n_elems : i, (s >> nb) & mask_bits(nb));
                    continue;
                }
                put(out, wb, i, r & mask_bits(wbits));
                (void)wbv;
            }
            if (opcode == 4 || opcode == 6) {
                if (!hi) {
                    R.v[rd] = from_b(out);
                    return std::nullopt;
                }
                B16 x = d;
                for (unsigned i = 0; i < 8; ++i) x[8 + i] = out[8 + i];
                R.v[rd] = from_b(x);
                return std::nullopt;
            }
            R.v[rd] = from_b(out);
            return std::nullopt;
        }
        // ---- two registers, miscellaneous ----
        case da::Op::SimdTwoMisc: {
            const unsigned opcode = f.bits(12, 5), u = f.u(), bitsz = 8 * eb;
            if (opcode >= 24 && opcode != 0x1F + 0 && opcode >= 12 && (opcode >= 24 || (opcode >= 12 && opcode <= 15))) {
                // floating point: opcode 12..15 (compares with zero, abs, neg) and 24..31
            }
            const bool fpop = opcode >= 24 || (opcode >= 12 && opcode <= 15);
            if (fpop) {
                const unsigned a = f.bits(23, 1);
                const auto run = [&](auto zero) -> std::optional<std::string> {
                    using T = decltype(zero);
                    using I = std::conditional_t<sizeof(T) == 4, std::uint32_t, u64>;
                    const unsigned count = f.data() / sizeof(T);
                    for (unsigned i = 0; i < count; ++i) {
                        const T x = std::bit_cast<T>(static_cast<I>(get(n, sizeof(T), i)));
                        T v = 0;
                        bool is_int = false;
                        u64 iv = 0;
                        const unsigned key = (u << 6) | (a << 5) | opcode;
                        const auto convert = [&](unsigned mode, bool sgn) {
                            is_int = true;
                            if (std::isnan(x)) {
                                iv = 0;
                                return;
                            }
                            const double r = mode == 0 ? std::nearbyint(static_cast<double>(x)) : mode == 1 ? std::ceil(static_cast<double>(x)) : mode == 2 ? std::floor(static_cast<double>(x))
                                           : mode == 3 ? std::trunc(static_cast<double>(x)) : std::round(static_cast<double>(x));
                            const unsigned bt = 8 * sizeof(T);
                            if (sgn) {
                                const double lim = std::ldexp(1.0, static_cast<int>(bt) - 1);
                                iv = r >= lim ? static_cast<u64>(smax_of(bt)) : r < -lim ? static_cast<u64>(smin_of(bt)) : static_cast<u64>(static_cast<i64>(r));
                            } else {
                                const double lim = std::ldexp(1.0, static_cast<int>(bt));
                                iv = r >= lim ? mask_bits(bt) : r <= 0 ? 0 : static_cast<u64>(r);
                            }
                        };
                        switch (key) {
                            case 24: v = std::nearbyint(x); break;                     // frintn
                            case 25: v = std::floor(x); break;                         // frintm
                            case (1u << 5) | 24: v = std::ceil(x); break;              // frintp
                            case (1u << 5) | 25: v = std::trunc(x); break;             // frintz
                            case (1u << 6) | 24: v = std::round(x); break;             // frinta
                            case (1u << 6) | 25: v = std::nearbyint(x); break;         // frintx
                            case (1u << 6) | (1u << 5) | 25: v = std::nearbyint(x); break;   // frinti
                            case 26: convert(0, true); break;
                            case 27: convert(2, true); break;
                            case 28: convert(4, true); break;
                            case (1u << 5) | 26: convert(1, true); break;
                            case (1u << 5) | 27: convert(3, true); break;
                            case (1u << 6) | 26: convert(0, false); break;
                            case (1u << 6) | 27: convert(2, false); break;
                            case (1u << 6) | 28: convert(4, false); break;
                            case (1u << 6) | (1u << 5) | 26: convert(1, false); break;
                            case (1u << 6) | (1u << 5) | 27: convert(3, false); break;
                            case 29: v = static_cast<T>(sext(get(n, sizeof(T), i), 8 * sizeof(T))); break;   // scvtf
                            case (1u << 6) | 29: v = static_cast<T>(get(n, sizeof(T), i)); break;            // ucvtf
                            case (1u << 5) | 15: v = std::fabs(x); break;
                            case (1u << 6) | (1u << 5) | 15: v = -x; break;
                            case (1u << 6) | (1u << 5) | 31: v = std::sqrt(x); break;
                            default: {
                                bool t = false;
                                switch (key) {
                                    case (1u << 5) | 12: t = x > 0; break;
                                    case (1u << 5) | 13: t = x == 0; break;
                                    case (1u << 5) | 14: t = x < 0; break;
                                    case (1u << 6) | (1u << 5) | 12: t = x >= 0; break;
                                    case (1u << 6) | (1u << 5) | 13: t = x <= 0; break;
                                    default: return unsupported("floating-point operation");
                                }
                                put(out, sizeof(T), i, t ? mask_bits(8 * sizeof(T)) : 0);
                                continue;
                            }
                        }
                        if (is_int) put(out, sizeof(T), i, iv);
                        else put(out, sizeof(T), i, std::bit_cast<I>(v));
                    }
                    return std::optional<std::string>();
                };
                if (fp_size && !f.q()) return unsupported("reserved");
                const std::optional<std::string> e = fp_size ? run(0.0) : run(0.0f);
                if (e) return e;
                return finish(out);
            }
            switch (opcode) {
                case 0:   // rev64 / rev32
                case 1: {   // rev16
                    const unsigned container = opcode == 1 ? 2 : (u ? 4 : 8);
                    if (container <= eb) return unsupported("reserved");
                    for (unsigned base = 0; base < f.data(); base += container)
                        for (unsigned i = 0; i < container / eb; ++i) put(out, eb, (base / eb) + (container / eb - 1 - i), get(n, eb, base / eb + i));
                    return finish(out);
                }
                case 2: case 6: {   // saddlp / uaddlp / sadalp / uadalp
                    if (size > 2) return unsupported("reserved");
                    const unsigned wb = 2 * eb;
                    for (unsigned i = 0; i < f.data() / wb; ++i) {
                        const u64 a = get(n, eb, 2 * i), b = get(n, eb, 2 * i + 1);
                        u64 r = u ? a + b : static_cast<u64>(sext(a, 8 * eb) + sext(b, 8 * eb));
                        if (opcode == 6) r += get(d, wb, i);
                        put(out, wb, i, r & mask_bits(8 * wb));
                    }
                    return finish(out);
                }
                case 4: {   // cls / clz
                    for (unsigned i = 0; i < elems; ++i) {
                        const u64 a = get(n, eb, i);
                        unsigned c = 0;
                        if (u) {
                            c = bitsz - static_cast<unsigned>(std::bit_width(a & mask_bits(bitsz)));
                        } else {
                            const u64 sign = (a >> (bitsz - 1)) & 1;
                            for (int k = static_cast<int>(bitsz) - 2; k >= 0 && ((a >> k) & 1) == sign; --k) ++c;
                        }
                        put(out, eb, i, c);
                    }
                    return finish(out);
                }
                case 5: {   // cnt / not / rbit
                    for (unsigned i = 0; i < 16; ++i) {
                        const std::uint8_t a = n[i];
                        if (!u) {
                            out[i] = static_cast<std::uint8_t>(std::popcount(static_cast<unsigned>(a)));
                        } else if (size == 0) {
                            out[i] = static_cast<std::uint8_t>(~a);
                        } else {
                            std::uint8_t r = 0;
                            for (unsigned k = 0; k < 8; ++k) r |= ((a >> k) & 1) << (7 - k);
                            out[i] = r;
                        }
                    }
                    return finish(out);
                }
                case 8: case 9: case 10: case 11: {   // compares with zero, abs, neg
                    for (unsigned i = 0; i < elems; ++i) {
                        const u64 a = get(n, eb, i);
                        const i64 sa = sext(a, bitsz);
                        u64 r;
                        const unsigned key = (u << 4) | opcode;
                        switch (key) {
                            case 8: r = sa > 0; break;
                            case 9: r = sa == 0; break;
                            case 10: r = sa < 0; break;
                            case 16 | 8: r = sa >= 0; break;
                            case 16 | 9: r = sa <= 0; break;
                            case 11: r = static_cast<u64>(sa < 0 ? -sa : sa); put(out, eb, i, r & mask_bits(bitsz)); continue;
                            default: r = static_cast<u64>(-sa); put(out, eb, i, r & mask_bits(bitsz)); continue;
                        }
                        put(out, eb, i, r ? mask_bits(bitsz) : 0);
                    }
                    return finish(out);
                }
                case 18: case 20: {   // xtn / sqxtun / sqxtn / uqxtn (narrow: the high half for the "2" forms)
                    if (size > 2) return unsupported("reserved");
                    const unsigned wb = 2 * eb, count = 8 / eb, wbits = 8 * wb;
                    for (unsigned i = 0; i < count; ++i) {
                        const u64 a = get(n, wb, i);
                        u64 r;
                        if (opcode == 18 && !u) r = a;
                        else if (opcode == 18) r = clamp_unsigned(sext(a, wbits), bitsz);                 // sqxtun
                        else if (!u) r = static_cast<u64>(clamp_signed(sext(a, wbits), bitsz));          // sqxtn
                        else r = std::min<u64>(a, mask_bits(bitsz));                                      // uqxtn
                        put(out, eb, f.q() ? i + count : i, r & mask_bits(bitsz));
                    }
                    if (f.q()) {
                        B16 x = d;
                        for (unsigned i = 0; i < 8; ++i) x[8 + i] = out[8 + i];
                        R.v[rd] = from_b(x);
                    } else {
                        R.v[rd] = from_b(out);
                    }
                    return std::nullopt;
                }
                case 22: case 23: {   // fcvtn / fcvtl between single and double (half precision is not run)
                    if (u || !fp_size) return unsupported("half-precision conversion");
                    const unsigned count = 2;
                    for (unsigned i = 0; i < count; ++i) {
                        if (opcode == 22) {
                            const float v = static_cast<float>(std::bit_cast<double>(get(n, 8, i)));
                            put(out, 4, f.q() ? i + 2 : i, std::bit_cast<u32>(v));
                        } else {
                            const double v = std::bit_cast<float>(static_cast<u32>(get(n, 4, f.q() ? i + 2 : i)));
                            put(out, 8, i, std::bit_cast<u64>(v));
                        }
                    }
                    if (opcode == 22 && f.q()) {
                        B16 x = d;
                        for (unsigned i = 0; i < 8; ++i) x[8 + i] = out[8 + i];
                        R.v[rd] = from_b(x);
                    } else {
                        R.v[rd] = from_b(out);
                    }
                    return std::nullopt;
                }
                case 19: {   // shll
                    if (size > 2) return unsupported("reserved");
                    const unsigned wb = 2 * eb, count = 8 / eb;
                    for (unsigned i = 0; i < count; ++i) put(out, wb, i, (get(n, eb, f.q() ? i + count : i) << (8 * eb)) & mask_bits(8 * wb));
                    R.v[rd] = from_b(out);
                    return std::nullopt;
                }
                default: return unsupported("integer operation");
            }
        }
        // ---- across lanes ----
        case da::Op::SimdAcross: {
            const unsigned opcode = f.bits(12, 5), u = f.u(), bitsz = 8 * eb;
            if (opcode == 12 || opcode == 15) {   // fmaxnmv / fmaxv / fminnmv / fminv
                const bool minimum = f.bits(23, 1);
                float r = std::bit_cast<float>(static_cast<u32>(get(n, 4, 0)));
                for (unsigned i = 1; i < 4; ++i) {
                    const float x = std::bit_cast<float>(static_cast<u32>(get(n, 4, i)));
                    r = minimum ? std::fmin(r, x) : std::fmax(r, x);
                }
                R.v[rd] = {std::bit_cast<u32>(r), 0};
                return std::nullopt;
            }
            i64 sacc = 0;
            u64 uacc = 0;
            bool first = true;
            for (unsigned i = 0; i < elems; ++i) {
                const u64 a = get(n, eb, i);
                const i64 sa = sext(a, bitsz);
                switch (opcode) {
                    case 3: sacc += u ? static_cast<i64>(a) : sa; break;       // saddlv / uaddlv
                    case 27: uacc += a; break;                                  // addv
                    case 10: sacc = first ? (u ? static_cast<i64>(a) : sa) : u ? std::max(sacc, static_cast<i64>(a)) : std::max(sacc, sa); break;
                    case 26: sacc = first ? (u ? static_cast<i64>(a) : sa) : u ? std::min(sacc, static_cast<i64>(a)) : std::min(sacc, sa); break;
                    default: return unsupported("reduction");
                }
                first = false;
            }
            const u64 r = opcode == 27 ? uacc & mask_bits(bitsz) : static_cast<u64>(sacc) & mask_bits(opcode == 3 ? 16 * eb : bitsz);
            R.v[rd] = {r, 0};
            return std::nullopt;
        }
        // ---- permute, extract, table lookup ----
        case da::Op::SimdPermute: {
            const unsigned opcode = f.bits(12, 3);
            for (unsigned i = 0; i < elems; ++i) {
                u64 r;
                const unsigned half = elems / 2;
                switch (opcode) {
                    case 1: r = get(i < half ? n : m, eb, 2 * (i % half)); break;                   // uzp1
                    case 5: r = get(i < half ? n : m, eb, 2 * (i % half) + 1); break;               // uzp2
                    case 2: r = get(i % 2 ? m : n, eb, (i / 2) * 2); break;                         // trn1
                    case 6: r = get(i % 2 ? m : n, eb, (i / 2) * 2 + 1); break;                     // trn2
                    case 3: r = get(i % 2 ? m : n, eb, i / 2); break;                               // zip1
                    case 7: r = get(i % 2 ? m : n, eb, half + i / 2); break;                        // zip2
                    default: return unsupported("reserved");
                }
                put(out, eb, i, r);
            }
            return finish(out);
        }
        case da::Op::SimdExt: {
            const unsigned pos = f.bits(11, 4);
            B16 both{};
            for (unsigned i = 0; i < f.data(); ++i) both[i] = n[i];
            std::array<std::uint8_t, 32> cat{};
            for (unsigned i = 0; i < f.data(); ++i) {
                cat[i] = n[i];
                cat[f.data() + i] = m[i];
            }
            for (unsigned i = 0; i < f.data(); ++i) out[i] = cat[pos + i];
            (void)both;
            return finish(out);
        }
        case da::Op::SimdTable: {
            const unsigned len = f.bits(13, 2) + 1, ext = f.bits(12, 1), t = rn;
            for (unsigned i = 0; i < f.data(); ++i) {
                const unsigned idx = m[i];
                if (idx < 16 * len) out[i] = to_b(R.v[(t + idx / 16) % 32])[idx % 16];
                else out[i] = ext ? d[i] : 0;
            }
            return finish(out);
        }
        // ---- shifts by immediate ----
        case da::Op::SimdShift: {
            const unsigned immh = f.bits(19, 4), immhb = f.bits(16, 7), opcode = f.bits(11, 5), u = f.u();
            if (immh == 0) return unsupported("reserved");
            const unsigned sz = static_cast<unsigned>(std::bit_width(immh)) - 1, e = 1u << sz, ebits = 8 * e, cnt = f.data() / e;
            const unsigned left = immhb - ebits, right = 2 * ebits - immhb;
            switch (opcode) {
                case 0: case 2: case 4: case 6: case 8: case 10: {   // sshr ssra srshr srsra / sri shl sli
                    for (unsigned i = 0; i < cnt; ++i) {
                        const u64 a = get(n, e, i), acc = get(d, e, i);
                        u64 r;
                        const bool rounding = opcode == 4 || opcode == 6;
                        const u64 round = rounding && right > 0 ? (u64{1} << (right - 1)) : 0;
                        if (opcode == 10 && !u) r = a << left;                                           // shl
                        else if (opcode == 10) r = ((a << left) & mask_bits(ebits)) | (acc & mask_bits(left));    // sli
                        else if (opcode == 8) r = right >= ebits ? acc : ((a >> right) | (acc & ~(mask_bits(ebits) >> right))) & mask_bits(ebits);   // sri
                        else if (u) r = right >= 64 ? 0 : (a + round) >> right;                          // ushr usra urshr ursra
                        else r = static_cast<u64>((sext(a, ebits) + static_cast<i64>(round)) >> std::min<unsigned>(right, 63));
                        if (opcode == 2 || opcode == 6) r += acc;
                        put(out, e, i, r & mask_bits(ebits));
                    }
                    return finish(out);
                }
                case 14: case 12: {   // sqshl / uqshl / sqshlu by immediate
                    for (unsigned i = 0; i < cnt; ++i) {
                        const u64 a = get(n, e, i);
                        if (ebits >= 64) return unsupported("64-bit saturating shift");
                        const i64 wide = (u && opcode == 14) ? static_cast<i64>(a << left) : sext(a, ebits) * (i64{1} << left);
                        u64 r;
                        if (opcode == 12) r = clamp_unsigned(wide, ebits);
                        else if (u) r = std::min<u64>(a << left, mask_bits(ebits));
                        else r = static_cast<u64>(clamp_signed(wide, ebits));
                        put(out, e, i, r & mask_bits(ebits));
                    }
                    return finish(out);
                }
                case 16: case 17: case 18: case 19: {   // shrn rshrn sqshrn sqrshrn (and the unsigned / sqshrun forms)
                    if (sz > 2) return unsupported("reserved");
                    const unsigned wb = 2 * e, wbits = 8 * wb, count = 8 / e;
                    const bool rounding = opcode == 17 || opcode == 19;
                    for (unsigned i = 0; i < count; ++i) {
                        const u64 a = get(n, wb, i);
                        const u64 round = rounding ? (u64{1} << (right - 1)) : 0;
                        u64 r;
                        if (opcode < 18 && !u) {
                            r = ((a + round) >> right);
                        } else if (u && opcode < 18) {   // sqshrun / sqrshrun
                            r = clamp_unsigned((sext(a, wbits) + static_cast<i64>(round)) >> right, ebits);
                        } else if (!u) {
                            r = static_cast<u64>(clamp_signed((sext(a, wbits) + static_cast<i64>(round)) >> right, ebits));
                        } else {
                            r = std::min<u64>((a + round) >> right, mask_bits(ebits));
                        }
                        put(out, e, f.q() ? i + count : i, r & mask_bits(ebits));
                    }
                    if (f.q()) {
                        B16 x = d;
                        for (unsigned i = 0; i < 8; ++i) x[8 + i] = out[8 + i];
                        R.v[rd] = from_b(x);
                    } else {
                        R.v[rd] = from_b(out);
                    }
                    return std::nullopt;
                }
                case 20: {   // sshll / ushll
                    if (sz > 2) return unsupported("reserved");
                    const unsigned wb = 2 * e, count = 8 / e;
                    for (unsigned i = 0; i < count; ++i) {
                        const u64 a = get(n, e, f.q() ? i + count : i);
                        const u64 v = u ? a : static_cast<u64>(sext(a, ebits));
                        put(out, wb, i, (v << left) & mask_bits(8 * wb));
                    }
                    R.v[rd] = from_b(out);
                    return std::nullopt;
                }
                case 28: case 31: {   // fixed-point conversions
                    if (sz < 2) return unsupported("reserved");
                    for (unsigned i = 0; i < cnt; ++i) {
                        const u64 a = get(n, e, i);
                        if (opcode == 28) {
                            const double v = std::ldexp(u ? static_cast<double>(a) : static_cast<double>(sext(a, ebits)), -static_cast<int>(right));
                            if (sz == 2) put(out, 4, i, std::bit_cast<u32>(static_cast<float>(v)));
                            else put(out, 8, i, std::bit_cast<u64>(v));
                        } else {
                            const double x = sz == 2 ? static_cast<double>(std::bit_cast<float>(static_cast<u32>(a))) : std::bit_cast<double>(a);
                            const double r = std::trunc(std::ldexp(x, static_cast<int>(right)));
                            u64 iv;
                            if (std::isnan(r)) iv = 0;
                            else if (u) iv = r <= 0 ? 0 : r >= std::ldexp(1.0, ebits) ? mask_bits(ebits) : static_cast<u64>(r);
                            else iv = r >= std::ldexp(1.0, ebits - 1) ? static_cast<u64>(smax_of(ebits)) : r < -std::ldexp(1.0, ebits - 1) ? static_cast<u64>(smin_of(ebits)) : static_cast<u64>(static_cast<i64>(r));
                            put(out, e, i, iv & mask_bits(ebits));
                        }
                    }
                    return finish(out);
                }
                default: return unsupported("shift");
            }
        }
        // ---- by element ----
        case da::Op::SimdByElem: {
            const unsigned opcode = f.bits(12, 4), u = f.u(), hbit = f.bits(11, 1), lbit = f.bits(21, 1), mbit = f.bits(20, 1);
            const bool is_fp = opcode == 1 || opcode == 5 || opcode == 9;
            if (is_fp) {
                const bool dbl = f.bits(22, 1);
                const unsigned idx = dbl ? hbit : (hbit << 1 | lbit), reg = mbit << 4 | f.bits(16, 4);
                const B16 e = to_b(R.v[reg]);
                const unsigned count = f.data() / (dbl ? 8 : 4);
                for (unsigned i = 0; i < count; ++i) {
                    if (dbl) {
                        const double a = std::bit_cast<double>(get(n, 8, i)), b = std::bit_cast<double>(get(e, 8, idx)), acc = std::bit_cast<double>(get(d, 8, i));
                        const double r = opcode == 1 ? std::fma(a, b, acc) : opcode == 5 ? std::fma(-a, b, acc) : a * b;
                        put(out, 8, i, std::bit_cast<u64>(r));
                    } else {
                        const float a = std::bit_cast<float>(static_cast<u32>(get(n, 4, i))), b = std::bit_cast<float>(static_cast<u32>(get(e, 4, idx))),
                                    acc = std::bit_cast<float>(static_cast<u32>(get(d, 4, i)));
                        const float r = opcode == 1 ? std::fma(a, b, acc) : opcode == 5 ? std::fma(-a, b, acc) : a * b;
                        put(out, 4, i, std::bit_cast<u32>(r));
                    }
                }
                return finish(out);
            }
            unsigned idx, reg;
            if (size == 1) {
                idx = hbit << 2 | lbit << 1 | mbit;
                reg = f.bits(16, 4);
            } else {
                idx = hbit << 1 | lbit;
                reg = mbit << 4 | f.bits(16, 4);
            }
            const B16 e = to_b(R.v[reg]);
            const u64 b = get(e, eb, idx);
            const bool longform = opcode == 2 || opcode == 3 || opcode == 6 || opcode == 7 || opcode == 10 || opcode == 11;
            if (longform) {
                const unsigned wb = 2 * eb, count = 8 / eb, hi = f.q();
                for (unsigned i = 0; i < count; ++i) {
                    const u64 a = get(n, eb, hi ? i + count : i);
                    const i64 va = u ? static_cast<i64>(a) : sext(a, 8 * eb), vb = u ? static_cast<i64>(b) : sext(b, 8 * eb);
                    const u64 acc = get(d, wb, i);
                    u64 r;
                    switch (opcode) {
                        case 2: r = acc + static_cast<u64>(va * vb); break;
                        case 6: r = acc - static_cast<u64>(va * vb); break;
                        case 10: r = static_cast<u64>(va * vb); break;
                        default: return unsupported("saturating long multiply");
                    }
                    put(out, wb, i, r & mask_bits(8 * wb));
                }
                R.v[rd] = from_b(out);
                return std::nullopt;
            }
            for (unsigned i = 0; i < elems; ++i) {
                const u64 a = get(n, eb, i), acc = get(d, eb, i);
                const i64 sa = sext(a, 8 * eb), sb = sext(b, 8 * eb);
                u64 r;
                switch (opcode) {
                    case 8: r = a * b; break;
                    case 0: r = acc + a * b; break;
                    case 4: r = acc - a * b; break;
                    case 12: r = static_cast<u64>(clamp_signed((2 * sa * sb) >> (8 * eb), 8 * eb)); break;
                    case 13: r = static_cast<u64>(clamp_signed((2 * sa * sb + (i64{1} << (8 * eb - 1))) >> (8 * eb), 8 * eb)); break;
                    default: return unsupported("by-element operation");
                }
                put(out, eb, i, r & mask_bits(8 * eb));
            }
            return finish(out);
        }
        // ---- scalar forms ----
        case da::Op::SimdScalarThree: {
            const unsigned opcode = f.bits(11, 5), u = f.u();
            if (opcode >= 24) {
                const bool dbl = f.bits(22, 1), a = f.bits(23, 1);
                const double x = dbl ? std::bit_cast<double>(get(n, 8, 0)) : std::bit_cast<float>(static_cast<u32>(get(n, 4, 0))),
                             y = dbl ? std::bit_cast<double>(get(m, 8, 0)) : std::bit_cast<float>(static_cast<u32>(get(m, 4, 0)));
                bool t = false;
                const unsigned key = (u << 6) | (a << 5) | opcode;
                double v = 0;
                bool value = false;
                switch (key) {
                    case 27: case (1u << 6) | 27: v = x * y; value = true; break;
                    case (1u << 6) | (1u << 5) | 26: v = std::fabs(x - y); value = true; break;
                    case 28: t = x == y; break;
                    case (1u << 6) | 28: t = x >= y; break;
                    case (1u << 6) | (1u << 5) | 28: t = x > y; break;
                    case (1u << 6) | 29: t = std::fabs(x) >= std::fabs(y); break;
                    case (1u << 6) | (1u << 5) | 29: t = std::fabs(x) > std::fabs(y); break;
                    default: return unsupported("floating-point operation");
                }
                if (value) R.v[rd] = {dbl ? std::bit_cast<u64>(v) : std::bit_cast<u32>(static_cast<float>(v)), 0};
                else R.v[rd] = {t ? (dbl ? ~0ull : 0xFFFFFFFFull) : 0, 0};
                return std::nullopt;
            }
            const u64 a = get(n, 8, 0), b = get(m, 8, 0);
            u64 r;
            switch ((u << 5) | opcode) {
                case 16: r = a + b; break;
                case 32 | 16: r = a - b; break;
                case 32 | 17: r = a == b ? ~0ull : 0; break;
                case 17: r = (a & b) ? ~0ull : 0; break;
                case 6: r = static_cast<i64>(a) > static_cast<i64>(b) ? ~0ull : 0; break;
                case 7: r = static_cast<i64>(a) >= static_cast<i64>(b) ? ~0ull : 0; break;
                case 32 | 6: r = a > b ? ~0ull : 0; break;
                case 32 | 7: r = a >= b ? ~0ull : 0; break;
                case 8: case 32 | 8: {
                    const i64 sh = static_cast<std::int8_t>(b & 0xFF);
                    if (sh >= 0) r = sh >= 64 ? 0 : a << sh;
                    else if (!u) r = static_cast<u64>(static_cast<i64>(a) >> std::min<i64>(-sh, 63));
                    else r = -sh >= 64 ? 0 : a >> -sh;
                    break;
                }
                default: return unsupported("scalar operation");
            }
            R.v[rd] = {r, 0};
            return std::nullopt;
        }
        case da::Op::SimdScalarTwoMisc: {
            const unsigned opcode = f.bits(12, 5), u = f.u(), a = f.bits(23, 1);
            if (opcode >= 26 && opcode <= 29 && !(u == 0 && a == 0 && opcode == 28 && false)) {
                const bool dbl = f.bits(22, 1);
                const double x = dbl ? std::bit_cast<double>(get(n, 8, 0)) : std::bit_cast<float>(static_cast<u32>(get(n, 4, 0)));
                const unsigned key = (u << 6) | (a << 5) | opcode;
                const unsigned bt = dbl ? 64 : 32;
                const auto to_int = [&](double r, bool sgn) -> u64 {
                    if (std::isnan(r)) return 0;
                    if (sgn) {
                        const double lim = std::ldexp(1.0, static_cast<int>(bt) - 1);
                        return r >= lim ? static_cast<u64>(smax_of(bt)) : r < -lim ? static_cast<u64>(smin_of(bt)) : static_cast<u64>(static_cast<i64>(r));
                    }
                    const double lim = std::ldexp(1.0, static_cast<int>(bt));
                    return r >= lim ? mask_bits(bt) : r <= 0 ? 0 : static_cast<u64>(r);
                };
                u64 iv;
                switch (key) {
                    case 29: {   // scvtf
                        const double v = dbl ? static_cast<double>(static_cast<i64>(get(n, 8, 0))) : static_cast<double>(sext(get(n, 4, 0), 32));
                        R.v[rd] = {dbl ? std::bit_cast<u64>(v) : std::bit_cast<u32>(static_cast<float>(sext(get(n, 4, 0), 32))), 0};
                        return std::nullopt;
                    }
                    case (1u << 6) | 29: {
                        const double v = dbl ? static_cast<double>(get(n, 8, 0)) : static_cast<double>(get(n, 4, 0));
                        R.v[rd] = {dbl ? std::bit_cast<u64>(v) : std::bit_cast<u32>(static_cast<float>(get(n, 4, 0))), 0};
                        return std::nullopt;
                    }
                    case (1u << 5) | 27: iv = to_int(std::trunc(x), true); break;
                    case (1u << 6) | (1u << 5) | 27: iv = to_int(std::trunc(x), false); break;
                    case 26: iv = to_int(std::nearbyint(x), true); break;
                    case (1u << 6) | 26: iv = to_int(std::nearbyint(x), false); break;
                    case 27: iv = to_int(std::floor(x), true); break;
                    case (1u << 6) | 27: iv = to_int(std::floor(x), false); break;
                    case (1u << 5) | 26: iv = to_int(std::ceil(x), true); break;
                    case (1u << 6) | (1u << 5) | 26: iv = to_int(std::ceil(x), false); break;
                    case 28: iv = to_int(std::round(x), true); break;
                    case (1u << 6) | 28: iv = to_int(std::round(x), false); break;
                    default: return unsupported("scalar conversion");
                }
                R.v[rd] = {iv, 0};
                return std::nullopt;
            }
            const i64 sa = static_cast<i64>(get(n, 8, 0));
            u64 r;
            switch ((u << 5) | opcode) {
                case 8: r = sa > 0 ? ~0ull : 0; break;
                case 9: r = sa == 0 ? ~0ull : 0; break;
                case 10: r = sa < 0 ? ~0ull : 0; break;
                case 32 | 8: r = sa >= 0 ? ~0ull : 0; break;
                case 32 | 9: r = sa <= 0 ? ~0ull : 0; break;
                case 11: r = static_cast<u64>(sa < 0 ? -sa : sa); break;
                case 32 | 11: r = static_cast<u64>(-sa); break;
                default: return unsupported("scalar operation");
            }
            R.v[rd] = {r, 0};
            return std::nullopt;
        }
        case da::Op::SimdScalarPair: {
            if (f.bits(23, 1) == 0 && f.bits(12, 5) == 13) {   // faddp
                if (f.bits(22, 1)) {
                    R.v[rd] = {std::bit_cast<u64>(std::bit_cast<double>(get(n, 8, 0)) + std::bit_cast<double>(get(n, 8, 1))), 0};
                } else {
                    R.v[rd] = {std::bit_cast<u32>(std::bit_cast<float>(static_cast<u32>(get(n, 4, 0))) + std::bit_cast<float>(static_cast<u32>(get(n, 4, 1)))), 0};
                }
                return std::nullopt;
            }
            if (f.bits(12, 5) == 27) {   // addp
                R.v[rd] = {get(n, 8, 0) + get(n, 8, 1), 0};
                return std::nullopt;
            }
            return unsupported("scalar pairwise operation");
        }
        case da::Op::SimdScalarShift: {
            const unsigned immh = f.bits(19, 4), immhb = f.bits(16, 7), opcode = f.bits(11, 5), u = f.u();
            if (immh == 0) return unsupported("reserved");
            const unsigned sz = static_cast<unsigned>(std::bit_width(immh)) - 1, ebits = 8u << sz, left = immhb - ebits, right = 2 * ebits - immhb;
            const u64 a = get(n, 1u << sz, 0), acc = get(d, 1u << sz, 0);
            u64 r;
            switch (opcode) {
                case 0: r = u ? (right >= 64 ? 0 : a >> right) : static_cast<u64>(static_cast<i64>(a) >> std::min(right, 63u)); break;
                case 2: r = acc + (u ? a >> right : static_cast<u64>(static_cast<i64>(a) >> std::min(right, 63u))); break;
                case 10: r = u ? ((a << left) | (acc & mask_bits(left))) : a << left; break;
                case 8: r = (a >> right) | (acc & ~(~0ull >> right)); break;
                case 28:
                case 31: {
                    if (sz < 2) return unsupported("reserved");
                    if (opcode == 28) {
                        const double v = std::ldexp(u ? static_cast<double>(a) : static_cast<double>(sext(a, ebits)), -static_cast<int>(right));
                        r = sz == 2 ? std::bit_cast<u32>(static_cast<float>(v)) : std::bit_cast<u64>(v);
                    } else {
                        const double x = sz == 2 ? static_cast<double>(std::bit_cast<float>(static_cast<u32>(a))) : std::bit_cast<double>(a);
                        const double t = std::trunc(std::ldexp(x, static_cast<int>(right)));
                        if (std::isnan(t)) r = 0;
                        else if (u) r = t <= 0 ? 0 : t >= std::ldexp(1.0, ebits) ? mask_bits(ebits) : static_cast<u64>(t);
                        else r = t >= std::ldexp(1.0, ebits - 1) ? static_cast<u64>(smax_of(ebits)) : t < -std::ldexp(1.0, ebits - 1) ? static_cast<u64>(smin_of(ebits)) : static_cast<u64>(static_cast<i64>(t));
                    }
                    break;
                }
                default: return unsupported("scalar shift");
            }
            R.v[rd] = {r & mask_bits(ebits), 0};
            return std::nullopt;
        }
        default: break;
    }
    return unsupported("instruction group");
}

}  // namespace cb::emulate::a64::detail
