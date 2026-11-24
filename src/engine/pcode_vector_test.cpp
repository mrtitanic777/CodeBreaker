// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_vector.{hpp,cpp}: every lane operation, at every lane width it takes, against a separate model. The integer operations are checked against exact arithmetic
// on a small big-integer type written here (each model is the manual's formula applied to exact numbers, then cut to the lane); the float operations against an exactly
// rounded rational model (a big integer quotient, remainder and the rounding rule applied by comparison, never the library's shifts) in all five rounding modes, with NaNs,
// infinities, zeros and denormals; the x86 NaN, minimum, compare and convert rules additionally against the host's own SSE instructions (guarded to x86 builds).
// What the host cannot check -- the ARM and PowerPC NaN, flush and flag rules -- is checked against the model only: it is written from the manuals, not from hardware.
#include <algorithm>
#include <array>
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
#include "engine/pcode_vector_model.hpp"
#include "engine/pcode_vector.hpp"

namespace cb {
namespace {

namespace vec = engine::vec;
using vec::Byte;
using Bytes = std::vector<Byte>;
using U64 = std::uint64_t;
using I64 = std::int64_t;

using namespace vecmodel;

// ---- generators
#ifndef CB_VEC_SEED
#define CB_VEC_SEED 0x5eed5eed   // (a different seed can be given on the compiler's command line to run the same checks on other operands)
#endif
std::mt19937_64 g_rng(CB_VEC_SEED);
U64 edgeLane(unsigned ew) {
    const unsigned w = 8 * ew;
    const U64 mask = vec::laneMask(ew);
    switch (g_rng() % 14) {
        case 0: return 0;
        case 1: return 1;
        case 2: return mask;
        case 3: return mask - 1;
        case 4: return U64{1} << (w - 1);
        case 5: return (U64{1} << (w - 1)) - 1;
        case 6: return (U64{1} << (w - 1)) + 1;
        case 7: return U64{1} << (g_rng() % w);
        case 8: return (U64{1} << (g_rng() % w)) - 1;
        case 9: return (g_rng() & 0xff) & mask;
        case 10: return (~U64{0} << (g_rng() % w)) & mask;
        default: return g_rng() & mask;
    }
}
Bytes randomVector(std::size_t n, unsigned ew) {
    Bytes v(n);
    for (std::size_t i = 0; i < n; i += ew) vec::store(v.data() + i, ew, edgeLane(ew));
    return v;
}
const unsigned kWidths[] = {1, 2, 4, 8};
const std::size_t kSizes[] = {8, 16, 32, 64, 256};
unsigned g_fails = 0;
void fail(const char* what, unsigned ew, std::size_t n, const Bytes& got, const Bytes& want, std::size_t at) {
    if (++g_fails <= 12) std::printf("  %s ew=%u n=%zu lane at byte %zu: got %llx want %llx\n", what, ew, n, at, static_cast<unsigned long long>(vec::load(got.data() + at, ew)),
                                      static_cast<unsigned long long>(vec::load(want.data() + at, ew)));
}

// Run `fn` (the library) and `model` (per lane, on Big values) on random vectors at every width and size and compare lane by lane.
using Model2 = Big (*)(const Big& a, const Big& b, unsigned ew);
template <class Lib, class Model>
void check2(const char* name, const Lib& lib, const Model& model, bool aSigned, bool bSigned, bool saturating, unsigned minEw = 1, unsigned maxEw = 8) {
    for (unsigned ew : kWidths) {
        if (ew < minEw || ew > maxEw) continue;
        for (std::size_t n : kSizes)
            for (int iter = 0; iter < 60; ++iter) {
                const Bytes a = randomVector(n, ew), b = randomVector(n, ew);
                Bytes got(n, 0), want(n, 0);
                lib(got.data(), a.data(), b.data(), n, ew);
                for (std::size_t i = 0; i < n; i += ew) {
                    const Big r = model(laneBig(vec::load(a.data() + i, ew), ew, aSigned), laneBig(vec::load(b.data() + i, ew), ew, bSigned), ew);
                    vec::store(want.data() + i, ew, lowBits(saturating ? r : r, 8 * ew));
                }
                if (got != want) {
                    for (std::size_t i = 0; i < n; i += ew)
                        if (std::memcmp(got.data() + i, want.data() + i, ew) != 0) {
                            fail(name, ew, n, got, want, i);
                            break;
                        }
                }
            }
    }
}

}  // namespace

CB_TEST("engine.pcode_vector.integer_lane_arithmetic_matches_exact_model", [] {
    g_fails = 0;
    using vec::Sign;
    const auto S = [](unsigned ew) { return 8 * ew; };
    (void)S;
    check2("add", [](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::add(o, a, b, n, ew); }, [](const Big& a, const Big& b, unsigned) { return a + b; }, false, false, false);
    check2("sub", [](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::sub(o, a, b, n, ew); }, [](const Big& a, const Big& b, unsigned) { return a - b; }, false, false, false);
    check2("mul", [](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::mul(o, a, b, n, ew); }, [](const Big& a, const Big& b, unsigned) { return a * b; }, false, false, false);
    for (const bool sg : {false, true}) {
        const Sign sign = sg ? Sign::Signed : Sign::Unsigned;
        check2(sg ? "mulHigh.s" : "mulHigh.u", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::mulHigh(o, a, b, n, ew, sign); },
               [](const Big& a, const Big& b, unsigned ew) { return shrFloor(a * b, 8 * ew); }, sg, sg, false);
        check2(sg ? "addSat.s" : "addSat.u", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::addSat(o, a, b, n, ew, sign); },
               [sg](const Big& a, const Big& b, unsigned ew) { return clamp(a + b, 8 * ew, sg); }, sg, sg, true);
        check2(sg ? "subSat.s" : "subSat.u", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::subSat(o, a, b, n, ew, sign); },
               [sg](const Big& a, const Big& b, unsigned ew) { return clamp(a - b, 8 * ew, sg); }, sg, sg, true);
        check2(sg ? "min.s" : "min.u", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::min(o, a, b, n, ew, sign); },
               [](const Big& a, const Big& b, unsigned) { return cmp(a, b) <= 0 ? a : b; }, sg, sg, false);
        check2(sg ? "max.s" : "max.u", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::max(o, a, b, n, ew, sign); },
               [](const Big& a, const Big& b, unsigned) { return cmp(a, b) >= 0 ? a : b; }, sg, sg, false);
        check2(sg ? "absDiff.s" : "absDiff.u", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::absDiff(o, a, b, n, ew, sign); },
               [](const Big& a, const Big& b, unsigned) { Big d = a - b; if (d.neg) d = -d; return d; }, sg, sg, false);
        for (const bool rnd : {false, true}) {
            check2("halvingAdd", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::halvingAdd(o, a, b, n, ew, sign, rnd); },
                   [rnd](const Big& a, const Big& b, unsigned) { return shrFloor(a + b + (rnd ? bigU(1) : Big{}), 1); }, sg, sg, false);
        }
        check2("halvingSub", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::halvingSub(o, a, b, n, ew, sign); },
               [](const Big& a, const Big& b, unsigned) { return shrFloor(a - b, 1); }, sg, sg, false);
    }
    // unsigned plus signed and the reverse (USQADD, SUQADD)
    for (const bool accSigned : {false, true}) {
        const Sign accSign = accSigned ? Sign::Signed : Sign::Unsigned;
        check2("addSatMixed", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::addSatMixed(o, a, b, n, ew, accSign); },
               [accSigned](const Big& a, const Big& b, unsigned ew) { return clamp(a + b, 8 * ew, accSigned); }, accSigned, !accSigned, true);
    }
    // doubling multiply high (SQDMULH, SQRDMULH), PMULHRSW
    for (const bool rnd : {false, true}) {
        check2("mulDoublingHigh", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::mulDoublingHigh(o, a, b, n, ew, rnd); },
               [rnd](const Big& a, const Big& b, unsigned ew) {
                   const unsigned w = 8 * ew;
                   return clamp(shrFloor(shlBig(a * b, 1) + (rnd ? shlBig(bigU(1), w - 1) : Big{}), w), w, true);
               },
               true, true, true);
    }
    check2("mulHighRoundHalfUp", [](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::mulHighRoundHalfUp(o, a, b, n, ew); },
           [](const Big& a, const Big& b, unsigned ew) { return shrFloor(shrFloor(a * b, 8 * ew - 2) + bigU(1), 1); }, true, true, false);
    // compare
    for (const bool sg : {false, true})
        for (int c = 0; c < 6; ++c) {
            const vec::Cmp cmpOp = static_cast<vec::Cmp>(c);
            check2("compare", [&](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { vec::compare(o, a, b, n, ew, cmpOp, sg ? Sign::Signed : Sign::Unsigned); },
                   [c](const Big& a, const Big& b, unsigned) {
                       const int k = cmp(a, b);
                       const bool t = c == 0 ? k == 0 : c == 1 ? k != 0 : c == 2 ? k < 0 : c == 3 ? k <= 0 : c == 4 ? k > 0 : k >= 0;
                       return t ? bigS(-1) : Big{};
                   },
                   sg, sg, false);
        }
    // SQRDMLAH / SQRDMLSH: acc +/- the rounded doubling high product, saturated, on exact integers
    for (unsigned ew : {1u, 2u, 4u, 8u})
        for (std::size_t n : kSizes)
            for (int iter = 0; iter < 40; ++iter) {
                const Bytes acc = randomVector(n, ew), a = randomVector(n, ew), b = randomVector(n, ew);
                for (const bool subtract : {false, true}) {
                    Bytes got(n), want(n);
                    vec::mulDoublingHighAcc(got.data(), acc.data(), a.data(), b.data(), n, ew, subtract);
                    const unsigned w = 8 * ew;
                    for (std::size_t i = 0; i < n; i += ew) {
                        const Big prod = shlBig(laneBig(vec::load(a.data() + i, ew), ew, true) * laneBig(vec::load(b.data() + i, ew), ew, true), 1);
                        const Big sum = shlBig(laneBig(vec::load(acc.data() + i, ew), ew, true), w) + (subtract ? -prod : prod) + shlBig(bigU(1), w - 1);
                        vec::store(want.data() + i, ew, lowBits(clamp(shrFloor(sum, w), w, true), w));
                    }
                    if (got != want) fail("mulDoublingHighAcc", ew, n, got, want, 0);
                }
            }
    CB_CHECK_EQ(g_fails, 0u);
});

