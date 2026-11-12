#include "cb/recompile/builders/vector.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <format>
#include <string>

#include "cb/core/error.hpp"
#include "cb/recompile/builders/reading.hpp"

namespace cb::recompile::builders::vector {
namespace {

using Statements = std::vector<std::string>;

constexpr const char* kFlush = "ctx.fpscr.enableFlushMode();";

// "1 << u" as decimal text, for the scale of a float-to-integer conversion (u is a 5-bit field in real code).
std::string power_of_two_text(const Instruction& insn, std::int64_t u) {
    if (u < 0 || u > 62) throw ParseError(insn.mnemonic, 0, 3, "scale " + std::to_string(u) + " is not in 0..62");
    return std::to_string(std::int64_t{1} << u);
}

// The bits of the single-precision float 2^-u (the scale of an integer-to-float conversion).
std::uint32_t scale_bits(const Instruction& insn, std::int64_t u) {
    if (u < 0 || u > 126) throw ParseError(insn.mnemonic, 0, 3, "scale " + std::to_string(u) + " is not in 0..126");
    const float scale = std::ldexp(1.0f, -static_cast<int>(u));
    std::uint32_t bits;
    std::memcpy(&bits, &scale, sizeof bits);
    return bits;
}

// vD = op(vA, vB) on four floats
std::string float_binary(std::string_view op, int d, int a, int b) {
    return std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_{}_ps(simde_mm_load_ps(ctx.v{}.f32), simde_mm_load_ps(ctx.v{}.f32)));", d, op, a, b);
}

// vD = op(vA, vB) on integer lanes of type `lane` (callers that swap the operands pass b, a)
std::string int_binary(std::string_view op, std::string_view lane, int d, int a, int b) {
    return std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.{}, simde_mm_{}("
                       "simde_mm_load_si128((simde__m128i*)ctx.v{}.{}), "
                       "simde_mm_load_si128((simde__m128i*)ctx.v{}.{})));",
                       d, lane, op, a, lane, b, lane);
}

// vD = rex::helper(vA, vB), all three as `lane`
std::string runtime_binary(std::string_view lane, std::string_view helper, int d, int a, int b) {
    return std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.{}, rex::{}("
                       "simde_mm_load_si128((simde__m128i*)ctx.v{}.{}), "
                       "simde_mm_load_si128((simde__m128i*)ctx.v{}.{})));",
                       d, lane, helper, a, lane, b, lane);
}

// the ctx.cr6.setFromMask(...) statement of a record-form integer compare
std::string cr6_from_integer(int d, std::string_view lane, std::string_view mask) {
    return std::format("ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v{}.{}), {});", d, lane, mask);
}

// ---------------------------------------------------------------------------
// Unpack (simple, si128) helpers
// ---------------------------------------------------------------------------
Statements unpack_high_bytes(int d, int a) {
    Statements o;
    o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.s16, simde_mm_cvtepi8_epi16("
        "simde_mm_unpackhi_epi64(simde_mm_load_si128((simde__m128i*)ctx.v{}.s8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s8))));", d, a, a));
    return o;
}

Statements unpack_high_halves(int d, int a) {
    Statements o;
    o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.s32, simde_mm_cvtepi16_epi32("
        "simde_mm_unpackhi_epi64(simde_mm_load_si128((simde__m128i*)ctx.v{}.s16), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s16))));", d, a, a));
    return o;
}

Statements unpack_low_bytes(int d, int a) {
    Statements o;
    o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.s32, simde_mm_cvtepi8_epi16("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s16)));", d, a));
    return o;
}

Statements unpack_low_halves(int d, int a) {
    Statements o;
    o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.s32, simde_mm_cvtepi16_epi32("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s16)));", d, a));
    return o;
}

