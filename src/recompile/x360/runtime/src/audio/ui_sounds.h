#pragma once

#include <cstdint>

// The system UI's sounds (native/x360/system/xui's XuiSoundXAudio elements: the Guide's focus / select / blade / tab
// sounds, hudbkgnd's open / close). XAM plays them through its own XAudio2 voices, beside the title's audio; here
// they are decoded on the host (XMA1 -> 16-bit PCM, cached per file), resampled to 48 kHz and added to the render
// driver's mix every tick (kernel/xboxkrnl_audio.cpp), so they reach the device and CB_AUDIO_DUMP like the guest's.
// CB_UI_SOUND_LOG=1 logs every sound played (with the render tick: 187.5 ticks per second of the dump).
namespace audio
{
    // Installs the XUI sound player (xui::host::SetSoundPlayer). Called once at start-up.
    void InitializeUiSounds();
    // Adds the playing sounds to `mix` (interleaved FL FR FC LFE BL BR, kFrameSamples frames) and advances them.
    void MixUiSounds(float* mix, uint32_t samples);
    // Starts the render driver's clock if no guest client has yet (kernel/xboxkrnl_audio.cpp).
    void StartRenderClock();
}