CB_TEST("engine.pcode_vector.unary_and_bit_operations_match_exact_model", [] {
    g_fails = 0;
    for (unsigned ew : kWidths)
        for (std::size_t n : kSizes)
            for (int iter = 0; iter < 60; ++iter) {
                const unsigned w = 8 * ew;
                const Bytes a = randomVector(n, ew);
                const auto each = [&](const char* name, auto&& lib, auto&& model) {
                    Bytes got(n, 0), want(n, 0);
                    lib(got.data());
                    for (std::size_t i = 0; i < n; i += ew) vec::store(want.data() + i, ew, model(vec::load(a.data() + i, ew)));
                    for (std::size_t i = 0; i < n; i += ew)
                        if (std::memcmp(got.data() + i, want.data() + i, ew) != 0) {
                            fail(name, ew, n, got, want, i);
                            break;
                        }
                };
                for (const bool sat : {false, true}) {
                    each("neg", [&](Byte* o) { vec::neg(o, a.data(), n, ew, sat); }, [&](U64 v) {
                        const Big r = -laneBig(v, ew, true);
                        return lowBits(sat ? clamp(r, w, true) : r, w);
                    });
                    each("abs", [&](Byte* o) { vec::abs(o, a.data(), n, ew, sat); }, [&](U64 v) {
                        Big r = laneBig(v, ew, true);
                        if (r.neg) r = -r;
                        return lowBits(sat ? clamp(r, w, true) : r, w);
                    });
                }
                each("popcount", [&](Byte* o) { vec::bitOp(o, a.data(), n, ew, vec::BitOp::Popcount); }, [&](U64 v) {
                    U64 c = 0;
                    for (unsigned k = 0; k < w; ++k) c += (v >> k) & 1;
                    return c;
                });
                each("clz", [&](Byte* o) { vec::bitOp(o, a.data(), n, ew, vec::BitOp::CountLeadingZeros); }, [&](U64 v) {
                    U64 c = 0;
                    for (int k = static_cast<int>(w) - 1; k >= 0 && !((v >> k) & 1); --k) ++c;
                    return c;
                });
                each("cls", [&](Byte* o) { vec::bitOp(o, a.data(), n, ew, vec::BitOp::CountLeadingSigns); }, [&](U64 v) {
                    const U64 top = (v >> (w - 1)) & 1;
                    U64 c = 0;
                    for (int k = static_cast<int>(w) - 2; k >= 0 && ((v >> k) & 1) == top; --k) ++c;
                    return c;
                });
                each("rbit", [&](Byte* o) { vec::bitOp(o, a.data(), n, ew, vec::BitOp::ReverseBits); }, [&](U64 v) {
                    U64 r = 0;
                    for (unsigned k = 0; k < w; ++k)
                        if ((v >> k) & 1) r |= U64{1} << (w - 1 - k);
                    return r;
                });
                each("rev", [&](Byte* o) { vec::bitOp(o, a.data(), n, ew, vec::BitOp::ReverseBytes); }, [&](U64 v) {
                    U64 r = 0;
                    for (unsigned k = 0; k < ew; ++k) r = (r << 8) | ((v >> (8 * k)) & 0xff);
                    return r;
                });
            }
    // bitwise operations and selects, byte by byte
    for (std::size_t n : kSizes) {
        Bytes a(n), b(n), m(n), got(n), want(n);
        for (int iter = 0; iter < 20; ++iter) {
            for (std::size_t i = 0; i < n; ++i) a[i] = static_cast<Byte>(g_rng()), b[i] = static_cast<Byte>(g_rng()), m[i] = static_cast<Byte>(g_rng());
            vec::bitAnd(got.data(), a.data(), b.data(), n);
            for (std::size_t i = 0; i < n; ++i) want[i] = static_cast<Byte>(a[i] & b[i]);
            CB_CHECK(got == want);
            vec::bitOr(got.data(), a.data(), b.data(), n);
            for (std::size_t i = 0; i < n; ++i) want[i] = static_cast<Byte>(a[i] | b[i]);
            CB_CHECK(got == want);
            vec::bitXor(got.data(), a.data(), b.data(), n);
            for (std::size_t i = 0; i < n; ++i) want[i] = static_cast<Byte>(a[i] ^ b[i]);
            CB_CHECK(got == want);
            vec::bitAndNot(got.data(), a.data(), b.data(), n);
            for (std::size_t i = 0; i < n; ++i) want[i] = static_cast<Byte>(a[i] & (b[i] ^ 0xff));
            CB_CHECK(got == want);
            vec::bitOrNot(got.data(), a.data(), b.data(), n);
            for (std::size_t i = 0; i < n; ++i) want[i] = static_cast<Byte>(a[i] | (b[i] ^ 0xff));
            CB_CHECK(got == want);
            vec::bitNot(got.data(), a.data(), n);
            for (std::size_t i = 0; i < n; ++i) want[i] = static_cast<Byte>(a[i] ^ 0xff);
            CB_CHECK(got == want);
            vec::bitSelect(got.data(), m.data(), a.data(), b.data(), n);
            for (std::size_t i = 0; i < n; ++i)
                for (int k = 0; k < 8; ++k) {
                    const bool pick = (m[i] >> k) & 1;
                    const int bit = ((pick ? a[i] : b[i]) >> k) & 1;
                    want[i] = static_cast<Byte>((want[i] & ~(1 << k)) | (bit << k));
                }
            CB_CHECK(got == want);
            for (unsigned ew : kWidths) {
                vec::blend(got.data(), a.data(), b.data(), m.data(), n, ew);
                for (std::size_t i = 0; i < n; i += ew) std::memcpy(want.data() + i, ((m[i + ew - 1] & 0x80) ? b : a).data() + i, ew);
                CB_CHECK(got == want);
            }
        }
    }
    CB_CHECK_EQ(g_fails, 0u);
});

