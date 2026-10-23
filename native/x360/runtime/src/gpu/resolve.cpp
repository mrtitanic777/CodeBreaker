// Resolve decoding and compute-shader generation (see resolve.h). Spec: analysis/xenos/07_resolve.md (§4 rectangle,
// §5 source, §6 destination, §7 conversion, §9 clears). Xenia (BSD-3, Nexia's draw_util.cc / resolve shaders) was
// read for behaviour; the code is our own. Float operations go through the shader library's helpers.

#include "gpu/resolve.h"

#include "gpu/gpu.h"
#include "gpu/output_merger.h"
#include "log.h"
#include "memory.h"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>

namespace gpu
{
    namespace
    {
        uint32_t Reg(uint32_t index) { return ReadRegister(index); }
        int32_t SignExtend(uint32_t value, uint32_t bits)
        {
            const uint32_t shift = 32 - bits;
            return int32_t(value << shift) >> shift;
        }

        constexpr uint32_t kColorInfo[4] = { 0x2001, 0x2003, 0x2004, 0x2005 };

        uint32_t SwapEndian(uint32_t v, uint32_t endian)
        {
            switch (endian & 3)
            {
            case 1: return ((v & 0x00FF00FFu) << 8) | ((v >> 8) & 0x00FF00FFu);
            case 2: return __builtin_bswap32(v);
            case 3: return (v << 16) | (v >> 16);
            default: return v;
            }
        }

        // Direct3D 11 float -> 16.8 fixed point (D3D11.3 functional spec §3.2.4.1; as Xenia's FloatToD3D11Fixed16p8):
        // NaN and |f| < 1/512 -> 0, saturate to the 16.8 range, round to nearest even.
        int32_t FloatToFixed16p8(float f)
        {
            if (!(std::fabs(f) >= 1.0f / 512.0f))
                return 0;
            if (f >= 32768.0f - 1.0f / 256.0f)
                return (1 << 23) - 1;
            if (f <= -32768.0f)
                return -32768 * 256;
            const uint32_t bits = std::bit_cast<uint32_t>(f);
            int32_t result = int32_t((bits & 0x7FFFFFu) | 0x800000u);
            if (bits >> 31)
                result = -result;
            const int32_t exponent = int32_t((bits >> 23) & 0xFF) - 127;
            const uint32_t extra = uint32_t(15 - exponent);
            if (extra)
            {
                uint32_t u = uint32_t(result);
                u += (1u << (extra - 1)) - 1 + ((u >> extra) & 1);
                result = int32_t(u) >> extra;
            }
            return result;
        }

        // Fields of a 32bpp destination format, in R, G, B, A order (07 §7.3 step 6); bits 0 = channel absent.
        struct PackLayout
        {
            uint32_t bits[4], shift[4];
            bool float32;   // k_32_FLOAT: the dword is R's float bits
        };

        bool DestLayout(uint32_t format, PackLayout& layout)
        {
            layout = {};
            switch (format)
            {
            case 6: case 14: case 50: // k_8_8_8_8, k_8_8_8_8_A, k_8_8_8_8_AS_16_16_16_16
                layout = { { 8, 8, 8, 8 }, { 0, 8, 16, 24 }, false };
                return true;
            case 7: case 54: // k_2_10_10_10(_AS_16_16_16_16)
                layout = { { 10, 10, 10, 2 }, { 0, 10, 20, 30 }, false };
                return true;
            case 16: case 55: // k_10_11_11(_AS_16_16_16_16)
                layout = { { 11, 11, 10, 0 }, { 0, 11, 22, 0 }, false };
                return true;
            case 17: case 56: // k_11_11_10(_AS_16_16_16_16)
                layout = { { 10, 11, 11, 0 }, { 0, 10, 21, 0 }, false };
                return true;
            case 13: case 25: // k_16_16_EDRAM, k_16_16
                layout = { { 16, 16, 0, 0 }, { 0, 16, 0, 0 }, false };
                return true;
            case 36: // k_32_FLOAT
                layout.float32 = true;
                return true;
            default:
                return false;
            }
        }

