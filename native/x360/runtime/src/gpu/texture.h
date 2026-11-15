#pragma once

// Textures (analysis/xenos/05a, 05b): texture fetch constant decode, the guest memory layout of every level (linear or
// 2D tiled, base level + mips, packed mip tails), the GPU untiler that converts one level from guest memory into the
// host texture, and a CPU reference of all of it for the tests.
//
// Implemented (05b §6.1): 2D textures of k_8, k_8_8, k_8_8_8_8 and k_16_16, the packed 16-bit formats, DXT,
// k_24_8 (the depth, as a float), k_16_FLOAT / k_16_16_FLOAT / k_16_16_16_16_FLOAT (halves to floats) and k_32(_32(_32_32))_FLOAT (the words as they are), linear or 2D tiled, every mip level
// with the per-level stride rules (05b §2.4) and the packed mip tail (05b §2.5, also when the base level is in it),
// all four endian modes (applied to each 32-bit word of memory, 05b §1.5), unsigned / signed / bias signs as
// fractions and every sign as integers. Anything else is reported by DecodeTextureFetch and stops the renderer.
//
// Choices where the hardware is not documented, each in one place:
//  - A fetch constant with mips but MipAddress 0 reads its mips from the base address: the XDK's XGGetMipLevelOffset
//    says so, and D3D creates exactly such headers whenever the base level is packed (a 16x16 texture with 5 levels:
//    one tail at the base, mip address 0). Xenia treats these textures as one level instead. The layout of all such
//    textures matches the XDK's tiling code exactly (the CB_SHADER_TEST XDK cases).
//  - A packed base level (smaller side <= 16) with a separate mip address keeps two tails (Xenia; D3D never creates
//    one, it puts all levels into the base tail).
//  - Components a format lacks repeat its last component (Xenia's host swizzles; unverified).
//  - Fractions: unsigned n / (2^n - 1), signed max(n / (2^(n-1) - 1), -1) (the host UNORM/SNORM conversions),
//    converted before filtering; bias 2x - 1 after filtering (shader side, exact for point sampling).
//  - Integers (NumFormat 1): the raw value (two's complement when signed) as a float, filtered as such.

#include <compare>
#include <cstdint>
#include <string>
#include <vector>

namespace gpu
{
    constexpr uint32_t kTextureMaxLevels = 14; // 8192 -> 1

    // How a texture's data reaches the host (texture.cpp HostRepresentationOf).
    enum class TextureNumbers : uint32_t
    {
        kUnsigned,  // fractions, every component unsigned or bias: host UNORM view
        kSigned,    // fractions, every component signed: host SNORM view
        kInteger,   // integers: float texture holding the integer values
    };

    // A decoded texture fetch constant (05a §2, §3) as far as it shapes the host texture.
    struct TextureFetch
    {
        uint32_t format = 0;          // GPUTEXTUREFORMAT
        uint32_t endian = 0;          // GPUENDIAN
        bool tiled = false;
        bool packedMips = false;
        uint32_t width = 0, height = 0;
        uint32_t pitch = 0;           // base-level row pitch in texels (fetch dword 0 bits 22-30, x 32)
        uint32_t baseAddress = 0;     // physical byte address of level 0 (0 = level 0 absent)
        uint32_t mipAddress = 0;      // physical byte address of level 1's storage (0 = from the base address)
        uint32_t minLevel = 0, maxLevel = 0; // levels that can be sampled (05a §3)
        uint32_t signs = 0;           // 4 x 2 bits, data components X..W (fetch dword 0 bits 2-9)
        uint32_t faces = 1;           // 6 for a cube texture (faces +X, -X, +Y, -Y, +Z, -Z, each level's in turn)
        TextureNumbers numbers = TextureNumbers::kUnsigned;
        bool volume = false;          // a 3D texture (fetch dimension 2): base level only, depth slices below
        uint32_t depth = 1;           // 3D: slices
        bool gamma = false;           // red, green, blue read through sRGB (fetch signs gamma) on an 8-bit RGBA host texture

        auto operator<=>(const TextureFetch&) const = default;
    };

