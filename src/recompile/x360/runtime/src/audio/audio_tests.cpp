// CB_AUDIO_TEST=1: XMA decoder tests. Transform self-test, then every analysis/audio_test/*.xma (XMA2 files made by
// tools/audio_test/make_cases.py with the XDK's xma2encode.exe) is decoded frame by frame and compared with the
// XDK tool's own decode of the same file (<name>.ref.wav, Microsoft's XMA2 decoder).

#include "audio/audio_tests.h"

#include "audio/apu.h"
#include "audio/xma_stream.h"
#include "memory.h"
#include "audio/wmapro.h"
#include "log.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <format>
#include <map>

namespace audio
{
    namespace
    {
        std::map<std::string, std::vector<uint8_t>> ReadRiff(const std::filesystem::path& path)
        {
            std::ifstream f(path, std::ios::binary);
            std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            std::map<std::string, std::vector<uint8_t>> chunks;
            for (size_t o = 12; o + 8 <= d.size();)
            {
                uint32_t size;
                std::memcpy(&size, &d[o + 4], 4);
                const size_t end = std::min(d.size(), o + 8 + size);
                chunks[std::string(reinterpret_cast<const char*>(&d[o]), 4)] = std::vector<uint8_t>(d.begin() + o + 8, d.begin() + end);
                o += 8 + size + (size & 1);
            }
            return chunks;
        }

        template<class T> T Le(const std::vector<uint8_t>& v, size_t o) { T x; std::memcpy(&x, &v[o], sizeof(T)); return x; }
    }

    int RunAudioTests()
    {
        int failures = 0;
        std::string report;
        failures += WmaProDecoder::TestTransform(report);
        Log("Audio test: transform\n{}", report);

        const std::filesystem::path dir = "analysis/audio_test";
        if (!std::filesystem::exists(dir))
        {
            Log("Audio test: {} not found (run tools/audio_test/make_cases.py); XMA cases skipped", dir.string());
            return failures;
        }
        guest::g_memory.Reserve();
        InitializeApu();
        int cases = 0, exact = 0, rounding = 0, contextFailures = 0;
        for (const auto& entry : std::filesystem::directory_iterator(dir))
        {
            if (entry.path().extension() != ".xma")
                continue;
            if (const char* only = std::getenv("CB_AUDIO_TEST_ONLY"); only && entry.path().stem().string().find(only) == std::string::npos)
                continue;
            cases++;
            const std::string name = entry.path().stem().string();
            auto xma = ReadRiff(entry.path());
            std::filesystem::path refPath = entry.path();
            refPath.replace_extension(".ref.wav");
            auto ref = ReadRiff(refPath);
            const auto& fmt = xma["fmt "];
            const int channels = Le<uint16_t>(fmt, 2);
            const int rate = int(Le<uint32_t>(fmt, 4));
            const int streams = Le<uint16_t>(fmt, 18);
            const uint32_t playBegin = Le<uint32_t>(fmt, 32);
            if (streams != 1)
            {
                Log("Audio test: {}: {} streams, skipped", name, streams);
                continue;
            }

            XmaStreamDecoder decoder;
            decoder.Initialize(channels, rate);
            std::vector<int16_t> decoded; // interleaved
            std::string error;
            std::vector<double> raw;
            if (!decoder.DecodeAll(xma["data"], decoded, error, &raw))
            {
                Log("Audio test: {}: FAILED to decode: {}", name, error);
                failures++;
                continue;
            }
            const auto& refData = ref["data"];
            const size_t refSamples = refData.size() / 2 / channels;
            const size_t ourSamples = decoded.size() / channels;
            // Find the alignment: the decoder's output carries the codec delay before the play region.
            size_t bestShift = 0;
            double bestError = 1e300;
            for (size_t shift = 0; shift <= 2048 && shift < ourSamples; shift++)
            {
                double err = 0;
                const size_t n = std::min<size_t>(refSamples, ourSamples - shift);
                for (size_t i = 0; i < n && i < 8192; i += 7)
                    for (int c = 0; c < channels; c++)
                        err += std::fabs(double(decoded[(shift + i) * channels + c]) - Le<int16_t>(refData, (i * channels + c) * 2));
                if (err < bestError)
                {
                    bestError = err;
                    bestShift = shift;
                }
            }
            size_t compared = 0, equal = 0;
            int maxDiff = 0;
            double nearestHalf = 1.0, farthestMismatch = 0.0; // |frac - 0.5| over mismatches
            int above = 0, below = 0;
            double squared = 0;
            const size_t n = std::min<size_t>(refSamples, ourSamples - bestShift);
            for (size_t i = 0; i < n; i++)
                for (int c = 0; c < channels; c++)
                {
                    const int a = decoded[(bestShift + i) * channels + c];
                    const int b = Le<int16_t>(refData, (i * channels + c) * 2);
                    compared++;
                    equal += a == b;
                    if (a != b)
                    {
                        const double r = raw[(bestShift + i) * channels + c];
                        const double d = std::fabs(r - std::floor(r) - 0.5);
                        farthestMismatch = std::max(farthestMismatch, d);
                        nearestHalf = std::min(nearestHalf, d);
                        (b > a ? above : below)++;
                    }
                    maxDiff = std::max(maxDiff, std::abs(a - b));
                    squared += double(a - b) * (a - b);
                }
            const bool same = equal == compared && n == refSamples;
            exact += same;
            // Microsoft's decoder computes in lower precision: values within 0.02 of a rounding boundary may round
            // the other way. Anything else is a decoder error.
            const bool roundingOnly = same || (maxDiff <= 1 && farthestMismatch < 0.02);
            rounding += roundingOnly;
            if (!roundingOnly)
                failures++;
            std::string contextReport;
            const int contextFailed = TestApuStream(xma["data"], channels, rate, contextReport);
            contextFailures += contextFailed;
            failures += contextFailed;
            Log("Audio test: {}: {} ch {} Hz, play begin {}, decoded {} samples, reference {}; aligned at +{}: {} of {} "
                "values equal, max |diff| {}, rms {:.3f}{}", name, channels, rate, playBegin, ourSamples, refSamples,
                bestShift, equal, compared, maxDiff, std::sqrt(squared / std::max<size_t>(compared, 1)),
                same ? "" : std::format("; mismatches lie within {:.4f} of x.5 (reference higher {} / lower {}){}",
                    farthestMismatch, above, below, roundingOnly ? "" : "  FAILED"));
            Log("Audio test: {}: through an XMA context: {}", name, contextReport);
            std::string loopReport;
            const int loopFailed = TestApuLoop(xma["data"], channels, rate, loopReport);
            contextFailures += loopFailed;
            failures += loopFailed;
            Log("Audio test: {}: hardware loop: {}", name, loopReport);
        }
        Log("Audio test: {} XMA case(s): {} bit-exact against the XDK decoder, {} equal up to rounding of values within "
            "0.02 of x.5; {} context stream failure(s); {} failure(s) in total", cases, exact, rounding, contextFailures,
            failures);
        return failures;
    }
}
