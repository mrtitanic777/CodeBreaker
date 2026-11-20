// xboxkrnl video exports (Vd*): the kernel side of the GPU driver. The title's D3D calls these to set up the ring
// buffer, interrupt callback and display, and VdSwap to queue a present. Behaviour notes marked "Canary" follow
// Xenia Canary where the console behaviour is not documented.

#include "gpu/gpu.h"
#include "kernel/kernel.h"
#include "settings.h"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <mutex>

namespace
{
    void WriteFloat(uint32_t address, float value)
    {
        uint32_t bits;
        memcpy(&bits, &value, 4);
        guest::g_memory.Write32(address, bits);
    }
}

// VOID VdGetCurrentDisplayGamma(PDWORD Type, PFLOAT Power): BT.709 (type 2), power 2.2222 (Canary).
GUEST_IMPORT(VdGetCurrentDisplayGamma)
{
    guest::g_memory.Write32(ctx.r3.u32, 2);
    WriteFloat(ctx.r4.u32, 2.22222233f);
}

// VOID VdGetCurrentDisplayInformation(PX_DISPLAY_INFO Info) (0x58 bytes)
GUEST_IMPORT(VdGetCurrentDisplayInformation)
{
    auto& mem = guest::g_memory;
    const ConsoleSettings& s = Settings();
    const uint32_t info = ctx.r3.u32;
    memset(mem.Translate(info), 0, 0x58);
    mem.Write16(info + 0x00, uint16_t(s.videoWidth));   // front buffer width/height
    mem.Write16(info + 0x02, uint16_t(s.videoHeight));
    mem.Write32(info + 0x08, 0);                         // scaler source rect
    mem.Write32(info + 0x0C, 0);
    mem.Write32(info + 0x10, s.videoWidth);
    mem.Write32(info + 0x14, s.videoHeight);
    mem.Write32(info + 0x18, s.videoWidth);             // scaled output size
    mem.Write32(info + 0x1C, s.videoHeight);
    mem.Write32(info + 0x20, 1);                        // vertical filter type
    mem.Write32(info + 0x30, 1);                        // horizontal filter type
    mem.Write16(info + 0x40, uint16_t(s.videoWidth / 4)); // overscan left/top/right/bottom
    mem.Write16(info + 0x42, uint16_t(s.videoHeight / 4));
    mem.Write16(info + 0x44, uint16_t(s.videoWidth / 4));
    mem.Write16(info + 0x46, uint16_t(s.videoHeight / 4));
    mem.Write16(info + 0x48, uint16_t(s.videoWidth));    // display size
    mem.Write16(info + 0x4A, uint16_t(s.videoHeight));
    WriteFloat(info + 0x4C, s.refreshRate);
    mem.Write32(info + 0x50, s.interlaced ? 1 : 0);
    mem.Write16(info + 0x56, uint16_t(s.videoWidth));    // actual display width
}

// DWORD VdSetDisplayMode(DWORD Flags)
GUEST_IMPORT(VdSetDisplayMode)
{
    ctx.r3.u64 = 0;
}

// DWORD VdSetDisplayModeOverride(DWORD Width, DWORD Height, DOUBLE Refresh, DWORD, DWORD)
GUEST_IMPORT(VdSetDisplayModeOverride)
{
    ctx.r3.u64 = 0;
}

// DWORD VdInitializeEngines(DWORD, PVOID Callback, PVOID Arg, PDWORD PfpMicrocode, PDWORD MeMicrocode)
GUEST_IMPORT(VdInitializeEngines)
{
    ctx.r3.u64 = 1;
}

// VOID VdShutdownEngines(): titles call Initialize/Shutdown/Initialize during startup.
GUEST_IMPORT(VdShutdownEngines)
{
}

// DWORD VdEnableDisableClockGating(BOOL Enable)
GUEST_IMPORT(VdEnableDisableClockGating)
{
    ctx.r3.u64 = 0;
}

// VOID VdSetGraphicsInterruptCallback(PVOID Callback, PVOID UserData)
GUEST_IMPORT(VdSetGraphicsInterruptCallback)
{
    gpu::SetInterruptCallback(ctx.r3.u32, ctx.r4.u32);
}

// VOID VdInitializeRingBuffer(PVOID PhysicalAddress, DWORD SizeLog2): size in bytes is 1 << (SizeLog2 + 3).
GUEST_IMPORT(VdInitializeRingBuffer)
{
    gpu::InitializeRingBuffer(ctx.r3.u32, ctx.r4.u32);
}

