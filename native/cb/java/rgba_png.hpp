// cb/java/rgba_png.hpp -- a small PNG codec for the Java lane's pictures: read any PNG as 8-bit RGBA, write 8-bit RGBA.
//
// The Java lane crops sprites out of the game's texture atlases (terrain.png, gui/items.png), scales them to the
// tiles a console's achievement list wants, and writes them back. All of it happens on straight (non-premultiplied)
// 8-bit RGBA pixels, so that is the one in-memory form: `decode` accepts every PNG variant (grey, grey+alpha,
// palette, RGB, RGBA; 1 to 16 bits; interlaced or not; tRNS transparency) and converts it, and `encode` writes the
// result back as an RGBA PNG with a single IDAT. 16-bit samples keep their high byte; grey samples of fewer than 8
// bits are scaled to the full range.
//
// A damaged picture is a FormatError (a bad signature, a chunk that runs past the end, a CRC that does not match, a
// zlib stream that ends early, an index outside the palette, an unknown row filter); a header that names a
// compression, filter or interlace method this reader does not know is an UnsupportedError.
#pragma once

#include <cstdint>

#include "cb/core/bytes.hpp"

namespace cb::java::rgba_png {

// Straight 8-bit RGBA, row by row, top to bottom; four bytes (R, G, B, A) per pixel.
struct Image {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    Bytes rgba;   // width * height * 4 bytes

    std::uint8_t* pixel(std::uint32_t x, std::uint32_t y) { return &rgba[(std::size_t{y} * width + x) * 4]; }
    const std::uint8_t* pixel(std::uint32_t x, std::uint32_t y) const { return &rgba[(std::size_t{y} * width + x) * 4]; }
};

// A transparent black image of the given size.
Image blank(std::uint32_t width, std::uint32_t height);

// Read a PNG file's bytes. At most kMaxPixels pixels (a header that promises more is a FormatError, not an
// allocation).
inline constexpr std::uint64_t kMaxPixels = std::uint64_t{1} << 28;
Image decode(ByteView png);

// The PNG file for an image (8-bit RGBA, every row filter 0, one IDAT chunk, default zlib level).
Bytes encode(const Image& image);

// The part of `image` inside the box [left, right) x [top, bottom). Pixels of the box outside the image come out
// transparent black (the box may overhang). An empty or inverted box gives a 0-sized image.
Image crop(const Image& image, std::int64_t left, std::int64_t top, std::int64_t right, std::int64_t bottom);

// A `size` x `size` copy by nearest-neighbour sampling (crisp pixels when scaling a sprite up). The source pixel of
// an output pixel is found by stepping half a source step, then one source step per output pixel (the way Pillow's
// NEAREST resize does). A `size` of 0 is a UsageError; an empty source image gives a blank result.
Image resize_nearest(const Image& image, std::uint32_t size);

}  // namespace cb::java::rgba_png
