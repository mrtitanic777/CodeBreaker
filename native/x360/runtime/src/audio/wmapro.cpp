// WMA Pro frame decoding for XMA (see wmapro.h).

#include "audio/wmapro.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <format>
#include <numbers>

namespace audio
{
    namespace
    {
        struct HuffmanEntry
        {
            uint32_t code;
            uint8_t length;
            int16_t symbol;
        };

#include "audio/wmapro_tables.inc"

        // Binary trie over a complete prefix code; decoded bit by bit, MSB first.
        class Codebook
        {
        public:
            template<size_t N>
            explicit Codebook(const HuffmanEntry (&entries)[N])
            {
                nodes_.push_back({ 0, 0 });
                for (const HuffmanEntry& e : entries)
                {
                    int node = 0;
                    for (int bit = e.length - 1; bit >= 0; bit--)
                    {
                        const int side = (e.code >> bit) & 1;
                        if (bit == 0)
                        {
                            nodes_[node][side] = -1 - int(symbols_.size());
                            symbols_.push_back(e.symbol);
                            break;
                        }
                        if (nodes_[node][side] == 0)
                        {
                            nodes_[node][side] = int(nodes_.size());
                            nodes_.push_back({ 0, 0 });
                        }
                        node = nodes_[node][side];
                    }
                }
            }

            int Decode(BitReader& b) const
            {
                int node = 0;
                for (;;)
                {
                    const int next = nodes_[node][b.Read1() ? 1 : 0];
                    if (next < 0)
                        return symbols_[-1 - next];
                    if (next == 0)
                        return symbols_.empty() ? 0 : symbols_[0]; // unreachable for complete codes
                    node = next;
                }
            }

        private:
            std::vector<std::array<int, 2>> nodes_;
            std::vector<int> symbols_;
        };

        const Codebook& ScaleCodes() { static const Codebook c(kScaleCodes); return c; }
        const Codebook& ScaleRlCodes() { static const Codebook c(kScaleRlCodes); return c; }
        const Codebook& Coef0Codes() { static const Codebook c(kCoef0Codes); return c; }
        const Codebook& Coef1Codes() { static const Codebook c(kCoef1Codes); return c; }
        const Codebook& Vec4Codes() { static const Codebook c(kVec4Codes); return c; }
        const Codebook& Vec2Codes() { static const Codebook c(kVec2Codes); return c; }
        const Codebook& Vec1Codes() { static const Codebook c(kVec1Codes); return c; }

        constexpr int kVec1Escape = 100; // last vec1 symbol: an escaped large value follows

        int Log2(uint32_t v) { return v ? 31 - __builtin_clz(v) : 0; }

        // Large value escape: 8, 16, 24 or 31 bits, selected by up to three prefix bits.
        uint32_t ReadLargeValue(BitReader& b)
        {
            uint32_t bits = 8;
            if (b.Read1())
            {
                bits += 8;
                if (b.Read1())
                {
                    bits += 8;
                    if (b.Read1())
                        bits += 7;
                }
            }
            return b.Read(bits);
        }

        // Sine window of `length` points: w[i] = sin((i + 0.5) * pi / (2 * length)).
        const std::vector<double>& SineWindow(int length)
        {
            static std::vector<double> windows[16];
            static const bool built = [] {
                for (int bits = 6; bits <= 10; bits++)
                {
                    const int n = 1 << bits;
                    windows[bits].resize(n);
                    for (int i = 0; i < n; i++)
                        windows[bits][i] = std::sin((i + 0.5) * (std::numbers::pi / (2.0 * n)));
                }
                return true;
            }();
            (void)built;
            return windows[Log2(uint32_t(length))];
        }

