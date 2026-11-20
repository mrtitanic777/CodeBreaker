// lzx_encode.h - the Microsoft LZX *compressor* XEX compression uses.
//
// An optimal-parse MS-LZX encoder. It is bit-for-bit deterministic -- the same
// input always gives the same stream -- which the `compress`, `remove-limits`
// and `machine` write paths rely on to produce the canonical file.
//
// The encoder keeps raw pointers inside a 32-bit context arena and does
// biased-pointer arithmetic; to stay exact AND portable to 64-bit it models
// the whole address space the codec touches as ONE arena and represents every
// "pointer" as a 32-bit signed byte-offset into it (see lzx_encode.cpp).
#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>

namespace xex::compress {

// Compress `in_size` bytes with a 32 KB LZX window, producing the raw LZX
// bitstream (the chunk/block XEX framing is applied by
// the caller). `window_size` must be a power of two >= 0x8000.
//
// E8 (Intel call) translation is applied when `e8_filesize` != 0 (matching the
// XEX header's "image base size"); pass 0 to disable.
std::vector<uint8_t> lzx_compress(const uint8_t* in, size_t in_size,
                                  uint32_t window_size = 0x8000,
                                  uint32_t e8_filesize = 0);

} // namespace xex::compress
