#include "audio/wav_dump.h"

#include "audio/host_output.h"
#include "log.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>

namespace audio
{
    namespace
    {
        std::mutex g_mutex;
        FILE* g_file = nullptr;
        uint64_t g_samples = 0;

        void Put16(uint8_t* p, uint16_t v) { std::memcpy(p, &v, 2); }
        void Put32(uint8_t* p, uint32_t v) { std::memcpy(p, &v, 4); }

        // RIFF/WAVE with WAVE_FORMAT_EXTENSIBLE, IEEE float, 5.1 (KSAUDIO_SPEAKER_5POINT1 = 0x3F).
        void WriteHeader()
        {
            uint8_t h[68] = {};
            const uint32_t dataBytes = uint32_t(std::min<uint64_t>(g_samples * kChannels * 4, 0xFFFFFF00u));
            std::memcpy(h, "RIFF", 4);
            Put32(h + 4, 60 + dataBytes);
            std::memcpy(h + 8, "WAVEfmt ", 8);
            Put32(h + 16, 40);
            Put16(h + 20, 0xFFFE);
            Put16(h + 22, kChannels);
            Put32(h + 24, kSampleRate);
            Put32(h + 28, kSampleRate * kChannels * 4);
            Put16(h + 32, kChannels * 4);
            Put16(h + 34, 32);
            Put16(h + 36, 22);
            Put16(h + 38, 32);
            Put32(h + 40, 0x3F);
            // KSDATAFORMAT_SUBTYPE_IEEE_FLOAT {00000003-0000-0010-8000-00AA00389B71}
            static const uint8_t kFloat[16] = { 3, 0, 0, 0, 0, 0, 0x10, 0, 0x80, 0, 0, 0xAA, 0, 0x38, 0x9B, 0x71 };
            std::memcpy(h + 44, kFloat, 16);
            std::memcpy(h + 60, "data", 4);
            Put32(h + 64, dataBytes);
            const long position = std::ftell(g_file);
            std::fseek(g_file, 0, SEEK_SET);
            std::fwrite(h, 1, sizeof(h), g_file);
            std::fseek(g_file, position, SEEK_SET);
            std::fflush(g_file);
        }

        const char* Path()
        {
            static const char* path = std::getenv("CB_AUDIO_DUMP");
            return path && *path ? path : nullptr;
        }
    }

    bool WavDumpEnabled()
    {
        return Path() != nullptr;
    }

    void DumpSamples(const float* interleaved, uint32_t samples)
    {
        if (!Path())
            return;
        std::lock_guard lock(g_mutex);
        if (!g_file)
        {
            g_file = std::fopen(Path(), "wb");
            if (!g_file)
                Fatal("CB_AUDIO_DUMP: cannot create {}", Path());
            WriteHeader();
            std::fseek(g_file, 68, SEEK_SET);
            Log("Audio: dumping the mixed output to {}", Path());
        }
        std::fwrite(interleaved, sizeof(float), size_t(samples) * kChannels, g_file);
        const uint64_t before = g_samples;
        g_samples += samples;
        if (before / kSampleRate != g_samples / kSampleRate)
            WriteHeader();
    }
}