        // DCT-IV of length n (power of two, 64..512): D[k] = sum x[j] cos(pi/n (j + 1/2)(k + 1/2)), computed with an
        // n/2-point complex FFT: u[j] = x[2j] + i x[n-1-2j], C = e^{-i pi (4k+1)/(4n)} FFT(u[j] e^{-i pi j/n}),
        // D[2k] = Re C[k], D[n-1-2k] = -Im C[k].
        void DctIv(const double* x, double* out, int n)
        {
            using Complex = std::complex<double>;
            const int m = n / 2;
            Complex v[256];
            for (int j = 0; j < m; j++)
                v[j] = Complex(x[2 * j], x[n - 1 - 2 * j]) * std::polar(1.0, -std::numbers::pi * j / n);
            // Iterative radix-2 FFT (forward, e^{-2 pi i jk/m}).
            for (int i = 1, j = 0; i < m; i++)
            {
                int bit = m >> 1;
                for (; j & bit; bit >>= 1)
                    j ^= bit;
                j ^= bit;
                if (i < j)
                    std::swap(v[i], v[j]);
            }
            for (int len = 2; len <= m; len <<= 1)
            {
                const double angle = -2.0 * std::numbers::pi / len;
                for (int i = 0; i < m; i += len)
                    for (int k = 0; k < len / 2; k++)
                    {
                        const Complex w = std::polar(1.0, angle * k);
                        const Complex a = v[i + k], b = v[i + k + len / 2] * w;
                        v[i + k] = a + b;
                        v[i + k + len / 2] = a - b;
                    }
            }
            for (int k = 0; k < m; k++)
            {
                const Complex c = v[k] * std::polar(1.0, -std::numbers::pi * (4 * k + 1) / (4.0 * n));
                out[2 * k] = c.real();
                out[n - 1 - 2 * k] = -c.imag();
            }
        }
    }

    // ---- Bit reader --------------------------------------------------------------------------------------------------

    uint32_t BitReader::Peek(uint32_t bits) const
    {
        uint32_t value = 0;
        for (uint32_t i = 0; i < bits; i++)
        {
            const uint64_t p = pos_ + i;
            const uint32_t bit = p < end_ ? (data_[p >> 3] >> (7 - (p & 7))) & 1 : 0;
            value = value << 1 | bit;
        }
        return value;
    }

    uint32_t BitReader::Read(uint32_t bits)
    {
        const uint32_t value = Peek(bits);
        pos_ += bits;
        return value;
    }

    int32_t BitReader::ReadSigned(uint32_t bits)
    {
        const uint32_t v = Read(bits);
        return int32_t(v << (32 - bits)) >> (32 - bits);
    }

    // ---- Decoder -----------------------------------------------------------------------------------------------------

    bool WmaProDecoder::Fail(const char* what)
    {
        error_ = what;
        return false;
    }

    void WmaProDecoder::Initialize(int channels, int sampleRate)
    {
        channels_ = std::clamp(channels, 1, kMaxChannels);
        sampleRate_ = sampleRate;
        // XMA's band layout uses the nominal rate class of the stream.
        const int rate = sampleRate > 44100 ? 48000 : sampleRate > 32000 ? 44100 : sampleRate > 24000 ? 32000 : 24000;
        for (int i = 0; i < kBlockSizes; i++)
        {
            const int length = kFrameSamples >> i;
            int band = 1;
            bandOffsets_[i][0] = 0;
            for (int x = 0; x < kMaxBands - 1 && bandOffsets_[i][band - 1] < length; x++)
            {
                const int offset = ((length * 2 * kCriticalFrequencies[x]) / rate + 2) & ~3;
                if (offset > bandOffsets_[i][band - 1])
                    bandOffsets_[i][band++] = offset;
                if (offset >= length)
                    break;
            }
            bandOffsets_[i][band - 1] = length;
            numBands_[i] = band - 1;
        }
        // Scale factors transmitted for one block size are reused for another by mapping each band's centre.
        for (int i = 0; i < kBlockSizes; i++)
            for (int b = 0; b < numBands_[i]; b++)
            {
                const int centre = ((bandOffsets_[i][b] + bandOffsets_[i][b + 1] - 1) << i) >> 1;
                for (int x = 0; x < kBlockSizes; x++)
                {
                    int v = 0;
                    while ((bandOffsets_[x][v + 1] << x) < centre)
                        v++;
                    scaleFactorOffsets_[i][x][b] = v;
                }
            }
        Reset();
    }

