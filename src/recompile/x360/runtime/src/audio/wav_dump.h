#pragma once

#include <cstdint>

// CB_AUDIO_DUMP=<file.wav>: the console's mixed output (what the render driver plays), 48 kHz, 6 channels
// FL FR FC LFE BL BR, 32-bit float, written as it is produced. The header is rewritten every second so the file stays
// valid when the process is killed.
namespace audio
{
    bool WavDumpEnabled();
    void DumpSamples(const float* interleaved, uint32_t samples);
}
