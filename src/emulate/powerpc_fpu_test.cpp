// emulate/powerpc_fpu_test.cpp -- the PowerPC floating-point unit (powerpc_fpu.hpp): the results and the status bits against the host's own instructions in every rounding mode
// (an independent check: SSE2 arithmetic under MXCSR), and the manual's tables for the cases the host does not share (NaN rules, the invalid-operation bits, enabled exceptions,
// the wrapped results, conversions, the FPSCR moves).
#include <immintrin.h>

#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <format>
#include <limits>
#include <random>
#include <string>

#include "core/testing.hpp"
#include "emulate/powerpc_fpu.hpp"

using namespace cb::emulate::powerpc;

namespace {

constexpr std::uint64_t kQuiet = 0x7FF8000000000000ull;
std::uint64_t bits_of(double x) { return std::bit_cast<std::uint64_t>(x); }
double dbl(std::uint64_t b) { return std::bit_cast<double>(b); }
bool nan_bits(std::uint64_t b) { return (b & 0x7FF0000000000000ull) == 0x7FF0000000000000ull && (b & 0x000FFFFFFFFFFFFFull) != 0; }

// ---- the host's arithmetic under a chosen rounding mode -----------------------------------------------------------------------------------------------------------------------

struct Host {
    std::uint64_t bits = 0;
    unsigned flags = 0;   // MXCSR: 1 invalid, 2 denormal operand, 4 zero divide, 8 overflow, 16 underflow, 32 inexact
};

unsigned mxcsr_rounding(unsigned ppc_rn) {   // PowerPC RN (nearest, zero, +inf, -inf) as MXCSR RC (nearest 0, down 1, up 2, zero 3)
    static const unsigned table[4] = {0, 3, 2, 1};
    return table[ppc_rn & 3];
}

enum class H : std::uint8_t { Add, Sub, Mul, Div, Sqrt, Frsp, Fcfid };

#if defined(_MSC_VER)
__declspec(noinline)
#else
__attribute__((noinline))
#endif
Host host_op(H op, bool single, std::uint64_t a, std::uint64_t b, unsigned ppc_rn) {
    volatile std::uint64_t va = a, vb = b;
    const unsigned saved = _mm_getcsr();
    _mm_setcsr((saved & ~0x7FFFu & ~0x6000u) | (mxcsr_rounding(ppc_rn) << 13) | 0x1F80u);   // all exceptions masked, flags clear
    Host h;
    if (single) {
        const __m128 x = _mm_set_ss(static_cast<float>(dbl(va))), y = _mm_set_ss(static_cast<float>(dbl(vb)));
        __m128 r = x;
        switch (op) {
            case H::Add: r = _mm_add_ss(x, y); break;
            case H::Sub: r = _mm_sub_ss(x, y); break;
            case H::Mul: r = _mm_mul_ss(x, y); break;
            case H::Div: r = _mm_div_ss(x, y); break;
            case H::Sqrt: r = _mm_sqrt_ss(y); break;
            default: break;
        }
        h.bits = bits_of(static_cast<double>(_mm_cvtss_f32(r)));
    } else {
        const __m128d x = _mm_set_sd(dbl(va)), y = _mm_set_sd(dbl(vb));
        __m128d r = x;
        switch (op) {
            case H::Add: r = _mm_add_sd(x, y); break;
            case H::Sub: r = _mm_sub_sd(x, y); break;
            case H::Mul: r = _mm_mul_sd(x, y); break;
            case H::Div: r = _mm_div_sd(x, y); break;
            case H::Sqrt: r = _mm_sqrt_sd(x, y); break;
            case H::Frsp: {
                const __m128 s = _mm_cvtsd_ss(_mm_setzero_ps(), y);
                r = _mm_cvtss_sd(x, s);
                break;
            }
            case H::Fcfid: r = _mm_cvtsi64_sd(x, static_cast<std::int64_t>(vb)); break;
        }
        h.bits = bits_of(_mm_cvtsd_f64(r));
    }
    h.flags = _mm_getcsr() & 0x3F;
    _mm_setcsr(saved);
    return h;
}

std::uint64_t random_double(std::mt19937_64& g, bool single) {
    std::uint64_t bits;
    switch (g() % 9) {
        case 0: bits = g(); break;
        case 1: bits = (g() & 0x800FFFFFFFFFFFFFull) | (static_cast<std::uint64_t>(1015 + g() % 20) << 52); break;   // near one
        case 2: bits = (g() & 0x800FFFFFFFFFFFFFull) | (static_cast<std::uint64_t>(2020 + g() % 27) << 52); break;   // huge
        case 3: bits = (g() & 0x800FFFFFFFFFFFFFull) | (static_cast<std::uint64_t>(g() % 12) << 52); break;           // tiny and denormal
        case 4: bits = (g() & 0x800FFFFFFFFFFFFFull) | (static_cast<std::uint64_t>(1 + g() % 2046) << 52); break;
        case 5: {
            static const double specials[] = {0.0, -0.0, 1.0, -1.0, 2.0, 0.5, 3.0, 1e300, -1e300, 4.9406564584124654e-324, 2.2250738585072014e-308, 1.7976931348623157e308,
                                              std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 16777216.0, 9007199254740993.0, 1.5, 2.5, -2.5, 0.1};
            bits = bits_of(specials[g() % (sizeof specials / sizeof *specials)]);
            break;
        }
        case 6: bits = bits_of(static_cast<double>(static_cast<std::int64_t>(g() % 20001) - 10000) / static_cast<double>(1 << (g() % 5))); break;
        case 7: bits = (g() & 0x800FFFFFFFFFFFFFull) | (static_cast<std::uint64_t>(1020 + g() % 8) << 52) | ((g() & 1) ? 0 : (g() & 0x000FFFFFFF000000ull)); break;   // short significands
        default: bits = (g() & 0x800FFFFFFFFFFFFFull) | (static_cast<std::uint64_t>(900 + g() % 250) << 52); break;
    }
    if (single) {   // a value a single holds (rounded, so a NaN stays one)
        const float f = static_cast<float>(dbl(bits));
        bits = bits_of(static_cast<double>(f));
    }
    return bits;
}

std::string describe(const char* what, std::uint64_t a, std::uint64_t b, unsigned rn, std::uint64_t got, std::uint64_t want, unsigned fpscr, unsigned flags) {
    return std::format("{} a={:016X} b={:016X} rn={} got={:016X} want={:016X} fpscr={:08X} host flags {:02X}", what, a, b, rn, got, want, fpscr, flags);
}

}  // namespace

