#pragma once

// Party voice chat (docs/x360/network_protocol.md "Voice"): while a signed-in user is in a party, the PC's
// microphone goes to the party's other members through the server and their voices are mixed to the PC's speakers.
// One voice stream per running port (the first signed-in user who is in a party). Started by live::Start.
//
// 16 kHz mono, 20 ms frames, IMA ADPCM (this file's own code). Audio devices: WASAPI shared mode, default
// communications devices, Windows converting to 16 kHz mono 16-bit. Test hooks (environment):
//   LIVE_VOICE_TONE=<Hz>     a sine tone instead of the microphone
//   LIVE_VOICE_DUMP=<file>   what is heard (the mix), as a 16 kHz mono WAV
// Without audio devices (Config::voiceDevices false, e.g. a headless run) only the tone and the dump work.

#include <cstdint>
#include <vector>

namespace live::voice
{
    constexpr uint32_t kSampleRate = 16000, kFrameSamples = 320; // 20 ms
    constexpr uint32_t kPayloadBytes = 4 + kFrameSamples / 2;    // predictor, index, 0, 160 bytes of codes

    // IMA ADPCM with the state carried across frames by each side (the frame header resets the decoder).
    struct AdpcmState
    {
        int16_t predictor = 0;
        uint8_t index = 0;
    };
    void Encode(AdpcmState& state, const int16_t* samples, uint8_t* payload);
    void Decode(const uint8_t* payload, int16_t* samples);

    struct Settings
    {
        bool devices = true; // WASAPI microphone and speakers
    };
    void Start(const Settings& settings);
    void Stop();

    // Members heard speaking in the last 300 ms (the party list's "talking" state: frames louder than a microphone's
    // background noise), and members whose microphone is open (frames in the last 2 s; muted ones send none).
    std::vector<uint64_t> Talking();
    std::vector<uint64_t> Headsets();
    // This PC in the party: its microphone (or the test tone) is open, and it is speaking now (not muted).
    bool MicOpen();
    bool LocalTalking();
    // Whether Windows has a default recording device (or the test tone is on), checked every second, in a party or
    // not; false without audio devices (Config::voiceDevices false).
    bool HasMicrophone();
    void SetMuted(bool muted); // the microphone (the Guide's mute)
    bool Muted();
    // The Party Options' Chat Channel: Game Chat (the title's own voice chat) sends and plays nothing of the party;
    // Party Chat (the default) is the party's voice.
    void SetGameChat(bool gameChat);
    bool GameChat();
}
