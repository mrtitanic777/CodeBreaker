// formats/png.hpp -- the one PNG codec: read any PNG as 8-bit straight-alpha RGBA, write 8-bit RGBA or RGB.
//
// `decode` accepts every PNG variant (grey, grey+alpha, palette, RGB, RGBA; 1 to 16 bits; interlaced or not; tRNS
// transparency) and converts it to straight 8-bit RGBA; `encode` writes RGBA back with a single IDAT, every row filter 0,
// at zlib's default level.
//
// A damaged picture is a FormatError (a bad signature, a chunk that runs past the end, a CRC that does not match, a zlib
// stream that ends early, an unknown row filter); a header that names a compression, filter or interlace method this
// reader does not know is an UnsupportedError.
//
// Two rule sets say how a sample becomes a pixel. Standard is the specification's: 16-bit samples keep their high byte,
// narrower grey samples are scaled to the full range, a tRNS colour key is compared sample for sample, a palette index
// outside the palette is an error. Imaging is what the Alpha port's imaging library did, which the Xbox 360 conversion
// reproduces (java/console/png_rgba.hpp): 16-bit grey is clipped to 255, a grey or RGB key makes a pixel transparent when
// its 8-bit value equals the key's low byte, and an index past the palette is black.
#pragma once

#include <cstdint>

#include "core/bytes.hpp"

namespace cb::formats::png {

// Straight 8-bit RGBA, row by row, top to bottom; four bytes (R, G, B, A) per pixel.
struct Image {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    Bytes rgba;   // width * height * 4 bytes

    std::uint8_t* pixel(std::uint32_t x, std::uint32_t y) { return &rgba[(std::size_t{y} * width + x) * 4]; }
    const std::uint8_t* pixel(std::uint32_t x, std::uint32_t y) const { return &rgba[(std::size_t{y} * width + x) * 4]; }
};

enum class Rules { standard, imaging };

// A transparent black image of the given size.
Image blank(std::uint32_t width, std::uint32_t height);

// At most kMaxPixels pixels (a header that promises more is a FormatError, not an allocation).
inline constexpr std::uint64_t kMaxPixels = std::uint64_t{1} << 28;
Image decode(ByteView png, Rules rules = Rules::standard);

// The PNG file of an 8-bit RGBA picture / of 8-bit RGB pixels (width * height * 4 / 3 bytes: anything else is an
// InternalError, a bug in the caller).
Bytes encode_rgba(std::uint32_t width, std::uint32_t height, ByteView rgba);
Bytes encode_rgb(std::uint32_t width, std::uint32_t height, ByteView rgb);
inline Bytes encode(const Image& image) { return encode_rgba(image.width, image.height, image.rgba); }

}  // namespace cb::formats::png