// VOID VdEnableRingBufferRPtrWriteBack(PVOID Address, DWORD BlockSizeLog2)
GUEST_IMPORT(VdEnableRingBufferRPtrWriteBack)
{
    gpu::EnableReadPointerWriteBack(ctx.r3.u32, ctx.r4.u32);
}

// ---- The system command buffer: XAM's Guide over the title's frames (CB_RECOMP_XAM) ----
// XAM draws the system UI (the Guide, notifications) with its own command buffer into an overlay texture and hands
// both to the kernel (VdSetSystemCommandBuffer, xam.xex 0x81842B08). At each Present the title's D3D takes them
// (VdGetSystemCommandBuffer) and passes them to VdSwap, which runs the command buffer in the title's ring before the
// flip, and the display composites the overlay over the front buffer. The GPU then writes the command buffer's
// identifier to the address D3D registered (VdSetSystemCommandBufferGpuIdentifierAddress); D3D writes it there itself
// when it skips the buffer. XAM draws its next frame once that identifier has come back (VdQuerySystemCommandBuffer).
// Descriptor (0x94 bytes): +0 command buffer, +4 its size in dwords (bits 0-23), +8 overlay texture base, +0xC its
// format, +0x10 a mode, +0x14 width, +0x18 height (no overlay: base 0), +0x20 and +0x58 two 0x38-byte scaler
// parameter blocks (D3DPRIVATE_SCALER_PARAMETERS: source and destination rectangles; +0x1C and +0x90 say whether each
// is given). The scaler blocks are not applied yet: the overlay is scaled to the whole frame.
namespace
{
    std::mutex g_systemBufferMutex;
    uint8_t g_systemBuffer[0x94] = {};
    uint32_t g_systemBufferId = 0;          // identifier of the descriptor last set
    bool g_systemBufferTaken = true;        // its command buffer was handed to a VdSwap already
    std::atomic<uint32_t> g_gpuIdentifierAddress = 0;
}

// VOID VdSetSystemCommandBuffer(PVOID Descriptor)
GUEST_IMPORT(VdSetSystemCommandBuffer)
{
    std::lock_guard lock(g_systemBufferMutex);
    std::memcpy(g_systemBuffer, guest::g_memory.Translate(ctx.r3.u32), sizeof(g_systemBuffer));
    g_systemBufferId++;
    g_systemBufferTaken = false;
    static const bool dump = std::getenv("CB_SYSTEM_COMMANDS_DUMP") != nullptr;
    if (dump)
    {
        auto& mem = guest::g_memory;
        std::string words;
        for (uint32_t i = 0; i < 0x94; i += 4)
            words += std::format(" {:08X}", mem.Read32(ctx.r3.u32 + i));
        Log("VdSetSystemCommandBuffer {}:{}", g_systemBufferId, words);
    }
}

// BOOL VdQuerySystemCommandBuffer(DWORD): whether the GPU has run the command buffer last set (its identifier is
// back at the registered address), so XAM may build the next one.
GUEST_IMPORT(VdQuerySystemCommandBuffer)
{
    std::lock_guard lock(g_systemBufferMutex);
    const uint32_t address = g_gpuIdentifierAddress.load();
    ctx.r3.u64 = (address == 0 || int32_t(guest::g_memory.Read32(address) - g_systemBufferId) >= 0) ? 1 : 0;
}

// VOID VdGetSystemCommandBuffer(PVOID Descriptor /* 0x94 bytes */, PDWORD Identifier): the descriptor last set and
// its identifier, both passed on to VdSwap. A command buffer is handed out once; later frames get the overlay alone.
// Without the recompiled XAM there is none (the runtime's own system UI draws at present).
GUEST_IMPORT(VdGetSystemCommandBuffer)
{
    auto& mem = guest::g_memory;
    uint8_t* out = mem.Translate(ctx.r3.u32);
    if (!kernel::RecompiledXam())
    {
        memset(out, 0, 0x94);
        mem.Write32(ctx.r3.u32, 0xBEEF0000);
        mem.Write32(ctx.r4.u32, 0xBEEF0001);
        return;
    }
    std::lock_guard lock(g_systemBufferMutex);
    std::memcpy(out, g_systemBuffer, sizeof(g_systemBuffer));
    if (g_systemBufferTaken)
    {
        mem.Write32(ctx.r3.u32, 0);
        mem.Write32(ctx.r3.u32 + 4, 0);
    }
    g_systemBufferTaken = true;
    mem.Write32(ctx.r4.u32, g_systemBufferId);
}

