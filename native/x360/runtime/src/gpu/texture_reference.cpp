// CPU reference of texture decode + untile (texture.h ReferenceTexture), written separately from texture.cpp and its
// HLSL for the CB_SHADER_TEST harness: its own fetch-constant reading (05a §2, §3), strides and packed tail
// positions from 05b §2.4/§2.5, the tiled address in the XDK's XGAddress2DTiledOffset formulation (05b §2.2: the
// same function as the untiler's, written the other way), the endian rule of 05b §1.5 and the number formats of §1.2.

#include "gpu/texture.h"

#include <algorithm>
#include <bit>
#include <cstring>

namespace gpu
{
    namespace
    {
        uint32_t CeilLog2(uint32_t v)
        {
            uint32_t r = 0;
            while ((1u << r) < v)
                r++;
            return r;
        }

        uint32_t RoundUp(uint32_t v, uint32_t multiple) { return (v + multiple - 1) / multiple * multiple; }

        // Byte offset of block (x, y) in a tiled surface `pitch` blocks wide (a multiple of 32), in the form of the
        // XDK's XGAddress2DTiledOffset (macro tile, micro tile, then the bank/pipe interleave), kept in bytes.
        uint32_t TiledByteOffset(uint32_t x, uint32_t y, uint32_t pitch, uint32_t bytesLog2)
        {
            const uint32_t macro = ((x >> 5) + (y >> 5) * (pitch >> 5)) << (bytesLog2 + 7);
            const uint32_t micro = ((x & 7) + ((y & 6) << 2)) << bytesLog2;
            const uint32_t offset = macro + ((micro & ~15u) << 1) + (micro & 15) + ((y & 8) << (3 + bytesLog2)) +
                                    ((y & 1) << 4);
            return ((offset & ~511u) << 3) + ((offset & 448) << 2) + (offset & 63) + ((y & 16) << 7) +
                   (((((y & 8) >> 2) + (x >> 3)) & 3) << 6);
        }

        uint32_t Swap(uint32_t v, uint32_t endian)
        {
            uint8_t b[4];
            std::memcpy(b, &v, 4);
            if (endian == 1 || endian == 2) // bytes swapped within each 16-bit half
            {
                std::swap(b[0], b[1]);
                std::swap(b[2], b[3]);
            }
            if (endian == 2 || endian == 3) // halves swapped
            {
                std::swap(b[0], b[2]);
                std::swap(b[1], b[3]);
            }
            std::memcpy(&v, b, 4);
            return v;
        }
    }

