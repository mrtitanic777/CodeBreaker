// cb/core/bytes.hpp -- the byte vocabulary: owning buffer, read-only view, writable span, real-mode far pointer.
//
// Binary data is never a std::string and never a vector<char>. A function that reads takes a ByteView; one that
// edits in place takes a ByteSpan; one that returns a new buffer returns Bytes. See docs/CPP_DESIGN.md 3.3.
#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb {

using Bytes = std::vector<std::uint8_t>;
using ByteView = std::span<const std::uint8_t>;
using ByteSpan = std::span<std::uint8_t>;

// A real-mode (segment:offset) far pointer. `linear()` is the 20-bit-plus address seg*16+off.
struct Far16 {
    std::uint16_t seg = 0;
    std::uint16_t off = 0;
    constexpr std::uint32_t linear() const { return static_cast<std::uint32_t>(seg) * 16u + off; }
    friend constexpr bool operator==(const Far16&, const Far16&) = default;
};

// The bytes of a text (no terminator).
inline ByteView as_bytes(std::string_view s) {
    return {reinterpret_cast<const std::uint8_t*>(s.data()), s.size()};
}

// The bytes of a view as text. Only for text that was bytes on disk (a name, a log), never for decoding:
// decode with cb::text::from_latin1 and friends.
inline std::string_view as_text(ByteView b) {
    return {reinterpret_cast<const char*>(b.data()), b.size()};
}

inline Bytes to_bytes(std::string_view s) {
    ByteView v = as_bytes(s);
    return Bytes(v.begin(), v.end());
}

// Append a view to a buffer (the replacement for `bytes + bytes`).
inline void append(Bytes& out, ByteView extra) { out.insert(out.end(), extra.begin(), extra.end()); }

inline bool equal_bytes(ByteView a, ByteView b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}

}  // namespace cb
