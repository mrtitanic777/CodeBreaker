#pragma once

#include <cstdint>
#include <string>

// Xenos GPU emulation, driven by the title's own (recompiled) Direct3D library.
//
// D3D writes PM4 command packets into a ring buffer in physical memory and pokes GPU registers in the MMIO block at
// 0x7FC80000 (register n at 0x7FC80000 + 4n). Registers below 0x4000 live in that guest memory so the title's direct
// register reads/writes need no hooks; the shader-constant registers (0x4000+) are host-side. A command-processor
// thread watches CP_RB_WPTR, executes packets and writes the read pointer back.
//
// The command processor executes everything that affects synchronisation or memory (indirect buffers, waits,
// register/memory writes, fences, occlusion reports, interrupts, predication), tracks the loaded shaders
// (shader_cache.h), decodes draws (draw_trace.h) and hands them to the renderer (renderer.h).
namespace gpu
{
    namespace reg
    {
        constexpr uint32_t kCpRbBase = 0x01C0;
        constexpr uint32_t kCpRbRptr = 0x01C4;
        constexpr uint32_t kCpRbWptr = 0x01C5;
        constexpr uint32_t kScratchUmsk = 0x01DC;
        constexpr uint32_t kScratchAddr = 0x01DD;
        constexpr uint32_t kScratchReg0 = 0x0578;
        constexpr uint32_t kWritebackStart = 0x0A04;
        constexpr uint32_t kWritebackSize = 0x0A05;
        constexpr uint32_t kCoherStatusHost = 0x0A31;
        constexpr uint32_t kRbEdramTiming = 0x0F00;
        constexpr uint32_t kRbBcControl = 0x0F01;
        constexpr uint32_t kRbHsioInterfaceAlignerValue = 0x0F2D;
        constexpr uint32_t kDcLutRwMode = 0x1921;
        constexpr uint32_t kDcLutRwIndex = 0x1922;
        constexpr uint32_t kDcLutSeqColor = 0x1923;
        constexpr uint32_t kDcLutPwlData = 0x1924;
        constexpr uint32_t kDcLut30Color = 0x1925;
        constexpr uint32_t kDcLutWriteEnMask = 0x1927;
        constexpr uint32_t kDcLutaControl = 0x1930;
        constexpr uint32_t kDcLutaBlackOffsetBlue = 0x1931; // .. kDcLutaWhiteOffsetRed = 0x1936
        constexpr uint32_t kD1ModeVblankVlineStatus = 0x1951;
        constexpr uint32_t kD1ModeViewportSize = 0x1961;
        constexpr uint32_t kPaScVizQueryStatus0 = 0x0C44;
        constexpr uint32_t kPaScVizQueryStatus1 = 0x0C45;
        constexpr uint32_t kVgtEventInitiator = 0x21F9;
        constexpr uint32_t kVgtDmaBase = 0x21FA;
        constexpr uint32_t kVgtDmaSize = 0x21FB;
        constexpr uint32_t kVgtDrawInitiator = 0x21FC;
        constexpr uint32_t kPaScVizQuery = 0x2293;
        constexpr uint32_t kRbSampleCountAddr = 0x2325;
        constexpr uint32_t kCount = 0x5003;
    }

    constexpr uint32_t kMmioBase = 0x7FC80000;
    constexpr uint32_t kMmioSize = 0x10000;
    constexpr uint32_t kWritebackVirtualBase = 0x7F000000;
    constexpr uint32_t kWritebackVirtualSize = 0x00C80000;

    // The display controller's gamma LUT (analysis/xenos/08_present.md §5). D3D writes it with DC_LUT_* register
    // writes in the PM4 stream; the presenter applies it to the front buffer.
    struct DisplayGamma
    {
        uint32_t table[256];     // 256-entry table, DC_LUT_30_COLOR layout: blue bits 0-9, green 10-19, red 20-29
        uint32_t pwl[128][3];    // PWL entries for R, G, B: DC_LUT_PWL_DATA layout, base bits 0-15, delta 16-31
        uint64_t tableWrites;    // component writes so far (for logs)
        uint64_t pwlWrites;
    };
    // Command-processor thread.
    const DisplayGamma& CurrentDisplayGamma();

    // Runtime-private type-3 packet written by VdSwap: { 'SWAP', physical front buffer, width, height }.
    constexpr uint32_t kOpcodeRuntimeSwap = 0x64;
    constexpr uint32_t kSwapSignature = 0x53574150; // "SWAP"

    // Commits the MMIO register block and the GPU writeback window, sets the fixed-value registers.
    void Initialize();

    void InitializeRingBuffer(uint32_t physical, uint32_t sizeLog2);
    void EnableReadPointerWriteBack(uint32_t address, uint32_t blockSizeLog2);
    // Sleeps until the GPU writes something the CPU may be polling for (a fence, a write-back), or the time is up.
    void WaitForProgress(uint32_t microseconds);
    void SetInterruptCallback(uint32_t callback, uint32_t userData);
    // Drops the interrupt callback when it is code in [start, end) (a title ExTerminateTitleProcess ends).
    void ClearInterruptCallbackIn(uint32_t start, uint32_t end);

    uint32_t ReadRegister(uint32_t index);
    // Emulated vblanks so far (60 per second of the vblank thread's clock).
    uint64_t VblankCount();
    // Sets a register without side effects (tests only).
    void SetRegister(uint32_t index, uint32_t value);
    // A register write with the command processor's side effects, e.g. DC_LUT_* (tests only).
    void TestWriteRegister(uint32_t index, uint32_t value);

    // Shader constants (register indices 0x4000 and up) in host byte order, contiguous from `first`, and a version per
    // block (ALU floats 0x4000-0x47FF, fetch 0x4800-0x48BF, bool/loop 0x4900-0x4927) that changes whenever a value in
    // that block changes: the renderer reuses a block's previous upload while its version is the same.
    enum class ConstantBlock { kFloat, kFetch, kBoolLoop, kCount };
    const uint32_t* HostRegisters(uint32_t first);
    uint64_t ConstantVersion(ConstantBlock block);

    // Frames presented (runtime swap packets executed).
    uint64_t SwapCount();
    // Feeds a GPU trace recorded by a Xenia-based renderer (.xtr) to the command processor, stopping after `frames`
    // swaps (0: all); CB_REPLAY_TRACE in main.cpp.
    void ReplayTrace(const std::string& path, uint64_t frames);
    // The runtime's own frames before the kernel and a title run (main.cpp's dashboard launcher): presents a black
    // width x height front buffer with the system UI overlay over it (xui::RenderOverlay: its full-screen scene), as
    // a title's swap does, counts a vblank and a frame, and feeds the input (input::OnFrame). The caller paces it at
    // 60 per second. EndRuntimeFrames releases the front buffer before the title starts.
    void PresentRuntimeFrame(uint32_t width, uint32_t height);
    void EndRuntimeFrames();
    // The command processor's current packet (stall reports).
    std::string DescribeCommandProcessor();
}
