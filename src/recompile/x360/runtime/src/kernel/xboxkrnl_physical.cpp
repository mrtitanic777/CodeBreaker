// xboxkrnl physical-memory exports (Mm*).
//
// Physical allocations come from one 512 MiB physical address space and are returned through the CPU window that
// matches their page size: 4 KiB pages at 0xE0000000 (offset by 0x1000), 64 KiB at 0xA0000000, 16 MiB at
// 0xC0000000 (see memory.h).

#include "kernel/kernel.h"

#include <map>
#include <mutex>

namespace
{
    using namespace guest;

    uint32_t PageSizeFor(uint32_t protect)
    {
        if (protect & kMemLargePages)
            return 0x10000;
        if (protect & kMem16MBPages)
            return 0x1000000;
        return 0x1000;
    }

    uint32_t RoundUp(uint32_t value, uint32_t multiple)
    {
        return uint32_t((uint64_t(value) + multiple - 1) / multiple * multiple);
    }

    // Each physical allocation's protection as MmAllocatePhysicalMemoryEx was given it (the page attributes the
    // kernel puts in its PTEs, MmQueryAddressProtect's answer), by physical start: { size, protect }.
    std::mutex g_protectMutex;
    std::map<uint32_t, std::pair<uint32_t, uint32_t>> g_physicalProtect;
}

// PVOID MmAllocatePhysicalMemoryEx(DWORD Flags, SIZE_T Size, DWORD Protect, ULONG_PTR MinimumAddress,
//                                  ULONG_PTR MaximumAddress, ULONG_PTR Alignment)
// Minimum/MaximumAddress bound the physical address. Allocation is top-down. Returns the window address, or 0.
GUEST_IMPORT(MmAllocatePhysicalMemoryEx)
{
    auto& mem = g_memory;
    const uint32_t size = ctx.r4.u32;
    const uint32_t protect = ctx.r5.u32;
    const uint32_t minAddress = ctx.r6.u32;
    const uint32_t maxAddress = ctx.r7.u32;
    const uint32_t alignment = ctx.r8.u32;
    if (!(protect & (kPageReadOnly | kPageReadWrite)) || size == 0)
    {
        ctx.r3.u64 = 0;
        return;
    }
    const uint32_t pageSize = PageSizeFor(protect);
    const uint32_t bytes = RoundUp(size, pageSize);
    const uint32_t physical = mem.physical.Allocate(bytes, RoundUp(std::max<uint32_t>(alignment, 1), pageSize),
        minAddress, maxAddress, true);
    if (physical == UINT32_MAX)
    {
        ctx.r3.u64 = 0;
        return;
    }
    mem.CommitPhysical(physical, bytes);
    memset(mem.Translate(kPhysicalWindow64k + physical), 0, bytes); // pages may be reused from a freed allocation
    {
        std::lock_guard lock(g_protectMutex);
        // Records of allocations freed with their title process (title_process.cpp) that this one reuses go.
        for (auto it = g_physicalProtect.lower_bound(physical); it != g_physicalProtect.end() && it->first < physical + bytes;)
            it = g_physicalProtect.erase(it);
        if (auto it = g_physicalProtect.lower_bound(physical); it != g_physicalProtect.begin() &&
            std::prev(it)->first + std::prev(it)->second.first > physical)
            g_physicalProtect.erase(std::prev(it));
        g_physicalProtect[physical] = { bytes, protect & ~(kMemLargePages | kMem16MBPages) };
    }
    ctx.r3.u64 = PhysicalToWindow(PhysicalWindowFor(pageSize), physical);
    // A title thread's allocation is the title process's: freed when it terminates.
    if (const kernel::GuestThread* t = kernel::CurrentThread(); t && t->params.processType == kernel::kProcessTypeTitle)
        kernel::TrackTitlePhysical(ctx.r3.u32);
}

// VOID MmFreePhysicalMemory(DWORD Type, PVOID BaseAddress)
GUEST_IMPORT(MmFreePhysicalMemory)
{
    const uint32_t address = ctx.r4.u32;
    if (!IsPhysicalWindow(address) || g_memory.physical.Free(WindowToPhysical(address)) == 0)
        Fatal("MmFreePhysicalMemory({:08X}): not the start of a physical allocation", address);
    {
        std::lock_guard lock(g_protectMutex);
        g_physicalProtect.erase(WindowToPhysical(address));
    }
    kernel::UntrackTitlePhysical(address);
}

// ULONG_PTR MmGetPhysicalAddress(PVOID Address)
GUEST_IMPORT(MmGetPhysicalAddress)
{
    const uint32_t address = ctx.r3.u32;
    if (!IsPhysicalWindow(address))
        Fatal("MmGetPhysicalAddress({:08X}): address is not in a physical memory window", address);
    ctx.r3.u64 = WindowToPhysical(address);
}

// SIZE_T MmQueryAllocationSize(PVOID BaseAddress)
GUEST_IMPORT(MmQueryAllocationSize)
{
    auto& mem = g_memory;
    const uint32_t address = ctx.r3.u32;
    uint32_t size = 0;
    if (IsPhysicalWindow(address))
    {
        size = mem.physical.SizeOf(WindowToPhysical(address));
    }
    else if (PageHeap* heap = mem.LookupVirtualHeap(address))
    {
        RegionInfo info;
        if (heap->Query(address, info) && info.allocationBase == address)
        {
            // Total size of the allocation: walk its regions.
            for (uint32_t a = address; heap->Query(a, info) && info.allocationBase == address; a += info.regionSize)
                size += info.regionSize;
        }
    }
    ctx.r3.u64 = size;
}

