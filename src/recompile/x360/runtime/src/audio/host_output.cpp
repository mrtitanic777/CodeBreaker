// WASAPI shared-mode output of the console's 5.1 mix (see host_output.h).

#include "audio/host_output.h"

#include "gpu/gpu.h"
#include "log.h"

#include <windows.h>
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <ksmedia.h>

#include <array>
#include <atomic>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <format>
#include <mutex>
#include <thread>
#include <vector>

namespace audio
{
    namespace
    {
        // Source speakers in render-driver order.
        constexpr std::array<DWORD, kChannels> kSourceSpeakers = {
            SPEAKER_FRONT_LEFT, SPEAKER_FRONT_RIGHT, SPEAKER_FRONT_CENTER,
            SPEAKER_LOW_FREQUENCY, SPEAKER_BACK_LEFT, SPEAKER_BACK_RIGHT,
        };
        constexpr uint32_t kQueueCapacity = kSampleRate / 2; // samples (0.5 s)

        struct Output
        {
            IAudioClient* client = nullptr;
            IAudioRenderClient* render = nullptr;
            HANDLE event = nullptr;
            uint32_t deviceChannels = 0;
            DWORD deviceMask = 0;
            uint32_t deviceRate = 0;
            uint32_t bufferFrames = 0;
            // matrix[device channel][source channel]
            std::vector<std::array<float, kChannels>> matrix;
            std::string mapping;
        };

        Output g_output;
        std::atomic<bool> g_active = false;
        std::mutex g_queueMutex;
        std::vector<float> g_queue = std::vector<float>(size_t(kQueueCapacity) * kChannels);
        uint32_t g_queueRead = 0, g_queueCount = 0; // in samples
        uint64_t g_dropped = 0;
        uint64_t g_underrun = 0;   // device samples played as silence because the queue ran dry (after the start)
        uint64_t g_played = 0;

        std::string SpeakerName(DWORD speaker)
        {
            switch (speaker)
            {
            case SPEAKER_FRONT_LEFT: return "FL";
            case SPEAKER_FRONT_RIGHT: return "FR";
            case SPEAKER_FRONT_CENTER: return "FC";
            case SPEAKER_LOW_FREQUENCY: return "LFE";
            case SPEAKER_BACK_LEFT: return "BL";
            case SPEAKER_BACK_RIGHT: return "BR";
            case SPEAKER_SIDE_LEFT: return "SL";
            case SPEAKER_SIDE_RIGHT: return "SR";
            case SPEAKER_BACK_CENTER: return "BC";
            default: return std::format("0x{:X}", speaker);
            }
        }

        // Maps the 5.1 source onto the device's speakers. Speakers the device has are passed through unchanged (back
        // channels go to the side speakers when the device has no back pair). Missing speakers are folded down with
        // the ITU-R BS.775 coefficients (centre and surround at -3 dB into the front pair, LFE dropped).
        void BuildMatrix(Output& o)
        {
            std::vector<DWORD> speakers;
            for (DWORD bit = 1; bit != 0 && speakers.size() < o.deviceChannels; bit <<= 1)
                if (o.deviceMask & bit)
                    speakers.push_back(bit);
            while (speakers.size() < o.deviceChannels)
                speakers.push_back(0); // channels without a position stay silent
            o.matrix.assign(o.deviceChannels, {});
            auto index = [&](DWORD speaker) -> int {
                for (size_t i = 0; i < speakers.size(); i++)
                    if (speakers[i] == speaker)
                        return int(i);
                return -1;
            };
            constexpr float kMinus3dB = 0.70710678f;
            const bool mono = o.deviceChannels == 1;
            for (uint32_t source = 0; source < kChannels; source++)
            {
                DWORD speaker = kSourceSpeakers[source];
                int target = index(speaker);
                if (target < 0 && speaker == SPEAKER_BACK_LEFT)
                    target = index(SPEAKER_SIDE_LEFT);
                if (target < 0 && speaker == SPEAKER_BACK_RIGHT)
                    target = index(SPEAKER_SIDE_RIGHT);
                if (target >= 0)
                {
                    o.matrix[target][source] = 1.0f;
                    continue;
                }
                if (speaker == SPEAKER_LOW_FREQUENCY)
                    continue;
                const int left = index(SPEAKER_FRONT_LEFT), right = index(SPEAKER_FRONT_RIGHT);
                const int center = mono ? 0 : -1;
                if (center >= 0)
                {
                    o.matrix[center][source] += speaker == SPEAKER_FRONT_CENTER ? 1.0f : kMinus3dB;
                    continue;
                }
                const bool toLeft = speaker == SPEAKER_FRONT_CENTER || speaker == SPEAKER_BACK_LEFT;
                const bool toRight = speaker == SPEAKER_FRONT_CENTER || speaker == SPEAKER_BACK_RIGHT;
                if (toLeft && left >= 0)
                    o.matrix[left][source] += kMinus3dB;
                if (toRight && right >= 0)
                    o.matrix[right][source] += kMinus3dB;
            }
            for (uint32_t d = 0; d < o.deviceChannels; d++)
            {
                std::string terms;
                for (uint32_t s = 0; s < kChannels; s++)
                    if (o.matrix[d][s] != 0.0f)
                        terms += std::format("{}{}{}", terms.empty() ? "" : "+",
                            o.matrix[d][s] == 1.0f ? "" : std::format("{:.3f}*", o.matrix[d][s]), SpeakerName(kSourceSpeakers[s]));
                o.mapping += std::format("{}{}={}", d ? " " : "", speakers[d] ? SpeakerName(speakers[d]) : "?",
                    terms.empty() ? "0" : terms);
            }
        }

