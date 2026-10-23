#include "memory.h"

#include "log.h"
#include "pool.h"
#include "ppc_lookup.h"

#include <windows.h>

#include <algorithm>

namespace guest
{
    Memory g_memory;

    namespace
    {
        constexpr uint32_t kNullGuardSize = 0x10000; // the first 64 KiB are never allocatable

        DWORD ToHostProtect(uint32_t protect)
        {
            DWORD host;
            if (protect & (kPageReadWrite | kPageExecuteReadWrite))
                host = PAGE_READWRITE;
            else if (protect & (kPageReadOnly | kPageExecuteRead | kPageExecute))
                host = PAGE_READONLY;
            else
                host = PAGE_NOACCESS;
            if ((protect & kPageGuard) && host != PAGE_NOACCESS)
                host |= PAGE_GUARD;
            return host;
        }
    }

    PageHeap::PageHeap(uint32_t start, uint32_t end, uint32_t pageSize)
        : start_(start), end_(end), pageSize_(pageSize), pages_((uint64_t(end) - start) / pageSize)
    {
        // Keep the null region permanently reserved so no allocation can return address 0.
        if (start_ == 0)
        {
            for (size_t i = 0; i < kNullGuardSize / pageSize_; i++)
            {
                pages_[i].state = State::Reserved;
                pages_[i].allocationBase = 0xFFFFFFFF;
            }
        }
    }

    void PageHeap::CommitPages(size_t first, size_t count, uint32_t protect)
    {
        size_t runStart = first;
        for (size_t i = first; i <= first + count; i++)
        {
            // Commit maximal runs of not-yet-committed pages; already-committed pages keep their contents.
            if (i == first + count || pages_[i].state == State::Committed)
            {
                if (i > runStart)
                    g_memory.HostCommit(PageAddress(runStart), uint32_t((i - runStart) * pageSize_), protect);
                runStart = i + 1;
            }
        }
        g_memory.HostProtect(PageAddress(first), uint32_t(count * pageSize_), protect);
        for (size_t i = first; i < first + count; i++)
        {
            pages_[i].state = State::Committed;
            pages_[i].protect = protect;
        }
    }

    uint32_t PageHeap::Allocate(uint32_t size, uint32_t alignment, bool commit, uint32_t protect, bool topDown)
    {
        if (size == 0)
            return 0;
        alignment = std::max(alignment, pageSize_);
        const size_t count = (uint64_t(size) + pageSize_ - 1) / pageSize_;
        const size_t step = alignment / pageSize_;
        const size_t total = pages_.size();
        if (count > total)
            return 0;
        std::lock_guard lock(mutex_);

        auto fits = [&](size_t first)
            {
                for (size_t p = first; p < first + count; p++)
                    if (pages_[p].state != State::Free)
                        return false;
                return true;
            };

        size_t found = SIZE_MAX;
        if (topDown)
        {
            for (size_t first = (total - count) / step * step;; first -= step)
            {
                if (fits(first))
                {
                    found = first;
                    break;
                }
                if (first < step)
                    break;
            }
        }
        else
        {
            for (size_t first = 0; first + count <= total; first += step)
            {
                if (fits(first))
                {
                    found = first;
                    break;
                }
            }
        }
        if (found == SIZE_MAX)
            return 0;

        const uint32_t address = PageAddress(found);
        for (size_t p = found; p < found + count; p++)
        {
            pages_[p] = {};
            pages_[p].state = State::Reserved;
            pages_[p].allocationBase = address;
            pages_[p].allocationProtect = protect;
            pages_[p].protect = protect;
        }
        pages_[found].allocationPages = uint32_t(count);
        if (commit)
            CommitPages(found, count, protect);
        return address;
    }