// ---------------------------------------------------------------------------
// vpkd3d128 : D3D pack (scalar). operands[0]=vD, [1]=vB(src), [2]=type,
//             [3]=mask, [4]=shift.  Emits "ctx.fpscr.enableFlushMode();" first.
// ---------------------------------------------------------------------------
Statements pack_d3d(const Instruction& insn, const Operands& ops) {
    const int d = vpr_at(insn, ops, 0);
    const int s = vpr_at(insn, ops, 1);
    const std::int64_t typ = imm_at(insn, ops, 2);
    Statements out;
    out.push_back(kFlush);
    auto is_typ = [&](std::int64_t v) { return typ == v; };

    if (is_typ(0)) {  // D3D color
        const std::int64_t mask = imm_at(insn, ops, 3);
        const std::int64_t shift = imm_at(insn, ops, 4);
        const std::int64_t indices[4] = {3, 0, 1, 2};
        for (std::int64_t i = 0; i < 4; ++i) {
            out.push_back(std::format("vTemp.u32[{}] = 0x404000FF;", i));
            out.push_back(std::format("vTemp.f32[{}] = !(ctx.v{}.f32[{}] >= 3.0f) ? 3.0f : (ctx.v{}.f32[{}] > "
        "vTemp.f32[{}] ? vTemp.f32[{}] : ctx.v{}.f32[{}]);", i, s, i, s, i, i, i, s, i));
            out.push_back(std::format("temp.u32 {}= uint32_t(vTemp.u8[{}]) << {};", i == 0 ? "" : "|", i * 4, indices[i] * 8));
        }
        if (mask == 3 && shift == 3) {
            out.push_back(std::format("ctx.v{}.u32[0] = 0;", d));
        } else {
            out.push_back(std::format("ctx.v{}.u32[{}] = temp.u32;", d, shift));
            if (mask >= 2 && shift < 3) out.push_back(std::format("ctx.v{}.u32[{}] = 0;", d, shift + 1));
        }
        return out;
    }

    if (is_typ(1)) {  // NORMSHORT2
        const std::int64_t sh = imm_at(insn, ops, 4);
        out.push_back(std::format("temp.s32 = ctx.v{}.s32[3] - 0x40400000;", s));
        out.push_back("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);");
        out.push_back("vTemp.u32[0] = uint32_t(uint16_t(temp.s32)) << 16;");
        out.push_back(std::format("temp.s32 = ctx.v{}.s32[2] - 0x40400000;", s));
        out.push_back("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);");
        out.push_back("vTemp.u32[0] |= uint16_t(temp.s32);");
        out.push_back(std::format("ctx.v{}.u32[{}] = vTemp.u32[0];", d, sh));
        return out;
    }

    if (is_typ(2)) {  // NORMPACKED32 (2:10:10:10)
        const std::int64_t sh = imm_at(insn, ops, 4);
        out.push_back("vTemp.u32[0] = 0;");
        out.push_back(std::format("temp.f32 = ctx.v{}.f32[3] < kPack2101010_Min10 ? kPack2101010_Min10 : "
        "(ctx.v{}.f32[3] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v{}.f32[3]);", s, s, s));
        out.push_back("vTemp.u32[0] = temp.u32 & 0x3FF;");
        out.push_back(std::format("temp.f32 = ctx.v{}.f32[2] < kPack2101010_Min10 ? kPack2101010_Min10 : "
        "(ctx.v{}.f32[2] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v{}.f32[2]);", s, s, s));
        out.push_back("vTemp.u32[0] |= (temp.u32 & 0x3FF) << 10;");
        out.push_back(std::format("temp.f32 = ctx.v{}.f32[1] < kPack2101010_Min10 ? kPack2101010_Min10 : "
        "(ctx.v{}.f32[1] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v{}.f32[1]);", s, s, s));
        out.push_back("vTemp.u32[0] |= (temp.u32 & 0x3FF) << 20;");
        out.push_back(std::format("temp.f32 = ctx.v{}.f32[0] < kPack2101010_Min2 ? kPack2101010_Min2 : "
        "(ctx.v{}.f32[0] > kPack2101010_Max2 ? kPack2101010_Max2 : ctx.v{}.f32[0]);", s, s, s));
        out.push_back("vTemp.u32[0] |= (temp.u32 & 0x3) << 30;");
        out.push_back(std::format("ctx.v{}.u32[{}] = vTemp.u32[0];", d, sh));
        return out;
    }

    if (is_typ(3) || is_typ(5)) {  // FLOAT16_2 (case 3) / FLOAT16_4 (case 5)
        const bool two = is_typ(3);
        std::int64_t n = two ? 2 : 4;
        const std::int64_t sh = imm_at(insn, ops, 4);
        for (std::int64_t i = 0; i < n; ++i) {
            std::int64_t src = 3 - i;
            std::int64_t dst = two ? (1 - i) + (2 * sh) : (3 - i) + (2 * sh);
            out.push_back(std::format("temp.u32 = (ctx.v{}.u32[{}]&0x7FFFFFFF);", s, src));
            out.push_back(std::format("vTemp.u8[0] = (temp.f32 != temp.f32) || (temp.f32 > 65504.0f) ? 0xFF : "
        "((ctx.v{}.u32[{}]&0x7f800000)>>23);", s, src));
            out.push_back(std::format("temp.u16 = vTemp.u8[0] != 0xFF ? ((ctx.v{}.u32[{}]&0x7FE000)>>13) : 0x0;", s, src));
            out.push_back(std::format("ctx.v{}.u16[{}] = vTemp.u8[0] != 0xFF ? (vTemp.u8[0] > 0x70 ? "
        "(((vTemp.u8[0]-0x70)<<10)+temp.u16) : (0x71-vTemp.u8[0] > 31 ? 0x0 : "
        "((0x400+temp.u16)>>(0x71-vTemp.u8[0])))) : 0x7FFF;", d, dst));
            out.push_back(std::format("ctx.v{}.u16[{}] |= ((ctx.v{}.u32[{}]&0x80000000)>>16);", d, dst, s, src));
        }
        return out;
    }

    if (is_typ(4)) {  // NORMSHORT4
        const std::int64_t sh = imm_at(insn, ops, 4);
        for (std::int64_t i = 0; i < 4; ++i) {
            std::int64_t src = 3 - i;
            std::int64_t dst = (3 - i) + (2 * sh);
            out.push_back(std::format("temp.s32 = ctx.v{}.s32[{}] - 0x40400000;", s, src));
            out.push_back("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);");
            out.push_back(std::format("ctx.v{}.u16[{}] = uint16_t(temp.s32);", d, dst));
        }
        return out;
    }

    if (is_typ(6)) {  // NORMPACKED64 (4:20:20:20)
        const std::int64_t sh = imm_at(insn, ops, 4);
        out.push_back("vTemp.u64[0] = 0;");
        out.push_back(std::format("temp.s32 = int32_t(ctx.v{}.f32[0]);", s));
        out.push_back("vTemp.u64[0] = uint64_t(temp.s32 & 0xFFFFF);");
        out.push_back(std::format("temp.s32 = int32_t(ctx.v{}.f32[1]);", s));
        out.push_back("vTemp.u64[0] |= uint64_t(temp.s32 & 0xFFFFF) << 20;");
        out.push_back(std::format("temp.s32 = int32_t(ctx.v{}.f32[2]);", s));
        out.push_back("vTemp.u64[0] |= uint64_t(temp.s32 & 0xFFFFF) << 40;");
        out.push_back(std::format("temp.s32 = int32_t(ctx.v{}.f32[3]);", s));
        out.push_back("vTemp.u64[0] |= uint64_t(temp.s32 & 0xF) << 60;");
        out.push_back(std::format("ctx.v{}.u64[{}] = vTemp.u64[0];", d, sh >> 1));
        return out;
    }

    out.push_back("__builtin_debugtrap();");
    return out;
}