// VOID VdSetSystemCommandBufferGpuIdentifierAddress(PVOID Address): where the GPU writes the identifier of each
// system command buffer it has run (D3D's write-back block; 0 when the device goes).
GUEST_IMPORT(VdSetSystemCommandBufferGpuIdentifierAddress)
{
    g_gpuIdentifierAddress = ctx.r3.u32;
}

// DWORD VdInitializeScalerCommandBuffer(..., PDWORD Dest (arg 10), DWORD DestCount (arg 11))
// The display scaler is not emulated: the buffer is filled with type-2 NOPs and its length returned.
GUEST_IMPORT(VdInitializeScalerCommandBuffer)
{
    auto& mem = guest::g_memory;
    const uint32_t dest = kernel::StackArg32(ctx, 10);
    const uint32_t count = kernel::StackArg32(ctx, 11);
    for (uint32_t i = 0; i < count; i++)
        mem.Write32(dest + i * 4, 0x80000000);
    ctx.r3.u64 = count;
}

// DWORD VdCallGraphicsNotificationRoutines(DWORD Event, PVOID Data): the title's D3D tells XAM about its display
// (1: scaler parameters, 9: the stereo 3D buffer it lends XAM) through XAM's routine
// (VdRegisterXamGraphicsNotification; xam.xex 0x816CBFD8 takes (?, Event, Data)).
GUEST_IMPORT(VdCallGraphicsNotificationRoutines)
{
    if (const uint32_t routine = kernel::XamGraphicsNotification())
    {
        PPCContext call = ctx;
        call.r5.u64 = ctx.r4.u32;
        call.r4.u64 = ctx.r3.u32;
        call.r3.u64 = 0;
        kernel::CallGuest(call, routine);
    }
    ctx.r3.u64 = 0;
}

// BOOL VdIsHSIOTrainingSucceeded()
GUEST_IMPORT(VdIsHSIOTrainingSucceeded)
{
    ctx.r3.u64 = 1;
}

// BOOL VdPersistDisplay(DWORD, PDWORD Out): hands back a 64-byte physical allocation the title frees later.
GUEST_IMPORT(VdPersistDisplay)
{
    auto& mem = guest::g_memory;
    if (const uint32_t out = ctx.r4.u32)
    {
        const uint32_t physical = mem.physical.Allocate(0x1000, 0x1000, 0, 0x1FFFFFFF, true);
        if (physical == UINT32_MAX)
            Fatal("VdPersistDisplay: out of physical memory");
        mem.CommitPhysical(physical, 0x1000);
        mem.Write32(out, guest::PhysicalToWindow(guest::kPhysicalWindow4k, physical));
    }
    ctx.r3.u64 = 1;
}

// DWORD VdRetrainEDRAM(...), VdRetrainEDRAMWorker(PVOID)
GUEST_IMPORT(VdRetrainEDRAM)
{
    ctx.r3.u64 = 0;
}

GUEST_IMPORT(VdRetrainEDRAMWorker)
{
    ctx.r3.u64 = 0;
}