    void WmaProDecoder::Reset()
    {
        for (Channel& c : channel_)
        {
            c = Channel{};
            c.prevBlockLength = kFrameSamples;
        }
    }

    int WmaProDecoder::DecodeSubframeLength(BitReader& b, int offset)
    {
        constexpr int kMinSubframe = kFrameSamples / 4;
        if (offset == kFrameSamples - kMinSubframe)
            return kMinSubframe;
        int shift = 0;
        if (b.Read1())
            shift = 1 + int(b.Read(1)); // maximum-length bit, then log2(log2(4)) + 1 - 1 bits
        const int length = kFrameSamples >> shift;
        return length < kMinSubframe ? -1 : length;
    }

    bool WmaProDecoder::DecodeTileHeader(BitReader& b)
    {
        int numSamples[kMaxChannels] = {};
        bool contains[kMaxChannels] = {};
        int channelsForSubframe = channels_;
        int minChannelLength = 0;
        for (int c = 0; c < channels_; c++)
            channel_[c].numSubframes = 0;
        const bool fixedLayout = b.Read1(); // max subframes is 4, so the bit is always present
        do
        {
            for (int c = 0; c < channels_; c++)
            {
                if (numSamples[c] == minChannelLength)
                    contains[c] = fixedLayout || channelsForSubframe == 1
                               || minChannelLength == kFrameSamples - kFrameSamples / 4 || b.Read1();
                else
                    contains[c] = false;
            }
            const int length = DecodeSubframeLength(b, minChannelLength);
            if (length <= 0)
                return Fail("subframe length");
            minChannelLength += length;
            for (int c = 0; c < channels_; c++)
            {
                Channel& ch = channel_[c];
                if (contains[c])
                {
                    if (ch.numSubframes >= kMaxSubframes)
                        return Fail("too many subframes");
                    ch.subframeLength[ch.numSubframes++] = length;
                    numSamples[c] += length;
                    if (numSamples[c] > kFrameSamples)
                        return Fail("channel longer than the frame");
                }
                else if (numSamples[c] <= minChannelLength)
                {
                    if (numSamples[c] < minChannelLength)
                    {
                        channelsForSubframe = 0;
                        minChannelLength = numSamples[c];
                    }
                    channelsForSubframe++;
                }
            }
        } while (minChannelLength < kFrameSamples);
        return true;
    }

    bool WmaProDecoder::DecodeChannelTransform(BitReader& b)
    {
        numGroups_ = 0;
        if (channels_ == 1)
            return true;
        int remaining = channelsForSubframe_;
        if (b.Read1())
            return Fail("channel transform bit");
        for (; remaining && numGroups_ < channelsForSubframe_; numGroups_++)
        {
            ChannelGroup& g = group_[numGroups_];
            g.numChannels = 0;
            g.transform = false;
            // With at most two channels the group takes every channel not yet grouped.
            g.numChannels = remaining;
            int n = 0;
            for (int i = 0; i < channelsForSubframe_; i++)
            {
                Channel& ch = channel_[channelIndexes_[i]];
                if (!ch.grouped)
                    g.data[n++] = ch.coefficients;
                ch.grouped = true;
            }
            if (g.numChannels == 2)
            {
                if (b.Read1())
                {
                    if (b.Read1())
                        return Fail("unknown channel transform type");
                }
                else
                {
                    // Mid/side for a stereo stream.
                    g.transform = true;
                    g.matrix[0] = 1.0;
                    g.matrix[1] = -1.0;
                    g.matrix[2] = 1.0;
                    g.matrix[3] = 1.0;
                }
            }
            if (g.transform)
            {
                if (!b.Read1())
                    for (int i = 0; i < numBandsCur_; i++)
                        g.transformBand[i] = b.Read1();
                else
                    std::fill(g.transformBand.begin(), g.transformBand.begin() + numBandsCur_, true);
            }
            remaining -= g.numChannels;
        }
        return true;
    }

