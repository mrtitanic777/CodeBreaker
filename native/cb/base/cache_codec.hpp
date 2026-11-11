// cb/base/cache_codec.hpp -- how a typed value becomes the bytes a cache entry holds.
//
// A value kept with Cache::keep<T> is written by Codec<T>::write and read back by Codec<T>::read (cb::io::ByteWriter
// and ByteReader, little-endian). Each codec states a `kSchema`: the version of the layout it writes. A cache entry
// records the schema it was written under, and an entry written under another schema is a miss, so changing a
// struct's layout means bumping its kSchema and nothing else.
//
// Codecs are provided for the arithmetic types, bool, double, std::string, Bytes, std::vector, std::optional,
// std::pair and std::map. A module's own record writes its codec next to the record:
//
//     template <> struct cb::base::cache::Codec<Verdict> {
//         static constexpr std::uint32_t kSchema = 3;
//         static void write(io::ByteWriter& w, const Verdict& v) { w.u8(v.code); Codec<std::string>::write(w, v.note); }
//         static Verdict read(io::ByteReader& r) { Verdict v; v.code = r.u8(); v.note = Codec<std::string>::read(r); return v; }
//     };
//
// A reader that finds the data short or damaged throws cb::TruncatedError / cb::FormatError; the cache turns that
// into a miss.
#pragma once

#include <algorithm>
#include <bit>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/io.hpp"

namespace cb::base::cache {

template <class T, class Enable = void>
struct Codec;   // specialise for each cached type

namespace detail {
// The schema of a container codec: the element schema mixed with a tag for the container's own layout.
constexpr std::uint32_t mix(std::uint32_t tag, std::uint32_t inner) { return (tag * 0x9E3779B1u) ^ (inner + 0x7F4A7C15u); }

// Counts come from the data: refuse one that cannot possibly fit in what is left.
inline std::size_t checked_count(io::ByteReader& r, std::uint64_t count, std::size_t min_item_bytes) {
    if (count > r.remaining() / std::max<std::size_t>(min_item_bytes, 1))
        throw FormatError("cache", r.pos(), "count larger than the data");
    return static_cast<std::size_t>(count);
}
}  // namespace detail

template <class T>
struct Codec<T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>>> {
    static constexpr std::uint32_t kSchema = 0x100 + sizeof(T) * 2 + (std::is_signed_v<T> ? 1 : 0);
    static void write(io::ByteWriter& w, T v) {
        if constexpr (sizeof(T) == 1) w.u8(static_cast<std::uint8_t>(v));
        else if constexpr (sizeof(T) == 2) w.u16(static_cast<std::uint16_t>(v));
        else if constexpr (sizeof(T) == 4) w.u32(static_cast<std::uint32_t>(v));
        else w.u64(static_cast<std::uint64_t>(v));
    }
    static T read(io::ByteReader& r) { return r.read_le<T>(); }
};

template <>
struct Codec<bool> {
    static constexpr std::uint32_t kSchema = 0x200;
    static void write(io::ByteWriter& w, bool v) { w.u8(v ? 1 : 0); }
    static bool read(io::ByteReader& r) { return r.u8() != 0; }
};

template <>
struct Codec<double> {
    static constexpr std::uint32_t kSchema = 0x201;
    static void write(io::ByteWriter& w, double v) { w.u64(std::bit_cast<std::uint64_t>(v)); }
    static double read(io::ByteReader& r) { return std::bit_cast<double>(r.u64()); }
};

template <>
struct Codec<std::string> {
    static constexpr std::uint32_t kSchema = 0x202;
    static void write(io::ByteWriter& w, const std::string& s) {
        w.u32(static_cast<std::uint32_t>(s.size()));
        w.bytes(as_bytes(s));
    }
    static std::string read(io::ByteReader& r) {
        const std::uint32_t n = r.u32();
        return std::string(as_text(r.bytes(n)));
    }
};

template <>
struct Codec<Bytes> {
    static constexpr std::uint32_t kSchema = 0x203;
    static void write(io::ByteWriter& w, const Bytes& b) {
        w.u32(static_cast<std::uint32_t>(b.size()));
        w.bytes(b);
    }
    static Bytes read(io::ByteReader& r) {
        const std::uint32_t n = r.u32();
        return r.bytes_copy(n);
    }
};

template <class T>
struct Codec<std::vector<T>, std::enable_if_t<!std::is_same_v<T, std::uint8_t>>> {
    static constexpr std::uint32_t kSchema = detail::mix(1, Codec<T>::kSchema);
    static void write(io::ByteWriter& w, const std::vector<T>& v) {
        w.u32(static_cast<std::uint32_t>(v.size()));
        for (const T& x : v) Codec<T>::write(w, x);
    }
    static std::vector<T> read(io::ByteReader& r) {
        const std::size_t n = detail::checked_count(r, r.u32(), 1);
        std::vector<T> out;
        out.reserve(n);
        for (std::size_t i = 0; i < n; ++i) out.push_back(Codec<T>::read(r));
        return out;
    }
};

template <class T>
struct Codec<std::optional<T>> {
    static constexpr std::uint32_t kSchema = detail::mix(2, Codec<T>::kSchema);
    static void write(io::ByteWriter& w, const std::optional<T>& v) {
        w.u8(v ? 1 : 0);
        if (v) Codec<T>::write(w, *v);
    }
    static std::optional<T> read(io::ByteReader& r) {
        if (r.u8() == 0) return std::nullopt;
        return Codec<T>::read(r);
    }
};

template <class A, class B>
struct Codec<std::pair<A, B>> {
    static constexpr std::uint32_t kSchema = detail::mix(3, detail::mix(Codec<A>::kSchema, Codec<B>::kSchema));
    static void write(io::ByteWriter& w, const std::pair<A, B>& p) {
        Codec<A>::write(w, p.first);
        Codec<B>::write(w, p.second);
    }
    static std::pair<A, B> read(io::ByteReader& r) {
        A a = Codec<A>::read(r);
        B b = Codec<B>::read(r);
        return {std::move(a), std::move(b)};
    }
};

template <class K, class V>
struct Codec<std::map<K, V>> {
    static constexpr std::uint32_t kSchema = detail::mix(4, detail::mix(Codec<K>::kSchema, Codec<V>::kSchema));
    static void write(io::ByteWriter& w, const std::map<K, V>& m) {
        w.u32(static_cast<std::uint32_t>(m.size()));
        for (const auto& [k, v] : m) {
            Codec<K>::write(w, k);
            Codec<V>::write(w, v);
        }
    }
    static std::map<K, V> read(io::ByteReader& r) {
        const std::size_t n = detail::checked_count(r, r.u32(), 2);
        std::map<K, V> out;
        for (std::size_t i = 0; i < n; ++i) {
            K k = Codec<K>::read(r);
            V v = Codec<V>::read(r);
            out.emplace(std::move(k), std::move(v));
        }
        return out;
    }
};

}  // namespace cb::base::cache
