// Write tracking for a copy of guest physical memory kept elsewhere (memory.h; the GPU's copy in video memory).
#include "memory.h"

#include "log.h"

#include <windows.h>

#include <algorithm>
#include <atomic>

namespace guest
{
    namespace
    {
        constexpr uint32_t kTrackPageShift = 12, kTrackPages = kPhysicalSize >> kTrackPageShift, kTrackWords = kTrackPages / 64;
        std::atomic<bool> g_tracking{ false };
        std::atomic<uint64_t> g_stale[kTrackWords];
        std::atomic<uint64_t> g_watched[kTrackWords];
        // Orders watching and unwatching (the protection and both bits change together). Held only around
        // VirtualProtect and the bits, never while touching guest memory, so no fault arrives while it is held.
        SRWLOCK g_trackLock = SRWLOCK_INIT;

        uint64_t Mask(uint32_t bit, uint32_t n) { return (n == 64 ? ~0ull : ((1ull << n) - 1)) << bit; }

        bool Bit(const std::atomic<uint64_t>* bits, uint32_t page)
        {
            return (bits[page / 64].load(std::memory_order_acquire) >> (page % 64)) & 1;
        }

        void SetBits(std::atomic<uint64_t>* bits, uint32_t page, uint32_t count, bool set)
        {
            for (const uint32_t end = page + count; page < end;)
            {
                const uint32_t bit = page % 64, n = std::min(64 - bit, end - page);
                if (set)
                    bits[page / 64].fetch_or(Mask(bit, n), std::memory_order_acq_rel);
                else
                    bits[page / 64].fetch_and(~Mask(bit, n), std::memory_order_acq_rel);
                page += n;
            }
        }

        // The pages in every window. Each window's view maps physical 0 at the window's base (the 4 KiB window's
        // offset is applied by Translate, not by its view).
        void ProtectPages(uint32_t page, uint32_t count, DWORD protect)
        {
            for (uint32_t window : { kPhysicalWindow64k, kPhysicalWindow16m, kPhysicalWindow4k })
            {
                DWORD old;
                if (!VirtualProtect(g_memory.base + window + (uint64_t(page) << kTrackPageShift),
                                    SIZE_T(count) << kTrackPageShift, protect, &old))
                    Fatal("Write tracking: VirtualProtect of physical pages {:X}+{:X} failed ({})", page, count,
                          GetLastError());
            }
        }

        // The watched pages of [page, page + count) become stale and writable (the lock held).
        void Unwatch(uint32_t page, uint32_t count)
        {
            for (uint32_t p = page, end = page + count; p < end;)
            {
                if (!Bit(g_watched, p))
                {
                    p++;
                    continue;
                }
                uint32_t run = 1;
                while (p + run < end && Bit(g_watched, p + run))
                    run++;
                ProtectPages(p, run, PAGE_READWRITE);
                SetBits(g_watched, p, run, false);
                SetBits(g_stale, p, run, true);
                p += run;
            }
        }

        bool PhysicalPageOf(const void* host, uint32_t& page)
        {
            const auto* p = static_cast<const uint8_t*>(host);
            if (p < g_memory.base)
                return false;
            const uint64_t offset = uint64_t(p - g_memory.base);
            if (offset < kPhysicalWindow64k)
                return false;
            const uint64_t window = offset >= kPhysicalWindow4k ? kPhysicalWindow4k
                                  : offset >= kPhysicalWindow16m ? kPhysicalWindow16m : kPhysicalWindow64k;
            if (offset - window >= kPhysicalSize)
                return false;
            page = uint32_t((offset - window) >> kTrackPageShift);
            return true;
        }

