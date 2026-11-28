// cb-unity: alone (file-local helpers named for what they do here)
// recompile/pcode_emit_vmx.cpp -- see pcode_emit_vmx.hpp. Each form follows XenonRecomp's translation of the instruction
// (src/recompile/xenonrecomp/XenonRecomp/recompiler.cpp), with the description's operands in the description's order.
#include "recompile/pcode_emit_vmx.hpp"

#include <cstdio>
#include <functional>
#include <map>
#include <string>

namespace cb::recompile::pcode_emit::vmx {

namespace {

using Args = std::vector<Arg>;

// the context's vector as SIMDe's types, and back
std::string I(const std::string& v) { return "__builtin_bit_cast(simde__m128i, (__uint128_t)(" + v + "))"; }
std::string F(const std::string& v) { return "__builtin_bit_cast(simde__m128, (__uint128_t)(" + v + "))"; }
std::string R(const std::string& e) { return "__builtin_bit_cast(__uint128_t, " + e + ")"; }
std::string P(const std::string& v) { return "__builtin_bit_cast(PPCVRegister, (__uint128_t)(" + v + "))"; }
std::string ea32(const std::string& v) { return "uint32_t(" + v + ")"; }
std::string hex(std::uint64_t v) { return "0x" + [&] { char b[32]; std::snprintf(b, sizeof b, "%llX", static_cast<unsigned long long>(v)); return std::string(b); }(); }

struct Form {
    unsigned inputs;    // the number of inputs this form takes
    bool vmx;           // floating point of the vector unit
    std::function<std::optional<std::string>(const Args&, const std::string& output)> write;
    bool statement = false;
};

std::optional<std::uint64_t> k(const Args& a, std::size_t i) { return i < a.size() ? a[i].constant : std::nullopt; }

// A binary operation of SIMDe on two vectors as floats (f) or integers (i), with the description's operand order or swapped.
Form fbin(const char* op) {
    return {2, true, [op](const Args& a, const std::string&) { return std::optional<std::string>(R(std::string(op) + "(" + F(a[0].value) + ", " + F(a[1].value) + ")")); }};
}
Form ibin(const char* op, bool swap = false) {
    return {2, false, [op, swap](const Args& a, const std::string&) {
                return std::optional<std::string>(R(std::string(op) + "(" + I(a[swap ? 1 : 0].value) + ", " + I(a[swap ? 0 : 1].value) + ")"));
            }};
}
Form fround(const char* mode) {
    return {1, true, [mode](const Args& a, const std::string&) {
                return std::optional<std::string>(R("simde_mm_round_ps(" + F(a[0].value) + ", " + mode + " | SIMDE_MM_FROUND_NO_EXC)"));
            }};
}
// per-lane float function (exp2f, log2f)
Form flane(const char* fn) {
    return {1, true, [fn](const Args& a, const std::string&) {
                return std::optional<std::string>("[&] { PPCVRegister v = " + P(a[0].value) + "; for (int i = 0; i < 4; i++) v.f32[i] = " + fn +
                                                  "(v.f32[i]); return __builtin_bit_cast(__uint128_t, v); }()");
            }};
}
// per-lane integer shift by the low bits of the same lane of vB (element width w bytes)
Form ishift(unsigned w, const char* type, const char* op) {
    return {2, false, [w, type, op](const Args& a, const std::string&) {
                const unsigned n = 16 / w, mask = 8 * w - 1;
                const std::string field = w == 1 ? "u8" : w == 2 ? "u16" : "u32";
                const std::string sfield = std::string(type)[0] == 'i' ? (w == 1 ? "s8" : w == 2 ? "s16" : "s32") : field;
                return std::optional<std::string>("[&] { PPCVRegister x = " + P(a[0].value) + ", y = " + P(a[1].value) + ", d; for (int i = 0; i < " +
                                                  std::to_string(n) + "; i++) d." + sfield + "[i] = " + type + "(x." + sfield + "[i] " + op + " (y." + field +
                                                  "[i] & " + std::to_string(mask) + ")); return __builtin_bit_cast(__uint128_t, d); }()");
            }};
}

const std::map<std::string, Form, std::less<>>& forms() {
    static const std::map<std::string, Form, std::less<>> table = [] {
        std::map<std::string, Form, std::less<>> t;
        // ---- floating point (the vector unit's: the VMX flush mode)
        t["vectorAddFloatingPoint"] = fbin("simde_mm_add_ps");
        t["vectorSubtractFloatingPoint"] = fbin("simde_mm_sub_ps");
        t["vectorMultiplyFloatingPoint"] = fbin("simde_mm_mul_ps");
        t["vectorMaximumFloatingPoint"] = fbin("simde_mm_max_ps");
        t["vectorMinimumFloatingPoint"] = fbin("simde_mm_min_ps");
        t["vectorCompareEqualToFloatingPoint"] = fbin("simde_mm_cmpeq_ps");
        t["vectorCompareGreaterThanFloatingPoint"] = fbin("simde_mm_cmpgt_ps");
        t["vectorCompareGreaterThanOrEqualToFloatingPoint"] = fbin("simde_mm_cmpge_ps");
        // a * c + b, the product rounded first (the console's vmaddfp is not fused)
        const auto madd = Form{3, true, [](const Args& a, const std::string&) {
                                   return std::optional<std::string>(
                                       R("simde_mm_add_ps(simde_mm_mul_ps(" + F(a[0].value) + ", " + F(a[1].value) + "), " + F(a[2].value) + ")"));
                               }};
        t["vectorMultiplyAddFloatingPoint"] = madd;
        t["vectorMultiplyAddFloatingPointToD"] = madd;
        t["vectorNegativeMultiplySubtractFloatingPoint"] = Form{3, true, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(" + F(a[0].value) + ", " + F(a[1].value) + "), " +
                                                F(a[2].value) + "), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))))"));
        }};
        t["vectorMultiplySum4FloatingPoint"] = Form{2, true, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_dp_ps(" + F(a[0].value) + ", " + F(a[1].value) + ", 0xFF)"));
        }};
        t["vectorMultiplySum3FloatingPoint"] = Form{2, true, [](const Args& a, const std::string&) {   // (yzw in the reversed order)
            return std::optional<std::string>(R("simde_mm_dp_ps(" + F(a[0].value) + ", " + F(a[1].value) + ", 0xEF)"));
        }};
        t["vectorReciprocalEstimateFloatingPoint"] = Form{1, true, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_div_ps(simde_mm_set1_ps(1), " + F(a[0].value) + ")"));
        }};
        t["vectorReciprocalSquareRootEstimateFloatingPoint"] = Form{1, true, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(" + F(a[0].value) + "))"));
        }};
        t["vector2RaisedToTheExponentEstimateFloatingPoint"] = flane("exp2f");
        t["vectorLog2EstimateFloatingPoint"] = flane("log2f");
        t["vectorRoundToFloatingPointIntegerNearest"] = fround("SIMDE_MM_FROUND_TO_NEAREST_INT");
        t["vectorRoundToFloatingPointIntegerTowardZero"] = fround("SIMDE_MM_FROUND_TO_ZERO");
        t["vectorRoundToFloatingPointIntegerTowardMinusInfinity"] = fround("SIMDE_MM_FROUND_TO_NEG_INF");
        t["vectorRoundToFloatingPointIntegerTowardPlusInfinity"] = fround("SIMDE_MM_FROUND_TO_POS_INF");
        const auto from_fixed = [](const char* convert) {
            return Form{2, true, [convert](const Args& a, const std::string&) -> std::optional<std::string> {
                            const std::optional<std::uint64_t> s = k(a, 1);
                            if (!s) return std::nullopt;
                            const std::string c = std::string(convert) + "(" + I(a[0].value) + ")";
                            if (*s == 0) return R(c);
                            return R("simde_mm_mul_ps(" + c + ", simde_mm_set1_ps(0x1p-" + std::to_string(*s) + "f))");
                        }};
        };
        t["vectorConvertFromSignedFixedPointWord"] = from_fixed("simde_mm_cvtepi32_ps");
        t["vectorConvertFromUnsignedFixedPointWord"] = from_fixed("simde_mm_cvtepu32_ps_");
        const auto to_fixed = [](const char* convert) {
            return Form{2, true, [convert](const Args& a, const std::string&) -> std::optional<std::string> {
                            const std::optional<std::uint64_t> s = k(a, 1);
                            if (!s) return std::nullopt;
                            const std::string v = *s == 0 ? F(a[0].value) : "simde_mm_mul_ps(" + F(a[0].value) + ", simde_mm_set1_ps(" + std::to_string(1u << *s) + "))";
                            return R(std::string(convert) + "(" + v + ")");
                        }};
        };
        t["vectorConvertToSignedFixedPointWordSaturate"] = to_fixed("simde_mm_vctsxs");
        t["vectorConvertToUnsignedFixedPointWordSaturate"] = to_fixed("simde_mm_vctuxs");
        t["vectorCompareBoundsFloatingPoint"] = Form{2, true, [](const Args& a, const std::string&) {
            return std::optional<std::string>("[&] { PPCVRegister d; ppc_vcmpbfp(d, " + P(a[0].value) + ", " + P(a[1].value) +
                                              "); return __builtin_bit_cast(__uint128_t, d); }()");
        }};
        // ---- integer and logical
        t["vectorLogicalAnd"] = ibin("simde_mm_and_si128");
        t["vectorCompareEqualToUnsignedWord"] = ibin("simde_mm_cmpeq_epi32");
        t["vectorCompareEqualToUnsignedHalfWord"] = ibin("simde_mm_cmpeq_epi16");
        t["vectorCompareEqualToUnsignedByte"] = ibin("simde_mm_cmpeq_epi8");
        t["vectorCompareGreaterThanUnsignedByte"] = ibin("simde_mm_cmpgt_epu8");
        t["vectorCompareGreaterThanUnsignedHalfWord"] = ibin("simde_mm_cmpgt_epu16");
        t["vectorMaximumUnsignedHalfWord"] = ibin("simde_mm_max_epu16");
        t["vectorMinimumUnsignedHalfWord"] = ibin("simde_mm_min_epu16");
        t["vectorAddUnsignedHalfWordSaturate"] = ibin("simde_mm_adds_epu16");
        t["vectorAddSignedHalfWordSaturate"] = ibin("simde_mm_adds_epi16");
        t["vectorAddUnsignedByteSaturate"] = ibin("simde_mm_adds_epu8");
        t["vectorAddUnsignedWordSaturate"] = ibin("simde_mm_adds_epu32");
        // (the AltiVec forms the instruction tests use and these titles do not: XenonRecomp's forms likewise)
        t["vectorAverageUnsignedHalfWord"] = ibin("simde_mm_avg_epu16");
        t["vectorLogicalAndWithComplement"] = ibin("simde_mm_andnot_si128", true);   // vA & ~vB
        t["vectorMaximumSignedHalfWord"] = ibin("simde_mm_max_epi16");
        t["vectorMinimumSignedHalfWord"] = ibin("simde_mm_min_epi16");
        t["vectorPackSignedHalfWordSignedSaturate"] = ibin("simde_mm_packs_epi16", true);
        t["vectorPackSignedHalfWordUnsignedSaturate"] = ibin("simde_mm_packus_epi16", true);
        t["vectorPackSignedWordUnsignedSaturate"] = ibin("simde_mm_packus_epi32", true);
        t["vectorSubtractSignedByteSaturate"] = ibin("simde_mm_subs_epi8");
        t["vectorSubtractSignedHalfWordSaturate"] = ibin("simde_mm_subs_epi16");
        t["vectorShiftRightHalfWord"] = ishift(2, "uint16_t", ">>");
        t["vectorShiftRightAlgebraicHalfWord"] = ishift(2, "int16_t", ">>");
        t["vectorAddSignedWordSaturate"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("[&] { PPCVRegister x = " + P(a[0].value) + ", y = " + P(a[1].value) +
                                              ", d; for (int i = 0; i < 4; i++) { const int64_t t = int64_t(x.s32[i]) + int64_t(y.s32[i]); "
                                              "d.s32[i] = t > INT_MAX ? INT_MAX : t < INT_MIN ? INT_MIN : int32_t(t); } return __builtin_bit_cast(__uint128_t, d); }()");
        }};
        t["vectorPackUnsignedHalfWordUnsignedModulo"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("[&] { PPCVRegister x = " + P(a[0].value) + ", y = " + P(a[1].value) +
                                              ", d; for (int i = 0; i < 8; i++) { d.u8[i] = uint8_t(y.u16[i]); d.u8[8 + i] = uint8_t(x.u16[i]); } "
                                              "return __builtin_bit_cast(__uint128_t, d); }()");
        }};
        t["vectorRotateLeftIntegerHalfWord"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("[&] { PPCVRegister x = " + P(a[0].value) + ", y = " + P(a[1].value) +
                                              ", d; for (int i = 0; i < 8; i++) d.u16[i] = uint16_t((x.u16[i] << (y.u16[i] & 0xF)) | (x.u16[i] >> ((16 - (y.u16[i] & 0xF)) & 0xF))); "
                                              "return __builtin_bit_cast(__uint128_t, d); }()");
        }};
        t["vectorShiftLeft"] = Form{2, false, [](const Args& a, const std::string&) {   // vsl: the whole vector by vB's low 3 bits
            return std::optional<std::string>("[&] { PPCVRegister x = " + P(a[0].value) + ", y = " + P(a[1].value) +
                                              ", d; const uint32_t sh = y.u8[0] & 0x7; const uint64_t lo = x.u64[0], hi = x.u64[1]; "
                                              "d.u64[1] = sh ? (hi << sh) | (lo >> (64 - sh)) : hi; d.u64[0] = lo << sh; return __builtin_bit_cast(__uint128_t, d); }()");
        }};
        t["vectorShiftRightByOctet"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("[&] { PPCVRegister x = " + P(a[0].value) + ", y = " + P(a[1].value) +
                                              ", d; const uint32_t sh = (y.u8[0] >> 3) & 0xF; for (uint32_t h = 0; h < 16; h++) d.u8[h] = h + sh < 16 ? x.u8[h + sh] : 0; "
                                              "return __builtin_bit_cast(__uint128_t, d); }()");
        }};
        t["vectorSplatImmediateSignedHalfWord"] = Form{1, false, [](const Args& a, const std::string&) -> std::optional<std::string> {
            const std::optional<std::uint64_t> u = k(a, 0);
            if (!u) return std::nullopt;
            const std::int64_t v = a[0].size == 1 ? static_cast<std::int8_t>(*u) : static_cast<std::int16_t>(*u);
            return R("simde_mm_set1_epi16(short(" + std::to_string(v) + "))");
        }};
        t["vectorMaximumSignedWord"] = ibin("simde_mm_max_epi32");
        t["vectorAverageSignedByte"] = ibin("simde_mm_avg_epi8");
        t["vectorAverageSignedHalfWord"] = ibin("simde_mm_avg_epi16");
        t["vectorMergeHighWord"] = ibin("simde_mm_unpackhi_epi32", true);
        t["vectorMergeHighHalfWord"] = ibin("simde_mm_unpackhi_epi16", true);
        t["vectorMergeHighByte"] = ibin("simde_mm_unpackhi_epi8", true);
        t["vectorMergeLowHalfWord"] = ibin("simde_mm_unpacklo_epi16", true);
        t["vectorMergeLowByte"] = ibin("simde_mm_unpacklo_epi8", true);
        t["vectorAddUnsignedByteModulo"] = ibin("simde_mm_add_epi8");
        t["vectorAddUnsignedHalfWordModulo"] = ibin("simde_mm_add_epi16");
        t["vectorSubtractUnsignedHalfWordModuloX"] = ibin("simde_mm_sub_epi16");
        t["vectorShiftLeftDoubleByOctetImmediate"] = Form{3, false, [](const Args& a, const std::string&) -> std::optional<std::string> {
            const std::optional<std::uint64_t> sh = k(a, 2);
            if (!sh || *sh > 15) return std::nullopt;
            return R("simde_mm_alignr_epi8(" + I(a[0].value) + ", " + I(a[1].value) + ", " + std::to_string(16 - *sh) + ")");
        }};
        t["vectorMergeLowWord"] = ibin("simde_mm_unpacklo_epi32", true);
        t["vectorPackSignedWordSignedSaturate"] = ibin("simde_mm_packs_epi32", true);   // (vB the low half, vA the high: the reversed order)
        t["vectorPackUnsignedHalfWordUnsignedSaturate"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_packus_epi16(simde_mm_min_epu16(" + I(a[1].value) + ", simde_mm_set1_epi16(0xFF)), simde_mm_min_epu16(" +
                                                I(a[0].value) + ", simde_mm_set1_epi16(0xFF)))"));
        }};
        t["vectorPackUnsignedWordUnsignedSaturate"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_packus_epi32(simde_mm_min_epu32(" + I(a[1].value) + ", simde_mm_set1_epi32(0xFFFF)), simde_mm_min_epu32(" +
                                                I(a[0].value) + ", simde_mm_set1_epi32(0xFFFF)))"));
        }};
        t["vectorPackUnsignedWordUnsignedModulo"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("[&] { PPCVRegister x = " + P(a[0].value) + ", y = " + P(a[1].value) +
                                              ", d; for (int i = 0; i < 4; i++) { d.u16[i] = uint16_t(y.u32[i]); d.u16[4 + i] = uint16_t(x.u32[i]); } "
                                              "return __builtin_bit_cast(__uint128_t, d); }()");
        }};
        t["vectorUnpackHighSignedHalfWord"] = Form{1, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_cvtepi16_epi32(simde_mm_unpackhi_epi64(" + I(a[0].value) + ", " + I(a[0].value) + "))"));
        }};
        t["vectorUnpackLowSignedHalfWord"] = Form{1, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_cvtepi16_epi32(" + I(a[0].value) + ")"));
        }};
        t["vectorUnpackHighSignedByte"] = Form{1, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_cvtepi8_epi16(simde_mm_unpackhi_epi64(" + I(a[0].value) + ", " + I(a[0].value) + "))"));
        }};
        t["vectorUnpackLowSignedByte"] = Form{1, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_cvtepi8_epi16(" + I(a[0].value) + ")"));
        }};
        t["vectorConditionalSelect"] = Form{3, false, [](const Args& a, const std::string&) {   // a bit of vB where vC has a one, else of vA
            return std::optional<std::string>(R("simde_mm_or_si128(simde_mm_andnot_si128(" + I(a[2].value) + ", " + I(a[0].value) + "), simde_mm_and_si128(" +
                                                I(a[2].value) + ", " + I(a[1].value) + "))"));
        }};
        t["vectorPermute"] = Form{3, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_perm_epi8_(" + I(a[0].value) + ", " + I(a[1].value) + ", " + I(a[2].value) + ")"));
        }};
        t["vectorSplatWord"] = Form{2, false, [](const Args& a, const std::string&) -> std::optional<std::string> {
            const std::optional<std::uint64_t> u = k(a, 1);
            if (!u) return std::nullopt;
            std::uint32_t perm = 3 - (*u & 3);
            perm |= (perm << 2) | (perm << 4) | (perm << 6);
            return R("simde_mm_shuffle_epi32(" + I(a[0].value) + ", " + hex(perm) + ")");
        }};
        t["vectorSplatHalfWord"] = Form{2, false, [](const Args& a, const std::string&) -> std::optional<std::string> {
            const std::optional<std::uint64_t> u = k(a, 1);
            if (!u) return std::nullopt;
            std::uint32_t perm = 7 - (*u & 7);
            perm = (perm * 2) | ((perm * 2 + 1) << 8);
            return R("simde_mm_shuffle_epi8(" + I(a[0].value) + ", simde_mm_set1_epi16(short(" + hex(perm) + ")))");
        }};
        t["vectorSplatImmediateSignedByte"] = Form{1, false, [](const Args& a, const std::string&) -> std::optional<std::string> {
            const std::optional<std::uint64_t> u = k(a, 0);
            if (!u) return std::nullopt;
            return R("simde_mm_set1_epi8(char(" + hex(*u & 0xFF) + "))");
        }};
        t["vectorPermuteWordImmediate"] = Form{2, false, [](const Args& a, const std::string&) -> std::optional<std::string> {
            const std::optional<std::uint64_t> u = k(a, 1);
            if (!u) return std::nullopt;
            const std::uint32_t x = 3 - (*u & 3), y = 3 - ((*u >> 2) & 3), z = 3 - ((*u >> 4) & 3), w = 3 - ((*u >> 6) & 3);
            return R("simde_mm_shuffle_epi32(" + I(a[0].value) + ", " + hex(x | (y << 2) | (z << 4) | (w << 6)) + ")");
        }};
        t["vectorRotateLeftImmediateAndMaskInsert"] = Form{4, false, [](const Args& a, const std::string&) -> std::optional<std::string> {
            const std::optional<std::uint64_t> mask = k(a, 2), z = k(a, 3);
            if (!mask || !z || *z > 3) return std::nullopt;
            static const char* const shuffles[] = {"SIMDE_MM_SHUFFLE(3, 2, 1, 0)", "SIMDE_MM_SHUFFLE(2, 1, 0, 3)", "SIMDE_MM_SHUFFLE(1, 0, 3, 2)",
                                                   "SIMDE_MM_SHUFFLE(0, 3, 2, 1)"};
            return R("simde_mm_blend_ps(" + F(a[0].value) + ", simde_mm_permute_ps(" + F(a[1].value) + ", " + shuffles[*z] + "), " + std::to_string(*mask & 0xF) + ")");
        }};
        // (each lane by the low bits of the same lane of vB, as XenonRecomp's loops: the AVX2 forms are not in the runtime's SIMDe headers)
        t["vectorShiftLeftIntegerWord"] = ishift(4, "uint32_t", "<<");
        t["vectorShiftRightWord"] = ishift(4, "uint32_t", ">>");
        t["vectorShiftRightAlgebraicWord"] = ishift(4, "int32_t", ">>");
        t["vectorShiftLeftIntegerHalfWord"] = ishift(2, "uint16_t", "<<");
        t["vectorShiftLeftIntegerByte"] = ishift(1, "uint8_t", "<<");
        t["vectorShiftRight"] = ibin("simde_mm_vsr");
        t["vectorShiftLeftByOctet"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("[&] { PPCVRegister x = " + P(a[0].value) + ", y = " + P(a[1].value) +
                                              ", d; const uint32_t sh = (y.u8[0] >> 3) & 0xF; for (uint32_t h = 0; h < 16; h++) d.u8[h] = h >= sh ? x.u8[h - sh] : 0; "
                                              "return __builtin_bit_cast(__uint128_t, d); }()");
        }};
        // ---- the D3D packs (XenonRecomp's helpers in ppc_context.h)
        t["vectorUnpackD3D"] = Form{2, false, [](const Args& a, const std::string&) -> std::optional<std::string> {
            const std::optional<std::uint64_t> imm = k(a, 1);
            if (!imm || (*imm >> 2) >= 6) return std::nullopt;
            return "[&] { PPCVRegister d; ppc_vupkd3d128(d, " + P(a[0].value) + ", " + std::to_string(*imm >> 2) + "); return __builtin_bit_cast(__uint128_t, d); }()";
        }};
        t["vectorPackD3D"] = Form{4, true, [](const Args& a, const std::string& output) -> std::optional<std::string> {
            const std::optional<std::uint64_t> type = k(a, 1), pack = k(a, 2), shift = k(a, 3);
            if (!type || !pack || !shift || *pack == 0 || *pack > 3 || *shift > 3 || *type >= 6 || output.empty()) return std::nullopt;
            return "[&] { PPCVRegister d = " + P(output) + "; ppc_vpkd3d128(d, " + P(a[0].value) + ", " + std::to_string(*type) + ", " + std::to_string(*pack) + ", " +
                   std::to_string(*shift) + "); return __builtin_bit_cast(__uint128_t, d); }()";
        }};
        // ---- the record forms' CR6: all elements true (LT), none (EQ)
        t["vectorCompareSetCR6"] = t["vectorRecordCR6"] = Form{1, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("uint8_t((((__uint128_t)(" + a[0].value + ") == ~(__uint128_t)0) << 3) | (((__uint128_t)(" + a[0].value + ") == 0) << 1))");
        }};
        // ---- the vector loads and stores of one address (VMX128's forms; AltiVec's (A, B) forms are declined: A there is r0, not 0)
        t["loadVectorLeftIndexed"] = Form{1, false, [](const Args& a, const std::string&) {
            const std::string e = ea32(a[0].value);
            return std::optional<std::string>(R("simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_HOST(" + e + " & ~0xFu)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(" +
                                                e + " & 0xF) * 16]))"));
        }};
        t["loadVectorRightIndexed"] = Form{1, false, [](const Args& a, const std::string&) {
            const std::string e = ea32(a[0].value);
            return std::optional<std::string>("(" + e + " & 0xF ? " + R("simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_HOST(" + e +
                                                                        " & ~0xFu)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(" + e + " & 0xF) * 16]))") +
                                              " : (__uint128_t)0)");
        }};
        t["loadVectorElementWordIndexed"] = Form{1, false, [](const Args& a, const std::string&) {   // (lvewx loads the aligned quadword, as lvx)
            return std::optional<std::string>(R("simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_HOST(" + ea32(a[0].value) +
                                                " & ~0xFu)), simde_mm_load_si128((simde__m128i*)VectorMaskL))"));
        }};
        t["loadVectorForShiftRight"] = Form{1, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_load_si128((simde__m128i*)&VectorShiftTableR[(" + ea32(a[0].value) + " & 0xF) * 16])"));
        }};
        t["storeVectorElementHalfWordIndexed"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("{ const uint32_t e = " + ea32(a[0].value) + " & ~0x1u; const PPCVRegister s = " + P(a[1].value) +
                                              "; PPC_STORE_U16(e, s.u16[7 - ((e & 0xF) >> 1)]); }");
        }, true};
        t["storeVectorElementByteIndexed"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("{ const uint32_t e = " + ea32(a[0].value) + "; const PPCVRegister s = " + P(a[1].value) +
                                              "; PPC_STORE_U8(e, s.u8[15 - (e & 0xF)]); }");
        }, true};
        t["loadVectorForShiftLeft"] = Form{1, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>(R("simde_mm_load_si128((simde__m128i*)&VectorShiftTableL[(" + ea32(a[0].value) + " & 0xF) * 16])"));
        }};
        t["storeVectorElementWordIndexed"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("{ const uint32_t e = " + ea32(a[0].value) + " & ~0x3u; const PPCVRegister s = " + P(a[1].value) +
                                              "; PPC_STORE_U32(e, s.u32[3 - ((e & 0xF) >> 2)]); }");
        }, true};
        t["storeVectorLeftIndexed"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("{ const uint32_t e = " + ea32(a[0].value) + "; const PPCVRegister s = " + P(a[1].value) +
                                              "; for (size_t i = 0; i < (16 - (e & 0xF)); i++) PPC_STORE_U8(e + i, s.u8[15 - i]); }");
        }, true};
        t["storeVectorRightIndexed"] = Form{2, false, [](const Args& a, const std::string&) {
            return std::optional<std::string>("{ const uint32_t e = " + ea32(a[0].value) + "; const PPCVRegister s = " + P(a[1].value) +
                                              "; for (size_t i = 0; i < (e & 0xF); i++) PPC_STORE_U8(e - i - 1, s.u8[i]); }");
        }, true};
        return t;
    }();
    return table;
}

}  // namespace

bool known(std::string_view name) { return forms().contains(name); }

std::optional<Written> emit(std::string_view name, const std::vector<Arg>& args, const std::string& output, std::string* why) {
    const auto it = forms().find(name);
    if (it == forms().end()) {
        if (why) *why = "the user operation " + std::string(name);
        return std::nullopt;
    }
    const Form& form = it->second;
    if (args.size() != form.inputs) {
        if (why) *why = "the user operation " + std::string(name) + " with " + std::to_string(args.size()) + " inputs";
        return std::nullopt;
    }
    std::optional<std::string> text = form.write(args, output);
    if (!text) {
        if (why) *why = "the user operation " + std::string(name) + " with these operands";
        return std::nullopt;
    }
    return Written{std::move(*text), form.statement, form.vmx};
}

}  // namespace cb::recompile::pcode_emit::vmx