    bool WmaProDecoder::DecodeScaleFactors(BitReader& b)
    {
        for (int i = 0; i < channelsForSubframe_; i++)
        {
            Channel& c = channel_[channelIndexes_[i]];
            c.scaleFactors = c.savedScaleFactors[c.scaleFactorIndex ^ 1];
            int* sf = c.scaleFactors;
            if (c.reuseScaleFactors)
            {
                const int* map = scaleFactorOffsets_[tableIndex_][c.tableIndex];
                for (int band = 0; band < numBandsCur_; band++)
                    sf[band] = c.savedScaleFactors[c.scaleFactorIndex][map[band]];
            }
            if (c.curSubframe == 0 || b.Read1())
            {
                if (!c.reuseScaleFactors)
                {
                    // DPCM from 45 / step.
                    c.scaleFactorStep = int(b.Read(2)) + 1;
                    int value = 45 / c.scaleFactorStep;
                    for (int band = 0; band < numBandsCur_; band++)
                    {
                        value += ScaleCodes().Decode(b);
                        sf[band] = value;
                    }
                }
                else
                {
                    // Run/level coded differences to the resampled factors.
                    for (int band = 0; band < numBandsCur_; band++)
                    {
                        const int index = ScaleRlCodes().Decode(b);
                        int skip, value, sign;
                        if (index == 0)
                        {
                            const uint32_t code = b.Read(14);
                            value = int(code >> 6);
                            sign = int(code & 1) - 1;
                            skip = int(code & 0x3F) >> 1;
                        }
                        else if (index == 1)
                            break;
                        else
                        {
                            skip = kScaleRlRun[index];
                            value = kScaleRlLevel[index];
                            sign = int(b.Read1()) - 1;
                        }
                        band += skip;
                        if (band >= numBandsCur_)
                            return Fail("scale factor run past the last band");
                        sf[band] += (value ^ sign) - sign;
                    }
                }
                c.scaleFactorIndex ^= 1;
                c.tableIndex = tableIndex_;
                c.reuseScaleFactors = true;
            }
            c.maxScaleFactor = sf[0];
            for (int band = 1; band < numBandsCur_; band++)
                c.maxScaleFactor = std::max(c.maxScaleFactor, sf[band]);
        }
        return true;
    }

    bool WmaProDecoder::DecodeCoefficients(BitReader& b, Channel& c)
    {
        const bool table1 = b.Read1();
        const Codebook& codes = table1 ? Coef1Codes() : Coef0Codes();
        const uint16_t* run = table1 ? kCoef1Run : kCoef0Run;
        const uint16_t* level = table1 ? kCoef1Level : kCoef0Level;
        double* coeffs = c.coefficients;
        bool runLevelMode = false;
        int cur = 0, zeros = 0;

        // Vector-coded coefficients, four at a time.
        while ((transmitNumVectorCoefficients_ || !runLevelMode) && cur + 3 < c.numVectorCoefficients)
        {
            uint32_t values[4];
            const int index = Vec4Codes().Decode(b);
            if (index < 0)
            {
                for (int i = 0; i < 4; i += 2)
                {
                    const int pair = Vec2Codes().Decode(b);
                    if (pair < 0)
                    {
                        uint32_t v0 = uint32_t(Vec1Codes().Decode(b));
                        if (v0 == kVec1Escape)
                            v0 += ReadLargeValue(b);
                        uint32_t v1 = uint32_t(Vec1Codes().Decode(b));
                        if (v1 == kVec1Escape)
                            v1 += ReadLargeValue(b);
                        values[i] = v0;
                        values[i + 1] = v1;
                    }
                    else
                    {
                        values[i] = uint32_t(pair) >> 4;
                        values[i + 1] = uint32_t(pair) & 0xF;
                    }
                }
            }
            else
            {
                values[0] = uint32_t(index) >> 12;
                values[1] = (uint32_t(index) >> 8) & 0xF;
                values[2] = (uint32_t(index) >> 4) & 0xF;
                values[3] = uint32_t(index) & 0xF;
            }
            for (int i = 0; i < 4; i++)
            {
                if (values[i])
                {
                    coeffs[cur] = b.Read1() ? double(values[i]) : -double(values[i]);
                    zeros = 0;
                }
                else
                {
                    coeffs[cur] = 0.0;
                    // Switch to run/level coding after subframe_length / 256 zeros in a row.
                    runLevelMode |= ++zeros > (subframeLength_ >> 8);
                }
                cur++;
            }
        }

        // Run/level coded coefficients for the rest; positions wrap at the subframe length.
        if (cur < subframeLength_)
        {
            std::fill(coeffs + cur, coeffs + subframeLength_, 0.0);
            const int mask = subframeLength_ - 1;
            for (int offset = cur; offset < subframeLength_; offset++)
            {
                const int code = codes.Decode(b);
                if (code > 1)
                {
                    offset += run[code];
                    const double v = level[code];
                    coeffs[offset & mask] = b.Read1() ? v : -v;
                }
                else if (code == 1)
                    break; // end of block
                else
                {
                    const uint32_t value = ReadLargeValue(b);
                    if (b.Read1())
                    {
                        if (b.Read1())
                        {
                            if (b.Read1())
                                return Fail("broken coefficient escape");
                            offset += int(b.Read(uint32_t(escapeLength_))) + 4;
                        }
                        else
                            offset += int(b.Read(2)) + 1;
                    }
                    coeffs[offset & mask] = b.Read1() ? double(value) : -double(value);
                }
            }
            // A run past the end of the block is ignored (the values already wrapped into place).
        }
        return true;
    }