// Add, subtract, multiply, divide, square root, frsp and fcfid: result, overflow, zero divide, invalid, inexact and (away from the tininess boundary where x86 and PowerPC differ) underflow,
// each in every rounding mode, for double and single precision, against the host's instructions.
CB_TEST("emulate.powerpc_fpu.arithmetic_matches_the_hosts_in_every_rounding_mode", [] {
    std::mt19937_64 g(20261124);
    struct Spec {
        const char* name;
        fpu::Op op;
        H host;
    };
    const Spec specs[] = {{"fadd", fpu::Op::Add, H::Add}, {"fsub", fpu::Op::Sub, H::Sub}, {"fmul", fpu::Op::Mul, H::Mul}, {"fdiv", fpu::Op::Div, H::Div},
                          {"fsqrt", fpu::Op::Sqrt, H::Sqrt}, {"frsp", fpu::Op::Frsp, H::Frsp}, {"fcfid", fpu::Op::Fcfid, H::Fcfid}};
    unsigned checked = 0, wrong = 0, inexact_seen = 0, overflow_seen = 0, underflow_seen = 0;
    for (const Spec& spec : specs)
        for (unsigned single = 0; single < 2; ++single) {
            if (single && (spec.host == H::Frsp || spec.host == H::Fcfid)) continue;
            for (unsigned rn = 0; rn < 4; ++rn)
                for (unsigned n = 0; n < 40000; ++n) {
                    std::uint64_t a = random_double(g, single), b = random_double(g, single);
                    if (spec.host == H::Fcfid) b = g() >> (g() % 64), b = (g() & 1) ? b : 0 - b;
                    const bool unordered = nan_bits(a) || nan_bits(b);
                    if (unordered) continue;   // (a NaN's payload and the default NaN's sign are the PowerPC's, tested below)
                    std::uint32_t fpscr = rn;
                    const unsigned operand_c = spec.op == fpu::Op::Mul ? 1 : 0;
                    const fpu::Result r = fpu::execute(fpscr, spec.op, single != 0, a, b, operand_c ? b : 0);
                    // (fmul multiplies FRA by FRC)
                    const Host h = host_op(spec.host, single != 0, a, b, rn);
                    ++checked;
                    const bool host_invalid = (h.flags & 1) != 0;
                    const bool ppc_invalid = (fpscr & fpu::kInvalidBits) != 0;
                    bool bad = false;
                    std::string why;
                    if (host_invalid != ppc_invalid) bad = true, why = "invalid";
                    else if (!host_invalid) {
                        if (r.bits != h.bits) bad = true, why = "result";
                        if (((h.flags & 32) != 0) != ((fpscr & fpu::kFI) != 0)) bad = true, why += " inexact";
                        if (((h.flags & 32) != 0) != ((fpscr & fpu::kXX) != 0)) bad = true, why += " XX";
                        if (((h.flags & 8) != 0) != ((fpscr & fpu::kOX) != 0)) bad = true, why += " overflow";
                        if (((h.flags & 4) != 0) != ((fpscr & fpu::kZX) != 0)) bad = true, why += " zerodiv";
                        const double mag = std::fabs(dbl(r.bits));
                        const double edge = single ? 1.1754943508222875e-38 : 2.2250738585072014e-308;
                        if (mag != edge && ((h.flags & 16) != 0) != ((fpscr & fpu::kUX) != 0)) bad = true, why += " underflow";
                        if (fpscr & fpu::kFI) {   // FR: the magnitude was incremented (the result is farther from zero than the toward-zero result)
                            const Host z = host_op(spec.host, single != 0, a, b, 1);
                            const bool incremented = std::fabs(dbl(h.bits)) > std::fabs(dbl(z.bits));
                            if (incremented != ((fpscr & fpu::kFR) != 0) && !(h.flags & 8)) bad = true, why += " FR";
                        } else if (fpscr & fpu::kFR) bad = true, why += " FR without FI";
                    }
                    inexact_seen += (h.flags & 32) != 0;
                    overflow_seen += (h.flags & 8) != 0;
                    underflow_seen += (h.flags & 16) != 0;
                    if (bad && ++wrong <= 10) std::printf("  %s%s: %s\n", spec.name, single ? "s" : "", describe(why.c_str(), a, b, rn, r.bits, h.bits, fpscr, h.flags).c_str());
                }
        }
    std::printf("powerpc_fpu host comparison: %u checked, %u wrong (%u inexact, %u overflow, %u underflow seen)\n", checked, wrong, inexact_seen, overflow_seen, underflow_seen);
    CB_CHECK(inexact_seen > 1000 && overflow_seen > 100 && underflow_seen > 100);
    CB_CHECK_EQ(wrong, 0u);
});

