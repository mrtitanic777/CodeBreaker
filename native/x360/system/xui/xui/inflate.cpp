#include "xui/inflate.h"

#include <array>

namespace xui
{
    namespace
    {
        struct BitReader
        {
            const uint8_t* data;
            size_t size, pos = 0;
            uint64_t buffer = 0;
            int count = 0;
            bool overrun = false;

            uint32_t Bits(int n)
            {
                while (count < n)
                {
                    if (pos < size)
                        buffer |= uint64_t(data[pos]) << count;
                    else
                        overrun = true;
                    pos++;
                    count += 8;
                }
                const uint32_t v = uint32_t(buffer & ((1ull << n) - 1));
                buffer >>= n;
                count -= n;
                return v;
            }
            void AlignToByte()
            {
                buffer >>= count & 7;
                count -= count & 7;
            }
        };

        // Canonical Huffman decoding table: counts per length and symbols sorted by code.
        struct Huffman
        {
            std::array<uint16_t, 16> counts{};
            std::vector<uint16_t> symbols;

            bool Build(const uint8_t* lengths, int n)
            {
                counts.fill(0);
                for (int i = 0; i < n; i++)
                    counts[lengths[i]]++;
                counts[0] = 0;
                int left = 1;
                for (int len = 1; len < 16; len++)
                {
                    left = (left << 1) - counts[len];
                    if (left < 0)
                        return false; // over-subscribed
                }
                std::array<uint16_t, 16> offsets{};
                for (int len = 1; len < 15; len++)
                    offsets[len + 1] = uint16_t(offsets[len] + counts[len]);
                symbols.assign(size_t(n), 0);
                for (int i = 0; i < n; i++)
                    if (lengths[i])
                        symbols[offsets[lengths[i]]++] = uint16_t(i);
                return true;
            }

            int Decode(BitReader& r) const
            {
                int code = 0, first = 0, index = 0;
                for (int len = 1; len < 16; len++)
                {
                    code |= int(r.Bits(1));
                    const int count = counts[len];
                    if (code - count < first)
                        return symbols[size_t(index + (code - first))];
                    index += count;
                    first += count;
                    first <<= 1;
                    code <<= 1;
                }
                return -1;
            }
        };

        constexpr uint16_t kLengthBase[29] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59,
                                               67, 83, 99, 115, 131, 163, 195, 227, 258 };
        constexpr uint8_t kLengthExtra[29] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4,
                                               5, 5, 5, 5, 0 };
        constexpr uint16_t kDistanceBase[30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385,
                                                 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385,
                                                 24577 };
        constexpr uint8_t kDistanceExtra[30] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10,
                                                 10, 11, 11, 12, 12, 13, 13 };

        bool Codes(BitReader& r, const Huffman& lit, const Huffman& dist, std::vector<uint8_t>& out, size_t start)
        {
            for (;;)
            {
                const int sym = lit.Decode(r);
                if (sym < 0 || r.overrun)
                    return false;
                if (sym < 256)
                {
                    out.push_back(uint8_t(sym));
                    continue;
                }
                if (sym == 256)
                    return true;
                const int l = sym - 257;
                if (l >= 29)
                    return false;
                const size_t length = kLengthBase[l] + r.Bits(kLengthExtra[l]);
                const int d = dist.Decode(r);
                if (d < 0 || d >= 30)
                    return false;
                const size_t distance = kDistanceBase[d] + r.Bits(kDistanceExtra[d]);
                if (distance > out.size() - start)
                    return false;
                const size_t from = out.size() - distance;
                for (size_t i = 0; i < length; i++)
                    out.push_back(out[from + i]);
            }
        }
    }

    bool Inflate(const uint8_t* data, size_t size, std::vector<uint8_t>& out, bool zlib, size_t* consumed)
    {
        BitReader r{ data, size };
        const size_t start = out.size();
        if (zlib)
        {
            if (size < 2)
                return false;
            const uint32_t cmf = r.Bits(8), flg = r.Bits(8);
            if ((cmf & 0x0F) != 8 || (cmf * 256 + flg) % 31 != 0 || (flg & 0x20))
                return false;
        }
        bool last = false;
        while (!last)
        {
            last = r.Bits(1) != 0;
            const uint32_t type = r.Bits(2);
            if (type == 0)
            {
                r.AlignToByte();
                const uint32_t len = r.Bits(16), nlen = r.Bits(16);
                if ((len ^ 0xFFFF) != nlen)
                    return false;
                // The bit buffer holds whole bytes only now.
                for (uint32_t i = 0; i < len; i++)
                    out.push_back(uint8_t(r.Bits(8)));
            }
            else if (type == 1)
            {
                uint8_t lengths[288 + 30];
                for (int i = 0; i < 144; i++) lengths[i] = 8;
                for (int i = 144; i < 256; i++) lengths[i] = 9;
                for (int i = 256; i < 280; i++) lengths[i] = 7;
                for (int i = 280; i < 288; i++) lengths[i] = 8;
                for (int i = 0; i < 30; i++) lengths[288 + i] = 5;
                Huffman lit, dist;
                lit.Build(lengths, 288);
                dist.Build(lengths + 288, 30);
                if (!Codes(r, lit, dist, out, start))
                    return false;
            }
            else if (type == 2)
            {
                const int hlit = int(r.Bits(5)) + 257, hdist = int(r.Bits(5)) + 1, hclen = int(r.Bits(4)) + 4;
                static constexpr uint8_t kOrder[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1,
                                                        15 };
                uint8_t clen[19] = {};
                for (int i = 0; i < hclen; i++)
                    clen[kOrder[i]] = uint8_t(r.Bits(3));
                Huffman cl;
                if (!cl.Build(clen, 19))
                    return false;
                uint8_t lengths[288 + 32] = {};
                int n = 0;
                while (n < hlit + hdist)
                {
                    const int sym = cl.Decode(r);
                    if (sym < 0 || r.overrun)
                        return false;
                    if (sym < 16)
                        lengths[n++] = uint8_t(sym);
                    else
                    {
                        int repeat;
                        uint8_t value = 0;
                        if (sym == 16)
                        {
                            if (n == 0)
                                return false;
                            value = lengths[n - 1];
                            repeat = 3 + int(r.Bits(2));
                        }
                        else if (sym == 17)
                            repeat = 3 + int(r.Bits(3));
                        else
                            repeat = 11 + int(r.Bits(7));
                        if (n + repeat > hlit + hdist)
                            return false;
                        while (repeat--)
                            lengths[n++] = value;
                    }
                }
                Huffman lit, dist;
                if (!lit.Build(lengths, hlit) || !dist.Build(lengths + hlit, hdist))
                    return false;
                if (!Codes(r, lit, dist, out, start))
                    return false;
            }
            else
                return false;
            if (r.overrun)
                return false;
        }
        r.AlignToByte();
        if (zlib)
        {
            uint32_t a = 1, b = 0;
            for (size_t i = start; i < out.size(); i++)
            {
                a = (a + out[i]) % 65521;
                b = (b + a) % 65521;
            }
            uint32_t adler = 0;
            for (int i = 0; i < 4; i++)
                adler = adler << 8 | r.Bits(8);
            if (r.overrun || adler != (b << 16 | a))
                return false;
        }
        if (consumed)
            *consumed = r.pos - size_t(r.count / 8);
        return true;
    }
}
