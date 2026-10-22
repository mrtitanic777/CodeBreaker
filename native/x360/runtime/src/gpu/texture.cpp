// Texture fetch decode, guest layout and the untile shader (see texture.h). Spec: analysis/xenos/05a (fetch constant),
// 05b §1 (formats, signs, endian), §2 (tiling, strides, packed mips, extents). Xenia (BSD-3, texture_util.cc,
// texture_load*.xesli) was read for behaviour; the XDK's XG library generated the layout test cases
// (tools/xdk_texture_ref). The code is our own.

#include "gpu/texture.h"

#include "gpu/resolve.h"

#include <algorithm>
#include <bit>
#include <format>
#include <map>
#include <tuple>

namespace gpu
{
    namespace
    {
        const char* const kTextureFormatNames[64] = {
            "1_REVERSE", "1", "8", "1_5_5_5", "5_6_5", "6_5_5", "8_8_8_8", "2_10_10_10", "8_A", "8_B", "8_8",
            "Cr_Y1_Cb_Y0_REP", "Y1_Cr_Y0_Cb_REP", "16_16_EDRAM", "8_8_8_8_A", "4_4_4_4", "10_11_11", "11_11_10",
            "DXT1", "DXT2_3", "DXT4_5", "16_16_16_16_EDRAM", "24_8", "24_8_FLOAT", "16", "16_16", "16_16_16_16",
            "16_EXPAND", "16_16_EXPAND", "16_16_16_16_EXPAND", "16_FLOAT", "16_16_FLOAT", "16_16_16_16_FLOAT", "32",
            "32_32", "32_32_32_32", "32_FLOAT", "32_32_FLOAT", "32_32_32_32_FLOAT", "32_AS_8", "32_AS_8_8",
            "16_MPEG", "16_16_MPEG", "8_INTERLACED", "32_AS_8_INTERLACED", "32_AS_8_8_INTERLACED", "16_INTERLACED",
            "16_MPEG_INTERLACED", "16_16_MPEG_INTERLACED", "DXN", "8_8_8_8_AS_16_16_16_16", "DXT1_AS_16_16_16_16",
            "DXT2_3_AS_16_16_16_16", "DXT4_5_AS_16_16_16_16", "2_10_10_10_AS_16_16_16_16", "10_11_11_AS_16_16_16_16",
            "11_11_10_AS_16_16_16_16", "32_32_32_FLOAT", "DXT3A", "DXT5A", "CTX1", "DXT3A_AS_1_1_1_1",
            "8_8_8_8_GAMMA_EDRAM", "2_10_10_10_FLOAT_EDRAM",
        };

        int32_t SignExtend(uint32_t value, uint32_t bits)
        {
            const uint32_t shift = 32 - bits;
            return int32_t(value << shift) >> shift;
        }

        uint32_t Log2Ceil(uint32_t v) { return v <= 1 ? 0 : 32 - std::countl_zero(v - 1); }
        uint32_t Log2Floor(uint32_t v) { return 31 - std::countl_zero(v); }
        uint32_t NextPow2(uint32_t v) { return 1u << Log2Ceil(v); }
        uint32_t Align(uint32_t v, uint32_t a) { return (v + a - 1) / a * a; }

        constexpr uint32_t kNotPacked = ~0u;

        // First packed level (05b §2.5): the level whose smaller side is 16 texels or less.
        uint32_t PackedLevel(const TextureFetch& f)
        {
            if (!f.packedMips)
                return kNotPacked;
            const uint32_t size = Log2Ceil(std::min(f.width, f.height));
            return size > 4 ? size - 4 : 0;
        }