    void ReferenceTexture(const uint32_t d[6], const uint8_t* memory, std::vector<std::vector<uint8_t>>& levels,
                          std::vector<std::pair<uint32_t, uint32_t>>* reads)
    {
        const uint32_t format = d[1] & 0x3F;
        const uint32_t bytes = format == 2 ? 1 : format == 10 ? 2 : 4;
        const uint32_t components = format == 2 ? 1 : format == 6 ? 4 : 2;
        const uint32_t componentBits = format == 25 ? 16 : 8;
        const uint32_t bytesLog2 = bytes == 1 ? 0 : bytes == 2 ? 1 : 2;
        const uint32_t width = (d[2] & 0x1FFF) + 1, height = ((d[2] >> 13) & 0x1FFF) + 1;
        const uint32_t basePitch = ((d[0] >> 22) & 0x1FF) * 32;
        const bool tiled = (d[0] >> 31) != 0, packed = ((d[5] >> 11) & 1) != 0, integer = (d[3] & 1) != 0;
        const uint32_t endian = (d[1] >> 6) & 3;
        uint32_t base = d[1] & 0x1FFFF000, mips = d[5] & 0x1FFFF000;

        // Level range (05a §3).
        uint32_t largest = 0;
        while ((2u << largest) <= std::max(width, height))
            largest++;
        uint32_t first = std::min((d[4] >> 2) & 0xF, largest);
        uint32_t last = std::max(std::min((d[4] >> 6) & 0xF, largest), first);
        const uint32_t baseForMips = base;
        if (last > 0 && base == 0)
            first = std::max(first, 1u);
        if (first > 0)
            base = 0;
        if (last == 0)
            mips = 0;
        else if (mips == 0)
            mips = baseForMips; // mips in the base tail (texture.h)

        const uint32_t tail = packed ? (CeilLog2(std::min(width, height)) > 4 ? CeilLog2(std::min(width, height)) - 4 : 0)
                                     : 0xFFFFFFFFu;
        // A storage: the base level, or mip level s (the tail is stored like level `tail`). Pitch in blocks, row bytes,
        // rows (05b §2.4).
        auto pitchOf = [&](bool isBase, uint32_t s) {
            return RoundUp(isBase ? basePitch : std::max((1u << CeilLog2(width)) >> s, 1u), 32);
        };
        auto rowBytesOf = [&](bool isBase, uint32_t s) {
            const uint32_t row = pitchOf(isBase, s) * bytes;
            return !tiled && !isBase ? RoundUp(row, 256) : row;
        };
        auto mipStorageSize = [&](uint32_t s) {
            const uint32_t rows = RoundUp(std::max((1u << CeilLog2(height)) >> s, 1u), 32);
            return RoundUp(rowBytesOf(false, s) * rows, 4096);
        };

        levels.assign(last + 1, {});
        for (uint32_t level = first; level <= last; level++)
        {
            const uint32_t w = std::max(width >> level, 1u), h = std::max(height >> level, 1u);
            const bool inTail = level >= tail;
            const uint32_t s = inTail ? tail : level;
            const bool isBase = level == 0;
            uint32_t storage = isBase ? base : mips;
            if (!isBase)
                for (uint32_t m = 1; m < s; m++)
                    storage += mipStorageSize(m);
            const uint32_t pitch = pitchOf(isBase, s), rowBytes = rowBytesOf(isBase, s);
            // Position in the tail (05b §2.5), in texels = blocks here.
            uint32_t ox = 0, oy = 0;
            if (inTail)
            {
                const uint32_t lw = CeilLog2(width), lh = CeilLog2(height);
                const uint32_t p = level - tail;
                if (p < 3)
                    (lw > lh ? oy : ox) = 16u >> p;
                else
                    (lw > lh ? ox : oy) = (1u << ((lw > lh ? lw : lh) - tail)) >> (p - 2);
            }
            const uint32_t hostBytes = integer ? 16 : bytes;
            std::vector<uint8_t>& out = levels[level];
            out.resize(size_t(w) * h * hostBytes);
            for (uint32_t y = 0; y < h; y++)
                for (uint32_t x = 0; x < w; x++)
                {
                    const uint32_t bx = ox + x, by = oy + y;
                    const uint32_t address = storage + (tiled ? TiledByteOffset(bx, by, pitch, bytesLog2)
                                                              : by * rowBytes + bx * bytes);
                    if (reads)
                        reads->emplace_back(address, address + bytes);
                    uint32_t word;
                    std::memcpy(&word, memory + (address & ~3u), 4);
                    word = Swap(word, endian);
                    uint8_t wordBytes[4];
                    std::memcpy(wordBytes, &word, 4);
                    uint8_t* dst = out.data() + (size_t(y) * w + x) * hostBytes;
                    if (!integer)
                    {
                        std::memcpy(dst, wordBytes + (address & 3), bytes);
                        continue;
                    }
                    uint32_t texel = 0;
                    std::memcpy(&texel, wordBytes + (address & 3), bytes);
                    float values[4];
                    for (uint32_t c = 0; c < 4; c++)
                    {
                        const uint32_t source = std::min(c, components - 1);
                        uint32_t v = (texel >> (source * componentBits)) & ((1u << componentBits) - 1);
                        const bool isSigned = ((d[0] >> (2 + 2 * c)) & 3) == 1;
                        values[c] = isSigned && (v >> (componentBits - 1)) ? float(int32_t(v) - int32_t(1u << componentBits))
                                                                            : float(v);
                    }
                    std::memcpy(dst, values, 16);
                }
        }
    }
}
