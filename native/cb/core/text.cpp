#include "cb/core/text.hpp"

#include <algorithm>
#include <array>
#include <charconv>

namespace cb::text {

namespace {
#include "cb/core/text_tables.inc"

constexpr char lower_char(char c) { return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c; }
constexpr char upper_char(char c) { return (c >= 'a' && c <= 'z') ? static_cast<char>(c - 32) : c; }
}  // namespace

// ---- case ---------------------------------------------------------------------------------------------

std::string lower(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = lower_char(c);
    return out;
}

std::string upper(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = upper_char(c);
    return out;
}

bool iequals(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (lower_char(a[i]) != lower_char(b[i])) return false;
    }
    return true;
}

bool istarts_with(std::string_view s, std::string_view prefix) {
    return s.size() >= prefix.size() && iequals(s.substr(0, prefix.size()), prefix);
}

bool iends_with(std::string_view s, std::string_view suffix) {
    return s.size() >= suffix.size() && iequals(s.substr(s.size() - suffix.size()), suffix);
}

bool ILess::operator()(std::string_view a, std::string_view b) const {
    const std::size_t n = std::min(a.size(), b.size());
    for (std::size_t i = 0; i < n; ++i) {
        const unsigned char x = static_cast<unsigned char>(lower_char(a[i]));
        const unsigned char y = static_cast<unsigned char>(lower_char(b[i]));
        if (x != y) return x < y;
    }
    return a.size() < b.size();
}

std::size_t IHash::operator()(std::string_view s) const {
    std::size_t h = 1469598103934665603ull;   // FNV-1a over the lowered bytes
    for (char c : s) {
        h ^= static_cast<unsigned char>(lower_char(c));
        h *= 1099511628211ull;
    }
    return h;
}

// ---- trimming and splitting ---------------------------------------------------------------------------

bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

std::string_view ltrim(std::string_view s) {
    std::size_t i = 0;
    while (i < s.size() && is_space(s[i])) ++i;
    return s.substr(i);
}

std::string_view rtrim(std::string_view s) {
    std::size_t n = s.size();
    while (n > 0 && is_space(s[n - 1])) --n;
    return s.substr(0, n);
}

std::string_view trim(std::string_view s) { return ltrim(rtrim(s)); }

std::string_view trim(std::string_view s, std::string_view chars) {
    const std::size_t a = s.find_first_not_of(chars);
    if (a == std::string_view::npos) return {};
    const std::size_t b = s.find_last_not_of(chars);
    return s.substr(a, b - a + 1);
}

std::vector<std::string_view> split(std::string_view s, char sep) {
    return split(s, std::string_view(&sep, 1));
}

std::vector<std::string_view> split(std::string_view s, std::string_view sep) {
    std::vector<std::string_view> out;
    if (sep.empty()) {
        out.push_back(s);
        return out;
    }
    std::size_t start = 0;
    while (true) {
        const std::size_t at = s.find(sep, start);
        if (at == std::string_view::npos) break;
        out.push_back(s.substr(start, at - start));
        start = at + sep.size();
    }
    out.push_back(s.substr(start));
    return out;
}

std::vector<std::string_view> split_n(std::string_view s, char sep, std::size_t max_splits) {
    std::vector<std::string_view> out;
    std::size_t start = 0;
    while (out.size() < max_splits) {
        const std::size_t at = s.find(sep, start);
        if (at == std::string_view::npos) break;
        out.push_back(s.substr(start, at - start));
        start = at + 1;
    }
    out.push_back(s.substr(start));
    return out;
}

std::vector<std::string_view> split_ws(std::string_view s) {
    std::vector<std::string_view> out;
    std::size_t i = 0;
    while (i < s.size()) {
        while (i < s.size() && is_space(s[i])) ++i;
        const std::size_t start = i;
        while (i < s.size() && !is_space(s[i])) ++i;
        if (i > start) out.push_back(s.substr(start, i - start));
    }
    return out;
}

std::vector<std::string_view> split_trim(std::string_view s, char sep) {
    std::vector<std::string_view> out = split(s, sep);
    for (auto& piece : out) piece = trim(piece);
    return out;
}

std::vector<std::string_view> lines(std::string_view s) {
    std::vector<std::string_view> out;
    std::size_t start = 0;
    while (start < s.size()) {
        const std::size_t nl = s.find('\n', start);
        if (nl == std::string_view::npos) {
            out.push_back(s.substr(start));
            break;
        }
        std::size_t end = nl;
        if (end > start && s[end - 1] == '\r') --end;
        out.push_back(s.substr(start, end - start));
        start = nl + 1;
    }
    return out;
}