// The fused operations: in round-to-nearest the host's fused multiply-add (std::fma) is the exact reference; in the directed modes the four roundings of one exact value must
// bracket it (down <= nearest <= up, a step apart at most, toward zero the one nearer zero) and agree on whether it is exact.
CB_TEST("emulate.powerpc_fpu.fused_multiply_add_rounds_once", [] {
    std::mt19937_64 g(777);
    unsigned wrong = 0, checked = 0;
    for (unsigned n = 0; n < 200000; ++n) {
        const std::uint64_t a = random_double(g, false), b = random_double(g, false), c = random_double(g, false);
        if (nan_bits(a) || nan_bits(b) || nan_bits(c)) continue;
        std::uint32_t s[4] = {0, 1, 2, 3};
        std::uint64_t r[4];
        for (unsigned rn = 0; rn < 4; ++rn) r[rn] = fpu::execute(s[rn], fpu::Op::Madd, false, a, b, c).bits;
        const double expect = std::fma(dbl(a), dbl(c), dbl(b));
        ++checked;
        bool bad = false;
        if (!std::isnan(expect)) {
            // (a result that is a NaN is an invalid operation: not compared here)
            if (r[0] != bits_of(expect) && !(std::fabs(expect) < 4.0e-308)) bad = true;   // (below the normal range std::fma may differ in how a library rounds a denormal)
            const double dn = dbl(r[3]), up = dbl(r[2]), tz = dbl(r[1]), ne = dbl(r[0]);
            if (!(dn <= ne && ne <= up) || (dn != up && std::nextafter(dn, INFINITY) != up)) bad = true;
            if (tz != (std::fabs(dn) < std::fabs(up) ? dn : up) && dn != up) bad = true;
            if ((dn == up) != ((s[2] & fpu::kFI) == 0) && !std::isinf(up) && !std::isinf(dn)) bad = true;
        }
        if (bad && ++wrong <= 10) std::printf("  fmadd a=%016llX b=%016llX c=%016llX: nearest %016llX expect %016llX\n", static_cast<unsigned long long>(a), static_cast<unsigned long long>(b), static_cast<unsigned long long>(c),
                                              static_cast<unsigned long long>(r[0]), static_cast<unsigned long long>(bits_of(expect)));
    }
    std::printf("powerpc_fpu fused: %u checked, %u wrong\n", checked, wrong);
    CB_CHECK_EQ(wrong, 0u);
});

