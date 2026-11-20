// XAM's memory allocator for titles and system code (XamAlloc / XamAllocEx / XamFree).
//
// XAM 17559 (0x816E5F10 / 0x816E63D8) turns the caller's flags into a heap choice: the title's heap for title callers
// (flags 0, 0x8000000, 0x18000000 ...), XAM's own heaps for system callers, the encrypted heap for 0x24400000.
// All of them are ordinary zero-filled heap blocks to the caller; here they come from one pool, XAM's title heap, a
// committed region of the 4 KiB virtual heap. Flag 0x40000000 asks for physical memory instead (0x816E6070), aligned
// as the attributes' bits 24-27 say.

#include "kernel/kernel.h"
#include "pool.h"

#include <mutex>

namespace
{
    using namespace guest;

    constexpr uint32_t kTitleHeapSize = 64 * 1024 * 1024;
    constexpr uint32_t kFlagPhysical = 0x40000000;
    constexpr uint32_t kInvalidArg = 0x80070057;
    constexpr uint32_t kOutOfMemory = 0x8007000E;

    GuestPool& TitleHeap()
    {
        static GuestPool pool;
        static std::once_flag once;
        std::call_once(once, [] {
            const uint32_t base = g_memory.virtual4k.Allocate(kTitleHeapSize, 0x10000, true, kPageReadWrite, true);
            if (base == 0)
                Fatal("XAM title heap: unable to reserve {:X} bytes", kTitleHeapSize);
            pool.Initialize(base, kTitleHeapSize);
            Log("XAM title heap at {:08X} ({:X} bytes)", base, kTitleHeapSize);
        });
        return pool;
    }

    uint32_t HeapAlloc(uint32_t flags, uint32_t size, uint32_t out)
    {
        if (out == 0)
            return kInvalidArg;
        const uint32_t block = TitleHeap().Allocate(size);
        if (block == 0)
            Fatal("XamAlloc(flags {:08X}, {:X} bytes): XAM title heap exhausted", flags, size);
        g_memory.Write32(out, block);
        return 0;
    }

    // Physical memory (4 KiB pages), aligned to 1 << ((attributes >> 24) & 0xF) bytes, at least a page.
    uint32_t PhysicalAlloc(uint32_t attributes, uint32_t size)
    {
        const uint32_t alignment = std::max<uint32_t>(1u << ((attributes >> 24) & 0xF), 0x1000);
        const uint32_t bytes = (std::max<uint32_t>(size, 1) + 0xFFF) & ~0xFFFu;
        const uint32_t physical = g_memory.physical.Allocate(bytes, alignment, 0, 0x1FFFFFFF, true);
        if (physical == UINT32_MAX)
            return 0;
        g_memory.CommitPhysical(physical, bytes);
        memset(g_memory.Translate(PhysicalToWindow(kPhysicalWindow4k, physical)), 0, bytes);
        return PhysicalToWindow(kPhysicalWindow4k, physical);
    }
}

// HRESULT XamAlloc(DWORD Flags, DWORD Size, PVOID* Block)
GUEST_IMPORT(XamAlloc)
{
    ctx.r3.u64 = HeapAlloc(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// HRESULT XamAllocEx(DWORD PhysicalAttributes, DWORD Flags, DWORD Size, PVOID* Block)
GUEST_IMPORT(XamAllocEx)
{
    const uint32_t attributes = ctx.r3.u32, flags = ctx.r4.u32, size = ctx.r5.u32, out = ctx.r6.u32;
    if (!(flags & kFlagPhysical))
    {
        ctx.r3.u64 = HeapAlloc(flags, size, out);
        return;
    }
    const uint32_t block = PhysicalAlloc(attributes, size);
    g_memory.Write32(out, block);
    ctx.r3.u64 = block ? 0 : kOutOfMemory;
}

// VOID XamFree(PVOID Block)
GUEST_IMPORT(XamFree)
{
    const uint32_t block = ctx.r3.u32;
    if (block == 0)
        return;
    if (IsPhysicalWindow(block))
    {
        if (g_memory.physical.Free(WindowToPhysical(block)) == 0)
            Fatal("XamFree({:08X}): not the start of a physical allocation", block);
        return;
    }
    if (!TitleHeap().Free(block))
        Fatal("XamFree({:08X}): not a XamAlloc block", block);
}
