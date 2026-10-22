#pragma once

#include <cstdint>

// The boot animation as the 17559 kernel plays it (xboxkrnl's Ani* exports; xam.xex imports AniStartBootAnimation,
// AniBlockOnAnimation, AniTerminateAnimation and AniSetLogo). From Nexia360's reading of the retail kernel
// (src/xenia/kernel/xboxkrnl/xboxkrnl_ani.cc, BSD-3) and bootanim.xex's own code:
//
//   AniStartBootAnimation: XexLoadImage("\Device\Flash\bootanim.xex", 0x40000009, 0x20449700, &h), then
//     ExCreateThread(stack 0x8000, flags 0x020000A0 = CPU 1 | ReturnKThreadPtr | PriorityClass1) on a thread that
//     calls bootanim's ordinal 1 as proc(h, flags). `flags` is a kernel boot word (*(u32*)0x80170000): bit 0x8000
//     turns the display off and skips the animation, 0x4000 changes device setup. A normal power-on passes neither.
//   AniBlockOnAnimation / AniTerminateAnimation: one routine with terminate = 0 / 1 that calls bootanim's ordinal 2
//     with that flag, then waits for the player thread. Ordinal 2(0) lets the animation reach its natural end;
//     ordinal 2(1) stops it at once.
//   bootanim's ordinal 3 draws a logo (AniSetLogo); not used yet.
//
// bootanim.xex must already be loaded (modules::Load). The player thread runs in the system process: bootanim's D3D
// then registers its device in VdGlobalXamDevice, as XAM's does, leaving VdGlobalDevice to the dashboard / title.
namespace kernel
{
    // Returns false if it is already running or bootanim.xex is not loaded.
    bool StartBootAnimation(uint32_t flags = 0);
    // AniBlockOnAnimation: lets the animation finish, then waits for its thread to end.
    void BlockOnBootAnimation();
    // AniTerminateAnimation: stops the animation now, then waits for its thread to end.
    void TerminateBootAnimation();
}
