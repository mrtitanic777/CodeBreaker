// cb/formats/read.hpp -- the few byte-reading helpers the executable-format readers share.
//
// Binaries are untrusted input. A *strict* read (`get`) throws TruncatedError when the value is not inside the
// data; a *tolerant* read (`try_get`) answers nullopt, and `get_or_zero` answers 0 -- the inspector views of a
// damaged image use those to give a shorter answer instead of failing. Offsets are 64-bit and the arithmetic on
// them cannot overflow (they come from 32-bit fields).
#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::formats {

// The little-endian value of type T at `off`, or nullopt when it does not lie inside `d`.
template <class T>
std::optional<T> try_get(ByteView d, std::uint64_t off) {
    if (off > d.size() || d.size() - off < sizeof(T)) return std::nullopt;
    T v = 0;
    for (std::size_t i = sizeof(T); i-- > 0;) v = static_cast<T>((v << 8) | d[off + i]);
    return v;
}

// The same, strict: TruncatedError(reader, off, ...) when it does not lie inside `d`.
template <class T>
T get(ByteView d, std::uint64_t off, std::string_view reader) {
    if (auto v = try_get<T>(d, off)) return *v;
    throw TruncatedError(reader, off, sizeof(T), off < d.size() ? d.size() - off : 0);
}

// The same, tolerant in the way the inspector views are: 0 outside the data.
template <class T>
T get_or_zero(ByteView d, std::uint64_t off) {
    return try_get<T>(d, off).value_or(T{0});
}

// `len` bytes at `off`, strict.
inline ByteView slice(ByteView d, std::uint64_t off, std::uint64_t len, std::string_view reader) {
    if (off > d.size() || d.size() - off < len)
        throw TruncatedError(reader, off, len, off < d.size() ? d.size() - off : 0);
    return d.subspan(off, len);
}

// The bytes from `off`, at most `len` of them (the clamp is explicit: a table that runs past the end of the file
// yields what is there).
inline ByteView clamped(ByteView d, std::uint64_t off, std::uint64_t len) {
    if (off >= d.size()) return {};
    return d.subspan(off, std::min<std::uint64_t>(len, d.size() - off));
}

// Position of the first byte `b` at or after `from` (and before `to`), if any.
inline std::optional<std::uint64_t> find_byte(ByteView d, std::uint8_t b, std::uint64_t from = 0,
                                              std::uint64_t to = UINT64_MAX) {
    const std::uint64_t end = std::min<std::uint64_t>(to, d.size());
    for (std::uint64_t i = from; i < end; ++i)
        if (d[i] == b) return i;
    return std::nullopt;
}

// Position of the last occurrence of `needle` in d[from, to), if any.
inline std::optional<std::uint64_t> rfind(ByteView d, ByteView needle, std::uint64_t from = 0,
                                          std::uint64_t to = UINT64_MAX) {
    const std::uint64_t end = std::min<std::uint64_t>(to, d.size());
    if (needle.size() > end || end - needle.size() < from) return std::nullopt;
    for (std::uint64_t i = end - needle.size() + 1; i-- > from;)
        if (std::equal(needle.begin(), needle.end(), d.begin() + static_cast<std::ptrdiff_t>(i))) return i;
    return std::nullopt;
}

// Position of the first occurrence of `needle` at or after `from`, if any.
inline std::optional<std::uint64_t> find(ByteView d, ByteView needle, std::uint64_t from = 0) {
    if (needle.empty() || from > d.size() || d.size() - from < needle.size()) return std::nullopt;
    for (std::uint64_t i = from; i + needle.size() <= d.size(); ++i)
        if (std::equal(needle.begin(), needle.end(), d.begin() + static_cast<std::ptrdiff_t>(i))) return i;
    return std::nullopt;
}

inline std::optional<std::uint64_t> find(ByteView d, std::string_view needle, std::uint64_t from = 0) {
    return find(d, as_bytes(needle), from);
}

// The zero-terminated string at `off` (latin1 -> UTF-8), at most `limit` bytes; tolerant: an unterminated string
// is what is there, and an offset outside the data gives "".
inline std::string cstring(ByteView d, std::uint64_t off, std::uint64_t limit = UINT64_MAX) {
    ByteView rest = clamped(d, off, limit);
    auto z = find_byte(rest, 0);
    return text::from_latin1(z ? rest.first(*z) : rest);
}

// The same, strict: FormatError when the terminator is missing.
inline std::string strict_cstring(ByteView d, std::uint64_t off, std::string_view reader) {
    auto z = off < d.size() ? find_byte(d, 0, off) : std::nullopt;
    if (!z) throw FormatError(reader, off, "string is not terminated");
    return text::from_latin1(d.subspan(off, *z - off));
}

inline bool starts_with(ByteView d, std::uint64_t off, std::string_view lit) {
    if (off > d.size() || d.size() - off < lit.size()) return false;
    for (std::size_t i = 0; i < lit.size(); ++i)
        if (d[off + i] != static_cast<std::uint8_t>(lit[i])) return false;  // bytes, not (signed) chars
    return true;
}

}  // namespace cb::formats
