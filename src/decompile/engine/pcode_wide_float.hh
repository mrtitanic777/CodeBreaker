// decompile/engine/pcode_wide_float.hh -- the x87's 10-byte extended float for the emulator's wide path (and its conversions to and from the 4- and 8-byte IEEE ones), by
// exact integer arithmetic on the significands and one rounding to nearest even (pcode_wide.hh's byte arithmetic does the integer work). The host's long double is a plain
// double on MSVC, so it cannot stand in. Where the description's p-code model and the processor differ, the processor's way is taken: an invalid operation (inf - inf,
// 0 * inf, 0 / 0, inf / inf, the root of a negative) gives the "real indefinite" NaN (negative, quiet, no payload); a NaN operand gives that NaN, quieted (the larger
// significand when both are NaNs, the first on a tie); a truncation out of range gives the integer indefinite (the most negative value). Rounding is to nearest even only (the
// p-code model has no other), and no exception flags are kept.
#pragma once

#include <utility>

#include "decompile/engine/pcode_wide.hh"

namespace cb::decompile::sleigh::wide {
namespace soft {

struct Fmt {
    unsigned size;
    int frac;        // fraction bits (IEEE); 63 for the extended format, whose integer bit is explicit
    int bias;
    bool explicitJ;
    unsigned maxExp;
};
constexpr Fmt kF16{2, 10, 15, false, 31};
constexpr Fmt kF32{4, 23, 127, false, 255};
constexpr Fmt kF64{8, 52, 1023, false, 2047};
constexpr Fmt kF80{10, 63, 16383, true, 32767};
inline const Fmt* formatOf(std::size_t size) { return size == 2 ? &kF16 : size == 4 ? &kF32 : size == 8 ? &kF64 : size == 10 ? &kF80 : nullptr; }

enum Kind { Zero, Finite, Inf, NaN };
// A float taken apart: finite value = (-1)^sign * mant * 2^(exp - 63) with mant's top bit set (a denormal is normalized, so exp can go below the format's minimum); a NaN's
// mant is its fraction aligned under the top bit with the top bit set.
struct Value {
    bool sign = false;
    Kind kind = Zero;
    int exp = 0;
    std::uint64_t mant = 0;
};

inline std::uint64_t load64(const Byte* a, std::size_t n) {
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < n && i < 8; ++i) v |= std::uint64_t{a[i]} << (8 * i);
    return v;
}
inline unsigned clz64(std::uint64_t v) {
    unsigned c = 0;
    for (std::uint64_t bit = std::uint64_t{1} << 63; c < 64 && (v & bit) == 0; bit >>= 1) ++c;
    return c;
}
inline Value unpack(const Fmt& f, const Byte* a) {
    Value v;
    if (f.explicitJ) {
        const std::uint64_t frac = load64(a, 8);
        const unsigned se = a[8] | (unsigned{a[9]} << 8);
        v.sign = (se & 0x8000) != 0;
        const unsigned e = se & 0x7fff;
        if (e == f.maxExp) {
            v.kind = (frac << 1) == 0 ? Inf : NaN;
            v.mant = frac | (std::uint64_t{1} << 63);
            return v;
        }
        if (frac == 0) return v;   // (a zero, or a pseudo-zero)
        const unsigned s = clz64(frac);
        v.kind = Finite;
        v.mant = frac << s;
        v.exp = (e == 0 ? 1 : static_cast<int>(e)) - f.bias - static_cast<int>(s);
        return v;
    }
    const std::uint64_t bits = load64(a, f.size);
    v.sign = ((bits >> (8 * f.size - 1)) & 1) != 0;
    const unsigned e = static_cast<unsigned>((bits >> f.frac) & f.maxExp);
    const std::uint64_t frac = bits & ((std::uint64_t{1} << f.frac) - 1);
    if (e == f.maxExp) {
        v.kind = frac == 0 ? Inf : NaN;
        v.mant = (std::uint64_t{1} << 63) | (frac << (63 - f.frac));
        return v;
    }
    if (e == 0) {
        if (frac == 0) return v;
        const unsigned s = clz64(frac);
        v.kind = Finite;
        v.mant = frac << s;
        v.exp = 64 - f.bias - f.frac - static_cast<int>(s);
        return v;
    }
    v.kind = Finite;
    v.mant = (std::uint64_t{1} << 63) | (frac << (63 - f.frac));
    v.exp = static_cast<int>(e) - f.bias;
    return v;
}

inline void putBits(const Fmt& f, Byte* r, bool sign, std::uint64_t mantOrFraction, unsigned expField) {
    if (f.explicitJ) {
        for (unsigned i = 0; i < 8; ++i) r[i] = static_cast<Byte>(mantOrFraction >> (8 * i));
        const unsigned se = (sign ? 0x8000u : 0u) | expField;
        r[8] = static_cast<Byte>(se);
        r[9] = static_cast<Byte>(se >> 8);
        return;
    }
    const std::uint64_t enc = (sign ? std::uint64_t{1} << (8 * f.size - 1) : 0) | (std::uint64_t{expField} << f.frac) | mantOrFraction;
    fromU64(r, f.size, enc);
}
inline void putZero(const Fmt& f, Byte* r, bool sign) { putBits(f, r, sign, 0, 0); }
inline void putInf(const Fmt& f, Byte* r, bool sign) { putBits(f, r, sign, f.explicitJ ? std::uint64_t{1} << 63 : 0, f.maxExp); }
// A NaN with the given payload (aligned under the top bit), made quiet.
inline void putNaN(const Fmt& f, Byte* r, bool sign, std::uint64_t mant63) {
    mant63 |= (std::uint64_t{1} << 63) | (std::uint64_t{1} << 62);
    putBits(f, r, sign, f.explicitJ ? mant63 : ((mant63 >> (63 - f.frac)) & ((std::uint64_t{1} << f.frac) - 1)), f.maxExp);
}
inline void putIndefinite(const Fmt& f, Byte* r) { putNaN(f, r, true, 0); }

inline bool bitAt(const Byte* m, std::size_t bytes, long i) { return i >= 0 && static_cast<std::size_t>(i) < 8 * bytes && ((m[i / 8] >> (i % 8)) & 1) != 0; }
inline bool anyBitBelow(const Byte* m, std::size_t bytes, long i) {   // any set bit in [0, i)
    for (long k = 0; k < i && static_cast<std::size_t>(k) < 8 * bytes; ++k)
        if (bitAt(m, bytes, k)) return true;
    return false;
}

// value = mag * 2^scale (and a little more when `sticky`), rounded to nearest even into format `f`.
inline void packFinite(const Fmt& f, Byte* r, bool sign, const Byte* mag, std::size_t magBytes, long scale, bool sticky) {
    const long length = static_cast<long>(8 * magBytes) - static_cast<long>(leadingZeros(mag, magBytes));
    if (length == 0) {
        putZero(f, r, sign);
        return;
    }
    long E = scale + length - 1;                           // the exponent of the leading bit
    const long p = f.explicitJ ? 64 : f.frac + 1, emin = 1 - f.bias;
    const long extra = E < emin ? emin - E : 0;            // bits a denormal loses
    const long K = p - extra;                              // bits of the significand kept
    const long drop = length - K;                          // bits of mag below them
    std::uint64_t I = 0;
    bool guard = false, st = sticky;
    if (drop <= 0) {
        I = load64(mag, magBytes) << (-drop);
    } else {
        guard = bitAt(mag, magBytes, drop - 1);
        st = st || anyBitBelow(mag, magBytes, drop - 1);
        if (drop < length) {
            Byte shifted[kMax];
            shiftRight(shifted, mag, magBytes, static_cast<std::uint64_t>(drop), 0);
            I = load64(shifted, 8);
        }
    }
    bool carried = false;
    if (guard && (st || (I & 1))) {
        ++I;
        carried = I == 0;
    }
    if (extra == 0) {
        if (carried || (K < 64 && I == (std::uint64_t{1} << K))) {
            I = std::uint64_t{1} << (K - 1);
            ++E;
        }
        const long biased = E + f.bias;
        if (biased >= static_cast<long>(f.maxExp)) {
            putInf(f, r, sign);
            return;
        }
        putBits(f, r, sign, f.explicitJ ? I : (I & ((std::uint64_t{1} << f.frac) - 1)), static_cast<unsigned>(biased));
        return;
    }
    // (a denormal: an IEEE one's integer is its fraction, and rounding up to the smallest normal carries into the exponent field by itself; the extended format says it with
    // its integer bit)
    if (f.explicitJ) putBits(f, r, sign, I, (I >> 63) != 0 ? 1 : 0);
    else putBits(f, r, sign, I, 0);
}
inline void packValue(const Fmt& f, Byte* r, const Value& v) {
    if (v.kind == Zero) {
        putZero(f, r, v.sign);
    } else if (v.kind == Inf) {
        putInf(f, r, v.sign);
    } else if (v.kind == NaN) {
        putNaN(f, r, v.sign, v.mant);
    } else {
        Byte m[8];
        fromU64(m, 8, v.mant);
        packFinite(f, r, v.sign, m, 8, static_cast<long>(v.exp) - 63, false);
    }
}

// The NaN that NaN operands hand on: the larger significand, the first on a tie.
inline Value pickNaN(const Value& a, const Value& b) {
    if (a.kind == NaN && b.kind == NaN) return b.mant > a.mant ? b : a;
    return a.kind == NaN ? a : b;
}

// a + b (a - b when `subtract`), format `f`.
inline void addSub(const Fmt& f, Byte* r, Value a, Value b, bool subtract) {
    if (subtract) b.sign = !b.sign;
    if (a.kind == NaN || b.kind == NaN) {
        const Value n = pickNaN(a, b);
        putNaN(f, r, n.sign, n.mant);
        return;
    }
    if (a.kind == Inf || b.kind == Inf) {
        if (a.kind == Inf && b.kind == Inf && a.sign != b.sign) putIndefinite(f, r);
        else putInf(f, r, a.kind == Inf ? a.sign : b.sign);
        return;
    }
    if (a.kind == Zero && b.kind == Zero) {
        putZero(f, r, a.sign && b.sign);
        return;
    }
    if (a.kind == Zero || b.kind == Zero) {
        packValue(f, r, a.kind == Zero ? b : a);
        return;
    }
    if (a.exp < b.exp) std::swap(a, b);   // (a has the larger exponent)
    constexpr std::size_t W = 24;
    Byte t[W], A[W], B[W], out[W];
    fromU64(t, W, a.mant);
    shiftLeft(A, t, W, 66);
    fromU64(t, W, b.mant);
    shiftLeft(t, t, W, 66);
    const std::uint64_t d = static_cast<std::uint64_t>(a.exp) - static_cast<std::uint64_t>(b.exp);
    bool sticky = false;
    if (d >= 8 * W) {
        sticky = true;
        zero(B, W);
    } else {
        sticky = anyBitBelow(t, W, static_cast<long>(d));
        shiftRight(B, t, W, d, 0);
    }
    bool sign = a.sign;
    if (a.sign == b.sign) {
        add(out, A, B, W);
    } else {
        if (compareUnsigned(A, B, W) < 0) {
            sign = b.sign;   // (only with equal exponents: nothing was shifted out)
            sub(out, B, A, W);
        } else {
            sub(out, A, B, W);
            if (sticky) {   // (the true difference lies just under this one)
                Byte one[W];
                fromU64(one, W, 1);
                sub(out, out, one, W);
            }
        }
        if (isZero(out, W) && !sticky) {
            putZero(f, r, false);
            return;
        }
    }
    packFinite(f, r, sign, out, W, static_cast<long>(a.exp) - 129, sticky);
}

inline void mulDiv(const Fmt& f, Byte* r, const Value& a, const Value& b, bool divide) {
    if (a.kind == NaN || b.kind == NaN) {
        const Value n = pickNaN(a, b);
        putNaN(f, r, n.sign, n.mant);
        return;
    }
    const bool sign = a.sign != b.sign;
    if (!divide) {
        if ((a.kind == Inf && b.kind == Zero) || (a.kind == Zero && b.kind == Inf)) return putIndefinite(f, r);
        if (a.kind == Inf || b.kind == Inf) return putInf(f, r, sign);
        if (a.kind == Zero || b.kind == Zero) return putZero(f, r, sign);
        Byte x[16], y[16], prod[16];
        fromU64(x, 16, a.mant);
        fromU64(y, 16, b.mant);
        mul(prod, x, y, 16);
        packFinite(f, r, sign, prod, 16, static_cast<long>(a.exp) + b.exp - 126, false);
        return;
    }
    if ((a.kind == Zero && b.kind == Zero) || (a.kind == Inf && b.kind == Inf)) return putIndefinite(f, r);
    if (a.kind == Inf || b.kind == Zero) return putInf(f, r, sign);
    if (a.kind == Zero || b.kind == Inf) return putZero(f, r, sign);
    constexpr std::size_t W = 24;
    Byte n[W], d[W], t[W], q[W], rem[W];
    fromU64(t, W, a.mant);
    shiftLeft(n, t, W, 66);
    fromU64(d, W, b.mant);
    divmod(q, rem, n, d, W);
    packFinite(f, r, sign, q, W, static_cast<long>(a.exp) - b.exp - 66, !isZero(rem, W));
}

inline void squareRoot(const Fmt& f, Byte* r, const Value& a) {
    if (a.kind == NaN) return putNaN(f, r, a.sign, a.mant);
    if (a.kind == Zero) return putZero(f, r, a.sign);
    if (a.sign) return putIndefinite(f, r);
    if (a.kind == Inf) return putInf(f, r, false);
    constexpr std::size_t W = 32;
    const long e2 = static_cast<long>(a.exp) - 63;
    const bool odd = (e2 & 1) != 0;
    Byte t[W], R[W], root[W] = {}, cand[W], sq[W];
    fromU64(t, W, a.mant);
    shiftLeft(t, t, W, odd ? 1 : 0);
    shiftLeft(R, t, W, 132);
    for (long bit = 100; bit >= 0; --bit) {   // (the root bit by bit: the largest value whose square does not pass R)
        std::memcpy(cand, root, W);
        cand[bit / 8] = static_cast<Byte>(cand[bit / 8] | (1u << (bit % 8)));
        mul(sq, cand, cand, W);
        if (compareUnsigned(sq, R, W) <= 0) std::memcpy(root, cand, W);
    }
    mul(sq, root, root, W);
    packFinite(f, r, false, root, W, (e2 - (odd ? 1 : 0)) / 2 - 66, compareUnsigned(sq, R, W) != 0);
}

// The comparison of two floats: -1, 0, 1, or 2 when one is a NaN.
inline int compare(const Value& a, const Value& b) {
    if (a.kind == NaN || b.kind == NaN) return 2;
    if (a.kind == Zero && b.kind == Zero) return 0;
    const auto rank = [](const Value& v) { return v.kind == Zero ? 0 : v.kind == Finite ? 1 : 2; };
    const auto magnitude = [&](const Value& x, const Value& y) {   // |x| against |y|
        if (rank(x) != rank(y)) return rank(x) < rank(y) ? -1 : 1;
        if (x.kind != Finite) return 0;
        if (x.exp != y.exp) return x.exp < y.exp ? -1 : 1;
        return x.mant == y.mant ? 0 : x.mant < y.mant ? -1 : 1;
    };
    const bool sa = a.kind != Zero && a.sign, sb = b.kind != Zero && b.sign;
    if (sa != sb) return sa ? -1 : 1;
    const int m = magnitude(a, b);
    return sa ? -m : m;
}

// The integer part of a finite value: floor (mode 0), ceil (1) or truncated (2) as a sign and a 64-bit magnitude; false when the value is too big for that (it is integral
// already then).
inline bool integerPart(const Value& v, int mode, bool& sign, std::uint64_t& mag) {
    sign = v.sign;
    if (v.exp >= 63) return false;
    if (v.exp < 0) {
        mag = (mode == 0 && v.sign) || (mode == 1 && !v.sign) ? 1 : 0;
        return true;
    }
    const unsigned sh = static_cast<unsigned>(63 - v.exp);
    std::uint64_t ip = v.mant >> sh;
    const bool fractional = (v.mant & ((std::uint64_t{1} << sh) - 1)) != 0;
    if (fractional && ((mode == 0 && v.sign) || (mode == 1 && !v.sign))) ++ip;
    mag = ip;
    return true;
}
inline void roundIntegral(const Fmt& f, Byte* r, const Value& v, int mode) {
    if (v.kind != Finite) return packValue(f, r, v);
    bool sign;
    std::uint64_t mag;
    if (!integerPart(v, mode, sign, mag)) return packValue(f, r, v);
    if (mag == 0) return putZero(f, r, v.sign);
    Byte m[8];
    fromU64(m, 8, mag);
    packFinite(f, r, sign, m, 8, 0, false);
}

inline bool floatOp(unsigned opcode, Byte* r, std::size_t rSize, const Byte* a, std::size_t aSize, const Byte* c, std::size_t cSize) {
    namespace gh = ghidra;
    const Fmt* fa = formatOf(aSize);
    switch (opcode) {
        case gh::CPUI_FLOAT_NAN: case gh::CPUI_FLOAT_EQUAL: case gh::CPUI_FLOAT_NOTEQUAL: case gh::CPUI_FLOAT_LESS: case gh::CPUI_FLOAT_LESSEQUAL: {
            if (!fa) return false;
            const Value x = unpack(*fa, a);
            if (opcode == gh::CPUI_FLOAT_NAN) {
                fromU64(r, rSize, x.kind == NaN ? 1 : 0);
                return true;
            }
            if (cSize != aSize) return false;
            const int cmp = compare(x, unpack(*fa, c));
            const bool yes = opcode == gh::CPUI_FLOAT_EQUAL      ? cmp == 0
                             : opcode == gh::CPUI_FLOAT_NOTEQUAL ? cmp != 0
                             : opcode == gh::CPUI_FLOAT_LESS     ? cmp == -1
                                                                 : (cmp == -1 || cmp == 0);
            fromU64(r, rSize, yes ? 1 : 0);
            return true;
        }
        case gh::CPUI_FLOAT_ADD: case gh::CPUI_FLOAT_SUB: case gh::CPUI_FLOAT_MULT: case gh::CPUI_FLOAT_DIV: {
            if (!fa || aSize != rSize || cSize != aSize) return false;
            const Value x = unpack(*fa, a), y = unpack(*fa, c);
            if (opcode == gh::CPUI_FLOAT_ADD || opcode == gh::CPUI_FLOAT_SUB) addSub(*fa, r, x, y, opcode == gh::CPUI_FLOAT_SUB);
            else mulDiv(*fa, r, x, y, opcode == gh::CPUI_FLOAT_DIV);
            return true;
        }
        case gh::CPUI_FLOAT_NEG: case gh::CPUI_FLOAT_ABS:
            if (!fa || aSize != rSize) return false;
            std::memcpy(r, a, aSize);
            r[aSize - 1] = static_cast<Byte>(opcode == gh::CPUI_FLOAT_NEG ? r[aSize - 1] ^ 0x80 : r[aSize - 1] & 0x7f);
            return true;
        case gh::CPUI_FLOAT_SQRT:
            if (!fa || aSize != rSize) return false;
            squareRoot(*fa, r, unpack(*fa, a));
            return true;
        case gh::CPUI_FLOAT_CEIL: case gh::CPUI_FLOAT_FLOOR: case gh::CPUI_FLOAT_ROUND: {
            if (!fa || aSize != rSize) return false;
            Value x = unpack(*fa, a);
            if (opcode == gh::CPUI_FLOAT_ROUND && x.kind == Finite) {   // (the p-code's round is floor(x + 0.5))
                Byte sum[16] = {};
                Value half;
                half.kind = Finite;
                half.exp = -1;
                half.mant = std::uint64_t{1} << 63;
                addSub(*fa, sum, x, half, false);
                x = unpack(*fa, sum);
            }
            roundIntegral(*fa, r, x, opcode == gh::CPUI_FLOAT_CEIL ? 1 : 0);
            return true;
        }
        case gh::CPUI_FLOAT_FLOAT2FLOAT: {
            const Fmt* fr = formatOf(rSize);
            if (!fa || !fr) return false;
            packValue(*fr, r, unpack(*fa, a));
            return true;
        }
        case gh::CPUI_FLOAT_INT2FLOAT: {
            const Fmt* fr = formatOf(rSize);
            if (!fr || aSize == 0 || aSize > 64) return false;
            const bool neg = signBit(a, aSize);
            Byte mag[64];
            if (neg) negate(mag, a, aSize);
            else std::memcpy(mag, a, aSize);
            if (isZero(mag, aSize)) putZero(*fr, r, false);
            else packFinite(*fr, r, neg, mag, aSize, 0, false);
            return true;
        }
        case gh::CPUI_FLOAT_TRUNC: {   // to a signed integer of any size; out of range, a NaN or an infinity give the integer indefinite (only the top bit set)
            if (!fa || rSize == 0 || rSize > 64) return false;
            const Value x = unpack(*fa, a);
            const std::size_t W = rSize > 9 ? rSize : 9;   // (room for the 64-bit significand and a sign)
            Byte mag[kMax] = {}, t[kMax] = {}, limit[kMax] = {};
            bool ok = x.kind == Zero || x.kind == Finite;
            if (ok && x.kind == Finite) {
                fromU64(t, W, x.mant);
                if (x.exp >= 63) {
                    if (x.exp > static_cast<int>(8 * rSize) - 1) ok = false;   // (2^(8n) and up is out of range whatever follows)
                    else shiftLeft(mag, t, W, static_cast<std::uint64_t>(x.exp - 63));
                } else if (x.exp >= 0) {
                    shiftRight(mag, t, W, static_cast<std::uint64_t>(63 - x.exp), 0);
                }   // (below 1: zero)
            }
            if (ok) {   // the magnitude must be under 2^(8n-1), or equal to it for a negative value
                limit[(8 * rSize - 1) / 8] = static_cast<Byte>(1u << ((8 * rSize - 1) % 8));
                const int c = compareUnsigned(mag, limit, W);
                ok = c < 0 || (c == 0 && x.sign);
            }
            if (!ok) {
                zero(r, rSize);
                r[rSize - 1] = 0x80;
                return true;
            }
            if (x.sign) negate(mag, mag, W);
            resize(r, rSize, mag, W);
            return true;
        }
        default: return false;
    }
}

}  // namespace soft

