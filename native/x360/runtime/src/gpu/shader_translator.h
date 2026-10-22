#pragma once

// Xenos microcode -> HLSL (SM 5.1) translation (analysis/xenos/03a-03c, 04, 05a). Semantics follow the spec exactly:
// legacy multiply, compare-select min/max, explicit denormal flushing, per-invocation predication; anything the
// translator does not implement stops with a Fatal naming the shader and instruction.
//
// Bindings (one root signature for all stages):
//   b0 system constants (XeSystemConstants below), b1 512 float constants, b2 bool + loop constants,
//   b3 fetch constants (raw register dwords 0x4800-0x48BF),
//   t0 guest physical memory (ByteAddressBuffer, 512 MiB),
//   t<n> in space1 = 1D/2D texture binding n (see TranslatedShader::textures), s<n> = sampler binding n.
//   Pixel shaders for drawing (TranslationOptions::outputMerger) also use b4 (OutputMergerConstants) and u0 (the
//   EDRAM, a rasterizer-ordered byte-address buffer); they have no render-target outputs.

#include "gpu/output_merger.h"
#include "gpu/shader_cache.h"

#include <cstdint>
#include <string>
#include <vector>

namespace gpu
{
    // Denormal float bits read as zero with the sign kept (03c §1), as hlsl_library.cpp xe_flush. Translated shaders
    // expect the float constants (b1) uploaded through this: they read constants without a flush.
    inline void FlushDenormals(uint32_t* words, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            if ((words[i] & 0x7F800000u) == 0)
                words[i] &= 0x80000000u;
    }

    // The same while copying: `out` may be write-combined upload memory, which must not be read back (slow).
    inline void CopyFlushingDenormals(uint32_t* out, const uint32_t* in, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            const uint32_t w = in[i];
            out[i] = (w & 0x7F800000u) == 0 ? w & 0x80000000u : w;
        }
    }

    extern const char* const kXenosHlslLibrary;

    // Layout of cbuffer XeSystemConstants (b0) in hlsl_library.cpp.
    struct XeSystemConstants
    {
        uint32_t flags;               // XE_SYS_VTX_* (PA_CL_VTE_CNTL formats)
        uint32_t vertexIndexOffset;   // VGT_INDX_OFFSET
        uint32_t minVertexIndex;      // VGT_MIN_VTX_INDX
        uint32_t maxVertexIndex;      // VGT_MAX_VTX_INDX
        uint32_t indexAddress;        // physical byte address of the index buffer
        uint32_t indexFormat;         // 0 auto (the host vertex id is the index), 1 16-bit, 2 32-bit
        uint32_t indexEndian;         // VGT_DMA_SIZE.swap_mode
        uint32_t pad0;
        uint32_t vsConstantBase, vsConstantCount, psConstantBase, psConstantCount;
        float ndcScale[4];
        float ndcOffset[4];
        float clipPlanes[6][4];
        uint32_t clipPlaneMask;
        uint32_t interpolatorCount;
        uint32_t paramGenFlags;       // bit 0: polygon (faceness valid), bit 1: point, bit 2: line
        uint32_t zero;                // always 0: initial register values the compiler cannot constant-fold
        // Point lists: xy = sprite diameter in guest pixels (PA_SU_POINT_SIZE), zw = host NDC radius per pixel of
        // diameter (the point-list geometry shader).
        float pointSize[4];
    };
    static_assert(sizeof(XeSystemConstants) == 13 * 16);

    struct TranslationOptions
    {
        uint32_t registerCount = 64;      // SQ_PROGRAM_CNTL VsMaxReg/PsMaxReg + 1
        // Pixel shaders.
        bool paramGen = false;            // SQ_PROGRAM_CNTL.ParamGen
        uint32_t paramGenRegister = 0;    // SQ_CONTEXT_MISC.ParamGenPos
        uint32_t flatInterpolators = 0;   // SQ_INTERPOLATOR_CNTL.ParamShade
        uint32_t centroidInterpolators = 0;
        // Point-list draws: the input has the geometry shader's sprite coordinates (TEXCOORD16) right after the
        // position, and PsParamGen's ZW are those coordinates.
        bool pointSprite = false;
        // Test entry point (compute shader over a buffer of register states) instead of a VS/PS.
        bool computeHarness = false;
        // Pixel shaders: write through the EDRAM output merger for this key (else plain SV_Target outputs, used only
        // to check that a shader compiles).
        bool useOutputMerger = false;
        OutputMergerKey outputMerger;
        // Plain-output pixel shaders drawn to host render targets (renderer.cpp "Host render targets"): RB_COLORCONTROL
        // bits 0-3 (AlphaFunc | AlphaTestEnable << 3), tested on colour 0's alpha before the outputs (discard).
        uint32_t alphaTest = 0;

        auto operator<=>(const TranslationOptions&) const = default;
    };

    struct TextureBinding
    {
        uint32_t fetchConstant;           // tf#
        uint32_t dimension;               // 0 1D, 1 2D, 3 cube
    };

    struct SamplerBinding
    {
        uint32_t fetchConstant;
        // Instruction overrides (3 = use the fetch constant; aniso 7 = use the fetch constant).
        uint32_t magFilter, minFilter, mipFilter, anisoFilter;
        bool operator==(const SamplerBinding&) const = default;
    };

    struct TranslatedShader
    {
        std::string hlsl;
        std::vector<TextureBinding> textures;
        std::vector<SamplerBinding> samplers;
    };

    // Compute-harness I/O layout: per test case, r0-r7 (8 x float4), then (ps, p0, a0, killed), then the first color
    // (PS) / interpolator (VS) export. Inputs are read from the same layout.
    constexpr uint32_t kHarnessRegisters = 8;
    constexpr uint32_t kHarnessCaseBytes = (kHarnessRegisters + 2) * 16;

    TranslatedShader TranslateShader(const Shader& shader, const TranslationOptions& options);

    // D3DCompile with IEEE strictness; returns false and the compiler output on failure.
    bool CompileHlsl(const std::string& source, const char* target, const char* name, std::vector<uint8_t>& bytecode,
                     std::string& errors);
}
