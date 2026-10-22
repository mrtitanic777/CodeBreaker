#pragma once

// PNG decoding (every image in the system UI packages is a PNG): all colour types and bit depths, Adam7 interlacing,
// palette transparency (tRNS). Chunk CRCs and the zlib checksum are checked. Colour-space chunks (gAMA, sRGB, iCCP)
// are ignored, as the console's loader ignores them: the stored values are the texture's values.

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace xui
{
    struct Image
    {
        uint32_t width = 0, height = 0;
        std::vector<uint8_t> rgba; // straight (not premultiplied) alpha, rows top to bottom
    };

    bool DecodePng(const uint8_t* data, size_t size, Image& out, std::string& error);
    // Other formats (the NXE dashboards' JPEG backgrounds and tiles, the launcher's BMP thumbnails), decoded by
    // Windows' WIC codecs. DecodeImage takes any, PNG by our own decoder.
    bool DecodeWithWic(const uint8_t* data, size_t size, Image& out, std::string& error);
    bool DecodeImage(const uint8_t* data, size_t size, Image& out, std::string& error);
}