        // RT formats whose bits a destination texture format stores unchanged (07 §7.1, Xenia's list).
        bool BitwiseEquivalent(uint32_t rtFormat, uint32_t destFormat)
        {
            switch (rtFormat)
            {
            case 0: case 1: return destFormat == 6 || destFormat == 14 || destFormat == 50;
            case 2: case 10: return destFormat == 7 || destFormat == 54;
            case 6: return destFormat == 31;
            case 7: return destFormat == 32;
            case 14: return destFormat == 36;
            case 15: return destFormat == 37;
            default: return false;
            }
        }

        bool IsFloatTextureFormat(uint32_t format)
        {
            switch (format)
            {
            case 30: case 31: case 32: case 36: case 37: case 38: case 57: return true;
            default: return false;
            }
        }

        // copy_sample_select made valid for the surface (07 §5.3, Xenia's SanitizeCopySampleSelect).
        uint32_t SanitizeSelect(uint32_t select, uint32_t msaa, bool depth)
        {
            if (msaa == 2)
            {
                select = std::min(select, 6u);
                if (depth)
                    select = select == 4 || select == 6 ? 0 : select == 5 ? 2 : select;
                return select;
            }
            if (msaa == 1)
            {
                if (select == 2 || select == 3)
                    select -= 2;
                else if (select > 4)
                    select = 4;
                if (depth && select == 4)
                    select = 0;
                return select;
            }
            return 0;
        }

        const char kChannel[] = "xyzw";
    }

    uint32_t TiledOffset2D(uint32_t x, uint32_t y, uint32_t pitchMacro, uint32_t bytesLog2)
    {
        const uint32_t outer = ((y >> 5) * pitchMacro + (x >> 5)) << 6;
        const uint32_t inner = (((y >> 1) & 7) << 3) | (x & 7);
        const uint32_t oi = (outer | inner) << bytesLog2;
        const uint32_t bank = (y >> 4) & 1;
        const uint32_t pipe = ((x >> 3) & 3) ^ (((y >> 3) & 1) << 1);
        return ((y & 1) << 4) | (pipe << 6) | (bank << 11) | (oi & 15) | (((oi >> 4) & 1) << 5) | (((oi >> 5) & 7) << 8) |
               ((oi >> 8) << 12);
    }

    void ResolveExtent(const ResolveConstants& c, uint32_t& start, uint32_t& end)
    {
        // 4 bytes per pixel: one 32x32 macro tile is 4 KiB.
        const uint32_t x1 = c.x0 + c.width, y1 = c.y0 + c.height;
        start = c.destBase + TiledOffset2D(c.x0 & ~31u, c.y0 & ~31u, c.destPitchMacro, 2);
        end = c.destBase + TiledOffset2D((x1 - 1) & ~31u, (y1 - 1) & ~31u, c.destPitchMacro, 2) + 0x1000;
    }