CB_TEST("engine.pcode_vector.shifts_match_exact_model_including_count_edges", [] {
    g_fails = 0;
    for (unsigned ew : kWidths)
        for (std::size_t n : kSizes)
            for (int iter = 0; iter < 40; ++iter) {
                const unsigned w = 8 * ew;
                const Bytes a = randomVector(n, ew), counts = randomVector(n, ew);
                const U64 immediates[] = {0, 1, w - 1, w, w + 1, 2 * w, 63, 64, 65, 127, 255, g_rng() % (2 * w), ~U64{0}};
                const auto model = [&](U64 v, vec::Shift sh, U64 count, vec::Count rule) -> U64 {
                    const bool isSigned = sh == vec::Shift::RightArithmetic;
                    const Big x = laneBig(v, ew, isSigned);
                    U64 c = count;
                    if (sh == vec::Shift::RotateLeft) {
                        c %= w;
                        const Big r = shlBig(laneBig(v, ew, false), static_cast<unsigned>(c)) + shrMag(laneBig(v, ew, false), static_cast<unsigned>(w - c));
                        return lowBits(r, w);
                    }
                    if (rule == vec::Count::Mask) c %= w;
                    if (c >= w) return sh == vec::Shift::RightArithmetic ? lowBits(x.neg ? bigS(-1) : Big{}, w) : 0;
                    if (sh == vec::Shift::Left) return lowBits(shlBig(x, static_cast<unsigned>(c)), w);
                    return lowBits(shrFloor(x, static_cast<unsigned>(c)), w);
                };
                for (int s = 0; s < 4; ++s)
                    for (int r = 0; r < 2; ++r) {
                        const vec::Shift sh = static_cast<vec::Shift>(s);
                        const vec::Count rule = r ? vec::Count::Mask : vec::Count::Saturate;
                        for (const U64 count : immediates) {
                            Bytes got(n), want(n);
                            vec::shiftImm(got.data(), a.data(), n, ew, sh, count, rule);
                            for (std::size_t i = 0; i < n; i += ew) vec::store(want.data() + i, ew, model(vec::load(a.data() + i, ew), sh, count, rule));
                            if (got != want) fail("shiftImm", ew, n, got, want, 0);
                        }
                        Bytes got(n), want(n);
                        vec::shiftVar(got.data(), a.data(), counts.data(), n, ew, sh, rule);
                        for (std::size_t i = 0; i < n; i += ew) vec::store(want.data() + i, ew, model(vec::load(a.data() + i, ew), sh, vec::load(counts.data() + i, ew), rule));
                        if (got != want) fail("shiftVar", ew, n, got, want, 0);
                    }
                // NEON register shifts: the count is the signed low byte
                for (const bool sg : {false, true})
                    for (const bool rnd : {false, true})
                        for (const bool sat : {false, true}) {
                            Bytes cv = counts;
                            for (std::size_t i = 0; i < n; i += ew)
                                if (g_rng() & 1) cv[i] = static_cast<Byte>(static_cast<int>(g_rng() % 140) - 70);
                            Bytes got(n), want(n);
                            vec::shiftNeon(got.data(), a.data(), cv.data(), n, ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned, rnd, sat);
                            for (std::size_t i = 0; i < n; i += ew) {
                                const int c = static_cast<std::int8_t>(cv[i]);
                                const Big x = laneBig(vec::load(a.data() + i, ew), ew, sg);
                                Big r;
                                if (c >= 0) {
                                    r = shlBig(x, static_cast<unsigned>(c));
                                    r = sat ? clamp(r, w, sg) : r;
                                } else {
                                    const unsigned k = static_cast<unsigned>(-c);
                                    r = shrFloor(rnd ? x + shlBig(bigU(1), k - 1) : x, k);
                                }
                                vec::store(want.data() + i, ew, lowBits(r, w));
                            }
                            if (got != want) fail("shiftNeon", ew, n, got, want, 0);
                        }
                // rounding right shift by an immediate, saturating left shift, shift and insert
                for (unsigned count = 0; count <= w; count += (w > 16 ? 5 : 1)) {
                    for (const bool sg : {false, true})
                        for (const bool rnd : {false, true}) {
                            Bytes got(n), want(n);
                            vec::shiftRightImm(got.data(), a.data(), n, ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned, count, rnd);
                            for (std::size_t i = 0; i < n; i += ew) {
                                const Big x = laneBig(vec::load(a.data() + i, ew), ew, sg);
                                vec::store(want.data() + i, ew, lowBits(shrFloor(rnd && count ? x + shlBig(bigU(1), count - 1) : x, count), w));
                            }
                            if (got != want) fail("shiftRightImm", ew, n, got, want, 0);
                        }
                    if (count < w) {
                        for (const bool sg : {false, true})
                            for (const bool toUnsigned : {false, true}) {
                                if (toUnsigned && !sg) continue;
                                Bytes got(n), want(n);
                                vec::shiftLeftSatImm(got.data(), a.data(), n, ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned, count, toUnsigned);
                                for (std::size_t i = 0; i < n; i += ew) {
                                    const Big x = shlBig(laneBig(vec::load(a.data() + i, ew), ew, sg), count);
                                    vec::store(want.data() + i, ew, lowBits(clamp(x, w, sg && !toUnsigned), w));
                                }
                                if (got != want) fail("shiftLeftSatImm", ew, n, got, want, 0);
                            }
                    }
                    const Bytes dst = randomVector(n, ew);
                    for (const bool left : {false, true}) {
                        if (left && count >= w) continue;
                        Bytes got(n), want(n);
                        vec::shiftInsert(got.data(), dst.data(), a.data(), n, ew, left, count);
                        for (std::size_t i = 0; i < n; i += ew) {
                            U64 d = vec::load(dst.data() + i, ew), s = vec::load(a.data() + i, ew), r = 0;
                            for (unsigned bit = 0; bit < w; ++bit) {
                                bool fromSrc;
                                int srcBit;
                                if (left) {
                                    fromSrc = bit >= count;
                                    srcBit = static_cast<int>(bit) - static_cast<int>(count);
                                } else {
                                    fromSrc = bit + count < w;
                                    srcBit = static_cast<int>(bit + count);
                                }
                                const U64 v = fromSrc ? (s >> srcBit) & 1 : (d >> bit) & 1;
                                r |= v << bit;
                            }
                            vec::store(want.data() + i, ew, r);
                        }
                        if (got != want) fail("shiftInsert", ew, n, got, want, 0);
                    }
                }
            }
    CB_CHECK_EQ(g_fails, 0u);
});

