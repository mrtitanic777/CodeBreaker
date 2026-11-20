#pragma once

#include <cstdint>
#include <string>

// Direct3D 12 renderer for the command processor's draws (analysis/xenos/00_plan.md step 4).
//
// Guest physical memory is the GPU's memory: the 512 MiB section is opened as a D3D12 heap and bound as one raw
// buffer, from which vertex shaders pull vertices and indices. The EDRAM is a 10 MiB buffer written only through the
// output merger in the pixel shaders (output_merger.h) and read/cleared by resolves (resolve.h: compute dispatches
// that write the tiled destination into guest memory through a UAV). Every draw snapshots its constants; textures
// (texture.h) are untiled from the guest-memory buffer by compute dispatches into host textures, again when their
// guest bytes changed (hashed once per frame) or a resolve wrote over them (recorded after that resolve, so nothing
// waits for the GPU); an unimplemented texture stops the run (CB_TEXTURE_MAGENTA=1: magenta instead). Recorded work
// is submitted in batches as it is recorded, so the GPU runs alongside the command processor, and the command
// processor waits for all of it at every point where the CPU could observe GPU progress (renderer.cpp). CB_TEXTURE_DUMP=1 writes every untiled texture level as tex_*.bmp.
//
// CB_NO_RENDER=1 disables the renderer (shaders are still translated and compiled per draw).
// CB_EDRAM_DUMP=<frame> writes the colour surface last drawn with RT0 (and its depth) as edram_frame<N>*.bmp plus
// the raw 10 MiB EDRAM (edram_frame<N>.bin) at that frame's swap.
// CB_FRONT_DUMP=<frame> detiles the front buffer handed to VdSwap into front_frame<N>.bmp at that frame's swap.
// At every swap (with a window) the front buffer goes through the present pass (present.h: detile + display gamma, on
// the GPU) into a guest-output texture for the window (presenter.h). CB_PRESENT_DUMP=<frame> writes that frame's
// output as present_frame<N>.bmp and checks it against the CPU reference (also headless).
// CB_D3D12_DEBUG=1 enables the D3D12 debug layer and copies its messages into the log.
namespace gpu
{
    struct DrawPacket;
    struct Shader;

    bool RendererEnabled();
    // Creates the renderer now (device objects, pipeline library, compile workers) instead of at the first draw:
    // before the boot animation, whose sound would otherwise start while the renderer is still being made.
    void PrepareRenderer();

    // Command-processor thread only.
    void RenderDraw(const DrawPacket& draw, const Shader* vertexShader, const Shader* pixelShader);
    // Why the command processor waits for the GPU (CB_GPU_STATS counts them).
    enum class SyncReason
    {
        kOther, kScratch, kRegToMem, kMemWrite, kCondWrite, kFence, kExtents, kOcclusion, kInterrupt, kSwap,
        kRingChunk, kDescriptors, kPresent, kCount
    };
    // Submits everything recorded and waits until the GPU has finished it.
    void RenderSync(SyncReason reason);
    // Submits everything recorded without waiting; returns the fence value the GPU reaches once it is done (0 with
    // the renderer off). Transient allocations stay in use until the next RenderSync.
    uint64_t RenderSubmit();
    // Whether the GPU has reached `value` (RenderSubmit), waiting up to `ms` milliseconds for it.
    bool RenderReached(uint64_t value, uint32_t ms);
    // CB_GPU_STATS: GPU waits since the previous call, by reason (one line).
    std::string TakeSyncStats();
    // GPU busy time (command lists, start to end) collected since the last call, in milliseconds.
    double TakeGpuMilliseconds();
    // The same split by kind of work (draws, resolves, texture untiles, present), as text.
    std::string TakeGpuBreakdown();
    // At a swap; `frame` = frames presented so far (including this one), the front buffer (physical) and its size.
    struct GuestOverlay;
    // `overlay`: XAM's system UI overlay for this frame (base 0 = none; the runtime's own system UI otherwise).
    void RenderSwap(uint64_t frame, uint32_t frontBuffer, uint32_t width, uint32_t height, const GuestOverlay& overlay);

    // Integration test of the renderer (resolves, clears, texture invalidation, present) and the texture tests
    // (layouts against the CPU reference and the XDK's cases, sampling); needs guest memory and the GPU
    // register block initialised. Returns the number of failures.
    int RunRendererTests();
}
