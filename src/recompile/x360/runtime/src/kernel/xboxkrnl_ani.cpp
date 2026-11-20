// The kernel's boot animation player (see ani.h).

#include "kernel/ani.h"

#include "kernel/kernel.h"
#include "modules.h"

#include <windows.h>

#include <mutex>

namespace kernel
{
    namespace
    {
        constexpr uint32_t kPlayerStackSize = 0x8000;
        constexpr uint32_t kPlayerCreationFlags = 0x020000A0; // CPU 1, ReturnKThreadPtr, PriorityClass1

        std::mutex g_mutex;
        GuestThread* g_player = nullptr;
        uint32_t g_stop = 0; // bootanim's ordinal 2

        void Stop(bool terminate)
        {
            GuestThread* player;
            uint32_t stop;
            {
                std::lock_guard lock(g_mutex);
                player = g_player;
                stop = g_stop;
            }
            if (player == nullptr)
                return;
            // Ordinal 2 is guest code: it runs on a short thread of its own in the system process, r3 = terminate.
            ThreadParams params;
            params.stackSize = 0x4000;
            params.startAddress = stop;
            params.startContext = terminate ? 1 : 0;
            params.processType = kProcessTypeSystem;
            auto runStop = [&](bool how) {
                params.startContext = how ? 1 : 0;
                GuestThread* stopper = CreateThread(params);
                StartThread(stopper);
                WaitForSingleObject(stopper->hostHandle, INFINITE);
            };
            // 17559's ordinal 2 waits for the animation (0) or ends it (1). 1888 ... 9199's only sets the request on
            // the player's object (2241 0x98011198: +40 finish at the end, +41 now) and does nothing before the player
            // has made it (its pointer, 2241 0x98043040, is still 0 at the start), so it is asked again until the
            // player returns; without the request it holds the last frame (the logo) for good.
            int requests = 0;
            do
                runStop(terminate), requests++;
            while (WaitForSingleObject(player->hostHandle, 100) == WAIT_TIMEOUT);
            if (requests > 1)
                Log("Boot animation: its stop request was made {} times", requests);
            Log("Boot animation ended (player thread exit code {:08X})", player->exitCode);
            std::lock_guard lock(g_mutex);
            g_player = nullptr;
        }
    }

    bool StartBootAnimation(uint32_t flags)
    {
        std::lock_guard lock(g_mutex);
        if (g_player != nullptr)
            return false;
        const modules::LoadedModule* bootanim = modules::FindByName("bootanim.xex");
        if (bootanim == nullptr)
            return false;
        const uint32_t play = modules::ExportAddress(*bootanim, 1);
        g_stop = modules::ExportAddress(*bootanim, 2);
        if (play == 0 || g_stop == 0)
            Fatal("bootanim.xex lacks ordinal 1 or 2");

        // The kernel's player thread does proc(handle, flags): the XAPI-style startup slot gives exactly that call.
        ThreadParams params;
        params.stackSize = kPlayerStackSize;
        params.xapiThreadStartup = play;
        params.startAddress = bootanim->handle;
        params.startContext = flags;
        params.creationFlags = kPlayerCreationFlags;
        params.processType = kProcessTypeSystem;
        g_player = CreateThread(params);
        g_player->affinity = (kPlayerCreationFlags >> 24) & 0x3F; // CPU 1
        StartThread(g_player);
        Log("Boot animation started: bootanim.xex ordinal 1 at {:08X} (flags {:08X}) on thread {}", play, flags, g_player->id);
        return true;
    }

    void BlockOnBootAnimation()
    {
        Stop(false);
    }

    void TerminateBootAnimation()
    {
        Stop(true);
    }
}