CB_TEST("engine.pcode_vector.width_changes_and_multiply_adds_match_exact_model", [] {
    g_fails = 0;
    for (unsigned ew : {2u, 4u, 8u})
        for (std::size_t n : kSizes)
            for (int iter = 0; iter < 60; ++iter) {
                const Bytes a = randomVector(n, ew);
                const unsigned wo = 4 * ew;   // the narrow lane's bits
                for (int m = 0; m < 4; ++m) {
                    const vec::Narrow mode = static_cast<vec::Narrow>(m);
                    const bool sg = mode == vec::Narrow::SignedToSigned || mode == vec::Narrow::SignedToUnsigned;
                    Bytes got(n / 2), want(n / 2);
                    vec::narrow(got.data(), a.data(), n, ew, mode);
                    for (std::size_t i = 0, j = 0; i < n; i += ew, j += ew / 2) {
                        const Big x = laneBig(vec::load(a.data() + i, ew), ew, sg);
                        const Big r = mode == vec::Narrow::Truncate ? x : clamp(x, wo, mode == vec::Narrow::SignedToSigned);
                        vec::store(want.data() + j, ew / 2, lowBits(r, wo));
                    }
                    if (got != want) fail("narrow", ew, n, got, want, 0);
                    for (unsigned count : {1u, 2u, wo, wo + 1u, 8 * ew}) {
                        if (count > 8 * ew) continue;
                        for (const bool rnd : {false, true}) {
                            vec::narrowShiftRight(got.data(), a.data(), n, ew, count, rnd, mode);
                            for (std::size_t i = 0, j = 0; i < n; i += ew, j += ew / 2) {
                                const Big x = laneBig(vec::load(a.data() + i, ew), ew, sg);
                                const Big s = shrFloor(rnd ? x + shlBig(bigU(1), count - 1) : x, count);
                                const Big r = mode == vec::Narrow::Truncate ? s : clamp(s, wo, mode == vec::Narrow::SignedToSigned);
                                vec::store(want.data() + j, ew / 2, lowBits(r, wo));
                            }
                            if (got != want) fail("narrowShiftRight", ew, n, got, want, 0);
                        }
                    }
                }
            }
    for (unsigned ew : {1u, 2u, 4u})
        for (std::size_t n : kSizes)
            for (int iter = 0; iter < 60; ++iter) {
                const Bytes a = randomVector(n, ew), b = randomVector(n, ew);
                const unsigned wo = 16 * ew;
                for (const bool sg : {false, true}) {
                    Bytes got(2 * n), want(2 * n);
                    vec::widen(got.data(), a.data(), n, ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned);
                    for (std::size_t i = 0; i < n; i += ew) vec::store(want.data() + 2 * i, 2 * ew, lowBits(laneBig(vec::load(a.data() + i, ew), ew, sg), wo));
                    if (got != want) fail("widen", ew, n, got, want, 0);
                    for (const bool sgB : {false, true}) {
                        vec::mulWiden(got.data(), a.data(), b.data(), n, ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned, sgB ? vec::Sign::Signed : vec::Sign::Unsigned);
                        for (std::size_t i = 0; i < n; i += ew)
                            vec::store(want.data() + 2 * i, 2 * ew, lowBits(laneBig(vec::load(a.data() + i, ew), ew, sg) * laneBig(vec::load(b.data() + i, ew), ew, sgB), wo));
                        if (got != want) fail("mulWiden", ew, n, got, want, 0);
                    }
                    for (const bool odd : {false, true}) {
                        Bytes g2(n), w2(n);
                        vec::mulEvenOdd(g2.data(), a.data(), b.data(), n, ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned, odd);
                        for (std::size_t i = odd ? 1 : 0, j = 0; i < n / ew; i += 2, j += 2 * ew)
                            vec::store(w2.data() + j, 2 * ew, lowBits(laneBig(vec::load(a.data() + i * ew, ew), ew, sg) * laneBig(vec::load(b.data() + i * ew, ew), ew, sg), wo));
                        if (g2 != w2) fail("mulEvenOdd", ew, n, g2, w2, 0);
                    }
                    Bytes p(n), pw(n);
                    vec::addLongPairwise(p.data(), nullptr, a.data(), n, ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned);
                    for (std::size_t i = 0, j = 0; i < n; i += 2 * ew, j += 2 * ew)
                        vec::store(pw.data() + j, 2 * ew, lowBits(laneBig(vec::load(a.data() + i, ew), ew, sg) + laneBig(vec::load(a.data() + i + ew, ew), ew, sg), wo));
                    if (p != pw) fail("addLongPairwise", ew, n, p, pw, 0);
                    const Bytes acc = randomVector(n, 2 * ew);
                    vec::addLongPairwise(p.data(), acc.data(), a.data(), n, ew, sg ? vec::Sign::Signed : vec::Sign::Unsigned);
                    for (std::size_t i = 0, j = 0; i < n; i += 2 * ew, j += 2 * ew)
                        vec::store(pw.data() + j, 2 * ew,
                                   lowBits(laneBig(vec::load(a.data() + i, ew), ew, sg) + laneBig(vec::load(a.data() + i + ew, ew), ew, sg) + bigU(vec::load(acc.data() + j, 2 * ew)), wo));
                    if (p != pw) fail("addLongPairwise+acc", ew, n, p, pw, 0);
                }
                // grouped multiply-add into lanes ewOut wide
                for (unsigned ewOut : {2u, 4u, 8u}) {
                    if (ewOut < ew || ewOut / ew > 8) continue;
                    for (const bool sa : {false, true})
                        for (const bool sb : {false, true})
                            for (const bool sat : {false, true})
                                for (const bool useAcc : {false, true}) {
                                    const Bytes acc = randomVector(n, ewOut);
                                    Bytes got(n), want(n);
                                    vec::multiplyAdd(got.data(), useAcc ? acc.data() : nullptr, a.data(), b.data(), n, ew, ewOut, sa ? vec::Sign::Signed : vec::Sign::Unsigned,
                                                     sb ? vec::Sign::Signed : vec::Sign::Unsigned, sat);
                                    const unsigned g = ewOut / ew;
                                    const bool anySigned = sa || sb;
                                    for (std::size_t o = 0; o < n; o += ewOut) {
                                        Big sum = useAcc ? laneBig(vec::load(acc.data() + o, ewOut), ewOut, anySigned) : Big{};
                                        for (unsigned k = 0; k < g; ++k)
                                            sum = sum + laneBig(vec::load(a.data() + o + k * ew, ew), ew, sa) * laneBig(vec::load(b.data() + o + k * ew, ew), ew, sb);
                                        vec::store(want.data() + o, ewOut, lowBits(sat ? clamp(sum, 8 * ewOut, anySigned) : sum, 8 * ewOut));
                                    }
                                    if (got != want) fail("multiplyAdd", ew, n, got, want, 0);
                                }
                }
            }
    // mulHighAddSat (AltiVec vmhaddshs, vmhraddshs at 2 bytes; the rule for every width) and sum of absolute differences
    for (unsigned ew : kWidths)
        for (std::size_t n : kSizes)
            for (int iter = 0; iter < 40; ++iter) {
                const unsigned w = 8 * ew;
                const Bytes a = randomVector(n, ew), b = randomVector(n, ew), c = randomVector(n, ew);
                for (const bool rnd : {false, true}) {
                    Bytes got(n), want(n);
                    vec::mulHighAddSat(got.data(), a.data(), b.data(), c.data(), n, ew, rnd);
                    for (std::size_t i = 0; i < n; i += ew) {
                        const Big p = laneBig(vec::load(a.data() + i, ew), ew, true) * laneBig(vec::load(b.data() + i, ew), ew, true);
                        const Big r = shrFloor(rnd ? p + shlBig(bigU(1), w - 2) : p, w - 1) + laneBig(vec::load(c.data() + i, ew), ew, true);
                        vec::store(want.data() + i, ew, lowBits(clamp(r, w, true), w));
                    }
                    if (got != want) fail("mulHighAddSat", ew, n, got, want, 0);
                }
            }
    for (std::size_t n : kSizes)
        for (int iter = 0; iter < 80; ++iter) {
            const Bytes a = randomVector(n, 1), b = randomVector(n, 1);
            if (n % 8 != 0) continue;
            Bytes got(n), want(n);
            vec::sumAbsDiff(got.data(), a.data(), b.data(), n);
            for (std::size_t g = 0; g < n; g += 8) {
                U64 s = 0;
                for (int k = 0; k < 8; ++k) s += a[g + k] > b[g + k] ? a[g + k] - b[g + k] : b[g + k] - a[g + k];
                vec::store(want.data() + g, 8, s);
            }
            if (got != want) fail("sumAbsDiff", 8, n, got, want, 0);
        }
    // lane access
    for (unsigned ew : kWidths) {
        Bytes v = randomVector(32, ew);
        for (std::size_t i = 0; i < 32 / ew; ++i) {
            const U64 x = edgeLane(ew);
            vec::setLane(v.data(), ew, i, x);
            CB_CHECK_EQ(vec::getLane(v.data(), ew, i), x);
            CB_CHECK_EQ(vec::load(v.data() + i * ew, ew), x);
        }
    }
    CB_CHECK_EQ(g_fails, 0u);
});