// ---------------------------------------------------------------------------
// vupkd3d128 : D3D unpack (scalar). operands[0]=vD, [1]=vB(src), [2]=type.
//              switch is on (operands[2] >> 2).
// ---------------------------------------------------------------------------
Statements unpack_d3d(const Instruction& insn, const Operands& ops) {
    const int d = vpr_at(insn, ops, 0);
    const int s = vpr_at(insn, ops, 1);
    const std::int64_t sel = imm_at(insn, ops, 2) >> 2;
    Statements out;

    if (sel == 0) {  // D3D color
        const std::int64_t indices[4] = {3, 0, 1, 2};
        for (std::int64_t i = 0; i < 4; ++i)
            out.push_back(std::format("vTemp.u32[{}] = ctx.v{}.u8[{}] | 0x3F800000;", i, s, indices[i]));
        out.push_back(std::format("ctx.v{} = vTemp;", d));
        return out;
    }

    if (sel == 1) {  // NORMSHORT2
        for (std::int64_t i = 0; i < 2; ++i) {
            out.push_back("temp.f32 = 3.0f;");
            out.push_back(std::format("temp.s32 += ctx.v{}.s16[{}];", s, 1 - i));
            out.push_back(std::format("vTemp.f32[{}] = temp.f32;", 3 - i));
        }
        out.push_back("vTemp.f32[1] = 0.0f;");
        out.push_back("vTemp.f32[0] = 1.0f;");
        out.push_back(std::format("ctx.v{} = vTemp;", d));
        return out;
    }

    if (sel == 2) {  // NORMPACKED32
        out.push_back(std::format("temp.s32 = (ctx.v{}.s32[0] << 22) >> 22;", s));
        out.push_back("vTemp.s32[0] = temp.s32 == -512 ? 0x7FC00000 : (temp.s32 + 0x40400000);");
        out.push_back(std::format("ctx.v{}.u32[3] = vTemp.u32[0];", d));
        out.push_back(std::format("temp.s32 = (ctx.v{}.s32[0] << 12) >> 22;", s));
        out.push_back("vTemp.s32[0] = temp.s32 + 0x40400000;");
        out.push_back(std::format("ctx.v{}.u32[2] = vTemp.u32[0];", d));
        out.push_back(std::format("temp.s32 = (ctx.v{}.s32[0] << 2) >> 22;", s));
        out.push_back("vTemp.s32[0] = temp.s32 + 0x40400000;");
        out.push_back(std::format("ctx.v{}.u32[1] = vTemp.u32[0];", d));
        out.push_back(std::format("vTemp.u32[0] = (ctx.v{}.u32[0] >> 30) | 0x3F800000;", s));
        out.push_back(std::format("ctx.v{}.u32[0] = vTemp.u32[0];", d));
        return out;
    }

    if (sel == 3) {  // FLOAT16_2
        for (std::int64_t i = 0; i < 2; ++i) {
            std::int64_t src = 1 - i;
            std::int64_t dst = 3 - i;
            out.push_back(std::format("temp.u32 = ctx.v{}.u16[{}];", s, src));
            out.push_back("vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | "
                    "(((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);");
            out.push_back("if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;");
            out.push_back(std::format("ctx.v{}.u32[{}] = vTemp.u32[0];", d, dst));
        }
        out.push_back(std::format("ctx.v{}.f32[1] = 0.0f;", d));
        out.push_back(std::format("ctx.v{}.f32[0] = 1.0f;", d));
        return out;
    }

    if (sel == 4) {  // NORMSHORT4
        for (std::int64_t i = 0; i < 4; ++i) {
            std::int64_t src = 3 - i;
            std::int64_t dst = 3 - i;
            out.push_back("temp.f32 = 3.0f;");
            out.push_back(std::format("temp.s32 += ctx.v{}.s16[{}];", s, src));
            out.push_back(std::format("ctx.v{}.f32[{}] = temp.f32;", d, dst));
        }
        return out;
    }

    if (sel == 5) {  // FLOAT16_4
        for (std::int64_t i = 0; i < 4; ++i) {
            std::int64_t src = 3 - i;
            std::int64_t dst = 3 - i;
            out.push_back(std::format("temp.u32 = ctx.v{}.u16[{}];", s, src));
            out.push_back("vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | "
                    "(((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);");
            out.push_back("if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;");
            out.push_back(std::format("ctx.v{}.u32[{}] = vTemp.u32[0];", d, dst));
        }
        return out;
    }

    if (sel == 6) {  // NORMPACKED64
        out.push_back(std::format("vTemp.u64[0] = ctx.v{}.u64[1];", s));
        out.push_back("temp.s32 = (int32_t(vTemp.u64[0] << 44) >> 44);");
        out.push_back(std::format("ctx.v{}.f32[0] = float(temp.s32);", d));
        out.push_back("temp.s32 = (int32_t(vTemp.u64[0] << 24) >> 44);");
        out.push_back(std::format("ctx.v{}.f32[1] = float(temp.s32);", d));
        out.push_back("temp.s32 = (int32_t(vTemp.u64[0] << 4) >> 44);");
        out.push_back(std::format("ctx.v{}.f32[2] = float(temp.s32);", d));
        out.push_back(std::format("ctx.v{}.f32[3] = float(vTemp.u64[0] >> 60);", d));
        return out;
    }

    out.push_back("__builtin_debugtrap();");
    return out;
}
constexpr auto kMnemonics = std::to_array<std::string_view>({
    // FP arithmetic
    "vaddfp", "vaddfp128", "vsubfp", "vsubfp128", "vmulfp128",
    "vmaddfp", "vmaddfp128", "vmaddcfp128", "vnmsubfp", "vnmsubfp128",
    "vmaxfp", "vmaxfp128", "vminfp", "vminfp128",
    "vrefp", "vrefp128", "vrsqrtefp", "vrsqrtefp128",
    "vexptefp", "vexptefp128", "vlogefp", "vlogefp128",
    // dot products
    "vmsum3fp128", "vmsum4fp128",
    // rounding
    "vrfim", "vrfim128", "vrfin", "vrfin128",
    "vrfip", "vrfip128", "vrfiz", "vrfiz128",
    // integer arithmetic / saturating
    "vaddsbs", "vaddshs", "vaddsws", "vaddubm", "vaddubs", "vadduhm",
    "vadduwm", "vadduws", "vadduhs",
    "vsubsbs", "vsubsws", "vsububm", "vsububs", "vsubuhs", "vsubuws",
    "vsubuhm", "vsubuwm", "vsubshs",
    "vmaxsw", "vmaxsh", "vmaxsb", "vminsh", "vminsb", "vminsw",
    "vmaxuh", "vminuh", "vmaxub", "vminub",
    // average
    "vavgsb", "vavgsh", "vavgsw", "vavgub", "vavguh",
    // logical
    "vand", "vand128", "vandc", "vandc128", "vor", "vor128",
    "vxor", "vxor128", "vnor", "vnor128", "vsel", "vsel128",
    // compare
    "vcmpbfp", "vcmpbfp128", "vcmpeqfp", "vcmpeqfp128",
    "vcmpequb", "vcmpequh", "vcmpequw", "vcmpequw128",
    "vcmpgefp", "vcmpgefp128", "vcmpgtfp", "vcmpgtfp128",
    "vcmpgtub", "vcmpgtuh", "vcmpgtuw",
    "vcmpgtsb", "vcmpgtsh", "vcmpgtsw",
    // conversion
    "vctsxs", "vcfpsxws128", "vctuxs", "vcfpuxws128",
    "vcfsx", "vcsxwfp128", "vcfux", "vcuxwfp128",
    // merge
    "vmrghb", "vmrghh", "vmrghw", "vmrghw128",
    "vmrglb", "vmrglh", "vmrglw", "vmrglw128",
    // permute
    "vperm", "vperm128", "vpermwi128", "vrlimi128",
    // shift / rotate
    "vsl", "vslb", "vslh", "vsldoi", "vsldoi128", "vslw", "vslw128",
    "vslo", "vslo128", "vsr", "vsrh", "vsrb", "vsrab", "vsrah",
    "vsraw", "vsraw128", "vsrw", "vsrw128", "vsro", "vsro128",
    "vrlh", "vrlw", "vrlw128",
    // splat
    "vspltb", "vsplth", "vspltisb", "vspltish", "vspltisw", "vspltisw128",
    "vspltw", "vspltw128",
    // pack
    "vpkshus", "vpkshus128", "vpkuhum", "vpkuhum128", "vpkuhus", "vpkuhus128",
    "vpkuwum", "vpkuwum128", "vpkuwus", "vpkuwus128",
    "vpkshss", "vpkshss128", "vpkswss", "vpkswss128", "vpkswus", "vpkswus128",
    "vpkd3d128",
    // unpack
    "vupkd3d128",
    "vupkhsb", "vupkhsb128", "vupkhsh", "vupkhsh128",
    "vupklsb", "vupklsb128", "vupklsh", "vupklsh128",
});

}  // namespace

