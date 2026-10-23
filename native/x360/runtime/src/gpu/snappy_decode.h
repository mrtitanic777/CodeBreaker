#pragma once

// Decoder for raw Snappy data (the format of Google's snappy::RawCompress: a varint of the uncompressed length, then
// literals and back-references), for reading GPU traces recorded by Xenia-based renderers (trace_replay in
// command_processor.cpp). Written from the format description; no library code.

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace gpu
{
    // Decodes `size` bytes at `in` into `out` (capacity `outSize`). False if the data is malformed or does not decode to
    // exactly `outSize` bytes.
    inline bool SnappyDecode(const uint8_t* in, size_t size, uint8_t* out, size_t outSize)
    {
        const uint8_t* const end = in + size;
        uint64_t length = 0;
        for (int shift = 0;; shift += 7)
        {
            if (in == end || shift > 35)
                return false;
            const uint8_t b = *in++;
            length |= uint64_t(b & 0x7F) << shift;
            if (!(b & 0x80))
                break;
        }
        if (length != outSize)
            return false;
        size_t pos = 0;
        while (in < end)
        {
            const uint8_t tag = *in++;
            if ((tag & 3) == 0) // literal
            {
                size_t n = (tag >> 2) + 1;
                if ((tag >> 2) >= 60)
                {
                    const size_t bytes = (tag >> 2) - 59;
                    if (size_t(end - in) < bytes)
                        return false;
                    n = 0;
                    for (size_t i = 0; i < bytes; i++)
                        n |= size_t(in[i]) << (8 * i);
                    n += 1;
                    in += bytes;
                }
                if (size_t(end - in) < n || outSize - pos < n)
                    return false;
                std::memcpy(out + pos, in, n);
                in += n;
                pos += n;
                continue;
            }
            size_t n, offset;
            if ((tag & 3) == 1) // copy, 1-byte offset
            {
                if (in == end)
                    return false;
                n = ((tag >> 2) & 7) + 4;
                offset = (size_t(tag >> 5) << 8) | *in++;
            }
            else if ((tag & 3) == 2) // copy, 2-byte offset
            {
                if (end - in < 2)
                    return false;
                n = (tag >> 2) + 1;
                offset = in[0] | size_t(in[1]) << 8;
                in += 2;
            }
            else // copy, 4-byte offset
            {
                if (end - in < 4)
                    return false;
                n = (tag >> 2) + 1;
                offset = in[0] | size_t(in[1]) << 8 | size_t(in[2]) << 16 | size_t(in[3]) << 24;
                in += 4;
            }
            if (offset == 0 || offset > pos || outSize - pos < n)
                return false;
            for (size_t i = 0; i < n; i++, pos++) // byte by byte: the source may overlap the destination
                out[pos] = out[pos - offset];
        }
        return pos == outSize;
    }
}