inline bool isFloatOp(unsigned opcode) {
    namespace gh = ghidra;
    switch (opcode) {
        case gh::CPUI_FLOAT_EQUAL: case gh::CPUI_FLOAT_NOTEQUAL: case gh::CPUI_FLOAT_LESS: case gh::CPUI_FLOAT_LESSEQUAL: case gh::CPUI_FLOAT_NAN: case gh::CPUI_FLOAT_ADD:
        case gh::CPUI_FLOAT_SUB: case gh::CPUI_FLOAT_MULT: case gh::CPUI_FLOAT_DIV: case gh::CPUI_FLOAT_NEG: case gh::CPUI_FLOAT_ABS: case gh::CPUI_FLOAT_SQRT:
        case gh::CPUI_FLOAT_INT2FLOAT: case gh::CPUI_FLOAT_FLOAT2FLOAT: case gh::CPUI_FLOAT_TRUNC: case gh::CPUI_FLOAT_CEIL: case gh::CPUI_FLOAT_FLOOR: case gh::CPUI_FLOAT_ROUND:
            return true;
        default: return false;
    }
}

// Any operation the wide path takes: the integer ones and the floats (those at 4, 8 and 10 bytes, at least one of them 10).
inline bool evaluateAny(unsigned opcode, Byte* r, std::size_t rSize, const Byte* a, std::size_t aSize, const Byte* c, std::size_t cSize, bool& divideByZero) {
    divideByZero = false;
    if (isFloatOp(opcode)) return soft::floatOp(opcode, r, rSize, a, aSize, c, cSize);
    return evaluate(opcode, r, rSize, a, aSize, c, cSize, divideByZero);
}

}  // namespace cb::decompile::sleigh::wide
