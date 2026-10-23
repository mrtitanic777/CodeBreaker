#pragma once

// Resolves (EDRAM -> guest memory copies) and the clears done with them (analysis/xenos/07_resolve.md).
//
// A resolve is a draw with RB_MODECONTROL.edram_mode = 6 (COPY). Its rectangle comes from vertex fetch constant 0
// (07 §4), the source surface from RB_COPY_CONTROL / RB_COLOR_INFO / RB_DEPTH_INFO / RB_SURFACE_INFO (07 §5), the
// destination from RB_COPY_DEST_* (07 §6). The copy is a compute dispatch that reads the EDRAM buffer with the
// output merger's address function (kEdramAddressHlsl) and writes the tiled destination into guest memory; the
// clears that follow it are a second compute dispatch writing the EDRAM buffer (07 §9).
//
// Choices where the hardware is not documented (07 §13), each in one place:
//  - the raw (bit copy) vs converting path follows Xenia's rule (07 §7.1);
//  - converting: decode as the output merger does (8_8_8_8 n * (1/255), 16_16 n * (32/32767) clamped to -32),
//    average in fp32 in sample order, scale by 2^exp_bias / samples (one exact multiply), pack fixed fields with
//    round half up (unorm) / half away from zero (snorm, sint) as Xenia;
//  - clamps use the shader library's compare semantics (max = a >= b ? a : b; NaN fails the comparison).
// Unimplemented cases (3D destinations, formats below 32 bpp or above, float packs, endian 8in64/8in128, copy commands
// 1_1_1_1 and NULL, 64bpp sources) stop with a message naming them.

#include <compare>
#include <cstdint>
#include <string>

namespace gpu
{
    // Everything that changes the generated copy shader.
    struct ResolveKey
    {
        uint32_t msaa = 0;          // RB_SURFACE_INFO.MsaaSamples: 0 1x, 1 2x, 2 4x
        uint32_t select = 0;        // sanitised copy_sample_select: 0-3 one sample, 4 = 0+1, 5 = 2+3, 6 = all four
        bool depth = false;         // source is the depth/stencil surface
        uint32_t sourceFormat = 0;  // EDRAM colour format (RB_COLOR_INFO.ColorFormat); unused for depth
        bool raw = false;           // bit copy (07 §7.2) instead of decode/convert/pack (07 §7.3)
        uint32_t destFormat = 0;    // texture format (RB_COPY_DEST_INFO.copy_dest_format; k_24_8(_FLOAT) for depth)
        uint32_t destNumber = 0;    // 0 unsigned fraction, 1 signed fraction, 2 uint, 3 sint, 7 float
        bool swap = false;          // copy_dest_swap: red/blue exchange
        uint32_t endian = 0;        // copy_dest_endian (0 none, 1 8in16, 2 8in32, 3 16in32)
        int32_t expBias = 0;        // copy_dest_exp_bias (converting path only)
        // Where the source samples are read (kRenderer only): 0 the EDRAM buffer; a host render target holding newer
        // data than the EDRAM (renderer.cpp "Host render targets"), at t0 (and its stencil at t1): 1 colour, 2 colour
        // multisampled, 3 depth/stencil, 4 depth/stencil multisampled. The dwords are the ones the target's store
        // pass would write (host_targets_hlsl.h).
        uint32_t hostSource = 0;

        auto operator<=>(const ResolveKey&) const = default;
    };

    // cbuffer of the copy shader.
    struct ResolveConstants
    {
        uint32_t x0, y0, width, height;      // rectangle in pixels (multiples of 8)
        uint32_t sourceBase, pitchTiles;     // EDRAM tile of the source surface, tiles per row at 32bpp
        uint32_t pad0, pad1;
        uint32_t destBase;                   // RB_COPY_DEST_BASE (physical byte address)
        uint32_t destPitchMacro;             // destination pitch aligned to 32, divided by 32
        uint32_t pad2, pad3;
    };
    static_assert(sizeof(ResolveConstants) == 48);

    // cbuffer of the clear shader: every sample of every pixel in the rectangle gets `value`.
    struct ClearConstants
    {
        uint32_t x0, y0, width, height;
        uint32_t base, pitchTiles, msaa, depth;
        uint32_t value, pad0, pad1, pad2;
    };
    static_assert(sizeof(ClearConstants) == 48);

    // A decoded resolve (DecodeResolve).
    struct ResolveInfo
    {
        uint32_t copyControl = 0, destBaseRegister = 0, destPitchRegister = 0, destInfo = 0; // raw registers, for logs
        bool empty = true;                   // nothing to copy or clear (07 §4)
        uint32_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
        uint32_t sourceSelect = 0;           // copy_src_select (0-3 RT, 4 depth)

        bool copy = false;                   // the copy runs (else only the clears)
        ResolveKey key;
        ResolveConstants constants = {};
        uint32_t extentStart = 0, extentEnd = 0; // destination bytes that may be written (physical, 07 §6.3)

        bool clearDepth = false, clearColor = false;
        ClearConstants depthClear = {}, colorClear = {};
    };

    // Decodes the current registers (and vertex fetch constant 0's vertices in guest memory) at a COPY draw.
    // Stops with a message for anything not implemented.
    ResolveInfo DecodeResolve();
    // One line describing the configuration (for the log, once per distinct configuration).
    std::string DescribeResolve(const ResolveInfo& info);

    // HLSL of the copy shader for `key` and of the clear shader. kRenderer: [numthreads(8, 8, 1)] over the rectangle,
    // constants at b5, EDRAM u0, guest memory u1 (byte addresses = physical). kHarness: same code, constants at b4,
    // one buffer u0 holding the EDRAM (bytes 0..kEdramBytes-1) followed by guest memory (shader_tests.cpp).
    enum class ResolveTarget { kRenderer, kHarness };
    std::string ResolveHlsl(const ResolveKey& key, ResolveTarget target);
    std::string ClearHlsl(ResolveTarget target);

    // Byte offset of block (x, y) in a 2D tiled surface (05b §2.2, 07 §6.1); `pitchMacro` = aligned pitch / 32,
    // `bytesLog2` = log2 of the bytes per block.
    uint32_t TiledOffset2D(uint32_t x, uint32_t y, uint32_t pitchMacro, uint32_t bytesLog2);
    // Destination bytes [start, end) a copy may write (07 §6.3): whole 32x32 macro tiles of the rectangle.
    void ResolveExtent(const ResolveConstants& constants, uint32_t& start, uint32_t& end);

    // ---- CPU reference (resolve_reference.cpp), written separately from the HLSL for the test harness ----------

    // Copies the rectangle from `edram` (kEdramBytes) into `memory` (guest physical memory from byte 0).
    void ReferenceResolve(const ResolveKey& key, const ResolveConstants& constants, const uint32_t* edram,
                          uint8_t* memory);
    void ReferenceClear(const ClearConstants& constants, uint32_t* edram);
}
