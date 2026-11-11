// cb/core/text.hpp -- text helpers. Text is a std::string holding UTF-8; parameters are std::string_view.
//
// CodeBreaker's text is overwhelmingly ASCII (instructions, C, identifiers, paths), so the algorithms here work
// on bytes and never index "characters". Case conversion is ASCII-only on purpose. The only two functions that
// know about code points are display_width and utf8_prefix (terminal columns, truncating a display string).
// Text that came out of a binary is decoded once, at the reader, with from_latin1 / from_cp437 / from_cp1252 /
// from_utf16le, and re-encoded by the matching to_* only where bytes are written back to a binary.
// See docs/CPP_DESIGN.md 3.2.
#pragma once

#include <charconv>
#include <cstdint>
#include <limits>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::text {

// ---- case (ASCII only) --------------------------------------------------------------------------------
std::string lower(std::string_view s);
std::string upper(std::string_view s);
bool iequals(std::string_view a, std::string_view b);     // ASCII case-insensitive equality
bool istarts_with(std::string_view s, std::string_view prefix);
bool iends_with(std::string_view s, std::string_view suffix);

// Strict-weak order and hash for case-insensitive (Windows-style) names; `IMap<V>` is the map to use.
struct ILess {
    bool operator()(std::string_view a, std::string_view b) const;
};
struct IHash {
    std::size_t operator()(std::string_view s) const;
};
struct IEqual {
    bool operator()(std::string_view a, std::string_view b) const { return iequals(a, b); }
};
template <class V>
using IMap = std::map<std::string, V, ILess>;

// ---- trimming and splitting ---------------------------------------------------------------------------
// Whitespace here is the ASCII set: space, \t \n \v \f \r.
bool is_space(char c);
std::string_view ltrim(std::string_view s);
std::string_view rtrim(std::string_view s);
std::string_view trim(std::string_view s);
std::string_view trim(std::string_view s, std::string_view chars);   // strip any of `chars` from both ends

// Split at every separator (like Python's str.split(sep): empty pieces are kept, "" gives one empty piece).
// The pieces are views into `s`; keep `s` alive while using them.
std::vector<std::string_view> split(std::string_view s, char sep);
std::vector<std::string_view> split(std::string_view s, std::string_view sep);
// At most `max_splits` splits; the rest of the text is the last piece.
std::vector<std::string_view> split_n(std::string_view s, char sep, std::size_t max_splits);
// Split at runs of whitespace; no empty pieces (like Python's str.split()).
std::vector<std::string_view> split_ws(std::string_view s);
// Split at `sep` and trim each piece; empty pieces are kept.
std::vector<std::string_view> split_trim(std::string_view s, char sep);
// The lines of a text, splitting on \n and \r\n. A final line without a terminator counts; a trailing
// terminator does not start an extra empty line.
std::vector<std::string_view> lines(std::string_view s);

// Join any range of string-likes with a separator.
template <class Range>
std::string join(const Range& parts, std::string_view sep) {
    std::string out;
    bool first = true;
    for (const auto& p : parts) {
        if (!first) out.append(sep);
        first = false;
        out.append(std::string_view(p));
    }
    return out;
}

// Replace every non-overlapping occurrence of `from` (non-empty) with `to`.
std::string replace_all(std::string_view s, std::string_view from, std::string_view to);
// Replace the first occurrence only.
std::string replace_first(std::string_view s, std::string_view from, std::string_view to);

// ---- identifiers --------------------------------------------------------------------------------------
// Every byte that is not [A-Za-z0-9_] becomes '_' (the old re.sub(r"\W", "_", s)).
std::string sanitize_identifier(std::string_view s);
// [A-Za-z_][A-Za-z0-9_]* (the old re.fullmatch(r"[A-Za-z_]\w*", s)).
bool is_identifier(std::string_view s);

// ---- numbers ------------------------------------------------------------------------------------------
// Text to integer: the whole text must be the number (no whitespace, no trailing junk), else nullopt. A
// leading '-' is accepted for signed types. base 16 accepts an optional "0x"/"0X" prefix; base 0 means "decimal,
// or hexadecimal if it starts with 0x". Overflow is nullopt.
template <class T>
std::optional<T> parse_int(std::string_view s, int base = 10) {
    bool negative = false;
    std::string_view body = s;
    if (!body.empty() && (body.front() == '-' || body.front() == '+')) {
        negative = body.front() == '-';
        body.remove_prefix(1);
    }
    if (base == 0 || base == 16) {
        if (body.size() > 2 && body[0] == '0' && (body[1] == 'x' || body[1] == 'X')) {
            body.remove_prefix(2);
            base = 16;
        } else if (base == 0) {
            base = 10;
        }
    }
    if (body.empty() || body.front() == '-' || body.front() == '+') return std::nullopt;
    if (negative && !std::is_signed_v<T>) return std::nullopt;
    // Parse the magnitude into the widest unsigned type, then range-check against T.
    unsigned long long mag = 0;
    auto [end, ec] = std::from_chars(body.data(), body.data() + body.size(), mag, base);
    if (ec != std::errc() || end != body.data() + body.size()) return std::nullopt;
    if constexpr (std::is_signed_v<T>) {
        constexpr unsigned long long kMax = static_cast<unsigned long long>(std::numeric_limits<T>::max());
        if (negative) {
            if (mag > kMax + 1) return std::nullopt;
            return static_cast<T>(0 - mag);   // two's complement wrap lands on the right negative value
        }
        if (mag > kMax) return std::nullopt;
        return static_cast<T>(mag);
    } else {
        if (mag > static_cast<unsigned long long>(std::numeric_limits<T>::max())) return std::nullopt;
        return static_cast<T>(mag);
    }
}

// A double as the shortest text that reads back as the same double (std::to_chars); never %g.
std::string format_double(double v);

// ---- encodings ----------------------------------------------------------------------------------------
// Decoders: bytes of a legacy code page or UTF-16LE to UTF-8. Every byte decodes (ISO-8859-1 is the identity on
// code points; CP437 and CP1252 have full tables; CP1252's five undefined bytes become the C1 control of the
// same value). from_utf16le leaves an unpaired surrogate as U+FFFD and ignores a trailing odd byte.
std::string from_latin1(ByteView b);
std::string from_cp437(ByteView b);
std::string from_cp1252(ByteView b);
std::string from_utf16le(ByteView b);

// Encoders: UTF-8 to the code page's bytes. A character the code page cannot hold becomes '?'.
Bytes to_latin1(std::string_view s);
Bytes to_cp437(std::string_view s);
Bytes to_cp1252(std::string_view s);
Bytes to_utf16le(std::string_view s);

// Is the whole text well-formed UTF-8 (no overlongs, surrogates or code points past U+10FFFF)?
bool is_valid_utf8(std::string_view s);
// Make a text safe to treat as UTF-8: bytes that are not valid are replaced by U+FFFD.
std::string sanitize_utf8(std::string_view s);
// Append a code point as UTF-8.
void append_utf8(std::string& out, char32_t cp);

// ---- display ------------------------------------------------------------------------------------------
// Terminal columns the text occupies: combining marks 0, East Asian wide and emoji 2, the rest 1.
std::size_t display_width(std::string_view s);
// The longest prefix whose display width is at most `columns`, cut on a code-point boundary.
std::string_view utf8_prefix(std::string_view s, std::size_t columns);

}  // namespace cb::text