CB_TEST("engine.pcode_vector.lane_movement_reductions_and_lookups_match_a_direct_model", [] {
    g_fails = 0;
    for (unsigned ew : kWidths)
        for (std::size_t n : kSizes) {
            const std::size_t lanes = n / ew;
            if (lanes < 2) continue;
            for (int iter = 0; iter < 30; ++iter) {
                const Bytes a = randomVector(n, ew), b = randomVector(n, ew);
                const auto lane = [&](const Bytes& v, std::size_t i) { return vec::load(v.data() + i * ew, ew); };
                const auto build = [&](auto&& pick) {
                    Bytes r(n);
                    for (std::size_t i = 0; i < lanes; ++i) vec::store(r.data() + i * ew, ew, pick(i));
                    return r;
                };
                for (const bool high : {false, true}) {
                    Bytes got(n);
                    vec::zip(got.data(), a.data(), b.data(), n, ew, high);
                    const Bytes want = build([&](std::size_t i) { return (i % 2 == 0 ? lane(a, i / 2 + (high ? lanes / 2 : 0)) : lane(b, i / 2 + (high ? lanes / 2 : 0))); });
                    if (got != want) fail("zip", ew, n, got, want, 0);
                }
                for (const bool odd : {false, true}) {
                    Bytes got(n);
                    vec::unzip(got.data(), a.data(), b.data(), n, ew, odd);
                    const Bytes wantU = build([&](std::size_t i) { return i < lanes / 2 ? lane(a, 2 * i + (odd ? 1 : 0)) : lane(b, 2 * (i - lanes / 2) + (odd ? 1 : 0)); });
                    if (got != wantU) fail("unzip", ew, n, got, wantU, 0);
                    vec::transpose(got.data(), a.data(), b.data(), n, ew, odd);
                    const Bytes wantT = build([&](std::size_t i) { return i % 2 == 0 ? lane(a, i + (odd ? 1 : 0)) : lane(b, i - 1 + (odd ? 1 : 0)); });
                    if (got != wantT) fail("transpose", ew, n, got, wantT, 0);
                }
                Bytes got(n), same = a;
                vec::reverseLanes(got.data(), a.data(), n, ew);
                CB_CHECK(got == build([&](std::size_t i) { return lane(a, lanes - 1 - i); }));
                vec::reverseLanes(same.data(), same.data(), n, ew);   // in place
                CB_CHECK(same == got);
                for (unsigned group : {2u, 4u, 8u, 16u}) {
                    if (group < ew || group > n) continue;
                    vec::reverseInGroups(got.data(), a.data(), n, ew, group);
                    const std::size_t per = group / ew;
                    CB_CHECK(got == build([&](std::size_t i) { return lane(a, i / per * per + (per - 1 - i % per)); }));
                }
                const U64 v = edgeLane(ew);
                vec::splat(got.data(), n, ew, v);
                CB_CHECK(got == build([&](std::size_t) { return v; }));
                // reductions
                U64 sum = 0, mn = lane(a, 0), mx = lane(a, 0);
                Big exact;
                I64 smn = vec::signExtend(lane(a, 0), ew), smx = smn;
                for (std::size_t i = 0; i < lanes; ++i) {
                    sum = (sum + lane(a, i)) & vec::laneMask(ew);
                    mn = std::min(mn, lane(a, i)), mx = std::max(mx, lane(a, i));
                    exact = exact + laneBig(lane(a, i), ew, true);
                    smn = std::min(smn, vec::signExtend(lane(a, i), ew)), smx = std::max(smx, vec::signExtend(lane(a, i), ew));
                }
                CB_CHECK_EQ(vec::reduce(a.data(), n, ew, vec::Reduce::Add, vec::Sign::Unsigned), sum);
                CB_CHECK_EQ(vec::reduce(a.data(), n, ew, vec::Reduce::Min, vec::Sign::Unsigned), mn);
                CB_CHECK_EQ(vec::reduce(a.data(), n, ew, vec::Reduce::Max, vec::Sign::Unsigned), mx);
                CB_CHECK_EQ(vec::signExtend(vec::reduce(a.data(), n, ew, vec::Reduce::Min, vec::Sign::Signed), ew), smn);
                CB_CHECK_EQ(vec::signExtend(vec::reduce(a.data(), n, ew, vec::Reduce::Max, vec::Sign::Signed), ew), smx);
                if (ew <= 4) CB_CHECK_EQ(vec::reduce(a.data(), n, ew, vec::Reduce::AddLong, vec::Sign::Signed), lowBits(exact, 64));
                // pairwise
                for (int p = 0; p < 5; ++p) {
                    Bytes pg(n);
                    vec::pairwise(pg.data(), a.data(), b.data(), n, ew, static_cast<vec::Pair>(p));
                    const Bytes want = build([&](std::size_t i) {
                        const Bytes& src = i < lanes / 2 ? a : b;
                        const std::size_t k = i < lanes / 2 ? i : i - lanes / 2;
                        const U64 x = lane(src, 2 * k), y = lane(src, 2 * k + 1);
                        const Big bx = laneBig(x, ew, p == 1 || p == 3), by = laneBig(y, ew, p == 1 || p == 3);
                        switch (p) {
                            case 0: return (x + y) & vec::laneMask(ew);
                            case 1: case 2: return cmp(bx, by) <= 0 ? x : y;
                            default: return cmp(bx, by) >= 0 ? x : y;
                        }
                    });
                    if (pg != want) fail("pairwise", ew, n, pg, want, 0);
                }
                // byte shifts, extract, lanes by index
                for (unsigned count : {0u, 1u, 3u, 15u, 16u, 17u}) {
                    for (const bool left : {false, true})
                        for (std::size_t group : {std::size_t{16}, n}) {
                            if (n % group) continue;
                            Bytes sg(n), want(n, 0);
                            vec::shiftBytes(sg.data(), a.data(), n, group, count, left);
                            for (std::size_t g = 0; g < n; g += group)
                                for (std::size_t i = 0; i < group; ++i) {
                                    const long src = left ? static_cast<long>(i) - count : static_cast<long>(i) + count;
                                    want[g + i] = (src >= 0 && src < static_cast<long>(group)) ? a[g + static_cast<std::size_t>(src)] : 0;
                                }
                            CB_CHECK(sg == want);
                        }
                }
                for (unsigned idx : {0u, 1u, 5u, static_cast<unsigned>(n)}) {
                    if (idx > n) continue;
                    Bytes e(n), want(n);
                    vec::extract(e.data(), a.data(), b.data(), n, idx);
                    for (std::size_t i = 0; i < n; ++i) want[i] = i + idx < n ? a[i + idx] : b[i + idx - n];
                    CB_CHECK(e == want);
                }
                Bytes idxv(lanes), pl(n);
                for (std::size_t i = 0; i < lanes; ++i) idxv[i] = static_cast<Byte>(g_rng() % (lanes + 2));
                vec::permuteLanes(pl.data(), a.data(), n, ew, idxv.data(), lanes);
                CB_CHECK(pl == build([&](std::size_t i) { return idxv[i] < lanes ? lane(a, idxv[i]) : U64{0}; }));
                // table lookups: TBL / TBX over up to four registers, PSHUFB, VPERM
                const Bytes table = randomVector(32, 1);
                Bytes ix(n);
                for (auto& x : ix) x = static_cast<Byte>(g_rng() % 40 + ((g_rng() & 7) == 0 ? 0x80 : 0));
                for (const bool keep : {false, true}) {
                    Bytes got2 = a, want2 = a;
                    vec::Lookup mode;
                    mode.keepOutOfRange = keep;
                    vec::tableLookup(got2.data(), table.data(), 32, ix.data(), n, mode);
                    for (std::size_t i = 0; i < n; ++i) want2[i] = ix[i] < 32 ? table[ix[i]] : (keep ? a[i] : Byte{0});
                    CB_CHECK(got2 == want2);
                }
                {
                    Bytes got2(n), want2(n);
                    vec::Lookup mode;
                    mode.indexMask = 0x0f;
                    mode.bit7Zero = true;
                    mode.groupBytes = 16;
                    const Bytes tb = randomVector(n, 1);
                    if (n % 16 == 0) {   // (pshufb works on groups of 16 bytes)
                        vec::tableLookup(got2.data(), tb.data(), n, ix.data(), n, mode);
                        for (std::size_t i = 0; i < n; ++i) want2[i] = (ix[i] & 0x80) ? Byte{0} : tb[i / 16 * 16 + (ix[i] & 15)];
                        CB_CHECK(got2 == want2);
                    }
                    mode = vec::Lookup{};
                    mode.indexMask = 0x1f;
                    vec::tableLookup(got2.data(), table.data(), 32, ix.data(), n, mode);
                    for (std::size_t i = 0; i < n; ++i) want2[i] = table[ix[i] & 0x1f];
                    CB_CHECK(got2 == want2);
                }
            }
        }
    CB_CHECK_EQ(g_fails, 0u);
});

