#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

// WMA Pro frame decoder with the parameters XMA uses (the Xbox 360 XMA hardware decodes a WMA Pro bitstream):
// 512 samples per frame per channel, up to 4 subframes of 512/256/128 samples, 1 or 2 channels per stream, frames
// prefixed with a 15-bit length, dynamic range control byte present, 16-bit sample scale.
//
// Written from the WMA Pro bitstream description (analysis/audio_spec.md); the codebooks are generated from FFmpeg's
// tables (wmapro_tables.inc). Arithmetic is double precision; the caller converts to the hardware's 16-bit output.
namespace audio
{
    class BitReader
    {
    public:
        BitReader() = default;
        BitReader(const uint8_t* data, uint64_t bitOffset, uint64_t bitEnd) : data_(data), pos_(bitOffset), end_(bitEnd) {}
        uint32_t Read(uint32_t bits);  // up to 32 bits, MSB first; past the end reads zeros and sets Overrun()
        uint32_t Peek(uint32_t bits) const;
        bool Read1() { return Read(1) != 0; }
        int32_t ReadSigned(uint32_t bits);
        void Skip(uint64_t bits) { pos_ += bits; }
        void SetEnd(uint64_t end) { end_ = end; }
        uint64_t Position() const { return pos_; }
        uint64_t End() const { return end_; }
        int64_t Left() const { return int64_t(end_) - int64_t(pos_); }
        bool Overrun() const { return pos_ > end_; }

    private:
        const uint8_t* data_ = nullptr;
        uint64_t pos_ = 0, end_ = 0;
    };

    struct WmaProFrameInfo
    {
        uint32_t lengthBits = 0;   // the frame's length field (bits, including the field itself)
        bool moreFrames = false;   // trailer bit
        uint8_t drcGain = 0;
        uint16_t trimStart = 0, trimEnd = 0; // samples to skip (frame header extension), 0 when absent
    };

    class WmaProDecoder
    {
    public:
        static constexpr int kFrameSamples = 512;
        static constexpr int kMaxChannels = 2;

        // sampleRate: the stream's rate (24000, 32000, 44100 or 48000 in XMA); it selects the scale factor bands.
        void Initialize(int channels, int sampleRate);
        // Forgets the overlap with the previous frame (the next frame's first half overlaps silence).
        void Reset();
        // Decodes one frame whose bits start at `bits` (at its 15-bit length field). On success the frame's 512
        // samples per channel are in output[c] (nominal range -1..1). False on a malformed frame (error() says why).
        bool DecodeFrame(BitReader bits, WmaProFrameInfo& info, double output[kMaxChannels][kFrameSamples]);
        const std::string& error() const { return error_; }
        int channels() const { return channels_; }

        // Self-test of the transform (fast DCT-IV against the definition); returns failures.
        static int TestTransform(std::string& report);

    private:
        static constexpr int kBlockSizes = 3; // 512, 256, 128
        static constexpr int kMaxBands = 29;
        static constexpr int kMaxSubframes = 32;

        struct Channel
        {
            int prevBlockLength = kFrameSamples;
            int numSubframes = 0;
            std::array<int, kMaxSubframes> subframeLength{};
            int curSubframe = 0;
            int decodedSamples = 0;
            bool grouped = false;
            bool transmitCoefficients = false;
            int quantStep = 0;
            bool reuseScaleFactors = false;
            int scaleFactorStep = 1;
            int maxScaleFactor = 0;
            int savedScaleFactors[2][kMaxBands] = {};
            int scaleFactorIndex = 0;
            int* scaleFactors = nullptr;
            int tableIndex = 0;
            int numVectorCoefficients = 0;
            double* coefficients = nullptr;
            // Output with overlap: the frame's samples are [0, 512); the next frame's overlap follows.
            std::array<double, kFrameSamples + kFrameSamples / 2> out{};
        };

        struct ChannelGroup
        {
            int numChannels = 0;
            bool transform = false;
            std::array<bool, kMaxBands> transformBand{};
            double matrix[4] = {};
            double* data[kMaxChannels] = {};
        };

        bool Fail(const char* what);
        bool DecodeTileHeader(BitReader& b);
        int DecodeSubframeLength(BitReader& b, int offset);
        bool DecodeSubframe(BitReader& b);
        bool DecodeChannelTransform(BitReader& b);
        bool DecodeScaleFactors(BitReader& b);
        bool DecodeCoefficients(BitReader& b, Channel& c);
        void InverseChannelTransform();
        void Window();
        void InverseMdct(const double* input, double* output, int length);

        int channels_ = 1;
        int sampleRate_ = 48000;
        int numBands_[kBlockSizes] = {};
        int bandOffsets_[kBlockSizes][kMaxBands + 1] = {};
        int scaleFactorOffsets_[kBlockSizes][kBlockSizes][kMaxBands] = {};
        std::array<Channel, kMaxChannels> channel_{};
        std::array<ChannelGroup, kMaxChannels> group_{};
        int numGroups_ = 0;
        int subframeLength_ = 0;
        int tableIndex_ = 0;
        int numBandsCur_ = 0;
        int escapeLength_ = 0;
        bool transmitNumVectorCoefficients_ = false;
        int channelsForSubframe_ = 0;
        int channelIndexes_[kMaxChannels] = {};
        bool parsedAllSubframes_ = false;
        std::array<double, kFrameSamples> scratch_{};
        std::string error_;
    };
}
