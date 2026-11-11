// cb/formats/lzx.hpp -- Microsoft LZX (LZXD) decoding, for the readers that hold LZX streams.
//
// An LZ77 window and canonical Huffman trees (main, length, aligned offset); the bit reader is the one the
// reference XEX decoder uses (16-bit little-endian words read MSB-first into a 32-bit accumulator, a realign at
// every 32 KB frame); the Intel E8 call translation is undone at the end. An Xbox 360 executable stores its image as
// a chain of SHA-1-linked blocks around one such stream (`deblock`).
//
// A damaged stream is a FormatError (TruncatedError if it runs out of data); a decoder given garbage stops, it does
// not run on producing zeros.
#pragma once

#include <cstddef>
#include <cstdint>

#include "cb/core/bytes.hpp"

namespace cb::formats::lzx {

// Decode a raw (de-blocked) LZXD stream to exactly `out_len` bytes. `window_bits` is the window's size as a power of
// two (15-21; XNB content uses 16). `reference`, when not empty, seeds the window with earlier data (LZX-DELTA): the
// stream's matches may reach into it; it is not part of the output.
Bytes decompress(ByteView data, unsigned window_bits, std::size_t out_len, ByteView reference = {});

// An XEX's block chain: the first block (`first_size` bytes, hashed `first_hash`, 20 bytes) heads a list in which each
// block names the size and SHA-1 of the next and holds chunks (a 16-bit big-endian length, then that many bytes; a
// zero length ends the block) of the stream. Returns the stream. With `verify` each block's SHA-1 is checked
// (FormatError: a wrong decrypt key or a corrupt image shows up here).
Bytes deblock(ByteView image, std::uint32_t first_size, ByteView first_hash, bool verify);

// An XEX's compressed image: deblock, then decompress with a window of `window_size` bytes (a power of two) to
// `out_len` bytes.
Bytes decompress_xex(ByteView image, std::uint32_t window_size, std::uint32_t first_size, ByteView first_hash,
                     std::size_t out_len);

}  // namespace cb::formats::lzx