// The invalid operations, the NaN rules and the summary bits (the manual's tables).
CB_TEST("emulate.powerpc_fpu.invalid_operations_and_nans", [] {
    const std::uint64_t inf = 0x7FF0000000000000ull, ninf = 0xFFF0000000000000ull, snan = 0x7FF0000000000001ull, qnan = 0x7FF8000000000321ull, one = 0x3FF0000000000000ull, zero = 0;
    const auto run = [](fpu::Op op, std::uint64_t a, std::uint64_t b, std::uint64_t c, std::uint32_t& fpscr, bool single = false) { return fpu::execute(fpscr, op, single, a, b, c); };
    {   // inf - inf: VXISI, the default NaN; FX, VX set, FEX not (VE clear)
        std::uint32_t s = 0;
        const auto r = run(fpu::Op::Sub, inf, inf, 0, s);
        CB_CHECK_EQ(r.bits, kQuiet);
        CB_CHECK_EQ(s & (fpu::kVXISI | fpu::kFX | fpu::kVX | fpu::kFEX), fpu::kVXISI | fpu::kFX | fpu::kVX);
        CB_CHECK_EQ((s & fpu::kFPRF) >> 12, 0x11u);
    }
    {   // inf / inf, 0 / 0, inf * 0, sqrt(-1), -inf sqrt
        std::uint32_t s = 0;
        run(fpu::Op::Div, inf, ninf, 0, s);
        CB_CHECK(s & fpu::kVXIDI);
        run(fpu::Op::Div, zero, zero, 0, s);
        CB_CHECK(s & fpu::kVXZDZ);
        run(fpu::Op::Mul, inf, 0, zero, s);
        CB_CHECK(s & fpu::kVXIMZ);
        run(fpu::Op::Sqrt, 0, 0xBFF0000000000000ull, 0, s);
        CB_CHECK(s & fpu::kVXSQRT);
        run(fpu::Op::Rsqrte, 0, 0xBFF0000000000000ull, 0, s);
        CB_CHECK(s & fpu::kVXSQRT);
        std::uint32_t t = 0;
        run(fpu::Op::Madd, inf, ninf, one, t);   // inf * 1 + (-inf)
        CB_CHECK(t & fpu::kVXISI);
        std::uint32_t u = 0;
        run(fpu::Op::Madd, inf, one, zero, u);   // fmadd FRA=inf FRB=1 FRC=0
        CB_CHECK(u & fpu::kVXIMZ);
    }
    {   // a signaling NaN: VXSNAN, the result its quieted self; a quiet NaN raises nothing; the first NaN in order FRA, FRB, FRC wins
        std::uint32_t s = 0;
        auto r = run(fpu::Op::Add, snan, one, 0, s);
        CB_CHECK_EQ(r.bits, snan | (1ull << 51));
        CB_CHECK(s & fpu::kVXSNAN);
        std::uint32_t t = 0;
        r = run(fpu::Op::Add, one, qnan, 0, t);
        CB_CHECK_EQ(r.bits, qnan);
        CB_CHECK_EQ(t & fpu::kInvalidBits, 0u);
        std::uint32_t u = 0;
        r = run(fpu::Op::Madd, one, qnan, 0x7FF8000000000777ull, u);
        CB_CHECK_EQ(r.bits, qnan);   // FRB before FRC
        std::uint32_t v = 0;
        r = run(fpu::Op::Mul, one, qnan, 0x7FF8000000000777ull, v);
        CB_CHECK_EQ(r.bits, 0x7FF8000000000777ull);   // fmul looks at FRA and FRC only
        std::uint32_t w = 0;
        r = run(fpu::Op::Nmadd, qnan, one, one, w);
        CB_CHECK_EQ(r.bits, qnan);   // a NaN is not negated
    }
    {   // x / 0: ZX and an infinity of the right sign; with ZE the target is left alone
        std::uint32_t s = 0;
        auto r = run(fpu::Op::Div, one, 0x8000000000000000ull, 0, s);
        CB_CHECK_EQ(r.bits, ninf);
        CB_CHECK(s & fpu::kZX);
        CB_CHECK(r.written);
        std::uint32_t t = fpu::kZE;
        r = run(fpu::Op::Div, one, zero, 0, t);
        CB_CHECK(!r.written);
        CB_CHECK(t & fpu::kFEX);
        std::uint32_t u = fpu::kVE;
        r = run(fpu::Op::Add, snan, one, 0, u);
        CB_CHECK(!r.written);
        CB_CHECK((u & (fpu::kVX | fpu::kFEX | fpu::kFX)) == (fpu::kVX | fpu::kFEX | fpu::kFX));
        CB_CHECK_EQ(u & fpu::kFPRF, 0u);   // FPRF is left alone by a stopped operation
    }
    {   // FX rises only on a transition; clearing a sticky bit and repeating raises it again
        std::uint32_t s = 0;
        run(fpu::Op::Div, one, zero, 0, s);
        CB_CHECK(s & fpu::kFX);
        s &= ~fpu::kFX;
        run(fpu::Op::Div, one, zero, 0, s);
        CB_CHECK_EQ(s & fpu::kFX, 0u);   // ZX was already set
    }
});