// DWORD MmQueryAddressProtect(PVOID VirtualAddress): protection of a committed page, 0 otherwise. A physical
// window's page has its allocation's (MmAllocatePhysicalMemoryEx's Protect: access and cache attributes, as the
// kernel puts them in the page's PTE; 3519's debug XAudio checks its XMA contexts' so).
GUEST_IMPORT(MmQueryAddressProtect)
{
    auto& mem = g_memory;
    const uint32_t address = ctx.r3.u32;
    if (IsPhysicalWindow(address))
    {
        const uint32_t physical = WindowToPhysical(address);
        std::lock_guard lock(g_protectMutex);
        auto it = g_physicalProtect.upper_bound(physical);
        if (it == g_physicalProtect.begin() || physical >= std::prev(it)->first + std::prev(it)->second.first)
            Fatal("MmQueryAddressProtect({:08X}): physical {:08X} is in no allocation of MmAllocatePhysicalMemoryEx "
                  "(the kernel's answer for other physical memory is not known)", address, physical);
        ctx.r3.u64 = std::prev(it)->second.second;
        return;
    }
    RegionInfo info;
    PageHeap* heap = mem.LookupVirtualHeap(address);
    ctx.r3.u64 = (heap && heap->Query(address, info) && info.state == kMemCommit) ? info.protect : 0;
}

// PVOID MmMapIoSpace(DWORD Type, ULONG_PTR PhysicalAddress, SIZE_T Size, DWORD Protect)
// Maps physical memory for CPU access. TU46 maps XMA contexts this way (XMAHALAllocateContexts: type 2, 0x40 bytes,
// PAGE_READWRITE | PAGE_WRITECOMBINE). For RAM, any physical window aliases the same memory; the 64 KiB window is
// returned. Device (MMIO) ranges and other types stop with a message.
GUEST_IMPORT(MmMapIoSpace)
{
    const uint32_t type = ctx.r3.u32, physical = ctx.r4.u32, size = ctx.r5.u32;
    if (type != 2 || physical >= kPhysicalSize || size > kPhysicalSize - physical)
        Fatal("MmMapIoSpace(type {}, physical {:08X}, size {:X}, protect {:X}) is not implemented", type, physical, size,
            ctx.r6.u32);
    ctx.r3.u64 = PhysicalToWindow(kPhysicalWindow64k, physical);
}

// NTSTATUS MmQueryStatistics(PMM_STATISTICS Statistics): Length (first dword) must be 0x68 or 0x70 (17559's dash
// passes 0x70; the extra 8 bytes are left zero). Counts in 4 KiB pages: total 512 MiB, the kernel's (the runtime's
// system heap and the reserved top 64 KiB), and a title section from the runtime's allocators; the system process
// section stays zero (no separate system heaps).
GUEST_IMPORT(MmQueryStatistics)
{
    auto& mem = g_memory;
    const uint32_t stats = ctx.r3.u32;
    const uint32_t length = stats ? mem.Read32(stats) : 0;
    if (length != 0x68 && length != 0x70)
        Fatal("MmQueryStatistics({:08X}): length {:X} (0x68 and 0x70 are known)", stats, length);
    constexpr uint32_t kTotalPages = kPhysicalSize / 0x1000, kTopReservedPages = 0x10;
    uint64_t systemReserved, systemCommitted, reserved = 0, committed = 0;
    mem.system.Usage(systemReserved, systemCommitted);
    for (PageHeap* heap : { &mem.virtual4k, &mem.virtual64k, &mem.stacks })
    {
        uint64_t r, c;
        heap->Usage(r, c);
        reserved += r;
        committed += c;
    }
    const uint32_t kernelPages = uint32_t(systemCommitted / 0x1000) + kTopReservedPages;
    const uint32_t physicalPages = mem.physical.UsedPages() - kTopReservedPages;
    const uint32_t heapPages = uint32_t(committed / 0x1000);
    const uint32_t used = kernelPages + physicalPages + heapPages;

    std::memset(mem.Translate(stats), 0, length);
    mem.Write32(stats + 0x00, length);
    mem.Write32(stats + 0x04, kTotalPages);
    mem.Write32(stats + 0x08, kernelPages);
    mem.Write32(stats + 0x0C, used < kTotalPages ? kTotalPages - used : 0); // title AvailablePages
    mem.Write32(stats + 0x10, mem.stacks.End() - mem.virtual4k.Start());   // TotalVirtualMemoryBytes
    mem.Write32(stats + 0x14, uint32_t(reserved));                         // ReservedVirtualMemoryBytes
    mem.Write32(stats + 0x18, physicalPages);                              // PhysicalPages
    mem.Write32(stats + 0x28, heapPages);                                  // HeapPages
    mem.Write32(stats + 0x64, kTotalPages - 1);                            // HighestPhysicalPage
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// PVOID MmAllocatePhysicalMemory(DWORD Flags, SIZE_T Size, DWORD Protect): MmAllocatePhysicalMemoryEx over all of
// physical memory, page alignment.
GUEST_IMPORT(MmAllocatePhysicalMemory)
{
    ctx.r6.u64 = 0;
    ctx.r7.u64 = 0xFFFFFFFF;
    ctx.r8.u64 = 0;
    __imp__MmAllocatePhysicalMemoryEx(ctx, base);
}