// VOID VdSwap(PDWORD RingSpace, PDWORD FetchConstant, PDWORD IdentifierAddress, PVOID SystemDescriptor,
//             DWORD SystemIdentifier, PDWORD FrontBuffer, PDWORD Format, PDWORD ColorSpace, PDWORD Width (arg 8),
//             PDWORD Height (arg 9))
// D3D reserves 64 dwords in the ring at RingSpace. They are filled with: the system command buffer
// (VdGetSystemCommandBuffer's descriptor, CB_RECOMP_XAM) as an INDIRECT_BUFFER, the front buffer's texture fetch
// constant written to SHADER_CONSTANT_FETCH_00 (base address converted to physical), the runtime's private swap
// packet (with the system identifier to write back and XAM's overlay), NOPs.
GUEST_IMPORT(VdSwap)
{
    auto& mem = guest::g_memory;
    const uint32_t ring = ctx.r3.u32;
    const uint32_t fetch = ctx.r4.u32;
    const uint32_t frontBuffer = mem.Read32(ctx.r8.u32);
    const uint32_t width = mem.Read32(kernel::StackArg32(ctx, 8));
    const uint32_t height = mem.Read32(kernel::StackArg32(ctx, 9));

    // Window addresses become physical (the 0xE0000000 window is offset by 0x1000); others already are.
    auto toPhysical = [](uint32_t address) {
        return guest::IsPhysicalWindow(address) ? guest::WindowToPhysical(address) : address;
    };

    uint32_t fetchDwords[6];
    for (int i = 0; i < 6; i++)
        fetchDwords[i] = mem.Read32(fetch + i * 4);
    // Dword 1 bits 31:12 hold the base address (4 KiB units). It comes from the D3D texture header, so it is a CPU
    // address in one of the physical windows, and it is the front buffer itself.
    const uint32_t baseVirtual = fetchDwords[1] & 0xFFFFF000;
    if (baseVirtual != frontBuffer)
        Fatal("VdSwap: fetch constant base {:08X} differs from the front buffer {:08X}", baseVirtual, frontBuffer);
    if (!guest::IsPhysicalWindow(baseVirtual) && baseVirtual >= guest::kPhysicalSize)
        Fatal("VdSwap: front buffer {:08X} is not in physical memory", baseVirtual);
    // What D3D guarantees (08_present §2; Canary asserts the same): the format normalised to k_8_8_8_8 (6) or
    // k_2_10_10_10_AS_16_16_16_16 (54), an RGB front buffer, and a size equal to the fetch constant's (+ border).
    const uint32_t format = mem.Read32(ctx.r9.u32);
    const uint32_t colorSpace = mem.Read32(ctx.r10.u32);
    const uint32_t border = fetchDwords[3] >> 31;
    const uint32_t fetchWidth = (fetchDwords[2] & 0x1FFF) + 1 + 2 * border;
    const uint32_t fetchHeight = ((fetchDwords[2] >> 13) & 0x1FFF) + 1 + 2 * border;
    if ((format != 6 && format != 54) || format != (fetchDwords[1] & 0x3F))
        Fatal("VdSwap: front-buffer format {} (fetch constant format {})", format, fetchDwords[1] & 0x3F);
    if (colorSpace != 0)
        Fatal("VdSwap: front-buffer colour space {} is not RGB", colorSpace);
    if (width != fetchWidth || height != fetchHeight)
        Fatal("VdSwap: size {}x{} differs from the fetch constant's {}x{}", width, height, fetchWidth, fetchHeight);
    fetchDwords[1] = (fetchDwords[1] & 0xFFF) | toPhysical(baseVirtual);
    static std::atomic<bool> logged = false;
    if (!logged.exchange(true))
        Log("VdSwap: front buffer {:08X} (physical {:08X}), {}x{}", frontBuffer, toPhysical(frontBuffer), width, height);

    uint32_t dword = 0;
    auto put = [&](uint32_t value) { mem.Write32(ring + dword++ * 4, value); };
    uint32_t identifierAddress = 0, identifier = 0, overlay[9] = {};
    if (kernel::RecompiledXam() && ctx.r6.u32 && ctx.r5.u32)
    {
        const uint32_t desc = ctx.r6.u32;
        identifierAddress = ctx.r5.u32;
        identifier = ctx.r7.u32;
        if (const uint32_t commands = mem.Read32(desc); commands != 0)
        {
            // XAM's command buffer draws the title's frame (dimmed) behind the Guide from texture fetch constant 15,
            // which it leaves alone when it resets the others (xam.xex's pixel shader: tfetch2D tf15 * c0).
            put((5u << 16) | (0x4800 + 15 * 6));                // type 0: SHADER_CONSTANT_FETCH_15
            for (uint32_t v : fetchDwords)
                put(v);
            const uint32_t size = mem.Read32(desc + 4) & 0xFFFFFF;
            put(0xC0000000u | (1u << 16) | (0x3Fu << 8));       // INDIRECT_BUFFER
            put(toPhysical(commands));
            put(size);
            // CB_SYSTEM_COMMANDS_DUMP=1: each system command buffer's dwords (big-endian) as syscmd_<id>.bin.
            static const bool dump = std::getenv("CB_SYSTEM_COMMANDS_DUMP") != nullptr;
            if (dump)
                if (FILE* f = std::fopen(std::format("syscmd_{}.bin", ctx.r7.u32).c_str(), "wb"))
                {
                    std::fwrite(mem.Translate(guest::PhysicalToWindow(guest::kPhysicalWindow4k, toPhysical(commands))), 4, size, f);
                    std::fclose(f);
                }
        }
        if (mem.Read32(desc + 8)) // +0x1C / +0x90 only say whether the scaler parameter blocks are given
        {
            overlay[0] = toPhysical(mem.Read32(desc + 8));
            overlay[1] = mem.Read32(desc + 0xC);
            overlay[2] = mem.Read32(desc + 0x14);
            overlay[3] = mem.Read32(desc + 0x18);
            overlay[4] = mem.Read32(desc + 0x10);
            // The first scaler block's source rectangle: the part of the overlay shown. Its output size and the
            // second block (the title's own scaler: its front buffer's source rectangle and output) are what the
            // display shows anyway here (the window presents the front buffer whole).
            if (mem.Read32(desc + 0x1C))
                for (uint32_t i = 0; i < 4; i++)
                    overlay[5 + i] = mem.Read32(desc + 0x20 + i * 4);
        }
    }
    put((5u << 16) | 0x4800);                                   // type 0: 6 registers at SHADER_CONSTANT_FETCH_00_0
    for (uint32_t v : fetchDwords)
        put(v);
    put(0xC0000000u | (14u << 16) | (gpu::kOpcodeRuntimeSwap << 8)); // type 3, 15 payload dwords
    put(gpu::kSwapSignature);
    put(toPhysical(frontBuffer));
    put(width);
    put(height);
    put(identifierAddress);
    put(identifier);
    for (uint32_t v : overlay)                                  // base, format, width, height, mode, source rect
        put(v);
    while (dword < 64)
        put(0x80000000);                                        // type-2 NOP
}