        // Position of packed level `level` inside its tail, in texels (= blocks for the implemented formats).
        void PackedOffset(uint32_t width, uint32_t height, uint32_t level, uint32_t& x, uint32_t& y)
        {
            const uint32_t lw = Log2Ceil(width), lh = Log2Ceil(height);
            const uint32_t smaller = std::min(lw, lh);
            const uint32_t first = smaller > 4 ? smaller - 4 : 0;
            const uint32_t p = level - first;
            x = y = 0;
            if (p < 3)
            {
                if (lw > lh)
                    y = 16u >> p; // wider than tall: the first three packed levels stack vertically
                else
                    x = 16u >> p;
            }
            else if (lw > lh)
                x = (1u << (lw - first)) >> (p - 2);
            else
                y = (1u << (lh - first)) >> (p - 2);
        }

        struct Storage
        {
            uint32_t pitchBlocks, rowBytes, rows;
        };

        // Strides of the base level's storage or of mip storage `level` (05b §2.4).
        Storage StorageOf(const TextureFetch& f, bool base, uint32_t level)
        {
            const uint32_t bytes = TextureBytesPerTexel(f.format);
            const uint32_t rowTexels = base ? f.pitch : std::max(NextPow2(f.width) >> level, 1u);
            const uint32_t rowsTexels = base ? f.height : std::max(NextPow2(f.height) >> level, 1u);
            Storage s;
            s.pitchBlocks = Align(rowTexels, 32);
            s.rowBytes = s.pitchBlocks * bytes;
            if (!f.tiled && !base)
                s.rowBytes = Align(s.rowBytes, 256);
            s.rows = Align(rowsTexels, 32);
            return s;
        }
    }

    const char* TextureFormatName(uint32_t format) { return kTextureFormatNames[format & 63]; }

    uint32_t TextureBytesPerTexel(uint32_t format)
    {
        switch (format)
        {
        case 2: return 1;
        case 10: return 2;
        default: return 4;
        }
    }

    uint32_t TextureComponents(uint32_t format)
    {
        switch (format)
        {
        case 2: return 1;
        case 6: return 4;
        default: return 2;
        }
    }

    std::string DescribeTextureTuple(const uint32_t d[6])
    {
        auto swizzle = [](uint32_t s) {
            std::string text;
            for (uint32_t i = 0; i < 4; i++)
                text += "XYZW01??"[(s >> (3 * i)) & 7];
            return text;
        };
        static const char* const kEndian[] = { "none", "8in16", "8in32", "16in32" };
        static const char* const kDimension[] = { "1D", "2D", "3D", "cube" };
        return std::format(
            "type {} {} {} {}x{}{} {} pitch {} endian {} {} levels {}-{}{} signs {}{}{}{} {} swizzle {} exp {} "
            "filter mag {} min {} mip {} aniso {} clamp {}/{}/{} border colour {}{}{} lod bias {} grad exp {}/{} "
            "vol filter {}/{}{}{}",
            d[0] & 3, TextureFormatName(d[1] & 0x3F), kDimension[(d[5] >> 9) & 3], (d[2] & 0x1FFF) + 1,
            ((d[2] >> 13) & 0x1FFF) + 1, ((d[1] >> 10) & 1) ? std::format(" stacked {}", (d[2] >> 26) + 1) : "",
            (d[0] >> 31) ? "tiled" : "linear", ((d[0] >> 22) & 0x1FF) * 32, kEndian[(d[1] >> 6) & 3],
            ((d[5] >> 11) & 1) ? "packed" : "not packed", (d[4] >> 2) & 0xF, (d[4] >> 6) & 0xF,
            (d[5] >> 12) ? " with mip address" : (d[1] >> 12) ? "" : " no base address", (d[0] >> 2) & 3,
            (d[0] >> 4) & 3, (d[0] >> 6) & 3, (d[0] >> 8) & 3, (d[3] & 1) ? "integer" : "fraction",
            swizzle((d[3] >> 1) & 0xFFF), SignExtend((d[3] >> 13) & 0x3F, 6), (d[3] >> 19) & 3, (d[3] >> 21) & 3,
            (d[3] >> 23) & 3, (d[3] >> 25) & 7, (d[0] >> 10) & 7, (d[0] >> 13) & 7, (d[0] >> 16) & 7, d[5] & 3,
            ((d[5] >> 2) & 1) ? " force W" : "", ((d[1] >> 11) & 1) ? " clamp policy OGL" : "",
            SignExtend((d[4] >> 12) & 0x3FF, 10), SignExtend((d[4] >> 22) & 0x1F, 5), SignExtend(d[4] >> 27, 5),
            d[4] & 1, (d[4] >> 1) & 1, (d[3] >> 31) ? " border texels" : "",
            ((d[3] >> 28) & 7) ? std::format(" arbitrary filter {}", (d[3] >> 28) & 7) : "");
    }

