// HLSL support code included in every translated shader: Xenos ALU semantics (analysis/xenos/03c), vertex unpacking
// (04) and fetch-constant access. All arithmetic that must not be reassociated or fused is `precise`, and shaders are
// compiled with IEEE strictness.

#include "gpu/shader_translator.h"

namespace gpu
{
    const char* const kXenosHlslLibrary = R"HLSL(
#define XE_FLT_MAX asfloat(0x7F7FFFFFu)

// ---- Bindings shared by all stages (root signature: shader_translator.h) --------------------------------------------

cbuffer XeSystemConstants : register(b0)
{
    uint4 xe_sys_flags;          // x: flags (XE_SYS_*), y: vertex index offset, z: min index, w: max index
    uint4 xe_sys_index;          // x: index buffer address (bytes), y: index format (0 auto, 1 16-bit, 2 32-bit), z: endian
    uint4 xe_sys_const_window;   // x: VS float constant base, y: VS count, z: PS base, w: PS count
    float4 xe_sys_ndc_scale;     // xyz: host clip-space scale applied to the guest position
    float4 xe_sys_ndc_offset;    // xyz: host clip-space offset (multiplied by W)
    float4 xe_sys_clip_planes[6];
    uint4 xe_sys_misc;           // x: user clip plane enable mask, y: interpolator count, z: ParamGen flags, w: always 0
    float4 xe_sys_point;         // xy: point sprite diameter (guest pixels), zw: host NDC radius per pixel of diameter
};
#define XE_SYS_VTX_XY_FMT 1u     // PA_CL_VTE_CNTL.VtxXyFmt: XY already divided by W
#define XE_SYS_VTX_Z_FMT 2u      // VtxZFmt
#define XE_SYS_VTX_W0_FMT 4u     // VtxW0Fmt: W is W (else 1/W)

cbuffer XeFloatConstants : register(b1)
{
    float4 xe_c[512];
};

cbuffer XeBoolLoopConstants : register(b2)
{
    uint4 xe_bool_loop[10];      // dwords 0-7: 256 bool constants, 8-39: loop constants
};

cbuffer XeFetchConstants : register(b3)
{
    uint4 xe_fetch[48];          // 32 texture constants x 6 dwords = 96 vertex constants x 2 dwords
};

uint xe_fetch_dword(uint index) { return xe_fetch[index >> 2][index & 3]; }
bool xe_bool(uint index) { return ((xe_bool_loop[index >> 7][(index >> 5) & 3] >> (index & 31)) & 1u) != 0; }
uint xe_loop_constant(uint index) { uint d = 8 + index; return xe_bool_loop[d >> 2][d & 3]; }

// ---- Operands -----------------------------------------------------------------------------------------------------

// Denormal inputs read as zero, sign kept (03c §1). Integer masking only: FXC folds a float select of
// `asfloat(u & 0x80000000)` into +0, losing the sign (found by the CB_SHADER_TEST harness).
float xe_flush(float x)
{
    uint u = asuint(x);
    return asfloat(u & ((u & 0x7F800000u) != 0u ? 0xFFFFFFFFu : 0x80000000u));
}
float4 xe_flush4(float4 x)
{
    uint4 u = asuint(x);
    return asfloat(u & ((u & 0x7F800000u) != 0u ? 0xFFFFFFFFu : 0x80000000u));
}

// Source modifiers are sign-bit operations, as on the hardware. Float negation lets FXC move negations across
// operations (-a - -b -> -(a - b)), which changes the sign of zero results.
float4 xe_abs4(float4 x) { return asfloat(asuint(x) & 0x7FFFFFFFu); }
float4 xe_neg4(float4 x) { return asfloat(asuint(x) ^ 0x80000000u); }
float xe_abs(float x) { return asfloat(asuint(x) & 0x7FFFFFFFu); }
float xe_neg(float x) { return asfloat(asuint(x) ^ 0x80000000u); }

// Bit-exact select of a or b. A float compare + movc is turned into a native min/max by GPU driver compilers, which
// drops the Xenos NaN and signed-zero behaviour; a mask select keeps it.
float xe_select(bool c, float a, float b)
{
    uint m = c ? 0xFFFFFFFFu : 0u;
    return asfloat((asuint(a) & m) | (asuint(b) & ~m));
}
float4 xe_select4(bool4 c, float4 a, float4 b)
{
    uint4 m = c ? 0xFFFFFFFFu : 0u;
    return asfloat((asuint(a) & m) | (asuint(b) & ~m));
}

// Float constant c[index] of a stage window; out-of-window reads return 0 (as Xenia's interpreter; hardware undocumented).
// The renderer uploads the constants flushed (shader_translator.h FlushDenormals), so reads need no flush.
float4 xe_const(int index, uint base, uint count)
{
    if (index < 0 || uint(index) >= count || base + uint(index) >= 512u)
        return float4(0.0, 0.0, 0.0, 0.0);
    return xe_c[base + uint(index)];
}

bool xe_is_nan(float x) { return (asuint(x) & 0x7FFFFFFFu) > 0x7F800000u; }
bool xe_is_inf(float x) { return (asuint(x) & 0x7FFFFFFFu) == 0x7F800000u; }

// ---- ALU (03c) ----------------------------------------------------------------------------------------------------

// Legacy multiply: a zero factor gives +0 even with inf/NaN (inputs are already flushed) (03c §7.1).
float4 xe_mul4(float4 a, float4 b)
{
    precise float4 p = a * b;
    uint4 zero = ((asuint(a) & 0x7FFFFFFFu) == 0u) || ((asuint(b) & 0x7FFFFFFFu) == 0u) ? 0u : 0xFFFFFFFFu;
    return asfloat(asuint(p) & zero);
}
float xe_mul(float a, float b) { return xe_mul4(a.xxxx, b.xxxx).x; }
// mad = legacy multiply, then a separately rounded add (03c §7.2: follow Xenia, no FMA).
float4 xe_mad4(float4 a, float4 b, float4 c)
{
    precise float4 p = xe_mul4(a, b);
    precise float4 s = p + c;
    return s;
}
// max/min are compare-selects: with a NaN operand the result is the second operand (03c §7.3).
// IEEE ordered a >= b / a < b on integer ordering keys. Even an integer mask select is not enough: with negated
// operands FXC rewrites (-a >= -b) as (b >= a) and the driver (NVIDIA 616.92) then emits a native NaN-dropping
// -min(a, b). Without a float compare there is nothing to pattern-match. Inputs are already flushed.
uint4 xe_order_key4(uint4 u) { return (u & 0x80000000u) != 0u ? ~u : (u | 0x80000000u); }
bool4 xe_not_nan4(uint4 u) { return (u & 0x7FFFFFFFu) <= 0x7F800000u; }
bool4 xe_ge4(float4 a, float4 b)
{
    uint4 ua = asuint(a), ub = asuint(b);
    bool4 zeros = ((ua | ub) & 0x7FFFFFFFu) == 0u;
    return xe_not_nan4(ua) && xe_not_nan4(ub) && (zeros || xe_order_key4(ua) >= xe_order_key4(ub));
}
bool4 xe_lt4(float4 a, float4 b)
{
    uint4 ua = asuint(a), ub = asuint(b);
    bool4 zeros = ((ua | ub) & 0x7FFFFFFFu) == 0u;
    return xe_not_nan4(ua) && xe_not_nan4(ub) && !zeros && xe_order_key4(ua) < xe_order_key4(ub);
}
bool4 xe_gt4(float4 a, float4 b) { return xe_lt4(b, a); }
bool4 xe_le4(float4 a, float4 b) { return xe_ge4(b, a); }
bool4 xe_eq4(float4 a, float4 b)
{
    uint4 ua = asuint(a), ub = asuint(b);
    bool4 zeros = ((ua | ub) & 0x7FFFFFFFu) == 0u;
    return xe_not_nan4(ua) && xe_not_nan4(ub) && (zeros || ua == ub);
}
bool4 xe_ne4(float4 a, float4 b) { return !xe_eq4(a, b); } // unordered: NaN compares not-equal
bool xe_ge(float a, float b) { return xe_ge4(a.xxxx, b.xxxx).x; }
bool xe_gt(float a, float b) { return xe_gt4(a.xxxx, b.xxxx).x; }
bool xe_le(float a, float b) { return xe_le4(a.xxxx, b.xxxx).x; }
bool xe_lt(float a, float b) { return xe_lt4(a.xxxx, b.xxxx).x; }
bool xe_eq(float a, float b) { return xe_eq4(a.xxxx, b.xxxx).x; }
bool xe_ne(float a, float b) { return xe_ne4(a.xxxx, b.xxxx).x; }
float4 xe_max4(float4 a, float4 b) { return xe_select4(xe_ge4(a, b), a, b); }
float4 xe_min4(float4 a, float4 b) { return xe_select4(xe_lt4(a, b), a, b); }
float xe_max(float a, float b) { return xe_max4(a.xxxx, b.xxxx).x; }
float xe_min(float a, float b) { return xe_min4(a.xxxx, b.xxxx).x; }
bool xe_is_zero(float x) { return (asuint(x) & 0x7FFFFFFFu) == 0u; }
bool xe_is_one(float x) { return asuint(x) == 0x3F800000u; }

float4 xe_frc4(float4 a) { precise float4 r = a - floor(a); return r; }
float xe_frc(float a) { precise float r = a - floor(a); return r; }

float xe_dp4(float4 a, float4 b)
{
    precise float s = xe_mul(a.x, b.x) + xe_mul(a.y, b.y);
    precise float t = s + xe_mul(a.z, b.z);
    precise float u = t + xe_mul(a.w, b.w);
    return u;
}
float xe_dp3(float4 a, float4 b)
{
    precise float s = xe_mul(a.x, b.x) + xe_mul(a.y, b.y);
    precise float t = s + xe_mul(a.z, b.z);
    return t;
}
float xe_dp2add(float4 a, float4 b, float4 c)
{
    precise float s = xe_mul(a.x, b.x) + xe_mul(a.y, b.y);
    precise float t = s + c.x;
    return t;
}

// cube from src1 = dir.zzxy (03c §5.1): (TC, SC, 2*MA, face).
float4 xe_cube(float4 s1)
{
    float z = s1.x, x = s1.z, y = s1.w;
    float az = xe_abs(z), ax = xe_abs(x), ay = xe_abs(y);
    float4 r;
    if (xe_ge(az, ax) && xe_ge(az, ay))
        r = float4(xe_neg(y), xe_lt(z, 0.0) ? xe_neg(x) : x, z, xe_lt(z, 0.0) ? 5.0 : 4.0);
    else if (xe_ge(ay, ax))
        r = float4(xe_lt(y, 0.0) ? xe_neg(z) : z, x, y, xe_lt(y, 0.0) ? 3.0 : 2.0);
    else
        r = float4(xe_neg(y), xe_lt(x, 0.0) ? z : xe_neg(z), x, xe_lt(x, 0.0) ? 1.0 : 0.0);
    precise float ma2 = r.z * 2.0;
    r.z = ma2;
    return r;
}

float xe_max4_reduce(float4 a) { return xe_max(xe_max(xe_max(a.x, a.y), a.z), a.w); }

// a0 = clamp_a0(value): NaN or below -256 -> -256, above 255 -> 255 (03c §6.2).
int xe_clamp_a0(float f)
{
    if (!xe_ge(f, -256.0))
        return -256;
    if (xe_gt(f, 255.0))
        return 255;
    return int(f);
}
float xe_round_half_up(float f) { precise float h = f + 0.5; return floor(h); }

// Special values are produced with integer operations: FXC folds float selects of sign-masked values into +0, and
// drivers rewrite compare-selects into native min/max (see above).
float xe_exp(float a) { return xe_is_nan(a) ? a : (xe_is_zero(a) ? 1.0 : exp2(a)); }
// IEEE log2 special cases: +-0 -> -inf, negative -> NaN (the host returned -inf for negatives), 1 -> 0.
float xe_log(float a)
{
    uint u = asuint(a);
    if (xe_is_nan(a))
        return a;
    if ((u & 0x7FFFFFFFu) == 0u)
        return asfloat(0xFF800000u);
    if ((u & 0x80000000u) != 0u && (u & 0x7FFFFFFFu) <= 0x7F800000u)
        return asfloat(0xFFC00000u);
    return xe_is_one(a) ? 0.0 : log2(a);
}
float xe_logc(float a)
{
    float r = xe_log(a);
    return asfloat(asuint(r) == 0xFF800000u ? 0xFF7FFFFFu : asuint(r));
}
float xe_rcp(float a) { return xe_is_nan(a) ? a : (xe_is_one(a) ? 1.0 : rcp(a)); }
// inf results -> +-FLT_MAX (clamp) or +-0 (FF), sign kept.
float xe_inf_to(float r, uint magnitude)
{
    uint u = asuint(r);
    return asfloat((u & 0x7FFFFFFFu) == 0x7F800000u ? ((u & 0x80000000u) | magnitude) : u);
}
float xe_rcpc(float a) { return xe_inf_to(xe_rcp(a), 0x7F7FFFFFu); }
float xe_rcpf(float a) { return xe_inf_to(xe_rcp(a), 0u); }
float xe_rsq(float a) { return xe_is_nan(a) ? a : (xe_is_one(a) ? 1.0 : rsqrt(a)); }
float xe_rsqc(float a) { return xe_inf_to(xe_rsq(a), 0x7F7FFFFFu); }
float xe_rsqf(float a) { return xe_inf_to(xe_rsq(a), 0u); }
// IEEE sqrt: -0 -> -0, NaN -> NaN, negative -> NaN (the host returned 0 for negatives and NaN).
float xe_sqrt(float a)
{
    uint u = asuint(a);
    if ((u & 0x7FFFFFFFu) == 0u || (u & 0x7FFFFFFFu) > 0x7F800000u)
        return a;
    if ((u & 0x80000000u) != 0u)
        return asfloat(0xFFC00000u);
    return sqrt(a);
}
// muls_prev2 per the XDK text (03c §6.1).
float xe_muls_prev2(float a, float b, float ps)
{
    uint p = asuint(ps);
    if (p == 0xFF7FFFFFu || p == 0xFF800000u || xe_is_nan(ps) || xe_le(b, 0.0) || xe_is_nan(b))
        return -XE_FLT_MAX;
    return xe_mul(a, ps);
}

// ---- Vertex fetch (04) --------------------------------------------------------------------------------------------

ByteAddressBuffer xe_memory : register(t0);

uint xe_swap(uint v, uint endian)
{
    if (endian == 1u)
        return ((v << 8) & 0xFF00FF00u) | ((v >> 8) & 0x00FF00FFu);
    if (endian == 2u)
        return (v << 24) | ((v << 8) & 0x00FF0000u) | ((v >> 8) & 0x0000FF00u) | (v >> 24);
    if (endian == 3u)
        return (v << 16) | (v >> 16);
    return v;
}

// Xenos half: exponent 31 is an ordinary exponent (no Inf/NaN); denormals read as zero (Xenia xenos_half_to_float).
float xe_half(uint h)
{
    uint mantissa = h & 0x3FFu, exponent = (h >> 10) & 0x1Fu;
    uint sign = (h & 0x8000u) << 16;
    if (exponent == 0u)
        return asfloat(sign);
    return asfloat(sign | ((exponent + 112u) << 23) | (mantissa << 13));
}

uint xe_field(uint v, uint offset, uint bits) { return bits == 32u ? v : (v >> offset) & ((1u << bits) - 1u); }
int xe_signed_field(uint v, uint offset, uint bits)
{
    return bits == 32u ? int(v) : (int(v << (32u - offset - bits)) >> (32u - bits));
}

// One component of a fixed-point vertex format (04 §5.1).
float xe_unpack_fixed(uint v, uint offset, uint bits, bool isSigned, bool isInteger, bool noZero)
{
    if (!isSigned)
    {
        float u = float(xe_field(v, offset, bits));
        if (isInteger)
            return u;
        precise float r = u / float(bits == 32u ? 4294967295.0 : float((1u << bits) - 1u));
        return r;
    }
    float s = float(xe_signed_field(v, offset, bits));
    if (isInteger)
        return s;
    if (noZero)
    {
        precise float t = s * 2.0 + 1.0;
        precise float r = t / (bits == 32u ? 4294967295.0 : float((1u << bits) - 1u));
        return r;
    }
    precise float r = s / (bits == 32u ? 2147483647.0 : float((1u << (bits - 1u)) - 1u));
    return xe_max(r, -1.0);
}

// Guest vertex index for host vertex `position` in the guest index stream (04 §6): index buffer read with the endian
// swap, low 24 bits, + VGT_INDX_OFFSET, clamped to [min, max]. Primitive reset is handled by the draw code.
float xe_vertex_index(uint position)
{
    uint index = position;
    uint format = xe_sys_index.y;
    if (format == 1u)
    {
        uint address = xe_sys_index.x + position * 2u;
        uint word = xe_memory.Load(address & ~3u);
        uint endian = xe_sys_index.z == 2u ? 1u : (xe_sys_index.z == 3u ? 0u : xe_sys_index.z);
        word = xe_swap(word, endian);
        index = (address & 2u) ? (word >> 16) : (word & 0xFFFFu);
    }
    else if (format == 2u)
    {
        index = xe_swap(xe_memory.Load(xe_sys_index.x + position * 4u), xe_sys_index.z);
    }
    index = ((index & 0xFFFFFFu) + xe_sys_flags.y) & 0xFFFFFFu;
    index = clamp(index, xe_sys_flags.z & 0xFFFFFFu, xe_sys_flags.w & 0xFFFFFFu);
    return float(index);
}

// ---- Texture fetch (05b §1.2) ---------------------------------------------------------------------------------------

// Source of result component `component` in the fetch-constant swizzle (dword 3 bits 1-12): data X..W = 0..3, then
// 4 = constant 0, 5 = constant 1; the invalid 6 and "keep" 7 read as 0 and 1 (Xenia).
uint xe_tex_swizzle(uint dword3, uint component)
{
    uint select = (dword3 >> (1u + 3u * component)) & 7u;
    return select < 4u ? select : (select & 1u) + 4u;
}

// The sampled data components `tv` (host UNORM/SNORM conversion or integer values, texture.h) to the fetch result:
// bias-signed data components become 2x - 1 (after filtering, exact for point sampling), then the swizzle, then
// ExpAdjust (dword 3 bits 13-18) as an exact multiply by 2^n. `td0`, `td3`: fetch-constant dwords 0 and 3.
float4 xe_tex_finish(float4 tv, uint td0, uint td3)
{
    uint4 signs = (td0.xxxx >> uint4(2u, 4u, 6u, 8u)) & 3u;
    precise float4 biased = tv * 2.0 - 1.0;
    float4 data = xe_select4(signs == 2u, biased, tv);
    float source[6] = { data.x, data.y, data.z, data.w, 0.0, 1.0 };
    float4 r = float4(source[xe_tex_swizzle(td3, 0u)], source[xe_tex_swizzle(td3, 1u)],
                      source[xe_tex_swizzle(td3, 2u)], source[xe_tex_swizzle(td3, 3u)]);
    precise float4 scaled = r * asfloat(uint((int(td3 << 13) >> 26) + 127) << 23);
    return scaled;
}
)HLSL";
}