    ResolveInfo DecodeResolve()
    {
        ResolveInfo info;
        const uint32_t control = Reg(0x2318);
        info.copyControl = control;
        info.destBaseRegister = Reg(0x2319);
        info.destPitchRegister = Reg(0x231A);
        info.destInfo = Reg(0x231B);
        const uint32_t command = (control >> 20) & 3;
        if (command >= 2)
            Fatal("GPU: resolve copy command {} ({}) is not implemented (RB_COPY_CONTROL {:08X})", command,
                  command == 2 ? "1_1_1_1 compare" : "NULL", control);

        // Rectangle (07 §4): three vertices in vertex fetch constant 0, written by the CPU.
        const uint32_t fetch0 = Reg(0x4800), fetch1 = Reg(0x4801);
        if ((fetch0 & 3) != 3 || ((fetch1 >> 2) & 0xFFFFFF) != 6)
            Fatal("GPU: resolve without a 6-dword vertex fetch constant 0 ({:08X} {:08X})", fetch0, fetch1);
        const uint32_t vertexAddress = fetch0 & 0xFFFFFFFCu;
        if (vertexAddress + 24 > guest::kPhysicalSize)
            Fatal("GPU: resolve vertices at {:08X} are outside physical memory", vertexAddress);
        const uint8_t* vertices = guest::g_memory.Translate(guest::kPhysicalWindow64k + vertexAddress);
        const float half = (Reg(0x2302) & 1) == 0 ? 0.5f : 0.0f; // PA_SU_VTX_CNTL.PixCenter ZERO: undo D3D's -0.5
        int32_t fixed[6];
        for (uint32_t i = 0; i < 6; i++)
        {
            uint32_t raw;
            std::memcpy(&raw, vertices + 4 * i, 4);
            const float v = std::bit_cast<float>(SwapEndian(raw, fetch1 & 3));
            const float shifted = v + half;
            fixed[i] = FloatToFixed16p8(shifted);
        }
        int32_t x0 = std::min({ fixed[0], fixed[2], fixed[4] }), y0 = std::min({ fixed[1], fixed[3], fixed[5] });
        int32_t x1 = std::max({ fixed[0], fixed[2], fixed[4] }), y1 = std::max({ fixed[1], fixed[3], fixed[5] });
        x0 = (x0 + 127) >> 8; // top-left rule
        y0 = (y0 + 127) >> 8;
        x1 = (x1 + 127) >> 8;
        y1 = (y1 + 127) >> 8;
        const uint32_t windowOffset = Reg(0x2080);
        const int32_t windowX = SignExtend(windowOffset & 0x7FFF, 15), windowY = SignExtend((windowOffset >> 16) & 0x7FFF, 15);
        if ((Reg(0x2205) >> 16) & 1) // PA_SU_SC_MODE_CNTL.vtx_window_offset_enable
        {
            x0 += windowX;
            x1 += windowX;
            y0 += windowY;
            y1 += windowY;
        }
        // Window scissor (+ window offset unless disabled) intersected with the screen scissor, kept non-negative.
        const uint32_t scissorTl = Reg(0x2081), scissorBr = Reg(0x2082);
        int32_t left = scissorTl & 0x3FFF, top = (scissorTl >> 16) & 0x3FFF;
        int32_t right = scissorBr & 0x3FFF, bottom = (scissorBr >> 16) & 0x3FFF;
        if (!(scissorTl >> 31))
        {
            left += windowX;
            right += windowX;
            top += windowY;
            bottom += windowY;
        }
        const uint32_t screenTl = Reg(0x200E), screenBr = Reg(0x200F);
        left = std::max(left, SignExtend(screenTl & 0x7FFF, 15));
        top = std::max(top, SignExtend((screenTl >> 16) & 0x7FFF, 15));
        right = std::min(right, SignExtend(screenBr & 0x7FFF, 15));
        bottom = std::min(bottom, SignExtend((screenBr >> 16) & 0x7FFF, 15));
        left = std::max(left, 0);
        top = std::max(top, 0);
        right = std::max(right, left);
        bottom = std::max(bottom, top);
        x0 = std::clamp(x0, left, right);
        x1 = std::clamp(x1, left, right);
        y0 = std::clamp(y0, top, bottom);
        y1 = std::clamp(y1, top, bottom);
        // 8x8 granularity, expanding (D3D aligns the corners itself).
        x0 &= ~7;
        y0 &= ~7;
        x1 = (x1 + 7) & ~7;
        y1 = (y1 + 7) & ~7;
        const uint32_t surfaceInfo = Reg(0x2000), pitch = surfaceInfo & 0x3FFF, msaa = (surfaceInfo >> 16) & 3;
        if (msaa == 3)
            Fatal("GPU: resolve with RB_SURFACE_INFO MsaaSamples 3 (undefined, {:08X})", surfaceInfo);
        const int32_t pitch8 = int32_t(pitch & ~7u);
        x0 = std::min(x0, pitch8);
        x1 = std::min(x1, pitch8);
        if (y1 - y0 > 16376)
            y1 = y0 + 16376;
        if (x0 >= x1 || y0 >= y1)
            return info; // nothing to copy or clear
        info.empty = false;
        info.x0 = uint32_t(x0);
        info.y0 = uint32_t(y0);
        info.x1 = uint32_t(x1);
        info.y1 = uint32_t(y1);
        const uint32_t width = info.x1 - info.x0, height = info.y1 - info.y0;

        // Source surface (07 §5.1).
        const uint32_t select = control & 7;
        if (select > 4)
            Fatal("GPU: resolve copy_src_select {} is undefined (RB_COPY_CONTROL {:08X})", select, control);
        info.sourceSelect = select;
        const bool depth = select == 4;
        const uint32_t pitchTiles = (pitch * (msaa == 2 ? 2 : 1) + 79) / 80;
        const uint32_t depthInfo = Reg(0x2002), depthBase = depthInfo & 0x7FF, depthFormat = (depthInfo >> 16) & 1;
        uint32_t sourceBase = depthBase, sourceFormat = 0;
        if (!depth)
        {
            const uint32_t colorInfo = Reg(kColorInfo[select]);
            sourceBase = colorInfo & 0x7FF; // 11-bit periodic addressing (06a §3.3)
            sourceFormat = (colorInfo >> 16) & 0xF;
            if (!OutputMergerSupportsColorFormat(sourceFormat))
                Fatal("GPU: resolve from render target {} format {} ({}) is not implemented", select, sourceFormat,
                      EdramColorFormatName(sourceFormat));
        }

        // Copy (07 §6, §7).
        ResolveKey& key = info.key;
        key.msaa = msaa;
        key.select = SanitizeSelect((control >> 4) & 7, msaa, depth);
        key.depth = depth;
        key.sourceFormat = sourceFormat;
        const uint32_t destInfo = info.destInfo;
        const uint32_t endian = destInfo & 7, array = (destInfo >> 3) & 1, destFormat = (destInfo >> 7) & 0x3F;
        const uint32_t number = (destInfo >> 13) & 7;
        if (array)
            Fatal("GPU: resolve to a 3D slice is not implemented (RB_COPY_DEST_INFO {:08X})", destInfo);
        if (endian > 3)
            Fatal("GPU: resolve with endian {} (8in64/8in128) to a 32bpp format is not implemented ({:08X})", endian,
                  destInfo);
        key.endian = endian;
        if (depth)
        {
            // Depth is always a raw copy in the depth buffer's format (07 §7.4).
            key.destFormat = depthFormat ? 23 : 22;
            key.raw = true;
        }
        else
        {
            PackLayout layout;
            if (!DestLayout(destFormat, layout))
                Fatal("GPU: resolve to texture format {} is not implemented (RB_COPY_DEST_INFO {:08X})", destFormat, destInfo);
            if (layout.float32 ? number != 7 : number > 3)
                Fatal("GPU: resolve to texture format {} with number format {} is not implemented ({:08X})", destFormat,
                      number, destInfo);
            key.destFormat = destFormat;
            key.destNumber = number;
            key.swap = (destInfo >> 24) & 1;
            key.expBias = SignExtend((destInfo >> 16) & 0x3F, 6);
            const bool numberMatches = number == (IsFloatTextureFormat(destFormat) ? 7u : 0u);
            key.raw = key.expBias == 0 && key.select < 4 && BitwiseEquivalent(sourceFormat, destFormat) && numberMatches;
        }
        ResolveConstants& c = info.constants;
        c.x0 = info.x0;
        c.y0 = info.y0;
        c.width = width;
        c.height = height;
        c.sourceBase = sourceBase;
        c.pitchTiles = pitchTiles;
        c.destBase = info.destBaseRegister;
        const uint32_t destPitch = info.destPitchRegister & 0x3FFF;
        c.destPitchMacro = (destPitch + 31) >> 5;
        ResolveExtent(c, info.extentStart, info.extentEnd);
        if (c.destBase & 0xFFFu)
            Fatal("GPU: resolve destination {:08X} is not 4 KiB aligned", c.destBase);
        if (uint64_t(info.extentEnd) > guest::kPhysicalSize || info.extentEnd < info.extentStart)
            Fatal("GPU: resolve destination {:08X}-{:08X} is outside physical memory", info.extentStart, info.extentEnd);
        info.copy = true;

        // Clears of the same rectangle after the copy (07 §9).
        auto clear = [&](ClearConstants& cc, uint32_t base, bool isDepth, uint32_t value) {
            cc = { info.x0, info.y0, width, height, base, pitchTiles, msaa, isDepth ? 1u : 0u, value, 0, 0, 0 };
        };
        if ((control >> 9) & 1)
        {
            info.clearDepth = true;
            clear(info.depthClear, depthBase, true, Reg(0x231D));
        }
        if (((control >> 8) & 1) && !depth)
        {
            info.clearColor = true;
            if (Reg(0x231F) != Reg(0x231E))
                Fatal("GPU: 32bpp resolve clear with RB_COLOR_CLEAR {:08X} != RB_COLOR_CLEAR_LO {:08X}", Reg(0x231E),
                      Reg(0x231F));
            clear(info.colorClear, sourceBase, false, Reg(0x231E));
        }
        return info;
    }