    std::string DecodeTextureFetch(const uint32_t d[6], TextureFetch& f)
    {
        f = {};
        if ((d[0] & 3) != 2)
            return std::format("fetch constant type {} is not a texture", d[0] & 3);
        f.format = d[1] & 0x3F;
        if (f.format != 2 && f.format != 6 && f.format != 10 && f.format != 25)
            return std::format("format k_{}", TextureFormatName(f.format));
        if (((d[5] >> 9) & 3) != 1)
            return "dimension other than 2D";
        if ((d[1] >> 10) & 1)
            return "stacked (array) texture";
        if (d[3] >> 31)
            return "border texels (BorderSize 1)";
        f.endian = (d[1] >> 6) & 3;
        f.tiled = d[0] >> 31;
        f.packedMips = (d[5] >> 11) & 1;
        f.width = (d[2] & 0x1FFF) + 1;
        f.height = ((d[2] >> 13) & 0x1FFF) + 1;
        f.pitch = ((d[0] >> 22) & 0x1FF) * 32;
        if (f.pitch < f.width)
            return std::format("pitch {} smaller than the width {}", f.pitch, f.width);

        // Levels (05a §3).
        uint32_t basePage = (d[1] >> 12) & 0x1FFFF, mipPage = (d[5] >> 12) & 0x1FFFF;
        const uint32_t sizeMaxLevel = Log2Floor(std::max(f.width, f.height));
        f.minLevel = std::min((d[4] >> 2) & 0xF, sizeMaxLevel);
        f.maxLevel = std::max(std::min((d[4] >> 6) & 0xF, sizeMaxLevel), f.minLevel);
        const uint32_t originalBase = basePage;
        if (f.maxLevel != 0 && basePage == 0)
            f.minLevel = std::max(f.minLevel, 1u);
        if (f.minLevel != 0)
            basePage = 0; // the base level is never sampled
        if (f.maxLevel == 0)
            mipPage = 0;
        f.baseAddress = basePage << 12;
        if (f.maxLevel != 0)
        {
            // Mip address 0: the mips are at the base address (texture.h), which only works inside one packed tail.
            if (mipPage == 0 && !(f.packedMips && PackedLevel(f) == 0))
                return "mips without a mip address outside a packed tail";
            f.mipAddress = (mipPage ? mipPage : originalBase) << 12;
        }
        if (f.baseAddress == 0 && f.mipAddress == 0)
            return "no texture data (base and mip addresses 0)";

        // Signs and number format (05b §1.2). Components the format lacks repeat the last one, sign included.
        const uint32_t components = TextureComponents(f.format);
        f.signs = (d[0] >> 2) & 0xFF;
        bool anyGamma = false, anySigned = false, anyUnsigned = false, anyBias = false;
        for (uint32_t c = 0; c < 4; c++)
        {
            const uint32_t sign = (f.signs >> (2 * c)) & 3;
            if (c >= components && sign != ((f.signs >> (2 * (components - 1))) & 3))
                return std::format("sign of component {}, which the format lacks, differs from component {}'s", c,
                                   components - 1);
            anyGamma |= sign == 3;
            anySigned |= sign == 1;
            anyUnsigned |= sign == 0;
            anyBias |= sign == 2;
        }
        if (anyGamma)
            return "gamma sign (PWL degamma, 05b §1.3)";
        if (d[3] & 1)
        {
            if (anyBias)
                return "bias sign with integer numbers";
            f.numbers = TextureNumbers::kInteger;
        }
        else if (anySigned && (anyUnsigned || anyBias))
            return "signed and unsigned components in one texture";
        else
            f.numbers = anySigned ? TextureNumbers::kSigned : TextureNumbers::kUnsigned;
        return {};
    }