        void RenderThread()
        {
            SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
            Output& o = g_output;
            std::vector<float> source;
            for (;;)
            {
                WaitForSingleObject(o.event, INFINITE);
                UINT32 padding = 0;
                if (FAILED(o.client->GetCurrentPadding(&padding)))
                    continue;
                const uint32_t frames = o.bufferFrames - padding;
                if (frames == 0)
                    continue;
                BYTE* data = nullptr;
                if (FAILED(o.render->GetBuffer(frames, &data)))
                    continue;
                // Underruns play silence for the missing samples.
                source.assign(size_t(frames) * kChannels, 0.0f);
                {
                    std::lock_guard lock(g_queueMutex);
                    const uint32_t take = std::min(frames, g_queueCount);
                    for (uint32_t i = 0; i < take; i++)
                    {
                        const uint32_t slot = (g_queueRead + i) % kQueueCapacity;
                        std::memcpy(&source[size_t(i) * kChannels], &g_queue[size_t(slot) * kChannels], kChannels * sizeof(float));
                    }
                    g_queueRead = (g_queueRead + take) % kQueueCapacity;
                    g_queueCount -= take;
                    if (g_played > 0 && take < frames)
                    {
                        // Every gap is a click: logged with its time (the first 200).
                        static uint32_t logged = 0;
                        if (logged++ < 200)
                            Log("Audio: host underrun at {:.3f} s of output: {} samples of silence", double(g_played) / kSampleRate,
                                frames - take);
                        g_underrun += frames - take;
                    }
                    // The first sound (the boot animation's): when it reaches the device, on the frames' clock (its
                    // picture's sync).
                    static bool heard = false;
                    for (uint32_t i = 0; !heard && i < take * kChannels; i++)
                        if (std::abs(source[i]) > 0.001f)
                        {
                            heard = true;
                            Log("Audio: first sound at {:.3f} s of output (vblank {}, frame {})",
                                double(g_played + i / kChannels) / kSampleRate, gpu::VblankCount(), gpu::SwapCount());
                        }
                    g_played += take;
                    // Every 10 s of output: queue level, silence inserted, samples dropped.
                    static uint64_t nextReport = 10 * kSampleRate;
                    if (g_played >= nextReport)
                    {
                        nextReport += 10 * kSampleRate;
                        Log("Audio: host output: {} samples queued, {} underrun and {} dropped samples so far",
                            g_queueCount, g_underrun, g_dropped);
                    }
                }
                float* out = reinterpret_cast<float*>(data);
                for (uint32_t i = 0; i < frames; i++)
                {
                    const float* in = &source[size_t(i) * kChannels];
                    for (uint32_t d = 0; d < o.deviceChannels; d++)
                    {
                        float v = 0.0f;
                        for (uint32_t s = 0; s < kChannels; s++)
                            v += o.matrix[d][s] * in[s];
                        out[size_t(i) * o.deviceChannels + d] = v;
                    }
                }
                o.render->ReleaseBuffer(frames, 0);
            }
        }

        bool Fail(const char* what, HRESULT hr)
        {
            Log("Audio: no host output ({} failed, 0x{:08X}); the audio clock runs from a timer", what, uint32_t(hr));
            return false;
        }
    }

