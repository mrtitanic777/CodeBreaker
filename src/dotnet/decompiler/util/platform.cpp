#include "dotnet/decompiler/util/platform.hpp"

#include <cstdint>
#include <functional>

namespace cb::dotnet::decompiler::util {

namespace {

// The next code point of UTF-8 text from `i` (a malformed byte is its own value).
std::uint32_t next_code_point(std::string_view s, std::size_t& i) {
    const unsigned char c = static_cast<unsigned char>(s[i++]);
    int extra = c >= 0xF0 ? 3 : c >= 0xE0 ? 2 : c >= 0xC0 ? 1 : 0;
    if (extra == 0) return c;
    std::uint32_t cp = c & (0x3F >> extra);
    while (extra-- > 0 && i < s.size() && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80) cp = (cp << 6) | (static_cast<unsigned char>(s[i++]) & 0x3F);
    return cp;
}

std::uint32_t to_upper(std::uint32_t cp) {
    if (cp >= 'a' && cp <= 'z') return cp - 32;
    if (cp < 0x80) return cp;
    if ((cp >= 0xE0 && cp <= 0xFE && cp != 0xF7)) return cp - 32;   // Latin-1 letters (not the division sign)
    if (cp == 0xFF) return 0x178;
    if (cp >= 0x3B1 && cp <= 0x3C9 && cp != 0x3C2) return cp - 32;   // Greek
    if (cp == 0x3C2) return 0x3A3;
    if (cp >= 0x430 && cp <= 0x44F) return cp - 32;   // Cyrillic
    if (cp >= 0x450 && cp <= 0x45F) return cp - 80;
    return cp;
}

}  // namespace

int compare_ordinal_ignore_case(std::string_view a, std::string_view b) {
    std::size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        const std::uint32_t x = to_upper(next_code_point(a, i));
        const std::uint32_t y = to_upper(next_code_point(b, j));
        if (x != y) return x < y ? -1 : 1;
    }
    if (i < a.size()) return 1;
    if (j < b.size()) return -1;
    return 0;
}

std::size_t hash_ordinal_ignore_case(std::string_view s) {
    std::size_t h = 1469598103934665603ULL;
    std::size_t i = 0;
    while (i < s.size()) {
        h ^= to_upper(next_code_point(s, i));
        h *= 1099511628211ULL;
    }
    return h;
}

}  // namespace cb::dotnet::decompiler::util
