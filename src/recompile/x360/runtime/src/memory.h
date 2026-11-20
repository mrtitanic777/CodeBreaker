#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <utility>
#include <vector>

// Guest (Xbox 360) address space: a 4 GiB host reservation indexed by 32-bit guest addresses. Host pages are
// committed only when the guest commits them, so a stray guest access faults (and is reported) instead of silently
// reading zeros.
//
// Layout (mirrors the console's virtual memory map):
//   0x00000000-0x3FFFFFFF  virtual, 4 KiB pages    (NtAllocateVirtualMemory without MEM_LARGE_PAGES)
//   0x40000000-0x6FFFFFFF  virtual, 64 KiB pages   (NtAllocateVirtualMemory with MEM_LARGE_PAGES)
//   0x70000000-0x7EFFFFFF  thread stacks
//   0x80000000-0x81FFFFFF  kernel: fixed kernel variables + system heap (kernel objects, PCRs, TLS)
//   0x82000000-...         title XEX image (system modules: dash 0x92000000, bootanim 0x98000000, plugins 0x9A..)
// The indirect-call table of every recompiled module lies after the 4 GiB (ppc_lookup.h).
//   0xA0000000-0xFFFFFFFF  physical memory views (not yet implemented)
namespace guest
{
    constexpr uint64_t kAddressSpaceSize = 0x100000000ull;
    constexpr uint64_t kHostTailSize = 0x10000; // host reservation past 4 GiB (see HostOffset)

    // Xbox allocation-type and protection flags (as passed by titles).
    constexpr uint32_t kMemCommit = 0x00001000;
    constexpr uint32_t kMemReserve = 0x00002000;
    constexpr uint32_t kMemDecommit = 0x00004000;
    constexpr uint32_t kMemRelease = 0x00008000;
    constexpr uint32_t kMemFree = 0x00010000;
    constexpr uint32_t kMemPrivate = 0x00020000;
    constexpr uint32_t kMemReset = 0x00080000;
    constexpr uint32_t kMemTopDown = 0x00100000;
    constexpr uint32_t kMemNoZero = 0x00800000;
    constexpr uint32_t kMemLargePages = 0x20000000;
    constexpr uint32_t kMem16MBPages = 0x80000000;

    constexpr uint32_t kPageNoAccess = 0x01;
    constexpr uint32_t kPageReadOnly = 0x02;
    constexpr uint32_t kPageReadWrite = 0x04;
    constexpr uint32_t kPageExecute = 0x10;
    constexpr uint32_t kPageExecuteRead = 0x20;
    constexpr uint32_t kPageExecuteReadWrite = 0x40;
    constexpr uint32_t kPageGuard = 0x100;
    constexpr uint32_t kPageNoCache = 0x200;
    constexpr uint32_t kPageWriteCombine = 0x400;

    struct RegionInfo
    {
        uint32_t baseAddress = 0;       // start of the run of pages with identical state/protection
        uint32_t allocationBase = 0;
        uint32_t allocationProtect = 0;
        uint32_t regionSize = 0;
        uint32_t state = kMemFree;      // kMemFree, kMemReserve or kMemCommit
        uint32_t protect = 0;
        uint32_t type = 0;              // kMemPrivate for allocated regions
    };

    // A range of guest pages with Windows-NT-like reserve/commit/release semantics.
    class PageHeap
    {
    public:
        PageHeap(uint32_t start, uint32_t end, uint32_t pageSize);

        uint32_t Start() const { return start_; }
        uint32_t End() const { return end_; }
        uint32_t PageSize() const { return pageSize_; }
        bool Contains(uint32_t address) const { return address >= start_ && address < end_; }

        // Reserves (and optionally commits) a new allocation anywhere in the heap, aligned to max(alignment, page size);
        // returns 0 if none fits.
        uint32_t Allocate(uint32_t size, uint32_t alignment, bool commit, uint32_t protect, bool topDown);
        // Reserves and/or commits [address, address + size). Reserving needs the pages free; committing alone needs
        // them reserved by a single allocation. wasCommitted reports whether every page was already committed.
        bool AllocateFixed(uint32_t address, uint32_t size, bool reserve, bool commit, uint32_t protect, bool* wasCommitted = nullptr);
        bool Decommit(uint32_t address, uint32_t size);
        // Releases a whole allocation; address must be its base. Returns the released size (0 on failure).
        uint32_t Release(uint32_t address);
        bool Protect(uint32_t address, uint32_t size, uint32_t protect, uint32_t* oldProtect);
        bool Query(uint32_t address, RegionInfo& info);
        // Bytes reserved (including committed) and committed in the heap.
        void Usage(uint64_t& reservedBytes, uint64_t& committedBytes);

