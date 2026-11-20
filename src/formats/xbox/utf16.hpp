// formats/xbox/utf16.hpp -- UTF-16 big-endian text, as the Xbox 360's databases and UI files keep it, to and from UTF-8.
//
// Decoding leaves an unpaired surrogate as U+FFFD and ignores a trailing odd byte (cb::text::from_utf16le's rule). Encoding is the exact
// inverse for well-formed text; `utf16_units` is how many 16-bit units the text takes, which is what the formats count (not code points).
#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "core/bytes.hpp"
#include "core/text.hpp"

namespace cb::formats::xbox {

inline std::string from_utf16be(ByteView raw) {
    Bytes le(raw.begin(), raw.end());
    for (std::size_t i = 0; i + 1 < le.size(); i += 2) std::swap(le[i], le[i + 1]);
    return text::from_utf16le(le);
}

inline Bytes to_utf16be(std::string_view utf8) {
    Bytes out = text::to_utf16le(utf8);
    for (std::size_t i = 0; i + 1 < out.size(); i += 2) std::swap(out[i], out[i + 1]);
    return out;
}

inline std::size_t utf16_units(std::string_view utf8) { return text::to_utf16le(utf8).size() / 2; }

}  // namespace cb::formats::xbox
