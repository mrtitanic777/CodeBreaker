// cb/build/x86asm/format_util.hpp -- the small text and byte helpers the object formats share: NASM's nasmlib string
// routines they use (nasm_isspace, nasm_skip_spaces, nasm_skip_word, nasm_stricmp, nasm_strnicmp), the little-endian
// writers (fwriteint16_t, fwriteint32_t) and the CRC the COFF COMDAT checksum uses (crc32b).
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::build::x86asm::fmt {

// The C locale's isspace on a character: space, \t \n \v \f \r.
constexpr bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

// The C locale's tolower / toupper.
constexpr char lower(char c) { return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c; }
constexpr char upper(char c) { return (c >= 'a' && c <= 'z') ? static_cast<char>(c - 32) : c; }

// `text` without its leading spaces (nasm_skip_spaces).
constexpr std::string_view skip_spaces(std::string_view text) {
    while (!text.empty() && is_space(text.front())) text.remove_prefix(1);
    return text;
}

// The number of leading non-space characters (nasm_skip_word, as a length).
constexpr std::size_t word_length(std::string_view text) {
    std::size_t n = 0;
    while (n < text.size() && !is_space(text[n])) ++n;
    return n;
}

// The first word of `text`; `text` becomes what follows it, with the spaces after it skipped. (The three-line idiom
// `while (*q && !isspace(*q)) q++; if (isspace(*q)) { *q++ = 0; while (*q && isspace(*q)) q++; }` of the directives.)
constexpr std::string_view take_word(std::string_view& text) {
    const std::size_t n = word_length(text);
    const std::string_view word = text.substr(0, n);
    text.remove_prefix(n);
    if (!text.empty()) {
        text.remove_prefix(1);
        text = skip_spaces(text);
    }
    return word;
}

// Case-blind equality of whole strings (nasm_stricmp(a, b) == 0).
constexpr bool equals_nocase(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (lower(a[i]) != lower(b[i])) return false;
    return true;
}

// Does `text` start with `prefix`, case-blind (nasm_strnicmp(text, prefix, strlen(prefix)) == 0)?
constexpr bool starts_nocase(std::string_view text, std::string_view prefix) {
    return text.size() >= prefix.size() && equals_nocase(text.substr(0, prefix.size()), prefix);
}

// Appenders: the little-endian integers of the object files.
inline void put8(Bytes& out, unsigned v) { out.push_back(static_cast<std::uint8_t>(v)); }
inline void put16(Bytes& out, unsigned v) {
    out.push_back(static_cast<std::uint8_t>(v));
    out.push_back(static_cast<std::uint8_t>(v >> 8));
}
inline void put32(Bytes& out, std::uint32_t v) {
    out.push_back(static_cast<std::uint8_t>(v));
    out.push_back(static_cast<std::uint8_t>(v >> 8));
    out.push_back(static_cast<std::uint8_t>(v >> 16));
    out.push_back(static_cast<std::uint8_t>(v >> 24));
}

// crc32b (nasmlib/crc32b.c): the reflected CRC-32 table step with no initial or final inversion; calls chain.
inline std::uint32_t crc32b(std::uint32_t crc, const std::uint8_t* data, std::size_t len) {
    static constexpr auto table = [] {
        std::array<std::uint32_t, 256> t{};
        for (std::uint32_t i = 0; i < 256; ++i) {
            std::uint32_t c = i;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? (c >> 1) ^ 0xEDB88320u : c >> 1;
            t[i] = c;
        }
        return t;
    }();
    while (len--) crc = (crc >> 8) ^ table[(crc ^ *data++) & 0xff];
    return crc;
}

}  // namespace cb::build::x86asm::fmt