// VOID VdSetStudioRGBMode(): applies the user's "reference levels" setting (studio 16-235 or full-range RGB) to the
// video encoder. The dash calls it at start-up and from its video compositor (resolved by ordinal 889) and ignores
// any result. The runtime presents full-range RGB through the host swap chain; nothing to switch.
GUEST_IMPORT(VdSetStudioRGBMode)
{
}

namespace
{
    std::atomic<uint32_t> g_3dVideoFormat = 0;
}

// DWORD VdSet3dVideoFormat(DWORD Format): stereoscopic output format (0 = none, 3 = frame-packed 720p/1080p). D3D's
// Set3DVideoFormat looks it up by ordinal (873) and ignores the result; XAM reads the format back with
// VdGet3dVideoFormat to draw the Guide in stereo. Stored; returns 1 (no caller reads it).
// Imported by xam.xex (CB_RECOMP_XAM), looked up by ordinal by the dash's D3D.
GUEST_IMPORT(VdSet3dVideoFormat)
{
    g_3dVideoFormat = ctx.r3.u32;
    ctx.r3.u64 = 1;
}
static kernel::DynamicExportRegistrar VdSet3dVideoFormat_registrar("xboxkrnl.exe", 873, __imp__VdSet3dVideoFormat);

// DWORD VdGet3dVideoFormat()
GUEST_IMPORT(VdGet3dVideoFormat)
{
    ctx.r3.u64 = g_3dVideoFormat.load();
}
static kernel::DynamicExportRegistrar VdGet3dVideoFormat_registrar("xboxkrnl.exe", 871, __imp__VdGet3dVideoFormat);

// BOOL VdReadEEDIDBlock(DWORD Block, PBYTE Buffer /* 128 bytes */): the connected display's E-EDID. The runtime has
// no HDMI sink to read, so no block is available (FALSE, buffer untouched), as with a non-HDMI connection.
GUEST_IMPORT(VdReadEEDIDBlock)
{
    ctx.r3.u64 = 0;
}

// DWORD VdGetGraphicsAsicID(): the graphics chip's revision. The older dashboards (1888 ... 3519) compare it with
// 0x10: below, VdInitializeEDRAM (the launch consoles' chip); otherwise they retrain the EDRAM themselves (D3D,
// guest code). This runtime stands for a later console (the 17559 system's), so a later chip: 0x11, the value rexglue
// documents for it.
GUEST_IMPORT(VdGetGraphicsAsicID)
{
    ctx.r3.u64 = 0x11;
}
