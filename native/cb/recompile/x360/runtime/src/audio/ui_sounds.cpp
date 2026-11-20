#include "audio/ui_sounds.h"

#include "audio/apu.h"
#include "audio/host_output.h"
#include "audio/xma_stream.h"
#include "log.h"
#include "xui/host.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace audio
{
    namespace
    {
        // A decoded sound at the output rate: one or two channels, interleaved, nominal range -1..1.
        struct Clip
        {
            int channels = 1;
            std::vector<float> samples;
        };

        struct Voice
        {
            std::shared_ptr<const Clip> clip;
            size_t position = 0; // frames played
            float gain = 1.0f;
            const void* source = nullptr;
        };

        std::mutex g_clipsMutex;
        std::map<const uint8_t*, std::shared_ptr<const Clip>> g_clips; // by file data (null: undecodable)
        std::mutex g_voicesMutex;
        std::vector<Voice> g_voices;

        template<class T> T Le(const uint8_t* p)
        {
            T v;
            std::memcpy(&v, p, sizeof(T));
            return v;
        }

        // RIFF WAVE with an XMA format: XMAWAVEFORMAT (0x165: tag, bits, encode options, largest skip, stream count,
        // loop count, version, then per stream { bytes/s, sample rate, loop start, loop end, subframe data,
        // channels, channel mask }) or XMA2WAVEFORMATEX (0x166: WAVEFORMATEX channels / rate).
        std::shared_ptr<const Clip> Decode(const uint8_t* data, size_t size, std::string_view name)
        {
            if (size < 12 || std::memcmp(data, "RIFF", 4) != 0 || std::memcmp(data + 8, "WAVE", 4) != 0)
            {
                Log("UI sound {}: not a RIFF wave", name);
                return nullptr;
            }
            const uint8_t* fmt = nullptr;
            size_t fmtSize = 0;
            std::vector<uint8_t> packets;
            for (size_t o = 12; o + 8 <= size;)
            {
                const uint32_t chunk = Le<uint32_t>(data + o + 4);
                const size_t body = std::min<size_t>(chunk, size - o - 8);
                if (std::memcmp(data + o, "fmt ", 4) == 0)
                {
                    fmt = data + o + 8;
                    fmtSize = body;
                }
                else if (std::memcmp(data + o, "data", 4) == 0)
                    packets.assign(data + o + 8, data + o + 8 + body);
                o += 8 + size_t(chunk) + (chunk & 1);
            }
            if (!fmt || fmtSize < 18)
            {
                Log("UI sound {}: no format chunk", name);
                return nullptr;
            }
            const uint16_t tag = Le<uint16_t>(fmt);
            int channels = 0, rate = 0;
            bool xma1 = false;
            if (tag == 0x165 && fmtSize >= 32 && Le<uint16_t>(fmt + 8) == 1)
            {
                rate = int(Le<uint32_t>(fmt + 16));
                channels = fmt[29];
                xma1 = true;
            }
            else if (tag == 0x166 && fmtSize >= 34 && Le<uint16_t>(fmt + 18) == 1)
            {
                channels = Le<uint16_t>(fmt + 2);
                rate = int(Le<uint32_t>(fmt + 4));
            }
            if (channels < 1 || channels > 2 || rate <= 0)
            {
                Log("UI sound {}: unsupported format {:04X} ({} channels, {} Hz)", name, tag, channels, rate);
                return nullptr;
            }
            XmaStreamDecoder decoder;
            decoder.Initialize(channels, rate);
            std::vector<int16_t> pcm;
            std::string error;
            if (!(xma1 ? decoder.DecodeXma1(packets, pcm, error) : decoder.DecodeAll(packets, pcm, error)))
            {
                Log("UI sound {}: {}", name, error);
                return nullptr;
            }
            // Linear resampling to the output rate.
            auto clip = std::make_shared<Clip>();
            clip->channels = channels;
            const size_t inFrames = pcm.size() / channels;
            const double step = double(rate) / kSampleRate;
            const size_t outFrames = inFrames ? size_t(double(inFrames - 1) / step) + 1 : 0;
            clip->samples.resize(outFrames * channels);
            for (size_t i = 0; i < outFrames; i++)
            {
                const double t = double(i) * step;
                const size_t k = size_t(t);
                const float f = float(t - double(k));
                for (int c = 0; c < channels; c++)
                {
                    const float a = pcm[k * channels + c] / 32768.0f;
                    const float b = k + 1 < inFrames ? pcm[(k + 1) * channels + c] / 32768.0f : a;
                    clip->samples[i * channels + c] = a + (b - a) * f;
                }
            }
            Log("UI sound {}: {} Hz, {} channel(s), {} ms", name, rate, channels, outFrames * 1000 / kSampleRate);
            return clip;
        }

        void Play(const xui::host::Sound& sound)
        {
            std::shared_ptr<const Clip> clip;
            {
                std::lock_guard lock(g_clipsMutex);
                auto it = g_clips.find(sound.data);
                if (it == g_clips.end())
                    it = g_clips.emplace(sound.data, Decode(sound.data, sound.size, sound.name)).first;
                clip = it->second;
            }
            if (!clip || clip->samples.empty())
                return;
            static const bool trace = std::getenv("CB_UI_SOUND_LOG") != nullptr;
            if (trace)
                Log("UI sound: {} ({} dB) at render tick {}", sound.name, sound.volumeDb, RenderTicks());
            StartRenderClock();
            Voice voice{ clip, 0, std::pow(10.0f, sound.volumeDb / 20.0f), sound.source };
            std::lock_guard lock(g_voicesMutex);
            // XuiSoundXAudio (dash 0x921DC338 -> 0x921DC0C8): the element's previous voice is retired, not cut: it
            // plays on to its end while the new one starts. (XUI stops an element's current voice when the element
            // is destroyed, 0x921DC2A0; ours free scenes as soon as they are replaced, before XUI would, so every
            // sound plays to its end instead.)
            g_voices.push_back(voice);
        }
    }

    void InitializeUiSounds()
    {
        xui::host::SetSoundPlayer(Play);
    }

    void MixUiSounds(float* mix, uint32_t samples)
    {
        std::lock_guard lock(g_voicesMutex);
        for (Voice& v : g_voices)
        {
            const Clip& clip = *v.clip;
            const size_t frames = clip.samples.size() / clip.channels;
            for (uint32_t i = 0; i < samples && v.position < frames; i++, v.position++)
            {
                // A mono sound goes to FL FR FC BL BR at full level, as the dash's own XUI sounds reach the 5.1 mix
                // (measured in CB_AUDIO_DUMP: equal on those five, none on LFE; XAM's Guide is the same XUI); a
                // stereo one to FL / FR.
                float* out = &mix[size_t(i) * kChannels];
                const float l = clip.samples[v.position * clip.channels] * v.gain;
                if (clip.channels == 2)
                {
                    out[0] += l;
                    out[1] += clip.samples[v.position * 2 + 1] * v.gain;
                    continue;
                }
                out[0] += l;
                out[1] += l;
                out[2] += l;
                out[4] += l;
                out[5] += l;
            }
        }
        std::erase_if(g_voices, [](const Voice& v) { return v.position >= v.clip->samples.size() / v.clip->channels; });
    }
}