    void WmaProDecoder::InverseChannelTransform()
    {
        for (int g = 0; g < numGroups_; g++)
        {
            ChannelGroup& group = group_[g];
            if (!group.transform)
                continue;
            for (int band = 0; band < numBandsCur_; band++)
            {
                const int start = bandOffsets_[tableIndex_][band];
                const int end = std::min(bandOffsets_[tableIndex_][band + 1], subframeLength_);
                if (group.transformBand[band])
                {
                    for (int y = start; y < end; y++)
                    {
                        const double d0 = group.data[0][y], d1 = group.data[1][y];
                        group.data[0][y] = d0 * group.matrix[0] + d1 * group.matrix[1];
                        group.data[1][y] = d0 * group.matrix[2] + d1 * group.matrix[3];
                    }
                }
                else if (channels_ == 2)
                {
                    for (int y = start; y < end; y++)
                    {
                        group.data[0][y] *= 181.0 / 128.0;
                        group.data[1][y] *= 181.0 / 128.0;
                    }
                }
            }
        }
    }

    void WmaProDecoder::InverseMdct(const double* input, double* output, int length)
    {
        // Half inverse MDCT: y[n] = s * DCT-IV[length - 1 - n], s = 2 / length (and 1 / 32768 for 16-bit scale).
        double d[kFrameSamples];
        DctIv(input, d, length);
        const double scale = 2.0 / length / 32768.0;
        for (int n = 0; n < length; n++)
            output[n] = d[length - 1 - n] * scale;
    }

    void WmaProDecoder::Window()
    {
        for (int i = 0; i < channelsForSubframe_; i++)
        {
            Channel& c = channel_[channelIndexes_[i]];
            int windowLength = c.prevBlockLength;
            double* start = c.coefficients - windowLength / 2;
            if (subframeLength_ < windowLength)
            {
                start += (windowLength - subframeLength_) / 2;
                windowLength = subframeLength_;
            }
            const std::vector<double>& w = SineWindow(windowLength);
            const int half = windowLength / 2;
            // Overlap-add of the previous block's tail [start, start + half) with this block's head.
            for (int k = 0; k < half; k++)
            {
                const int j = windowLength - 1 - k;
                const double s0 = start[k], s1 = start[j];
                start[k] = s0 * w[j] - s1 * w[k];
                start[j] = s0 * w[k] + s1 * w[j];
            }
            c.prevBlockLength = subframeLength_;
        }
    }

