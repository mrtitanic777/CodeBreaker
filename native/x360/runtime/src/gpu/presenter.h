#pragma once

#include <chrono>
#include <cstdint>

#include <d3d12.h>

// The game window: shows the frames the title presents (VdSwap) with Direct3D 12.
//
// At every swap the renderer writes the guest output (the front buffer as the display controller shows it: detiled,
// display gamma applied; present.h) into one of three R10G10B10A2 textures and submits it here. A presenter thread
// owns the window and a flip-model swap chain on its own queue and shows the newest guest output (mailbox: a newer
// one replaces one not shown yet), so presenting never stalls GPU emulation; the title paces itself by the emulated
// vblank. Queues synchronise with fences only (no CPU copies or waits per frame). Frames are scaled to the window
// with bilinear filtering, keeping their aspect ratio. Closing the window ends the process. CB_HEADLESS=1 runs
// without a window.
namespace gpu
{
    // Opens the window (unless headless). Returns immediately; the window lives on its own thread.
    void StartPresenter(uint32_t width, uint32_t height);
    bool PresenterActive();
    // Waits (at most `timeout`) until the window is up with its first (black) frame on the screen, full screen
    // included: the boot animation starts after it, so its sound starts with its picture.
    void WaitForWindow(std::chrono::milliseconds timeout);
    // Leaving for another process: the window's picture fades to black over `duration`; returns
    // once black is on the screen (at once without a window). New frames stay black until ClearFade (the launch
    // failed: the title's picture comes back).
    void FadeToBlack(std::chrono::milliseconds duration);
    void ClearFade();

    // A guest-output texture: R10G10B10A2_TYPELESS with unordered access, in the COMMON state between uses.
    struct GuestOutput
    {
        uint32_t index;
        ID3D12Resource* texture;
        ID3D12Fence* readFence;  // the window's reads of the texture are finished when this fence reaches readValue
        uint64_t readValue;
    };
    // Command-processor thread: a texture of this size that the window is neither showing nor about to show. The
    // writer's queue must wait for readFence >= readValue before writing it.
    GuestOutput AcquireGuestOutput(uint32_t width, uint32_t height);
    // The texture acquired as `index` is complete once `fence` reaches `value`: show it (released when headless).
    void SubmitGuestOutput(uint32_t index, ID3D12Fence* fence, uint64_t value);
}
