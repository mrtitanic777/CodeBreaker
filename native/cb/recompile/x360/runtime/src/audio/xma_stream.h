#pragma once

#include "audio/wmapro.h"

#include <cstdint>
#include <string>
#include <vector>

// Decodes a whole single-stream XMA2 packet sequence (2048-byte packets, each starting with a 32-bit header:
// frame count 6 bits, bit offset of the first frame that starts in the packet 15 bits, metadata 3 bits, packet
// skip 8 bits). Frames are continuous across the packets' payloads. Used by the decoder tests.
namespace audio
{
    class XmaStreamDecoder
    {
    public:
        void Initialize(int channels, int sampleRate);
        // 16-bit interleaved output of every frame.
        // frameStarts: each decoded frame's start as a bit offset in the packet sequence (packet headers included).
        bool DecodeAll(const std::vector<uint8_t>& packets, std::vector<int16_t>& out, std::string& error,
                       std::vector<double>* raw = nullptr, std::vector<uint32_t>* frameStarts = nullptr);

        // XMA1 (WAVE_FORMAT_XMA 0x165, the system UI's sounds): the packet header is a 4-bit sequence number, 2 bits,
        // the 15-bit offset of the first frame and 11 bits of packet skip; there is no frame count, so a single
        // stream's frames are walked by their length fields across the payloads until the data ends (or a length
        // of 0x7FFF, the padding of the last packet).
        bool DecodeXma1(const std::vector<uint8_t>& packets, std::vector<int16_t>& out, std::string& error);

        // The hardware's float -> 16-bit conversion of a decoded sample.
        static int16_t ToPcm16(double sample);

    private:
        WmaProDecoder decoder_;
    };
}
