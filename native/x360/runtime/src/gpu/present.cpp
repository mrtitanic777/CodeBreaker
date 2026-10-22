// Front-buffer presentation: decode, shader and CPU reference (see present.h). Spec: analysis/xenos/08_present.md
// §4 (front buffer), §5 (gamma); Xenia (BSD-3, Nexia's apply_gamma_table shader and RequestSwapTexture) was read for
// behaviour; the code is our own.

#include "gpu/present.h"

#include "gpu/gpu.h"
#include "gpu/resolve.h"
#include "log.h"
#include "memory.h"

#include <algorithm>
#include <cstring>
#include <format>

namespace gpu
{
    namespace
    {
        uint32_t Reg(uint32_t index) { return ReadRegister(index); }
    }

    PresentConstants DecodePresent(uint32_t frontBuffer, uint32_t width, uint32_t height, const DisplayGamma& gamma)
    {
        uint32_t d[6];
        for (uint32_t i = 0; i < 6; i++)
            d[i] = Reg(0x4800 + i);
        const uint32_t type = d[0] & 3, format = d[1] & 0x3F, dimension = (d[5] >> 9) & 3;
        const bool tiled = d[0] >> 31, stacked = (d[1] >> 10) & 1, border = d[3] >> 31;
        const uint32_t base = d[1] & 0x1FFFF000, pitch = ((d[0] >> 22) & 0x1FF) * 32;
        const uint32_t fetchWidth = (d[2] & 0x1FFF) + 1, fetchHeight = ((d[2] >> 13) & 0x1FFF) + 1;
        const uint32_t swizzle = (d[3] >> 1) & 0xFFF;
        const std::string fetch = std::format("{:08X} {:08X} {:08X} {:08X} {:08X} {:08X}", d[0], d[1], d[2], d[3], d[4], d[5]);

        if (type != 2 || dimension != 1 || stacked || border)
            Fatal("GPU: swap: front-buffer fetch constant is not a plain 2D texture (type {}, dimension {}, stacked {}, "
                  "border {}; {})", type, dimension, int(stacked), int(border), fetch);
        if (format == 54)
            Fatal("GPU: swap: 10-bit front buffer (k_2_10_10_10_AS_16_16_16_16, PWL gamma) is not implemented ({})", fetch);
        if (format != 6)
            Fatal("GPU: swap: front-buffer format {} is not implemented ({})", format, fetch);
        // Red, green, blue from data components Z, Y, X (LE_X8R8G8B8: memory bytes B, G, R, X); alpha is not displayed.
        if ((swizzle & 0x1FF) != (2u | 1u << 3 | 0u << 6))
            Fatal("GPU: swap: front-buffer swizzle {:03X} is not LE_X8R8G8B8's ({})", swizzle, fetch);
        if (base != frontBuffer || fetchWidth != width || fetchHeight != height)
            Fatal("GPU: swap: fetch constant 0 ({}x{} at {:08X}) does not match the swap ({}x{} at {:08X}; {})", fetchWidth,
                  fetchHeight, base, width, height, frontBuffer, fetch);
        if (pitch < width)
            Fatal("GPU: swap: front-buffer pitch {} is smaller than its width {} ({})", pitch, width, fetch);

        const uint32_t lutControl = Reg(reg::kDcLutaControl);
        if (lutControl != 0)
            Fatal("GPU: swap: DC_LUTA_CONTROL {:08X} with an 8:8:8:8 front buffer is not implemented", lutControl);
        for (uint32_t r = reg::kDcLutaBlackOffsetBlue; r < reg::kDcLutaBlackOffsetBlue + 6; r++)
            if (Reg(r) != 0)
                Fatal("GPU: swap: DC_LUTA black/white offset register {:X} = {:08X} is not implemented", r, Reg(r));

        PresentConstants c{};
        c.base = base;
        c.pitch = pitch;
        c.tiled = tiled ? 1 : 0;
        c.endian = (d[1] >> 6) & 3;
        c.width = width;
        c.height = height;
        std::memcpy(c.table, gamma.table, sizeof(c.table));
        uint32_t start, end;
        PresentExtent(c, start, end);
        if (uint64_t(end) > guest::kPhysicalSize)
            Fatal("GPU: swap: front buffer {:08X}-{:08X} is outside physical memory ({})", start, end, fetch);
        return c;
    }

    std::string DescribePresent(const PresentConstants& c)
    {
        static const char* const kEndian[] = { "none", "8in16", "8in32", "16in32" };
        return std::format("swap: front buffer {:08X} {}x{} k_8_8_8_8 {} pitch {} endian {}; fetch 0: {:08X} {:08X} {:08X} "
                           "{:08X} {:08X} {:08X}", c.base, c.width, c.height, c.tiled ? "tiled" : "linear", c.pitch,
                           kEndian[c.endian & 3], Reg(0x4800), Reg(0x4801), Reg(0x4802), Reg(0x4803), Reg(0x4804),
                           Reg(0x4805));
    }

