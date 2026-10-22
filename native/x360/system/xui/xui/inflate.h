#pragma once

// DEFLATE (RFC 1951) decoder, with the zlib wrapper (RFC 1950) optional. Used for PNG image data and the glyph pages
// of the console's .xtt fonts.

#include <cstddef>
#include <cstdint>
#include <vector>

namespace xui
{
    // Appends the decoded bytes to `out`. `zlib`: the input starts with a zlib header and ends with an Adler-32
    // checksum (checked). Returns false on malformed input. `consumed` (optional) receives the input bytes used.
    bool Inflate(const uint8_t* data, size_t size, std::vector<uint8_t>& out, bool zlib, size_t* consumed = nullptr);
}