    bool PageHeap::AllocateFixed(uint32_t address, uint32_t size, bool reserve, bool commit, uint32_t protect, bool* wasCommitted)
    {
        if (!Contains(address) || size == 0 || uint64_t(address) + size > end_ || (address - start_) % pageSize_)
            return false;
        const size_t first = PageIndex(address);
        const size_t count = (uint64_t(size) + pageSize_ - 1) / pageSize_;
        std::lock_guard lock(mutex_);

        bool allFree = true, allInOneAllocation = true, allCommitted = true;
        const uint32_t allocation = pages_[first].allocationBase;
        for (size_t p = first; p < first + count; p++)
        {
            allFree &= pages_[p].state == State::Free;
            allInOneAllocation &= pages_[p].state != State::Free && pages_[p].allocationBase == allocation && allocation != 0xFFFFFFFF;
            allCommitted &= pages_[p].state == State::Committed;
        }
        if (wasCommitted)
            *wasCommitted = allCommitted;

        if (allFree)
        {
            if (!reserve)
                return false; // committing never-reserved memory
            for (size_t p = first; p < first + count; p++)
            {
                pages_[p] = {};
                pages_[p].state = State::Reserved;
                pages_[p].allocationBase = address;
                pages_[p].allocationProtect = protect;
                pages_[p].protect = protect;
            }
            pages_[first].allocationPages = uint32_t(count);
        }
        else if (!allInOneAllocation)
        {
            return false;
        }
        if (commit)
            CommitPages(first, count, protect);
        return true;
    }

    bool PageHeap::Decommit(uint32_t address, uint32_t size)
    {
        if (!Contains(address) || size == 0 || uint64_t(address) + size > end_)
            return false;
        const size_t first = PageIndex(address);
        const size_t count = (uint64_t(size) + pageSize_ - 1) / pageSize_;
        std::lock_guard lock(mutex_);
        for (size_t p = first; p < first + count; p++)
            if (pages_[p].state == State::Free || pages_[p].allocationBase == 0xFFFFFFFF)
                return false;
        g_memory.HostDecommit(PageAddress(first), uint32_t(count * pageSize_));
        for (size_t p = first; p < first + count; p++)
            pages_[p].state = State::Reserved;
        return true;
    }

    uint32_t PageHeap::Release(uint32_t address)
    {
        if (!Contains(address))
            return 0;
        std::lock_guard lock(mutex_);
        const size_t first = PageIndex(address);
        if (pages_[first].allocationBase != address || pages_[first].allocationPages == 0)
            return 0;
        const size_t count = pages_[first].allocationPages;
        g_memory.HostDecommit(address, uint32_t(count * pageSize_));
        for (size_t p = first; p < first + count; p++)
            pages_[p] = {};
        return uint32_t(count * pageSize_);
    }

    bool PageHeap::Protect(uint32_t address, uint32_t size, uint32_t protect, uint32_t* oldProtect)
    {
        if (!Contains(address) || size == 0 || uint64_t(address) + size > end_)
            return false;
        const size_t first = PageIndex(address);
        const size_t count = (uint64_t(size) + pageSize_ - 1) / pageSize_;
        std::lock_guard lock(mutex_);
        for (size_t p = first; p < first + count; p++)
            if (pages_[p].state != State::Committed)
                return false;
        if (oldProtect)
            *oldProtect = pages_[first].protect;
        g_memory.HostProtect(PageAddress(first), uint32_t(count * pageSize_), protect);
        for (size_t p = first; p < first + count; p++)
            pages_[p].protect = protect;
        return true;
    }

    void PageHeap::Usage(uint64_t& reservedBytes, uint64_t& committedBytes)
    {
        std::lock_guard lock(mutex_);
        reservedBytes = committedBytes = 0;
        for (const Page& page : pages_)
        {
            reservedBytes += page.state != State::Free ? pageSize_ : 0;
            committedBytes += page.state == State::Committed ? pageSize_ : 0;
        }
    }