// Overflow, underflow and the rounding modes' results, the wrapped results of enabled exceptions, FPRF classes.
CB_TEST("emulate.powerpc_fpu.overflow_underflow_and_classes", [] {
    const std::uint64_t max = 0x7FEFFFFFFFFFFFFFull, two = 0x4000000000000000ull, one = 0x3FF0000000000000ull, tiny = 0x0010000000000000ull, inf = 0x7FF0000000000000ull;
    // max * 2 in each mode: nearest and +inf give infinity, toward zero and -inf the greatest finite number (a negative operand mirrors it)
    const std::uint64_t expect_pos[4] = {inf, max, inf, max};
    for (unsigned rn = 0; rn < 4; ++rn) {
        std::uint32_t s = rn;
        auto r = fpu::execute(s, fpu::Op::Mul, false, max, 0, two);
        CB_CHECK_EQ(r.bits, expect_pos[rn]);
        CB_CHECK(s & fpu::kOX);
        CB_CHECK(s & fpu::kXX);
        CB_CHECK(s & fpu::kFI);
        CB_CHECK_EQ((s & fpu::kFR) != 0, expect_pos[rn] == inf);
        std::uint32_t t = rn;
        r = fpu::execute(t, fpu::Op::Mul, false, max | (1ull << 63), 0, two);
        const std::uint64_t expect_neg[4] = {inf | (1ull << 63), max | (1ull << 63), max | (1ull << 63), inf | (1ull << 63)};
        CB_CHECK_EQ(r.bits, expect_neg[rn]);
    }
    {   // overflow enabled: the result is rounded with the exponent wrapped by 1536 (double) or 192 (single)
        std::uint32_t s = fpu::kOE;
        const auto r = fpu::execute(s, fpu::Op::Mul, false, max, 0, two);
        CB_CHECK(r.written);
        CB_CHECK_EQ(r.bits, 0x1FFFFFFFFFFFFFFFull);   // max*2 = (2-2^-52)*2^1024, exactly, then the exponent less 1536: (2-2^-52)*2^-512
        CB_CHECK((s & fpu::kFEX) != 0 && (s & fpu::kOX) != 0);
        std::uint32_t t = fpu::kOE;
        const auto q = fpu::execute(t, fpu::Op::Mul, true, 0x47EFFFFFE0000000ull, 0, bits_of(2.0));   // single max * 2
        CB_CHECK_EQ(q.bits, bits_of((2.0 - std::ldexp(1.0, -23)) * std::ldexp(1.0, -64)));
    }
    {   // underflow: tiny * 0.5 is a denormal, exact: no UX; tiny * 0.25 + rounding: UX; UE set: UX always, and the exponent wrapped by +1536
        std::uint32_t s = 0;
        auto r = fpu::execute(s, fpu::Op::Mul, false, tiny, 0, bits_of(0.5));
        CB_CHECK_EQ(r.bits, 0x0008000000000000ull);
        CB_CHECK_EQ(s & fpu::kUX, 0u);
        CB_CHECK_EQ((s & fpu::kFPRF) >> 12, 0x14u);   // positive denormal
        std::uint32_t t = 0;
        r = fpu::execute(t, fpu::Op::Mul, false, tiny | 1, 0, bits_of(0.5));   // (2^-1022 + 1ulp) / 2 needs a bit more than a denormal has
        CB_CHECK(t & fpu::kUX);
        CB_CHECK(t & fpu::kFI);
        std::uint32_t u = fpu::kUE;
        r = fpu::execute(u, fpu::Op::Mul, false, tiny, 0, bits_of(0.5));
        CB_CHECK(u & fpu::kUX);
        CB_CHECK_EQ(r.bits, bits_of(std::ldexp(1.0, -1023 + 1536)));
    }
    {   // exact zero sum: +0, -0 only in round toward -infinity; x + x of opposite zeros likewise
        for (unsigned rn = 0; rn < 4; ++rn) {
            std::uint32_t s = rn;
            const auto r = fpu::execute(s, fpu::Op::Sub, false, one, one, 0);
            CB_CHECK_EQ(r.bits, rn == 3 ? 0x8000000000000000ull : 0ull);
            std::uint32_t t = rn;
            const auto z = fpu::execute(t, fpu::Op::Add, false, 0x8000000000000000ull, 0, 0);
            CB_CHECK_EQ(z.bits, rn == 3 ? 0x8000000000000000ull : 0ull);   // -0 + +0
            CB_CHECK_EQ((t & fpu::kFPRF) >> 12, rn == 3 ? 0x12u : 0x02u);
        }
    }
    {   // FPRF classes
        const struct {
            std::uint64_t a;
            unsigned cls;
        } cases[] = {{inf, 0x05}, {inf | (1ull << 63), 0x09}, {one, 0x04}, {one | (1ull << 63), 0x08}, {0x0008000000000000ull, 0x14}, {0x8008000000000000ull, 0x18}, {0, 0x02}, {1ull << 63, 0x12}};
        for (const auto& c : cases) {
            std::uint32_t s = 0;
            fpu::execute(s, fpu::Op::Add, false, c.a, 0, 0);   // x + 0
            if (c.a == (1ull << 63)) continue;                  // (-0 + +0 is +0)
            CB_CHECK_EQ((s & fpu::kFPRF) >> 12, c.cls);
        }
    }
    {   // frsp in each mode: 1 + 2^-30 to single
        const std::uint64_t x = bits_of(1.0 + std::ldexp(1.0, -30));
        for (unsigned rn = 0; rn < 4; ++rn) {
            std::uint32_t s = rn;
            const auto r = fpu::execute(s, fpu::Op::Frsp, true, 0, x, 0);
            CB_CHECK_EQ(r.bits, rn == 2 ? bits_of(1.0 + std::ldexp(1.0, -23)) : bits_of(1.0));
            CB_CHECK_EQ((s & fpu::kFR) != 0, rn == 2);
            CB_CHECK(s & fpu::kFI);
        }
    }
});

