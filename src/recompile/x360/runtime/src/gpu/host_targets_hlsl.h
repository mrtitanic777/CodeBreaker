#pragma once

// Shaders of the host render-target path (renderer.cpp "Host render targets"): copies between the EDRAM buffer (the
// output merger's exact bits, output_merger.h) and host render targets. The EDRAM address functions are
// kEdramAddressHlsl's, prepended by the renderer, so both paths place every sample at the same dword.
//
// Host samples: guest 1x is one host sample; guest 2x and 4x targets have 4 host samples (the standard 4x pattern the
// ROV path rasterises with). Guest 2x samples 0/1 are host samples 1/2; guest 4x samples 0..3 are host 0, 3, 2, 1
// (output_merger.cpp kHost2x / kHost4x). Host samples no guest sample uses are filled from the nearest one on load and
// never stored.
//
// Encodings (output_merger.cpp): colour 8_8_8_8 = R | G << 8 | B << 16 | A << 24; depth D24S8 = Z24 << 8 | S.

namespace gpu
{
    constexpr const char* kHostTargetCommonHlsl = R"HLSL(
cbuffer XeHostTarget : register(b0)
{
    uint xe_ht_base;        // EDRAM tile
    uint xe_ht_pitch_tiles; // tiles per row of tiles
    uint xe_ht_msaa;        // 0 1x, 1 2x, 2 4x
    uint xe_ht_bit;         // load of one stencil bit (no SV_StencilRef): the bit; else unused
    uint2 xe_ht_size;       // the target in pixels
    uint2 xe_ht_pad;
};

// The guest sample a host sample shows (load) and the host sample that holds a guest sample (store).
uint xe_ht_guest_sample(uint host)
{
    if (xe_ht_msaa == 0u)
        return 0u;
    if (xe_ht_msaa == 1u)
        return host == 2u || host == 3u ? 1u : 0u;
    return host == 0u ? 0u : (host == 1u ? 3u : (host == 2u ? 2u : 1u));
}
uint xe_ht_host_sample(uint guest)
{
    if (xe_ht_msaa == 0u)
        return 0u;
    if (xe_ht_msaa == 1u)
        return guest == 0u ? 1u : 2u;
    return guest == 0u ? 0u : (guest == 1u ? 3u : (guest == 2u ? 2u : 1u));
}
)HLSL";

    // Load: a full-screen triangle at sample frequency over the target; the EDRAM is the main root signature's u0.
    constexpr const char* kHostTargetLoadVs = R"HLSL(
float4 main(uint id : SV_VertexID) : SV_Position
{
    float2 uv = float2((id << 1) & 2u, id & 2u);
    return float4(uv * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);
}
)HLSL";

    constexpr const char* kHostTargetLoadColorPs = R"HLSL(
RWByteAddressBuffer xe_edram : register(u0);
float4 main(float4 position : SV_Position, uint sample : SV_SampleIndex) : SV_Target
{
    uint2 p = uint2(position.xy);
    uint v = xe_edram.Load(xe_edram_address(xe_ht_base, xe_ht_pitch_tiles,
                                            xe_edram_sample(p, xe_ht_guest_sample(sample), xe_ht_msaa), false));
    return float4(uint4(v, v >> 8, v >> 16, v >> 24) & 0xFFu) * (1.0 / 255.0);
}
)HLSL";

    // Depth and stencil with SV_StencilRef (when the GPU supports it).
    constexpr const char* kHostTargetLoadDepthStencilRefPs = R"HLSL(
RWByteAddressBuffer xe_edram : register(u0);
float main(float4 position : SV_Position, uint sample : SV_SampleIndex, out uint stencil : SV_StencilRef) : SV_Depth
{
    uint2 p = uint2(position.xy);
    uint v = xe_edram.Load(xe_edram_address(xe_ht_base, xe_ht_pitch_tiles,
                                            xe_edram_sample(p, xe_ht_guest_sample(sample), xe_ht_msaa), true));
    stencil = v & 0xFFu;
    return float(v >> 8) / 16777215.0;
}
)HLSL";

    // Without SV_StencilRef: depth (stencil cleared to 0 by the pass's REPLACE with ref 0), then one pass per stencil
    // bit that discards where the bit is clear (stencil REPLACE with ref 0xFF and write mask 1 << xe_ht_bit).
    constexpr const char* kHostTargetLoadDepthPs = R"HLSL(