    bool PageHeap::Query(uint32_t address, RegionInfo& info)
    {
        if (!Contains(address))
            return false;
        std::lock_guard lock(mutex_);
        const size_t index = PageIndex(address);
        const Page& page = pages_[index];
        size_t last = index;
        auto same = [&](const Page& other)
            {
                return other.state == page.state && other.allocationBase == page.allocationBase &&
                    (page.state == State::Free || other.protect == page.protect);
            };
        while (last + 1 < pages_.size() && same(pages_[last + 1]))
            last++;
        info = {};
        info.baseAddress = PageAddress(index);
        info.regionSize = uint32_t((last - index + 1) * pageSize_);
        if (page.state == State::Free)
        {
            info.state = kMemFree;
            info.protect = kPageNoAccess;
            return true;
        }
        info.allocationBase = page.allocationBase;
        info.allocationProtect = page.allocationProtect;
        info.state = page.state == State::Committed ? kMemCommit : kMemReserve;
        info.protect = page.state == State::Committed ? page.protect : 0;
        info.type = kMemPrivate;
        return true;
    }

    PhysicalAllocator::PhysicalAllocator()
        : used_(kPhysicalSize / kPage, false), allocationPages_(kPhysicalSize / kPage, 0)
    {
        // The top 64 KiB of physical memory is never handed out (reserved by the system on a console).
        for (size_t p = (kPhysicalSize - 0x10000) / kPage; p < used_.size(); p++)
            used_[p] = true;
    }

    uint32_t PhysicalAllocator::Allocate(uint32_t size, uint32_t alignment, uint32_t minAddress, uint32_t maxAddress, bool topDown)
    {
        const size_t count = (uint64_t(size) + kPage - 1) / kPage;
        const size_t step = std::max<uint32_t>(alignment, kPage) / kPage;
        const size_t low = (uint64_t(std::min(minAddress, kPhysicalSize - 1)) + kPage - 1) / kPage;
        const size_t high = std::min<uint64_t>(uint64_t(maxAddress) + 1, kPhysicalSize) / kPage; // exclusive
        if (count == 0 || high < low + count)
            return UINT32_MAX;
        std::lock_guard lock(mutex_);

        auto fits = [&](size_t first)
            {
                for (size_t p = first; p < first + count; p++)
                    if (used_[p])
                        return false;
                return true;
            };
        const size_t firstAligned = (low + step - 1) / step * step;
        size_t found = SIZE_MAX;
        if (topDown)
        {
            for (size_t first = (high - count) / step * step; first >= firstAligned; first -= step)
            {
                if (fits(first))
                {
                    found = first;
                    break;
                }
                if (first < step)
                    break;
            }
        }
        else
        {
            for (size_t first = firstAligned; first + count <= high; first += step)
            {
                if (fits(first))
                {
                    found = first;
                    break;
                }
            }
        }
        if (found == SIZE_MAX)
            return UINT32_MAX;
        for (size_t p = found; p < found + count; p++)
            used_[p] = true;
        allocationPages_[found] = uint32_t(count);
        return uint32_t(found * kPage);
    }

    uint32_t PhysicalAllocator::Free(uint32_t physical)
    {
        std::lock_guard lock(mutex_);
        const size_t first = physical / kPage;
        if (physical % kPage || first >= allocationPages_.size() || allocationPages_[first] == 0)
            return 0;
        const size_t count = allocationPages_[first];
        for (size_t p = first; p < first + count; p++)
            used_[p] = false;
        allocationPages_[first] = 0;
        return uint32_t(count * kPage);
    }

    uint32_t PhysicalAllocator::UsedPages()
    {
        std::lock_guard lock(mutex_);
        return uint32_t(std::count(used_.begin(), used_.end(), true));
    }

    uint32_t PhysicalAllocator::SizeOf(uint32_t physical)
    {
        std::lock_guard lock(mutex_);
        const size_t first = physical / kPage;
        return first < allocationPages_.size() ? allocationPages_[first] * kPage : 0;
    }