    TextureLayout ComputeTextureLayout(const TextureFetch& f)
    {
        TextureLayout layout;
        layout.levelCount = f.maxLevel + 1;
        const uint32_t packed = PackedLevel(f);
        const uint32_t lastStored = std::min(f.maxLevel, packed);
        // Offsets of the stored mip levels from the mip address (level 1 at 0; the tail is stored like its level).
        uint32_t mipOffset[kTextureMaxLevels + 1] = {};
        for (uint32_t level = 1; level < lastStored && level < kTextureMaxLevels; level++)
        {
            const Storage s = StorageOf(f, false, level);
            mipOffset[level + 1] = mipOffset[level] + Align(s.rowBytes * s.rows, 4096);
        }
        // Per storage (address and strides: a packed base tail and the mip tail can share the address, 05b §2.5): the
        // block extent of the levels it holds (05b §2.6).
        struct Extent
        {
            Storage storage;
            uint32_t right = 0, bottom = 0;
        };
        std::map<std::tuple<uint32_t, uint32_t, uint32_t>, Extent> extents;
        for (uint32_t level = f.minLevel; level <= f.maxLevel; level++)
        {
            TextureLevelSource& out = layout.levels[level];
            out.width = std::max(f.width >> level, 1u);
            out.height = std::max(f.height >> level, 1u);
            const bool inTail = packed != kNotPacked && level >= packed;
            const uint32_t storageLevel = inTail ? packed : level;
            Storage s;
            if (level == 0)
            {
                s = StorageOf(f, true, 0);
                out.storage = f.baseAddress;
            }
            else
            {
                s = StorageOf(f, false, storageLevel);
                out.storage = f.mipAddress + (storageLevel == 0 ? 0 : mipOffset[storageLevel]);
            }
            out.pitchBlocks = s.pitchBlocks;
            out.rowBytes = s.rowBytes;
            if (inTail)
                PackedOffset(f.width, f.height, level, out.offsetX, out.offsetY);
            out.present = true;
            Extent& e = extents[{ out.storage, s.pitchBlocks, s.rowBytes }];
            e.storage = s;
            e.right = std::max(e.right, out.offsetX + out.width);
            e.bottom = std::max(e.bottom, out.offsetY + out.height);
        }
        const uint32_t bytes = TextureBytesPerTexel(f.format), bytesLog2 = std::countr_zero(bytes);
        for (const auto& [key, e] : extents)
        {
            const uint32_t start = std::get<0>(key);
            uint32_t size;
            if (f.tiled)
            {
                const uint32_t tile = bytesLog2 == 0 ? 0xA00 : bytesLog2 == 1 ? 0xC00 : 0x400u << bytesLog2;
                size = TiledOffset2D((e.right - 1) & ~31u, (e.bottom - 1) & ~31u, e.storage.pitchBlocks / 32, bytesLog2) +
                       tile;
            }
            else
                size = e.storage.rowBytes * (e.bottom - 1) + bytes * e.right;
            layout.extents.emplace_back(start, start + size);
        }
        return layout;
    }

    TextureUntileConstants UntileConstants(const TextureFetch& f, const TextureLevelSource& level)
    {
        TextureUntileConstants c = {};
        c.storage = level.storage;
        c.pitch = f.tiled ? level.pitchBlocks : level.rowBytes;
        c.tiled = f.tiled ? 1 : 0;
        c.endian = f.endian;
        c.offsetX = level.offsetX;
        c.offsetY = level.offsetY;
        c.width = level.width;
        c.height = level.height;
        c.bytesLog2 = std::countr_zero(TextureBytesPerTexel(f.format));
        c.components = TextureComponents(f.format);
        c.numbers = uint32_t(f.numbers);
        c.signs = f.signs;
        return c;
    }

