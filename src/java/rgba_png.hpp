// java/rgba_png.hpp -- a small PNG codec for the Java lane's pictures: read any PNG as 8-bit RGBA, write 8-bit RGBA.
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

#include "core/bytes.hpp"
#include "formats/png.hpp"

namespace cb::java::rgba_png {

// The codec is formats/png.hpp's: straight 8-bit RGBA, row by row, top to bottom; four bytes (R, G, B, A) per pixel.
using Image = formats::png::Image;
using formats::png::blank;
using formats::png::kMaxPixels;

// Read a PNG file's bytes (standard rules; at most kMaxPixels pixels).
inline Image decode(ByteView png) { return formats::png::decode(png); }

// The PNG file for an image (8-bit RGBA, every row filter 0, one IDAT chunk, default zlib level).
using formats::png::encode;

// The part of `image` inside the box [left, right) x [top, bottom). Pixels of the box outside the image come out
// transparent black (the box may overhang). An empty or inverted box gives a 0-sized image.
Image crop(const Image& image, std::int64_t left, std::int64_t top, std::int64_t right, std::int64_t bottom);

// A `size` x `size` copy by nearest-neighbour sampling (crisp pixels when scaling a sprite up). The source pixel of
// an output pixel is found by stepping half a source step, then one source step per output pixel (the way Pillow's
// NEAREST resize does). A `size` of 0 is a UsageError; an empty source image gives a blank result.
Image resize_nearest(const Image& image, std::uint32_t size);

}  // namespace cb::java::rgba_png
