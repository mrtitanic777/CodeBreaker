#pragma once

#include <cstdint>

// The guest's clock: what the recompiled code's time base (mftb, PPC_QUERY_TIMEBASE: 50 MHz) and KeTimeStampBundle's
// interrupt time and tick count read.
//
// Normally it is the host's steady clock since start. CB_FIXED_CLOCK=1 (tests) makes it repeatable: it advances by
// exactly one refresh period per vblank, and each vblank waits (up to 250 ms) for the frame of the previous one to be
// presented, so a time-driven animation (the boot animation) shows the same picture at frame N in every run whatever
// the host's speed. Between vblanks it advances by one nanosecond per query (it never stands still).
// The audio render clock follows it too (audio::AdvanceFixedClock: the frames due at each vblank, each one's
// callbacks finished before the next). 2026-10-19, two boots: the boot animation's sound is identical, its frames
// 60-400 differ in 0-0.9 % of pixels (before: 4-65 %); the dash's sound drifts after it starts (loading takes host
// time while the vblanks wait at most 250 ms).
namespace guest_clock
{
    bool Fixed();
    uint64_t Nanoseconds();
    // The vblank thread (gpu/command_processor.cpp), once per vblank; in fixed mode it first waits for the swap.
    void Vblank(uint64_t periodNs);
}
