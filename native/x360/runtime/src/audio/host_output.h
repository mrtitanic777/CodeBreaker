#pragma once

#include <cstdint>
#include <string>

// Host audio output: plays the console's mixed output (48 kHz, 6 channels FL FR FC LFE BL BR, float) on the default
// Windows render device through WASAPI in shared mode. Windows converts the rate when the device mix rate differs.
//
// The console's audio clock is emulated by the render driver (kernel/xboxkrnl_audio.cpp); this module only queues
// what that clock produced and reports how much is still queued, so the clock can be steered to the device's rate.
namespace audio
{
    constexpr uint32_t kSampleRate = 48000;
    constexpr uint32_t kChannels = 6;
    constexpr uint32_t kFrameSamples = 256; // one render-driver frame

    // Opens the default render device. False (and a log line) when there is none or CB_AUDIO=0; the caller then
    // runs its clock from a timer.
    bool StartHostOutput();
    bool HostOutputActive();
    // Appends samples (interleaved FL FR FC LFE BL BR) to the device queue; drops them when the queue is full.
    void QueueHostSamples(const float* interleaved, uint32_t samples);
    // Samples queued and not yet handed to the device.
    uint32_t QueuedHostSamples();
    // Description of the device format and channel mapping (for the log).
    std::string DescribeHostOutput();
}