// Conversions to integer by RN, saturation and VXCVI; fcfid.
CB_TEST("emulate.powerpc_fpu.integer_conversions", [] {
    const auto word = [](std::uint32_t& s, double x, bool zero_mode, unsigned bits) {
        return fpu::execute(s, bits == 32 ? (zero_mode ? fpu::Op::Fctiwz : fpu::Op::Fctiw) : (zero_mode ? fpu::Op::Fctidz : fpu::Op::Fctid), false, 0, bits_of(x), 0);
    };
    struct Row {
        double x;
        std::int64_t by_mode[4];   // nearest, zero, +inf, -inf
    };
    const Row rows[] = {{2.5, {2, 2, 3, 2}}, {3.5, {4, 3, 4, 3}}, {-2.5, {-2, -2, -2, -3}}, {-3.5, {-4, -3, -3, -4}}, {0.4, {0, 0, 1, 0}}, {-0.4, {0, 0, 0, -1}}, {7.0, {7, 7, 7, 7}}, {2147483646.5, {2147483646, 2147483646, 2147483647, 2147483646}}};
    for (const Row& row : rows)
        for (unsigned rn = 0; rn < 4; ++rn) {
            std::uint32_t s = rn;
            const auto r = word(s, row.x, false, 32);
            CB_CHECK_EQ(static_cast<std::int32_t>(r.bits), static_cast<std::int32_t>(row.by_mode[rn]));
            CB_CHECK_EQ(s & fpu::kVXCVI, 0u);
            CB_CHECK_EQ((s & fpu::kFI) != 0, row.x != std::trunc(row.x));
            std::uint32_t z = rn;
            const auto q = word(z, row.x, true, 32);
            CB_CHECK_EQ(static_cast<std::int32_t>(q.bits), static_cast<std::int32_t>(row.by_mode[1]));
        }
    {   // out of range, infinities and NaNs: VXCVI and the saturated value; 2147483647.5 rounds to 2^31, out of range
        std::uint32_t s = 0;
        auto r = word(s, 3e9, false, 32);
        CB_CHECK_EQ(static_cast<std::uint32_t>(r.bits), 0x7FFFFFFFu);
        CB_CHECK(s & fpu::kVXCVI);
        std::uint32_t t = 0;
        r = word(t, -3e9, false, 32);
        CB_CHECK_EQ(static_cast<std::uint32_t>(r.bits), 0x80000000u);
        std::uint32_t u = 0;
        r = word(u, 2147483647.5, false, 32);
        CB_CHECK(u & fpu::kVXCVI);
        std::uint32_t v = 0;
        r = fpu::execute(v, fpu::Op::Fctiw, false, 0, kQuiet, 0);
        CB_CHECK_EQ(static_cast<std::uint32_t>(r.bits), 0x80000000u);
        CB_CHECK(v & fpu::kVXCVI);
        std::uint32_t w = 0;
        fpu::execute(w, fpu::Op::Fctid, false, 0, 0x7FF0000000000001ull, 0);
        CB_CHECK((w & (fpu::kVXCVI | fpu::kVXSNAN)) == (fpu::kVXCVI | fpu::kVXSNAN));
        std::uint32_t x = fpu::kVE;
        r = word(x, 3e9, false, 32);
        CB_CHECK(!r.written);
        std::uint32_t y = 0;
        r = word(y, 9.3e18, false, 64);
        CB_CHECK_EQ(r.bits, 0x7FFFFFFFFFFFFFFFull);
        r = word(y, -9.3e18, false, 64);
        CB_CHECK_EQ(r.bits, 0x8000000000000000ull);
        r = word(y, -9223372036854775808.0, false, 64);
        CB_CHECK_EQ(r.bits, 0x8000000000000000ull);
    }
    {   // fcfid: 2^53 + 1 is not a double; each mode picks a neighbor
        const std::int64_t v = (std::int64_t{1} << 53) + 1;
        const double expect[4] = {9007199254740992.0, 9007199254740992.0, 9007199254740994.0, 9007199254740992.0};
        for (unsigned rn = 0; rn < 4; ++rn) {
            std::uint32_t s = rn;
            const auto r = fpu::execute(s, fpu::Op::Fcfid, false, 0, static_cast<std::uint64_t>(v), 0);
            CB_CHECK_EQ(r.bits, bits_of(expect[rn]));
            CB_CHECK(s & fpu::kFI);
        }
    }
});

