#pragma once

// The display controller's view of the front buffer (analysis/xenos/08_present.md §4, §5): what the console scans
// out at a swap. The front buffer is decoded by fetch constant 0, which VdSwap writes just before its swap packet
// (format k_8_8_8_8, endian, 2D tiled or linear, pitch), then every colour channel goes through the display gamma
// LUT the title wrote with DC_LUT_* (the 256-entry table for 8:8:8:8 front buffers: input = the 8-bit channel,
// output = the 10-bit entry). The result is an R10G10B10A2 image, so the 10-bit ramp is not truncated before the
// window scales it.
//
// Not implemented (each stops with specifics): formats other than k_8_8_8_8 (2_10_10_10 front buffers use the PWL
// LUT, never seen in TU46), swizzles other than LE_X8R8G8B8's (Z, Y, X), DC_LUTA_CONTROL other than 0 (256-entry
// table), nonzero DC_LUTA black/white offsets, borders, stacked or non-2D textures.

#include <cstdint>
#include <string>
#include <vector>

namespace gpu
{
    struct DisplayGamma;

    // XAM's system UI overlay for a swap (CB_RECOMP_XAM: the descriptor of VdSetSystemCommandBuffer): a texture the
    // display composites over the front buffer, scaled to it. base 0 = none. Physical base, D3D format, size.
    struct GuestOverlay
    {
        uint32_t base = 0, format = 0, width = 0, height = 0, mode = 0;
        // The part of the texture shown (D3DVIDEO_SCALER_PARAMETERS.ScalerSourceRect of the descriptor's first scaler
        // block, x1 y1 x2 y2); all zero = the whole texture.
        uint32_t source[4] = {};
    };

    // cbuffer of the present shader.
    struct PresentConstants
    {
        uint32_t base;        // physical byte address of the front buffer
        uint32_t pitch;       // row pitch in texels (a multiple of 32)
        uint32_t tiled;       // 1 = 2D tiled (05b §2.2), 0 = linear
        uint32_t endian;      // GPUENDIAN of the fetch constant (0 none, 1 8in16, 2 8in32, 3 16in32)
        uint32_t width, height;
        uint32_t overlay;     // 1: blend the system UI overlay (t1) into the front buffer before the gamma table;
                              // 2: blend XAM's overlay texture from guest memory (ov*), scaled to the front buffer
        uint32_t pad1;
        uint32_t ovBase, ovPitch, ovTiled, ovEndian; // XAM's overlay: k_8_8_8_8, premultiplied (GuestOverlay)
        uint32_t ovWidth, ovHeight, pad2, pad3;     // its texture's size
        uint32_t ovX1, ovY1, ovX2, ovY2;            // the source rectangle scaled to the frame
        uint32_t table[256];  // DisplayGamma::table (blue bits 0-9, green 10-19, red 20-29)
    };
    static_assert(sizeof(PresentConstants) == 80 + 1024);

    // Fills the ov* fields of `constants` for XAM's overlay (overlay = 2); stops on a format not implemented.
    void SetGuestOverlay(PresentConstants& constants, const GuestOverlay& overlay);

    // Decodes fetch constant 0 and DC_LUTA_* for a swap of `frontBuffer` (physical), `width` x `height` (VdSwap's
    // values). Stops with a message for anything not implemented.
    PresentConstants DecodePresent(uint32_t frontBuffer, uint32_t width, uint32_t height, const DisplayGamma& gamma);
    // One line describing fetch constant 0 as presented (for the log).
    std::string DescribePresent(const PresentConstants& constants);
    // Guest bytes [start, end) the front buffer occupies (whole 32x32 macro tiles when tiled).
    void PresentExtent(const PresentConstants& constants, uint32_t& start, uint32_t& end);

    // HLSL of the present pass: [numthreads(16, 8, 1)] over width x height, constants b0, guest memory t0 (raw
    // buffer, byte address = physical), the system UI overlay t1 (Texture2D<uint4> over R8G8B8A8_UINT, premultiplied,
    // width x height), output u0 (RWTexture2D<uint4> over an R10G10B10A2_UINT view). Integer operations only.
    //
    // The overlay is what XAM's HUD draws into the front buffer (xui/system_ui.h): each 8-bit channel becomes
    // overlay + round(channel * (255 - overlay alpha) / 255) before the display gamma table.
    extern const char kPresentHlsl[];

    // CPU reference, written separately from the HLSL: R10G10B10A2 pixels (red bits 0-9, green 10-19, blue 20-29,
    // alpha 3 in 30-31), rows of `width`. `memory` is guest physical memory from byte 0.
    // `overlay`: premultiplied RGBA8 rows of `width` (constants.overlay = 1), else null.
    void ReferencePresent(const PresentConstants& constants, const uint8_t* memory, const uint8_t* overlay,
                          std::vector<uint32_t>& out);
}