std::optional<Statements> emit(const Instruction& insn, const Operands& ops) {
    // a record form is a mnemonic ending in '.'
    const bool rec = insn.mnemonic.ends_with('.');
    const std::string_view core = rec ? insn.mnemonic.substr(0, insn.mnemonic.size() - 1) : insn.mnemonic;
    auto is = [&](std::initializer_list<std::string_view> names) { return std::ranges::find(names, core) != names.end(); };

    // =======================================================================
    // Vector Floating Point Arithmetic
    // =======================================================================
    if (is({"vaddfp", "vaddfp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(kFlush); o.push_back(float_binary("add", d, a, b));
        return o;
    }
    if (is({"vsubfp", "vsubfp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(kFlush); o.push_back(float_binary("sub", d, a, b));
        return o;
    }
    if (core == "vmulfp128") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(kFlush); o.push_back(float_binary("mul", d, a, b));
        return o;
    }
    if (is({"vmaddfp", "vmaddfp128", "vmaddcfp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), c = vpr_at(insn, ops, 2), b = vpr_at(insn, ops, 3);
        Statements o; o.push_back(kFlush);
        o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_add_ps(simde_mm_mul_ps("
        "simde_mm_load_ps(ctx.v{}.f32), simde_mm_load_ps(ctx.v{}.f32)), "
        "simde_mm_load_ps(ctx.v{}.f32)));", d, a, c, b));
        return o;
    }
    if (is({"vnmsubfp", "vnmsubfp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), c = vpr_at(insn, ops, 2), b = vpr_at(insn, ops, 3);
        Statements o; o.push_back(kFlush);
        o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_xor_ps(simde_mm_sub_ps("
        "simde_mm_mul_ps(simde_mm_load_ps(ctx.v{}.f32), simde_mm_load_ps(ctx.v{}.f32)), "
        "simde_mm_load_ps(ctx.v{}.f32)), "
        "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));", d, a, c, b));
        return o;
    }
    if (is({"vmaxfp", "vmaxfp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(kFlush); o.push_back(float_binary("max", d, a, b));
        return o;
    }
    if (is({"vminfp", "vminfp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(kFlush); o.push_back(float_binary("min", d, a, b));
        return o;
    }
    if (is({"vrefp", "vrefp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        Statements o; o.push_back(kFlush);
        o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_div_ps(simde_mm_set1_ps(1), "
        "simde_mm_load_ps(ctx.v{}.f32)));", d, a));
        return o;
    }
    if (is({"vrsqrtefp", "vrsqrtefp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        Statements o; o.push_back(kFlush);
        o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_div_ps(simde_mm_set1_ps(1), "
        "simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v{}.f32))));", d, a));
        return o;
    }
    if (is({"vexptefp", "vexptefp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        Statements o; o.push_back(kFlush);
        for (std::int64_t i = 0; i < 4; ++i) o.push_back(std::format("ctx.v{}.f32[{}] = exp2f(ctx.v{}.f32[{}]);", d, i, a, i));
        return o;
    }
    if (is({"vlogefp", "vlogefp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        Statements o; o.push_back(kFlush);
        for (std::int64_t i = 0; i < 4; ++i) o.push_back(std::format("ctx.v{}.f32[{}] = log2f(ctx.v{}.f32[{}]);", d, i, a, i));
        return o;
    }

    // =======================================================================
    // Vector Dot Products (VMX128)
    // =======================================================================
    if (core == "vmsum3fp128" || core == "vmsum4fp128") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(kFlush);
        o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_dp_ps("
        "simde_mm_load_ps(ctx.v{}.f32), simde_mm_load_ps(ctx.v{}.f32), {}));", d, a, b, core == "vmsum3fp128" ? "0xEF" : "0xFF"));
        return o;
    }

    // =======================================================================
    // Vector Rounding
    // =======================================================================
    {
        const std::pair<const char*, const char*> round_modes[] = {
            {"vrfim", "SIMDE_MM_FROUND_TO_NEG_INF | SIMDE_MM_FROUND_NO_EXC"},
            {"vrfin", "SIMDE_MM_FROUND_TO_NEAREST_INT | SIMDE_MM_FROUND_NO_EXC"},
            {"vrfip", "SIMDE_MM_FROUND_TO_POS_INF | SIMDE_MM_FROUND_NO_EXC"},
            {"vrfiz", "SIMDE_MM_FROUND_TO_ZERO | SIMDE_MM_FROUND_NO_EXC"},
        };
        // (the "128" alias maps to its base form)
        const bool alias = core.size() > 3 && core.compare(core.size() - 3, 3, "128") == 0;
        for (const auto& e : round_modes) {
            if (core == e.first || (alias && core.substr(0, core.size() - 3) == e.first)) {
                const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
                Statements o; o.push_back(kFlush);
                o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_round_ps(simde_mm_load_ps(ctx.v{}.f32), {}));", d, a, e.second));
                return o;
            }
        }
    }

    // =======================================================================
    // Vector Integer Arithmetic
    // =======================================================================
    {
        // mnemonic -> (simde op, lane type)
        static const std::tuple<const char*, const char*, const char*> intbin[] = {
            {"vaddsbs", "adds_epi8", "s8"},  {"vaddshs", "adds_epi16", "s16"},
            {"vaddubm", "add_epi8", "u8"},   {"vaddubs", "adds_epu8", "u8"},
            {"vadduhm", "add_epi16", "u16"}, {"vadduwm", "add_epi32", "u32"},
            {"vadduhs", "adds_epu16", "u16"},
            {"vsubsbs", "subs_epi8", "s8"},  {"vsububm", "sub_epi8", "u8"},
            {"vsububs", "subs_epu8", "u8"},  {"vsubuhs", "subs_epu16", "u16"},
            {"vsubuhm", "sub_epi16", "u16"}, {"vsubuwm", "sub_epi32", "u32"},
            {"vsubshs", "subs_epi16", "s16"},
            {"vmaxsw", "max_epi32", "s32"},  {"vmaxsh", "max_epi16", "s16"},
            {"vmaxsb", "max_epi8", "s8"},    {"vminsh", "min_epi16", "s16"},
            {"vminsb", "min_epi8", "s8"},    {"vminsw", "min_epi32", "s32"},
            {"vmaxuh", "max_epu16", "u16"},  {"vminuh", "min_epu16", "u16"},
            {"vmaxub", "max_epu8", "u8"},    {"vminub", "min_epu8", "u8"},
            {"vavgub", "avg_epu8", "u8"},    {"vavguh", "avg_epu16", "u16"},
        };
        for (const auto& e : intbin) {
            if (core == std::get<0>(e)) {
                const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
                Statements o; o.push_back(int_binary(std::get<1>(e), std::get<2>(e), d, a, b));
                return o;
            }
        }
    }

    if (core == "vaddsws") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back("{");
        o.push_back(std::format("simde__m128i a = simde_mm_load_si128((simde__m128i*)ctx.v{}.u8);", a));
        o.push_back(std::format("simde__m128i b = simde_mm_load_si128((simde__m128i*)ctx.v{}.u8);", b));
        o.push_back("simde__m128i sum = simde_mm_add_epi32(a, b);");
        o.push_back("simde__m128i overflow = simde_mm_and_si128(simde_mm_xor_si128(a, sum), "
              "simde_mm_xor_si128(b, sum));");
        o.push_back("simde__m128i sat_val = simde_mm_xor_si128(simde_mm_srai_epi32(a, 31), "
              "simde_mm_set1_epi32(0x7FFFFFFF));");
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_blendv_epi8("
        "sum, sat_val, overflow));", d));
        o.push_back("}");
        return o;
    }

    if (core == "vadduws") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u32, rex::simde_mm_adds_epu32("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32)));", d, a, b));
        return o;
    }

    if (core == "vsubsws") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        for (std::int64_t i = 0; i < 4; ++i) {
            o.push_back(std::format("temp.s64 = int64_t(ctx.v{}.s32[{}]) - int64_t(ctx.v{}.s32[{}]);", a, i, b, i));
            o.push_back(std::format("ctx.v{}.s32[{}] = temp.s64 > INT_MAX ? INT_MAX : "
        "temp.s64 < INT_MIN ? INT_MIN : temp.s64;", d, i));
        }
        return o;
    }

    if (core == "vsubuws") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u32, simde_mm_sub_epi32("
        "simde_mm_load_si128((simde__m128i*) ctx.v{}.u32), "
        "simde_mm_min_epu32(simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32))));", d, a, a, b));
        return o;
    }

    // =======================================================================
    // Vector Average (rex helpers for the signed forms)
    // =======================================================================
    if (core == "vavgsb") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(runtime_binary("u8", "simde_mm_avg_epi8", d, a, b));
        return o;
    }
    if (core == "vavgsh") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(runtime_binary("u16", "simde_mm_avg_epi16", d, a, b));
        return o;
    }
    if (core == "vavgsw") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(runtime_binary("s32", "simde_mm_avg_epi32", d, a, b));
        return o;
    }

    // =======================================================================
    // Vector Logical
    // =======================================================================
    if (is({"vand", "vand128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(int_binary("and_si128", "u8", d, a, b));
        return o;
    }
    if (is({"vandc", "vandc128"})) {
        // vD = vA & ~vB : emit_vec_int_binary_swapped(andnot_si128, u8) -> op(vB, vA)
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(int_binary("andnot_si128", "u8", d, b, a));
        return o;
    }
    if (is({"vor", "vor128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        std::string body;
        if (a != b) {
            body = std::format("simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8)));", a, b);
        } else {
            body = std::format("simde_mm_load_si128((simde__m128i*)ctx.v{}.u8));", a);
        }
        Statements o; o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, {}", d, body));
        return o;
    }
    if (is({"vxor", "vxor128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        std::string body;
        if (a != b) {
            body = std::format("simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8)));", a, b);
        } else {
            body = "simde_mm_setzero_si128());";
        }
        Statements o; o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, {}", d, body));
        return o;
    }
    if (is({"vnor", "vnor128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_xor_si128("
        "simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8)), "
        "simde_mm_set1_epi32(-1)));", d, a, b));
        return o;
    }
    if (is({"vsel", "vsel128"})) {
        const int d = vpr_at(insn, ops, 0);
        const int a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2), c = vpr_at(insn, ops, 3);  // operands: vD, vA, vB, vC(mask)
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_or_si128("
        "simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8)), "
        "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8))));", d, c, a, c, b));
        return o;
    }

    // =======================================================================
    // Vector Compare (record forms update CR6 via setFromMask)
    // =======================================================================
    if (is({"vcmpbfp", "vcmpbfp128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(kFlush);
        o.push_back(std::format("simde_mm_store_ps(vTemp.f32, simde_mm_and_ps(simde_mm_cmpgt_ps("
        "simde_mm_load_ps(ctx.v{}.f32), simde_mm_load_ps(ctx.v{}.f32)), "
        "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));", a, b));
        o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_and_ps(simde_mm_cmplt_ps("
        "simde_mm_load_ps(ctx.v{}.f32), simde_mm_xor_ps(simde_mm_load_ps(ctx.v{}.f32), "
        "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))))), "
        "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x40000000)))));", d, a, b));
        o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_or_ps(simde_mm_load_ps(vTemp.f32), "
        "simde_mm_load_ps(ctx.v{}.f32)));", d, d));
        if (rec) {
            o.push_back(std::format("ctx.cr6.setFromMask(simde_mm_castsi128_ps(simde_mm_or_si128("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.f32), "
        "simde_mm_slli_epi32(simde_mm_load_si128((simde__m128i*)ctx.v{}.f32), 1))), 0xF);", d, d));
        }
        return o;
    }

    {
        const std::pair<const char*, const char*> fcmp[] = {
            {"vcmpeqfp", "cmpeq"}, {"vcmpeqfp128", "cmpeq"}, {"vcmpgefp", "cmpge"}, {"vcmpgefp128", "cmpge"},
            {"vcmpgtfp", "cmpgt"}, {"vcmpgtfp128", "cmpgt"},
        };
        for (const auto& e : fcmp) {
            if (core == e.first) {
                const int d = vpr_at(insn, ops, 0);
                const int a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
                Statements o; o.push_back(kFlush); o.push_back(float_binary(e.second, d, a, b));
                if (rec) o.push_back(std::format("ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v{}.f32), 0xF);", d));
                return o;
            }
        }
    }

    if (core == "vcmpequb") {
        const int d = vpr_at(insn, ops, 0);
        const int a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(int_binary("cmpeq_epi8", "u8", d, a, b));
        if (rec) o.push_back(cr6_from_integer(d, "u8", "0xFFFF"));
        return o;
    }
    if (core == "vcmpequh") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_cmpeq_epi16("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u16), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u16)));", d, a, b));
        if (rec) o.push_back(cr6_from_integer(d, "u16", "0xFFFF"));
        return o;
    }
    if (is({"vcmpequw", "vcmpequw128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_cmpeq_epi32("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32)));", d, a, b));
        if (rec) o.push_back(std::format("ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v{}.f32), 0xF);", d));
        return o;
    }
    if (core == "vcmpgtub") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, rex::simde_mm_cmpgt_epu8("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8)));", d, a, b));
        if (rec) o.push_back(cr6_from_integer(d, "u8", "0xFFFF"));
        return o;
    }
    if (core == "vcmpgtuh") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, rex::simde_mm_cmpgt_epu16("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u16), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u16)));", d, a, b));
        if (rec) o.push_back(cr6_from_integer(d, "u16", "0xFFFF"));
        return o;
    }
    if (core == "vcmpgtuw") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u32, simde_mm_cmpgt_epi32("
        "simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), "
        "simde_mm_set1_epi32((int32_t)0x80000000)), "
        "simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), "
        "simde_mm_set1_epi32((int32_t)0x80000000))));", d, a, b));
        if (rec) o.push_back(std::format("ctx.cr6.setFromMask(simde_mm_castsi128_ps("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32)), 0xF);", d));
        return o;
    }
    if (core == "vcmpgtsb") {
        const int d = vpr_at(insn, ops, 0);
        const int a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(int_binary("cmpgt_epi8", "u8", d, a, b));
        if (rec) o.push_back(cr6_from_integer(d, "u8", "0xFFFF"));
        return o;
    }
    if (core == "vcmpgtsh") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_cmpgt_epi16("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u16), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u16)));", d, a, b));
        if (rec) o.push_back(cr6_from_integer(d, "u16", "0xFFFF"));
        return o;
    }
    if (core == "vcmpgtsw") {
        const int d = vpr_at(insn, ops, 0);
        const int a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o; o.push_back(int_binary("cmpgt_epi32", "u32", d, a, b));
        if (rec) o.push_back(std::format("ctx.cr6.setFromMask(simde_mm_castsi128_ps("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32)), 0xF);", d));
        return o;
    }

    // =======================================================================
    // Vector Conversion
    // =======================================================================
    if (is({"vctsxs", "vcfpsxws128", "vctuxs", "vcfpuxws128"})) {
        const bool signed_form = core == "vctsxs" || core == "vcfpsxws128";
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        const std::int64_t u = imm_at(insn, ops, 2);
        std::string inner;
        if (u != 0) {
            std::string scale = power_of_two_text(insn, u);   // (1 << u)
            inner = std::format("simde_mm_mul_ps(simde_mm_load_ps(ctx.v{}.f32), simde_mm_set1_ps({}))", a, scale);
        } else {
            inner = std::format("simde_mm_load_ps(ctx.v{}.f32)", a);
        }
        Statements o; o.push_back(kFlush);
        if (signed_form)
            o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.s32, rex::simde_mm_vctsxs({}));", d, inner));
        else
            o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u32, rex::simde_mm_vctuxs({}));", d, inner));
        return o;
    }
    if (is({"vcfsx", "vcsxwfp128", "vcfux", "vcuxwfp128"})) {
        const bool signed_form = core == "vcfsx" || core == "vcsxwfp128";
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        const std::int64_t u = imm_at(insn, ops, 2);
        Statements o; o.push_back(kFlush);
        if (u != 0) {
            std::int64_t bits = scale_bits(insn, u);
            if (signed_form)
                o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32)), "
        "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x{:X})))));", d, a, bits));
            else
                o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_mul_ps(rex::simde_mm_cvtepu32_ps_("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32)), "
        "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x{:X})))));", d, a, bits));
        } else {
            if (signed_form)
                o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_cvtepi32_ps("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32)));", d, a));
            else
                o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, rex::simde_mm_cvtepu32_ps_("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32)));", d, a));
        }
        return o;
    }

    // =======================================================================
    // Vector Merge (emit_vec_int_binary_swapped)
    // =======================================================================
    {
        static const std::tuple<const char*, const char*, const char*> merge[] = {
            {"vmrghb", "unpackhi_epi8", "u8"},  {"vmrghh", "unpackhi_epi16", "u16"},
            {"vmrghw", "unpackhi_epi32", "u32"},
            {"vmrglb", "unpacklo_epi8", "u8"},  {"vmrglh", "unpacklo_epi16", "u16"},
            {"vmrglw", "unpacklo_epi32", "u32"},
        };
        // (the "128" alias maps to its base form)
        const bool alias = core.size() > 3 && core.compare(core.size() - 3, 3, "128") == 0;
        for (const auto& e : merge) {
            if (core == std::get<0>(e) || (alias && core.substr(0, core.size() - 3) == std::get<0>(e))) {
                const int d = vpr_at(insn, ops, 0), b = vpr_at(insn, ops, 2), a = vpr_at(insn, ops, 1);
                Statements o; o.push_back(int_binary(std::get<1>(e), std::get<2>(e), d, b, a));  // swapped
                return o;
            }
        }
    }

    // =======================================================================
    // Vector Permute
    // =======================================================================
    if (is({"vperm", "vperm128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2), c = vpr_at(insn, ops, 3);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, rex::simde_mm_perm_epi8_("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8)));", d, a, b, c));
        return o;
    }
    if (core == "vpermwi128") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        std::int64_t imm = imm_at(insn, ops, 2);
        std::int64_t x = 3 - (imm & 0x3);
        std::int64_t y = 3 - ((imm >> 2) & 0x3);
        std::int64_t z = 3 - ((imm >> 4) & 0x3);
        std::int64_t w = 3 - ((imm >> 6) & 0x3);
        std::int64_t perm = x | (y << 2) | (z << 4) | (w << 6);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u32, simde_mm_shuffle_epi32("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), 0x{:X}));", d, a, perm));
        return o;
    }
    if (core == "vrlimi128") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        const std::int64_t blend = imm_at(insn, ops, 2);
        const std::int64_t index = imm_at(insn, ops, 3);
        // the permute that rotates the element at `index` into place
        static constexpr std::array<int, 4> kRotations = {(3 << 6) | (2 << 4) | (1 << 2) | 0, (2 << 6) | (1 << 4) | (0 << 2) | 3,
                                                          (1 << 6) | (0 << 4) | (3 << 2) | 2, (0 << 6) | (3 << 4) | (2 << 2) | 1};
        if (index < 0 || index > 3) throw ParseError(insn.mnemonic, 0, 4, "rotate index " + std::to_string(index) + " is not in 0..3");
        const int shuffle = kRotations[static_cast<std::size_t>(index)];
        Statements o;
        o.push_back(std::format("simde_mm_store_ps(ctx.v{}.f32, simde_mm_blend_ps("
        "simde_mm_load_ps(ctx.v{}.f32), simde_mm_permute_ps("
        "simde_mm_load_ps(ctx.v{}.f32), {}), {}));", d, d, a, shuffle, blend));
        return o;
    }

    // =======================================================================
    // Vector Shift / Rotate
    // =======================================================================
    if (core == "vsl" || core == "vslo" || core == "vslo128" || core == "vsro" || core == "vsro128" || core == "vsr") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        const char* helper = core == "vsl" ? "simde_mm_vsl"
                             : (core == "vslo" || core == "vslo128") ? "simde_mm_vslo"
                             : (core == "vsr") ? "simde_mm_vsr" : "simde_mm_vsro";
        Statements o; o.push_back(runtime_binary("u8", helper, d, a, b));
        return o;
    }
    if (is({"vsldoi", "vsldoi128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        const std::int64_t sh = imm_at(insn, ops, 3);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_alignr_epi8("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), {}));", d, a, b, 16 - sh));
        return o;
    }
    // per-lane shifts: "ctx.v<d>.<dty>[i] = ctx.v<a>.<dty>[i] <op> (ctx.v<b>.<bty>[bi] & <mask>);"
    {
        struct LaneShift { const char* mn; const char* dty; const char* op; const char* bty; const char* mask; int lanes; int bstride; };
        static const LaneShift lane_shifts[] = {
            {"vslb", "u8", "<<", "u8", "0x7", 16, 1},    {"vslh", "u16", "<<", "u16", "0xF", 8, 1},
            {"vsrh", "u16", ">>", "u16", "0xF", 8, 1},   {"vsrb", "u8", ">>", "u8", "0x7", 16, 1},
            {"vsrab", "s8", ">>", "u8", "0x7", 16, 1},   {"vsrah", "s16", ">>", "u16", "0xF", 8, 1},
            {"vslw", "u32", "<<", "u8", "0x1F", 4, 4},   {"vslw128", "u32", "<<", "u8", "0x1F", 4, 4},
            {"vsraw", "s32", ">>", "u8", "0x1F", 4, 4},  {"vsraw128", "s32", ">>", "u8", "0x1F", 4, 4},
            {"vsrw", "u32", ">>", "u8", "0x1F", 4, 4},   {"vsrw128", "u32", ">>", "u8", "0x1F", 4, 4},
        };
        for (const auto& e : lane_shifts) {
            if (core == e.mn) {
                const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
                Statements o;
                for (std::int64_t i = 0; i < e.lanes; ++i)
                    o.push_back(std::format("ctx.v{}.{}[{}] = ctx.v{}.{}[{}] {} (ctx.v{}.{}[{}] & {});", d, e.dty, i, a, e.dty, i, e.op, b, e.bty, i * e.bstride, e.mask));
                return o;
            }
        }
    }
    if (core == "vrlh") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        for (std::int64_t i = 0; i < 8; ++i) {
            o.push_back(std::format("{{ uint16_t sh = ctx.v{}.u16[{}] & 0xF;", b, i));
            o.push_back(std::format("ctx.v{}.u16[{}] = (ctx.v{}.u16[{}] << sh) | "
        "(sh ? (ctx.v{}.u16[{}] >> (16 - sh)) : 0); }}", d, i, a, i, a, i));
        }
        return o;
    }
    if (is({"vrlw", "vrlw128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        for (std::int64_t i = 0; i < 4; ++i) {
            o.push_back(std::format("{{ uint32_t sh = ctx.v{}.u32[{}] & 0x1F;", b, i));
            o.push_back(std::format("ctx.v{}.u32[{}] = (ctx.v{}.u32[{}] << sh) | "
        "(sh ? (ctx.v{}.u32[{}] >> (32 - sh)) : 0); }}", d, i, a, i, a, i));
        }
        return o;
    }

    // =======================================================================
    // Vector Splat  (element-reversal math reproduced verbatim)
    // =======================================================================
    if (core == "vspltb") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        std::int64_t perm = 15 - imm_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_shuffle_epi8("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u8), "
        "simde_mm_set1_epi8(char(0x{:X}))));", d, a, perm));
        return o;
    }
    if (core == "vsplth") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        std::int64_t perm = 7 - imm_at(insn, ops, 2);
        perm = (perm * 2) | ((perm * 2 + 1) * 256);   // (perm*2) | ((perm*2+1) << 8)
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u16, simde_mm_shuffle_epi8("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u16), "
        "simde_mm_set1_epi16(short(0x{:X}))));", d, a, perm));
        return o;
    }
    if (is({"vspltw", "vspltw128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        std::int64_t perm = 3 - imm_at(insn, ops, 2);
        perm |= (perm * 4) | (perm * 16) | (perm * 64);   // (perm << 2) | (perm << 4) | (perm << 6)
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u32, simde_mm_shuffle_epi32("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), 0x{:X}));", d, a, perm));
        return o;
    }
    if (is({"vspltisb", "vspltish", "vspltisw", "vspltisw128"})) {
        const int d = vpr_at(insn, ops, 0);
        std::int64_t s = imm_at(insn, ops, 1) & 0x1F;
        if (s & 0x10) s -= 0x20;
        Statements o;
        if (core == "vspltisb")
            o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, "
        "simde_mm_set1_epi8(char(0x{:X})));", d, s & 0xFF));
        else if (core == "vspltish")
            o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.s16, "
        "simde_mm_set1_epi16(short(0x{:X})));", d, s & 0xFFFF));
        else
            o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u32, "
        "simde_mm_set1_epi32(int(0x{:X})));", d, s & 0xFFFFFFFF));
        return o;
    }

    // =======================================================================
    // Vector Pack
    // =======================================================================
    if (is({"vpkshus", "vpkshus128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_packus_epi16("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s16), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s16)));", d, b, a));
        return o;
    }
    if (is({"vpkuhum", "vpkuhum128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u8, simde_mm_packus_epi16("
        "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u16), "
        "simde_mm_set1_epi16(0xFF)), "
        "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u16), "
        "simde_mm_set1_epi16(0xFF))));", d, b, a));
        return o;
    }
    if (is({"vpkuwum", "vpkuwum128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u16, simde_mm_packus_epi32("
        "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), "
        "simde_mm_set1_epi32(0xFFFF)), "
        "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v{}.u32), "
        "simde_mm_set1_epi32(0xFFFF))));", d, b, a));
        return o;
    }
    if (is({"vpkshss", "vpkshss128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.s8, simde_mm_packs_epi16("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s16), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s16)));", d, b, a));
        return o;
    }
    if (is({"vpkswss", "vpkswss128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.s16, simde_mm_packs_epi32("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s32), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s32)));", d, b, a));
        return o;
    }
    if (is({"vpkswus", "vpkswus128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        o.push_back(std::format("simde_mm_store_si128((simde__m128i*)ctx.v{}.u16, simde_mm_packus_epi32("
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s32), "
        "simde_mm_load_si128((simde__m128i*)ctx.v{}.s32)));", d, b, a));
        return o;
    }
    if (is({"vpkuhus", "vpkuhus128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        for (std::int64_t i = 0; i < 8; ++i) {
            o.push_back(std::format("ctx.v{}.u8[{}] = ctx.v{}.u16[{}] > 0xFF ? 0xFF : (uint8_t)ctx.v{}.u16[{}];", d, 15 - i, a, 7 - i, a, 7 - i));
            o.push_back(std::format("ctx.v{}.u8[{}] = ctx.v{}.u16[{}] > 0xFF ? 0xFF : (uint8_t)ctx.v{}.u16[{}];", d, 7 - i, b, 7 - i, b, 7 - i));
        }
        return o;
    }
    if (is({"vpkuwus", "vpkuwus128"})) {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1), b = vpr_at(insn, ops, 2);
        Statements o;
        for (std::int64_t i = 0; i < 4; ++i) {
            o.push_back(std::format("ctx.v{}.u16[{}] = ctx.v{}.u32[{}] > 0xFFFF ? 0xFFFF : (uint16_t)ctx.v{}.u32[{}];", d, 7 - i, a, 3 - i, a, 3 - i));
            o.push_back(std::format("ctx.v{}.u16[{}] = ctx.v{}.u32[{}] > 0xFFFF ? 0xFFFF : (uint16_t)ctx.v{}.u32[{}];", d, 3 - i, b, 3 - i, b, 3 - i));
        }
        return o;
    }

    if (core == "vpkd3d128") return pack_d3d(insn, ops);

    // =======================================================================
    // Vector Unpack
    // =======================================================================
    if (core == "vupkd3d128") return unpack_d3d(insn, ops);

    // VMX128 misidentification: vupkhsb128 / vupklsb128 with 0x60 as the third operand are the halfword forms
    const bool halfword_form = insn.operands.size() > 2 && ops.imm(insn.operands[2]) == std::optional<std::int64_t>(0x60);
    if (core == "vupkhsb" || core == "vupkhsb128") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        return core == "vupkhsb128" && halfword_form ? unpack_high_halves(d, a) : unpack_high_bytes(d, a);
    }
    if (core == "vupkhsh" || core == "vupkhsh128") return unpack_high_halves(vpr_at(insn, ops, 0), vpr_at(insn, ops, 1));
    if (core == "vupklsb" || core == "vupklsb128") {
        const int d = vpr_at(insn, ops, 0), a = vpr_at(insn, ops, 1);
        return core == "vupklsb128" && halfword_form ? unpack_low_halves(d, a) : unpack_low_bytes(d, a);
    }
    if (core == "vupklsh" || core == "vupklsh128") return unpack_low_halves(vpr_at(insn, ops, 0), vpr_at(insn, ops, 1));

    return std::nullopt;
}