    std::string DescribeResolve(const ResolveInfo& info)
    {
        const ResolveKey& k = info.key;
        std::string text = std::format("resolve {} {}x MSAA select {} -> ", k.depth ? "depth" : EdramColorFormatName(k.sourceFormat),
                                       1u << k.msaa, k.select);
        text += std::format("format {} number {} swap {} endian {} bias {} ({} path), dest {:08X} pitch {:08X}, rect ({},{})-({},{})",
                            k.destFormat, k.destNumber, int(k.swap), k.endian, k.expBias, k.raw ? "raw" : "converting",
                            info.destBaseRegister, info.destPitchRegister, info.x0, info.y0, info.x1, info.y1);
        if (info.clearColor)
            text += std::format(", colour clear {:08X}", info.colorClear.value);
        if (info.clearDepth)
            text += std::format(", depth clear {:08X}", info.depthClear.value);
        return text;
    }

    namespace
    {
        std::string Bindings(ResolveTarget target)
        {
            std::string text = R"HLSL(
// ---- Resolve (resolve.cpp) ---------------------------------------------------------------------------------------
cbuffer XeResolveConstants : register(b{})
{
    uint4 xe_rs_rect;      // x0, y0, width, height (pixels)
    uint4 xe_rs_surface;   // EDRAM base tile, pitch in tiles (32bpp); clear: msaa, depth
    uint4 xe_rs_dest;      // copy: destination base (physical bytes), pitch / 32; clear: x = value
};
)HLSL";
            text.replace(text.find("b{}"), 3, target == ResolveTarget::kHarness ? "b4" : "b5");
            if (target == ResolveTarget::kHarness)
                text += std::format(R"HLSL(
RWByteAddressBuffer xe_test_io : register(u0);
uint xe_rs_edram_load(uint a) {{ return xe_test_io.Load(a); }}
void xe_rs_edram_store(uint a, uint v) {{ xe_test_io.Store(a, v); }}
void xe_rs_memory_store(uint a, uint v) {{ xe_test_io.Store({}u + a, v); }}
)HLSL", kEdramBytes);
            else
                text += R"HLSL(
RWByteAddressBuffer xe_rs_edram : register(u0);
RWByteAddressBuffer xe_rs_memory : register(u1);
uint xe_rs_edram_load(uint a) { return xe_rs_edram.Load(a); }
void xe_rs_edram_store(uint a, uint v) { xe_rs_edram.Store(a, v); }
void xe_rs_memory_store(uint a, uint v) { xe_rs_memory.Store(a, v); }
)HLSL";
            return text + kEdramAddressHlsl;
        }

        // Fixed-point pack of one channel (07 §7.3 step 6), as an HLSL expression returning uint.
        std::string PackField(const std::string& v, uint32_t bits, uint32_t number)
        {
            const uint32_t mask = bits == 32 ? 0xFFFFFFFFu : (1u << bits) - 1;
            switch (number)
            {
            case 0: return std::format("xe_rs_unorm({}, {:.1f})", v, double(mask));
            case 1: return std::format("xe_rs_snorm({}, {:.1f}, 0x{:X}u)", v, double((1u << (bits - 1)) - 1), mask);
            case 2: return std::format("xe_rs_uint({}, {:.1f})", v, double(mask));
            default:
                return std::format("xe_rs_sint({}, {:.1f}, {:.1f}, 0x{:X}u)", v, -double(1u << (bits - 1)),
                                   double((1u << (bits - 1)) - 1), mask);
            }
        }
    }

    std::string ResolveHlsl(const ResolveKey& key, ResolveTarget target)
    {
        std::string text = Bindings(target);
        text += R"HLSL(
uint xe_rs_tiled(uint2 p, uint pitchMacro)   // 2D tiled byte offset of a 4-byte block (05b §2.2)
{
    uint outer = ((p.y >> 5) * pitchMacro + (p.x >> 5)) << 6;
    uint inner = (((p.y >> 1) & 7u) << 3) | (p.x & 7u);
    uint oi = (outer | inner) << 2;
    uint bank = (p.y >> 4) & 1u;
    uint pipe = ((p.x >> 3) & 3u) ^ (((p.y >> 3) & 1u) << 1);
    return ((p.y & 1u) << 4) | (pipe << 6) | (bank << 11) | (oi & 15u) | (((oi >> 4) & 1u) << 5) |
           (((oi >> 5) & 7u) << 8) | ((oi >> 8) << 12);
}

// Packs (round half up / half away from zero, as Xenia). Clamps use the library's compare semantics.
uint xe_rs_unorm(float v, float m)
{
    precise float t = saturate(v) * m;
    precise float h = t + 0.5;
    return uint(floor(h));
}
uint xe_rs_snorm(float v, float m, uint mask)
{
    float c = xe_min(xe_max(v, -1.0), 1.0);
    precise float t = c * m;
    precise float h = t + asfloat((asuint(t) & 0x80000000u) | 0x3F000000u);
    return uint(int(h)) & mask;
}
uint xe_rs_uint(float v, float m)
{
    float c = xe_min(xe_max(v, 0.0), m);
    precise float h = c + 0.5;
    return uint(h);
}
uint xe_rs_sint(float v, float lo, float hi, uint mask)
{
    float c = xe_min(xe_max(v, lo), hi);
    precise float h = c + asfloat((asuint(c) & 0x80000000u) | 0x3F000000u);
    return uint(int(h)) & mask;
}
)HLSL";
        // Decode of one EDRAM dword (as the output merger, plus Xenia's -32 floor for 16_16).
        if (!key.raw)
        {
            if (key.sourceFormat == 0)
                text += R"HLSL(
float4 xe_rs_decode(uint v)
{
    precise float4 r = float4((uint4(v, v >> 8, v >> 16, v >> 24) & 0xFFu)) * asfloat(0x3B808081u); // n * (1/255)
    return r;
}
)HLSL";
            else if (key.sourceFormat == 4)
                text += R"HLSL(
