// XEXMagic - Standalone XEX decompressor/decompiler
// Adapted from Xenia (BSD license) - byte_order.h

#ifndef XEXMAGIC_BYTE_ORDER_H_
#define XEXMAGIC_BYTE_ORDER_H_

#include <cstdint>
#include <cstring>
#include <type_traits>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

#if __cpp_lib_endian
#include <bit>
#endif

#if !__cpp_lib_endian
#ifdef __BYTE_ORDER__
namespace std {
enum class endian {
  little = __ORDER_LITTLE_ENDIAN__,
  big = __ORDER_BIG_ENDIAN__,
  native = __BYTE_ORDER__
};
}
#else
// Hardcode to little endian (Windows/x86)
namespace std {
enum class endian { little = 0, big = 1, native = 0 };
}
#endif
#endif

static_assert((std::endian::native == std::endian::big) ||
              (std::endian::native == std::endian::little));

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

namespace xe {

#if defined(_MSC_VER) && !defined(__clang__)
#define XEXMAGIC_BYTE_SWAP_16 _byteswap_ushort
#define XEXMAGIC_BYTE_SWAP_32 _byteswap_ulong
#define XEXMAGIC_BYTE_SWAP_64 _byteswap_uint64
#else
#define XEXMAGIC_BYTE_SWAP_16 __builtin_bswap16
#define XEXMAGIC_BYTE_SWAP_32 __builtin_bswap32
#define XEXMAGIC_BYTE_SWAP_64 __builtin_bswap64
#endif

template <class T>
inline T byte_swap(T value) {
  static_assert(
      sizeof(T) == 8 || sizeof(T) == 4 || sizeof(T) == 2 || sizeof(T) == 1,
      "byte_swap(T value): Type T has illegal size");
  if constexpr (sizeof(T) == 8) {
    uint64_t temp =
        XEXMAGIC_BYTE_SWAP_64(*reinterpret_cast<uint64_t*>(&value));
    return *reinterpret_cast<T*>(&temp);
  } else if constexpr (sizeof(T) == 4) {
    uint32_t temp =
        XEXMAGIC_BYTE_SWAP_32(*reinterpret_cast<uint32_t*>(&value));
    return *reinterpret_cast<T*>(&temp);
  } else if constexpr (sizeof(T) == 2) {
    uint16_t temp =
        XEXMAGIC_BYTE_SWAP_16(*reinterpret_cast<uint16_t*>(&value));
    return *reinterpret_cast<T*>(&temp);
  } else if constexpr (sizeof(T) == 1) {
    return value;
  }
}

template <typename T>
inline void store_and_swap(void* addr, T value) {
  T swapped = byte_swap(value);
  std::memcpy(addr, &swapped, sizeof(T));
}

template <typename T, std::endian E>
struct endian_store {
  endian_store() = default;
  endian_store(const T& src) { set(src); }
  endian_store(const endian_store& other) { set(other); }
  operator T() const { return get(); }

  void set(const T& src) {
    if constexpr (std::endian::native == E) {
      value = src;
    } else {
      value = xe::byte_swap(src);
    }
  }
  void set(const endian_store& other) { value = other.value; }
  T get() const {
    if constexpr (std::endian::native == E) {
      return value;
    }
    return xe::byte_swap(value);
  }

  endian_store<T, E>& operator+=(int a) {
    *this = *this + a;
    return *this;
  }
  endian_store<T, E>& operator-=(int a) {
    *this = *this - a;
    return *this;
  }
  endian_store<T, E>& operator++() {
    *this += 1;
    return *this;
  }
  endian_store<T, E> operator++(int) {
    *this += 1;
    return (*this - 1);
  }
  endian_store<T, E>& operator--() {
    *this -= 1;
    return *this;
  }
  endian_store<T, E> operator--(int) {
    *this -= 1;
    return (*this + 1);
  }

  T value;
};

template <typename T>
using be = endian_store<T, std::endian::big>;
template <typename T>
using le = endian_store<T, std::endian::little>;

}  // namespace xe

// Convenience: pull be<T> and le<T> into global scope
using xe::be;
using xe::le;

#endif  // XEXMAGIC_BYTE_ORDER_H_