    // Decodes the 6 dwords. Returns an empty string when the renderer implements the texture, else what is missing.
    std::string DecodeTextureFetch(const uint32_t dwords[6], TextureFetch& fetch);
    // Everything about the constant except its addresses, for logs (one line, once per distinct tuple).
    std::string DescribeTextureTuple(const uint32_t dwords[6]);
    const char* TextureFormatName(uint32_t format); // GPUTEXTUREFORMAT without the k_ prefix
    // Bytes per texel (per 4x4 block for DXT) and data components of an implemented format.
    uint32_t TextureBytesPerTexel(uint32_t format);
    uint32_t TextureComponents(uint32_t format);
    // Texels per block side: 4 for the DXT formats, else 1. Strides, packed-tail positions and extents are in blocks.
    uint32_t TextureBlockSize(uint32_t format);
    // How the untile shader reads the format: one component per 8 or 16 bits; a packed 16-bit texel (its components'
    // widths: TexturePackedWidths, a nibble each, X first) expanded to 8 bits each; or a DXT block decoded.
    enum : uint32_t { kUntilePlain = 0, kUntilePacked16 = 1, kUntileDxt1 = 2, kUntileDxt2_3 = 3, kUntileDxt4_5 = 4,
                      kUntileDepth24 = 5, kUntileFloat32 = 6, kUntileDepth24Float = 7, kUntileHalf4 = 8, kUntileDxn = 9, kUntile2_10_10_10 = 10,
                      kUntileHalf2 = 11 };
    uint32_t TextureUntileKind(uint32_t format);
    uint32_t TexturePackedWidths(uint32_t format);

    // Where host level `level` comes from.
    struct TextureLevelSource
    {
        bool present = false;         // the level can be sampled and has data
        uint32_t width = 0, height = 0; // texels (max(size >> level, 1))
        uint32_t storage = 0;         // physical byte address of the storage holding it (a level, or a packed tail)
        uint32_t pitchBlocks = 0;     // tiled: row pitch of that storage in blocks (a multiple of 32)
        uint32_t rowBytes = 0;        // linear: bytes per row of that storage
        uint32_t offsetX = 0, offsetY = 0; // blocks: position inside the packed tail (0 when not packed)
        uint32_t faceStride = 0;      // bytes from one cube face's storage to the next (the storage, 4 KB aligned)
    };

    struct TextureLayout
    {
        uint32_t levelCount = 0;      // host levels 0 .. maxLevel
        TextureLevelSource levels[kTextureMaxLevels];
        // Guest bytes [start, end) the texture may read (05b §2.6), for stream-order invalidation and hashing.
        std::vector<std::pair<uint32_t, uint32_t>> extents;
    };

    TextureLayout ComputeTextureLayout(const TextureFetch& fetch);

    // cbuffer of the untile shader: one dispatch per host level.
    struct TextureUntileConstants
    {
        uint32_t storage, pitch, tiled, endian;   // pitch: tiled = blocks, linear = row bytes
        uint32_t offsetX, offsetY, width, height;
        uint32_t bytesLog2, components, numbers, signs;
        uint32_t kind, packedWidths, zStride, depth; // TextureUntileKind, TexturePackedWidths; 3D: linear bytes between slices, slices
    };
    static_assert(sizeof(TextureUntileConstants) == 64);

    TextureUntileConstants UntileConstants(const TextureFetch& fetch, const TextureLevelSource& level);

    // HLSL of the untile pass: [numthreads(8, 8, 1)] over the level, constants b0, guest memory t0 (raw buffer, byte
    // address = physical), destination u0: RWTexture2D<uint4> over a *_UINT view of the host texture (fractions:
    // the raw components, the SRV converts them) or, with XE_UNTILE_FLOAT defined, RWTexture2D<float4> (integers).
    // Integer operations only, apart from the final int -> float conversion of the integer path.
    extern const char kTextureUntileHlsl[];

    // ---- CPU reference (texture_reference.cpp), written separately from the code above for the tests ------------

    // Host bytes of every level, tightly packed rows, in the host texture's format (UINT components of 1 or 2 bytes
    // for fractions; 4 floats for integers). Levels that are not present are empty. `memory` = physical memory.
    // `reads` (optional) receives every byte range [start, end) read, one per texel.
    // The byte offset of texel (x, y, z) in a tiled volume: `pitchBlocks` texels a row (a multiple of 32), `height` rows a
    // slice, the volume tiled in 32 x 32 x 4 macro tiles. The CPU twin of the untile pass's `xe_un_tiled3d`; the tests check
    // it is a bijection of the padded volume onto the dense range of its bytes, and read volumes laid out by it through
    // the GPU pass.
    uint32_t ReferenceVolumeOffset(uint32_t x, uint32_t y, uint32_t z, uint32_t pitchBlocks, uint32_t height, uint32_t bytesLog2);

    void ReferenceTexture(const uint32_t dwords[6], const uint8_t* memory, std::vector<std::vector<uint8_t>>& levels,
                          std::vector<std::pair<uint32_t, uint32_t>>* reads = nullptr);
}