    private:
        enum class State : uint8_t { Free, Reserved, Committed };
        struct Page
        {
            uint32_t allocationBase = 0;
            uint32_t allocationPages = 0; // only meaningful on the allocation's first page
            uint32_t allocationProtect = 0;
            uint32_t protect = 0;
            State state = State::Free;
        };

        size_t PageIndex(uint32_t address) const { return (address - start_) / pageSize_; }
        uint32_t PageAddress(size_t index) const { return start_ + uint32_t(index * pageSize_); }
        void CommitPages(size_t first, size_t count, uint32_t protect);

        uint32_t start_;
        uint32_t end_;
        uint32_t pageSize_;
        std::vector<Page> pages_;
        std::mutex mutex_;
    };

    // The console's 512 MiB of physical memory, seen by the CPU through three mirrored windows:
    //   0xA0000000 (64 KiB pages), 0xC0000000 (16 MiB pages), 0xE0000000 (4 KiB pages); physical = virtual & 0x1FFFFFFF, plus 0x1000 in the last.
    // One shared section backs all three windows, so a write through any window is visible through the others (and
    // to GPU emulation at the physical address). The 0xE0000000 window is offset by 0x1000 (see HostOffset below).
    constexpr uint32_t kPhysicalSize = 0x20000000;
    constexpr uint32_t kPhysicalWindow64k = 0xA0000000;
    constexpr uint32_t kPhysicalWindow16m = 0xC0000000;
    constexpr uint32_t kPhysicalWindow4k = 0xE0000000;

    inline bool IsPhysicalWindow(uint32_t address) { return address >= kPhysicalWindow64k; }

    // The console's 4 KiB-page window is offset: virtual 0xE0000000 + X is physical X + 0x1000 (D3D's own
    // virtual-to-physical conversion adds it). Host views start on 64 KiB boundaries, so the window's view is mapped
    // without the offset and every CPU access at or above 0xE0000000 adds 0x1000 (as the recompiled code's PPC_HOST
    // does). The host reservation extends 64 KiB past 4 GiB for the window's last page.
    constexpr uint32_t kPhysicalWindow4kOffset = 0x1000;
    inline uint32_t HostOffset(uint32_t address) { return address >= kPhysicalWindow4k ? kPhysicalWindow4kOffset : 0; }
    // Physical address behind an address in one of the physical windows.
    inline uint32_t WindowToPhysical(uint32_t address) { return (address & (kPhysicalSize - 1)) + HostOffset(address); }
    // Address of physical memory through a window.
    inline uint32_t PhysicalToWindow(uint32_t window, uint32_t physical)
    {
        return window + physical - (window == kPhysicalWindow4k ? kPhysicalWindow4kOffset : 0);
    }
    inline uint32_t PhysicalWindowFor(uint32_t pageSize)
    {
        return pageSize >= 0x1000000 ? kPhysicalWindow16m : pageSize >= 0x10000 ? kPhysicalWindow64k : kPhysicalWindow4k;
    }

    // Allocator for physical address ranges (4 KiB granularity).
    class PhysicalAllocator
    {
    public:
        PhysicalAllocator();
        // Physical address of a new range inside [minAddress, maxAddress], or UINT32_MAX.
        uint32_t Allocate(uint32_t size, uint32_t alignment, uint32_t minAddress, uint32_t maxAddress, bool topDown);
        // Releases the allocation starting at the physical address; returns its size (0 if none).
        uint32_t Free(uint32_t physical);
        uint32_t SizeOf(uint32_t physical);
        // Pages handed out (including the reserved top 64 KiB).
        uint32_t UsedPages();

    private:
        static constexpr uint32_t kPage = 0x1000;
        std::vector<bool> used_;
        std::vector<uint32_t> allocationPages_; // pages, stored on an allocation's first page
        std::mutex mutex_;
    };

    struct Memory
    {
        uint8_t* base = nullptr;

        PageHeap virtual4k{ 0x00000000, 0x40000000, 0x1000 };
        PageHeap virtual64k{ 0x40000000, 0x70000000, 0x10000 };
        PageHeap stacks{ 0x70000000, 0x7F000000, 0x1000 };
        // Up to xam.xex's image (0x815F0000-0x81C00000, where the console's kernel loads it: CB_RECOMP_XAM).
        PageHeap system{ 0x80300000, 0x815F0000, 0x1000 };
        // NtAllocateEncryptedMemory's range (the console's hypervisor-encrypted pages, 64 KiB each; xam.xex).
        PageHeap encrypted{ 0x8C000000, 0x90000000, 0x10000 };
        PhysicalAllocator physical;
        void* physicalSection_ = nullptr;