        // The CPU's first write to a watched page, on any thread: lift the watch and retry the instruction (an atomic
        // one included). A write fault on a page that is writable by now (another thread lifted it first) retries too.
        LONG CALLBACK WriteTrackingHandler(EXCEPTION_POINTERS* info)
        {
            const EXCEPTION_RECORD* record = info->ExceptionRecord;
            if (record->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || record->NumberParameters < 2 ||
                record->ExceptionInformation[0] != 1)
                return EXCEPTION_CONTINUE_SEARCH;
            const void* target = reinterpret_cast<const void*>(record->ExceptionInformation[1]);
            uint32_t page;
            if (!PhysicalPageOf(target, page))
                return EXCEPTION_CONTINUE_SEARCH;
            AcquireSRWLockExclusive(&g_trackLock);
            Unwatch(page, 1);
            ReleaseSRWLockExclusive(&g_trackLock);
            MEMORY_BASIC_INFORMATION region;
            if (VirtualQuery(target, &region, sizeof(region)) && region.Protect == PAGE_READWRITE)
                return EXCEPTION_CONTINUE_EXECUTION;
            return EXCEPTION_CONTINUE_SEARCH;
        }

        void PageRange(uint32_t physical, uint32_t size, uint32_t& first, uint32_t& count)
        {
            physical &= kPhysicalSize - 1;
            const uint64_t end = std::min<uint64_t>(uint64_t(physical) + size, kPhysicalSize);
            first = physical >> kTrackPageShift;
            count = uint32_t(((end + (1u << kTrackPageShift) - 1) >> kTrackPageShift) - first);
        }
    }

    void EnableWriteTracking()
    {
        if (g_tracking.exchange(true))
            return;
        SetBits(g_stale, 0, kTrackPages, true);
        // First in line: ahead of the guest fault handler, which treats any other fault in guest memory as fatal.
        if (!AddVectoredExceptionHandler(1, WriteTrackingHandler))
            Fatal("Write tracking: AddVectoredExceptionHandler failed");
    }

    bool WriteTracking() { return g_tracking.load(std::memory_order_relaxed); }

    void MarkPhysicalStale(uint32_t physical, uint32_t size)
    {
        if (!WriteTracking() || size == 0)
            return;
        uint32_t first, count;
        PageRange(physical, size, first, count);
        SetBits(g_stale, first, count, true);
    }

    void WatchStalePages(uint32_t physical, uint32_t size, std::vector<std::pair<uint32_t, uint32_t>>& runs)
    {
        if (!WriteTracking() || size == 0)
            return;
        uint32_t first, count;
        PageRange(physical, size, first, count);
        // Without the lock first: most ranges have no stale page.
        bool any = false;
        for (uint32_t p = first, end = first + count; p < end && !any;)
        {
            const uint32_t bit = p % 64, n = std::min(64 - bit, end - p);
            any = (g_stale[p / 64].load(std::memory_order_acquire) & Mask(bit, n)) != 0;
            p += n;
        }
        if (!any)
            return;
        AcquireSRWLockExclusive(&g_trackLock);
        for (uint32_t p = first, end = first + count; p < end;)
        {
            if (!Bit(g_stale, p))
            {
                p++;
                continue;
            }
            uint32_t run = 1;
            while (p + run < end && Bit(g_stale, p + run))
                run++;
            // Protected before the stale bit is cleared and the copy recorded: a write from now on faults (stale
            // again), and one before it reaches the copy, which runs on the GPU later.
            ProtectPages(p, run, PAGE_READONLY);
            SetBits(g_watched, p, run, true);
            SetBits(g_stale, p, run, false);
            runs.emplace_back(p, run);
            p += run;
        }
        ReleaseSRWLockExclusive(&g_trackLock);
    }

    void PrepareHostWrite(uint32_t address, uint32_t size)
    {
        if (!WriteTracking() || size == 0 || !IsPhysicalWindow(address))
            return;
        uint32_t first, count;
        PageRange(WindowToPhysical(address), size, first, count);
        AcquireSRWLockExclusive(&g_trackLock);
        Unwatch(first, count);
        ReleaseSRWLockExclusive(&g_trackLock);
    }
}
