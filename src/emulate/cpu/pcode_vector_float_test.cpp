// cb-unity: alone (file-local names that other files in a batch also use)
// The float lane operations of engine/pcode_vector against an exactly rounded model: the exact value of each result (a big integer numerator, a power of two, and for a
// quotient a denominator; a square root as a big integer root with a "more than" mark) is rounded by dividing and comparing the remainder with half, in each of the five
// rounding modes, with the exceptions an IEEE operation raises worked out from the exact value. NaN, infinity, zero and denormal operands, and the three machines' NaN rules
// (see FpEnv), are in the operand mix. The x86 rules (NaN choice, minimum and maximum, compares, conversions, and the exception flags) are also run against the host's own SSE
// instructions on x86 builds. The ARM and PowerPC rules, which no host here can run, are checked against the model alone.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <string>
#include <vector>

#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define CB_VEC_HOST_SSE 1
#include <emmintrin.h>
#endif

#include "core/testing.hpp"
#include "emulate/cpu/pcode_vector.hpp"
#include "emulate/cpu/pcode_vector_model.hpp"

namespace cb {
namespace {

namespace vec = emulate::vec;
using namespace vecmodel;
using vec::Byte;
using Bytes = std::vector<Byte>;

struct Format {
    unsigned ew, eb, fb;
    int bias;
    U64 expMax() const { return (U64{1} << eb) - 1; }
    U64 fracMask() const { return (U64{1} << fb) - 1; }
    U64 sign() const { return U64{1} << (eb + fb); }
    int emin() const { return 1 - bias; }
};
const Format kF16{2, 5, 10, 15}, kF32{4, 8, 23, 127}, kF64{8, 11, 52, 1023}, kBf16{2, 8, 7, 127};
const Format kFormats[] = {kF16, kF32, kF64};

#ifndef CB_VEC_SEED
#define CB_VEC_SEED 0xf10a7   // (a different seed can be given on the compiler's command line to run the same checks on other operands)
#endif
std::mt19937_64 g_rng(CB_VEC_SEED);

// a decoded operand: exact value n * 2^e for a number
struct Val {
    enum Kind { Zero, Num, Inf, QNan, SNan } kind = Zero;
    bool neg = false;
    Big n;
    int e = 0;
    U64 raw = 0;
    bool isNan() const { return kind == QNan || kind == SNan; }
};
Val decode(const Format& f, U64 bits, const vec::FpEnv& env, unsigned& flags) {
    Val v;
    v.neg = (bits & f.sign()) != 0;
    const U64 ex = (bits >> f.fb) & f.expMax(), fr = bits & f.fracMask();
    v.raw = bits;
    if (ex == f.expMax()) {
        v.kind = fr == 0 ? Val::Inf : ((fr >> (f.fb - 1)) & 1 ? Val::QNan : Val::SNan);
        return v;
    }
    if (ex == 0) {
        if (fr == 0) return v;
        flags |= vec::kInputDenormal;
        if (env.flushInputs) return v;
        v.kind = Val::Num;
        v.n = bigU(fr);
        v.e = f.emin() - static_cast<int>(f.fb);
        return v;
    }
    v.kind = Val::Num;
    v.n = bigU((U64{1} << f.fb) | fr);
    v.e = static_cast<int>(ex) - f.bias - static_cast<int>(f.fb);
    return v;
}
U64 quiet(const Format& f) { return U64{1} << (f.fb - 1); }
U64 infBits(const Format& f, bool neg) { return (neg ? f.sign() : 0) | (f.expMax() << f.fb); }
U64 zeroBits(const Format& f, bool neg) { return neg ? f.sign() : 0; }
U64 maxBits(const Format& f, bool neg) { return (neg ? f.sign() : 0) | ((f.expMax() - 1) << f.fb) | f.fracMask(); }
U64 defNan(const Format& f, const vec::FpEnv& env) { return (env.nan == vec::NanRule::X86 ? f.sign() : 0) | (f.expMax() << f.fb) | quiet(f); }

// The NaN a machine returns for operands in order (false if none is a NaN).
bool nanResult(const Format& f, const vec::FpEnv& env, const std::vector<const Val*>& ops, unsigned& flags, U64& out) {
    bool any = false, snan = false;
    for (const Val* o : ops) {
        any = any || o->isNan();
        snan = snan || o->kind == Val::SNan;
    }
    if (!any) return false;
    if (snan) flags |= vec::kInvalid;
    const Val* pick = nullptr;
    if (env.nan == vec::NanRule::Arm) {
        if (env.defaultNan) {
            out = defNan(f, env);
            return true;
        }
        for (const Val* o : ops)
            if (!pick && o->kind == Val::SNan) pick = o;
        for (const Val* o : ops)
            if (!pick && o->kind == Val::QNan) pick = o;
    } else {
        for (const Val* o : ops)
            if (!pick && o->isNan()) pick = o;
    }
    out = pick->raw | quiet(f);
    return true;
}

// The float nearest (by `env`) to the exact value (N * 2^k / D), plus an infinitesimal when `above`; raises the exceptions of the rounding.
U64 roundExact(const Format& f, const vec::FpEnv& env, bool neg, const Big& N, int k, const Big& D, bool above, unsigned& flags) {
    if (N.zero()) return zeroBits(f, neg);
    const auto cmpPow = [&](int e) {   // N 2^k / D against 2^e
        const Big lhs = shlBig(N, static_cast<unsigned>(std::max(k - e, 0)));
        const Big rhs = shlBig(D, static_cast<unsigned>(std::max(e - k, 0)));
        return cmp(lhs, rhs);
    };
    int e = static_cast<int>(bitLength(N)) - static_cast<int>(bitLength(D)) + k;
    while (cmpPow(e) < 0) --e;
    while (cmpPow(e + 1) >= 0) ++e;
    const int emin = f.emin();
    const bool tiny = e < emin;
    if (tiny && env.flushResults) {
        flags |= vec::kUnderflow | (env.nan == vec::NanRule::X86 ? vec::kInexact : 0u);
        return zeroBits(f, neg);
    }
    int qe = std::max(e, emin) - static_cast<int>(f.fb);
    const int s = k - qe;
    const Big num = shlBig(N, static_cast<unsigned>(std::max(s, 0))), den = shlBig(D, static_cast<unsigned>(std::max(-s, 0)));
    Big m, rem;
    divMod(num, den, m, rem);
    int half = cmp(shlBig(rem, 1), den);   // remainder against half
    if (rem.zero() && above) half = -1;
    else if (half == 0 && above) half = 1;
    const bool inexact = !rem.zero() || above;
    bool inc = false;
    const bool odd = bitSet(m, 0);
    switch (env.round) {
        case vec::Round::NearestEven: inc = half > 0 || (half == 0 && odd); break;
        case vec::Round::NearestAway: inc = half >= 0 && inexact; break;
        case vec::Round::TowardZero: inc = false; break;
        case vec::Round::TowardPositive: inc = !neg && inexact; break;
        case vec::Round::TowardNegative: inc = neg && inexact; break;
    }
    if (inc) m = m + bigU(1);
    if (inexact) flags |= vec::kInexact;
    if (tiny && inexact) flags |= vec::kUnderflow;
    if (cmp(m, shlBig(bigU(1), f.fb + 1)) >= 0) {
        m = shrMag(m, 1);
        ++qe;
    }
    const U64 mv = lowBits(m, 64);
    if (cmp(m, shlBig(bigU(1), f.fb)) >= 0) {
        const long field = static_cast<long>(qe) + static_cast<long>(f.fb) + f.bias;
        if (field >= static_cast<long>(f.expMax())) {
            flags |= vec::kOverflow | vec::kInexact;
            const bool toInf = env.round == vec::Round::NearestEven || env.round == vec::Round::NearestAway || (env.round == vec::Round::TowardPositive && !neg) ||
                               (env.round == vec::Round::TowardNegative && neg);
            return toInf ? infBits(f, neg) : maxBits(f, neg);
        }
        return (neg ? f.sign() : 0) | (static_cast<U64>(field) << f.fb) | (mv & f.fracMask());
    }
    return (neg ? f.sign() : 0) | mv;
}

// the exact sum of two signed values (n 2^e) as sign, magnitude, exponent
void exactSum(const Val& a, bool negA, const Val& b, bool negB, bool& neg, Big& mag, int& k) {
    const int ke = std::min(a.n.zero() ? b.e : a.e, b.n.zero() ? a.e : b.e);
    Big x = a.n.zero() ? Big{} : shlBig(a.n, static_cast<unsigned>(a.e - ke)), y = b.n.zero() ? Big{} : shlBig(b.n, static_cast<unsigned>(b.e - ke));
    if (negA) x = -x;
    if (negB) y = -y;
    Big s = x + y;
    neg = s.neg;
    if (neg) s = -s;
    mag = s;
    k = ke;
}

U64 modelAdd(const Format& f, const vec::FpEnv& env, U64 ab, U64 bb, bool subtract, unsigned& flags) {
    const Val a = decode(f, ab, env, flags), b0 = decode(f, bb, env, flags);
    U64 r;
    if (nanResult(f, env, {&a, &b0}, flags, r)) return r;
    Val b = b0;
    if (subtract) b.neg = !b.neg;
    if (a.kind == Val::Inf && b.kind == Val::Inf && a.neg != b.neg) { flags |= vec::kInvalid; return defNan(f, env); }
    if (a.kind == Val::Inf) return infBits(f, a.neg);
    if (b.kind == Val::Inf) return infBits(f, b.neg);
    if (a.kind == Val::Zero && b.kind == Val::Zero) return a.neg == b.neg ? zeroBits(f, a.neg) : zeroBits(f, env.round == vec::Round::TowardNegative);
    bool neg;
    Big mag;
    int k;
    exactSum(a, a.neg, b, b.neg, neg, mag, k);
    if (mag.zero()) return zeroBits(f, env.round == vec::Round::TowardNegative);
    return roundExact(f, env, neg, mag, k, bigU(1), false, flags);
}
U64 modelMul(const Format& f, const vec::FpEnv& env, U64 ab, U64 bb, unsigned& flags, bool extended = false) {
    const Val a = decode(f, ab, env, flags), b = decode(f, bb, env, flags);
    U64 r;
    if (nanResult(f, env, {&a, &b}, flags, r)) return r;
    const bool neg = a.neg != b.neg;
    if ((a.kind == Val::Inf && b.kind == Val::Zero) || (a.kind == Val::Zero && b.kind == Val::Inf)) {
        if (extended) return (neg ? f.sign() : 0) | (static_cast<U64>(f.bias + 1) << f.fb);   // fmulx: +-2.0
        flags |= vec::kInvalid;
        return defNan(f, env);
    }
    if (a.kind == Val::Inf || b.kind == Val::Inf) return infBits(f, neg);
    if (a.kind == Val::Zero || b.kind == Val::Zero) return zeroBits(f, neg);
    return roundExact(f, env, neg, a.n * b.n, a.e + b.e, bigU(1), false, flags);
}
U64 modelDiv(const Format& f, const vec::FpEnv& env, U64 ab, U64 bb, unsigned& flags) {
    const Val a = decode(f, ab, env, flags), b = decode(f, bb, env, flags);
    U64 r;
    if (nanResult(f, env, {&a, &b}, flags, r)) return r;
    const bool neg = a.neg != b.neg;
    if ((a.kind == Val::Inf && b.kind == Val::Inf) || (a.kind == Val::Zero && b.kind == Val::Zero)) { flags |= vec::kInvalid; return defNan(f, env); }
    if (a.kind == Val::Inf) return infBits(f, neg);
    if (b.kind == Val::Inf || a.kind == Val::Zero) return zeroBits(f, neg);
    if (b.kind == Val::Zero) { flags |= vec::kDivideByZero; return infBits(f, neg); }
    return roundExact(f, env, neg, a.n, a.e - b.e, b.n, false, flags);
}
U64 modelSqrt(const Format& f, const vec::FpEnv& env, U64 ab, unsigned& flags) {
    const Val a = decode(f, ab, env, flags);
    U64 r;
    if (nanResult(f, env, {&a}, flags, r)) return r;
    if (a.kind == Val::Zero) return zeroBits(f, a.neg);
    if (a.neg) { flags |= vec::kInvalid; return defNan(f, env); }
    if (a.kind == Val::Inf) return infBits(f, false);
    Big n = a.n;
    int e = a.e;
    if (e & 1) { n = shlBig(n, 1); --e; }
    const unsigned s = static_cast<unsigned>(std::max(0, (140 - static_cast<int>(bitLength(n))) / 2 + 1));
    const Big scaled = shlBig(n, 2 * s);
    const Big root = isqrt(scaled);
    const bool inexact = cmp(root * root, scaled) != 0;
    return roundExact(f, env, false, shlBig(root, 1) + (inexact ? bigU(1) : Big{}), e / 2 - static_cast<int>(s) - 1, bigU(1), false, flags);
}
U64 modelFma(const Format& f, const vec::FpEnv& env, U64 ab, U64 bb, U64 cb, bool negProduct, bool negAddend, unsigned& flags) {
    const Val a = decode(f, ab, env, flags), b = decode(f, bb, env, flags), c = decode(f, cb, env, flags);
    const bool prodInvalid = (a.kind == Val::Inf && b.kind == Val::Zero) || (a.kind == Val::Zero && b.kind == Val::Inf);
    if (env.nan == vec::NanRule::Arm && prodInvalid && c.kind == Val::QNan && !a.isNan() && !b.isNan()) { flags |= vec::kInvalid; return defNan(f, env); }
    U64 r;
    // the order a NaN is looked for in: ARM, the addend first; PowerPC, the first product operand, the addend, the second product operand
    const std::vector<const Val*> order = env.nan == vec::NanRule::Arm ? std::vector<const Val*>{&c, &a, &b} : env.nan == vec::NanRule::Ppc ? std::vector<const Val*>{&a, &c, &b} : std::vector<const Val*>{&a, &b, &c};
    if (nanResult(f, env, order, flags, r)) return r;
    if (prodInvalid) { flags |= vec::kInvalid; return defNan(f, env); }
    const bool sp = (a.neg != b.neg) != negProduct, sc = c.neg != negAddend;
    if (a.kind == Val::Inf || b.kind == Val::Inf) {
        if (c.kind == Val::Inf && sc != sp) { flags |= vec::kInvalid; return defNan(f, env); }
        return infBits(f, sp);
    }
    if (c.kind == Val::Inf) return infBits(f, sc);
    const bool pz = a.kind == Val::Zero || b.kind == Val::Zero;
    if (pz && c.kind == Val::Zero) return sp == sc ? zeroBits(f, sp) : zeroBits(f, env.round == vec::Round::TowardNegative);
    Val p;
    p.kind = Val::Num;
    if (!pz) {
        p.n = a.n * b.n;
        p.e = a.e + b.e;
    }
    bool neg;
    Big mag;
    int k;
    exactSum(p, sp, c, sc, neg, mag, k);
    if (mag.zero()) return zeroBits(f, env.round == vec::Round::TowardNegative);
    return roundExact(f, env, neg, mag, k, bigU(1), false, flags);
}

// exact comparison of two number values: -1, 0, 1
int cmpVal(const Val& a, const Val& b) {
    const auto signedValue = [](const Val& v, int ke) { Big x = v.kind == Val::Zero ? Big{} : shlBig(v.n, static_cast<unsigned>(v.e - ke)); return v.neg ? -x : x; };
    const int ke = std::min(a.kind == Val::Zero ? b.e : a.e, b.kind == Val::Zero ? a.e : b.e);
    if (a.kind == Val::Inf || b.kind == Val::Inf) {
        const int x = a.kind == Val::Inf ? (a.neg ? -2 : 2) : 0, y = b.kind == Val::Inf ? (b.neg ? -2 : 2) : 0;
        if (x != y) return x < y ? -1 : 1;
        if (x != 0) return 0;
    }
    return cmp(signedValue(a, ke), signedValue(b, ke));
}
int modelOrder(const Val& a, const Val& b) { return (a.isNan() || b.isNan()) ? 2 : cmpVal(a, b); }

U64 modelMinMax(const Format& f, const vec::FpEnv& env, U64 ab, U64 bb, bool isMax, unsigned& flags) {
    const Val a = decode(f, ab, env, flags), b = decode(f, bb, env, flags);
    const auto flushed = [&](const Val& v, U64 bits) { return v.kind == Val::Zero ? zeroBits(f, v.neg) : bits; };
    U64 r;
    switch (env.minmax) {
        case vec::MinMax::X86: {
            if (a.isNan() || b.isNan()) { flags |= vec::kInvalid; return flushed(b, bb); }
            const int o = cmpVal(a, b);
            if (o == 0) return flushed(b, bb);
            return (isMax ? o > 0 : o < 0) ? flushed(a, ab) : flushed(b, bb);
        }
        case vec::MinMax::ArmNumber:
            if (a.kind == Val::QNan && !b.isNan()) return flushed(b, bb);
            if (b.kind == Val::QNan && !a.isNan()) return flushed(a, ab);
            [[fallthrough]];
        default: {
            if (nanResult(f, env, {&a, &b}, flags, r)) return r;
            if (a.kind == Val::Zero && b.kind == Val::Zero && a.neg != b.neg) return zeroBits(f, !isMax);
            const int o = cmpVal(a, b);
            return (isMax ? o >= 0 : o <= 0) ? flushed(a, ab) : flushed(b, bb);
        }
    }
}

// the exact integer rounding of a number value (n 2^e) to an integer: rounded magnitude, and whether it was inexact
Big roundToInteger(const Val& v, vec::Round mode, bool& inexact) {
    Big q, rem;
    if (v.e >= 0) { inexact = false; return shlBig(v.n, static_cast<unsigned>(v.e)); }
    const Big den = shlBig(bigU(1), static_cast<unsigned>(-v.e));
    divMod(v.n, den, q, rem);
    inexact = !rem.zero();
    const int half = cmp(shlBig(rem, 1), den);
    bool inc = false;
    switch (mode) {
        case vec::Round::NearestEven: inc = half > 0 || (half == 0 && bitSet(q, 0)); break;
        case vec::Round::NearestAway: inc = half >= 0 && inexact; break;
        case vec::Round::TowardZero: inc = false; break;
        case vec::Round::TowardPositive: inc = !v.neg && inexact; break;
        case vec::Round::TowardNegative: inc = v.neg && inexact; break;
    }
    return inc ? q + bigU(1) : q;
}

// ---- operands
U64 specialFloat(const Format& f) {
    const U64 sign = (g_rng() & 1) ? f.sign() : 0;
    switch (g_rng() % 24) {
        case 0: return sign;                                                          // zero
        case 1: return sign | (f.expMax() << f.fb);                                    // infinity
        case 2: return sign | (f.expMax() << f.fb) | (U64{1} << (f.fb - 1)) | (g_rng() & (f.fracMask() >> 2));   // quiet NaN
        case 3: return sign | (f.expMax() << f.fb) | std::max<U64>(1, g_rng() & (f.fracMask() >> 1));            // signaling NaN
        case 4: return sign | (1ull << f.fb);                                          // smallest normal
        case 5: return sign | 1;                                                       // smallest denormal
        case 6: return sign | f.fracMask();                                            // largest denormal
        case 7: return sign | ((f.expMax() - 1) << f.fb) | f.fracMask();               // largest finite
        case 8: return sign | (static_cast<U64>(f.bias) << f.fb);                      // 1.0
        case 9: return sign | (static_cast<U64>(f.bias + 1) << f.fb);                  // 2.0
        case 10: return sign | (static_cast<U64>(f.bias - 1) << f.fb);                 // 0.5
        case 11: return sign | (g_rng() & f.fracMask());                               // a denormal
        case 12: return sign | ((static_cast<U64>(f.bias) + (g_rng() % 6)) << f.fb) | (g_rng() & f.fracMask());   // small magnitudes
        case 13: return sign | ((static_cast<U64>(f.bias) - (g_rng() % 6)) << f.fb) | (g_rng() & f.fracMask());
        case 14: return sign | ((f.expMax() - 1 - (g_rng() % 3)) << f.fb) | (g_rng() & f.fracMask());           // near overflow
        case 15: return sign | ((1 + (g_rng() % 3)) << f.fb) | (g_rng() & f.fracMask());                        // near underflow
        default: return g_rng() & (f.sign() | (f.expMax() << f.fb) | f.fracMask());
    }
}
vec::FpEnv randomEnv() {
    vec::FpEnv env;
    env.round = static_cast<vec::Round>(g_rng() % 5);
    env.nan = static_cast<vec::NanRule>(g_rng() % 3);
    env.minmax = static_cast<vec::MinMax>(g_rng() % 4);
    env.flushInputs = (g_rng() % 6) == 0;
    env.flushResults = (g_rng() % 6) == 0;
    env.defaultNan = (g_rng() % 4) == 0;
    return env;
}

unsigned g_fails = 0;
void report(const char* what, const Format& f, const vec::FpEnv& env, U64 a, U64 b, U64 c, U64 got, U64 want, unsigned gf, unsigned wf) {
    if (++g_fails <= 12)
        std::printf("  %s ew=%u round=%d nan=%d mm=%d fin=%d fout=%d dn=%d a=%llx b=%llx c=%llx: got %llx/%x want %llx/%x\n", what, f.ew, static_cast<int>(env.round), static_cast<int>(env.nan),
                    static_cast<int>(env.minmax), env.flushInputs, env.flushResults, env.defaultNan, static_cast<unsigned long long>(a), static_cast<unsigned long long>(b),
                    static_cast<unsigned long long>(c), static_cast<unsigned long long>(got), gf, static_cast<unsigned long long>(want), wf);
}

}  // namespace

CB_TEST("emulate.pcode_vector.float_arithmetic_is_exactly_rounded_in_every_mode", [] {
    g_fails = 0;
    for (const Format& f : kFormats)
        for (int iter = 0; iter < 2500; ++iter) {
            const vec::FpEnv env = randomEnv();
            const U64 a = specialFloat(f), b = specialFloat(f), c = specialFloat(f);
            Bytes av(f.ew), bv(f.ew), cv(f.ew), out(f.ew);
            vec::store(av.data(), f.ew, a), vec::store(bv.data(), f.ew, b), vec::store(cv.data(), f.ew, c);
            const auto run2 = [&](const char* name, vec::Fp2 op, U64 (*model)(const Format&, const vec::FpEnv&, U64, U64, unsigned&)) {
                unsigned gf = 0, wf = 0;
                vec::fp2(out.data(), av.data(), bv.data(), f.ew, f.ew, op, env, gf);
                const U64 want = model(f, env, a, b, wf), got = vec::load(out.data(), f.ew);
                if (got != want || gf != wf) report(name, f, env, a, b, 0, got, want, gf, wf);
            };
            run2("add", vec::Fp2::Add, [](const Format& f, const vec::FpEnv& e, U64 x, U64 y, unsigned& fl) { return modelAdd(f, e, x, y, false, fl); });
            run2("sub", vec::Fp2::Sub, [](const Format& f, const vec::FpEnv& e, U64 x, U64 y, unsigned& fl) { return modelAdd(f, e, x, y, true, fl); });
            run2("mul", vec::Fp2::Mul, [](const Format& f, const vec::FpEnv& e, U64 x, U64 y, unsigned& fl) { return modelMul(f, e, x, y, fl); });
            run2("mulx", vec::Fp2::MulExtended, [](const Format& f, const vec::FpEnv& e, U64 x, U64 y, unsigned& fl) { return modelMul(f, e, x, y, fl, true); });
            run2("div", vec::Fp2::Div, modelDiv);
            run2("min", vec::Fp2::Min, [](const Format& f, const vec::FpEnv& e, U64 x, U64 y, unsigned& fl) { return modelMinMax(f, e, x, y, false, fl); });
            run2("max", vec::Fp2::Max, [](const Format& f, const vec::FpEnv& e, U64 x, U64 y, unsigned& fl) { return modelMinMax(f, e, x, y, true, fl); });
            run2("absdiff", vec::Fp2::AbsDiff, [](const Format& f, const vec::FpEnv& e, U64 x, U64 y, unsigned& fl) { return modelAdd(f, e, x, y, true, fl) & ~f.sign(); });
            {
                unsigned gf = 0, wf = 0;
                vec::fp1(out.data(), av.data(), f.ew, f.ew, vec::Fp1::Sqrt, env, gf);
                const U64 want = modelSqrt(f, env, a, wf), got = vec::load(out.data(), f.ew);
                if (got != want || gf != wf) report("sqrt", f, env, a, 0, 0, got, want, gf, wf);
            }
            for (int op = 0; op < 4; ++op) {
                unsigned gf = 0, wf = 0;
                vec::fp3(out.data(), av.data(), bv.data(), cv.data(), f.ew, f.ew, static_cast<vec::Fp3>(op), env, gf);
                const U64 want = modelFma(f, env, a, b, c, op == 2 || op == 3, op == 1 || op == 3, wf), got = vec::load(out.data(), f.ew);
                if (got != want || gf != wf) report("fma", f, env, a, b, c, got, want, gf, wf);
            }
            {
                U64 want = a & ~f.sign();
                unsigned absFlags = 0;
                vec::fp1(out.data(), av.data(), f.ew, f.ew, vec::Fp1::Abs, env, absFlags);
                CB_CHECK_EQ(vec::load(out.data(), f.ew), want);
                want = a ^ f.sign();
                unsigned fl = 0;
                vec::fp1(out.data(), av.data(), f.ew, f.ew, vec::Fp1::Neg, env, fl);
                CB_CHECK_EQ(vec::load(out.data(), f.ew), want);
                CB_CHECK_EQ(fl, 0u);
            }
        }
    CB_CHECK_EQ(g_fails, 0u);
});

CB_TEST("emulate.pcode_vector.float_compare_round_and_convert_match_the_exact_model", [] {
    g_fails = 0;
    for (const Format& f : kFormats)
        for (int iter = 0; iter < 2500; ++iter) {
            const vec::FpEnv env = randomEnv();
            const U64 a = specialFloat(f), b = specialFloat(f);
            Bytes av(f.ew), bv(f.ew), out(f.ew);
            vec::store(av.data(), f.ew, a), vec::store(bv.data(), f.ew, b);
            // compare
            for (int c = 0; c < 8; ++c)
                for (const bool signaling : {false, true}) {
                    unsigned gf = 0, wf = 0;
                    vec::fpCompare(out.data(), av.data(), bv.data(), f.ew, f.ew, static_cast<vec::FpCmp>(c), signaling, env, gf);
                    const Val x = decode(f, a, env, wf), y = decode(f, b, env, wf);
                    const int o = modelOrder(x, y);
                    if (o == 2 && (signaling || x.kind == Val::SNan || y.kind == Val::SNan)) wf |= vec::kInvalid;
                    bool t = false;
                    switch (static_cast<vec::FpCmp>(c)) {
                        case vec::FpCmp::Eq: t = o == 0; break;
                        case vec::FpCmp::Ne: t = o != 0; break;
                        case vec::FpCmp::Lt: t = o == -1; break;
                        case vec::FpCmp::Le: t = o == -1 || o == 0; break;
                        case vec::FpCmp::Gt: t = o == 1; break;
                        case vec::FpCmp::Ge: t = o == 1 || o == 0; break;
                        case vec::FpCmp::Unordered: t = o == 2; break;
                        case vec::FpCmp::Ordered: t = o != 2; break;
                    }
                    const U64 want = t ? (f.ew == 8 ? ~U64{0} : (U64{1} << (8 * f.ew)) - 1) : 0, got = vec::load(out.data(), f.ew);
                    if (got != want || gf != wf) report("compare", f, env, a, b, static_cast<U64>(c), got, want, gf, wf);
                }
            // round to integral
            for (const bool exact : {false, true}) {
                unsigned gf = 0, wf = 0;
                vec::fp1(out.data(), av.data(), f.ew, f.ew, exact ? vec::Fp1::RoundToIntExact : vec::Fp1::RoundToInt, env, gf);
                U64 want;
                const Val x = decode(f, a, env, wf);
                U64 nanBits;
                if (nanResult(f, env, {&x}, wf, nanBits)) want = nanBits;
                else if (x.kind == Val::Zero) want = zeroBits(f, x.neg);
                else if (x.kind == Val::Inf) want = infBits(f, x.neg);
                else {
                    bool inexact;
                    const Big r = roundToInteger(x, env.round, inexact);
                    if (inexact && exact) wf |= vec::kInexact;
                    vec::FpEnv plain;
                    unsigned scratch = 0;
                    want = r.zero() ? zeroBits(f, x.neg) : roundExact(f, plain, x.neg, r, 0, bigU(1), false, scratch);
                }
                const U64 got = vec::load(out.data(), f.ew);
                if (got != want || gf != wf) report(exact ? "rintx" : "rint", f, env, a, 0, 0, got, want, gf, wf);
            }
            // float -> integer lanes of 1, 2, 4 and 8 bytes, signed and unsigned, with a fraction-bit scale
            for (unsigned iEw : {1u, 2u, 4u, 8u})
                for (const bool sg : {false, true})
                    for (const bool x86 : {false, true}) {
                        const unsigned fracBits = (g_rng() % 4 == 0) ? static_cast<unsigned>(g_rng() % 40) : 0;
                        Bytes io(8, 0);
                        unsigned gf = 0, wf = 0;
                        vec::floatToInt(io.data(), av.data(), f.ew, f.ew, iEw, sg ? vec::Sign::Signed : vec::Sign::Unsigned, fracBits,
                                        x86 ? vec::IntOverflow::X86Indefinite : vec::IntOverflow::Saturate, env, gf);
                        const Val x = decode(f, a, env, wf);
                        const unsigned w = 8 * iEw;
                        const Big hi = sg ? shlBig(bigU(1), w - 1) - bigU(1) : shlBig(bigU(1), w) - bigU(1), lo = sg ? -shlBig(bigU(1), w - 1) : Big{};
                        const U64 indefinite = sg ? (U64{1} << (w - 1)) : (w == 64 ? ~U64{0} : (U64{1} << w) - 1);
                        U64 want;
                        if (x.isNan()) { wf |= vec::kInvalid; want = x86 ? indefinite : 0; }
                        else if (x.kind == Val::Zero) want = 0;
                        else {
                            Val scaled = x;
                            scaled.e += static_cast<int>(fracBits);
                            Big r;
                            bool inexact = false;
                            bool inf = x.kind == Val::Inf;
                            if (!inf) r = roundToInteger(scaled, env.round, inexact);
                            Big sr = x.neg ? -r : r;
                            const bool outside = inf || cmp(sr, hi) > 0 || cmp(sr, lo) < 0;
                            if (outside) {
                                wf |= vec::kInvalid;
                                if (x86) want = indefinite;
                                else want = lowBits(x.neg ? lo : hi, w);
                            } else {
                                want = lowBits(sr, w);
                                if (inexact) wf |= vec::kInexact;
                            }
                        }
                        const U64 got = vec::load(io.data(), iEw);
                        if (got != want || gf != wf) report("floatToInt", f, env, a, iEw * 4 + (sg ? 2 : 0) + (x86 ? 1 : 0), fracBits, got, want, gf, wf);
                    }
            // integer -> float, from lanes of every width
            for (unsigned iEw : {1u, 2u, 4u, 8u})
                for (const bool sg : {false, true}) {
                    const U64 raw = g_rng() & ((iEw == 8) ? ~U64{0} : (U64{1} << (8 * iEw)) - 1);
                    Bytes iv(8, 0);
                    vec::store(iv.data(), iEw, raw);
                    const unsigned fracBits = (g_rng() % 4 == 0) ? static_cast<unsigned>(g_rng() % 40) : 0;
                    unsigned gf = 0, wf = 0;
                    vec::intToFloat(out.data(), iv.data(), iEw, iEw, f.ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned, fracBits, env, gf);
                    Big v = laneBig(raw, iEw, sg);
                    const bool neg = v.neg;
                    if (neg) v = -v;
                    const U64 want = roundExact(f, env, neg, v, -static_cast<int>(fracBits), bigU(1), false, wf), got = vec::load(out.data(), f.ew);
                    if (got != want || gf != wf) report("intToFloat", f, env, raw, iEw * 2 + (sg ? 1 : 0), fracBits, got, want, gf, wf);
                }
            // float -> float
            for (const Format& g : kFormats) {
                unsigned gf = 0, wf = 0;
                Bytes cv(g.ew);
                vec::floatConvert(cv.data(), av.data(), f.ew, f.ew, g.ew, env, gf);
                const Val x = decode(f, a, env, wf);
                U64 want;
                if (x.isNan()) {
                    if (x.kind == Val::SNan) wf |= vec::kInvalid;
                    if (env.nan == vec::NanRule::Arm && env.defaultNan) want = defNan(g, env);
                    else {
                        const U64 frac = a & f.fracMask();
                        const U64 moved = g.fb >= f.fb ? frac << (g.fb - f.fb) : frac >> (f.fb - g.fb);
                        want = (x.neg ? g.sign() : 0) | (g.expMax() << g.fb) | moved | quiet(g);
                    }
                } else if (x.kind == Val::Zero) want = zeroBits(g, x.neg);
                else if (x.kind == Val::Inf) want = infBits(g, x.neg);
                else want = roundExact(g, env, x.neg, x.n, x.e, bigU(1), false, wf);
                const U64 got = vec::load(cv.data(), g.ew);
                if (got != want || gf != wf) report("floatConvert", g, env, a, f.ew, 0, got, want, gf, wf);
            }
        }
    // bfloat16: widening is the high half of a single, narrowing rounds the single
    for (int iter = 0; iter < 3000; ++iter) {
        const vec::FpEnv env = randomEnv();
        const U64 s = specialFloat(kF32);
        Bytes sv(4), bf(2), back(4);
        vec::store(sv.data(), 4, s);
        unsigned gf = 0, wf = 0;
        vec::singleToBfloat16(bf.data(), sv.data(), 4, env, gf);
        const Val x = decode(kF32, s, env, wf);
        U64 want;
        if (x.isNan()) {
            if (x.kind == Val::SNan) wf |= vec::kInvalid;
            want = (env.nan == vec::NanRule::Arm && env.defaultNan) ? defNan(kBf16, env) : (x.neg ? kBf16.sign() : 0) | (kBf16.expMax() << kBf16.fb) | ((s & kF32.fracMask()) >> 16) | quiet(kBf16);
        } else if (x.kind == Val::Zero) want = zeroBits(kBf16, x.neg);
        else if (x.kind == Val::Inf) want = infBits(kBf16, x.neg);
        else want = roundExact(kBf16, env, x.neg, x.n, x.e, bigU(1), false, wf);
        if (vec::load(bf.data(), 2) != want || gf != wf) report("singleToBfloat16", kBf16, env, s, 0, 0, vec::load(bf.data(), 2), want, gf, wf);
        vec::bfloat16ToSingle(back.data(), bf.data(), 2);
        CB_CHECK_EQ(vec::load(back.data(), 4), vec::load(bf.data(), 2) << 16);
    }
    CB_CHECK_EQ(g_fails, 0u);
});

#ifdef CB_VEC_HOST_SSE
// The x86 rules against the host's own SSE: for single and double precision, the nearest-even arithmetic, the minimum, maximum, compares and conversions, and the exception flags
// the instructions set in MXCSR, on the operand mix with NaNs, infinities, zeros and denormals.
namespace {
// (clang schedules the intrinsics apart from the reads of MXCSR, so the exception flags of an operation cannot be read back in a build by it: the results are still compared)
#if defined(__clang__)
constexpr bool kHostFlagsReliable = false;
#else
constexpr bool kHostFlagsReliable = true;
#endif
unsigned mxcsrFlags(unsigned csr) {
    unsigned f = 0;
    if (csr & 1) f |= vec::kInvalid;
    if (csr & 2) f |= vec::kInputDenormal;
    if (csr & 4) f |= vec::kDivideByZero;
    if (csr & 8) f |= vec::kOverflow;
    if (csr & 16) f |= vec::kUnderflow;
    if (csr & 32) f |= vec::kInexact;
    return f;
}
float asF(U64 b) { std::uint32_t v = static_cast<std::uint32_t>(b); float f; std::memcpy(&f, &v, 4); return f; }
U64 fBits(float f) { std::uint32_t v; std::memcpy(&v, &f, 4); return v; }
double asD(U64 b) { double d; std::memcpy(&d, &b, 8); return d; }
U64 dBits(double d) { U64 v; std::memcpy(&v, &d, 8); return v; }
}  // namespace

CB_TEST("emulate.pcode_vector.x86_float_rules_match_the_host_sse_unit", [] {
    g_fails = 0;
    vec::FpEnv env;   // nearest even, the x86 NaN rule and minimum
    const unsigned savedCsr = _mm_getcsr();
    _mm_setcsr(savedCsr & ~0x8040u & ~0x6000u);   // no flush-to-zero, no denormals-are-zero, round to nearest
    for (const bool dbl : {false, true}) {
        const Format& f = dbl ? kF64 : kF32;
        for (int iter = 0; iter < 20000; ++iter) {
            const U64 a = specialFloat(f), b = specialFloat(f);
            Bytes av(f.ew), bv(f.ew), out(f.ew);
            vec::store(av.data(), f.ew, a), vec::store(bv.data(), f.ew, b);
            const auto run = [&](const char* name, vec::Fp2 op, auto&& hostOp) {
                unsigned gf = 0;
                vec::fp2(out.data(), av.data(), bv.data(), f.ew, f.ew, op, env, gf);
                _mm_setcsr(_mm_getcsr() & ~0x3fu);
                const U64 want = hostOp();
                const unsigned wf = mxcsrFlags(_mm_getcsr());
                const U64 got = vec::load(out.data(), f.ew);
                // (the host reports a denormal operand only when it is used; the library reports every one it reads: compare the rest, and the input flag when both agree it was read)
                if (got != want || (kHostFlagsReliable && (gf & ~vec::kInputDenormal) != (wf & ~vec::kInputDenormal))) report(name, f, env, a, b, 0, got, want, gf, wf);
            };
            if (!dbl) {
                run("add", vec::Fp2::Add, [&] { return fBits(_mm_cvtss_f32(_mm_add_ss(_mm_set_ss(asF(a)), _mm_set_ss(asF(b))))); });
                run("sub", vec::Fp2::Sub, [&] { return fBits(_mm_cvtss_f32(_mm_sub_ss(_mm_set_ss(asF(a)), _mm_set_ss(asF(b))))); });
                run("mul", vec::Fp2::Mul, [&] { return fBits(_mm_cvtss_f32(_mm_mul_ss(_mm_set_ss(asF(a)), _mm_set_ss(asF(b))))); });
                run("div", vec::Fp2::Div, [&] { return fBits(_mm_cvtss_f32(_mm_div_ss(_mm_set_ss(asF(a)), _mm_set_ss(asF(b))))); });
                run("min", vec::Fp2::Min, [&] { return fBits(_mm_cvtss_f32(_mm_min_ss(_mm_set_ss(asF(a)), _mm_set_ss(asF(b))))); });
                run("max", vec::Fp2::Max, [&] { return fBits(_mm_cvtss_f32(_mm_max_ss(_mm_set_ss(asF(a)), _mm_set_ss(asF(b))))); });
            } else {
                run("add", vec::Fp2::Add, [&] { return dBits(_mm_cvtsd_f64(_mm_add_sd(_mm_set_sd(asD(a)), _mm_set_sd(asD(b))))); });
                run("sub", vec::Fp2::Sub, [&] { return dBits(_mm_cvtsd_f64(_mm_sub_sd(_mm_set_sd(asD(a)), _mm_set_sd(asD(b))))); });
                run("mul", vec::Fp2::Mul, [&] { return dBits(_mm_cvtsd_f64(_mm_mul_sd(_mm_set_sd(asD(a)), _mm_set_sd(asD(b))))); });
                run("div", vec::Fp2::Div, [&] { return dBits(_mm_cvtsd_f64(_mm_div_sd(_mm_set_sd(asD(a)), _mm_set_sd(asD(b))))); });
                run("min", vec::Fp2::Min, [&] { return dBits(_mm_cvtsd_f64(_mm_min_sd(_mm_set_sd(asD(a)), _mm_set_sd(asD(b))))); });
                run("max", vec::Fp2::Max, [&] { return dBits(_mm_cvtsd_f64(_mm_max_sd(_mm_set_sd(asD(a)), _mm_set_sd(asD(b))))); });
            }
            {   // square root
                unsigned gf = 0;
                vec::fp1(out.data(), av.data(), f.ew, f.ew, vec::Fp1::Sqrt, env, gf);
                _mm_setcsr(_mm_getcsr() & ~0x3fu);
                const U64 want = dbl ? dBits(_mm_cvtsd_f64(_mm_sqrt_sd(_mm_setzero_pd(), _mm_set_sd(asD(a))))) : fBits(_mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(asF(a)))));
                const unsigned wf = mxcsrFlags(_mm_getcsr());
                const U64 got = vec::load(out.data(), f.ew);
                if (got != want || (kHostFlagsReliable && (gf & ~vec::kInputDenormal) != (wf & ~vec::kInputDenormal))) report("sqrt", f, env, a, 0, 0, got, want, gf, wf);
            }
            // compares: the predicate set of CMPPS / CMPPD
            const struct { vec::FpCmp cmp; bool signaling; int imm; } preds[] = {{vec::FpCmp::Eq, false, 0}, {vec::FpCmp::Lt, true, 1}, {vec::FpCmp::Le, true, 2},
                                                                                  {vec::FpCmp::Unordered, false, 3}, {vec::FpCmp::Ne, false, 4}, {vec::FpCmp::Ordered, false, 7}};
            for (const auto& p : preds) {
                unsigned gf = 0;
                vec::fpCompare(out.data(), av.data(), bv.data(), f.ew, f.ew, p.cmp, p.signaling, env, gf);
                _mm_setcsr(_mm_getcsr() & ~0x3fu);
                U64 want;
                if (!dbl) {
                    __m128 x = _mm_set_ss(asF(a)), y = _mm_set_ss(asF(b)), r;
                    switch (p.imm) {
                        case 0: r = _mm_cmpeq_ss(x, y); break;
                        case 1: r = _mm_cmplt_ss(x, y); break;
                        case 2: r = _mm_cmple_ss(x, y); break;
                        case 3: r = _mm_cmpunord_ss(x, y); break;
                        case 4: r = _mm_cmpneq_ss(x, y); break;
                        default: r = _mm_cmpord_ss(x, y); break;
                    }
                    want = fBits(_mm_cvtss_f32(r));
                } else {
                    __m128d x = _mm_set_sd(asD(a)), y = _mm_set_sd(asD(b)), r;
                    switch (p.imm) {
                        case 0: r = _mm_cmpeq_sd(x, y); break;
                        case 1: r = _mm_cmplt_sd(x, y); break;
                        case 2: r = _mm_cmple_sd(x, y); break;
                        case 3: r = _mm_cmpunord_sd(x, y); break;
                        case 4: r = _mm_cmpneq_sd(x, y); break;
                        default: r = _mm_cmpord_sd(x, y); break;
                    }
                    want = dBits(_mm_cvtsd_f64(r));
                }
                const unsigned wf = mxcsrFlags(_mm_getcsr());
                const U64 got = vec::load(out.data(), f.ew);
                if (got != want || (kHostFlagsReliable && (gf & ~vec::kInputDenormal) != (wf & ~vec::kInputDenormal))) report("cmp", f, env, a, b, static_cast<U64>(p.imm), got, want, gf, wf);
            }
            // conversions to 32 bit integers: CVTSS2SI-style rounding (nearest even) and truncation, indefinite 0x80000000; and the width changes
            {
                unsigned gf = 0;
                Bytes io(4);
                vec::FpEnv rne = env;
                vec::floatToInt(io.data(), av.data(), f.ew, f.ew, 4, vec::Sign::Signed, 0, vec::IntOverflow::X86Indefinite, rne, gf);
                _mm_setcsr(_mm_getcsr() & ~0x3fu);
                const U64 want = static_cast<std::uint32_t>(dbl ? _mm_cvtsd_si32(_mm_set_sd(asD(a))) : _mm_cvtss_si32(_mm_set_ss(asF(a))));
                const unsigned wf = mxcsrFlags(_mm_getcsr());
                if (vec::load(io.data(), 4) != want || (kHostFlagsReliable && (gf & ~vec::kInputDenormal) != (wf & ~vec::kInputDenormal))) report("cvt2si", f, env, a, 0, 0, vec::load(io.data(), 4), want, gf, wf);
                gf = 0;
                rne.round = vec::Round::TowardZero;
                vec::floatToInt(io.data(), av.data(), f.ew, f.ew, 4, vec::Sign::Signed, 0, vec::IntOverflow::X86Indefinite, rne, gf);
                _mm_setcsr(_mm_getcsr() & ~0x3fu);
                const U64 wantT = static_cast<std::uint32_t>(dbl ? _mm_cvttsd_si32(_mm_set_sd(asD(a))) : _mm_cvttss_si32(_mm_set_ss(asF(a))));
                const unsigned wfT = mxcsrFlags(_mm_getcsr());
                if (vec::load(io.data(), 4) != wantT || (kHostFlagsReliable && (gf & ~vec::kInputDenormal) != (wfT & ~vec::kInputDenormal))) report("cvtt2si", f, env, a, 0, 0, vec::load(io.data(), 4), wantT, gf, wfT);
                // integer to float
                const std::int32_t iv = static_cast<std::int32_t>(g_rng());
                Bytes iv4(4);
                vec::store(iv4.data(), 4, static_cast<std::uint32_t>(iv));
                gf = 0;
                vec::intToFloat(out.data(), iv4.data(), 4, 4, f.ew, vec::Sign::Signed, 0, env, gf);
                _mm_setcsr(_mm_getcsr() & ~0x3fu);
                const U64 wantI = dbl ? dBits(_mm_cvtsd_f64(_mm_cvtsi32_sd(_mm_setzero_pd(), iv))) : fBits(_mm_cvtss_f32(_mm_cvtsi32_ss(_mm_setzero_ps(), iv)));
                if (vec::load(out.data(), f.ew) != wantI) report("cvtsi2", f, env, static_cast<U64>(iv), 0, 0, vec::load(out.data(), f.ew), wantI, gf, 0);
                // single <-> double
                if (!dbl) {
                    Bytes dv(8);
                    gf = 0;
                    vec::floatConvert(dv.data(), av.data(), 4, 4, 8, env, gf);
                    _mm_setcsr(_mm_getcsr() & ~0x3fu);
                    const U64 w2 = dBits(_mm_cvtsd_f64(_mm_cvtss_sd(_mm_setzero_pd(), _mm_set_ss(asF(a)))));
                    if (vec::load(dv.data(), 8) != w2) report("cvtss2sd", f, env, a, 0, 0, vec::load(dv.data(), 8), w2, gf, 0);
                } else {
                    Bytes sv(4);
                    gf = 0;
                    vec::floatConvert(sv.data(), av.data(), 8, 8, 4, env, gf);
                    _mm_setcsr(_mm_getcsr() & ~0x3fu);
                    const U64 w2 = fBits(_mm_cvtss_f32(_mm_cvtsd_ss(_mm_setzero_ps(), _mm_set_sd(asD(a)))));
                    const unsigned wf2 = mxcsrFlags(_mm_getcsr());
                    if (vec::load(sv.data(), 4) != w2 || (kHostFlagsReliable && (gf & ~vec::kInputDenormal) != (wf2 & ~vec::kInputDenormal))) report("cvtsd2ss", f, env, a, 0, 0, vec::load(sv.data(), 4), w2, gf, wf2);
                }
            }
        }
    }
    _mm_setcsr(savedCsr);
    CB_CHECK_EQ(g_fails, 0u);
});
#endif

}  // namespace cb
