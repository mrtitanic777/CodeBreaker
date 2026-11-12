// cb/decompile/describe_internal.hpp -- helpers the describe_*.cpp files share (not an interface: nothing outside
// decompile/describe*.cpp includes it).
#pragma once

#include <algorithm>
#include <format>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/text.hpp"
#include "cb/decompile/describe.hpp"

namespace cb::decompile::describe::detail {

// "%x" of a number (a negative one with a minus sign, as the description files have always shown them)
inline std::string hx(Va v) {
    return v < 0 ? std::format("-{:x}", static_cast<unsigned long long>(-v)) : std::format("{:x}", static_cast<unsigned long long>(v));
}
// "%0<width>X"
inline std::string hex_upper(Va v, int width) { return std::format("{:0{}X}", static_cast<unsigned long long>(v), width); }

inline bool eq_any(std::string_view s, std::initializer_list<std::string_view> xs) {
    for (std::string_view x : xs)
        if (s == x) return true;
    return false;
}
inline bool contains(std::string_view s, std::string_view p) { return s.find(p) != std::string_view::npos; }

// 0x followed by lower-case hex digits (Capstone's way to write a number)
inline bool is_hex_lit(std::string_view s) {
    if (s.size() < 3 || s[0] != '0' || s[1] != 'x') return false;
    for (std::size_t i = 2; i < s.size(); ++i) {
        char c = s[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) return false;
    }
    return true;
}
inline bool is_dec_lit(std::string_view s) {
    if (s.empty()) return false;
    for (char c : s)
        if (c < '0' || c > '9') return false;
    return true;
}
// a number as Capstone writes it: hex with 0x or decimal
inline bool is_num_lit(std::string_view s) { return is_hex_lit(s) || is_dec_lit(s); }

// A number in Capstone's text (0x.. or decimal); the text must be a number.
inline std::optional<std::int64_t> number_of(std::string_view s) { return text::parse_int<std::int64_t>(s, 0); }
inline std::optional<std::int64_t> hex_of(std::string_view s) { return text::parse_int<std::int64_t>(s, 16); }

inline std::string_view before_comma(std::string_view s) { return s.substr(0, s.find(',')); }

inline std::vector<std::string> split_strip_commas(std::string_view s) {
    std::vector<std::string> out;
    for (std::string_view part : text::split_trim(s, ',')) out.emplace_back(part);
    return out;
}

inline std::int64_t word_at(ByteView b, std::int64_t at) { return b[static_cast<std::size_t>(at)] | (b[static_cast<std::size_t>(at) + 1] << 8); }

}  // namespace cb::decompile::describe::detail