    void SetGuestOverlay(PresentConstants& c, const GuestOverlay& o)
    {
        // XAM's overlay (xam.xex 0x81842B08 fills the descriptor from its texture's fetch constant): the format is the
        // fetch constant's (bits 0-5) alone. XAM resolves it 2D tiled, pitch the width rounded up to 32, with no endian
        // swap (RB_COPY_DEST_INFO 01000300): bytes B, G, R, A in memory (the controller buttons' colours check it). Its draws composite
        // premultiplied (blend 07010706) over the title's frame, dimmed, so the whole texture is opaque.
        if ((o.format & 0x3F) != 6)
            Fatal("GPU: swap: XAM's overlay format {} is not implemented ({}x{} at {:08X}, mode {})", o.format, o.width,
                  o.height, o.base, o.mode);
        if (o.width == 0 || o.height == 0 || o.width > 2048 || o.height > 2048)
            Fatal("GPU: swap: XAM's overlay size {}x{} at {:08X}", o.width, o.height, o.base);
        if (o.mode != 0)
            Fatal("GPU: swap: XAM's overlay mode {} is not implemented ({}x{} at {:08X})", o.mode, o.width, o.height, o.base);
        const bool whole = (o.source[0] | o.source[1] | o.source[2] | o.source[3]) == 0;
        c.ovX1 = whole ? 0 : o.source[0];
        c.ovY1 = whole ? 0 : o.source[1];
        c.ovX2 = whole ? o.width : o.source[2];
        c.ovY2 = whole ? o.height : o.source[3];
        if (c.ovX1 >= c.ovX2 || c.ovY1 >= c.ovY2 || c.ovX2 > o.width || c.ovY2 > o.height)
            Fatal("GPU: swap: XAM's overlay source rectangle {},{}-{},{} on a {}x{} texture", c.ovX1, c.ovY1, c.ovX2,
                  c.ovY2, o.width, o.height);
        static const uint32_t endian = [] { const char* v = std::getenv("CB_XAM_OVERLAY_ENDIAN"); return v ? uint32_t(std::atoi(v)) : 0u; }();
        static const uint32_t tiled = [] { const char* v = std::getenv("CB_XAM_OVERLAY_LINEAR"); return v ? 0u : 1u; }();
        c.overlay = 2;
        c.ovBase = o.base & 0x1FFFF000;
        c.ovPitch = (o.width + 31) & ~31u;
        c.ovTiled = tiled;
        c.ovEndian = endian;
        c.ovWidth = o.width;
        c.ovHeight = o.height;
        const uint32_t end = c.ovBase + (tiled ? TiledOffset2D((o.width - 1) & ~31u, (o.height - 1) & ~31u, c.ovPitch / 32, 2) + 0x1000
                                               : c.ovPitch * 4 * o.height);
        if (uint64_t(end) > guest::kPhysicalSize)
            Fatal("GPU: swap: XAM's overlay {:08X}-{:08X} is outside physical memory", c.ovBase, end);
        static bool logged = false;
        if (!logged)
        {
            logged = true;
            Log("GPU: XAM's system UI overlay: {}x{} at {:08X}, format {:08X}, mode {}, source {},{}-{},{}", o.width,
                o.height, c.ovBase, o.format, o.mode, c.ovX1, c.ovY1, c.ovX2, c.ovY2);
        }
    }

    void PresentExtent(const PresentConstants& c, uint32_t& start, uint32_t& end)
    {
        start = c.base;
        if (c.tiled)
            end = c.base + TiledOffset2D((c.width - 1) & ~31u, (c.height - 1) & ~31u, c.pitch / 32, 2) + 0x1000;
        else
            end = c.base + c.pitch * 4 * c.height;
    }

