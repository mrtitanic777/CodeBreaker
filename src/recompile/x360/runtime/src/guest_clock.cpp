#include "guest_clock.h"

#include "audio/apu.h"
#include "gpu/gpu.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <thread>

namespace guest_clock
{
    namespace
    {
        const bool g_fixed = [] { const char* v = std::getenv("CB_FIXED_CLOCK"); return v && v[0] == '1'; }();
        const auto g_start = std::chrono::steady_clock::now();
        std::atomic<uint64_t> g_vblankNs = 0;  // fixed mode: the clock at the last vblank
        std::atomic<uint64_t> g_queries = 0;   // fixed mode: queries since it (1 ns each)
        uint64_t g_lastSwap = 0;
    }

    bool Fixed()
    {
        return g_fixed;
    }

    uint64_t Nanoseconds()
    {
        if (!g_fixed)
            return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() -
                                                                                  g_start).count());
        return g_vblankNs + g_queries.fetch_add(1);
    }

    void Vblank(uint64_t periodNs)
    {
        if (!g_fixed)
            return;
        // Frame-locked: on a vblank where the guest's cadence (vblanks per frame, as last seen: 2 for the boot
        // animation's 30 frames/s) expects a frame, the next vblank waits until it is presented (or 250 ms, for the
        // stretches without frames: loading).
        static uint32_t sinceSwap = 0, cadence = 1;
        sinceSwap++;
        if (sinceSwap >= cadence)
        {
            const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(250);
            while (gpu::SwapCount() == g_lastSwap && std::chrono::steady_clock::now() < deadline)
                std::this_thread::sleep_for(std::chrono::microseconds(200));
        }
        if (gpu::SwapCount() != g_lastSwap)
        {
            cadence = std::clamp(sinceSwap, 1u, 4u);
            sinceSwap = 0;
        }
        g_lastSwap = gpu::SwapCount();
        g_queries = 0;
        g_vblankNs += periodNs;
        audio::AdvanceFixedClock(g_vblankNs);
    }
}

// The recompiled code's mftb (ppc_shared/ppc_lookup.h): 50 MHz ticks.
uint64_t PpcQueryTimebase() noexcept
{
    return guest_clock::Nanoseconds() / 20;
}
