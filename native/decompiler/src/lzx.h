// XEXMagic - Standalone XEX decompressor/decompiler
// Adapted from Xenia (BSD license) - lzx.h

#ifndef XEXMAGIC_LZX_H_
#define XEXMAGIC_LZX_H_

#include <cstdint>
#include <cstddef>

namespace xe {
struct xex2_delta_patch;
}  // namespace xe

int lzx_decompress(const void* lzx_data, size_t lzx_len, void* dest,
                   size_t dest_len, uint32_t window_size, void* window_data,
                   size_t window_data_len);

int lzxdelta_apply_patch(xe::xex2_delta_patch* patch, size_t patch_len,
                         uint32_t window_size, void* dest);

#endif  // XEXMAGIC_LZX_H_
