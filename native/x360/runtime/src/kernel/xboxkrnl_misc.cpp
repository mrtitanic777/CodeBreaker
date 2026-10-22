// Miscellaneous xboxkrnl exports.

#include "kernel/kernel.h"

#include <algorithm>
#include <mutex>
#include <vector>

namespace
{
    std::mutex g_terminateMutex;
    std::vector<uint32_t> g_terminateRegistrations; // EX_TITLE_TERMINATE_REGISTRATION addresses
}

// VOID KiApcNormalRoutineNop(PVOID NormalContext, PVOID SystemArgument1, PVOID SystemArgument2)
// The kernel's do-nothing APC routine; titles use it as a default callback. It returns immediately.
GUEST_IMPORT(KiApcNormalRoutineNop)
{
}

// VOID ExRegisterTitleTerminateNotification(PEX_TITLE_TERMINATE_REGISTRATION Registration, BOOLEAN Create)
// Registers (Create = TRUE) or removes a routine the kernel runs when the title terminates (Registration: +0 the
// routine, called with the registration; +4 its priority). ExTerminateTitleProcess runs them
// (kernel/title_process.cpp).
GUEST_IMPORT(ExRegisterTitleTerminateNotification)
{
    std::lock_guard lock(g_terminateMutex);
    const uint32_t registration = ctx.r3.u32;
    auto it = std::find(g_terminateRegistrations.begin(), g_terminateRegistrations.end(), registration);
    if (ctx.r4.u32)
    {
        if (it == g_terminateRegistrations.end())
            g_terminateRegistrations.push_back(registration);
    }
    else if (it != g_terminateRegistrations.end())
    {
        g_terminateRegistrations.erase(it);
    }
}


// ---- Event tracing (ETX) -----------------------------------------------------------------------------------------
// Producers register their event descriptions; events reach a consumer only when a trace session listens (a
// development tool). A retail console has none, so registration succeeds and logged events go nowhere.
// dash.xex: EtxProducerRegister(descriptor, info, 9, 0, 0x40, producer) with a zeroed 0x40-byte producer block.

// NTSTATUS EtxProducerRegister(...)
GUEST_IMPORT(EtxProducerRegister)
{
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS EtxProducerUnregister(...)
GUEST_IMPORT(EtxProducerUnregister)
{
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS EtxProducerLog(...): no trace session listens.
GUEST_IMPORT(EtxProducerLog)
{
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS FscSetCacheElementCount(ULONG CacheIndex, ULONG ElementCount): sizes the kernel's file system cache (in
// 4 KB pages; the dash's XSetFileCacheSize asks for 1 MB). Host file I/O goes through the PC's own cache.
GUEST_IMPORT(FscSetCacheElementCount)
{
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// DWORD XUsbcamGetState(): the Xbox Live Vision camera (2 = attached). None is attached: 0.
GUEST_IMPORT(XUsbcamGetState)
{
    ctx.r3.u64 = 0;
}

// DWORD XUsbcamCreate(PVOID Buffer, DWORD BufferSize, PHANDLE Camera): sets up the Xbox Live Vision camera driver
// over the caller's frame buffer (the dash's XCamInitialize passes 0x4B000 bytes, one 640x480 frame). The camera is
// hot-pluggable, so creation does not need one attached; XUsbcamGetState then reports whether one is. No kernel
// binary to confirm with: Xenia Canary also succeeds (titles stop initialising otherwise).
GUEST_IMPORT(XUsbcamCreate)
{
    constexpr uint32_t kCameraObjectSize = 0x40;
    if (ctx.r5.u32)
        guest::g_memory.Write32(ctx.r5.u32, guest::g_memory.SystemAlloc(kCameraObjectSize));
    ctx.r3.u64 = 0;
}

// VOID XUsbcamDestroy(HANDLE Camera): releases what XUsbcamCreate set up (the dash's XCamShutdown when its last user
// leaves). Here: the object XUsbcamCreate allocated.
GUEST_IMPORT(XUsbcamDestroy)
{
    if (ctx.r3.u32)
        guest::g_memory.SystemFree(ctx.r3.u32);
}

std::vector<uint32_t> kernel::TitleTerminateRegistrations()
{
    std::lock_guard lock(g_terminateMutex);
    return g_terminateRegistrations;
}