float4 xe_rs_decode(uint v)
{
    precise float2 r = float2(int2(v << 16, v) >> 16) * asfloat(0x3A800100u); // n * (32/32767)
    return float4(xe_max(r.x, -32.0), xe_max(r.y, -32.0), 0.0, 0.0);
}
)HLSL";
            else
                Fatal("GPU: resolve decode of EDRAM format {} is not implemented", key.sourceFormat);
        }

        // Samples read (07 §5.3): one, a pair or all four, in sample order.
        uint32_t samples[4], count = 1;
        samples[0] = key.select < 4 ? key.select : key.select == 5 ? 2 : 0;
        if (key.select >= 4)
        {
            samples[1] = samples[0] + 1;
            count = 2;
            if (key.select == 6)
            {
                samples[2] = 2;
                samples[3] = 3;
                count = 4;
            }
        }

        // A host render target as the source: the dword of guest sample s is the one its store pass writes, read from
        // the host sample holding it (host_targets_hlsl.h: 2x samples 0/1 are host 1/2, 4x 0..3 are host 0, 3, 2, 1).
        if (key.hostSource)
        {
            if (target != ResolveTarget::kRenderer)
                Fatal("GPU: a resolve from a host render target runs in the renderer only");
            const bool ms = key.hostSource == 2 || key.hostSource == 4;
            const std::string load = ms ? "Load(p, sample)" : "Load(uint3(p, 0u))";
            if (key.hostSource <= 2)
                text += std::format(R"HLSL(
{0}<uint4> xe_rs_host_color : register(t0);
uint xe_rs_source(uint2 p, uint sample)
{{
    uint4 c = xe_rs_host_color.{1};
    return c.x | (c.y << 8) | (c.z << 16) | (c.w << 24);
}}
)HLSL", ms ? "Texture2DMS" : "Texture2D", load);
            else
                text += std::format(R"HLSL(
{0}<float> xe_rs_host_depth : register(t0);
{0}<uint2> xe_rs_host_stencil : register(t1);
uint xe_rs_source(uint2 p, uint sample)
{{
    float z = xe_rs_host_depth.{1};
    uint st = xe_rs_host_stencil.{1}.y;
    return (uint(round(saturate(z) * 16777215.0)) << 8) | (st & 0xFFu);
}}
)HLSL", ms ? "Texture2DMS" : "Texture2D", load);
        }

        text += R"HLSL(
