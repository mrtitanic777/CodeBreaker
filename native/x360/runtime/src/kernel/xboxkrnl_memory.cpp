// xboxkrnl virtual-memory exports (Nt*VirtualMemory).
// Semantics follow the NT memory model as implemented by the Xbox 360 kernel; edge cases (64 KiB rounding of new
// reservations, negative sizes) were cross-checked against Xenia Canary.

#include "kernel/kernel.h"
#include "modules.h"

namespace
{
    constexpr uint32_t kStatusMemoryNotAllocated = 0xC00000A0;
    constexpr uint32_t kReservationGranularity = 0x10000;

    uint32_t RoundUp(uint32_t value, uint32_t multiple)
    {
        return uint32_t((uint64_t(value) + multiple - 1) / multiple * multiple);
    }
}

// NTSTATUS NtAllocateVirtualMemory(PVOID* BaseAddress, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect,
//                                  BOOLEAN DebugMemory)
GUEST_IMPORT(NtAllocateVirtualMemory)
{
    using namespace guest;
    auto& mem = g_memory;
    const uint32_t baseAddressPtr = ctx.r3.u32;
    const uint32_t regionSizePtr = ctx.r4.u32;
    const uint32_t type = ctx.r5.u32;
    const uint32_t protect = ctx.r6.u32;
    // r7 DebugMemory: devkit-only memory pool; there is no separate pool here, so it is ignored.

    auto fail = [&](uint32_t status) { ctx.r3.u64 = status; };
    if (baseAddressPtr == 0 || regionSizePtr == 0)
        return fail(kernel::kStatusInvalidParameter);
    const uint32_t requestedBase = mem.Read32(baseAddressPtr);
    const int32_t rawSize = int32_t(mem.Read32(regionSizePtr));
    if (rawSize == 0)
        return fail(kernel::kStatusInvalidParameter);
    if (!(type & (kMemCommit | kMemReserve | kMemReset)))
        return fail(kernel::kStatusInvalidParameter);
    if ((type & kMemReset) && (type & ~kMemReset))
        return fail(kernel::kStatusInvalidParameter);
    if (type & kMemReset)
        Fatal("NtAllocateVirtualMemory: MEM_RESET is not implemented (base {:08X})", requestedBase);

    PageHeap* heap;
    if (requestedBase != 0)
    {
        heap = mem.LookupVirtualHeap(requestedBase);
        if (heap == nullptr)
            return fail(kernel::kStatusInvalidParameter);
    }
    else
    {
        heap = (type & kMemLargePages) ? &mem.virtual64k : &mem.virtual4k;
    }
    const uint32_t pageSize = heap->PageSize();
    const uint32_t regionBase = requestedBase - requestedBase % pageSize;
    const uint32_t size = RoundUp(rawSize < 0 ? uint32_t(-rawSize) : uint32_t(rawSize),
        regionBase ? pageSize : kReservationGranularity);

    uint32_t address = 0;
    bool wasCommitted = false;
    if (regionBase != 0)
    {
        // With a base address the owning heap decides the page size; MEM_LARGE_PAGES is not consulted.
        if (heap->AllocateFixed(regionBase, size, (type & kMemReserve) != 0, (type & kMemCommit) != 0, protect, &wasCommitted))
            address = regionBase;
    }
    else
    {
        address = heap->Allocate(size, kReservationGranularity, (type & kMemCommit) != 0, protect, (type & kMemTopDown) != 0);
        // A title thread's reservation is the title process's: released when it terminates.
        if (address != 0)
            if (const kernel::GuestThread* t = kernel::CurrentThread(); t && t->params.processType == kernel::kProcessTypeTitle)
                kernel::TrackTitleVirtual(address);
    }
    if (address == 0)
        return fail(kernel::kStatusNoMemory);

    // Newly committed host pages are already zero; re-committing committed pages leaves their contents, as on NT.
    mem.Write32(baseAddressPtr, address);
    mem.Write32(regionSizePtr, size);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS NtFreeVirtualMemory(PVOID* BaseAddress, PSIZE_T RegionSize, ULONG FreeType, BOOLEAN DebugMemory)
GUEST_IMPORT(NtFreeVirtualMemory)
{
    using namespace guest;
    auto& mem = g_memory;
    const uint32_t baseAddressPtr = ctx.r3.u32;
    const uint32_t regionSizePtr = ctx.r4.u32;
    const uint32_t freeType = ctx.r5.u32;

    uint32_t regionBase = mem.Read32(baseAddressPtr);
    uint32_t size = mem.Read32(regionSizePtr);
    if (regionBase == 0)
    {
        ctx.r3.u64 = kStatusMemoryNotAllocated;
        return;
    }
    PageHeap* heap = mem.LookupVirtualHeap(regionBase);
    if (heap == nullptr)
    {
        ctx.r3.u64 = kernel::kStatusInvalidParameter;
        return;
    }
    bool ok;
    if (freeType == kMemDecommit)
    {
        size = RoundUp(size, heap->PageSize());
        ok = heap->Decommit(regionBase, size);
    }
    else
    {
        size = heap->Release(regionBase);
        ok = size != 0;
        if (ok)
            kernel::UntrackTitleVirtual(regionBase);
    }
    if (!ok)
    {
        ctx.r3.u64 = kernel::kStatusUnsuccessful;
        return;
    }
    mem.Write32(baseAddressPtr, regionBase);
    mem.Write32(regionSizePtr, size);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS NtQueryVirtualMemory(PVOID BaseAddress, PMEMORY_BASIC_INFORMATION Info, BOOLEAN DebugMemory)
// MEMORY_BASIC_INFORMATION: BaseAddress, AllocationBase, AllocationProtect, RegionSize, State, Protect, Type.
GUEST_IMPORT(NtQueryVirtualMemory)
{
    using namespace guest;
    auto& mem = g_memory;
    const uint32_t address = ctx.r3.u32;
    const uint32_t info = ctx.r4.u32;
    PageHeap* heap = mem.LookupVirtualHeap(address);
    RegionInfo region;
    if (heap == nullptr || !heap->Query(address, region))
    {
        ctx.r3.u64 = kernel::kStatusInvalidParameter;
        return;
    }
    mem.Write32(info + 0x00, region.baseAddress);
    mem.Write32(info + 0x04, region.allocationBase);
    mem.Write32(info + 0x08, region.allocationProtect);
    mem.Write32(info + 0x0C, region.regionSize);
    mem.Write32(info + 0x10, region.state);
    mem.Write32(info + 0x14, region.protect);
    mem.Write32(info + 0x18, region.type);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// ULONG KeGetImagePageTableEntry(PVOID Address): the page table entry of a page of a loaded executable image, 0 for
// any other address. Bit 0 is PRESENT; 0x40000000 marks 4 KB pages (images below 0x90000000; above it the loader
// uses 64 KB pages); the low bits hold the page number, masked with 0x400FFFFF as the kernel does (Xenia Canary /
// Nexia360). dash.xex's Lua loader checks bit 0 for every page it reads bytecode from (code must come from signed
// image pages) and bug-checks 0xF7 otherwise. The page number is the page's index in its image window here, not a
// physical frame; no caller uses it.
GUEST_IMPORT(KeGetImagePageTableEntry)
{
    const uint32_t address = ctx.r3.u32;
    uint32_t entry = 0;
    for (const modules::LoadedModule* m : modules::All())
    {
        if (address >= m->xex.imageBase && address - m->xex.imageBase < m->xex.imageSize)
        {
            const bool smallPages = address < 0x90000000;
            entry = (address - (smallPages ? 0x80000000 : 0x90000000)) / (smallPages ? 0x1000 : 0x10000);
            entry = (entry | (smallPages ? 0x40000000 : 0) | 1) & 0x400FFFFF;
            break;
        }
    }
    ctx.r3.u64 = entry;
}

// NTSTATUS NtProtectVirtualMemory(PVOID* BaseAddress, PSIZE_T RegionSize, ULONG NewProtect, PULONG OldProtect,
//                                 ULONG Type): the pages covering the range; the base and size are rounded to the heap's pages.
GUEST_IMPORT(NtProtectVirtualMemory)
{
    auto& mem = guest::g_memory;
    const uint32_t basePtr = ctx.r3.u32, sizePtr = ctx.r4.u32, oldPtr = ctx.r6.u32;
    const uint32_t address = mem.Read32(basePtr), size = mem.Read32(sizePtr);
    guest::PageHeap* heap = mem.LookupVirtualHeap(address);
    const uint32_t page = heap ? heap->PageSize() : 0x1000;
    const uint32_t first = address & ~(page - 1);
    const uint32_t last = (address + std::max<uint32_t>(size, 1) + page - 1) & ~(page - 1);
    uint32_t old = 0;
    if (heap == nullptr || !heap->Protect(first, last - first, ctx.r5.u32, &old))
    {
        ctx.r3.u64 = 0xC0000045; // STATUS_INVALID_PAGE_PROTECTION / the range is not committed
        return;
    }
    mem.Write32(basePtr, first);
    mem.Write32(sizePtr, last - first);
    if (oldPtr)
        mem.Write32(oldPtr, old);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS NtAllocateEncryptedMemory(ULONG Type, SIZE_T Size, PVOID* BaseAddress): pages the console's hypervisor
// encrypts (0x8C000000-0x8FFFFFFF), committed, in 64 KiB units, at most 16 MiB at a time (xam.xex's startup asks for
// 128 KiB). Nothing reads them through another mapping here, so plain committed memory behaves the same.
GUEST_IMPORT(NtAllocateEncryptedMemory)
{
    auto& mem = guest::g_memory;
    const uint32_t size = (ctx.r4.u32 + 0xFFFF) & ~0xFFFFu, out = ctx.r5.u32;
    if (size == 0 || size > 0x1000000 || out == 0)
    {
        ctx.r3.u64 = kernel::kStatusInvalidParameter;
        return;
    }
    const uint32_t address = mem.encrypted.Allocate(size, 0x10000, true, guest::kPageReadWrite, false);
    if (address == 0)
    {
        ctx.r3.u64 = kernel::kStatusNoMemory;
        return;
    }
    mem.Write32(out, address);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS NtFreeEncryptedMemory(ULONG Type, PVOID BaseAddress): releases an allocation of the above.
GUEST_IMPORT(NtFreeEncryptedMemory)
{
    auto& mem = guest::g_memory;
    const uint32_t address = ctx.r4.u32;
    ctx.r3.u64 = mem.encrypted.Contains(address) && mem.encrypted.Release(address) ? kernel::kStatusSuccess
                                                                                    : kernel::kStatusInvalidParameter;
}