    void Memory::Reserve()
    {
        // Windows 10 placeholder APIs (looked up at run time; present since version 1803).
        using VirtualAlloc2Fn = PVOID(WINAPI*)(HANDLE, PVOID, SIZE_T, ULONG, ULONG, MEM_EXTENDED_PARAMETER*, ULONG);
        using MapViewOfFile3Fn = PVOID(WINAPI*)(HANDLE, HANDLE, PVOID, ULONG64, SIZE_T, ULONG, ULONG, MEM_EXTENDED_PARAMETER*, ULONG);
        HMODULE kernelbase = GetModuleHandleW(L"kernelbase.dll");
        auto virtualAlloc2 = reinterpret_cast<VirtualAlloc2Fn>(GetProcAddress(kernelbase, "VirtualAlloc2"));
        auto mapViewOfFile3 = reinterpret_cast<MapViewOfFile3Fn>(GetProcAddress(kernelbase, "MapViewOfFile3"));
        if (!virtualAlloc2 || !mapViewOfFile3)
            Fatal("Windows 10 version 1803 or newer is required (VirtualAlloc2/MapViewOfFile3 not found)");
        const HANDLE process = GetCurrentProcess();

        // One placeholder for the whole 4 GiB plus the indirect-call table after it (ppc_lookup.h), split into the table,
        // [0, A0000000) and the three 512 MiB physical windows.
        base = static_cast<uint8_t*>(virtualAlloc2(process, nullptr, PPC_FUNC_TABLE_OFFSET + PPC_FUNC_TABLE_SIZE,
            MEM_RESERVE | MEM_RESERVE_PLACEHOLDER, PAGE_NOACCESS, nullptr, 0));
        if (base == nullptr)
            Fatal("Unable to reserve the 4 GiB guest address space and the function table (error {})", GetLastError());
        static_assert(PPC_FUNC_TABLE_OFFSET == kAddressSpaceSize + kHostTailSize);
        if (!VirtualFree(base, SIZE_T(PPC_FUNC_TABLE_OFFSET), MEM_RELEASE | MEM_PRESERVE_PLACEHOLDER) ||
            !virtualAlloc2(process, base + PPC_FUNC_TABLE_OFFSET, SIZE_T(PPC_FUNC_TABLE_SIZE),
                MEM_RESERVE | MEM_REPLACE_PLACEHOLDER, PAGE_NOACCESS, nullptr, 0))
            Fatal("Unable to reserve the indirect-call table (error {})", GetLastError());
        for (uint64_t split : { uint64_t(kPhysicalWindow64k), uint64_t(kPhysicalWindow16m), uint64_t(kPhysicalWindow4k) })
        {
            const uint64_t start = split == kPhysicalWindow64k ? 0 : split - kPhysicalSize;
            if (!VirtualFree(base + start, SIZE_T(split - start), MEM_RELEASE | MEM_PRESERVE_PLACEHOLDER))
                Fatal("Unable to split the guest address-space placeholder at {:08X} (error {})", split, GetLastError());
        }
        if (!virtualAlloc2(process, base, kPhysicalWindow64k, MEM_RESERVE | MEM_REPLACE_PLACEHOLDER, PAGE_NOACCESS, nullptr, 0))
            Fatal("Unable to reserve the guest virtual range (error {})", GetLastError());
        // The 64 KiB past 4 GiB that the offset 4 KiB window reaches (beyond physical memory: never mapped).
        if (!VirtualFree(base + kPhysicalWindow4k, kPhysicalSize, MEM_RELEASE | MEM_PRESERVE_PLACEHOLDER) ||
            !virtualAlloc2(process, base + kAddressSpaceSize, kHostTailSize, MEM_RESERVE | MEM_REPLACE_PLACEHOLDER,
                PAGE_NOACCESS, nullptr, 0))
            Fatal("Unable to reserve the end of the guest address space (error {})", GetLastError());

        // Physical memory: one pagefile-backed section, committed as a whole (SEC_COMMIT; host RAM is only used for
        // pages that are touched). Fully committed, its view can also be opened by Direct3D 12 as a heap
        // (OpenExistingHeapFromAddress rejects SEC_RESERVE views), so the GPU reads and writes guest memory directly.
        physicalSection_ = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE | SEC_COMMIT, 0, kPhysicalSize, nullptr);
        if (physicalSection_ == nullptr)
            Fatal("Unable to create the physical memory section (error {})", GetLastError());
        for (uint32_t window : { kPhysicalWindow64k, kPhysicalWindow16m, kPhysicalWindow4k })
        {
            if (!mapViewOfFile3(physicalSection_, process, base + window, 0, kPhysicalSize, MEM_REPLACE_PLACEHOLDER,
                    PAGE_READWRITE, nullptr, 0))
                Fatal("Unable to map the physical window at {:08X} (error {})", window, GetLastError());
        }
        Log("Guest memory reserved at host {:p} (physical windows at A0000000/C0000000/E0000000)", static_cast<void*>(base));
    }

    void Memory::CommitPhysical(uint32_t physical, uint32_t size)
    {
        // The section is committed as a whole; allocations only reserve address ranges in the allocator.
        (void)physical;
        (void)size;
    }

    void Memory::HostCommit(uint32_t address, uint32_t size, uint32_t protect)
    {
        if (size == 0)
            return;
        if (VirtualAlloc(base + address, size, MEM_COMMIT, ToHostProtect(protect)) == nullptr)
            Fatal("Unable to commit guest memory {:08X}+{:X} (error {})", address, size, GetLastError());
    }

    void Memory::HostDecommit(uint32_t address, uint32_t size)
    {
        if (size != 0)
            VirtualFree(base + address, size, MEM_DECOMMIT);
    }

    void Memory::HostProtect(uint32_t address, uint32_t size, uint32_t protect)
    {
        DWORD old;
        if (size != 0 && !VirtualProtect(base + address, size, ToHostProtect(protect), &old))
            Fatal("Unable to protect guest memory {:08X}+{:X} (error {})", address, size, GetLastError());
    }

    void Memory::CommitFunctionTable(uint32_t guestStart, uint32_t guestSize)
    {
        uint8_t* first = base + PPC_FUNC_TABLE_OFFSET + uint64_t(guestStart) * 2;
        if (VirtualAlloc(first, SIZE_T(guestSize) * 2, MEM_COMMIT, PAGE_READWRITE) == nullptr)
            Fatal("Unable to commit the indirect-call table for {:08X}+{:X} (error {})", guestStart, guestSize, GetLastError());
    }

    bool Memory::FunctionTableAddress(const void* host, uint32_t& guest) const
    {
        const auto p = static_cast<const uint8_t*>(host);
        const uint8_t* table = base + PPC_FUNC_TABLE_OFFSET;
        if (p < table || p >= table + PPC_FUNC_TABLE_SIZE)
            return false;
        guest = uint32_t((p - table) / 2) & ~3u;
        return true;
    }

    bool Memory::IsGuestPointer(const void* host) const
    {
        auto p = static_cast<const uint8_t*>(host);
        return p >= base && p < base + kAddressSpaceSize + kHostTailSize;
    }

    PageHeap* Memory::LookupVirtualHeap(uint32_t address)
    {
        if (virtual4k.Contains(address))
            return &virtual4k;
        if (virtual64k.Contains(address))
            return &virtual64k;
        return nullptr;
    }

    uint32_t Memory::SystemAlloc(uint32_t size, uint32_t alignment)
    {
        // Small blocks (kernel objects, strings) come from the kernel pool, 16 bytes at a time, as on the console: as
        // whole pages they took 4 KiB each, and the pages the pool leaves held ~750 of them (Doritos Crash Course
        // created more at its start). Larger or more strictly aligned blocks are pages.
        if (size <= 0x800 && alignment <= 16)
            if (const uint32_t pooled = KernelPool().Allocate(size))
                return pooled;
        uint32_t address = system.Allocate(size, alignment, true, kPageReadWrite, false);
        if (address == 0)
            Fatal("System heap exhausted allocating {:X} bytes", size);
        return address;
    }

    void Memory::SystemFree(uint32_t address)
    {
        if (KernelPool().Free(address))
            return;
        system.Release(address);
    }
}
