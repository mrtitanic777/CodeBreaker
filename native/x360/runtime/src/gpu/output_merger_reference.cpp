// CPU reference of the EDRAM output merger (output_merger.h), written from analysis/xenos/06a and 01b independently
// of the HLSL generator, for the GPU test harness (shader_tests.cpp). Float operations mirror the documented choices
// one by one (IEEE single precision, no fused multiply-add: the runtime is built with /fp:strict and without FMA).

#include "gpu/output_merger.h"

#include "log.h"

#include <bit>
#include <cmath>

namespace gpu
{
    namespace
    {
        bool IsNan(float x) { return (std::bit_cast<uint32_t>(x) & 0x7FFFFFFFu) > 0x7F800000u; }

        // The shader library's compare-select semantics (03c §7.3): max = a >= b ? a : b, min = a < b ? a : b, with
        // +0 and -0 equal and NaN operands failing the comparison.
        bool Ge(float a, float b)
        {
            if (IsNan(a) || IsNan(b))
                return false;
            return a >= b; // IEEE: -0 >= +0
        }
        bool Lt(float a, float b)
        {
            if (IsNan(a) || IsNan(b))
                return false;
            return a < b;
        }
        float Max(float a, float b) { return Ge(a, b) ? a : b; }
        float Min(float a, float b) { return Lt(a, b) ? a : b; }

        float Saturate(float x)
        {
            if (IsNan(x) || x <= 0.0f)
                return 0.0f;
            return x >= 1.0f ? 1.0f : x;
        }

        void Range(uint32_t format, float c[4])
        {
            for (int i = 0; i < 4; i++)
            {
                if (format == 0)
                    c[i] = Saturate(c[i]);
                else
                    c[i] = Min(Max(IsNan(c[i]) ? 0.0f : c[i], -32.0f), 32.0f);
            }
        }

        void Decode(uint32_t format, uint32_t v, float c[4])
        {
            if (format == 0)
            {
                const float k = std::bit_cast<float>(0x3B808081u); // 1/255
                for (int i = 0; i < 4; i++)
                    c[i] = float((v >> (8 * i)) & 0xFF) * k;
                return;
            }
            const float k = std::bit_cast<float>(0x3A800100u); // 32/32767
            c[0] = float(int16_t(v & 0xFFFF)) * k;
            c[1] = float(int16_t(v >> 16)) * k;
            c[2] = c[3] = 0.0f;
        }

        uint32_t Encode(uint32_t format, const float c[4])
        {
            if (format == 0)
            {
                uint32_t packed = 0;
                for (int i = 0; i < 4; i++)
                {
                    float t = c[i] * 255.0f;
                    t = t + 0.5f;
                    packed |= uint32_t(std::floor(t)) << (8 * i);
                }
                return packed;
            }
            uint32_t packed = 0;
            for (int i = 0; i < 2; i++)
            {
                const float t = c[i] * std::bit_cast<float>(0x447FFE00u); // 32767/32
                const float h = t + std::copysign(0.5f, t);
                packed |= (uint32_t(int32_t(h)) & 0xFFFF) << (16 * i);
            }
            return packed;
        }

        uint32_t ChannelBits(uint32_t format, uint32_t channel)
        {
            if (format == 4)
                return channel == 0 ? 0x0000FFFFu : channel == 1 ? 0xFFFF0000u : 0u;
            return 0xFFu << (8 * channel);
        }

        // Blend factor (01b §6.4) for channel `i` (0-2 colour, 3 alpha).
        float Factor(uint32_t factor, uint32_t i, const float src[4], const float dst[4], const float k[4])
        {
            const bool alpha = i == 3;
            switch (factor)
            {
            case 1: return 1.0f;
            case 4: return src[i];
            case 5: return 1.0f - src[i];
            case 6: return src[3];
            case 7: return 1.0f - src[3];
            case 8: return dst[i];
            case 9: return 1.0f - dst[i];
            case 10: return dst[3];
            case 11: return 1.0f - dst[3];
            case 12: return k[i];
            case 13: return 1.0f - k[i];
            case 14: return k[3];
            case 15: return 1.0f - k[3];
            case 16: return alpha ? 1.0f : Min(src[3], 1.0f - dst[3]);
            default: Fatal("reference: blend factor {}", factor);
            }
        }
    }

    void EdramSample(uint32_t msaa, uint32_t x, uint32_t y, uint32_t s, uint32_t& sx, uint32_t& sy)
    {
        switch (msaa)
        {
        case 0: sx = x; sy = y; break;
        case 1: sx = x; sy = 2 * y + s; break;                       // 1x2 samples, sample 0 on top
        default: sx = 2 * x + (s & 1); sy = 2 * y + (s >> 1); break; // 2x2 samples
        }
    }

    uint32_t EdramAddress(uint32_t base, uint32_t pitchTiles, uint32_t sx, uint32_t sy, bool depth)
    {
        uint32_t column = sx % 80;
        if (depth)
            column = (column + 40) % 80;
        const uint32_t tile = (base + (sy / 16) * pitchTiles + sx / 80) % 2048;
        return (tile * kEdramTileDwords + (sy % 16) * 80 + column) * 4;
    }