std::string replace_all(std::string_view s, std::string_view from, std::string_view to) {
    if (from.empty()) return std::string(s);
    std::string out;
    std::size_t start = 0;
    while (true) {
        const std::size_t at = s.find(from, start);
        if (at == std::string_view::npos) break;
        out.append(s.substr(start, at - start));
        out.append(to);
        start = at + from.size();
    }
    out.append(s.substr(start));
    return out;
}

std::string replace_first(std::string_view s, std::string_view from, std::string_view to) {
    const std::size_t at = from.empty() ? std::string_view::npos : s.find(from);
    if (at == std::string_view::npos) return std::string(s);
    std::string out(s.substr(0, at));
    out.append(to);
    out.append(s.substr(at + from.size()));
    return out;
}

// ---- identifiers --------------------------------------------------------------------------------------

namespace {
constexpr bool is_alpha_(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
constexpr bool is_digit_(char c) { return c >= '0' && c <= '9'; }
}  // namespace

std::string sanitize_identifier(std::string_view s) {
    std::string out(s);
    for (char& c : out) {
        if (!(is_alpha_(c) || is_digit_(c) || c == '_')) c = '_';
    }
    return out;
}

bool is_identifier(std::string_view s) {
    if (s.empty() || !(is_alpha_(s[0]) || s[0] == '_')) return false;
    return std::all_of(s.begin() + 1, s.end(), [](char c) { return is_alpha_(c) || is_digit_(c) || c == '_'; });
}

// ---- numbers ------------------------------------------------------------------------------------------

std::string format_double(double v) {
    std::array<char, 64> buf{};
    auto [end, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), v);
    if (ec != std::errc()) return "nan";
    return std::string(buf.data(), end);
}

// ---- UTF-8 --------------------------------------------------------------------------------------------

void append_utf8(std::string& out, char32_t cp) {
    if (cp < 0x80) {
        out.push_back(static_cast<char>(cp));
    } else if (cp < 0x800) {
        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp < 0x10000) {
        out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
}

namespace {

// Decode one code point at s[i]; advances i. Invalid input yields U+FFFD and consumes one byte.
// `valid` (when given) is cleared if the sequence was not well-formed.
char32_t decode_utf8(std::string_view s, std::size_t& i, bool* valid = nullptr) {
    const auto b0 = static_cast<unsigned char>(s[i]);
    auto bad = [&]() {
        if (valid != nullptr) *valid = false;
        ++i;
        return char32_t{0xFFFD};
    };
    if (b0 < 0x80) {
        ++i;
        return b0;
    }
    std::size_t need = 0;
    char32_t cp = 0;
    char32_t min = 0;
    if (b0 >= 0xC2 && b0 <= 0xDF) { need = 1; cp = b0 & 0x1Fu; min = 0x80; }
    else if (b0 >= 0xE0 && b0 <= 0xEF) { need = 2; cp = b0 & 0x0Fu; min = 0x800; }
    else if (b0 >= 0xF0 && b0 <= 0xF4) { need = 3; cp = b0 & 0x07u; min = 0x10000; }
    else return bad();
    if (s.size() - i <= need) return bad();   // not enough bytes left for the continuation bytes
    for (std::size_t k = 1; k <= need; ++k) {
        const auto b = static_cast<unsigned char>(s[i + k]);
        if ((b & 0xC0) != 0x80) return bad();
        cp = (cp << 6) | (b & 0x3Fu);
    }
    if (cp < min || cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) return bad();
    i += need + 1;
    return cp;
}

}  // namespace

bool is_valid_utf8(std::string_view s) {
    bool valid = true;
    for (std::size_t i = 0; i < s.size() && valid;) decode_utf8(s, i, &valid);
    return valid;
}

std::string sanitize_utf8(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size();) append_utf8(out, decode_utf8(s, i));
    return out;
}

// ---- encodings ----------------------------------------------------------------------------------------

std::string from_latin1(ByteView b) {
    std::string out;
    out.reserve(b.size());
    for (std::uint8_t c : b) append_utf8(out, c);
    return out;
}

std::string from_cp437(ByteView b) {
    std::string out;
    out.reserve(b.size());
    for (std::uint8_t c : b) append_utf8(out, c < 0x80 ? char32_t{c} : char32_t{kCp437High[c - 0x80]});
    return out;
}

std::string from_cp1252(ByteView b) {
    std::string out;
    out.reserve(b.size());
    for (std::uint8_t c : b) {
        char32_t cp = c;
        if (c >= 0x80 && c < 0xA0) cp = kCp1252Mid[c - 0x80];
        append_utf8(out, cp);
    }
    return out;
}

