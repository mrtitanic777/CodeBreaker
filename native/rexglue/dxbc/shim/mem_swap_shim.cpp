// mem_swap_shim.cpp - portable implementations of the byte-swap copy helpers
// declared in rex/memory/utils.h. The real ones (in the unvendored src/memory)
// use SIMD; these use the compiler byte-swap builtins and are correct on any
// target. Only the copy_and_swap<T> template (in the header) calls these.
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace rex {
namespace memory {

void copy_and_swap_16_aligned(void* dest, const void* src, size_t count) {
    auto d = static_cast<uint16_t*>(dest);
    auto s = static_cast<const uint16_t*>(src);
    for (size_t i = 0; i < count; ++i) d[i] = __builtin_bswap16(s[i]);
}
void copy_and_swap_16_unaligned(void* dest, const void* src, size_t count) {
    copy_and_swap_16_aligned(dest, src, count);
}
void copy_and_swap_32_aligned(void* dest, const void* src, size_t count) {
    auto d = static_cast<uint32_t*>(dest);
    auto s = static_cast<const uint32_t*>(src);
    for (size_t i = 0; i < count; ++i) d[i] = __builtin_bswap32(s[i]);
}
void copy_and_swap_32_unaligned(void* dest, const void* src, size_t count) {
    copy_and_swap_32_aligned(dest, src, count);
}
void copy_and_swap_64_aligned(void* dest, const void* src, size_t count) {
    auto d = static_cast<uint64_t*>(dest);
    auto s = static_cast<const uint64_t*>(src);
    for (size_t i = 0; i < count; ++i) d[i] = __builtin_bswap64(s[i]);
}
void copy_and_swap_64_unaligned(void* dest, const void* src, size_t count) {
    copy_and_swap_64_aligned(dest, src, count);
}
void copy_and_swap_16_in_32_aligned(void* dest, const void* src, size_t count) {
    auto d = static_cast<uint32_t*>(dest);
    auto s = static_cast<const uint32_t*>(src);
    for (size_t i = 0; i < count; ++i) {
        uint32_t v = s[i];
        d[i] = (v >> 16) | (v << 16);
    }
}
void copy_and_swap_16_in_32_unaligned(void* dest, const void* src, size_t count) {
    copy_and_swap_16_in_32_aligned(dest, src, count);
}

}  // namespace memory
}  // namespace rex
