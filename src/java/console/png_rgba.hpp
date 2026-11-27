// java/console/png_rgba.hpp -- force a PNG to 8-bit straight-alpha RGBA (colour type 6).
//
// WHY: the Xbox 360 texture loader expects 32-bit RGBA. A palettized (indexed, colour type 3) or grayscale PNG
// loaded raw is decoded as garbage colours -- the "RGB problem" from the Alpha port. Diffing Alpha's shipped Content
// against the stock jar showed the ONLY transform its pipeline applied was: convert every non-RGBA PNG to RGBA8888,
// honouring palette transparency (tRNS). No alpha-bleed, no channel swap, no premultiply -- those were all confirmed
// absent. So exactly that is done here: a PNG of any colour type other than 6 is decoded and re-encoded as RGBA8888;
// an RGBA PNG is passed through byte for byte.
//
// The decoder reads every colour type and bit depth (1/2/4/8/16-bit grey, RGB, palette with tRNS, grey+alpha, RGBA),
// interlaced or not. The pixels it gives are the ones the Alpha pipeline's imaging library gave: 16-bit samples keep
// their high byte, 16-bit grey is clipped to 255, a grey or RGB tRNS colour makes a pixel transparent when the pixel's
// 8-bit value equals the tRNS value's LOW byte, palette tRNS is one alpha per index and an index past the palette is
// black. The compressed bytes of the PNG written differ from that library's (a different deflate).
#pragma once

#include <cstdint>
#include <optional>

#include "core/bytes.hpp"
#include "formats/png.hpp"

namespace cb::java::console::png {

// The IHDR colour type (0 grey, 2 RGB, 3 indexed, 4 grey+alpha, 6 RGBA) of a PNG; nullopt if the bytes are not one
// (no PNG signature, or too short to hold the header).
std::optional<std::uint8_t> color_type(ByteView png);

// True if this PNG is not RGBA (colour type 6), i.e. would misrender on the 360.
bool needs_rgba(ByteView png);

// A decoded picture: the codec's (formats/png.hpp), width x height pixels, 4 bytes each (R, G, B, A), row by row from the top.
using Image = formats::png::Image;

// The pixels of a PNG of any colour type (see the rules above: the codec's imaging rules). FormatError when it cannot be read.
inline Image decode_rgba(ByteView png) { return formats::png::decode(png, formats::png::Rules::imaging); }

// An 8-bit RGBA PNG of the picture.
inline Bytes encode_rgba(const Image& image) { return formats::png::encode(image); }

struct Converted {
    Bytes png;
    bool changed = false;   // false: the input was already RGBA (or not a PNG) and is returned as it was
};

// An RGBA PNG (or anything that is not a PNG) is returned unchanged; any other PNG is decoded and re-encoded as
// 8-bit RGBA. FormatError when a PNG cannot be read (a bad or damaged header, a truncated or corrupt data stream).
Converted normalize_to_rgba(ByteView png);

}  // namespace cb::java::console::png