// Compares and the status register moves.
CB_TEST("emulate.powerpc_fpu.compares_and_status_register_moves", [] {
    const std::uint64_t one = 0x3FF0000000000000ull, two = 0x4000000000000000ull, qnan = 0x7FF8000000000000ull, snan = 0x7FF0000000000001ull;
    std::uint32_t s = 0;
    CB_CHECK_EQ(fpu::compare(s, one, two, false), 8u);
    CB_CHECK_EQ((s & fpu::kFPCC) >> 12, 8u);
    CB_CHECK_EQ(fpu::compare(s, two, one, false), 4u);
    CB_CHECK_EQ(fpu::compare(s, one, one, false), 2u);
    CB_CHECK_EQ(fpu::compare(s, 0, 1ull << 63, false), 2u);   // +0 == -0
    CB_CHECK_EQ(s & fpu::kInvalidBits, 0u);
    CB_CHECK_EQ(fpu::compare(s, qnan, one, false), 1u);   // fcmpu with a quiet NaN raises nothing
    CB_CHECK_EQ(s & fpu::kInvalidBits, 0u);
    CB_CHECK_EQ(fpu::compare(s, qnan, one, true), 1u);    // fcmpo: VXVC
    CB_CHECK_EQ(s & fpu::kInvalidBits, fpu::kVXVC);
    std::uint32_t t = 0;
    fpu::compare(t, snan, one, false);
    CB_CHECK_EQ(t & fpu::kInvalidBits, fpu::kVXSNAN);
    std::uint32_t u = 0;
    fpu::compare(u, snan, one, true);
    CB_CHECK_EQ(u & fpu::kInvalidBits, fpu::kVXSNAN | fpu::kVXVC);
    std::uint32_t v = fpu::kVE;
    fpu::compare(v, snan, one, true);
    CB_CHECK_EQ(v & fpu::kInvalidBits, fpu::kVXSNAN);   // (with VE set only VXSNAN)
    // mtfsf: fields chosen by FM; FEX and VX are computed, not written; mffs reads the register back
    std::uint32_t f = 0;
    fpu::move_to_fields(f, 0xFF, 0xFFFFFFFFull);
    CB_CHECK_EQ(f, 0xFFFFFFFFu & ~(1u << 11) & ~0u);
    CB_CHECK((f & fpu::kFEX) != 0 && (f & fpu::kVX) != 0);   // (computed: sources set with VE set)
    std::uint32_t g = 0;
    fpu::move_to_fields(g, 0x01, 0x0000000Dull);   // field 7: XE=1, NI=1? (bits: XE NI RN1 RN0 = 1101)
    CB_CHECK_EQ(g, 0xDu);
    fpu::move_to_fields(g, 0x80, 0xFFFFFFFFull);   // field 0: FX FEX VX OX: FEX and VX are not taken from the source
    CB_CHECK((g & 0xF0000000u) == (fpu::kFX | fpu::kOX | fpu::kFEX | fpu::kVX) || (g & 0xF0000000u) == (fpu::kFX | fpu::kOX));
    CB_CHECK_EQ(fpu::move_from(0x12345678u), 0xFFF8000012345678ull);
    // mtfsb1 raises FX on a transition; mtfsb0 clears; bits 1 and 2 are not directly writable
    std::uint32_t h = 0;
    fpu::set_bit(h, 6, true);   // ZX? bit 5 is ZX; bit 6 is XX
    CB_CHECK((h & fpu::kXX) != 0 && (h & fpu::kFX) != 0);
    fpu::set_bit(h, 6, false);
    CB_CHECK_EQ(h & fpu::kXX, 0u);
    std::uint32_t k = 0;
    fpu::set_bit(k, 1, true);
    CB_CHECK_EQ(k, 0u);
    fpu::set_bit(k, 24, true);   // VE
    fpu::set_bit(k, 7, true);    // VXSNAN: FX, VX, FEX (VE set)
    CB_CHECK((k & (fpu::kFX | fpu::kVX | fpu::kFEX | fpu::kVXSNAN)) == (fpu::kFX | fpu::kVX | fpu::kFEX | fpu::kVXSNAN));
    // mcrfs: the field is returned and its exception bits cleared (FEX and VX follow)
    const unsigned field0 = fpu::move_to_cr_from_field(k, 0);
    CB_CHECK_EQ(field0, 0xEu);   // FX FEX VX (OX clear)
    CB_CHECK_EQ(k & fpu::kFX, 0u);
    CB_CHECK((k & fpu::kVXSNAN) != 0);   // field 1 untouched
    const unsigned field1 = fpu::move_to_cr_from_field(k, 1);
    CB_CHECK_EQ(field1, 1u);   // VXSNAN
    CB_CHECK_EQ(k & fpu::kInvalidBits, 0u);
    CB_CHECK_EQ(k & (fpu::kVX | fpu::kFEX), 0u);
    // mtfsfi
    std::uint32_t m = 0;
    fpu::move_to_field_immediate(m, 7, 3);   // RN = 3
    CB_CHECK_EQ(m, 3u);
    fpu::move_to_field_immediate(m, 1, 0xF);
    CB_CHECK_EQ(m & 0x0F000000u, 0x0F000000u);
});