        void Reserve();
        // Commits backing pages for a physical range (visible through every physical window).
        void CommitPhysical(uint32_t physical, uint32_t size);
        // Host-level commit/decommit of [address, address + size) (rounded out to host pages).
        void HostCommit(uint32_t address, uint32_t size, uint32_t protect = kPageReadWrite);
        void HostDecommit(uint32_t address, uint32_t size);
        void HostProtect(uint32_t address, uint32_t size, uint32_t protect);
        bool IsGuestPointer(const void* host) const;
        // A fourth view of physical memory outside the guest's address space (physical 0 at its start), mapped on
        // first use: never write-protected by the write tracking, so a GPU heap can be opened over it.
        void* HostView();
        // Commits the indirect-call table slots for guest code addresses [guestStart, guestStart + guestSize) (zeroed).
        void CommitFunctionTable(uint32_t guestStart, uint32_t guestSize);
        // If `host` lies in the indirect-call table, the guest code address of its slot.
        bool FunctionTableAddress(const void* host, uint32_t& guest) const;

        // Virtual heap that owns the address, or nullptr.
        PageHeap* LookupVirtualHeap(uint32_t address);

        uint8_t* Translate(uint32_t address) const { return base + address + HostOffset(address); }
        uint32_t ToGuest(const void* host) const
        {
            const uint64_t offset = uint64_t(static_cast<const uint8_t*>(host) - base);
            return uint32_t(offset >= uint64_t(kPhysicalWindow4k) + kPhysicalWindow4kOffset ? offset - kPhysicalWindow4kOffset : offset);
        }

        uint8_t Read8(uint32_t a) const { return *Translate(a); }
        uint16_t Read16(uint32_t a) const { uint16_t v; memcpy(&v, Translate(a), 2); return __builtin_bswap16(v); }
        uint32_t Read32(uint32_t a) const { uint32_t v; memcpy(&v, Translate(a), 4); return __builtin_bswap32(v); }
        uint64_t Read64(uint32_t a) const { uint64_t v; memcpy(&v, Translate(a), 8); return __builtin_bswap64(v); }
        void Write8(uint32_t a, uint8_t v) { *Translate(a) = v; }
        void Write16(uint32_t a, uint16_t v) { v = __builtin_bswap16(v); memcpy(Translate(a), &v, 2); }
        void Write32(uint32_t a, uint32_t v) { v = __builtin_bswap32(v); memcpy(Translate(a), &v, 4); }
        void Write64(uint32_t a, uint64_t v) { v = __builtin_bswap64(v); memcpy(Translate(a), &v, 8); }

        // Atomics on big-endian guest words. They compare-and-swap the raw stored value, exactly like the recompiled
        // lwarx/stwcx. sequences, so guest and runtime atomics on the same word interoperate.
        bool CompareExchange32(uint32_t a, uint32_t expected, uint32_t desired)
        {
            return __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(Translate(a)),
                __builtin_bswap32(expected), __builtin_bswap32(desired));
        }
        // Returns the new value.
        uint32_t AtomicAdd32(uint32_t a, int32_t delta)
        {
            for (;;)
            {
                const uint32_t old = __builtin_bswap32(__atomic_load_n(reinterpret_cast<uint32_t*>(Translate(a)), __ATOMIC_SEQ_CST));
                if (CompareExchange32(a, old, old + uint32_t(delta)))
                    return old + uint32_t(delta);
            }
        }

        // Allocations for the runtime's own kernel structures (zeroed, committed read/write).
        uint32_t SystemAlloc(uint32_t size, uint32_t alignment = 16);
        void SystemFree(uint32_t address);
    };

    extern Memory g_memory;

    // Write tracking for a copy of physical memory kept elsewhere (the GPU's, gpu.h LocalMemory), 4 KiB pages. A page
    // is stale (the copy is behind; the CPU writes it freely) or watched (the copy agrees; read-only in all three
    // windows, so the CPU's first write faults, lifts the protection and makes it stale again). Off until enabled.
    void EnableWriteTracking();
    bool WriteTracking();
    void MarkPhysicalStale(uint32_t physical, uint32_t size);
    // Watches the stale pages of [physical, physical + size) and appends them to `runs` as (first page, pages): the
    // caller brings its copy of them up to date. A CPU write after this call makes them stale again.
    void WatchStalePages(uint32_t physical, uint32_t size, std::vector<std::pair<uint32_t, uint32_t>>& runs);
    // Before the host writes guest memory from kernel mode (ReadFile, recv), where no fault reaches the tracking: the
    // watched pages of the range become stale and writable. Any guest address; only the physical windows are tracked.
    void PrepareHostWrite(uint32_t address, uint32_t size);
}