RWByteAddressBuffer xe_edram : register(u0);
float main(float4 position : SV_Position, uint sample : SV_SampleIndex) : SV_Depth
{
    uint2 p = uint2(position.xy);
    uint v = xe_edram.Load(xe_edram_address(xe_ht_base, xe_ht_pitch_tiles,
                                            xe_edram_sample(p, xe_ht_guest_sample(sample), xe_ht_msaa), true));
    return float(v >> 8) / 16777215.0;
}
)HLSL";
    constexpr const char* kHostTargetLoadStencilBitPs = R"HLSL(
RWByteAddressBuffer xe_edram : register(u0);
void main(float4 position : SV_Position, uint sample : SV_SampleIndex)
{
    uint2 p = uint2(position.xy);
    uint v = xe_edram.Load(xe_edram_address(xe_ht_base, xe_ht_pitch_tiles,
                                            xe_edram_sample(p, xe_ht_guest_sample(sample), xe_ht_msaa), true));
    if (((v >> xe_ht_bit) & 1u) == 0u)
        discard;
}
)HLSL";

    // Store: one thread per pixel writes each guest sample's dword. Root: b0, t0-t1 (the target), u0 (EDRAM).
    constexpr const char* kHostTargetStoreColorCs = R"HLSL(
#if XE_HT_MS
Texture2DMS<uint4> xe_ht_color : register(t0);
#else
Texture2D<uint4> xe_ht_color : register(t0);
#endif
RWByteAddressBuffer xe_edram : register(u0);
[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (any(id.xy >= xe_ht_size))
        return;
    uint samples = xe_ht_msaa == 0u ? 1u : (xe_ht_msaa == 1u ? 2u : 4u);
    for (uint s = 0u; s < samples; s++)
    {
#if XE_HT_MS
        uint4 c = xe_ht_color.Load(id.xy, xe_ht_host_sample(s));
#else
        uint4 c = xe_ht_color.Load(uint3(id.xy, 0u));
#endif
        uint v = c.x | (c.y << 8) | (c.z << 16) | (c.w << 24);
        xe_edram.Store(xe_edram_address(xe_ht_base, xe_ht_pitch_tiles, xe_edram_sample(id.xy, s, xe_ht_msaa), false), v);
    }
}
)HLSL";

    constexpr const char* kHostTargetStoreDepthCs = R"HLSL(
#if XE_HT_MS
Texture2DMS<float> xe_ht_depth : register(t0);
Texture2DMS<uint2> xe_ht_stencil : register(t1);
#else
Texture2D<float> xe_ht_depth : register(t0);
Texture2D<uint2> xe_ht_stencil : register(t1);
#endif
RWByteAddressBuffer xe_edram : register(u0);
[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (any(id.xy >= xe_ht_size))
        return;
    uint samples = xe_ht_msaa == 0u ? 1u : (xe_ht_msaa == 1u ? 2u : 4u);
    for (uint s = 0u; s < samples; s++)
    {
#if XE_HT_MS
        float z = xe_ht_depth.Load(id.xy, xe_ht_host_sample(s));
        uint st = xe_ht_stencil.Load(id.xy, xe_ht_host_sample(s)).y;
#else
        float z = xe_ht_depth.Load(uint3(id.xy, 0u));
        uint st = xe_ht_stencil.Load(uint3(id.xy, 0u)).y;
#endif
        uint z24 = uint(round(saturate(z) * 16777215.0));
        xe_edram.Store(xe_edram_address(xe_ht_base, xe_ht_pitch_tiles, xe_edram_sample(id.xy, s, xe_ht_msaa), true),
                       (z24 << 8) | (st & 0xFFu));
    }
}
)HLSL";
}