[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= xe_rs_rect.z || id.y >= xe_rs_rect.w)
        return;
    uint2 p = xe_rs_rect.xy + id.xy;
)HLSL";
        static const uint32_t kHostSample2x[2] = { 1, 2 }, kHostSample4x[4] = { 0, 3, 2, 1 };
        for (uint32_t i = 0; i < count; i++)
            if (key.hostSource)
                text += std::format("    uint d{} = xe_rs_source(p, {}u);\n", i,
                                    key.msaa == 1 ? kHostSample2x[samples[i] & 1] : key.msaa == 2 ? kHostSample4x[samples[i]] : 0);
            else
                text += std::format("    uint d{} = xe_rs_edram_load(xe_edram_address(xe_rs_surface.x, xe_rs_surface.y, "
                                    "xe_edram_sample(p, {}u, {}u), {}));\n", i, samples[i], key.msaa, key.depth ? "true" : "false");
        if (key.raw)
        {
            text += "    uint result = d0;\n";
            if (key.swap && !key.depth)
            {
                if (key.sourceFormat == 0 || key.sourceFormat == 1)
                    text += "    result = (result & 0xFF00FF00u) | ((result & 0xFFu) << 16) | ((result >> 16) & 0xFFu);\n";
                else if (key.sourceFormat == 2 || key.sourceFormat == 3 || key.sourceFormat == 10 || key.sourceFormat == 12)
                    text += "    result = (result & ~0x3FF003FFu) | ((result & 0x3FFu) << 20) | ((result >> 20) & 0x3FFu);\n";
            }
        }
        else
        {
            text += "    precise float4 c = xe_rs_decode(d0);\n";
            for (uint32_t i = 1; i < count; i++)
                text += std::format("    c = c + xe_rs_decode(d{});\n", i);
            // 2^exp_bias / samples, an exact power of two (07 §7.3 steps 3-4).
            const int32_t exponent = key.expBias + 127 - (count == 4 ? 2 : count == 2 ? 1 : 0);
            if (exponent != 127)
                text += std::format("    c = c * asfloat(0x{:08X}u);\n", uint32_t(exponent) << 23);
            if (key.swap)
                text += "    c = c.zyxw;\n";
            PackLayout layout;
            if (!DestLayout(key.destFormat, layout))
                Fatal("GPU: resolve to texture format {} is not implemented", key.destFormat);
            if (layout.float32)
            {
                text += "    uint result = asuint(c.x);\n";
            }
            else
            {
                text += "    uint result = 0u;\n";
                for (uint32_t ch = 0; ch < 4; ch++)
                    if (layout.bits[ch])
                        text += std::format("    result |= {} << {}u;\n",
                                            PackField(std::format("c.{}", kChannel[ch]), layout.bits[ch], key.destNumber),
                                            layout.shift[ch]);
            }
        }
        if (key.endian)
            text += std::format("    result = xe_swap(result, {}u);\n", key.endian);
        text += "    xe_rs_memory_store(xe_rs_dest.x + xe_rs_tiled(p, xe_rs_dest.y), result);\n}\n";
        return text;
    }

    std::string ClearHlsl(ResolveTarget target)
    {
        return Bindings(target) + R"HLSL(
// Every sample of every pixel of the rectangle gets the clear dword (07 §9.3).
[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= xe_rs_rect.z || id.y >= xe_rs_rect.w)
        return;
    uint2 p = xe_rs_rect.xy + id.xy;
    uint msaa = xe_rs_surface.z;
    bool depth = xe_rs_surface.w != 0u;
    uint samples = 1u << msaa;
    for (uint s = 0u; s < 4u; s++)
        if (s < samples)
            xe_rs_edram_store(xe_edram_address(xe_rs_surface.x, xe_rs_surface.y, xe_edram_sample(p, s, msaa), depth),
                              xe_rs_dest.x);
}
)HLSL";
    }
}
