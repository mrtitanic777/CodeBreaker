// HLSL generation for the EDRAM output merger (see output_merger.h). Spec: analysis/xenos/06a (layout, encodings),
// 06b §4 (design), 01b §2 (RB state). Every float comparison and select goes through the library's integer helpers
//.

#include "gpu/output_merger.h"

#include "log.h"

#include <format>

namespace gpu
{
    const char* const kEdramAddressHlsl = R"HLSL(
// 1x-equivalent 32bpp sample coordinates of pixel `p` sample `s` (XDK permutation, analysis/xenos/06a §4.2).
uint2 xe_edram_sample(uint2 p, uint s, uint msaa)
{
    if (msaa == 0u)
        return p;
    if (msaa == 1u)
        return uint2(4u * (p.x >> 2) + (p.x & 1u) + 2u * s, 4u * (p.y >> 1) + (p.y & 1u) + 2u * ((p.x >> 1) & 1u));
    return uint2(4u * (p.x >> 1) + (p.x & 1u) + 2u * (s & 1u), 4u * (p.y >> 1) + (p.y & 1u) + 2u * (s >> 1));
}

// Byte offset of sample `q` of a surface at tile `base` with `pitchTiles` tiles per row (06a §3); depth/stencil
// swaps the 40-column halves of each tile row (06a §5.1); tiles wrap at 2048 (06a §3.3).
uint xe_edram_address(uint base, uint pitchTiles, uint2 q, bool depth)
{
    uint column = q.x % 80u;
    if (depth)
        column = column < 40u ? column + 40u : column - 40u;
    uint tile = (base + (q.y >> 4) * pitchTiles + q.x / 80u) & 2047u;
    return (tile * 1280u + (q.y & 15u) * 80u + column) * 4u;
}
)HLSL";

    namespace
    {
        const char kComponents[] = "xyzw";

        bool IsFixed16(uint32_t format) { return format == 4; }

        // Blend factor of 01b §6.4 for the colour (rgb) or alpha channel group. `src`, `dst`, `k` are float4 names.
        std::string Factor(uint32_t factor, bool alpha, uint32_t format)
        {
            const char* c = alpha ? "w" : "xyz";
            auto one = [&](const std::string& v) { return std::format("xe_om_one_minus({})", v); };
            if (IsFixed16(format) && (factor == 6 || factor == 7 || factor == 10 || factor == 11 || factor == 16 ||
                                      (alpha && (factor == 4 || factor == 5 || factor == 8 || factor == 9))))
                Fatal("GPU: blend factor {} reads alpha of a 16_16 render target, which the XDK documents as "
                      "undefined", factor);
            switch (factor)
            {
            case 0: return "";                                   // ZERO: the operand is skipped
            case 1: return alpha ? "1.0" : "float3(1.0, 1.0, 1.0)";
            case 4: return std::format("src.{}", c);
            case 5: return one(std::format("src.{}", c));
            case 6: return alpha ? "src.w" : "src.www";
            case 7: return alpha ? one("src.w") : one("src.www");
            case 8: return std::format("dst.{}", c);
            case 9: return one(std::format("dst.{}", c));
            case 10: return alpha ? "dst.w" : "dst.www";
            case 11: return alpha ? one("dst.w") : one("dst.www");
            case 12: return std::format("k.{}", c);
            case 13: return one(std::format("k.{}", c));
            case 14: return alpha ? "k.w" : "k.www";
            case 15: return alpha ? one("k.w") : one("k.www");
            case 16: return alpha ? "1.0" : "xe_om_min(src.w, xe_om_one_minus(dst.w)).xxx";
            default:
                Fatal("GPU: blend factor {} is undefined (RB_BLENDCONTROL)", factor);
            }
        }

        // `result = op(src * fs, dst * fd)` for one channel group. Products are clamped to the format range (06b §3):
        // for 8_8_8_8 both operands are in [0, 1] so nothing can leave it; for 16_16 (-32..32) a product can.
        std::string BlendGroup(uint32_t srcFactor, uint32_t op, uint32_t dstFactor, bool alpha, uint32_t format)
        {
            const char* c = alpha ? "w" : "xyz";
            const char* type = alpha ? "float" : "float3";
            const char* zero = alpha ? "0.0" : "float3(0.0, 0.0, 0.0)";
            if (op == 2 || op == 3) // MIN/MAX ignore the factors (D3D9 semantics, 01b §2)
                return std::format("    r.{} = xe_om_{}(src.{}, dst.{});\n", c, op == 2 ? "min" : "max", c, c);
            if (op > 4)
                Fatal("GPU: blend op {} is undefined (RB_BLENDCONTROL)", op);
            auto term = [&](const char* operand, uint32_t factor) -> std::string {
                const std::string f = Factor(factor, alpha, format);
                if (f.empty())
                    return zero;
                const std::string product = std::format("xe_om_mul({}.{}, {})", operand, c, f);
                return IsFixed16(format) ? std::format("xe_om_clamp32({})", product) : product;
            };
            std::string text = std::format("    {{\n        {} bs = {};\n        {} bd = {};\n", type, term("src", srcFactor),
                                           type, term("dst", dstFactor));
            switch (op)
            {
            case 0: text += std::format("        precise {} br = bs + bd;\n", type); break;
            case 1: text += std::format("        precise {} br = bs - bd;\n", type); break;
            case 4: text += std::format("        precise {} br = bd - bs;\n", type); break;
            }
            text += std::format("        r.{} = br;\n    }}\n", c);
            return text;
        }

        // Format helpers: decode (dword -> float4), range clamp (NaN -> 0), encode (clamped float4 -> dword).
        std::string FormatFunctions(uint32_t format)
        {
            switch (format)
            {
            case 0:
                return R"HLSL(
float4 xe_om_decode_0(uint v)
{
    precise float4 r = float4((uint4(v, v >> 8, v >> 16, v >> 24) & 0xFFu)) * asfloat(0x3B808081u); // n * (1/255)
    return r;
}
float4 xe_om_range_0(float4 c) { return saturate(c); }
uint xe_om_encode_0(float4 c)
{
    precise float4 t = c * 255.0 + 0.5;
    uint4 n = uint4(floor(t));
    return n.x | (n.y << 8) | (n.z << 16) | (n.w << 24);
}
)HLSL";
            case 4:
                return R"HLSL(
// 16_16 fixed point, range -32..32 (Xenia's scale 32767/32; 06a §6.1). Only R and G exist.
float4 xe_om_decode_4(uint v)
{
    precise float2 r = float2(int2(v << 16, v) >> 16) * asfloat(0x3A800100u); // n * (32/32767)
    return float4(r, 0.0, 0.0);
}
float4 xe_om_range_4(float4 c)
{
    uint4 u = asuint(c);
    float4 r = xe_select4((u & 0x7FFFFFFFu) > 0x7F800000u, 0.0, c);   // NaN -> 0
    return xe_min4(xe_max4(r, -32.0), 32.0);
}
float3 xe_om_clamp32(float3 c) { return xe_om_range_4(float4(c, 0.0)).xyz; }
float xe_om_clamp32(float c) { return xe_om_range_4(c.xxxx).x; }
uint xe_om_encode_4(float4 c)
{
    precise float2 t = c.xy * asfloat(0x447FFE00u);                     // * 32767/32
    precise float2 h = t + asfloat((asuint(t) & 0x80000000u) | 0x3F000000u); // +-0.5, away from zero
    int2 n = int2(h);
    return (uint(n.x) & 0xFFFFu) | (uint(n.y) << 16);
}
)HLSL";
            default:
                Fatal("GPU: render target format {} ({}) is not implemented by the EDRAM output merger", format,
                      EdramColorFormatName(format));
            }
        }

        // Bits of the dword written by each colour channel (R, G, B, A) of a format.
        uint32_t ChannelBits(uint32_t format, uint32_t channel)
        {
            if (format == 4)
                return channel == 0 ? 0x0000FFFFu : channel == 1 ? 0xFFFF0000u : 0u;
            return 0xFFu << (8 * channel);
        }

        std::string CompareExpression(uint32_t func, const std::string& a, const std::string& b, bool isFloat)
        {
            switch (func & 7)
            {
            case 0: return "false";
            case 1: return isFloat ? std::format("xe_lt({}, {})", a, b) : std::format("({} < {})", a, b);
            case 2: return isFloat ? std::format("xe_eq({}, {})", a, b) : std::format("({} == {})", a, b);
            case 3: return isFloat ? std::format("xe_le({}, {})", a, b) : std::format("({} <= {})", a, b);
            case 4: return isFloat ? std::format("xe_gt({}, {})", a, b) : std::format("({} > {})", a, b);
            case 5: return isFloat ? std::format("xe_ne({}, {})", a, b) : std::format("({} != {})", a, b);
            case 6: return isFloat ? std::format("xe_ge({}, {})", a, b) : std::format("({} >= {})", a, b);
            default: return "true";
            }
        }

        // New stencil value for op (01b §6.3), `s` = old stencil, `ref` = reference.
        std::string StencilOp(uint32_t op)
        {
            switch (op & 7)
            {
            case 0: return "s";
            case 1: return "0u";
            case 2: return "ref";
            case 3: return "min(s + 1u, 255u)";
            case 4: return "(s == 0u ? 0u : s - 1u)";
            case 5: return "(~s & 0xFFu)";
            case 6: return "((s + 1u) & 0xFFu)";
            default: return "((s - 1u) & 0xFFu)";
            }
        }

        // Stencil test and op selection for one face's state (func/fail/zpass/zfail at `shift`).
        std::string StencilFace(uint32_t control, uint32_t shift, const char* refMaskWrite)
        {
            const uint32_t func = (control >> shift) & 7, fail = (control >> (shift + 3)) & 7;
            const uint32_t zpass = (control >> (shift + 6)) & 7, zfail = (control >> (shift + 9)) & 7;
            std::string text = std::format("        uint rmw = {};\n", refMaskWrite);
            text += "        uint ref = rmw & 0xFFu, mask = (rmw >> 8) & 0xFFu, writeMask = (rmw >> 16) & 0xFFu;\n";
            text += std::format("        stencilPass = {};\n",
                                CompareExpression(func, "(ref & mask)", "(s & mask)", false));
            text += std::format("        uint ns = !stencilPass ? {} : (!depthPass ? {} : {});\n", StencilOp(fail),
                                StencilOp(zfail), StencilOp(zpass));
            text += "        s = (s & ~writeMask) | (ns & writeMask);\n";
            return text;
        }
    }

    bool OutputMergerSupportsColorFormat(uint32_t format)
    {
        return format == 0 || format == 4;
    }

    const char* EdramColorFormatName(uint32_t format)
    {
        switch (format)
        {
        case 0: return "8_8_8_8";
        case 1: return "8_8_8_8_GAMMA";
        case 2: return "2_10_10_10";
        case 3: return "2_10_10_10_FLOAT";
        case 4: return "16_16";
        case 5: return "16_16_16_16";
        case 6: return "16_16_FLOAT";
        case 7: return "16_16_16_16_FLOAT";
        case 10: return "2_10_10_10_AS_10_10_10_10";
        case 12: return "2_10_10_10_FLOAT_AS_16_16_16_16";
        case 14: return "32_FLOAT";
        case 15: return "32_32_FLOAT";
        default: return "undefined";
        }
    }

    std::string AlphaTestHlsl(uint32_t alphaTest)
    {
        return std::format(R"HLSL(
cbuffer XeOutputConstants : register(b4)
{{
    uint4 xe_om_color_base;
    uint4 xe_om_surface;
    float4 xe_om_blend_constant;
    float4 xe_om_alpha_poly;    // x: alpha ref
    float4 xe_om_poly_back;
}};
bool xe_alpha_pass(float a) {{ return {}; }}
)HLSL", CompareExpression(alphaTest & 7, "a", "xe_om_alpha_poly.x", true));
    }

    std::string OutputMergerHlsl(const OutputMergerKey& key, OutputMergerTarget target)
    {
        std::string text = R"HLSL(
// ---- EDRAM output merger (output_merger.cpp) ----------------------------------------------------------------------
cbuffer XeOutputConstants : register(b4)
{
    uint4 xe_om_color_base;     // EDRAM tile of RT0-3
    uint4 xe_om_surface;        // x: pitch in tiles (32bpp), y: depth base, z: stencil front, w: stencil back
    float4 xe_om_blend_constant;
    float4 xe_om_alpha_poly;    // x: alpha ref, y/z: front polygon offset scale/offset, w: back scale
    float4 xe_om_poly_back;     // x: back polygon offset
};

float xe_om_min(float a, float b) { return xe_min(a, b); }
float3 xe_om_min(float3 a, float3 b) { return xe_min4(float4(a, 0.0), float4(b, 0.0)).xyz; }
float xe_om_max(float a, float b) { return xe_max(a, b); }
float3 xe_om_max(float3 a, float3 b) { return xe_max4(float4(a, 0.0), float4(b, 0.0)).xyz; }
float xe_om_one_minus(float a) { precise float r = 1.0 - a; return r; }
float3 xe_om_one_minus(float3 a) { precise float3 r = 1.0 - a; return r; }
float xe_om_mul(float a, float b) { precise float r = a * b; return r; }
float3 xe_om_mul(float3 a, float3 b) { precise float3 r = a * b; return r; }
)HLSL";
        text += kEdramAddressHlsl;

        // Colour functions, one per enabled RT: shader colour + old dword -> new dword.
        bool formatEmitted[16] = {};
        for (uint32_t rt = 0; rt < 4; rt++)
        {
            const uint32_t format = key.colorFormat[rt];
            if (format == kRtDisabled)
                continue;
            if (!formatEmitted[format])
            {
                text += FormatFunctions(format);
                formatEmitted[format] = true;
            }
            const uint32_t blend = key.blendControl[rt];
            const uint32_t srcColor = blend & 0x1F, opColor = (blend >> 5) & 7, dstColor = (blend >> 8) & 0x1F;
            const uint32_t srcAlpha = (blend >> 16) & 0x1F, opAlpha = (blend >> 21) & 7, dstAlpha = (blend >> 24) & 0x1F;
            const bool opaque = srcColor == 1 && opColor == 0 && dstColor == 0 && srcAlpha == 1 && opAlpha == 0 &&
                                dstAlpha == 0;
            uint32_t keep = 0xFFFFFFFFu;
            for (uint32_t c = 0; c < 4; c++)
                if ((key.colorMask[rt] >> c) & 1)
                    keep &= ~ChannelBits(format, c);
            const bool needsOld = !opaque || keep != 0;
            text += std::format("\nuint xe_om_color_{}(float4 color, uint old)\n{{\n", rt);
            if (key.expBias[rt] != 0)
            {
                // Exact power of two (06a §6: applied after alpha test / alpha-to-mask, before clamping and blending).
                text += std::format("    precise float4 biased = color * asfloat(0x{:08X}u);\n",
                                    uint32_t(key.expBias[rt] + 127) << 23);
                text += std::format("    float4 src = xe_om_range_{}(biased);\n", format);
            }
            else
                text += std::format("    float4 src = xe_om_range_{}(color);\n", format);
            if (opaque)
            {
                text += "    float4 r = src;\n";
            }
            else
            {
                text += std::format("    float4 dst = xe_om_decode_{}(old);\n", format);
                text += std::format("    float4 k = xe_om_range_{}(xe_om_blend_constant);\n", format);
                text += "    float4 r = src;\n";
                text += BlendGroup(srcColor, opColor, dstColor, false, format);
                if (!IsFixed16(format)) // 16_16 has no alpha channel
                    text += BlendGroup(srcAlpha, opAlpha, dstAlpha, true, format);
                text += std::format("    r = xe_om_range_{}(r);\n", format);
            }
            text += std::format("    uint packed = xe_om_encode_{}(r);\n", format);
            if (needsOld && keep != 0)
                text += std::format("    return (old & 0x{:08X}u) | (packed & 0x{:08X}u);\n}}\n", keep, ~keep);
            else
                text += "    return packed;\n}\n";
        }

        // Depth/stencil of one sample: old dword (S 0-7, Z 8-31) + new 24-bit Z -> new dword and pass/fail.
        const uint32_t dc = key.depthControl;
        const bool stencil = dc & 1, zEnable = (dc >> 1) & 1, zWrite = (dc >> 2) & 1;
        const bool depthStencil = stencil || zEnable;
        if (depthStencil)
        {
            text += "\nuint xe_om_depth_stencil(uint old, uint z, bool front, out bool passed)\n{\n";
            text += "    uint oldZ = old >> 8, s = old & 0xFFu;\n";
            text += std::format("    bool depthPass = {};\n",
                                zEnable ? CompareExpression((dc >> 4) & 7, "z", "oldZ", false) : std::string("true"));
            text += "    bool stencilPass = true;\n";
            if (stencil)
            {
                const bool twoSided = key.polygonal && ((dc >> 7) & 1);
                if (twoSided)
                {
                    text += "    if (front)\n    {\n" + StencilFace(dc, 8, "xe_om_surface.z") + "    }\n";
                    text += "    else\n    {\n" + StencilFace(dc, 20, "xe_om_surface.w") + "    }\n";
                }
                else
                {
                    text += "    {\n" + StencilFace(dc, 8, "xe_om_surface.z") + "    }\n";
                }
            }
            text += "    passed = stencilPass && depthPass;\n";
            if (zEnable && zWrite)
                text += "    if (passed)\n        oldZ = z;\n";
            text += "    return (oldZ << 8) | s;\n}\n";

            if (key.depthFormat == 0)
            {
                // unorm24, round to nearest even (06a §6.6).
                text += R"HLSL(
uint xe_om_depth_encode(float z)
{
    precise float t = saturate(z) * 16777215.0;
    return uint(round(t));
}
)HLSL";
            }
            else
            {
                // 20e4 (d3dref9 CFloat24 with round to nearest even, 06a §6.6).
                text += R"HLSL(
uint xe_om_depth_encode(float z)
{
    uint u = asuint(z);
    if ((u & 0x80000000u) != 0u || (u & 0x7FFFFFFFu) == 0u || (u & 0x7FFFFFFFu) > 0x7F800000u)
        return 0u;
    if (u >= 0x3FFFFFF8u)
        return 0xFFFFFFu;
    if (u < 0x38800000u)
    {
        uint shift = min(113u - (u >> 23), 24u);
        u = (0x800000u | (u & 0x7FFFFFu)) >> shift;
    }
    else
    {
        u -= 112u << 23;
    }
    u += 3u + ((u >> 3) & 1u);
    return (u >> 3) & 0xFFFFFFu;
}
)HLSL";
            }
        }

        if (target == OutputMergerTarget::kHarness)
            return text;

        // ---- Pixel shader part: the ROV and the per-pixel sequence (alpha test, depth/stencil, colour) ----------
        text += "\nRasterizerOrderedByteAddressBuffer xe_edram : register(u0);\n";
        const uint32_t samples = 1u << key.msaa;
        text += "\nvoid xe_output_merger(float4 position, float2 dz, float4 colors[4], float shaderDepth, bool frontFace,"
                " uint hostCoverage)\n{\n";
        if (key.runsShader && (key.alphaTest & 8))
        {
            // Alpha test on RT0's alpha before the exponent bias; failing also skips depth/stencil (01b §2).
            text += std::format("    if (!({}))\n        return;\n",
                                CompareExpression(key.alphaTest & 7, "colors[0].w", "xe_om_alpha_poly.x", true));
        }
        text += std::format("    bool front = {};\n", key.polygonal ? "frontFace" : "true");
        text += "    uint2 pixel = uint2(position.xy);\n";
        text += "    uint pitch = xe_om_surface.x;\n";
        if (depthStencil)
        {
            if (key.shaderDepth)
                text += "    float zc = shaderDepth;\n";
            else
            {
                text += "    float zc = position.z;\n";
                if (key.polyOffsetFront || key.polyOffsetBack)
                {
                    // offset = max slope (per pixel) * scale / 16 (slope in 1/16-pixel units) + offset (01a §5).
                    text += "    {\n        float slope = xe_max(xe_abs(dz.x), xe_abs(dz.y));\n";
                    text += "        float2 so = float2(0.0, 0.0);\n";
                    if (key.polyOffsetFront)
                        text += "        if (front) so = xe_om_alpha_poly.yz;\n";
                    if (key.polyOffsetBack)
                        text += "        if (!front) so = float2(xe_om_alpha_poly.w, xe_om_poly_back.x);\n";
                    text += "        precise float o = slope * (so.x * 0.0625) + so.y;\n";
                    text += "        precise float zo = zc + o;\n        zc = zo;\n    }\n";
                }
            }
        }
        // Guest sample offsets (1/16 pixel, XDK) and the host sample (standard 4x pattern) each maps to.
        static const int kOffsets2x[2][2] = { { 4, -4 }, { -4, 4 } };
        static const int kOffsets4x[4][2] = { { -2, -2 }, { 2, 2 }, { -6, 6 }, { 6, -6 } };
        static const uint32_t kHost2x[2] = { 1, 2 }, kHost4x[4] = { 0, 3, 2, 1 };
        for (uint32_t s = 0; s < samples; s++)
        {
            text += std::format("    // sample {}\n", s);
            if (key.msaa != 0)
                text += std::format("    if ((hostCoverage & {}u) != 0u)\n", 1u << (key.msaa == 1 ? kHost2x[s] : kHost4x[s]));
            text += "    {\n";
            text += std::format("        uint2 q = xe_edram_sample(pixel, {}u, {}u);\n", s, key.msaa);
            text += "        bool passed = true;\n";
            if (depthStencil)
            {
                if (key.msaa != 0 && !key.shaderDepth)
                {
                    // Translated pixel shaders run per sample (shader_translator.cpp): position.z is then the depth at
                    // the host sample, so the guest sample's depth is offset from there; the depth-only shader runs per
                    // pixel (position.z at the centre).
                    static const int kHostOffsets[4][2] = { { -2, -6 }, { 6, -2 }, { -6, 2 }, { 2, 6 } }; // D3D 4x
                    const int* o = key.msaa == 1 ? kOffsets2x[s] : kOffsets4x[s];
                    const int* h = key.runsShader ? kHostOffsets[key.msaa == 1 ? kHost2x[s] : kHost4x[s]] : nullptr;
                    const int dx = o[0] - (h ? h[0] : 0), dy = o[1] - (h ? h[1] : 0);
                    text += std::format("        precise float zs = zc + dz.x * {} + dz.y * {};\n", dx / 16.0, dy / 16.0);
                }
                else
                    text += "        float zs = zc;\n";
                text += "        uint daddr = xe_edram_address(xe_om_surface.y, pitch, q, true);\n";
                text += "        uint dold = xe_edram.Load(daddr);\n";
                text += "        uint dnew = xe_om_depth_stencil(dold, xe_om_depth_encode(zs), front, passed);\n";
                text += "        if (dnew != dold)\n            xe_edram.Store(daddr, dnew);\n";
            }
            bool anyColor = false;
            for (uint32_t rt = 0; rt < 4; rt++)
                anyColor |= key.colorFormat[rt] != kRtDisabled;
            if (anyColor)
            {
                text += "        if (passed)\n        {\n";
                for (uint32_t rt = 0; rt < 4; rt++)
                {
                    if (key.colorFormat[rt] == kRtDisabled)
                        continue;
                    text += std::format("            uint a{} = xe_edram_address(xe_om_color_base.{}, pitch, q, false);\n",
                                        rt, kComponents[rt]);
                    text += std::format("            xe_edram.Store(a{}, xe_om_color_{}(colors[{}], xe_edram.Load(a{})));\n",
                                        rt, rt, rt, rt);
                }
                text += "        }\n";
            }
            text += "    }\n";
        }
        text += "}\n";

        if (!key.runsShader)
        {
            // Depth-only mode / no pixel shader: only the depth/stencil part runs (Xenia's assumption, 01b §1).
            text += "\nstruct XePixelInput\n{\n    float4 position : SV_Position;\n    bool frontFace : SV_IsFrontFace;\n";
            if (key.msaa != 0)
                text += "    uint coverage : SV_Coverage;\n";
            text += "};\n\nvoid main(XePixelInput input)\n{\n";
            text += "    float2 dz = float2(ddx(input.position.z), ddy(input.position.z));\n";
            text += "    float4 colors[4] = { (0.0).xxxx, (0.0).xxxx, (0.0).xxxx, (0.0).xxxx };\n";
            text += std::format("    xe_output_merger(input.position, dz, colors, 0.0, input.frontFace, {});\n}}\n",
                                key.msaa != 0 ? "input.coverage" : "1u");
        }
        return text;
    }
}
