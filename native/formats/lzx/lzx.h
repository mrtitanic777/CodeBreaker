// lzx.h -- Microsoft LZX (LZXD) decoding (formats/lzx.py), for the readers that hold LZX streams.
#pragma once

#include <cstdint>
#include <vector>

#include "../core/bytes.h"

namespace cbf::lzx {

// a raw (de-blocked) LZXD stream -> out_len bytes; ref_data seeds the window (LZX-DELTA)
std::vector<uint8_t> lzxd_decompress(Bytes data, int64_t window_bits, int64_t out_len, const Bytes *ref_data = nullptr);
// a XEX's block chain -> the LZX stream (each block's SHA-1 checked when verify)
std::vector<uint8_t> deblock(Bytes buf, int64_t first_size, Bytes first_hash, bool verify);

}  // namespace cbf::lzx
