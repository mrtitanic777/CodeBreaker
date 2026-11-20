#pragma once

#include <cassert>
#include <cstdint>
#if defined(_MSC_VER) && !defined(__clang__)
#include <stdlib.h>   // _byteswap_ushort / _byteswap_ulong / _byteswap_uint64 (MSVC has no __builtin_bswap*)
#define XENON_BSWAP16(v) _byteswap_ushort(v)
#define XENON_BSWAP32(v) _byteswap_ulong(v)
#define XENON_BSWAP64(v) _byteswap_uint64(v)
#else
#define XENON_BSWAP16(v) __builtin_bswap16(v)
#define XENON_BSWAP32(v) __builtin_bswap32(v)
#define XENON_BSWAP64(v) __builtin_bswap64(v)
#endif

template<typename T>
inline T ByteSwap(T value)
{
    if constexpr (sizeof(T) == 1)
        return value;
    else if constexpr (sizeof(T) == 2)
        return static_cast<T>(XENON_BSWAP16(static_cast<uint16_t>(value)));
    else if constexpr (sizeof(T) == 4)
        return static_cast<T>(XENON_BSWAP32(static_cast<uint32_t>(value)));
    else if constexpr (sizeof(T) == 8)
        return static_cast<T>(XENON_BSWAP64(static_cast<uint64_t>(value)));

    assert(false && "Unexpected byte size.");
    return value;
}

template<typename T>
inline void ByteSwapInplace(T& value)
{
    value = ByteSwap(value);
}