std::string_view runtime_helpers() {
    return R"CBX(
// ---------------------------------------------------------------------------
// VMX runtime helpers the vector statements call. They are the runtime's (the rex namespace
// of native/rexglue/kernel: ppc.h and its SIMD extension header), declared here, not defined.
//
//   namespace rex {
//     simde__m128i simde_mm_adds_epu32(simde__m128i a, simde__m128i b);   // saturating u32 add
//     simde__m128i simde_mm_avg_epi8 (simde__m128i a, simde__m128i b);    // signed byte average
//     simde__m128i simde_mm_avg_epi16(simde__m128i a, simde__m128i b);    // signed hword average
//     simde__m128i simde_mm_avg_epi32(simde__m128i a, simde__m128i b);    // signed word average
//     simde__m128i simde_mm_cmpgt_epu8 (simde__m128i a, simde__m128i b);  // unsigned byte  >
//     simde__m128i simde_mm_cmpgt_epu16(simde__m128i a, simde__m128i b);  // unsigned hword >
//     simde__m128i simde_mm_vctsxs(simde__m128 v);      // float->signed word saturate
//     simde__m128i simde_mm_vctuxs(simde__m128 v);      // float->unsigned word saturate
//     simde__m128  simde_mm_cvtepu32_ps_(simde__m128i v); // unsigned word -> float
//     simde__m128i simde_mm_perm_epi8_(simde__m128i a, simde__m128i b, simde__m128i c); // vperm
//     simde__m128i simde_mm_vsl (simde__m128i a, simde__m128i b);  // 128-bit shift-left  (bits)
//     simde__m128i simde_mm_vsr (simde__m128i a, simde__m128i b);  // 128-bit shift-right (bits)
//     simde__m128i simde_mm_vslo(simde__m128i a, simde__m128i b);  // 128-bit shift-left  (octets)
//     simde__m128i simde_mm_vsro(simde__m128i a, simde__m128i b);  // 128-bit shift-right (octets)
//   }
//
// Constants referenced by vpkd3d128 (NORMPACKED32) come from ppc/context.h:
//   kPack2101010_Min10 / kPack2101010_Max10 / kPack2101010_Min2 / kPack2101010_Max2
// ---------------------------------------------------------------------------
)CBX";
}

std::span<const std::string_view> mnemonics() { return kMnemonics; }

}  // namespace cb::recompile::builders::vector