    bool StartHostOutput()
    {
        // CB_AUDIO=0 never opens a device; CB_AUDIO=1 opens one even in headless runs.
        const char* setting = std::getenv("CB_AUDIO");
        const char* headless = std::getenv("CB_HEADLESS");
        const bool forced = setting && setting[0] == '1';
        if ((setting && setting[0] == '0') || (!forced && headless && headless[0] == '1'))
        {
            Log("Audio: host output off ({}); the audio clock runs from a timer", setting && setting[0] == '0' ? "CB_AUDIO=0" : "headless");
            return false;
        }
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
            return Fail("CoInitializeEx", hr);
        IMMDeviceEnumerator* enumerator = nullptr;
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&enumerator));
        if (FAILED(hr))
            return Fail("MMDeviceEnumerator", hr);
        IMMDevice* device = nullptr;
        hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
        enumerator->Release();
        if (FAILED(hr))
            return Fail("GetDefaultAudioEndpoint", hr);
        Output& o = g_output;
        hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&o.client));
        device->Release();
        if (FAILED(hr))
            return Fail("IAudioClient", hr);
        WAVEFORMATEX* mix = nullptr;
        hr = o.client->GetMixFormat(&mix);
        if (FAILED(hr))
            return Fail("GetMixFormat", hr);
        o.deviceChannels = mix->nChannels;
        o.deviceRate = mix->nSamplesPerSec;
        o.deviceMask = mix->wFormatTag == WAVE_FORMAT_EXTENSIBLE ? reinterpret_cast<WAVEFORMATEXTENSIBLE*>(mix)->dwChannelMask
                     : o.deviceChannels == 1 ? SPEAKER_FRONT_CENTER : SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
        CoTaskMemFree(mix);

        // 48 kHz float in the device's channel layout; Windows resamples to the device rate when they differ.
        WAVEFORMATEXTENSIBLE format{};
        format.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
        format.Format.nChannels = WORD(o.deviceChannels);
        format.Format.nSamplesPerSec = kSampleRate;
        format.Format.wBitsPerSample = 32;
        format.Format.nBlockAlign = WORD(o.deviceChannels * 4);
        format.Format.nAvgBytesPerSec = kSampleRate * format.Format.nBlockAlign;
        format.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
        format.Samples.wValidBitsPerSample = 32;
        format.dwChannelMask = o.deviceMask;
        format.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
        constexpr REFERENCE_TIME kBufferDuration = 20 * 10000; // 20 ms device buffer
        hr = o.client->Initialize(AUDCLNT_SHAREMODE_SHARED,
            AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
            kBufferDuration, 0, &format.Format, nullptr);
        if (FAILED(hr))
            return Fail("IAudioClient::Initialize", hr);
        o.event = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        UINT32 bufferFrames = 0;
        if (FAILED(hr = o.client->SetEventHandle(o.event)) || FAILED(hr = o.client->GetBufferSize(&bufferFrames)))
            return Fail("SetEventHandle/GetBufferSize", hr);
        o.bufferFrames = bufferFrames;
        hr = o.client->GetService(IID_PPV_ARGS(&o.render));
        if (FAILED(hr))
            return Fail("IAudioRenderClient", hr);
        BuildMatrix(o);
        // CB_AUDIO_MUTE=1 (tests): the stream runs with the device's timing but this session is muted in the Windows
        // mixer, so measurements do not play through the speakers. Nothing else changes.
        if (const char* mute = std::getenv("CB_AUDIO_MUTE"); mute && mute[0] == '1')
        {
            ISimpleAudioVolume* volume = nullptr;
            if (SUCCEEDED(o.client->GetService(IID_PPV_ARGS(&volume))))
            {
                volume->SetMute(TRUE, nullptr);
                volume->Release();
                Log("Audio: host output muted (CB_AUDIO_MUTE=1)");
            }
        }
        std::thread(RenderThread).detach();
        hr = o.client->Start();
        if (FAILED(hr))
            return Fail("IAudioClient::Start", hr);
        g_active = true;
        Log("Audio: host output {}", DescribeHostOutput());
        return true;
    }

    bool HostOutputActive()
    {
        return g_active;
    }

    void QueueHostSamples(const float* interleaved, uint32_t samples)
    {
        if (!g_active)
            return;
        std::lock_guard lock(g_queueMutex);
        for (uint32_t i = 0; i < samples; i++)
        {
            if (g_queueCount == kQueueCapacity)
            {
                static uint32_t logged = 0;
                if (logged++ < 200)
                    Log("Audio: host queue full at {:.3f} s of output: {} samples dropped", double(g_played) / kSampleRate,
                        samples - i);
                g_dropped += samples - i;
                return;
            }
            const uint32_t slot = (g_queueRead + g_queueCount) % kQueueCapacity;
            std::memcpy(&g_queue[size_t(slot) * kChannels], interleaved + size_t(i) * kChannels, kChannels * sizeof(float));
            g_queueCount++;
        }
    }

    uint32_t QueuedHostSamples()
    {
        std::lock_guard lock(g_queueMutex);
        return g_queueCount;
    }

    std::string DescribeHostOutput()
    {
        const Output& o = g_output;
        return std::format("{} channel(s) (mask 0x{:X}), device rate {} Hz (48 kHz converted by Windows: {}), buffer {} "
            "samples; mapping {}", o.deviceChannels, o.deviceMask, o.deviceRate, o.deviceRate != kSampleRate ? "yes" : "no",
            o.bufferFrames, o.mapping);
    }
}