CB_TEST("engine.pcode_vector.mask_polynomial_crc_and_pixel_operations_match_published_values_and_models", [] {
    g_fails = 0;
    // MOVMSK: bit i is the top bit of lane i
    for (unsigned ew : kWidths)
        for (std::size_t n : {std::size_t{8}, std::size_t{16}, std::size_t{32}}) {
            if (n / ew > 64) continue;
            const Bytes a = randomVector(n, ew);
            U64 want = 0;
            for (std::size_t i = 0; i < n / ew; ++i)
                if (laneBig(vec::load(a.data() + i * ew, ew), ew, true).neg) want |= U64{1} << i;
            CB_CHECK_EQ(vec::moveMask(a.data(), n, ew), want);
        }
    // carry-less products: bit k of the product is the XOR of a(i) & b(k - i)
    const auto polyBit = [](U64 a, U64 b, unsigned k) {
        unsigned x = 0;
        for (unsigned i = 0; i <= k; ++i)
            if (i < 64 && k - i < 64) x ^= static_cast<unsigned>(((a >> i) & 1) & ((b >> (k - i)) & 1));
        return x;
    };
    for (int iter = 0; iter < 300; ++iter) {
        const U64 a = edgeLane(8), b = edgeLane(8);
        Bytes p(16);
        vec::polyMul64(p.data(), a, b);
        U64 lo = 0, hi = 0;
        for (unsigned k = 0; k < 127; ++k) (k < 64 ? lo : hi) |= static_cast<U64>(polyBit(a, b, k)) << (k % 64);
        CB_CHECK_EQ(vec::load(p.data(), 8), lo);
        CB_CHECK_EQ(vec::load(p.data() + 8, 8), hi);
        for (unsigned ew : kWidths) {
            const Bytes x = randomVector(16, ew), y = randomVector(16, ew);
            Bytes got(16), wide(32);
            vec::polyMul(got.data(), x.data(), y.data(), 16, ew);
            for (std::size_t i = 0; i < 16; i += ew) {
                U64 want = 0;
                for (unsigned k = 0; k < 8 * ew; ++k) want |= static_cast<U64>(polyBit(vec::load(x.data() + i, ew), vec::load(y.data() + i, ew), k)) << k;
                CB_CHECK_EQ(vec::load(got.data() + i, ew), want);
            }
            if (ew <= 4) {
                vec::polyMulWiden(wide.data(), x.data(), y.data(), 16, ew);
                for (std::size_t i = 0; i < 16; i += ew) {
                    U64 want = 0;
                    for (unsigned k = 0; k < 16 * ew; ++k) want |= static_cast<U64>(polyBit(vec::load(x.data() + i, ew), vec::load(y.data() + i, ew), k)) << k;
                    CB_CHECK_EQ(vec::load(wide.data() + 2 * i, 2 * ew), want);
                }
            }
        }
    }
    // CRC-32 and CRC-32C: the published check values for "123456789" (0xCBF43926 and 0xE3069283), fed in every width, and a bit by bit model of the definition (the
    // polynomial division of the message, most significant bit first, written without the reflection the library uses)
    const char* text = "123456789";
    for (const bool castagnoli : {false, true}) {
        std::uint32_t acc = 0xffffffffu;
        for (int i = 0; i < 9; ++i) acc = vec::crc32Update(acc, static_cast<unsigned char>(text[i]), 1, castagnoli);
        CB_CHECK_EQ(~acc, castagnoli ? 0xE3069283u : 0xCBF43926u);
        acc = 0xffffffffu;   // "1234" as one word, "5678" as one word, "9"
        std::uint64_t w8 = 0;
        for (int i = 0; i < 8; ++i) w8 |= static_cast<std::uint64_t>(static_cast<unsigned char>(text[i])) << (8 * i);
        acc = vec::crc32Update(acc, w8, 8, castagnoli);
        acc = vec::crc32Update(acc, static_cast<unsigned char>(text[8]), 1, castagnoli);
        CB_CHECK_EQ(~acc, castagnoli ? 0xE3069283u : 0xCBF43926u);
        acc = 0xffffffffu;
        acc = vec::crc32Update(acc, static_cast<std::uint32_t>(w8), 4, castagnoli);
        acc = vec::crc32Update(acc, w8 >> 32, 4, castagnoli);
        acc = vec::crc32Update(acc, static_cast<unsigned char>(text[8]), 1, castagnoli);
        CB_CHECK_EQ(~acc, castagnoli ? 0xE3069283u : 0xCBF43926u);
        acc = 0xffffffffu;
        for (int i = 0; i + 1 < 9; i += 2) acc = vec::crc32Update(acc, static_cast<unsigned char>(text[i]) | (static_cast<unsigned>(static_cast<unsigned char>(text[i + 1])) << 8), 2, castagnoli);
        acc = vec::crc32Update(acc, static_cast<unsigned char>(text[8]), 1, castagnoli);
        CB_CHECK_EQ(~acc, castagnoli ? 0xE3069283u : 0xCBF43926u);
        // the model: arbitrary start value and data, remainder of (message * x^32) modulo the polynomial, bit by bit (the instruction has no inversion)
        for (int iter = 0; iter < 200; ++iter) {
            const std::uint32_t start = static_cast<std::uint32_t>(g_rng());
            const U64 data = g_rng();
            const unsigned size = 1u << (g_rng() % 4);
            const std::uint32_t poly = castagnoli ? 0x1EDC6F41u : 0x04C11DB7u;   // the polynomials as usually written (most significant bit first)
            const auto reverse32 = [](std::uint32_t v) { std::uint32_t r = 0; for (int i = 0; i < 32; ++i) r |= ((v >> i) & 1u) << (31 - i); return r; };
            std::uint32_t reg = reverse32(start);   // the register in the non reflected form
            for (unsigned byte = 0; byte < size; ++byte) {
                const unsigned b = static_cast<unsigned>((data >> (8 * byte)) & 0xff);
                for (int bit = 7; bit >= 0; --bit) {   // the reflected input is fed least significant bit first, i.e. the byte's bits reversed for the non reflected form
                    const unsigned in = (b >> (7 - bit)) & 1u;
                    const unsigned top = (reg >> 31) & 1u;
                    reg <<= 1;
                    if (top ^ in) reg ^= poly;
                }
            }
            CB_CHECK_EQ(vec::crc32Update(start, data, size, castagnoli), reverse32(reg));
        }
    }
    // VPKPX / VUPKHPX: the big-endian bit numbering of the manual (bit 0 is the most significant of a word)
    for (int iter = 0; iter < 200; ++iter) {
        const Bytes a = randomVector(16, 4), b = randomVector(16, 4);
        Bytes got(16);
        vec::packPixel32(got.data(), a.data(), b.data(), 16);
        for (int i = 0; i < 8; ++i) {
            const U64 w = i < 4 ? vec::load(a.data() + 4 * i, 4) : vec::load(b.data() + 4 * (i - 4), 4);
            const auto be = [&](int from, int to) { U64 v = 0; for (int k = from; k <= to; ++k) v = (v << 1) | ((w >> (31 - k)) & 1); return v; };
            const U64 want = (be(7, 7) << 15) | (be(8, 12) << 10) | (be(16, 20) << 5) | be(24, 28);
            CB_CHECK_EQ(vec::load(got.data() + 2 * i, 2), want);
        }
        const Bytes px = randomVector(16, 2);
        Bytes wide(32);
        vec::unpackPixel16(wide.data(), px.data(), 16);
        for (int i = 0; i < 8; ++i) {
            const U64 h = vec::load(px.data() + 2 * i, 2);
            const auto be = [&](int from, int to) { U64 v = 0; for (int k = from; k <= to; ++k) v = (v << 1) | ((h >> (15 - k)) & 1); return v; };
            const U64 want = ((be(0, 0) ? 0xffu : 0u) << 24) | (be(1, 5) << 16) | (be(6, 10) << 8) | be(11, 15);
            CB_CHECK_EQ(vec::load(wide.data() + 4 * i, 4), want);
        }
    }
    CB_CHECK_EQ(g_fails, 0u);
});

}  // namespace cb