    bool WmaProDecoder::DecodeSubframe(BitReader& b)
    {
        int offset = kFrameSamples, length = kFrameSamples;
        for (int i = 0; i < channels_; i++)
        {
            channel_[i].grouped = false;
            if (offset > channel_[i].decodedSamples)
            {
                offset = channel_[i].decodedSamples;
                length = channel_[i].subframeLength[channel_[i].curSubframe];
            }
        }
        int totalSamples = kFrameSamples * channels_;
        channelsForSubframe_ = 0;
        for (int i = 0; i < channels_; i++)
        {
            Channel& c = channel_[i];
            totalSamples -= c.decodedSamples;
            if (offset == c.decodedSamples && length == c.subframeLength[c.curSubframe])
            {
                totalSamples -= c.subframeLength[c.curSubframe];
                c.decodedSamples += c.subframeLength[c.curSubframe];
                channelIndexes_[channelsForSubframe_++] = i;
            }
        }
        if (totalSamples == 0)
            parsedAllSubframes_ = true;

        tableIndex_ = Log2(uint32_t(kFrameSamples / length));
        numBandsCur_ = numBands_[tableIndex_];
        for (int i = 0; i < channelsForSubframe_; i++)
        {
            Channel& c = channel_[channelIndexes_[i]];
            c.coefficients = &c.out[offset + kFrameSamples / 2];
        }
        subframeLength_ = length;
        escapeLength_ = Log2(uint32_t(length - 1)) + 1;

        // Extended header: fill bits to skip.
        if (b.Read1())
        {
            int64_t fill = b.Read(2);
            if (fill == 0)
            {
                const uint32_t bits = b.Read(4);
                fill = int64_t(b.Read(bits)) + 1;
            }
            if (int64_t(b.Position()) + fill > int64_t(b.End()))
                return Fail("fill bits past the end of the frame");
            b.Skip(uint64_t(fill));
        }
        if (b.Read1())
            return Fail("reserved subframe bit set");
        if (!DecodeChannelTransform(b))
            return false;

        bool transmit = false;
        for (int i = 0; i < channelsForSubframe_; i++)
        {
            Channel& c = channel_[channelIndexes_[i]];
            c.transmitCoefficients = b.Read1();
            transmit |= c.transmitCoefficients;
        }
        if (transmit)
        {
            int quantStep = 90; // 90 * 16 bits per sample >> 4
            transmitNumVectorCoefficients_ = b.Read1();
            if (transmitNumVectorCoefficients_)
            {
                const uint32_t bits = uint32_t(Log2(uint32_t((length + 3) / 4)) + 1);
                for (int i = 0; i < channelsForSubframe_; i++)
                {
                    const int count = int(b.Read(bits)) << 2;
                    if (count > length)
                        return Fail("too many vector coded coefficients");
                    channel_[channelIndexes_[i]].numVectorCoefficients = count;
                }
            }
            else
            {
                for (int i = 0; i < channelsForSubframe_; i++)
                    channel_[channelIndexes_[i]].numVectorCoefficients = length;
            }
            int step = b.ReadSigned(6);
            quantStep += step;
            if (step == -32 || step == 31)
            {
                const int sign = (step == 31) - 1;
                int quant = 0;
                while (int64_t(b.Position()) + 5 < int64_t(b.End()) && (step = int(b.Read(5))) == 31)
                    quant += 31;
                quantStep += ((quant + step) ^ sign) - sign;
            }
            if (channelsForSubframe_ == 1)
                channel_[channelIndexes_[0]].quantStep = quantStep;
            else
            {
                const uint32_t modifierBits = b.Read(3);
                for (int i = 0; i < channelsForSubframe_; i++)
                {
                    Channel& c = channel_[channelIndexes_[i]];
                    c.quantStep = quantStep;
                    if (b.Read1())
                        c.quantStep += modifierBits ? int(b.Read(modifierBits)) + 1 : 1;
                }
            }
            if (!DecodeScaleFactors(b))
                return false;
        }

        for (int i = 0; i < channelsForSubframe_; i++)
        {
            Channel& c = channel_[channelIndexes_[i]];
            if (c.transmitCoefficients && b.Position() < b.End())
            {
                if (!DecodeCoefficients(b, c))
                    return false;
            }
            else
                std::fill(c.coefficients, c.coefficients + length, 0.0);
        }

        if (transmit)
        {
            InverseChannelTransform();
            for (int i = 0; i < channelsForSubframe_; i++)
            {
                Channel& c = channel_[channelIndexes_[i]];
                const int* sf = c.scaleFactors;
                for (int band = 0; band < numBandsCur_; band++)
                {
                    const int start = bandOffsets_[tableIndex_][band];
                    const int end = std::min(bandOffsets_[tableIndex_][band + 1], length);
                    const int exponent = c.quantStep - (c.maxScaleFactor - sf[band]) * c.scaleFactorStep;
                    const double quant = std::pow(10.0, exponent / 20.0);
                    for (int k = start; k < end; k++)
                        scratch_[k] = c.coefficients[k] * quant;
                }
                InverseMdct(scratch_.data(), c.coefficients, length);
            }
        }

        Window();
        for (int i = 0; i < channelsForSubframe_; i++)
        {
            Channel& c = channel_[channelIndexes_[i]];
            if (c.curSubframe >= c.numSubframes)
                return Fail("subframe beyond the tile layout");
            c.curSubframe++;
        }
        return true;
    }

