// CPU reference of resolves and resolve clears (resolve.h), written from analysis/xenos/07_resolve.md §5-§9
// independently of the HLSL generator, for the GPU test harness (shader_tests.cpp). IEEE single precision, no fused
// multiply-add (the runtime is built with /fp:strict and without FMA).

#include "gpu/output_merger.h"
#include "gpu/resolve.h"

#include "log.h"

#include <bit>
#include <cmath>
#include <cstring>

namespace gpu
{
    namespace
    {
        bool IsNan(float x) { return (std::bit_cast<uint32_t>(x) & 0x7FFFFFFFu) > 0x7F800000u; }
        // The shader library's max/min: a >= b ? a : b / a < b ? a : b, NaN failing the comparison.
        float Max(float a, float b) { return !IsNan(a) && !IsNan(b) && a >= b ? a : b; }
        float Min(float a, float b) { return !IsNan(a) && !IsNan(b) && a < b ? a : b; }
        float Saturate(float x)
        {
            if (IsNan(x) || x <= 0.0f)
                return 0.0f;
            return x >= 1.0f ? 1.0f : x;
        }
        float HalfAwayFromZero(float x) { return (std::bit_cast<uint32_t>(x) >> 31) ? -0.5f : 0.5f; }

        uint32_t Endian(uint32_t v, uint32_t endian)
        {
            if (endian == 1)
                return ((v << 8) & 0xFF00FF00u) | ((v >> 8) & 0x00FF00FFu);
            if (endian == 2)
                return (v >> 24) | ((v >> 8) & 0xFF00u) | ((v << 8) & 0xFF0000u) | (v << 24);
            if (endian == 3)
                return (v << 16) | (v >> 16);
            return v;
        }

        void Decode(uint32_t format, uint32_t v, float c[4])
        {
            if (format == 0)
            {
                const float k = std::bit_cast<float>(0x3B808081u); // 1/255
                for (int i = 0; i < 4; i++)
                    c[i] = float((v >> (8 * i)) & 0xFF) * k;
            }
            else if (format == 4)
            {
                const float k = std::bit_cast<float>(0x3A800100u); // 32/32767
                c[0] = Max(float(int16_t(v & 0xFFFF)) * k, -32.0f);
                c[1] = Max(float(int16_t(v >> 16)) * k, -32.0f);
                c[2] = c[3] = 0.0f;
            }
            else
                Fatal("Resolve reference: EDRAM format {} has no decode", format);
        }

        uint32_t Pack(float v, uint32_t bits, uint32_t number)
        {
            const uint32_t mask = bits == 32 ? 0xFFFFFFFFu : (1u << bits) - 1;
            const float top = float((1u << (bits - 1)) - 1);
            switch (number)
            {
            case 0:
            {
                const float t = Saturate(v) * float(mask);
                const float h = t + 0.5f;
                return uint32_t(std::floor(h));
            }
            case 1:
            {
                const float t = Min(Max(v, -1.0f), 1.0f) * top;
                const float h = t + HalfAwayFromZero(t);
                return uint32_t(int32_t(h)) & mask;
            }
            case 2:
            {
                const float h = Min(Max(v, 0.0f), float(mask)) + 0.5f;
                return uint32_t(h);
            }
            default:
            {
                const float c = Min(Max(v, -float(1u << (bits - 1))), top);
                const float h = c + HalfAwayFromZero(c);
                return uint32_t(int32_t(h)) & mask;
            }
            }
        }

        // R, G, B, A field widths and positions of the 32bpp destination formats (07 §7.3).
        bool Layout(uint32_t format, uint32_t bits[4], uint32_t shift[4])
        {
            auto set = [&](uint32_t b0, uint32_t b1, uint32_t b2, uint32_t b3) {
                bits[0] = b0; bits[1] = b1; bits[2] = b2; bits[3] = b3;
                shift[0] = 0; shift[1] = b0; shift[2] = b0 + b1; shift[3] = b0 + b1 + b2;
                return true;
            };
            switch (format)
            {
            case 6: case 14: case 50: return set(8, 8, 8, 8);
            case 7: case 54: return set(10, 10, 10, 2);
            case 16: case 55: return set(11, 11, 10, 0);
            case 17: case 56: return set(10, 11, 11, 0);
            case 13: case 25: return set(16, 16, 0, 0);
            default: return false;
            }
        }
    }

    void ReferenceResolve(const ResolveKey& key, const ResolveConstants& k, const uint32_t* edram, uint8_t* memory)
    {
        // Samples of the selection, in order (07 §5.3): 0-3 one sample, 4 = 0 and 1, 5 = 2 and 3, 6 = 0 to 3.
        uint32_t list[4] = { key.select, 0, 0, 0 }, count = 1;
        if (key.select == 4) { list[0] = 0; list[1] = 1; count = 2; }
        if (key.select == 5) { list[0] = 2; list[1] = 3; count = 2; }
        if (key.select == 6) { list[0] = 0; list[1] = 1; list[2] = 2; list[3] = 3; count = 4; }
        const float scale = std::ldexp(1.0f, key.expBias) / float(count);

        for (uint32_t j = 0; j < k.height; j++)
            for (uint32_t i = 0; i < k.width; i++)
            {
                const uint32_t x = k.x0 + i, y = k.y0 + j;
                uint32_t d[4];
                for (uint32_t s = 0; s < count; s++)
                {
                    uint32_t qx, qy;
                    EdramSample(key.msaa, x, y, list[s], qx, qy);
                    d[s] = edram[EdramAddress(k.sourceBase, k.pitchTiles, qx, qy, key.depth) / 4];
                }
                uint32_t result;
                if (key.raw)
                {
                    result = d[0];
                    if (key.swap && !key.depth && (key.sourceFormat == 0 || key.sourceFormat == 1))
                        result = (result & 0xFF00FF00u) | ((result & 0xFF) << 16) | ((result >> 16) & 0xFF);
                }
                else
                {
                    float c[4], t[4];
                    Decode(key.sourceFormat, d[0], c);
                    for (uint32_t s = 1; s < count; s++)
                    {
                        Decode(key.sourceFormat, d[s], t);
                        for (int n = 0; n < 4; n++)
                            c[n] = c[n] + t[n];
                    }
                    for (int n = 0; n < 4; n++)
                        c[n] = c[n] * scale;
                    if (key.swap)
                        std::swap(c[0], c[2]);
                    uint32_t bits[4], shift[4];
                    if (key.destFormat == 36)
                        result = std::bit_cast<uint32_t>(c[0]);
                    else if (Layout(key.destFormat, bits, shift))
                    {
                        result = 0;
                        for (int n = 0; n < 4; n++)
                            if (bits[n])
                                result |= Pack(c[n], bits[n], key.destNumber) << shift[n];
                    }
                    else
                        Fatal("Resolve reference: destination format {} has no pack", key.destFormat);
                }
                result = Endian(result, key.endian);
                const uint32_t address = k.destBase + TiledOffset2D(x, y, k.destPitchMacro, 2);
                std::memcpy(memory + address, &result, 4);
            }
    }

    void ReferenceClear(const ClearConstants& k, uint32_t* edram)
    {
        for (uint32_t j = 0; j < k.height; j++)
            for (uint32_t i = 0; i < k.width; i++)
                for (uint32_t s = 0; s < (1u << k.msaa); s++)
                {
                    uint32_t qx, qy;
                    EdramSample(k.msaa, k.x0 + i, k.y0 + j, s, qx, qy);
                    edram[EdramAddress(k.base, k.pitchTiles, qx, qy, k.depth != 0) / 4] = k.value;
                }
    }
}