std::string from_utf16le(ByteView b) {
    std::string out;
    const std::size_t units = b.size() / 2;
    auto unit = [&](std::size_t i) { return static_cast<char16_t>(b[2 * i] | (b[2 * i + 1] << 8)); };
    for (std::size_t i = 0; i < units; ++i) {
        const char16_t u = unit(i);
        if (u >= 0xD800 && u <= 0xDBFF && i + 1 < units && unit(i + 1) >= 0xDC00 && unit(i + 1) <= 0xDFFF) {
            const char32_t cp = 0x10000 + ((char32_t{u} - 0xD800) << 10) + (char32_t{unit(i + 1)} - 0xDC00);
            append_utf8(out, cp);
            ++i;
        } else if (u >= 0xD800 && u <= 0xDFFF) {
            append_utf8(out, 0xFFFD);
        } else {
            append_utf8(out, u);
        }
    }
    return out;
}

namespace {

// Encode UTF-8 through `lookup`, which maps a code point to a byte or -1 if the code page lacks it.
template <class Lookup>
Bytes encode_with(std::string_view s, Lookup lookup) {
    Bytes out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size();) {
        const int b = lookup(decode_utf8(s, i));
        out.push_back(b < 0 ? std::uint8_t{'?'} : static_cast<std::uint8_t>(b));
    }
    return out;
}

}  // namespace

Bytes to_latin1(std::string_view s) {
    return encode_with(s, [](char32_t cp) { return cp < 0x100 ? static_cast<int>(cp) : -1; });
}

Bytes to_cp437(std::string_view s) {
    return encode_with(s, [](char32_t cp) {
        if (cp < 0x80) return static_cast<int>(cp);
        for (std::size_t i = 0; i < std::size(kCp437High); ++i) {
            if (kCp437High[i] == cp) return static_cast<int>(0x80 + i);
        }
        return -1;
    });
}

Bytes to_cp1252(std::string_view s) {
    return encode_with(s, [](char32_t cp) {
        if (cp < 0x80 || (cp >= 0xA0 && cp < 0x100)) return static_cast<int>(cp);
        for (std::size_t i = 0; i < std::size(kCp1252Mid); ++i) {
            if (kCp1252Mid[i] == cp) return static_cast<int>(0x80 + i);
        }
        return -1;
    });
}

Bytes to_utf16le(std::string_view s) {
    Bytes out;
    auto put = [&](char16_t u) {
        out.push_back(static_cast<std::uint8_t>(u & 0xFF));
        out.push_back(static_cast<std::uint8_t>(u >> 8));
    };
    for (std::size_t i = 0; i < s.size();) {
        const char32_t cp = decode_utf8(s, i);
        if (cp >= 0x10000) {
            const char32_t v = cp - 0x10000;
            put(static_cast<char16_t>(0xD800 + (v >> 10)));
            put(static_cast<char16_t>(0xDC00 + (v & 0x3FF)));
        } else {
            put(static_cast<char16_t>(cp));
        }
    }
    return out;
}

// ---- display ------------------------------------------------------------------------------------------

namespace {

int column_width(char32_t cp) {
    if (cp == 0 || cp < 0x20 || (cp >= 0x7F && cp < 0xA0)) return 0;
    // Combining marks and zero-width characters.
    if ((cp >= 0x0300 && cp <= 0x036F) || (cp >= 0x200B && cp <= 0x200F) || (cp >= 0xFE00 && cp <= 0xFE0F) ||
        (cp >= 0x20D0 && cp <= 0x20FF) || cp == 0xFEFF)
        return 0;
    // East Asian wide / fullwidth and emoji planes.
    if ((cp >= 0x1100 && cp <= 0x115F) || (cp >= 0x2E80 && cp <= 0xA4CF) || (cp >= 0xAC00 && cp <= 0xD7A3) ||
        (cp >= 0xF900 && cp <= 0xFAFF) || (cp >= 0xFE30 && cp <= 0xFE6F) || (cp >= 0xFF00 && cp <= 0xFF60) ||
        (cp >= 0xFFE0 && cp <= 0xFFE6) || (cp >= 0x1F300 && cp <= 0x1F64F) || (cp >= 0x1F900 && cp <= 0x1F9FF) ||
        (cp >= 0x20000 && cp <= 0x3FFFD))
        return 2;
    return 1;
}

}  // namespace

std::size_t display_width(std::string_view s) {
    std::size_t width = 0;
    for (std::size_t i = 0; i < s.size();) width += static_cast<std::size_t>(column_width(decode_utf8(s, i)));
    return width;
}

std::string_view utf8_prefix(std::string_view s, std::size_t columns) {
    std::size_t width = 0;
    std::size_t i = 0;
    while (i < s.size()) {
        std::size_t next = i;
        const int w = column_width(decode_utf8(s, next));
        if (width + static_cast<std::size_t>(w) > columns) break;
        width += static_cast<std::size_t>(w);
        i = next;
    }
    return s.substr(0, i);
}

}  // namespace cb::text
