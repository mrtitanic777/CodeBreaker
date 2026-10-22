#include "audio/xma_stream.h"

#include <algorithm>
#include <cmath>
#include <format>

namespace audio
{
    namespace
    {
        constexpr uint32_t kPacketBytes = 2048;
        constexpr uint32_t kPayloadBits = (kPacketBytes - 4) * 8;
    }

    void XmaStreamDecoder::Initialize(int channels, int sampleRate)
    {
        decoder_.Initialize(channels, sampleRate);
    }

    int16_t XmaStreamDecoder::ToPcm16(double sample)
    {
        const double v = std::nearbyint(sample * 32768.0);
        return int16_t(std::clamp(v, -32768.0, 32767.0));
    }

    bool XmaStreamDecoder::DecodeAll(const std::vector<uint8_t>& packets, std::vector<int16_t>& out, std::string& error,
        std::vector<double>* raw, std::vector<uint32_t>* frameStarts)
    {
        const size_t count = packets.size() / kPacketBytes;
        std::vector<uint8_t> payload;
        payload.reserve(count * (kPacketBytes - 4));
        for (size_t k = 0; k < count; k++)
            payload.insert(payload.end(), packets.begin() + k * kPacketBytes + 4, packets.begin() + (k + 1) * kPacketBytes);
        const uint64_t totalBits = uint64_t(payload.size()) * 8;
        double pcm[WmaProDecoder::kMaxChannels][WmaProDecoder::kFrameSamples];
        const int channels = decoder_.channels();
        for (size_t k = 0; k < count; k++)
        {
            const uint8_t* h = &packets[k * kPacketBytes];
            const uint32_t header = uint32_t(h[0]) << 24 | uint32_t(h[1]) << 16 | uint32_t(h[2]) << 8 | h[3];
            const uint32_t frames = header >> 26;
            const uint32_t offset = (header >> 11) & 0x7FFF;
            uint64_t position = uint64_t(k) * kPayloadBits + offset;
            for (uint32_t f = 0; f < frames; f++)
            {
                if (position + 15 > totalBits)
                    return true; // truncated final frame
                BitReader reader(payload.data(), position, totalBits);
                const uint32_t length = reader.Peek(15);
                if (position + length > totalBits)
                    return true;
                if (frameStarts)
                    frameStarts->push_back(uint32_t(position / kPayloadBits * kPacketBytes * 8 + 32 + position % kPayloadBits));
                WmaProFrameInfo info;
                if (!decoder_.DecodeFrame(reader, info, pcm))
                {
                    error = std::format("packet {} frame {} (bit {}): {}", k, f, position, decoder_.error());
                    return false;
                }
                for (int i = 0; i < WmaProDecoder::kFrameSamples; i++)
                    for (int c = 0; c < channels; c++)
                    {
                        out.push_back(ToPcm16(pcm[c][i]));
                        if (raw)
                            raw->push_back(pcm[c][i] * 32768.0);
                    }
                position += length;
            }
        }
        return true;
    }

    bool XmaStreamDecoder::DecodeXma1(const std::vector<uint8_t>& packets, std::vector<int16_t>& out, std::string& error)
    {
        const size_t count = packets.size() / kPacketBytes;
        if (count == 0)
            return true;
        std::vector<uint8_t> payload;
        payload.reserve(count * (kPacketBytes - 4));
        for (size_t k = 0; k < count; k++)
            payload.insert(payload.end(), packets.begin() + k * kPacketBytes + 4, packets.begin() + (k + 1) * kPacketBytes);
        const uint64_t totalBits = uint64_t(payload.size()) * 8;
        const uint32_t header = uint32_t(packets[0]) << 24 | uint32_t(packets[1]) << 16 | uint32_t(packets[2]) << 8 | packets[3];
        uint64_t position = (header >> 11) & 0x7FFF;
        double pcm[WmaProDecoder::kMaxChannels][WmaProDecoder::kFrameSamples];
        const int channels = decoder_.channels();
        while (position + 15 <= totalBits)
        {
            BitReader reader(payload.data(), position, totalBits);
            const uint32_t length = reader.Peek(15);
            if (length == 0x7FFF || length <= 15 || position + length > totalBits)
                break;
            WmaProFrameInfo info;
            if (!decoder_.DecodeFrame(reader, info, pcm))
            {
                error = std::format("frame at bit {}: {}", position, decoder_.error());
                return false;
            }
            for (int i = 0; i < WmaProDecoder::kFrameSamples; i++)
                for (int c = 0; c < channels; c++)
                    out.push_back(ToPcm16(pcm[c][i]));
            position += length;
        }
        return true;
    }
}
