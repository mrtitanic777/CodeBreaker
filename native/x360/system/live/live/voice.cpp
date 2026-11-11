// Party voice chat (live/voice.h).

#include "live/voice.h"

#include "log.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <map>
#include <mutex>
#include <string>
#include <thread>

namespace live::detail
{
    // client.cpp: the user whose voice goes out (the first signed-in user in a party).
    struct VoiceTarget
    {
        bool active = false;
        std::string host;
        uint16_t port = 0;
        std::array<uint8_t, 8> token{};
    };
    VoiceTarget CurrentVoiceTarget();
}

namespace live::voice
{
    namespace
    {
        // IMA ADPCM tables (the IMA / DVI standard).
        constexpr int kIndexTable[16] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8 };
        constexpr int kStepTable[89] = { 7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
            50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408,
            449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499,
            2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635,
            13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 };

        int DecodeNibble(int code, int& predictor, int& index)
        {
            const int step = kStepTable[index];
            int diff = step >> 3;
            if (code & 4)
                diff += step;
            if (code & 2)
                diff += step >> 1;
            if (code & 1)
                diff += step >> 2;
            predictor += (code & 8) ? -diff : diff;
            predictor = std::clamp(predictor, -32768, 32767);
            index = std::clamp(index + kIndexTable[code], 0, 88);
            return predictor;
        }
    }

    void Encode(AdpcmState& state, const int16_t* samples, uint8_t* payload)
    {
        int predictor = state.predictor, index = state.index;
        payload[0] = uint8_t(uint16_t(predictor) >> 8);
        payload[1] = uint8_t(predictor);
        payload[2] = uint8_t(index);
        payload[3] = 0;
        for (uint32_t i = 0; i < kFrameSamples; i++)
        {
            const int step = kStepTable[index];
            int diff = samples[i] - predictor;
            int code = 0;
            if (diff < 0)
            {
                code = 8;
                diff = -diff;
            }
            if (diff >= step)
            {
                code |= 4;
                diff -= step;
            }
            if (diff >= step >> 1)
            {
                code |= 2;
                diff -= step >> 1;
            }
            if (diff >= step >> 2)
                code |= 1;
            DecodeNibble(code, predictor, index); // track exactly what the decoder will reconstruct
            uint8_t& byte = payload[4 + i / 2];
            byte = (i & 1) ? uint8_t(byte | code << 4) : uint8_t(code);
        }
        state.predictor = int16_t(predictor);
        state.index = uint8_t(index);
    }

    void Decode(const uint8_t* payload, int16_t* samples)
    {
        int predictor = int16_t(uint16_t(payload[0]) << 8 | payload[1]);
        int index = std::min<int>(payload[2], 88);
        for (uint32_t i = 0; i < kFrameSamples; i++)
        {
            const uint8_t byte = payload[4 + i / 2];
            samples[i] = int16_t(DecodeNibble((i & 1) ? byte >> 4 : byte & 15, predictor, index));
        }
    }

    namespace
    {
        constexpr uint8_t kMagic[4] = { 'X', 'V', '0', '1' };

        // A frame louder than this (mean absolute sample) is speech, not a microphone's background noise: the party
        // list's talking icon (frames are sent all the time while the microphone is open).
        constexpr int kTalkingLevel = 1000; // about -30 dBFS: speech, not breathing or a controller click

        bool Loud(const int16_t* samples)
        {
            int64_t sum = 0;
            for (uint32_t i = 0; i < kFrameSamples; i++)
                sum += std::abs(int(samples[i]));
            return sum / int64_t(kFrameSamples) > kTalkingLevel;
        }

        struct Speaker
        {
            std::deque<std::vector<int16_t>> frames;
            bool started = false;
            std::chrono::steady_clock::time_point lastHeard; // any frame (their microphone is open)
            std::chrono::steady_clock::time_point lastLoud;  // a frame with speech
        };

        struct State
        {
            Settings settings;
            std::thread thread;
            std::atomic<bool> stop = false, muted = false, gameChat = false;
            std::atomic<bool> micOpen = false; // this PC's microphone (or the test tone) is open for the party
            std::atomic<bool> micPresent = false; // Windows has a default recording device (checked every second)
            std::mutex mutex; // speakers, lastLocalLoud
            std::map<uint64_t, Speaker> speakers;
            std::chrono::steady_clock::time_point lastLocalLoud; // this PC's own speech
            // Voice mail (mutex): what is recorded / played, and how far.
            bool mailRecording = false, mailPlaying = false, mailHeard = false;
            std::vector<uint8_t> mailRecorded, mailPlay;
            size_t mailPlayed = 0; // frames of mailPlay played
            AdpcmState mailEncoder;
        };

        State& V()
        {
            static State state;
            return state;
        }

        // WASAPI endpoint in shared mode with Windows converting to 16 kHz mono 16-bit.
        // Whether Windows has a default recording device now (the party list's own headset icon, owner 2026-10-16:
        // shown from the start of a party, before the microphone stream opens). COM initialised by the caller.
        bool DefaultCaptureDevice()
        {
            IMMDeviceEnumerator* enumerator = nullptr;
            if (FAILED(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                                        reinterpret_cast<void**>(&enumerator))))
                return false;
            IMMDevice* device = nullptr;
            const bool found = SUCCEEDED(enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &device));
            enumerator->Release();
            if (device)
                device->Release();
            return found;
        }

        struct Endpoint
        {
            IAudioClient* client = nullptr;
            IAudioCaptureClient* capture = nullptr;
            IAudioRenderClient* render = nullptr;
            UINT32 bufferFrames = 0;

            bool Open(EDataFlow flow)
            {
                IMMDeviceEnumerator* enumerator = nullptr;
                IMMDevice* device = nullptr;
                if (FAILED(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
                                            __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator))))
                    return false;
                // The default devices (eConsole), not the communications ones: a stream on the communications
                // device counts as a call and Windows turns other sounds down ("When Windows detects communications
                // activity", 80% by default); the owner did not want party chat to do that (2026-10-16).
                const bool found = SUCCEEDED(enumerator->GetDefaultAudioEndpoint(flow, eConsole, &device));
                enumerator->Release();
                if (!found)
                    return false;
                const HRESULT activated = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                                                           reinterpret_cast<void**>(&client));
                device->Release();
                if (FAILED(activated))
                    return false;
                // Game chat: like communications for Windows' processing, but it never attenuates other streams.
                IAudioClient2* client2 = nullptr;
                if (SUCCEEDED(client->QueryInterface(__uuidof(IAudioClient2), reinterpret_cast<void**>(&client2))))
                {
                    AudioClientProperties properties{};
                    properties.cbSize = sizeof(properties);
                    properties.bIsOffload = FALSE;
                    properties.eCategory = AudioCategory_GameChat;
                    client2->SetClientProperties(&properties);
                    client2->Release();
                }
                WAVEFORMATEX format{};
                format.wFormatTag = WAVE_FORMAT_PCM;
                format.nChannels = 1;
                format.nSamplesPerSec = kSampleRate;
                format.wBitsPerSample = 16;
                format.nBlockAlign = 2;
                format.nAvgBytesPerSec = kSampleRate * 2;
                const DWORD flags = AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY;
                if (FAILED(client->Initialize(AUDCLNT_SHAREMODE_SHARED, flags, 2000000, 0, &format, nullptr)) ||
                    FAILED(client->GetBufferSize(&bufferFrames)))
                    return Close(), false;
                const HRESULT service = flow == eCapture
                    ? client->GetService(__uuidof(IAudioCaptureClient), reinterpret_cast<void**>(&capture))
                    : client->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&render));
                if (FAILED(service) || FAILED(client->Start()))
                    return Close(), false;
                return true;
            }

            void Close()
            {
                if (client)
                    client->Stop();
                if (capture)
                    capture->Release();
                if (render)
                    render->Release();
                if (client)
                    client->Release();
                client = nullptr;
                capture = nullptr;
                render = nullptr;
            }
        };

        // A 16 kHz mono 16-bit WAV written as it grows (the header is patched at the end).
        struct WavDump
        {
            FILE* file = nullptr;
            uint32_t bytes = 0;

            void Open(const char* path)
            {
                file = std::fopen(path, "wb");
                if (!file)
                    return;
                uint8_t header[44] = {};
                std::fwrite(header, 1, 44, file);
            }

            void Write(const int16_t* samples, size_t count)
            {
                if (!file)
                    return;
                std::fwrite(samples, 2, count, file);
                bytes += uint32_t(count * 2);
                if (bytes % (kSampleRate * 2) < count * 2) // about once a second: a valid file even if the process ends
                    Patch();
            }

            void Close()
            {
                if (!file)
                    return;
                Patch();
                std::fclose(file);
                file = nullptr;
            }

            void Patch()
            {
                auto le32 = [](uint8_t* p, uint32_t v) { for (int i = 0; i < 4; i++) p[i] = uint8_t(v >> (8 * i)); };
                uint8_t h[44] = { 'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1,
                                  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 16, 0, 'd', 'a', 't', 'a' };
                le32(h + 4, 36 + bytes);
                le32(h + 24, kSampleRate);
                le32(h + 28, kSampleRate * 2);
                le32(h + 40, bytes);
                std::fseek(file, 0, SEEK_SET);
                std::fwrite(h, 1, 44, file);
                std::fseek(file, 0, SEEK_END);
                std::fflush(file);
            }
        };

        // Mixes one 20 ms frame of everyone heard (each speaker starts after two buffered frames: jitter room).
        bool MixFrame(int16_t* out)
        {
            State& v = V();
            std::lock_guard lock(v.mutex);
            int32_t mix[kFrameSamples] = {};
            bool any = false;
            for (auto& [xuid, sp] : v.speakers)
            {
                if (!sp.started && sp.frames.size() >= 2)
                    sp.started = true;
                if (!sp.started)
                    continue;
                if (sp.frames.empty())
                {
                    sp.started = false;
                    continue;
                }
                for (uint32_t i = 0; i < kFrameSamples; i++)
                    mix[i] += sp.frames.front()[i];
                sp.frames.pop_front();
                any = true;
            }
            if (v.mailPlaying)
            {
                if ((v.mailPlayed + 1) * kPayloadBytes <= v.mailPlay.size())
                {
                    int16_t frame[kFrameSamples];
                    Decode(v.mailPlay.data() + v.mailPlayed * kPayloadBytes, frame);
                    for (uint32_t i = 0; i < kFrameSamples; i++)
                        mix[i] += frame[i];
                    v.mailPlayed++;
                    any = true;
                }
                if ((v.mailPlayed + 1) * kPayloadBytes > v.mailPlay.size())
                    v.mailPlaying = false;
            }
            for (uint32_t i = 0; i < kFrameSamples; i++)
                out[i] = int16_t(std::clamp(mix[i], -32768, 32767));
            return any;
        }

        void Run()
        {
            State& v = V();
            CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            const char* toneText = std::getenv("LIVE_VOICE_TONE");
            const double tone = toneText ? std::atof(toneText) : 0.0;
            WavDump dump;
            if (const char* path = std::getenv("LIVE_VOICE_DUMP"))
                dump.Open(path);
            Endpoint mic, speakers;
            bool devicesOpen = false;
            SOCKET sock = INVALID_SOCKET;
            sockaddr_storage server{};
            int serverLen = 0;
            std::string serverKey;
            AdpcmState encoder;
            uint32_t sequence = 0;
            std::vector<int16_t> captured;
            double phase = 0.0;
            auto clock = std::chrono::steady_clock::now();
            auto lastKeepAlive = clock - std::chrono::seconds(10);
            uint64_t tonesSent = 0;
            auto lastDeviceCheck = clock - std::chrono::seconds(10);
            while (!v.stop)
            {
                if (const auto t = std::chrono::steady_clock::now(); t - lastDeviceCheck >= std::chrono::seconds(1))
                {
                    v.micPresent = tone > 0 || (v.settings.devices && DefaultCaptureDevice());
                    lastDeviceCheck = t;
                }
                const detail::VoiceTarget target = detail::CurrentVoiceTarget();
                bool mail;
                {
                    std::lock_guard lock(v.mutex);
                    mail = v.mailRecording || v.mailPlaying;
                }
                if (!target.active && !mail)
                {
                    if (devicesOpen)
                    {
                        mic.Close();
                        speakers.Close();
                        devicesOpen = false;
                        v.micOpen = false;
                        Log("Voice: microphone and speakers closed (no party, no voice mail)");
                    }
                    {
                        std::lock_guard lock(v.mutex);
                        v.speakers.clear();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    clock = std::chrono::steady_clock::now();
                    continue;
                }
                // The server's UDP address (the TCP connection's host and port); none for voice mail alone.
                const std::string key = target.active ? target.host + ":" + std::to_string(target.port) : std::string();
                if (!target.active && sock != INVALID_SOCKET)
                {
                    closesocket(sock);
                    sock = INVALID_SOCKET;
                    serverKey.clear();
                }
                if (target.active && (sock == INVALID_SOCKET || key != serverKey))
                {
                    if (sock != INVALID_SOCKET)
                        closesocket(sock);
                    sock = INVALID_SOCKET;
                    addrinfo hints{}, *result = nullptr;
                    hints.ai_family = AF_UNSPEC;
                    hints.ai_socktype = SOCK_DGRAM;
                    if (getaddrinfo(target.host.c_str(), std::to_string(target.port).c_str(), &hints, &result) == 0)
                    {
                        sock = socket(result->ai_family, SOCK_DGRAM, IPPROTO_UDP);
                        std::memcpy(&server, result->ai_addr, result->ai_addrlen);
                        serverLen = int(result->ai_addrlen);
                        freeaddrinfo(result);
                        u_long nonBlocking = 1;
                        if (sock != INVALID_SOCKET)
                            ioctlsocket(sock, FIONBIO, &nonBlocking);
                    }
                    serverKey = key;
                }
                if (!devicesOpen)
                {
                    if (v.settings.devices)
                    {
                        if (!mic.Open(eCapture))
                            Log("Voice: no microphone (party chat listens only)");
                        if (!speakers.Open(eRender))
                            Log("Voice: no speakers for party chat");
                    }
                    devicesOpen = true;
                    Log("Voice: {}: {}{}{}", target.active ? "in a party or chat" : "voice mail",
                        mic.client ? "microphone open" : "no microphone",
                        speakers.client ? ", speakers open" : ", no speakers", tone > 0 ? " (test tone)" : "");
                    v.micOpen = mic.client || tone > 0;
                    captured.clear();
                    encoder = {};
                    clock = std::chrono::steady_clock::now();
                }
                auto send = [&](const uint8_t* payload, size_t size) {
                    if (sock == INVALID_SOCKET)
                        return;
                    uint8_t packet[16 + kPayloadBytes];
                    std::memcpy(packet, kMagic, 4);
                    std::memcpy(packet + 4, target.token.data(), 8);
                    const uint32_t seq = sequence++;
                    packet[12] = uint8_t(seq >> 24);
                    packet[13] = uint8_t(seq >> 16);
                    packet[14] = uint8_t(seq >> 8);
                    packet[15] = uint8_t(seq);
                    if (size)
                        std::memcpy(packet + 16, payload, size);
                    sendto(sock, reinterpret_cast<const char*>(packet), int(16 + size), 0,
                           reinterpret_cast<const sockaddr*>(&server), serverLen);
                };
                const auto now = std::chrono::steady_clock::now();
                if (target.active && now - lastKeepAlive > std::chrono::seconds(1))
                {
                    send(nullptr, 0);
                    lastKeepAlive = now;
                }
                // What goes out: the test tone at real-time pace, else the microphone.
                if (tone > 0)
                {
                    while (clock + std::chrono::milliseconds(20) <= now)
                    {
                        for (uint32_t i = 0; i < kFrameSamples; i++)
                        {
                            captured.push_back(int16_t(8000 * std::sin(phase)));
                            phase += 2 * 3.14159265358979 * tone / kSampleRate;
                        }
                        clock += std::chrono::milliseconds(20);
                    }
                }
                else if (mic.capture)
                {
                    UINT32 packetFrames = 0;
                    while (SUCCEEDED(mic.capture->GetNextPacketSize(&packetFrames)) && packetFrames)
                    {
                        BYTE* data = nullptr;
                        UINT32 frames = 0;
                        DWORD flags = 0;
                        if (FAILED(mic.capture->GetBuffer(&data, &frames, &flags, nullptr, nullptr)))
                            break;
                        const int16_t* samples = reinterpret_cast<const int16_t*>(data);
                        for (UINT32 i = 0; i < frames; i++)
                            captured.push_back((flags & AUDCLNT_BUFFERFLAGS_SILENT) ? 0 : samples[i]);
                        mic.capture->ReleaseBuffer(frames);
                    }
                }
                while (captured.size() >= kFrameSamples)
                {
                    {
                        // Voice mail records the microphone whatever the party does with it.
                        std::lock_guard lock(v.mutex);
                        if (v.mailRecording)
                        {
                            uint8_t payload[kPayloadBytes];
                            Encode(v.mailEncoder, captured.data(), payload); // each frame carries the decoder's state
                            v.mailRecorded.insert(v.mailRecorded.end(), payload, payload + kPayloadBytes);
                            if (Loud(captured.data()))
                                v.mailHeard = true;
                            if (v.mailRecorded.size() >= size_t(kMailFrames) * kPayloadBytes)
                            {
                                v.mailRecording = false;
                                Log("Voice: voice mail recorded (15 s, the most)");
                            }
                        }
                    }
                    if (target.active && !v.muted && !v.gameChat)
                    {
                        if (Loud(captured.data()))
                        {
                            std::lock_guard lock(v.mutex);
                            v.lastLocalLoud = std::chrono::steady_clock::now();
                        }
                        uint8_t payload[kPayloadBytes];
                        Encode(encoder, captured.data(), payload);
                        send(payload, sizeof(payload));
                        if (tone > 0 && ++tonesSent == 50)
                            Log("Voice: 1 s of test tone sent");
                    }
                    captured.erase(captured.begin(), captured.begin() + kFrameSamples);
                }
                // What comes in: frames from the other members.
                uint8_t packet[2048];
                for (;;)
                {
                    const int n = sock == INVALID_SOCKET ? -1 : recv(sock, reinterpret_cast<char*>(packet), sizeof(packet), 0);
                    if (n < 16 + int(kPayloadBytes) || std::memcmp(packet, kMagic, 4) != 0)
                    {
                        if (n < 0)
                            break;
                        continue;
                    }
                    uint64_t xuid = 0;
                    for (int i = 0; i < 8; i++)
                        xuid = xuid << 8 | packet[4 + i];
                    if (v.gameChat)
                        continue; // Game Chat: the party is not heard
                    std::vector<int16_t> frame(kFrameSamples);
                    Decode(packet + 16, frame.data());
                    std::lock_guard lock(v.mutex);
                    Speaker& sp = v.speakers[xuid];
                    if (sp.lastHeard.time_since_epoch().count() == 0)
                        Log("Voice: hearing {:016X}", xuid);
                    sp.lastHeard = std::chrono::steady_clock::now();
                    if (Loud(frame.data()))
                        sp.lastLoud = sp.lastHeard;
                    if (sp.frames.size() < 25) // half a second at most behind
                        sp.frames.push_back(std::move(frame));
                }
                // Playback: keep about 60 ms queued in the speakers (or play to the dump at real-time pace).
                static auto playClock = std::chrono::steady_clock::now();
                if (speakers.render)
                {
                    UINT32 padding = 0;
                    while (SUCCEEDED(speakers.client->GetCurrentPadding(&padding)) && padding < kSampleRate * 60 / 1000 &&
                           speakers.bufferFrames - padding >= kFrameSamples)
                    {
                        int16_t mixed[kFrameSamples];
                        MixFrame(mixed);
                        dump.Write(mixed, kFrameSamples);
                        BYTE* data = nullptr;
                        if (FAILED(speakers.render->GetBuffer(kFrameSamples, &data)))
                            break;
                        std::memcpy(data, mixed, sizeof(mixed));
                        speakers.render->ReleaseBuffer(kFrameSamples, 0);
                    }
                }
                else
                {
                    const auto t = std::chrono::steady_clock::now();
                    if (t - playClock > std::chrono::milliseconds(200))
                        playClock = t;
                    while (playClock + std::chrono::milliseconds(20) <= t)
                    {
                        int16_t mixed[kFrameSamples];
                        MixFrame(mixed);
                        dump.Write(mixed, kFrameSamples);
                        playClock += std::chrono::milliseconds(20);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            mic.Close();
            speakers.Close();
            if (sock != INVALID_SOCKET)
                closesocket(sock);
            dump.Close();
            CoUninitialize();
        }
    }

    void StartMailRecording()
    {
        State& v = V();
        std::lock_guard lock(v.mutex);
        v.mailPlaying = false;
        v.mailRecording = true;
        v.mailHeard = false;
        v.mailRecorded.clear();
        v.mailEncoder = {};
        Log("Voice: recording voice mail");
    }

    void StartMailPlayback(std::vector<uint8_t> frames)
    {
        State& v = V();
        std::lock_guard lock(v.mutex);
        v.mailRecording = false;
        frames.resize(frames.size() / kPayloadBytes * kPayloadBytes);
        v.mailPlay = std::move(frames);
        v.mailPlayed = 0;
        v.mailPlaying = !v.mailPlay.empty();
        Log("Voice: playing voice mail ({} ms)", v.mailPlay.size() / kPayloadBytes * 20);
    }

    void StopMail()
    {
        State& v = V();
        std::lock_guard lock(v.mutex);
        if (v.mailRecording)
            Log("Voice: voice mail recorded ({} ms)", v.mailRecorded.size() / kPayloadBytes * 20);
        v.mailRecording = false;
        v.mailPlaying = false;
    }

    MailStatus Mail()
    {
        State& v = V();
        std::lock_guard lock(v.mutex);
        MailStatus m;
        m.recording = v.mailRecording;
        m.playing = v.mailPlaying;
        m.heard = v.mailHeard;
        if (v.mailPlaying)
        {
            m.frames = uint32_t(v.mailPlayed);
            m.total = uint32_t(v.mailPlay.size() / kPayloadBytes);
        }
        else
        {
            m.frames = uint32_t(v.mailRecorded.size() / kPayloadBytes);
            m.total = kMailFrames;
        }
        return m;
    }

    std::vector<uint8_t> MailRecording()
    {
        State& v = V();
        std::lock_guard lock(v.mutex);
        return v.mailRecorded;
    }

    void Start(const Settings& settings)
    {
        State& v = V();
        if (v.thread.joinable())
            return;
        v.settings = settings;
        v.stop = false;
        v.thread = std::thread(Run);
    }

    void Stop()
    {
        State& v = V();
        if (!v.thread.joinable())
            return;
        v.stop = true;
        v.thread.join();
    }

    std::vector<uint64_t> Talking()
    {
        State& v = V();
        std::lock_guard lock(v.mutex);
        std::vector<uint64_t> out;
        const auto now = std::chrono::steady_clock::now();
        for (const auto& [xuid, sp] : v.speakers)
            if (now - sp.lastLoud < std::chrono::milliseconds(300))
                out.push_back(xuid);
        return out;
    }

    std::vector<uint64_t> Headsets()
    {
        State& v = V();
        std::lock_guard lock(v.mutex);
        std::vector<uint64_t> out;
        const auto now = std::chrono::steady_clock::now();
        for (const auto& [xuid, sp] : v.speakers)
            if (now - sp.lastHeard < std::chrono::seconds(2))
                out.push_back(xuid);
        return out;
    }

    bool MicOpen()
    {
        return V().micOpen;
    }

    bool HasMicrophone()
    {
        return V().micPresent;
    }

    bool LocalTalking()
    {
        State& v = V();
        std::lock_guard lock(v.mutex);
        return !v.muted && std::chrono::steady_clock::now() - v.lastLocalLoud < std::chrono::milliseconds(300);
    }

    void SetMuted(bool muted)
    {
        V().muted = muted;
    }

    bool Muted()
    {
        return V().muted;
    }

    void SetGameChat(bool gameChat)
    {
        V().gameChat = gameChat;
    }

    bool GameChat()
    {
        return V().gameChat;
    }
}