    const char kTextureUntileHlsl[] = R"HLSL(
cbuffer XeUntileConstants : register(b0)
{
    uint4 xe_un_layout;  // storage address, pitch (tiled: blocks, linear: row bytes), tiled, endian
    uint4 xe_un_rect;    // position in the packed tail (blocks), level width, height
    uint4 xe_un_format;  // log2 bytes per texel, components, numbers (0 unsigned, 1 signed, 2 integer), signs
};
ByteAddressBuffer xe_memory : register(t0);
#ifdef XE_UNTILE_FLOAT
RWTexture2D<float4> xe_untile_dest : register(u0);
#else
RWTexture2D<uint4> xe_untile_dest : register(u0);
#endif

// 2D tiled byte offset of block p (05b §2.2).
uint xe_un_tiled(uint2 p, uint pitchMacro, uint bytesLog2)
{
    uint outer = ((p.y >> 5) * pitchMacro + (p.x >> 5)) << 6;
    uint inner = (((p.y >> 1) & 7u) << 3) | (p.x & 7u);
    uint oi = (outer | inner) << bytesLog2;
    uint bank = (p.y >> 4) & 1u;
    uint pipe = ((p.x >> 3) & 3u) ^ (((p.y >> 3) & 1u) << 1);
    return ((p.y & 1u) << 4) | (pipe << 6) | (bank << 11) | (oi & 15u) | (((oi >> 4) & 1u) << 5) |
           (((oi >> 5) & 7u) << 8) | ((oi >> 8) << 12);
}

uint xe_un_swap(uint v, uint endian)
{
    if (endian == 1u || endian == 2u)
        v = ((v & 0x00FF00FFu) << 8) | ((v >> 8) & 0x00FF00FFu);
    if (endian == 2u || endian == 3u)
        v = (v << 16) | (v >> 16);
    return v;
}

[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= xe_un_rect.z || id.y >= xe_un_rect.w)
        return;
    uint2 p = id.xy + xe_un_rect.xy;
    uint bytesLog2 = xe_un_format.x;
    uint offset = xe_un_layout.z != 0u ? xe_un_tiled(p, xe_un_layout.y >> 5, bytesLog2)
                                       : p.y * xe_un_layout.y + (p.x << bytesLog2);
    uint address = xe_un_layout.x + offset;
    // The endian swap works on 32-bit words of memory; the texel is then read little-endian from its word.
    uint word = xe_un_swap(xe_memory.Load(address & ~3u), xe_un_layout.w);
    uint texel = bytesLog2 == 2u ? word : (word >> ((address & 3u) << 3)) & (bytesLog2 == 1u ? 0xFFFFu : 0xFFu);
    uint components = xe_un_format.y;
    uint bits = (components == 2u && bytesLog2 == 2u) ? 16u : 8u;
    uint mask = (1u << bits) - 1u;
    uint4 c;
    c.x = texel & mask;
    c.y = components > 1u ? (texel >> bits) & mask : c.x;
    c.z = components > 2u ? (texel >> (2u * bits)) & mask : c.y;
    c.w = components > 3u ? (texel >> (3u * bits)) & mask : c.z;
#ifdef XE_UNTILE_FLOAT
    // Integers: the value of each component (two's complement when signed) as a float.
    uint4 signs = (xe_un_format.wwww >> uint4(0u, 2u, 4u, 6u)) & 3u;
    int4 extended = int4(c << (32u - bits)) >> int(32u - bits);
    uint4 m = signs == 1u ? 0xFFFFFFFFu : 0u; // a bit-mask select (host compilers rewrite float selects)
    xe_untile_dest[id.xy] = asfloat((asuint(float4(extended)) & m) | (asuint(float4(c)) & ~m));
#else
    xe_untile_dest[id.xy] = c;
#endif
}
)HLSL";
}
