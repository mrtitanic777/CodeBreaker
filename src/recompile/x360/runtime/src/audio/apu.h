#pragma once

#include <cstdint>
#include <string>
#include <vector>

// The XMA decoder block of the Xbox 360 audio processing unit (APU).
//
// Registers: 0x7FEA0000 + 4 * index, accessed byte-reversed (little-endian) by the title's XMA HAL (statically
// linked into XAudio2): 0x600 context array physical address, 0x601 written 2 / 3 around a lock sequence,
// 0x606 current / 0x607 next context index, 0x650-0x659 kick, 0x690-0x699 lock, 0x6A0-0x6A9 clear (one bit per
// context, 32 contexts per register). Writes reach the runtime through an MMIO write trap (mmio_trap.h).
//
// Contexts: 320 x 64 bytes in physical memory, big-endian words (layout from the title's XMA HAL and the XDK's
// xmadecoder.h / xmahardwareabstraction.h; see apu.cpp). A kick decodes on the kicking thread before the store
// returns, as much as the output ring and the input buffers allow (the hardware keeps decoding while a context is
// enabled; the title only observes the context under a lock, so finishing the work at the kick is equivalent).
namespace audio
{
    // Render-driver ticks so far (kernel/xboxkrnl_audio.cpp), for traces.
    uint64_t RenderTicks();
    // CB_FIXED_CLOCK=1 only: plays the audio frames due by the guest time guestNs (kernel/xboxkrnl_audio.cpp).
    void AdvanceFixedClock(uint64_t guestNs);
    // Allocates the context array and the register block and installs the register trap.
    void InitializeApu();
    // XMACreateContext / XMAReleaseContext.
    uint32_t AllocateXmaContext(); // guest virtual address, 0 when none is free
    void ReleaseXmaContext(uint32_t context);
    // The kernel's XMA API (xboxkrnl_audio.cpp: XMAInitializeContext ..., the older dashboards' XAudio): a context's
    // field (word, shift, bits; the layout in apu.cpp) read or written under the APU's lock. The hardware keeps decoding
    // an enabled context; here an enabled context is decoded after each write that `kick`s it (as at a register kick).
    uint32_t XmaField(uint32_t context, int word, int shift, int bits);
    void SetXmaField(uint32_t context, int word, int shift, int bits, uint32_t value, bool kick);
    void EnableXmaContext(uint32_t context);
    void DisableXmaContext(uint32_t context);
    // CB_AUDIO_TEST: streams an XMA2 packet sequence through a context (ping-pong input buffers, drained output
    // ring) and compares with the plain stream decode. Returns failures.
    int TestApuStream(const std::vector<uint8_t>& packets, int channels, int sampleRate, std::string& report);
    // CB_AUDIO_TEST: a hardware loop (loop end / subframe end, loop start / subframe skip, loop count).
    int TestApuLoop(const std::vector<uint8_t>& packets, int channels, int sampleRate, std::string& report);
}
