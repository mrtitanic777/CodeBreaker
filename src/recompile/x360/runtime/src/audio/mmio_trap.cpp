#include "audio/mmio_trap.h"

#include "log.h"
#include "memory.h"

#include <windows.h>

#include <array>
#include <cstring>
#include <mutex>
#include <vector>

namespace audio
{
    namespace
    {
        constexpr uint32_t kPageSize = 0x1000;
        constexpr uint64_t kTrapFlag = 0x100;

        struct Trap
        {
            uint32_t guestPage = 0;
            uint8_t* host = nullptr;
            MmioWriteHandler handler = nullptr;
            std::array<uint8_t, kPageSize> shadow{}; // page contents after the last dispatched write
            std::recursive_mutex mutex;
        };

        std::vector<Trap*> g_traps;

        struct Pending
        {
            Trap* trap = nullptr;
            uint32_t faultOffset = 0;
        };
        thread_local Pending t_pending;

        void Protect(Trap& trap, DWORD protect)
        {
            DWORD old = 0;
            if (!VirtualProtect(trap.host, kPageSize, protect, &old))
                Fatal("MMIO trap: VirtualProtect({:08X}) failed ({})", trap.guestPage, GetLastError());
        }

        // Reports the word at the fault (even when its value did not change: writing the same kick bits again is a
        // new kick) and every other changed word, in address order. Called with the page writable and the lock held.
        void Dispatch(Trap& trap, uint32_t faultOffset)
        {
            const uint32_t faultWord = faultOffset & ~3u;
            std::vector<uint32_t> written;
            for (uint32_t offset = 0; offset < kPageSize; offset += 4)
                if (offset == faultWord || std::memcmp(trap.host + offset, trap.shadow.data() + offset, 4) != 0)
                    written.push_back(offset);
            for (uint32_t offset : written)
            {
                uint32_t raw;
                std::memcpy(&raw, trap.host + offset, 4);
                trap.handler(trap.guestPage + offset, __builtin_bswap32(raw));
            }
            // Handlers may have rewritten registers.
            std::memcpy(trap.shadow.data(), trap.host, kPageSize);
        }

        Trap* FindTrap(uintptr_t hostAddress)
        {
            for (Trap* trap : g_traps)
                if (hostAddress >= uintptr_t(trap->host) && hostAddress < uintptr_t(trap->host) + kPageSize)
                    return trap;
            return nullptr;
        }

        LONG CALLBACK TrapHandler(EXCEPTION_POINTERS* info)
        {
            const DWORD code = info->ExceptionRecord->ExceptionCode;
            if (code == EXCEPTION_ACCESS_VIOLATION && info->ExceptionRecord->NumberParameters >= 2
                && info->ExceptionRecord->ExceptionInformation[0] == 1)
            {
                Trap* trap = FindTrap(info->ExceptionRecord->ExceptionInformation[1]);
                if (!trap)
                    return EXCEPTION_CONTINUE_SEARCH;
                if (t_pending.trap)
                    Fatal("MMIO trap: nested store to {:08X} while one is pending", trap->guestPage);
                trap->mutex.lock(); // released after the single step
                Protect(*trap, PAGE_READWRITE);
                t_pending.trap = trap;
                t_pending.faultOffset = uint32_t(info->ExceptionRecord->ExceptionInformation[1] - uintptr_t(trap->host));
                info->ContextRecord->EFlags |= kTrapFlag;
                return EXCEPTION_CONTINUE_EXECUTION;
            }
            if (code == EXCEPTION_SINGLE_STEP && t_pending.trap)
            {
                Trap& trap = *t_pending.trap;
                const uint32_t faultOffset = t_pending.faultOffset;
                t_pending.trap = nullptr;
                info->ContextRecord->EFlags &= ~kTrapFlag;
                Dispatch(trap, faultOffset);
                Protect(trap, PAGE_READONLY);
                trap.mutex.unlock();
                return EXCEPTION_CONTINUE_EXECUTION;
            }
            return EXCEPTION_CONTINUE_SEARCH;
        }
    }

    void InstallMmioWriteTrap(uint32_t guestPage, MmioWriteHandler handler)
    {
        static const bool installed = [] { return AddVectoredExceptionHandler(1, TrapHandler) != nullptr; }();
        if (!installed)
            Fatal("MMIO trap: AddVectoredExceptionHandler failed");
        auto* trap = new Trap;
        trap->guestPage = guestPage;
        trap->host = guest::g_memory.Translate(guestPage);
        trap->handler = handler;
        std::memcpy(trap->shadow.data(), trap->host, kPageSize);
        Protect(*trap, PAGE_READONLY);
        g_traps.push_back(trap);
    }

    void WriteMmio(uint32_t guestAddress, uint32_t value)
    {
        Trap* trap = FindTrap(uintptr_t(guest::g_memory.Translate(guestAddress)));
        if (!trap)
        {
            guest::g_memory.Write32(guestAddress, value);
            return;
        }
        std::lock_guard lock(trap->mutex);
        DWORD old = 0;
        VirtualProtect(trap->host, kPageSize, PAGE_READWRITE, &old);
        guest::g_memory.Write32(guestAddress, value);
        std::memcpy(trap->shadow.data() + (guestAddress - trap->guestPage), trap->host + (guestAddress - trap->guestPage), 4);
        if (old != PAGE_READWRITE)
            VirtualProtect(trap->host, kPageSize, old, &old);
    }
}
