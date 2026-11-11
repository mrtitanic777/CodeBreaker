// cb/formats/xbox/texture.hpp -- Xbox 360 texture untiling, DDS export and DXT decoding.
//
// Xbox 360 GPU textures are stored in a tiled (swizzled) memory layout for cache coherency, and DXT colour/index words
// are 16-bit byte-swapped versus PC. This converts a tiled Xbox 360 surface to a linear one and wraps it in a standard
// DDS so any PC tool can open it; decode_dxt() turns a DXT1/DXT3/DXT5 surface (tiled or linear) into RGBA pixels for any
// program that does not read DDS.
//
// The tiled-address math is the well-established Gildor/UModel `XGAddress2DTiledOffset` formula (also used by Noesis and
// UEViewer), reimplemented here.
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::formats::xbox::texture {

enum class Format { Dxt1, Dxt3, Dxt5 };

struct FormatInfo {
    std::uint32_t block_bytes;   // of one 4x4 block
    unsigned log2_block_bytes;
    std::string_view fourcc;     // in the DDS header
    std::string_view name;       // "dxt1"
};
const FormatInfo& info(Format f);

// "dxt1", "dxt3" or "dxt5" (lower case); nullopt for anything else.
std::optional<Format> parse_format(std::string_view name);

// The surface's size in 4x4 blocks.
struct BlockSize {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
};
BlockSize blocks(std::uint32_t width, std::uint32_t height);

// Byte offset of tiled block (x, y) -- Gildor/UModel XGAddress2DTiledOffset. x, y and `width_blocks` are in blocks;
// `log2_block_bytes` = log2(bytes per block), which the result is already scaled by.
std::uint64_t tiled_offset_2d(std::uint32_t x, std::uint32_t y, std::uint32_t width_blocks, unsigned log2_block_bytes);

// Tiled Xbox 360 surface -> linear block layout (`blocks(w, h)` blocks, rows top to bottom). A block that lies beyond the
// end of `data` stays zero. UnsupportedError for a surface too big to hold (over 16 GB).
Bytes untile(ByteView data, std::uint32_t width, std::uint32_t height, Format format);

// Linear block layout -> tiled Xbox 360 surface (the inverse of untile; the blocks the layout skips are zero).
Bytes tile(ByteView linear, std::uint32_t width, std::uint32_t height, Format format);

// Byte-swap every 16-bit word (Xbox 360 DXT endian fix). FormatError if the length is odd.
Bytes swap16(ByteView data);

// The standard 128-byte DDS header for a DXT-compressed surface; `to_dds` puts it in front of the linear data.
Bytes dds_header(std::uint32_t width, std::uint32_t height, Format format);
Bytes to_dds(ByteView linear, std::uint32_t width, std::uint32_t height, Format format);

// RGBA pixels, 4 bytes each (R, G, B, A), rows top to bottom.
struct Image {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    Bytes rgba;
};

struct DecodeOptions {
    bool tiled = true;          // the surface is in the Xbox 360's tiled layout (false for a linear PC surface)
    bool swapped = true;        // its 16-bit words are swapped, as the Xbox 360 keeps them
    bool force_opaque = false;  // ignore alpha (an opaque mask stored as DXT5)
};

// A DXT1/DXT3/DXT5 surface as an `width` x `height` image. A block that lies beyond the end of `data` is transparent
// black.
Image decode_dxt(ByteView data, std::uint32_t width, std::uint32_t height, Format format, const DecodeOptions& options = {});

}  // namespace cb::formats::xbox::texture