    const char kPresentHlsl[] = R"HLSL(
cbuffer PresentConstants : register(b0)
{
    uint4 xe_pr_layout;    // base, pitch (texels), tiled, endian
    uint4 xe_pr_size;      // width, height, overlay
    uint4 xe_pr_ov_layout; // XAM's overlay: base, pitch, tiled, endian
    uint4 xe_pr_ov_size;   // texture width, height
    uint4 xe_pr_ov_rect;   // source rectangle x1, y1, x2, y2
    uint4 xe_pr_gamma[64]; // display gamma table, DC_LUT_30_COLOR layout
};
ByteAddressBuffer xe_memory : register(t0);
Texture2D<uint4> xe_overlay : register(t1);
RWTexture2D<uint4> xe_output : register(u0);

uint xe_pr_tiled(uint2 p, uint pitchMacro)   // 2D tiled byte offset of a 4-byte block (05b §2.2)
{
    uint outer = ((p.y >> 5) * pitchMacro + (p.x >> 5)) << 6;
    uint inner = (((p.y >> 1) & 7u) << 3) | (p.x & 7u);
    uint oi = (outer | inner) << 2;
    uint bank = (p.y >> 4) & 1u;
    uint pipe = ((p.x >> 3) & 3u) ^ (((p.y >> 3) & 1u) << 1);
    return ((p.y & 1u) << 4) | (pipe << 6) | (bank << 11) | (oi & 15u) | (((oi >> 4) & 1u) << 5) |
           (((oi >> 5) & 7u) << 8) | ((oi >> 8) << 12);
}

uint xe_pr_swap(uint v, uint endian)
{
    if (endian == 1u)
        return ((v & 0x00FF00FFu) << 8) | ((v >> 8) & 0x00FF00FFu);
    if (endian == 2u)
        return (v << 24) | ((v & 0xFF00u) << 8) | ((v >> 8) & 0xFF00u) | (v >> 24);
    if (endian == 3u)
        return (v << 16) | (v >> 16);
    return v;
}

uint4 xe_pr_ov_texel(int2 p)                // XAM's overlay texel (R, G, B, A; memory bytes B, G, R, A), clamped to its edges
{
    uint2 q = uint2(clamp(p, int2(xe_pr_ov_rect.xy), int2(xe_pr_ov_rect.zw) - 1));
    uint offset = xe_pr_ov_layout.z != 0u ? xe_pr_tiled(q, xe_pr_ov_layout.y >> 5) : (q.y * xe_pr_ov_layout.y + q.x) << 2;
    uint v = xe_pr_swap(xe_memory.Load(xe_pr_ov_layout.x + offset), xe_pr_ov_layout.w);
    return uint4((v >> 16) & 0xFFu, (v >> 8) & 0xFFu, v & 0xFFu, v >> 24);
}

uint4 xe_pr_ov_sample(uint2 p)              // bilinear, 8-bit weights, at the frame pixel's centre
{
    int u = int(xe_pr_ov_rect.x * 256u + ((2u * p.x + 1u) * (xe_pr_ov_rect.z - xe_pr_ov_rect.x) * 256u) / (2u * xe_pr_size.x)) - 128;
    int v = int(xe_pr_ov_rect.y * 256u + ((2u * p.y + 1u) * (xe_pr_ov_rect.w - xe_pr_ov_rect.y) * 256u) / (2u * xe_pr_size.y)) - 128;
    int2 i = int2(u >> 8, v >> 8);
    uint fx = uint(u & 255), fy = uint(v & 255);
    uint4 sum = xe_pr_ov_texel(i) * ((256u - fx) * (256u - fy)) + xe_pr_ov_texel(i + int2(1, 0)) * (fx * (256u - fy)) +
                xe_pr_ov_texel(i + int2(0, 1)) * ((256u - fx) * fy) + xe_pr_ov_texel(i + int2(1, 1)) * (fx * fy);
    return (sum + 32768u) >> 16;
}

uint xe_pr_gamma_entry(uint i)
{
    return xe_pr_gamma[i >> 2][i & 3u];
}

[numthreads(16, 8, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= xe_pr_size.x || id.y >= xe_pr_size.y)
        return;
    uint offset = xe_pr_layout.z != 0u ? xe_pr_tiled(id.xy, xe_pr_layout.y >> 5) : (id.y * xe_pr_layout.y + id.x) << 2;
    uint v = xe_pr_swap(xe_memory.Load(xe_pr_layout.x + offset), xe_pr_layout.w);
    // k_8_8_8_8 shown through the swizzle (Z, Y, X): red = bits 16-23, green = 8-15, blue = 0-7; each channel's
    // 8-bit value indexes the table and takes that channel's 10-bit entry.
    uint3 c = uint3((v >> 16) & 0xFFu, (v >> 8) & 0xFFu, v & 0xFFu);
    if (xe_pr_size.z != 0u)
    {
        uint4 o = xe_pr_size.z == 2u ? xe_pr_ov_sample(id.xy) : xe_overlay.Load(int3(id.xy, 0));
        c = min(o.rgb + (c * (255u - o.a) + 127u) / 255u, 255u);
    }
    uint r = (xe_pr_gamma_entry(c.r) >> 20) & 0x3FFu;
    uint g = (xe_pr_gamma_entry(c.g) >> 10) & 0x3FFu;
    uint b = xe_pr_gamma_entry(c.b) & 0x3FFu;
    xe_output[id.xy] = uint4(r, g, b, 3u);
}
)HLSL";

    namespace
    {
        uint32_t Swap32(uint32_t v, uint32_t endian)
        {
            switch (endian)
            {
            case 1: return ((v & 0x00FF00FFu) << 8) | ((v >> 8) & 0x00FF00FFu);
            case 2: return __builtin_bswap32(v);
            case 3: return (v << 16) | (v >> 16);
            default: return v;
            }
        }

        // The shader's xe_pr_ov_sample, separately.
        void SampleGuestOverlay(const PresentConstants& c, const uint8_t* memory, uint32_t x, uint32_t y, uint8_t out[4])
        {
            auto texel = [&](int tx, int ty, uint32_t rgba[4]) {
                const uint32_t qx = uint32_t(std::clamp(tx, int(c.ovX1), int(c.ovX2) - 1));
                const uint32_t qy = uint32_t(std::clamp(ty, int(c.ovY1), int(c.ovY2) - 1));
                const uint32_t offset = c.ovTiled ? TiledOffset2D(qx, qy, c.ovPitch / 32, 2) : (qy * c.ovPitch + qx) * 4;
                uint32_t v;
                std::memcpy(&v, memory + c.ovBase + offset, 4);
                v = Swap32(v, c.ovEndian);
                rgba[0] = (v >> 16) & 0xFF;
                rgba[1] = (v >> 8) & 0xFF;
                rgba[2] = v & 0xFF;
                rgba[3] = v >> 24;
            };
            const int u = int(c.ovX1 * 256u + ((2u * x + 1u) * (c.ovX2 - c.ovX1) * 256u) / (2u * c.width)) - 128;
            const int v = int(c.ovY1 * 256u + ((2u * y + 1u) * (c.ovY2 - c.ovY1) * 256u) / (2u * c.height)) - 128;
            const int ix = u >> 8, iy = v >> 8;
            const uint32_t fx = uint32_t(u & 255), fy = uint32_t(v & 255);
            uint32_t t00[4], t10[4], t01[4], t11[4];
            texel(ix, iy, t00);
            texel(ix + 1, iy, t10);
            texel(ix, iy + 1, t01);
            texel(ix + 1, iy + 1, t11);
            for (int i = 0; i < 4; i++)
                out[i] = uint8_t((t00[i] * ((256u - fx) * (256u - fy)) + t10[i] * (fx * (256u - fy)) +
                                  t01[i] * ((256u - fx) * fy) + t11[i] * (fx * fy) + 32768u) >> 16);
        }
    }

    void ReferencePresent(const PresentConstants& c, const uint8_t* memory, const uint8_t* overlay,
                          std::vector<uint32_t>& out)
    {
        out.assign(size_t(c.width) * c.height, 0);
        for (uint32_t y = 0; y < c.height; y++)
            for (uint32_t x = 0; x < c.width; x++)
            {
                const uint32_t offset = c.tiled ? TiledOffset2D(x, y, c.pitch / 32, 2) : (y * c.pitch + x) * 4;
                uint32_t v;
                std::memcpy(&v, memory + c.base + offset, 4);
                switch (c.endian)
                {
                case 1: v = ((v & 0x00FF00FFu) << 8) | ((v >> 8) & 0x00FF00FFu); break;
                case 2: v = __builtin_bswap32(v); break;
                case 3: v = (v << 16) | (v >> 16); break;
                default: break;
                }
                uint32_t red = (v >> 16) & 0xFF, green = (v >> 8) & 0xFF, blue = v & 0xFF;
                uint8_t sampled[4];
                if (c.overlay == 2)
                    SampleGuestOverlay(c, memory, x, y, sampled);
                if (c.overlay == 2 || (c.overlay && overlay))
                {
                    const uint8_t* o = c.overlay == 2 ? sampled : overlay + (size_t(y) * c.width + x) * 4;
                    const uint32_t keep = 255u - o[3];
                    red = std::min(255u, o[0] + (red * keep + 127) / 255);
                    green = std::min(255u, o[1] + (green * keep + 127) / 255);
                    blue = std::min(255u, o[2] + (blue * keep + 127) / 255);
                }
                const uint32_t r10 = (c.table[red] >> 20) & 0x3FF;
                const uint32_t g10 = (c.table[green] >> 10) & 0x3FF;
                const uint32_t b10 = c.table[blue] & 0x3FF;
                out[size_t(y) * c.width + x] = r10 | g10 << 10 | b10 << 20 | 3u << 30;
            }
    }
}
