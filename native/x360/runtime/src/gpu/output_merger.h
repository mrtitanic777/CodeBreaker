#pragma once

// EDRAM emulation: the render backend (depth/stencil, alpha test, blending, format packing) done by the translated
// pixel shader on a rasterizer-ordered view of a 10 MiB buffer (analysis/xenos/06a, 06b §4). The buffer holds the
// EDRAM's bits exactly; every render target / depth buffer is an interpretation of it at some base, pitch, format and
// MSAA level, so aliasing between them needs no tracking.
//
// Storage model (06a): 2048 tiles of 80x16 32-bit samples (tile t, row r, column c at dword t*1280 + r*80 + c).
// A pixel's samples sit at 1x-equivalent sample coordinates by the XDK permutation (06a §4.2); depth/stencil swaps the
// two 40-column halves of each tile row (06a §5.1); tile indices wrap at 2048 (06a §3.3).
//
// Choices where the hardware is not documented (one place each, so they can be switched after a hardware test):
//  - colour encodes round half up (8_8_8_8: floor(c*255+0.5); 16_16: Xenia's n = trunc(c*32767/32 +- 0.5));
//  - decodes multiply by the reciprocal (n * (1/255)): deterministic on CPU and GPU;
//  - blending is IEEE fp32 on the decoded values with the source, constant, factors and products clamped to the
//    format's range (Xenia's ROV model), not the render backend's fixed-point datapath;
//  - unorm24 depth rounds to nearest even (Xenia; truncation gives wrong clears), 20e4 rounds to nearest even;
//  - MSAA rasterises with the host's standard 4x pattern (guest samples mapped to the nearest host sample), so partial
//    coverage at edges differs slightly; full-coverage draws (clears) are exact.

#include <compare>
#include <cstdint>
#include <string>

namespace gpu
{
    constexpr uint32_t kEdramBytes = 10 * 1024 * 1024;
    constexpr uint32_t kEdramTileDwords = 80 * 16;
    constexpr uint32_t kRtDisabled = 0xFF;

    // Everything that changes the generated output-merger code.
    struct OutputMergerKey
    {
        uint32_t msaa = 0;                 // RB_SURFACE_INFO.MsaaSamples: 0 1x, 1 2x, 2 4x
        uint32_t colorFormat[4] = { kRtDisabled, kRtDisabled, kRtDisabled, kRtDisabled }; // RB_COLOR_INFO.ColorFormat
        uint32_t colorMask[4] = {};        // RB_COLOR_MASK nibble (bit 0 R .. bit 3 A), only for enabled RTs
        uint32_t blendControl[4] = {};     // RB_BLENDCONTROL
        int32_t expBias[4] = {};           // RB_COLOR_INFO.ColorExpBias
        uint32_t depthControl = 0;         // RB_DEPTHCONTROL; 0 when neither depth nor stencil is enabled
        uint32_t depthFormat = 0;          // RB_DEPTH_INFO.DepthFormat: 0 D24S8, 1 D24FS8 (20e4)
        uint32_t alphaTest = 0;            // RB_COLORCONTROL bits 0-3 (AlphaFunc | AlphaTestEnable << 3); 0 = off
        bool runsShader = true;            // false: depth-only mode or no pixel shader (no colour, kill, alpha test)
        bool polygonal = true;             // front/back faces exist (else everything is front-facing)
        bool polyOffsetFront = false;      // PA_SU_SC_MODE_CNTL poly_offset_front_enable (para for non-polygons)
        bool polyOffsetBack = false;
        bool shaderDepth = false;          // the pixel shader exports oDepth

        auto operator<=>(const OutputMergerKey&) const = default;
    };

    // cbuffer XeOutputConstants (b4), per draw.
    struct OutputMergerConstants
    {
        uint32_t colorBase[4];             // EDRAM tile of each RT (0-2047)
        uint32_t pitchTiles;               // tiles per row of tiles at 32bpp
        uint32_t depthBase;
        uint32_t stencilFront;             // RB_STENCILREFMASK: ref | mask << 8 | writemask << 16
        uint32_t stencilBack;              // RB_STENCILREFMASK_BF
        float blendConstant[4];            // RB_BLEND_RED..ALPHA
        float alphaRef;                    // RB_ALPHA_REF
        float polyOffsetFrontScale, polyOffsetFrontOffset;
        float polyOffsetBackScale;
        float polyOffsetBackOffset;
        float pad[3];
    };
    static_assert(sizeof(OutputMergerConstants) == 5 * 16);

    // HLSL of the output merger for `key`: helper functions plus, for a pixel shader, the entry point. The translated
    // shader provides XePixelInput, XeState, xe_initial_state and xe_run (runsShader) and the library.
    //  - kPixelShader: `void main(XePixelInput input)` with the ROV (u0) and the constants (b4).
    //  - kHarness: the per-key functions only (xe_om_color_<i>, xe_om_depth_stencil, ...) and the constants, for the
    //    compute test harness (shader_tests.cpp), without u0.
    enum class OutputMergerTarget { kPixelShader, kHarness };
    // For plain-output pixel shaders (host render targets): the output constants (b4) and
    // `bool xe_alpha_pass(float alpha)` for RB_COLORCONTROL's alpha test bits, compared as the output merger does.
    std::string AlphaTestHlsl(uint32_t alphaTest);
    std::string OutputMergerHlsl(const OutputMergerKey& key, OutputMergerTarget target);

    // Shared address arithmetic (also for resolves): byte offset in the EDRAM buffer of a pixel's sample.
    extern const char* const kEdramAddressHlsl;

    // Whether the output merger implements a colour format / depth format (others are fatal at draw time).
    bool OutputMergerSupportsColorFormat(uint32_t format);
    const char* EdramColorFormatName(uint32_t format);

    // ---- CPU reference (output_merger_reference.cpp), written separately from the HLSL for the test harness ----

    // 1x-equivalent 32bpp sample coordinates of pixel (x, y) sample s (06a §4.2).
    void EdramSample(uint32_t msaa, uint32_t x, uint32_t y, uint32_t s, uint32_t& sx, uint32_t& sy);
    // Byte offset of a sample of a surface at `base` (tiles) with `pitchTiles`.
    uint32_t EdramAddress(uint32_t base, uint32_t pitchTiles, uint32_t sx, uint32_t sy, bool depth);
    // New dword of RT `rt` for shader colour `color` over `old`.
    uint32_t ReferenceColor(const OutputMergerKey& key, uint32_t rt, const float color[4], uint32_t old,
                            const OutputMergerConstants& constants);
    // Depth/stencil update of one sample: returns the new dword and whether the sample passed.
    uint32_t ReferenceDepthStencil(const OutputMergerKey& key, uint32_t old, uint32_t z24, bool frontFace,
                                   const OutputMergerConstants& constants, bool& pass);
    uint32_t ReferenceDepthUnorm24(float z);
    uint32_t ReferenceDepthFloat24(float z);
}