    uint32_t ReferenceColor(const OutputMergerKey& key, uint32_t rt, const float color[4], uint32_t old,
                            const OutputMergerConstants& constants)
    {
        const uint32_t format = key.colorFormat[rt], blend = key.blendControl[rt];
        float src[4];
        const float bias = std::ldexp(1.0f, key.expBias[rt]);
        for (int i = 0; i < 4; i++)
            src[i] = key.expBias[rt] ? color[i] * bias : color[i];
        Range(format, src);
        float r[4] = { src[0], src[1], src[2], src[3] };
        const uint32_t sc = blend & 0x1F, oc = (blend >> 5) & 7, dc = (blend >> 8) & 0x1F;
        const uint32_t sa = (blend >> 16) & 0x1F, oa = (blend >> 21) & 7, da = (blend >> 24) & 0x1F;
        const bool opaque = sc == 1 && oc == 0 && dc == 0 && sa == 1 && oa == 0 && da == 0;
        if (!opaque)
        {
            float dst[4], k[4] = { constants.blendConstant[0], constants.blendConstant[1], constants.blendConstant[2],
                                   constants.blendConstant[3] };
            Decode(format, old, dst);
            Range(format, k);
            const uint32_t channels = format == 4 ? 3 : 4; // 16_16: R, G (and an unused third lane), no alpha
            for (uint32_t i = 0; i < channels; i++)
            {
                const bool alpha = i == 3;
                const uint32_t fs = alpha ? sa : sc, op = alpha ? oa : oc, fd = alpha ? da : dc;
                if (op == 2)
                {
                    r[i] = Min(src[i], dst[i]);
                    continue;
                }
                if (op == 3)
                {
                    r[i] = Max(src[i], dst[i]);
                    continue;
                }
                auto term = [&](const float* operand, uint32_t factor) {
                    if (factor == 0)
                        return 0.0f;
                    float p = operand[i] * Factor(factor, i, src, dst, k);
                    if (format == 4)
                        p = Min(Max(IsNan(p) ? 0.0f : p, -32.0f), 32.0f);
                    return p;
                };
                const float bs = term(src, fs), bd = term(dst, fd);
                r[i] = op == 0 ? bs + bd : op == 1 ? bs - bd : bd - bs;
            }
            Range(format, r);
        }
        const uint32_t packed = Encode(format, r);
        uint32_t keep = 0xFFFFFFFFu;
        for (uint32_t c = 0; c < 4; c++)
            if ((key.colorMask[rt] >> c) & 1)
                keep &= ~ChannelBits(format, c);
        return (old & keep) | (packed & ~keep);
    }

    uint32_t ReferenceDepthStencil(const OutputMergerKey& key, uint32_t old, uint32_t z24, bool frontFace,
                                   const OutputMergerConstants& constants, bool& pass)
    {
        const uint32_t dc = key.depthControl;
        auto compare = [](uint32_t func, uint32_t a, uint32_t b) {
            switch (func & 7)
            {
            case 0: return false;
            case 1: return a < b;
            case 2: return a == b;
            case 3: return a <= b;
            case 4: return a > b;
            case 5: return a != b;
            case 6: return a >= b;
            default: return true;
            }
        };
        uint32_t z = old >> 8, s = old & 0xFF;
        const bool depthPass = ((dc >> 1) & 1) ? compare((dc >> 4) & 7, z24, z) : true;
        bool stencilPass = true;
        if (dc & 1)
        {
            const bool back = key.polygonal && ((dc >> 7) & 1) && !frontFace;
            const uint32_t shift = back ? 20 : 8;
            const uint32_t rmw = back ? constants.stencilBack : constants.stencilFront;
            const uint32_t ref = rmw & 0xFF, mask = (rmw >> 8) & 0xFF, writeMask = (rmw >> 16) & 0xFF;
            stencilPass = compare((dc >> shift) & 7, ref & mask, s & mask);
            const uint32_t op = !stencilPass ? (dc >> (shift + 3)) & 7
                              : !depthPass ? (dc >> (shift + 9)) & 7 : (dc >> (shift + 6)) & 7;
            uint32_t ns = s;
            switch (op)
            {
            case 0: ns = s; break;
            case 1: ns = 0; break;
            case 2: ns = ref; break;
            case 3: ns = s < 255 ? s + 1 : 255; break;
            case 4: ns = s ? s - 1 : 0; break;
            case 5: ns = ~s & 0xFF; break;
            case 6: ns = (s + 1) & 0xFF; break;
            case 7: ns = (s - 1) & 0xFF; break;
            }
            s = (s & ~writeMask) | (ns & writeMask);
        }
        pass = stencilPass && depthPass;
        if (pass && ((dc >> 1) & 1) && ((dc >> 2) & 1))
            z = z24;
        return (z << 8) | s;
    }

    uint32_t ReferenceDepthUnorm24(float z)
    {
        const float t = Saturate(z) * 16777215.0f;
        return uint32_t(std::nearbyint(t)); // default rounding mode: nearest, ties to even
    }

    uint32_t ReferenceDepthFloat24(float z)
    {
        uint32_t u = std::bit_cast<uint32_t>(z);
        if (!(z > 0.0f))
            return 0; // negative, zero, NaN
        if (u >= 0x3FFFFFF8u)
            return 0xFFFFFF;
        if (u < 0x38800000u)
        {
            const uint32_t shift = std::min<uint32_t>(113 - (u >> 23), 24);
            u = (0x800000u | (u & 0x7FFFFFu)) >> shift;
        }
        else
        {
            u -= 112u << 23;
        }
        u += 3 + ((u >> 3) & 1);
        return (u >> 3) & 0xFFFFFF;
    }
}