    bool WmaProDecoder::DecodeFrame(BitReader b, WmaProFrameInfo& info, double output[kMaxChannels][kFrameSamples])
    {
        error_.clear();
        info = {};
        const uint64_t start = b.Position();
        info.lengthBits = b.Read(15);
        if (info.lengthBits < 16)
            return Fail("frame length too small");
        // Everything the frame may read lies inside its length.
        b.SetEnd(std::min(b.End(), start + info.lengthBits));
        if (!DecodeTileHeader(b))
            return false;
        if (channels_ > 1 && b.Read1())
        {
            if (b.Read1())
                b.Skip(uint64_t(4 * channels_ * channels_)); // post-processing transform (unused by the decoder)
        }
        info.drcGain = uint8_t(b.Read(8));
        if (b.Read1())
        {
            if (b.Read1())
                info.trimStart = uint16_t(b.Read(10));
            if (b.Read1())
                info.trimEnd = uint16_t(b.Read(10));
        }
        parsedAllSubframes_ = false;
        for (int c = 0; c < channels_; c++)
        {
            channel_[c].decodedSamples = 0;
            channel_[c].curSubframe = 0;
            channel_[c].reuseScaleFactors = false;
        }
        while (!parsedAllSubframes_)
            if (!DecodeSubframe(b))
                return false;
        if (b.Overrun())
            return Fail("frame read past its length");

        for (int c = 0; c < channels_; c++)
        {
            Channel& ch = channel_[c];
            std::copy(ch.out.begin(), ch.out.begin() + kFrameSamples, output[c]);
            // The second half of this frame's last block overlaps the next frame.
            std::copy(ch.out.begin() + kFrameSamples, ch.out.begin() + kFrameSamples + kFrameSamples / 2, ch.out.begin());
        }

        // The frame ends with a padding bit and the trailer bit ("more frames in this packet").
        const uint64_t used = b.Position() - start;
        if (used + 2 != info.lengthBits)
            return Fail(used + 2 > info.lengthBits ? "frame data longer than its length field" : "frame data shorter than its length field");
        b.Skip(1);
        info.moreFrames = b.Read1();
        return true;
    }

    int WmaProDecoder::TestTransform(std::string& report)
    {
        int failures = 0;
        for (int n = 64; n <= 512; n *= 2)
        {
            double x[512], fast[512];
            uint32_t seed = 12345u + uint32_t(n);
            for (int i = 0; i < n; i++)
            {
                seed = seed * 1664525u + 1013904223u;
                x[i] = (int32_t(seed) >> 8) / 8388608.0 * 1000.0;
            }
            DctIv(x, fast, n);
            double worst = 0;
            for (int k = 0; k < n; k++)
            {
                long double sum = 0;
                for (int j = 0; j < n; j++)
                    sum += x[j] * std::cos(std::numbers::pi_v<long double> / n * (j + 0.5L) * (k + 0.5L));
                worst = std::max(worst, std::fabs(double(sum) - fast[k]));
            }
            const bool ok = worst < 1e-9 * n * 1000.0;
            failures += !ok;
            report += std::format("DCT-IV n={}: max |fast - definition| = {:.3g}{}\n", n, worst, ok ? "" : "  FAILED");
        }
        return failures;
    }
}
